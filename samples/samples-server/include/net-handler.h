#ifndef NET_HANDLER_H_
#define NET_HANDLER_H_
#include <vector>
#include "clientlog-api.h"
#include "ms-epoll-tcp.h"
#include "Guard.h"
#include "comm-func.h"
#include "comm_typedef.h"
#include "net-packet.h"

template <typename HANDLE>
class TcpNetHandler : public HANDLE
{
public:
    enum enumProcessPktOpt
    {
        enumProcessPktInIOCallBack,
        enumProcessPktInThreadPool
    };

    TcpNetHandler() { m_pThis = this; }
    virtual ~TcpNetHandler() { m_pThis = NULL; }
    static void TcpIOCallbackFunc(void* p_param1, void* p_param2)
    {
        if (m_pThis == NULL){
            return;
        }

        TcpEvent_S* p_evt = (TcpEvent_S*)p_param1;
        TcpRecvDataParam_S* p_recv = (TcpRecvDataParam_S*)p_param2;
        TcpAcceptedParam_S* p_acpt = (TcpAcceptedParam_S*)p_param2;

        switch ( p_evt->ui_event )
        {
        case EVT_TCP_CREATED:
            m_pThis->on_tcp_created( p_evt->h_link );
            break;
        case EVT_TCP_CLOSED:
            m_pThis->on_tcp_closed( p_evt->h_link );
            break;
        case EVT_TCP_ACCEPTED:
            m_pThis->on_tcp_accepted( p_evt->h_link, p_acpt->h_acceptLink );
            break;
        case EVT_TCP_CONNECTED:
            m_pThis->on_tcp_connected( p_evt->h_link );
            break;
        case EVT_TCP_DISCONNECTED:
            m_pThis->on_tcp_disconnected( p_evt->h_link );
            break;
        case EVT_TCP_RECEIVEDATA:
            if ( p_recv->ui_status == STATUS_RECV_END ){
                m_pThis->on_tcp_recv_data( p_evt->h_link, p_recv->p_RecvData, p_recv->ui_size );
            }
            break;
        case EVT_TCP_ERRPROTOCOL:
            m_pThis->on_tcp_err_protocol( p_evt->h_link );
            break;
        default:
            break;
        }
    }

    static HTCPLINK tcp_create( const char* p_addr, unsigned short us_port )
    {
        return ::ms_tcp_create( TcpNetHandler<HANDLE>::TcpIOCallbackFunc, p_addr, us_port );
    }

    static void tcp_destroy( HTCPLINK h_link )
    {
        ::ms_tcp_destroy( h_link );
    }

    static bool tcp_get_link_addr( HTCPLINK h_link, unsigned int ui_type, unsigned int *pui_ip, unsigned short *pus_port )
    {
        return ::ms_tcp_get_link_addr( h_link, ui_type, pui_ip, pus_port );
    }

private:
    static TcpNetHandler<HANDLE>* m_pThis;
};

template <typename HANDLE>
TcpNetHandler<HANDLE>* TcpNetHandler<HANDLE>::m_pThis = NULL;



template<typename PACKET>
class TcpNetProtocolEvent
{
public:
    virtual ~TcpNetProtocolEvent(){}
protected:
    virtual void on_tcp_created( HTCPLINK h_link ) = 0;
    virtual void on_tcp_closed( HTCPLINK h_link ) = 0;
    virtual void on_tcp_accepted( HTCPLINK h_link, HTCPLINK h_accept_link ) = 0;
    virtual void on_tcp_connected( HTCPLINK h_link ) = 0;
    virtual void on_tcp_disconnected( HTCPLINK h_link ) = 0;
    virtual void on_tcp_recv_data( HTCPLINK h_link, const CRefCountPtr<PACKET>& p_pkt ) = 0;
    virtual void on_tcp_recv_unknown_packet( HTCPLINK h_link, const char* p_data, unsigned int ui_size )
    {
        if( p_data ){
            MS_LOGER_ERROR( "on_tcp_recv_unknown_packet, link=0x%04x, size=%d,data=%s", h_link, ui_size, p_data );
        } else {
            MS_LOGER_ERROR( "on_tcp_recv_unknown_packet, link=0x%04x, size=%d", h_link, ui_size );
        }
    }
    virtual void on_tcp_recv_data( HTCPLINK h_link, const char* p_data, unsigned int ui_size )
    {
        //on_tcp_recv_data( HTCPLINK h_link, const CRefCountPtr<PACKET>& p_pkt )
        std::vector<char>::size_type vs_old_size = mv_recv_buf.size();
        mv_recv_buf.resize( vs_old_size + ui_size );
        memcpy( mv_recv_buf.data() + vs_old_size, p_data, ui_size );
        if( mv_recv_buf.size() < sizeof( PACKET ) ){
            return;
        }

        if( ui_size < sizeof(PACKET) ) {
            return;
        }
        unsigned int ui_msg_head = htonl( MSG_HEAD_MAGIC );
        unsigned int ui_data_offset = 0;
        unsigned int ui_buf_data_len = mv_recv_buf.size();
        while( (ui_buf_data_len - ui_data_offset) > 0 ){
            if( memcmp( mv_recv_buf.data() + ui_data_offset, &ui_msg_head, sizeof( ui_msg_head ) ) != 0 ){
                ui_data_offset++;
                continue;
            }

            UINT32 ui_total_len = parse_data_total_len<PACKET>( mv_recv_buf.data() + ui_data_offset );
            if( ui_total_len + ui_data_offset > mv_recv_buf.size() ){
                std::vector<char> vectemp;
                unsigned int ui_temp_len = mv_recv_buf.size() - ui_data_offset;
                vectemp.resize( ui_temp_len );
                memcpy( vectemp.data(), mv_recv_buf.data() + ui_data_offset, ui_temp_len );
                mv_recv_buf.resize( ui_temp_len );
                mv_recv_buf = vectemp;
                return;
            }

            PACKET *p_msg = NULL;
            int i_parse_len = 0;
            i_parse_len = parse_data( mv_recv_buf.data() + ui_data_offset, ui_buf_data_len - ui_data_offset, p_msg );
            if( i_parse_len > 0 ){
                ui_data_offset += i_parse_len;
                CRefCountPtr<PACKET> pcref_packet = CRefCountPtr<PACKET>( p_msg );
                on_tcp_recv_data( h_link, pcref_packet );
            } else if( i_parse_len == 0 ){
                break;
            } else {
                ui_data_offset++;
                if( i_parse_len == CTRL_PRO_NOT_EXIST_ERROR ){
                    on_tcp_recv_unknown_packet( h_link, p_data, ui_size );
                }
            }
        }

        std::vector<char> vectemp;
        unsigned int ui_temp_len = mv_recv_buf.size() - ui_data_offset;
        vectemp.resize( ui_temp_len );
        memcpy( vectemp.data(), mv_recv_buf.data() + ui_data_offset, ui_temp_len );
        mv_recv_buf.resize( ui_temp_len );
        mv_recv_buf = vectemp;
    }

    virtual void on_tcp_err_protocol( HTCPLINK h_link )
    {

    }

    template <typename HANDLE> friend class TcpNetHandler;

public:
    static bool send_packet( HTCPLINK h_link, const PACKET& p_pkt, unsigned int ui_flag )
    {
        return send_packet( h_link, &p_pkt, ui_flag );
    }
    static bool send_packet( HTCPLINK h_link, const CRefCountPtr<PACKET>& p_pkt, unsigned int ui_flag )
    {
        return send_packet( h_link, *p_pkt, ui_flag );
    }

    static bool send_packet( HTCPLINK h_link, const PACKET* p_pkt, unsigned int ui_flag )
    {
        if( p_pkt == NULL ){
            return false;
        }

        std::vector<char> vc_buffer;
        switch( p_pkt->msg_type ){
#define CASE_PACKET( n, T_req )\
        case n: {\
            write_packet(vc_buffer, *(T_req*)p_pkt);\
            ::ms_tcp_send( h_link, (INT8 *)&vc_buffer[0], vc_buffer.size() );\
            break;\
            }
        CASE_PACKET( IR_MONITOR_VER_NEGO_REQ, MSVerReq_S );
        CASE_PACKET( IR_MONITOR_LINK_AUTH_REQ,  MSAuthReq_S );
#undef CASE_PACKET
        default:
            break;
        }

        vc_buffer.clear();
        return true;
    }

    void recv_data_buff_init()
    {
        mv_recv_buf.clear();
    }

private:
    std::vector<char>  mv_recv_buf;
};

#endif
