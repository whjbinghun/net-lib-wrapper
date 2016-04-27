#include "test-client-handle.h"

TestClientHandle::TestClientHandle()
    : LinkHdr( "TestClientHandle" )
{

}

TestClientHandle::~TestClientHandle()
{

}

bool TestClientHandle::recv_data( const CRefCountPtr<MsgHeader_S> &p_pkt )
{
    UINT32 ui_type = p_pkt->msg_type;
    MS_LOGER_INFO( "RecvData, type=%d", ui_type );
    switch( ui_type ){
    case IR_MONITOR_VER_NEGO_REQ:
        on_ver_nego_req( CRefCountPtr<MSVerReq_S>(p_pkt) );
        break;
    case IR_MONITOR_LINK_AUTH_REQ:
        on_link_auth_req( CRefCountPtr<MSAuthReq_S>(p_pkt) );
        break;
    }
    return true;
}

bool TestClientHandle::on_ver_nego_req( const CRefCountPtr<MSVerReq_S> &p_ver_pkt )
{
    MSCommResp_S st_resp;
    st_resp.magic_type = p_ver_pkt->magic_type;
    st_resp.msg_type = IR_MONITOR_VER_NEGO_RESP;
    st_resp.seq_id = p_ver_pkt->seq_id;
    st_resp.total_len = sizeof( st_resp );

    if( p_ver_pkt->body.ui_major_ver == 1 ){
        if( p_ver_pkt->body.ui_sub_ver == 0 ){
            st_resp.i_ret = 0;
        } else {
            st_resp.i_ret = 2;
        }
    } else {
        st_resp.i_ret = 2;
    }

    return send_packet( &st_resp );
}

bool TestClientHandle::on_link_auth_req( const CRefCountPtr<MSAuthReq_S> &p_auth_pkt  )
{
    MSAuthResp_S st_resp;
    st_resp.magic_type = p_auth_pkt->magic_type;
    st_resp.msg_type = IR_MONITOR_LINK_AUTH_RESP;
    st_resp.seq_id = p_auth_pkt->seq_id;
    //st_resp.total_len = sizeof( st_resp );

    st_resp.i_ret = 0;
    st_resp.body.ui_client_id = 1;
    st_resp.body.i_client_type = 1;
    st_resp.body.ul_out_dev_premission = 1;
    for( UINT32 ui_n = 0; ui_n < 100000; ui_n++ ){
        st_resp.body.ui_client_level_map[ui_n] = (UINT64)ui_n;
    }
    st_resp.body.str_extend = "abcdefghijklmnopqrstuvwxyz.10086!";
    return send_packet( &st_resp );
}
