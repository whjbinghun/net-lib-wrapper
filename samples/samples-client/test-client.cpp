#include "test-client.h"
#include "comm-func.h"
#include "md5.h"
#include <stdio.h>


Test_Client::Test_Client( std::string str_server_ip, unsigned short us_port )
    :mstr_serverAddr( str_server_ip ), ms_serverPort( us_port )
    ,mh_tcpLink( INVALID_HTCPLINK ), mui_client_id( 0 )
{
    pthread_mutex_init( &mo_link_lock, NULL );
    pthread_mutex_init( &mo_addr_lock, NULL );
    pthread_mutex_init( &mo_client_lock, NULL );
}

Test_Client::~Test_Client()
{
    pthread_mutex_destroy( &mo_link_lock );
    pthread_mutex_destroy( &mo_addr_lock );
    pthread_mutex_destroy( &mo_client_lock );
}

bool Test_Client::Init()
{
    ::ms_tcp_init();
    MS_LOGER_INFO( "Init." );

    process_connect_server();
    return true;
}

bool Test_Client::Uninit()
{
    return true;
}

void Test_Client::on_tcp_created( HTCPLINK h_link )
{
    unsigned int ui_ip = 0;
    unsigned short us_port = 0;
    if( !tcp_get_link_addr( h_link, LINK_ADDR_LOCAL, &ui_ip, &us_port ) ){
        MS_LOGER_ERROR( "TcpGetLinkAddr error.link=0x%04x", h_link );
    }

    std::string str_ip = ConvertIP( ui_ip );
    MS_LOGER_INFO( "OnTcpCreated, link=0x%04x,ip=%s,port=%d.", h_link, str_ip.c_str(), us_port );
}

void Test_Client::on_tcp_closed( HTCPLINK h_link )
{
    if( mh_tcpLink == h_link ){
        mh_tcpLink = INVALID_HTCPLINK;
    }

    MS_LOGER_INFO( "OnTcpClosed, link=0x%04x", h_link );
}

void Test_Client::on_tcp_accepted( HTCPLINK h_link, HTCPLINK h_accept_link )
{
    return;
}

void Test_Client::on_tcp_connected( HTCPLINK h_link )
{
    unsigned int ui_ip = 0;
    unsigned short us_port = 0;
    if( !tcp_get_link_addr( h_link, LINK_ADDR_REMOTE, &ui_ip, &us_port ) ){
        MS_LOGER_ERROR( "TcpGetLinkAddr error.link=0x%04x", h_link );
        return;
    }

    unsigned int ui_ip_local = 0;
    unsigned short us_port_local = 0;
    if( !tcp_get_link_addr( h_link, LINK_ADDR_LOCAL, &ui_ip_local, &us_port_local ) ){
        MS_LOGER_ERROR( "TcpGetLinkAddr error.link=0x%04x", h_link );
        return;
    }

    std::string str_remote_ip = ConvertIP( ui_ip );
    std::string str_local_ip = ConvertIP( ui_ip_local );
    mstr_localAddr = str_local_ip;
    mstr_serverAddr = str_remote_ip;
    ms_serverPort = us_port;

    MS_LOGER_INFO( "OnTcpConnect, link=0x%04x,remote ip:port=%s:%d, local ip:port=%s:%d",
                   h_link, str_remote_ip.c_str(), us_port, str_local_ip.c_str(), us_port_local );
}

void Test_Client::on_tcp_disconnected( HTCPLINK h_link )
{
//    if( h_link == mh_tcpLink ){
//        mh_tcpLink = INVALID_HTCPLINK;
//    }
    return;
}

void Test_Client::on_tcp_recv_data( HTCPLINK h_link, const CRefCountPtr<MsgHeader_S>& p_pkt )
{
    if( mh_tcpLink != h_link ){
        MS_LOGER_ERROR( "Invalid TcpLink=0x%04x.", h_link );
        return;
    }

    unsigned int ui_type =  p_pkt->msg_type ;
    MS_LOGER_INFO( "on_tcp_recv_data, type=%d", ui_type );
    switch( ui_type ){
    case IR_MONITOR_VER_NEGO_RESP:
    {
        on_ver_nego_ack( CRefCountPtr<MSCommResp_S>(p_pkt) );
    }
        return;
    case IR_MONITOR_LINK_AUTH_RESP:
    {
        on_link_auth_ack( CRefCountPtr<MSAuthResp_S>(p_pkt) );
    }
        return;
    default:
        return;
    }
}

bool Test_Client::process_connect_server()
{
    std::string str_server_ip;
    unsigned short us_server_port;
    do{
        CGuardLock<pthread_mutex_t> guard( &mo_addr_lock );
        str_server_ip = mstr_serverAddr;
        us_server_port = ms_serverPort;
    }while( 0 );

    if( get_link() != INVALID_HTCPLINK ){
        MS_LOGER_INFO( "disconnect and reconnect" );
        tcp_destroy( get_link() );
        set_link( INVALID_HTCPLINK );
    }

    bool b_connect = false;
    std::string str_lisAddr = "192.168.8.109";
    HTCPLINK h_link = tcp_create( str_lisAddr.c_str(), 0 );
    if( h_link != INVALID_HTCPLINK ){
        b_connect = ::ms_tcp_connect( h_link, str_server_ip.c_str(), us_server_port );
    }

    if( !b_connect ){
        MS_LOGER_ERROR( "connect to server failed. server ip:port=%s:%d", str_server_ip.c_str(), us_server_port );
        tcp_destroy( h_link );
    } else {
        set_link( h_link );
        MS_LOGER_INFO( "connect to server succ. server ip:port=%s:%d", str_server_ip.c_str(), us_server_port );

        do_ver_nego_req();
    }

    return true;
}

void Test_Client::do_ver_nego_req()
{
    MSVerReq_S *p_req = new MSVerReq_S();
    p_req->magic_type = MSG_HEAD_MAGIC;
    p_req->msg_type = IR_MONITOR_VER_NEGO_REQ;
    p_req->seq_id = 0;
    p_req->total_len = sizeof( MSVerReq_S );
    p_req->body.ui_major_ver = 1;
    p_req->body.ui_sub_ver = 0;
    send_packet( get_link(), p_req, 1 );

    delete p_req;
    p_req = NULL;
}

void Test_Client::on_ver_nego_ack( const CRefCountPtr<MSCommResp_S> &p_pkt )
{
    if( p_pkt->i_ret != 0 ){
        MS_LOGER_ERROR( "on_ver_nego_ack error." );
        return;
    }

    do_link_auth_req();
}

void Test_Client::do_link_auth_req()
{
    MSAuthReq_S *p_auth_req = new MSAuthReq_S();
    p_auth_req->magic_type = MSG_HEAD_MAGIC;
    p_auth_req->msg_type = IR_MONITOR_LINK_AUTH_REQ;
    p_auth_req->seq_id = 0;
    p_auth_req->total_len = sizeof( MSAuthReq_S );

    UINT32 ui_sec = ::time(NULL);
    std::string str_time_of_day;
    GetLocalTimeDay( ui_sec, str_time_of_day );
    std::string str_user_name = MSADMIN_USER;
    std::string str_digest =  str_user_name + str_time_of_day;

    UINT8 auc_digest_digest[16] = {0};
    UINT8 auc_degist_tmp[16] = {0};

    const int i_str_len = 128;
    INT8 *pc_str_tmp = new INT8[i_str_len]();
    memset( pc_str_tmp, 0, i_str_len );
    snprintf( pc_str_tmp, i_str_len, "%s", str_digest.c_str() );
    get_md5( pc_str_tmp, strlen( pc_str_tmp), NULL, auc_digest_digest );

    INT8  ac_md5[33] = {0};
    INT32  i_rand_tmp=0;
    UINT32 ui_time_stamp_tmp=0;

    Md5Val_S st_md5;
    memcpy(&st_md5, auc_digest_digest, sizeof(st_md5));
    snprintf(ac_md5, sizeof(ac_md5), "%08x%08x%08x%08x", htonl(st_md5.n_a), htonl(st_md5.n_b), htonl(st_md5.n_c), htonl(st_md5.n_d));

    INT8  *pc_str = new INT8[i_str_len]();
    memset(pc_str, 0, i_str_len);
    snprintf(pc_str, i_str_len, "%s%s%urand=%d", str_user_name.c_str(), ac_md5, ui_time_stamp_tmp, i_rand_tmp);
    get_md5( pc_str, strlen(pc_str), NULL, auc_degist_tmp );

    p_auth_req->body.ui_client_type = CLIENT_MONITOR;
    p_auth_req->body.ui_time_stamp = ui_time_stamp_tmp;
    p_auth_req->body.ui_rand = i_rand_tmp;
    strcpy( p_auth_req->body.ac_login_id, str_user_name.c_str() );
    memcpy( p_auth_req->body.auc_digest, auc_degist_tmp, sizeof(auc_degist_tmp) );
    send_packet( get_link(), p_auth_req, 1 );

    delete p_auth_req;
    p_auth_req = NULL;
}

void Test_Client::on_link_auth_ack( const CRefCountPtr<MSAuthResp_S> &p_pkt )
{
    mui_client_id = p_pkt->body.ui_client_id;
}
