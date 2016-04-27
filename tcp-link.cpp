#include <pthread.h>
#include <list>
#include "tcp-link.h"
#include "clientlog-api.h"
#include "ms-epoll.h"

pthread_mutex_t MsTcpLink::m_link_lock;
std::map<HTCPLINK, CRefCountPtr<MsTcpLink> > MsTcpLink::m_maplink;
bool MsTcpLink::mb_send_flag;

//from MSLogServer
bool get_ip_by_name(const std::string& name, unsigned int& ipv4)
{
    if (name.empty())
    {
        return false;
    }

    unsigned long ip = ::inet_addr(name.c_str());
    if (ip != INADDR_NONE)
    {
        ipv4 = ntohl(ip);
        return true;
    }

    hostent* host = ::gethostbyname(name.c_str());
    if (host != NULL &&
        host->h_addrtype == AF_INET &&
        host->h_addr_list[0] != NULL)
    {
        ip = *(unsigned long*)host->h_addr_list[0];
        ipv4 = ntohl(ip);
        return true;
    }
    else
    {
        return false;
    }

}

MsTcpLink::MsTcpLink()
{
    pthread_mutex_init( &mo_send_buff_list_lock, NULL );
    pthread_mutex_init( &mo_last_alive_time_lock, NULL );
    mi_socket = -1;
    mh_link = 0;
    mp_tcpcallback = NULL;
    memset( &m_local_address, 0, sizeof(m_local_address) );
    memset( &m_remote_address, 0, sizeof(m_remote_address) );
    time( &mt_last_alive_time );
    m_link_type = link_TypeNull;
    mb_need_check_alive = false;    
}

MsTcpLink::~MsTcpLink()
{
    pthread_mutex_destroy( &mo_send_buff_list_lock );
    pthread_mutex_destroy( &mo_last_alive_time_lock );
}

bool MsTcpLink::find_link( HTCPLINK h_link, CRefCountPtr<MsTcpLink> &p_link )
{
    CGuardLock<pthread_mutex_t> guard( &m_link_lock );//守护锁
    std::map<HTCPLINK, CRefCountPtr<MsTcpLink> >::iterator it = m_maplink.find( h_link );
    if( it == m_maplink.end() ){
        MS_LOGER_ERROR( "find link failed. link=0x%04x", h_link );
        return false;
    }
    p_link = it->second;
    return true;
}

bool MsTcpLink::del_link( HTCPLINK h_link )
{    
    CGuardLock<pthread_mutex_t> guard( &m_link_lock );
    std::map<HTCPLINK, CRefCountPtr<MsTcpLink> >::iterator it = m_maplink.find( h_link );
    if( it == m_maplink.end() ){
        MS_LOGER_ERROR( "find link failed. link=0x%04x", h_link );
        return false;
    }
    CRefCountPtr<MsTcpLink> p_reflink = it->second;
    CSingletonMsEpollHandler::Instance()->del_epoll_read( p_reflink->get_socket() );
    m_maplink.erase( it );
    return true;
}

void MsTcpLink::free_all_link_map()//清除map中的所有链接
{
    std::list<CRefCountPtr<MsTcpLink> > link_list;
    do{
        CGuardLock<pthread_mutex_t> guard( &m_link_lock );
        if( m_maplink.empty() ){
            return;
        }
        std::map<HTCPLINK, CRefCountPtr<MsTcpLink> >::iterator it = m_maplink.begin();
        for( ; it != m_maplink.end(); ++it ){
            CRefCountPtr<MsTcpLink> p_link = it->second;
            link_list.push_back( p_link );
        }
    }while( 0 );

    std::list<CRefCountPtr<MsTcpLink> >::iterator it_link = link_list.begin();
    for( ; it_link != link_list.end(); ++it_link ){
        CRefCountPtr<MsTcpLink> p_link = *it_link;
        p_link->link_destroy();
    }
}

void MsTcpLink::check_link_timeout()//检查链接超时
{
    //if no any data IO after accept link, close it when timeout.
    //check handle and memory leak.
    std::list<CRefCountPtr<MsTcpLink> > link_list;
    do{
        CGuardLock<pthread_mutex_t> guard( &m_link_lock );
        if( m_maplink.empty() ){
            return;
        }
        time_t st_cur_time = time( NULL );
        std::map<HTCPLINK, CRefCountPtr<MsTcpLink> >::iterator it = m_maplink.begin();
        for( ; it != m_maplink.end(); ++it ){
            CRefCountPtr<MsTcpLink> p_link = it->second;
            if( !p_link->is_alive( st_cur_time ) && (p_link->get_link_type() != link_TypeListen) ){
                link_list.push_back( p_link );
            }
        }

    }while( 0 );

    //如果链接超时，则清除链接
    std::list<CRefCountPtr<MsTcpLink> >::iterator it_link = link_list.begin();
    for( ; it_link != link_list.end(); ++it_link ){
        CRefCountPtr<MsTcpLink> p_link = *it_link;
        p_link->link_destroy();
    }
}

void MsTcpLink::do_send_buff()
{
    if( false == MsTcpLink::get_send_flag() ){
        return;
    }

    MS_LOGER_INFO( "do_send_buff" );

    MsTcpLink::set_send_flag( false );
    CGuardLock<pthread_mutex_t> guard( &m_link_lock );
    std::map<HTCPLINK, CRefCountPtr<MsTcpLink> >::iterator it_link = m_maplink.begin();
    for( ; it_link != m_maplink.end(); ++it_link ){
        CRefCountPtr<MsTcpLink> pcrf_link = it_link->second;

        //send data from databuf.
        CGuardLock<pthread_mutex_t> guard_buff( &pcrf_link->mo_send_buff_list_lock );
        std::list<send_buff_s>::iterator it = pcrf_link->m_send_buff_list.begin();
        for( ; it != pcrf_link->m_send_buff_list.end(); ){
            send_buff_s st_send_buff = *it;
            char *p_data = st_send_buff.p_data;
            unsigned int ui_size = st_send_buff.ui_size;
            if( (p_data == NULL) || (ui_size == 0) ){
                MS_LOGER_ERROR( "data buff is null." );
                if( p_data != NULL ){
                    delete []p_data;
                    p_data = NULL;
                }
                it = pcrf_link->m_send_buff_list.erase( it );
                continue;
            }

            int i_send_size = 0;
            while( (unsigned int)i_send_size < ui_size ){
                int i_send_tmp = pcrf_link->send_data( p_data + i_send_size, ui_size - i_send_size );
                if( i_send_tmp < 0 ){
                    int i_error = errno;
                    if( i_error != EAGAIN ){
                        //close??
                        MS_LOGER_ERROR( "send data failed. error=%d,%s", i_error, strerror( i_error ) );
                    } else {
                        MS_LOGER_ERROR( "send data failed. EAGAIN." );
                    }
                    break;
                }
                i_send_size += i_send_tmp;
                if( (i_send_tmp == 0) && (errno == EAGAIN) ){
                    break;
                }
                if( (unsigned int)i_send_size == ui_size ){
                    break;
                }
            }

            bool b_break = false;
            if( ((unsigned int)i_send_size < ui_size) && (i_send_size != 0) ){
                char *p_new = new char[ui_size - i_send_size];
                if( p_new == NULL ){
                    MS_LOGER_ERROR( "new failed." );
                    return;
                }
                memcpy( p_new, p_data + i_send_size, ui_size - i_send_size );
                send_buff_s st_send_tmp;
                st_send_tmp.p_data = p_new;
                st_send_tmp.ui_size = ui_size - i_send_size;
                pcrf_link->m_send_buff_list.push_front( st_send_tmp );
                b_break = true;
            }

            if( i_send_size != 0 ){
                delete []p_data;
                p_data = NULL;
                it = pcrf_link->m_send_buff_list.erase( it );
            }
            if( b_break ){
                break;
            }
        }

        if( !pcrf_link->m_send_buff_list.empty() ){
            MsTcpLink::set_send_flag( true );
        }
        time( &pcrf_link->mt_last_alive_time );
    }

    return;
}

bool MsTcpLink::link_create( ms_tcp_io_callback backFunc, const char* p_localAddr, unsigned short us_localPort )
{
    if( backFunc == NULL ){
        MS_LOGER_ERROR( "backfunc is null." );
    }

    mp_tcpcallback = backFunc;
    //可用于根据指定的地址族、数据类型和协议来分配一个套接口的描述字及其所用的资源的函数
    mi_socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );//tcp协议
    if( mi_socket < 0 ){
        MS_LOGER_ERROR( "create socket err. err = %s", strerror( errno ) );
        return false;
    }

    m_local_address.sin_family = AF_INET;
    m_local_address.sin_port = htons( us_localPort );

    std::string str_name;
    unsigned int ui_ipv4 = 0;
    if( p_localAddr ){
        str_name = p_localAddr;
        if( get_ip_by_name( str_name, ui_ipv4 ) ){
            m_local_address.sin_addr.s_addr = htonl( ui_ipv4 );
        } else {
            MS_LOGER_ERROR( "get_ip_by_name failed. ip=%s, port=%s", str_name.c_str(), us_localPort );
            m_local_address.sin_addr.s_addr = htonl( INADDR_ANY );
        }
    } else {
        m_local_address.sin_addr.s_addr = htonl( INADDR_ANY );
    }

    int i_opt = 1;
    //用于任意类型、任意状态套接口的设置选项值
    setsockopt( mi_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&i_opt, sizeof(int) );

    //from MSLogerServer
//        set_socket_opt(fd, SOL_SOCKET, SO_KEEPALIVE, (int)1);
//        set_socket_opt(fd, SOL_TCP, TCP_KEEPIDLE, (int)1800);

    //将一本地地址与一套接口捆绑。本函数适用于未连接的数据报或流类套接口
    if (::bind( mi_socket, (sockaddr*)&m_local_address, sizeof(m_local_address) ) < 0)
    {
        ::close( mi_socket );
        MS_LOGER_ERROR( "bind err. err = %s", strerror( errno ) );
        return false;
    }

    //update local address
    socklen_t len = sizeof( m_local_address );
    ::getsockname( mi_socket, (sockaddr *)&m_local_address, &len );  //用于获取一个套接字的名字

    return true;
}

bool MsTcpLink::link_connect( const char* p_addr, unsigned short us_port )
{
    if( (p_addr == NULL) || (us_port == 0) ){
        MS_LOGER_ERROR( "link_connect failed. wrong para." );
        return false;
    }

    std::string str_name = p_addr;
    unsigned int ui_ipv4 = 0;
    if( get_ip_by_name( str_name, ui_ipv4 ) ){
        sockaddr_in st_addr_remote;
        st_addr_remote.sin_family = AF_INET;
        st_addr_remote.sin_port = htons( us_port );
        st_addr_remote.sin_addr.s_addr = htonl( ui_ipv4 );

        set_conn_nonblock( mi_socket );
        set_link_type( link_TypeConnect );

        //用于建立与指定socket的连接。
        int i_ret = ::connect( mi_socket, (const sockaddr *)&st_addr_remote, sizeof( m_remote_address ) );
        if( ((i_ret == -1) && errno == (EINPROGRESS)) || i_ret != -1 ){
            socklen_t len = sizeof( m_local_address );
            ::getsockname( mi_socket, (sockaddr *)&m_local_address, &len );
            memcpy( &m_remote_address, &st_addr_remote, sizeof(st_addr_remote) );
            set_conn_nonblock( mi_socket );
            //call back
            send_link_event( EVT_TCP_CONNECTED );
            time( &mt_last_alive_time );
            return true;
        } else {
            MS_LOGER_ERROR( "connect failed. ip=%s, port=%d, error=%s", p_addr, us_port, strerror( errno ) );
            ::close( mi_socket );
            mi_socket = -1;
            send_link_event( EVT_TCP_DISCONNECTED );
            return false;
        }
    } else {
        MS_LOGER_ERROR( "get_ip_by_name failed. ip=%s, port=%s", str_name.c_str(), us_port );
    }

    return false;
}

bool MsTcpLink::link_listen( unsigned int ui_block )
{
    if( ui_block == 0  ){
        MS_LOGER_ERROR( "link_listen failed. ui_block = 0." );
        return false;
    }

    set_conn_nonblock( mi_socket );
    set_link_type( link_TypeListen );

    if( -1 != ::listen( mi_socket, ui_block ) ){//在套接字函数中表示让一个套接字处于监听到来的连接请求的状态
        time( &mt_last_alive_time );

        return true;
    }
    MS_LOGER_ERROR( "listen failed. socket=%d", mi_socket );
    return false;
}

bool MsTcpLink::link_send( const char* p_data, unsigned int ui_size )
{
    if( (p_data == NULL) || (ui_size == 0) ){
        MS_LOGER_ERROR( "link_send failed. p_data is null or size is zero." );
        return false;
    }

    int i_send_size = send_data( p_data, ui_size );    
    if( i_send_size < 0 ){
        int i_error = errno;
        if( i_error == EAGAIN ){
            i_send_size = 0;
        } else {
            MS_LOGER_ERROR( "send data failed. shutdown socket=%d, error= %d:%s", mi_socket, errno, strerror( errno ) );
            ::shutdown( mi_socket, SHUT_RDWR );//禁止在一个套接口上进行数据的接收与发送
            return false;
        }
    }

    if( (unsigned int)i_send_size == ui_size ){
        return true;
    }

    if( (unsigned int)i_send_size < ui_size ){
        send_buff_s st_send_buff;
        char *p_buff = new char[ui_size - i_send_size];
        if( p_buff == NULL ){
            MS_LOGER_ERROR( "malloc failed." );
            return false;
        }
        memcpy( p_buff, p_data + i_send_size, ui_size - i_send_size );
        st_send_buff.p_data = p_buff;
        st_send_buff.ui_size = ui_size - i_send_size;

        CGuardLock<pthread_mutex_t> guard( &mo_send_buff_list_lock );
        m_send_buff_list.push_back( st_send_buff );

        MsTcpLink::set_send_flag( true );
    }

    //MsEpoll::attach_to_epoll( mi_socket, mh_link, true );
    time( &mt_last_alive_time );

    return true;
}

void MsTcpLink::link_destroy()
{
    del_send_buff();
    MsTcpLink::del_link( mh_link );
    if( mi_socket != -1 ){
        ::shutdown( mi_socket, SHUT_RDWR );
        mi_socket = -1;
        send_link_event( EVT_TCP_CLOSED );
    }
}

bool MsTcpLink::on_event_accept( )
{
    MS_LOGER_INFO( "on_event_accept." );
    time_t st_time;
    time( &st_time );

    while( true ){
        MsTcpLink *p_link = new MsTcpLink();
        if( !p_link->accept( mi_socket, mp_tcpcallback ) ){
            if( errno != EAGAIN ){
                MS_LOGER_ERROR( "accept err, err = %s", strerror( errno ) );
            }
            break;
        }

        CRefCountPtr<MsTcpLink> p_reflink = CRefCountPtr<MsTcpLink>( p_link );
        HTCPLINK h_link = INVALID_HTCPLINK;
        MsTcpLink::add_tcp_link( h_link, p_reflink );
        p_reflink->set_link( h_link );
        p_reflink->update_link_time( st_time );        

        send_accept_link( h_link );
        CSingletonMsEpollHandler::Instance()->attach_to_epoll(  p_reflink->get_socket(), p_reflink->get_link() );
    }
    update_link_time( st_time );

    return true;
}

bool MsTcpLink::on_event_connected()
{
    MS_LOGER_INFO( "on_event_connected." );
    set_link_type( link_TypeConnected );
    time( &mt_last_alive_time );
    return true;
}

bool MsTcpLink::on_event_read()
{
    const int i_recv_buff_size = 10 * 1024 * 1024;
    char *p_buffer = new char[i_recv_buff_size]();
    if( p_buffer == NULL ){
        MS_LOGER_ERROR( "malloc failed. err=%s", strerror( errno ) );
        return false;
    }
    //recv begin
    send_recv_data( NULL, 0, STATUS_RECV_BEGIN );
    int i_recv_size = 0;
    while( true ){        
        int i_recv_tmp = ::read( mi_socket, p_buffer + i_recv_size, i_recv_buff_size - i_recv_size );
        if( (i_recv_tmp < 0) && (errno != ECONNRESET) ){
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK) ){
                break;
            }
            if( errno == EINTR ){
                CSingletonMsEpollHandler::Instance()->mod_epoll_read( mi_socket, mh_link );
            }
            MS_LOGER_INFO( "read error, err=%s, socket=%d, link=0x%04x", strerror( errno ), mi_socket, mh_link );
            break;
        } else if( i_recv_tmp == 0 ) {
            MS_LOGER_INFO( "read error, len=%d, err=%s, socket=%d, link=0x%04x", i_recv_tmp, strerror( errno ), mi_socket, mh_link );
            CSingletonMsEpollHandler::Instance()->del_epoll_read( mi_socket );
            link_destroy();
            break;
        } else {
            //OK
            send_recv_data( NULL, 0, STATUS_RECV_MIDDLE );
        }
        i_recv_size += i_recv_tmp;
    }
    //recv end
    send_recv_data( p_buffer, i_recv_size, STATUS_RECV_END );
    time( &mt_last_alive_time );
    delete []p_buffer;
    p_buffer = NULL;

    return true;
}

bool MsTcpLink::on_event_write()
{    
    MS_LOGER_INFO( "on_event_write." );
    //send data from databuf.
    CGuardLock<pthread_mutex_t> guard( &mo_send_buff_list_lock );
    std::list<send_buff_s>::iterator it = m_send_buff_list.begin();
    for( ; it != m_send_buff_list.end(); ){
        send_buff_s st_send_buff = *it;
        char *p_data = st_send_buff.p_data;
        unsigned int ui_size = st_send_buff.ui_size;
        if( (p_data == NULL) || (ui_size == 0) ){
            MS_LOGER_ERROR( "data buff is null." );
            if( p_data != NULL ){
                delete []p_data;
                p_data = NULL;
            }
            it = m_send_buff_list.erase( it );
            continue;
        }

        int i_send_size = 0;
        while( (unsigned int)i_send_size < ui_size ){
            int i_send_tmp = send_data( p_data + i_send_size, ui_size - i_send_size );
            if( i_send_tmp < 0 ){                
                int i_error = errno;
                if( i_error != EAGAIN ){
                    //close??
                    MS_LOGER_ERROR( "send data failed. error=%d,%s", i_error, strerror( i_error ) );
                } else {
                    MS_LOGER_ERROR( "send data failed. EAGAIN." );
                }
                break;
            }
            i_send_size += i_send_tmp;
            if( (i_send_tmp == 0) && (errno == EAGAIN) ){
                break;
            }
            if( (unsigned int)i_send_size == ui_size ){
                break;
            }
        }

        bool b_break = false;
        if( ((unsigned int)i_send_size < ui_size) && (i_send_size != 0) ){
            char *p_new = new char[ui_size - i_send_size];
            if( p_new == NULL ){
                MS_LOGER_ERROR( "new failed." );
                return false;
            }
            memcpy( p_new, p_data + i_send_size, ui_size - i_send_size );
            send_buff_s st_send_tmp;
            st_send_tmp.p_data = p_new;
            st_send_tmp.ui_size = ui_size - i_send_size;
            m_send_buff_list.push_front( st_send_tmp );
            b_break = true;
        }

        if( i_send_size != 0 ){
            delete []p_data;
            p_data = NULL;
            it = m_send_buff_list.erase( it );
        }
        if( b_break ){
            break;
        }
    }
    time( &mt_last_alive_time );

    return true;
}

int MsTcpLink::set_conn_nonblock( int fd )
{
    int flag = fcntl( fd, F_GETFL, 0);//改变已打开的文件性质
    if( -1 == flag ) {
        MS_LOGER_FATAL("fcntl get error, err=%s", strerror( errno ));
        return -1;
    }
    flag |= O_NONBLOCK;
    if( fcntl( fd, F_SETFL, flag ) == -1 ) {
        MS_LOGER_FATAL("fcntl set error, err=%s", strerror( errno ));
        return -1;
    }

    return 0;
}

int MsTcpLink::send_data( const char* p_data, unsigned int ui_size )
{
    int i_write = ::write( mi_socket, p_data, ui_size );//把指针buf所指的内存写入count个字节到参数fd所指的文件内
    return i_write;
}

bool MsTcpLink::accept( unsigned int ui_listen_fd, ms_tcp_io_callback p_callback )
{
    struct sockaddr_in clientaddr;
    socklen_t clilen = sizeof(struct sockaddr_in);
    int ui_socket = ::accept( ui_listen_fd, (sockaddr *)&clientaddr, &clilen );
    if( ui_socket < 0 ){
        MS_LOGER_ERROR( "accept failed. err=%s", strerror( errno ) );
        return false;
    }

    memcpy( &m_remote_address, &clientaddr, sizeof(clientaddr) );

    socklen_t local_size = sizeof(m_local_address);
    if (-1 == ::getsockname(ui_socket, (sockaddr*)&m_local_address, &local_size)){
        ::close(ui_socket);
        return false;
    }

    mp_tcpcallback = p_callback;
    set_link_type( link_TypeAccept );

    mi_socket = ui_socket;

    //set tcp_nodelay
    int flag = 1;
    setsockopt( ui_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof( int ) );

    set_check_alive_flag( true );
    set_conn_nonblock( ui_socket );    

    return true;
}

void MsTcpLink::send_link_event( unsigned int ui_event )
{
    MS_LOGER_INFO( "link = 0x%04x, socket = %d, event=%d", mh_link, mi_socket, ui_event );
    TcpEvent_S st_event;
    st_event.h_link = mh_link;
    st_event.ui_event = ui_event;
    (*mp_tcpcallback)( &st_event, 0 );
}

void MsTcpLink::send_accept_link( HTCPLINK h_accept_link )
{
    MS_LOGER_INFO( "link = 0x%04x, accept link=%d", mh_link, h_accept_link );
    TcpEvent_S st_event;
    st_event.h_link = mh_link;
    st_event.ui_event = EVT_TCP_ACCEPTED;

    TcpAcceptedParam_S param;
    param.h_acceptLink = h_accept_link;
    (*mp_tcpcallback)( &st_event, &param );
}

void MsTcpLink::send_recv_data( char *buffer, int size, int i_status )
{
    MS_LOGER_INFO( "link = 0x%04x, recvdata", mh_link );

    TcpEvent_S st_event;
    st_event.h_link = mh_link;
    st_event.ui_event = EVT_TCP_RECEIVEDATA;

    TcpRecvDataParam_S param;
    param.p_RecvData = buffer;
    param.ui_size = size;
    param.ui_status = i_status;
    (*mp_tcpcallback)( &st_event, &param );
}

void MsTcpLink::update_link_time( time_t &st_time )
{
    CGuardLock<pthread_mutex_t> guard( &mo_last_alive_time_lock );
    mt_last_alive_time = st_time;
}

time_t MsTcpLink::get_link_time()
{
    CGuardLock<pthread_mutex_t> guard( &mo_last_alive_time_lock );
    return mt_last_alive_time;
}

bool MsTcpLink::is_alive( time_t &st_time )
{
    if( mb_need_check_alive ){
        if( st_time < get_link_time() ){
            update_link_time( st_time );
            return true;
        }
        return (st_time - get_link_time()) < MsTcpLink::ui_check_alive_timeout;
    }
    return true;
}

void MsTcpLink::del_send_buff()
{
    CGuardLock<pthread_mutex_t> guard( &mo_send_buff_list_lock );
    std::list<send_buff_s>::iterator it = m_send_buff_list.begin();
    for( ; it != m_send_buff_list.end(); ){
        send_buff_s st_buff = *it;
        if( st_buff.p_data != NULL ){
            delete []st_buff.p_data;
            st_buff.p_data = NULL;
        }
        it = m_send_buff_list.erase( it );
    }
}

bool MsTcpLink::get_link_addr( unsigned int ui_type, unsigned int *pui_ip, unsigned short *pus_port )
{
    sockaddr_in *p_addr = NULL;
    if( ui_type == LINK_ADDR_LOCAL ){
        p_addr = &m_local_address;
    } else if( ui_type == LINK_ADDR_REMOTE ){
        p_addr = &m_remote_address;
    } else {
        MS_LOGER_FATAL( "get_link_addr, undefine type=%d", ui_type );
        return false;
    }
    *pui_ip = ntohl( p_addr->sin_addr.s_addr );
    *pus_port = ntohs( p_addr->sin_port );
    return true;
}

void MsTcpLink::set_check_alive_flag( bool b_flag )
{
    mb_need_check_alive = b_flag;
}
