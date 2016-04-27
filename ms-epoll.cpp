#include <string.h>
#include "ms-epoll.h"
#include "tcp-link.h"
#include "Guard.h"
#include "clientlog-api.h"

bool MsEpoll::init()
{
    if( -1 != mi_epoll_fd ){
        return false;
    }

    //生成一个epoll专用的文件描述符。它其实是在内核申请一空间，用来存放你相关的socket fd上是否发生以及发生了什么事件
    mi_epoll_fd = ::epoll_create( 1024 );
    if( mi_epoll_fd < 0 ){
        MS_LOGER_ERROR( "epoll_create failed. error=%s", strerror( errno ) );
        return false;
    }

    mi_exit_event = ::eventfd( 0, EFD_NONBLOCK );//用于进程或者线程间的通信
    if( mi_exit_event < 0 ){
        MS_LOGER_ERROR( "eventfd failed." );
        ::close( mi_epoll_fd );
        mi_epoll_fd = -1;
        return false;
    }

    epoll_event event;
    memset( &event, 0, sizeof(event) );
    event.events = EPOLLIN;
    event.data.fd = mi_exit_event;

    //用于控制某个epoll文件描述符上的事件，可以注册事件，修改事件，删除事件。
    if( ::epoll_ctl( mi_epoll_fd, EPOLL_CTL_ADD, mi_exit_event, &event) < 0 ){//注册事件
        MS_LOGER_ERROR( "epoll_ctl epoll_ctl_add exit_event failed." );
        ::close( mi_epoll_fd );
        mi_epoll_fd = -1;
        ::close( mi_exit_event );
        mi_exit_event = -1;
        return false;
    }

    //MsTcpLink初始化
    MsTcpLink::init_link_lock();
    MsTcpLink::init_link_map();
    MsTcpLink::set_send_flag( false );

    if( ms_pthread_creat( &m_pthread_id, &worker_thread, this ) < 0 ){
        MS_LOGER_ERROR( "create worker_thread failed." );
        ::close( mi_epoll_fd );
        mi_epoll_fd = -1;
        ::close( mi_exit_event );
        mi_exit_event = -1;
        return false;
    }

    mb_timeout_thread_flag = true;
    if( ms_pthread_creat( &m_pthread_timeout, &timeoutThreadProc, this) < 0 ){
        MS_LOGER_ERROR( "create timeout_thread_proc failed." );
        mb_timeout_thread_flag = false;
        ::close( mi_epoll_fd );
        mi_epoll_fd = -1;
        ::close( mi_exit_event );
        mi_exit_event = -1;
        return false;
    }

    return true;
}

void MsEpoll::end()
{
    unsigned long long ull_exit = 1;
    ::write( mi_exit_event, &ull_exit, sizeof(ull_exit) );

    if( m_pthread_id != 0 ){
        void *result;
        pthread_join( m_pthread_id, &result );
        m_pthread_id = 0;
    }

    mb_timeout_thread_flag = false;
    if( m_pthread_timeout != 0 ){
        void *result;
        pthread_join( m_pthread_timeout, &result );
        m_pthread_timeout = 0;
    }

    ::close( mi_exit_event );
    ::close( mi_epoll_fd );
    mi_epoll_fd = -1;
    mi_exit_event = -1;

    MsTcpLink::free_all_link_map();
    MsTcpLink::destroy_link_lock();

    return;
}

void *MsEpoll::worker_thread( void *arg )
{
    MsEpoll *p_this = (MsEpoll *)arg;
    const int i_max_eventnum = 512;
    epoll_event events[i_max_eventnum];

    while( true ){
        int i_event_num = epoll_wait( p_this->mi_epoll_fd, events, i_max_eventnum, -1 );
        if( i_event_num < 0 ){
            MsTcpLink::do_send_buff();
            continue;
        }
        for( int i = 0; i < i_event_num; i++ ){
            if( events[i].data.fd == p_this->mi_exit_event ){
                MS_LOGER_INFO( "fd is exit_event, return." );
                return NULL;
            }

            unsigned int ui_event = events[i].events;
            HTCPLINK h_link = events[i].data.u64 >> 32;
            CRefCountPtr<MsTcpLink> p_link;
            if( !MsTcpLink::find_link( h_link, p_link ) ){
                MS_LOGER_ERROR( "find link failed, link = 0x%04x", h_link );
                continue;
            }
            if( MsEpoll::is_disconnected( ui_event ) ){
                //notify disconnect
                p_link->send_link_event( EVT_TCP_DISCONNECTED );
                p_link->del_send_buff();
                MsTcpLink::del_link( h_link );
                continue;
            }
            if( p_link->get_link_type() == MsTcpLink::link_TypeListen ){
                if( MsEpoll::is_readable( ui_event ) ){
                    p_link->on_event_accept( );
                }
                continue;
            }
            if( p_link->get_link_type() == MsTcpLink::link_TypeConnect ){
                if( MsEpoll::is_readable( ui_event ) || MsEpoll::is_writeable( ui_event ) ){
                    p_link->on_event_connected();
                }
                continue;
            }
            if( p_link->get_link_type() == MsTcpLink::link_TypeConnected ||
                p_link->get_link_type() == MsTcpLink::link_TypeAccept ){
                if( MsEpoll::is_readable( ui_event ) ){
                    p_link->on_event_read();
                }
                //if( MsEpoll::is_writeable( ui_event) ){
                //    p_link->on_event_write();
                //}
                continue;
            }
        }
        MsTcpLink::do_send_buff();
    }
    return NULL;
}

void *MsEpoll::timeoutThreadProc( void *arg )
{
    MsEpoll *p_this = (MsEpoll *)arg;
    while( p_this->mb_timeout_thread_flag ){
        MsTcpLink::check_link_timeout();
        sleep( 5 );
    }

    MS_LOGER_INFO( "thread timeoutThreadProc exit." );
    return NULL;
}

bool MsEpoll::is_disconnected( unsigned int ui_event )
{
    return ui_event & ( EPOLLERR | EPOLLHUP | EPOLLRDHUP );
}

bool MsEpoll::is_readable( unsigned int ui_event )
{
    return ui_event & EPOLLIN;
}

bool MsEpoll::is_writeable( unsigned int ui_event )
{
    return ui_event & EPOLLOUT;
}

bool MsEpoll::attach_to_epoll( int i_fd, HTCPLINK h_link, bool b_write )
{
    unsigned int ui_events = 0;
    if( b_write ){
        ui_events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
    } else {
        ui_events = EPOLLIN | EPOLLET;
    }

    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events   = ui_events;
    event.data.u64 = h_link;
    event.data.u64 <<= 32;
    event.data.fd  = i_fd;

    if( ::epoll_ctl( mi_epoll_fd, EPOLL_CTL_ADD, i_fd, &event ) < 0 ){
        MS_LOGER_ERROR( "epoll_ctl EPOLL_CTL_ADD failed." );
        return false;
    }
    return true;
}

bool MsEpoll::mod_epoll_read( int i_fd, HTCPLINK h_link )
{
    unsigned int ui_events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;

    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events   = ui_events;
    event.data.u64 = h_link;
    event.data.u64 <<= 32;
    event.data.fd  = i_fd;

    if( ::epoll_ctl( mi_epoll_fd, EPOLL_CTL_MOD, i_fd, &event ) < 0 ){
        MS_LOGER_ERROR( "epoll_ctl EPOLL_CTL_MOD failed." );
        return false;
    }
    return true;
}

bool MsEpoll::del_epoll_read( int i_fd )
{
    epoll_event event;
    event.data.fd = i_fd;
    event.events = EPOLLIN | EPOLLET;
    if( epoll_ctl( mi_epoll_fd, EPOLL_CTL_DEL, i_fd, &event ) < 0 ){
        MS_LOGER_ERROR( "epoll_ctl EPOLL_CTL_DEL failed." );
        return false;
    }
    return true;
}

//创建线程
int ms_pthread_creat(pthread_t *pst_tid, pfRoutine pf_func, void *pv_func_para
                     , bool b_detached, unsigned int ui_stack_size, int i_policy, int i_priority)
{
    pthread_attr_t st_attr;
    struct sched_param st_param;
    size_t ui_stack_tmp = 0;
    int  i_ret = 0;
    int  i_policy_tmp = 0;
    int  i_priority_tmp = 0;
    pthread_t st_tid_tmp = 0;

    if ( pst_tid == NULL ) {
        pst_tid = &st_tid_tmp;
    }
    if ( pf_func == NULL ) {
        return -1;
    }

    i_ret = pthread_attr_init(&st_attr);//初始化一个线程对象的属性,需要用pthread_attr_destroy函数对其去除初始化。
    if (i_ret != 0) {
        return -1;
    }

    if ((ui_stack_size > 0) && (ui_stack_size < PTHREAD_STACK_MIN)) {//PTHREAD_STACK_MIN:宏指示一个线程堆栈的最小字节,定义在limits.h
        ui_stack_tmp = PTHREAD_STACK_MIN;//输出ui_stack_tmp的值
    } else if (ui_stack_size == 0) {
        i_ret = pthread_attr_getstacksize(&st_attr, &ui_stack_tmp);//函数设定线程堆栈时，设定的线程堆栈大小值必须大于等于PTHREAD_STACK_MIN宏定义的值，否则返回错误。
        if (i_ret != 0) {
            return -1;
        }
    } else {
        ui_stack_tmp = ui_stack_size;
    }

    if (-1 == i_policy) {
        pthread_attr_getschedpolicy(&st_attr, &i_policy_tmp);
    } else {
        i_policy_tmp = i_policy;
    }

    pthread_attr_setstacksize(&st_attr, ui_stack_tmp);
    pthread_attr_getschedparam(&st_attr, &st_param);
    if ( i_priority >= 0 ) {
        if (i_priority > sched_get_priority_max(i_policy_tmp)) {
            i_priority_tmp = sched_get_priority_max(i_policy_tmp);
        } else if (i_priority < sched_get_priority_min(i_policy_tmp)) {
            i_priority_tmp = sched_get_priority_min(i_policy_tmp);
        } else {
            i_priority_tmp = i_priority;
        }
        st_param.sched_priority = i_priority_tmp;
    }
    pthread_attr_setschedparam(&st_attr, &st_param);
    pthread_attr_setschedpolicy(&st_attr, i_policy_tmp);

    if ( b_detached == true ) {
        pthread_attr_setdetachstate(&st_attr, PTHREAD_CREATE_DETACHED);
    }

    i_ret = pthread_create(pst_tid, &st_attr, pf_func, pv_func_para);//创建线程，第三个参数是线程运行函数的起始地址。最后一个参数是运行函数的参数。
    if (i_ret != 0) {//若线程创建成功，则返回0。若线程创建失败，则返回出错编号
        return -1;
    }

    return 0;
}
