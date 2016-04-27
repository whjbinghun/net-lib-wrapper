#include <signal.h>
#include "ms-epoll-tcp.h"
#include "tcp-link.h"
#include "ms-epoll.h"
#include "clientlog-api.h"

//获取路径
std::string GetExePath( std::string &str_exepath )
{
    char sysfile[15] = "/proc/self/exe";//根据当前进程，从exe中获取当前进程的路径
    char szFile[512] = {0};
    readlink( sysfile, szFile, 512 );//把sysfile读取到szFile内存空间中

    char* psz = strrchr(szFile,'\\');//查找字符在指定字符串中从正面开始的最后一次出现的位置，截取之后的字符串
    if( NULL == psz)
    {
        psz = strrchr(szFile,'/');
    }
    if( psz != NULL)
    {
        *(psz+1)='\0';
    }

    str_exepath = szFile;
    return str_exepath;
}

bool ms_tcp_init( )
{
    //信号的默认处理规则SIGPIPE信号的默认执行动作是terminate(终止、退出),
    //所以client会退出。若不想客户端退出可以把SIGPIPE设为SIG_IGN
    //交给系统init去回收。 这里子进程就不会产生僵尸进程了。
    signal( SIGPIPE, SIG_IGN );
    std::string str_exe_path;
    GetExePath( str_exe_path );
    str_exe_path += "MS_EPOLLTCP/";
    ms_log_init( str_exe_path.c_str(), LOG_Local );

    return CSingletonMsEpollHandler::Instance()->init();
}

void ms_tcp_uninit()
{
    CSingletonMsEpollHandler::Instance()->end();
    CSingletonMsEpollHandler::Release();
    return;
}

//tcp创建
HTCPLINK ms_tcp_create( ms_tcp_io_callback backFunc, const char* p_localAddr, unsigned short us_localPort )
{
    MsTcpLink *p_link = new MsTcpLink();
    if(  p_link && p_link->link_create( backFunc, p_localAddr, us_localPort ) ){
        CRefCountPtr<MsTcpLink> p_reflink = CRefCountPtr<MsTcpLink>( p_link );
        HTCPLINK h_link = INVALID_HTCPLINK;
        MsTcpLink::add_tcp_link( h_link, p_reflink );
        p_reflink->set_link( h_link );
        p_reflink->send_link_event( EVT_TCP_CREATED );
        return h_link;
    } else {
        MS_LOGER_ERROR( "ms_tcp_create failed." );
        delete p_link;
    }

    return INVALID_HTCPLINK;
}

//tcp destory
bool ms_tcp_destroy( HTCPLINK h_link )
{
    CRefCountPtr<MsTcpLink> p_link;
    if( MsTcpLink::find_link( h_link, p_link ) ){
        p_link->link_destroy();
        return true;
    } else {
        MS_LOGER_ERROR( "ms_tcp_destroy failed. can not find link." );
    }

    return false;
}

bool ms_tcp_connect( HTCPLINK h_link, const char* p_addr, unsigned short us_port )
{
    CRefCountPtr<MsTcpLink> p_link;
    if( MsTcpLink::find_link( h_link, p_link ) ){
        if( p_link->link_connect( p_addr, us_port ) ){
            CSingletonMsEpollHandler::Instance()->attach_to_epoll( p_link->get_socket(), h_link );

            return true;
        } else {
            MS_LOGER_ERROR( "link_connect failed. " );
        }
    } else {
        MS_LOGER_ERROR( "ms_tcp_connect failed. can not find link." );
    }

    return false;
}

bool ms_tcp_listen( HTCPLINK h_link, unsigned int ui_block )
{
    CRefCountPtr<MsTcpLink> p_link;
    if( MsTcpLink::find_link( h_link, p_link ) ){
        if( p_link->link_listen( ui_block ) ){
            int i_fd = p_link->get_socket();
            CSingletonMsEpollHandler::Instance()->attach_to_epoll( i_fd, h_link );

            return true;
        } else {
            MS_LOGER_ERROR( "link_listen failed." );
        }
    } else {
        MS_LOGER_ERROR( "ms_tcp_listen failed. can not find link." );
    }

    return false;
}

bool ms_tcp_send( HTCPLINK h_link, const char* p_data, unsigned int ui_size )
{
    CRefCountPtr<MsTcpLink> p_link;
    if( MsTcpLink::find_link( h_link, p_link ) ){
        return p_link->link_send( p_data, ui_size );
    }

    MS_LOGER_ERROR( "ms_tcp_send failed." );
    return false;
}

bool ms_tcp_get_link_addr( HTCPLINK h_link, unsigned int ui_type, unsigned int *pui_ip, unsigned short *pus_port )
{
    CRefCountPtr<MsTcpLink> p_link;
    if( MsTcpLink::find_link( h_link, p_link ) ){
        return p_link->get_link_addr( ui_type, pui_ip, pus_port );
    }

    MS_LOGER_ERROR( "ms_tcp_get_link_addr failed." );
    return false;
}
