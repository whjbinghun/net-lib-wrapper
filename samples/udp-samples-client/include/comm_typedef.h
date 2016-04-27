#ifndef COMM_TYPEDEF_H_
#define COMM_TYPEDEF_H_

#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/if_ether.h>
#include <unistd.h>
#include <pthread.h>
#include <string>

#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <map>


#define __32_BIT__  1
#if __32_BIT__

typedef char                INT8;
typedef short               INT16;
typedef int                 INT32;
typedef signed long long    INT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned long long  UINT64;

#endif

#define MSG_TYPE_RESP_OFFSET     0x8000
#define CTRL_PRO_NOT_EXIST_ERROR -2

#define MSG_HEAD_MAGIC        0xA2A22A2A
#define REPLAY_HEAD_MAGIC     0xA1A11A1A
#define VIDEO_HEAD_MAGIC      0xA0A05A5A

#define MS_ANA_VERSION 1
#define CONN_ALIVE_SECONDS 180
#define PRESET_MAX_NUM     16
#define CTRL_CMD_PARSE_THREAD_NUM   1
#define CTRL_CLIENT_MGR_THREAD_NUM  1
#define USER_SECRET_FILE_PATH   "secret.enc"
#define CTRL_RESP_TMP_DATA_LEN           (512 * 1024)

#define CTRL_VISI_JPG_FILE_LEN           (1920 * 1080 * 4)
#define CTRL_INFR_FILE_LEN               (640 * 480 * 32)

#define MONITOR_CNAME_LEN        (256)

#define TIME_LEN    14

#define ONE_M_BYTE (1024*1024)

#define MIN_COMPRESS_SIZE 1024

#define INVALID_SOCKET_PORT   0x0000


/*��������Ա�ʺ�*/
#define MSADMIN_USER  "msadmin"

typedef enum CLIENT_TYPE{
    CLIENT_CONFIG = 1,          //���÷���ͻ���
    CLIENT_PRESET = 2,          //Ԥ�õ�ͻ���
    CLIENT_MONITOR = 3,         //ʵʱԤ���ͻ���
    CLIENT_REPLAY = 4,          //�طſͻ���
    CLIENT_ACCOUNT_MANAGE = 5,  //�û�����ͻ���
    CLIENT_ELECTORN_MAP = 6,    //���ӵ�ͼ�ͻ���
    CLIENT_28181_GATEWAY = 7,   //28181 �����豸

    CLIENT_ERROR                //����
}CLIENT_TYPE_E;

//�û��ȼ�
typedef enum USER_TYPE{
    USER_TYPE_ROOT  = 0,    //��������Ա
    USER_TYPE_ADMIN = 1,    //��ͨ����Ա
    USER_TYPE_OPERATOR = 2, //����Ա
    USER_TYPE_OBSERVER = 3, //�۲�Ա

    USER_TYPE_ERROR
}USER_TYPE_E;

/* Ȩ�޹��� */
//�豸�޹أ��������ͣ�
#define LEVEL_DEVICE_CFG                            (1<<0)  /* �豸���飬��ӣ��޸ģ�ɾ��   ֻ�г�������Ա����*/
#define LEVEL_USER_MGR                              (1<<1)  /*�û�����   ��� �޸� ɾ�� ��ȡ�����û�Ȩ��*/
#define LEVEL_USER_CHG_OWN_PASSWD                   (1<<2) /*�û��޸��Լ�������*/

//�豸���Ȩ��
#define LEVEL_ELECTRON_MAP_SET                      (1<<10)  /*���ӵ�ͼ����*/
#define LEVEL_PRESET_SET                            (1<<11) /*Ԥ�õ���ӣ��޸ģ�ɾ��*/
#define LEVEL_PRESET_ANALYSE_SET                    (1<<12) /*Ԥ�õ��������*/
#define LEVEL_PRESET_ALARM_CONDITION_SET            (1<<13) /*Ԥ�õ㱨����������*/
#define LEVEL_INFRARED_RECORD_CONDITION_SET         (1<<14) /*����¼����������*/
#define LEVEL_CRUISE_PLAN_SET                       (1<<15) /*Ѳ�췽������ */
#define LEVEL_CRUISE_PLAN_CONTROLL                  (1<<16) /*Ѳ�����   ����Ա����*/
#define LEVEL_GOTO_PRESET                           (1<<17) /*�ٻ�Ԥ�õ�*/
#define LEVEL_PTZ_SLIDE_CONTROLL                    (1<<18) /*��̨�������*/
#define LEVEL_INFRARED_PARA_SET                     (1<<19) /*�����������*/
#define LEVEL_VISIBLE_PARA_SET                      (1<<20) /*�ɼ����������*/
#define LEVEL_INFRARED_CLIENT_ANALYSE_SET           (1<<21) /*�ͻ���˽�з�����������������   �۲�Ա����*/
#define LEVEL_VOD_REPLAY                            (1<<22) /*¼��ط�*/

#define LEVEL_OUT_DEV_ALL_PREMISSION  (LEVEL_USER_MGR | LEVEL_USER_CHG_OWN_PASSWD)
#define LEVEL_DEV_ALL_PREMISSION  (LEVEL_ELECTRON_MAP_SET | LEVEL_PRESET_SET | LEVEL_PRESET_ANALYSE_SET | LEVEL_PRESET_ALARM_CONDITION_SET \
                                 | LEVEL_INFRARED_RECORD_CONDITION_SET | LEVEL_CRUISE_PLAN_SET | LEVEL_CRUISE_PLAN_CONTROLL | LEVEL_GOTO_PRESET \
                                 | LEVEL_PTZ_SLIDE_CONTROLL | LEVEL_INFRARED_PARA_SET | LEVEL_VISIBLE_PARA_SET | LEVEL_INFRARED_CLIENT_ANALYSE_SET \
                                 | LEVEL_VOD_REPLAY )


/*********************** enum �� ************************/
typedef enum
{
    IR_MONITOR_VER_NEGO_REQ = 0x0001, /* �汾Э������ */
    IR_MONITOR_LINK_AUTH_REQ = 0x0002, /* ��Ȩ���� */
    IR_MONITOR_LINK_HEART_REQ = 0x0003, /* ����������� */
    IR_MONITOR_LINK_REL_REQ = 0x0004, /* �����ͷ����� */
    IR_MONITOR_XML_DATA_REQ = 0x0005, /* XML�ļ����� */
    IR_MONITOR_VID_DATA_PLAY_REQ = 0x0006, /* ��Ƶ���ݲ������� */
    IR_MONITOR_VID_DATA_STOP_REQ = 0x0007, /* ��Ƶ����ֹͣ���� */
    IR_MONITOR_VID_KEY_DATA_PLAY_REQ   = 0x0008, /* �ؼ���Ƶ֡���ݲ������� */
    IR_MONITOR_VID_KEY_DATA_STOP_REQ   = 0x0009, /* �ؼ���Ƶ֡����ֹͣ���� */
    IR_MONITOR_VID_SUB_STREAM_PLAY_REQ = 0x000A, /* ��������Ƶ���ݲ������� */
    IR_MONITOR_VID_SUB_STREAM_STOP_REQ = 0x000B, /* ��������Ƶ����ֹͣ���� */

    IR_MONITOR_REPLAY_SETUP_REQ         = 0x0010, /* �طŽ������� */
    IR_MONITOR_REPLAY_PLAY_REQ			= 0x0011, /* �طŲ������� */
    IR_MONITOR_REPLAY_PAUSE_REQ		    = 0x0012, /* �ط���ͣ���� */
    IR_MONITOR_REPLAY_RESUME_REQ		= 0x0013, /* �طŻָ����� */
    IR_MONITOR_REPLAY_STOP_REQ			= 0x0014, /* �ط�ֹͣ���� */
    IR_MONITOR_REPLAY_FRAME_REQ		    = 0x0015, /* �ط�֡�������� */
    IR_MONITOR_REPLAY_HEART_REQ		    = 0x0016, /* �ط��������� */
    IR_MONITOR_REPLAY_PRESET_LIST_REQ	= 0x0017, /* �ط��ļ��б����� */
    IR_MONITOR_SET_REPLAY_NUM_REQ       = 0x0018,
    IR_MONITOR_SET_RESERVE_HARD_CAPACITY_REQ = 0x0019,
    IR_MONITOR_REPLAY_PLAY_INIT_REQ     = 0x001A, /* �طŵ�֡���ų�ʼ������ */

    IR_MONITOR_GROUP_CFG_ADD_REQ = 0x0020, /* ������Ϣ�������� */
    IR_MONITOR_GROUP_CFG_DEL_REQ = 0x0021, /* ������Ϣɾ������ */
    IR_MONITOR_GROUP_CFG_CHG_REQ = 0x0022, /* ������Ϣ������� */
    IR_MONITOR_SET_CRUISE_RESTART_FLAG_REQ = 0x0023, /*����Ѳ��������־*/
    IR_MONITOR_GET_CRUISE_RESTART_FLAG_REQ = 0x0024, /*��ȡѲ��������־*/

    IR_MONITOR_IR_ANALYSE_ADD_REQ = 0x0030, /* ��������������� */
    IR_MONITOR_IR_ANALYSE_DEL_REQ = 0x0031, /* �������ɾ������ */
    IR_MONITOR_IR_FOCUS_OUT_REQ   = 0x0032, /* ���⽹���Զ���� */
    IR_MONITOR_IR_FOCUS_IN_REQ    = 0x0033, /* ���⽹��������� */
    IR_MONITOR_IR_TEMP_CHK_REQ    = 0x0034, /* �����¶�У׼���� */
    IR_MONITOR_VISI_FOCUS_INC_REQ	 = 0x0035, /* �ɼ�������������� */
    IR_MONITOR_VISI_FOCUS_INC_STOP_REQ = 0x0036, /*�ɼ����������ֹͣ����*/
    IR_MONITOR_VISI_FOCUS_DEC_REQ	 = 0x0037, /* �ɼ��ⷴ��������� */
    IR_MONITOR_VISI_FOCUS_DEC_STOP_REQ = 0x0038, /*�ɼ��ⷴ�����ֹͣ����*/
    IR_MONITOR_IR_ANALYSE_LOAD_REQ = 0x0039, /* ��������������� */
    IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_SET_REQ = 0x003a, /*˽�з���������������*/
    IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_DEL_REQ = 0x003b, /*˽�з�����������ɾ��*/
    IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_RELOAD_REQ = 0x003c, /*˽�з���������������*/

    IR_MONITOR_RAIL_CTRL_SET_REQ            = 0x0040, /* ��������Ԥ�õ����� */
    IR_MONITOR_RAIL_CTRL_CALL_BACK_REQ		= 0x0041, /* �����ٻ�ԭ������ */
    IR_MONITOR_RAIL_CTRL_DEL_PRESET_REQ	    = 0x0042, /* ����ɾ��Ԥ�õ����� */
    IR_MONITOR_RAIL_CTRL_STEP_LEFT_REQ		= 0x0043, /* ���쵥���������� */
    IR_MONITOR_RAIL_CTRL_STEP_RIGHT_REQ	    = 0x0044, /* ���쵥���������� */
    IR_MONITOR_RAIL_CTRL_LEFT_REQ			= 0x0045, /* ��������������� */
    IR_MONITOR_RAIL_CTRL_RIGHT_REQ			= 0x0046, /* ��������������� */
    IR_MONITOR_RAIL_CTRL_STOP_REQ			= 0x0047, /* ����ֹͣ���� */
    IR_MONITOR_RAIL_CTRL_TO_SET_POINT_REQ	= 0x0048, /* �������е����õ����� */
    IR_MONITOR_RAIL_CTRL_TO_DISTANCE_REQ	= 0x0049, /* �������е�ָ��λ������ */
    IR_MONITOR_RAIL_CTRL_SET_SPEED_REQ		= 0x004a, /* ���õ����ٶ����� */
    IR_MONITOR_RAIL_CTRL_CHK_INFO_REQ		= 0x004b, /* ����У׼���� */
    IR_MONITOR_RAIL_CTRL_CUR_POS_REQ		= 0x004c, /* ��ȡ���쵱ǰλ�ü��ٶ����� */

    IR_MONITOR_RAIL_CTRL_INFO_TRANS_REQ	    = 0x004f, /* ���촫����Ϣ���� */

    IR_MONITOR_GET_LIVE_COLOR_IDX_REQ		= 0x0050, /* ʵʱ�л�ȡ��ǰ���԰����� */
    IR_MONITOR_SET_LIVE_COLOR_IDX_REQ		= 0x0051, /* ʵʱ�����õ�ɫ������ */
    IR_MONITOR_SET_LIVE_ADJUST_RANGE_REQ	= 0x0052, /* ʵʱ���ֶ�����ɫ���¶ȷ�Χ */
    IR_MONITOR_SET_LIVE_ADJUST_TYPE_REQ	    = 0x0053, /* ʵʱ������ɫ�������ֶ������Զ� */
    IR_MONITOR_GET_REPLAY_COLOR_IDX_REQ	    = 0x0054, /* �ط��л�ȡ��ǰ���԰����� */
    IR_MONITOR_SET_REPLAY_COLOR_IDX_REQ	    = 0x0055, /* �ط������õ�ɫ������ */
    IR_MONITOR_SET_REPLAY_ADJUST_RANGE_REQ	= 0x0056, /* �ط����ֶ�����ɫ���¶ȷ�Χ */
    IR_MONITOR_SET_REPLAY_ADJUST_TYPE_REQ	= 0x0057, /* �ط�������ɫ�������ֶ������Զ� */
    IR_MONITOR_REPLAY_ANALYSE_ADD_REQ		= 0x0058, /* �ط������÷��� */
    IR_MONITOR_REPLAY_ANALYSE_DEL_REQ		= 0x0059, /* �ط���ɾ������ */
    IR_MONITOR_REPLAY_GET_JPG_ON_TIME_REQ	= 0x005a, /* �ط��л�ȡָ��ʱ������ */

    IR_MONITOR_PLATFORM_LEFT_REQ        	= 0x0060, /* ��̨�������� */
    IR_MONITOR_PLATFORM_RIGHT_REQ       	= 0x0061, /* ��̨�������� */
    IR_MONITOR_PLATFORM_UP_REQ        		= 0x0062, /* ��̨�������� */
    IR_MONITOR_PLATFORM_DOWN_REQ        	= 0x0063, /* ��̨�������� */
    IR_MONITOR_PLATFORM_LEFT_UP_REQ   		= 0x0064, /* ��̨���������� */
    IR_MONITOR_PLATFORM_RIGHT_UP_REQ  		= 0x0065, /* ��̨���������� */
    IR_MONITOR_PLATFORM_LEFT_DOWN_REQ  		= 0x0066, /* ��̨���������� */
    IR_MONITOR_PLATFORM_RIGHT_DAWN_REQ		= 0x0067, /* ��̨���������� */
    IR_MONITOR_PLATFORM_STOP_REQ        	= 0x0068, /* ��ֹ̨ͣ���� */
    IR_MONITOR_PLATFORM_SET_PRESET_REQ 		= 0x0069, /* ��̨����Ԥ��λ����(del,�µļ�IR_MONITOR_SET_PRESET_REQ) */
    IR_MONITOR_PLATFORM_SET_SPEED_REQ  		= 0x006a, /* ��̨�����ٶ����� */
    IR_MONITOR_PLATFORM_TO_PRESET_REQ  		= 0x006b, /* ��̨�ƶ���Ԥ�õ����� */
    IR_MONITOR_PLATFORM_DEL_PRESET_REQ 		= 0x006c, /* ��̨ɾ��Ԥ�õ����� */
    IR_MONITOR_PLATFORM_CRUISE_REQ     		= 0x006d, /* ��̨Ѳ������(del,�µļ�IR_MONITOR_START_CRUISE_REQ) */

    IR_MONITOR_GET_CRUISE_INFO_REQ          = 0x0070, /* ��ȡѲ����Ϣ���� */
    IR_MONITOR_GET_PRESET_INFO_REQ          = 0x0071, /* ��ȡ����Ԥ�õ���Ϣ���� */
    IR_MONITOR_CHANGE_CRUISE_INFO_REQ       = 0x0072, /* �������޸�Ѳ���������� */
    IR_MONITOR_DEL_CRUISE_INFO_REQ          = 0x0073, /* ɾ��Ѳ���������� */
    IR_MONITOR_SET_JPG_INFO_REQ             = 0x0074, /* ����ͼƬ���� */
    IR_MONITOR_GET_JPG_INFO_REQ             = 0x0075, /* ��ȡͼƬ���� */
    IR_MONITOR_DEL_JPG_INFO_REQ             = 0x0076, /* ɾ��ͼƬ���� */
    IR_MONITOR_START_CRUISE_REQ             = 0x0077, /* ��ʼѲ������ */
    IR_MONITOR_STOP_CRUISE_REQ              = 0x0078, /* ֹͣѲ������ */

    IR_MONITOR_SET_PRESET_REQ      			= 0x0079, /* ����Ԥ�õ����� */
    IR_MONITOR_DEL_PRESET_REQ      			= 0x007a, /* ɾ��Ԥ�õ����� */
    IR_MONITOR_TO_PRESET_REQ      			= 0x007b, /* �ƶ���Ԥ�õ����� */

    /*����Ԥ�õ�������*/
    IR_MONITOR_PRESET_ANA_ADD_REQ           = 0x007c, /*����Ԥ�õ��������*/
    IR_MONITOR_PRESET_ANA_CHG_REQ           = 0x007d,  /*change preset point analyse request*/
    IR_MONITOR_PRESET_ANA_DEL_REQ           = 0x007e,  /*del preset point analyse request*/
    IR_MONITOR_MOD_PRESET_REQ               = 0x007f,  /*change preset point position request*/

    /* �û����뼰Ȩ���޸� */
    IR_MONITOR_ADD_ACCOUNT_REQ              = 0x0080,  /*����˺�*/
    IR_MONITOR_CHG_ACCOUNT_PREMISSION_REQ   = 0x0081,  /*�޸��˺�Ȩ��*/
    IR_MONITOR_CHG_ACCOUNT_PASSWORD_REQ     = 0x0082,  /*�޸��˺�����*/
    IR_MONITOR_DEL_ACCOUNT_REQ              = 0x0083,  /*ɾ���˺�*/
    IR_MONITOR_QUERY_ACCOUNT_REQ            = 0x0084,  /*�����˺��б�*/

    IR_MONITOR_SET_ALARM_CONDITION_REQ  = 0x0090, /*����������������*/
    IR_MONITOR_GET_ALARM_CONDITION_REQ  = 0x0091, /*����������ȡ����*/
    IR_MONITOR_ALARM_PIC_LIST_REQ       = 0x0092, /*����ͼƬ�б��ȡ����*/
    IR_MONITOR_ALARM_PIC_GET_REQ        = 0x0093, /*����ͼƬ��ȡ����*/
    IR_MONITOR_RECORD_CONDITION_REQ     = 0x0094, /*����¼���ļ�¼��������������*/
    IR_MONITOR_RECORD_CONDITION_GET_REQ = 0x0095, /*����¼���ļ�¼��������ȡ����*/
    IR_MONITOR_DEL_ALARM_CONDITION_REQ  = 0x0096, /*��������ɾ������*/
    IR_MONITOR_TIMER_PIC_GET_REQ        = 0x0097, /*get pic by time*/
    IR_MONITOR_DEV_GET_CUR_PIC_REQ      = 0x0098, /* ��ȡ��ǰ�豸ͼƬ���� */

    IR_MONITOR_GET_CUR_CRUISE_INFO_REQ  = 0x0100, /* ��ȡ��ǰѲ����Ϣ���� */

    IR_MONITOR_FRAME_ANA_ADD_REQ 		= 0x0110, /* �ط���ͣʱ������������ */
    IR_MONITOR_FRAME_ANA_DEL_REQ 		= 0x0111, /* �ط���ͣʱ����ɾ������ */
    IR_MONITOR_FRAME_CLR_SET_REQ 		= 0x0112, /* �ط���ͣʱ��ɫ������ */
    IR_MONITOR_FRAME_ADJUST_RANGE_REQ 	= 0x0113, /* �ط���ͣʱɫ�귶Χ���� */
    IR_MONITOR_FRAME_ADJUST_TYPE_REQ 	= 0x0114, /* �ط���ͣʱɫ���������� */

    IR_MONITOR_FRAME_SET_RADIATION_REQ          = 0x0115, /*�ط�������ͣʱ���������*/
    IR_MONITOR_FRAME_SET_DISTANCE_REQ           = 0x0116, /*�ط�������ͣʱ�������*/
    IR_MONITOR_FRAME_SET_ENVIRONMENT_TEMP_REQ   = 0x0117, /*�ط�������ͣʱ���⻷���¶�*/
    IR_MONITOR_FRAME_SET_OPPOHITE_HUMIDITY_REQ  = 0x0118, /*�ط�������ͣʱ�������ʪ��*/

    IR_MONITOR_SET_JPG_REQ              = 0x0120, /* set pic info*/
    IR_MONITOR_SET_E_PIC_REQ     		= 0x0121, /* ���õ��ӵ�ͼͼƬ���� */
    IR_MONITOR_SET_E_PIC_INFO_REQ     	= 0x0122, /* ���õ��ӵ�ͼͼƬ��Ϣ���� */
    IR_MONITOR_GET_E_PIC_LIST_REQ     	= 0x0123, /* ��ȡ���ӵ�ͼͼƬ�б����� */
    IR_MONITOR_GET_E_PIC_INFO_REQ     	= 0x0124, /* ��ȡ���ӵ�ͼͼƬ����Ϣ���� */
    IR_MONITOR_DEL_E_PIC_REQ        	= 0x0125, /* ɾ�����ӵ�ͼͼƬ����Ϣ���� */

    IR_MONITOR_START_FIXED_POINT_CRUISE_REQ = 0x0130, /* start fixed point cruise */
    IR_MONITOR_STOP_FIXED_POINT_CRUISE_REQ = 0x131,   /* stop fixed point cruise */
    IR_MONITOR_REPLAY_FIXED_POINT_CRUISE_LIST_REQ = 0x0132, /* �طŵ���¼���ļ��б����� */
    IR_MONITOR_GET_FIXED_POINT_CRUISE_STATE_REQ = 0x0133, /* ��ȡ����Ѳ��״̬ */

    IR_MONITOR_PICTURE_SET_RADIATION_REQ = 0x0138, /*���ú���ͼ�������������*/
    IR_MONITOR_PICTURE_SET_DISTANCE_REQ = 0x0139,  /*���ú���ͼ�����������*/
    IR_MONITOR_PICTURE_SET_ENVIRONMENT_TEMP_REQ = 0x013a, /* ���ú���ͼ������������¶�*/
    IR_MONITOR_PICTURE_SET_OPPOHITE_HUMIDITY_REQ = 0x013b, /*���ú���ͼ����������ʪ��*/
    IR_MONITOR_PICTURE_GET_RADIATION_REQ = 0x013c, /*��ȡ����ͼ�������������*/
    IR_MONITOR_PICTURE_GET_DISTANCE_REQ = 0x013d,  /*��ȡ����ͼ�����������*/
    IR_MONITOR_PICTURE_GET_ENVIRONMENT_TEMP_REQ = 0x013e, /*��ȡ����ͼ������������¶�*/
    IR_MONITOR_PICTURE_GET_OPPOHITE_HUMIDITY_REQ = 0x013f, /*��ȡ����ͼ����������ʪ��*/
    IR_MONITOR_SLIDE_SET_REQ = 0x0150, /* ������������ */
    IR_MONITOR_SLIDE_DEL_REQ = 0x0151, /* ����ɾ������ */

    IR_MONITOR_RLY_SET_RADIATION_REQ            = 0x0140, /*�ط����ú���ͼ�������������*/
    IR_MONITOR_RLY_SET_DISTANCE_REQ             = 0x0141, /*�ط����ú���ͼ�����������*/
    IR_MONITOR_RLY_SET_ENVIRONMENT_TEMP_REQ     = 0x0142, /*�ط����ú���ͼ������������¶�*/
    IR_MONITOR_RLY_SET_OPPOHITE_HUMIDITY_REQ    = 0x0143, /*�ط����ú���ͼ����������ʪ��*/
    IR_MONITOR_RLY_GET_RADIATION_REQ            = 0x0144, /*�طŻ�ȡ����ͼ�������������*/
    IR_MONITOR_RLY_GET_DISTANCE_REQ             = 0x0145, /*�طŻ�ȡ����ͼ�����������*/
    IR_MONITOR_RLY_GET_ENVIRONMENT_TEMP_REQ     = 0x0146, /*�طŻ�ȡ����ͼ������������¶�*/
    IR_MONITOR_RLY_GET_OPPOHITE_HUMIDITY_REQ    = 0x0147, /*�طŻ�ȡ����ͼ����������ʪ��*/

    IR_MONITOR_GET_ANALYS_TEMP_REQ 			= 0x0160, /* �¶��������߻�ȡ���� */
    IR_MONITOR_CURVE_UP_FRAME_REQ			= 0x0165, /* �¶�����������һ֡ͼƬ���� */
    IR_MONITOR_CURVE_DOWN_FRAME_REQ			= 0x0166, /* �¶�����������һ֡ͼƬ���� */

    IR_MONITOR_GET_TIME_REQ     = 0x016f, /* ��ȡ����˵�ǰʱ������ */
    IR_MONITOR_28181_QUERY_DEVICE_INFO  = 0x0200, /* IPC�豸��Ϣ����Э��*/
    IR_MONITOR_28181_DEVICE_BOOT = 0x0204, /*�豸Զ������*/
    IR_MONITOR_28181_SEND_FRAME = 0x0205, /*��ʼ������Ƶ����Э��*/
    IR_MONITOR_28181_STOP_SEND_FRAME = 0x0206, /*ֹͣ������Ƶ����Э��*/

    IR_MONITOR_CLI_LOGOUT_NOTICE      	= 0x5000, /* �ͻ����˳�֪ͨ */
    IR_MONITOR_VID_ERR_NOTICE           = 0x5001, /* ��Ƶ�����쳣����֪ͨ */

    IR_MONITOR_ARRIVAL_PRESET_NOTICE   	= 0x5010, /* ����Ԥ�õ�֪ͨ */
    IR_MONITOR_LEAVE_PRESET_NOTICE     	= 0x5011, /* �뿪Ԥ�õ�֪ͨ */
    IR_MONITOR_CRUISE_STOP_NOTICE     	= 0x5012, /* Ѳ��ֹ֪ͣͨ */
    IR_MONITOR_CRUISE_START_NOTICE     	= 0x5013, /* Ѳ����ʼ֪ͨ */
    IR_MONITOR_DELETE_PRESET_NOTICE     = 0x5014, /* ɾ��Ԥ�õ�֪ͨ */
    IR_MONITOR_START_FIXED_POINT_CRUISE_NOTICE = 0x5015, /* start fixed point cruise notice */
    IR_MONITOR_STOP_FIXED_POINT_CRUISE_NOTICE = 0x5016, /* stop fixed point cruise notice */
    IR_MONITOR_PICTURE_SET_RADIATION_NOTICE = 0x5017, /*���ú���ͼ�����֪ͨ��������*/
    IR_MONITOR_PICTURE_SET_DISTANCE_NOTICE = 0x5018,  /*���ú���ͼ�����֪ͨ������*/
    IR_MONITOR_PICTURE_SET_ENVIRONMENT_TEMP_NOTICE = 0x5019, /* ���ú���ͼ�����֪ͨ�������¶�*/
    IR_MONITOR_PICTURE_SET_OPPOHITE_HUMIDITY_NOTICE = 0x501a, /*���ú���ͼ�����֪ͨ�����ʪ��*/
    IR_MONITOR_LINK_DISCONNECT_NOTICE  = 0x501b, /*����֪ͨ*/
    IR_MONITOR_SET_CRUISE_RESTART_FLAG_NOTICE = 0x501c, /*Ѳ��������־*/
    IR_MONITOR_SET_INFRA_RECORD_CONDITION_NOTICE = 0x501d,  /*����¼�������㲥֪ͨ*/

    IR_MONITOR_ADD_GROUP_NOTICE  =  0x5020, /*������֪ͨ*/
    IR_MONITOR_CHG_GROUP_NOTICE  =  0x5021, /*�޸���֪ͨ*/
    IR_MONITOR_DEL_GROUP_NOTICE  =  0x5022, /*ɾ����֪ͨ*/
    IR_MONITOR_GET_GROUP_CRUISE_INFO_NOTICE  =  0x5023, /*���»�ȡѲ������֪ͨ*/
    IR_MONITOR_GET_XML_NOTICE  =  0x5024, /*���»�ȡxml֪ͨ*/

    IR_MONITOR_NOTICE_CUR_DEV_PIC  =  0x5030, /* �㲥�豸��ǰͼƬ֪ͨ */
    IR_MONITOR_28181_ALARM_NOTIFY = 0x5050, /*����֪ͨ */

    IR_MONITOR_VER_NEGO_RESP = IR_MONITOR_VER_NEGO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_LINK_AUTH_RESP = IR_MONITOR_LINK_AUTH_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_LINK_HEART_RESP = IR_MONITOR_LINK_HEART_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_LINK_REL_RESP = IR_MONITOR_LINK_REL_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_XML_DATA_RESP = IR_MONITOR_XML_DATA_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VID_DATA_PLAY_RESP = IR_MONITOR_VID_DATA_PLAY_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VID_DATA_STOP_RESP = IR_MONITOR_VID_DATA_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VID_KEY_DATA_PLAY_RESP  = IR_MONITOR_VID_KEY_DATA_PLAY_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VID_KEY_DATA_STOP_RESP  = IR_MONITOR_VID_KEY_DATA_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VID_SUB_STREAM_PLAY_RESP = IR_MONITOR_VID_SUB_STREAM_PLAY_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VID_SUB_STREAM_STOP_RESP = IR_MONITOR_VID_SUB_STREAM_STOP_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_REPLAY_SETUP_RESP  		= IR_MONITOR_REPLAY_SETUP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_PLAY_RESP   		= IR_MONITOR_REPLAY_PLAY_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_PAUSE_RESP  		= IR_MONITOR_REPLAY_PAUSE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_RESUME_RESP 		= IR_MONITOR_REPLAY_RESUME_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_STOP_RESP   		= IR_MONITOR_REPLAY_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_FRAME_RESP  		= IR_MONITOR_REPLAY_FRAME_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_HEART_RESP  		= IR_MONITOR_REPLAY_HEART_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_PRESET_LIST_RESP  = IR_MONITOR_REPLAY_PRESET_LIST_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_REPLAY_NUM_RESP      = IR_MONITOR_SET_REPLAY_NUM_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_RESERVE_HARD_CAPACITY_RESP = IR_MONITOR_SET_RESERVE_HARD_CAPACITY_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_PLAY_INIT_RESP    = IR_MONITOR_REPLAY_PLAY_INIT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_GET_JPG_ON_TIME_RESP = IR_MONITOR_REPLAY_GET_JPG_ON_TIME_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_GROUP_CFG_ADD_RESP = IR_MONITOR_GROUP_CFG_ADD_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GROUP_CFG_DEL_RESP = IR_MONITOR_GROUP_CFG_DEL_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GROUP_CFG_CHG_RESP = IR_MONITOR_GROUP_CFG_CHG_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_CRUISE_RESTART_FLAG_RESP = IR_MONITOR_SET_CRUISE_RESTART_FLAG_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GET_CRUISE_RESTART_FLAG_RESP = IR_MONITOR_GET_CRUISE_RESTART_FLAG_REQ + MSG_TYPE_RESP_OFFSET,

    /* ���������������У׼ */
    IR_MONITOR_IR_ANALYSE_ADD_RESP  = IR_MONITOR_IR_ANALYSE_ADD_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_IR_ANALYSE_DEL_RESP  = IR_MONITOR_IR_ANALYSE_DEL_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_IR_FOCUS_OUT_RESP    = IR_MONITOR_IR_FOCUS_OUT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_IR_FOCUS_IN_RESP     = IR_MONITOR_IR_FOCUS_IN_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_IR_TEMP_CHK_RESP     = IR_MONITOR_IR_TEMP_CHK_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VISI_FOCUS_INC_RESP  = IR_MONITOR_VISI_FOCUS_INC_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VISI_FOCUS_DEC_RESP  = IR_MONITOR_VISI_FOCUS_DEC_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VISI_FOCUS_INC_STOP_RESP = IR_MONITOR_VISI_FOCUS_INC_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_VISI_FOCUS_DEC_STOP_RESP = IR_MONITOR_VISI_FOCUS_DEC_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_IR_ANALYSE_LOAD_RESP = IR_MONITOR_IR_ANALYSE_LOAD_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_SET_RESP = IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_SET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_DEL_RESP = IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_DEL_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_RELOAD_RESP = IR_MONITOR_PRIVATE_ANALYSE_ALARM_CONDITION_RELOAD_REQ + MSG_TYPE_RESP_OFFSET,

    /* ����ظ� */
    IR_MONITOR_RAIL_CTRL_SET_RESP           = IR_MONITOR_RAIL_CTRL_SET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_CALL_BACK_RESP     = IR_MONITOR_RAIL_CTRL_CALL_BACK_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_DEL_PRESET_RESP    = IR_MONITOR_RAIL_CTRL_DEL_PRESET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_STEP_LEFT_RESP     = IR_MONITOR_RAIL_CTRL_STEP_LEFT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_STEP_RIGHT_RESP    = IR_MONITOR_RAIL_CTRL_STEP_RIGHT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_LEFT_RESP		    = IR_MONITOR_RAIL_CTRL_LEFT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_RIGHT_RESP         = IR_MONITOR_RAIL_CTRL_RIGHT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_STOP_RESP		    = IR_MONITOR_RAIL_CTRL_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_TO_SET_POINT_RESP	= IR_MONITOR_RAIL_CTRL_TO_SET_POINT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_TO_DISTANCE_RESP	= IR_MONITOR_RAIL_CTRL_TO_DISTANCE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_SET_SPEED_RESP		= IR_MONITOR_RAIL_CTRL_SET_SPEED_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_CHK_INFO_RESP		= IR_MONITOR_RAIL_CTRL_CHK_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RAIL_CTRL_CUR_POS_RESP		= IR_MONITOR_RAIL_CTRL_CUR_POS_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_RAIL_CTRL_INFO_TRANS_RESP	= IR_MONITOR_RAIL_CTRL_INFO_TRANS_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_GET_LIVE_COLOR_IDX_RESP		= IR_MONITOR_GET_LIVE_COLOR_IDX_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_LIVE_COLOR_IDX_RESP		= IR_MONITOR_SET_LIVE_COLOR_IDX_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_LIVE_ADJUST_RANGE_RESP   = IR_MONITOR_SET_LIVE_ADJUST_RANGE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_LIVE_ADJUST_TYPE_RESP    = IR_MONITOR_SET_LIVE_ADJUST_TYPE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GET_REPLAY_COLOR_IDX_RESP    = IR_MONITOR_GET_REPLAY_COLOR_IDX_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_REPLAY_COLOR_IDX_RESP    = IR_MONITOR_SET_REPLAY_COLOR_IDX_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_REPLAY_ADJUST_RANGE_RESP	= IR_MONITOR_SET_REPLAY_ADJUST_RANGE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_REPLAY_ADJUST_TYPE_RESP	= IR_MONITOR_SET_REPLAY_ADJUST_TYPE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_ANALYSE_ADD_RESP	    = IR_MONITOR_REPLAY_ANALYSE_ADD_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_REPLAY_ANALYSE_DEL_RESP	    = IR_MONITOR_REPLAY_ANALYSE_DEL_REQ + MSG_TYPE_RESP_OFFSET,

    /* ��̨�ظ�ID */
    IR_MONITOR_PLATFORM_LEFT_RESP       	= IR_MONITOR_PLATFORM_LEFT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_RIGHT_RESP     	    = IR_MONITOR_PLATFORM_RIGHT_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_UP_RESP      	    = IR_MONITOR_PLATFORM_UP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_DOWN_RESP       	= IR_MONITOR_PLATFORM_DOWN_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_LEFT_UP_RESP  	    = IR_MONITOR_PLATFORM_LEFT_UP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_RIGHT_UP_RESP  	    = IR_MONITOR_PLATFORM_RIGHT_UP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_LEFT_DOWN_RESP 	    = IR_MONITOR_PLATFORM_LEFT_DOWN_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_RIGHT_DAWN_RESP     = IR_MONITOR_PLATFORM_RIGHT_DAWN_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_STOP_RESP       	= IR_MONITOR_PLATFORM_STOP_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_SET_PRESET_RESP     = IR_MONITOR_PLATFORM_SET_PRESET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_SET_SPEED_RESP 	    = IR_MONITOR_PLATFORM_SET_SPEED_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_TO_PRESET_RESP      = IR_MONITOR_PLATFORM_TO_PRESET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_DEL_PRESET_RESP     = IR_MONITOR_PLATFORM_DEL_PRESET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PLATFORM_CRUISE_RESP    	    = IR_MONITOR_PLATFORM_CRUISE_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_GET_CRUISE_INFO_RESP     = IR_MONITOR_GET_CRUISE_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GET_PRESET_INFO_RESP     = IR_MONITOR_GET_PRESET_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_CHANGE_CRUISE_INFO_RESP  = IR_MONITOR_CHANGE_CRUISE_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_DEL_CRUISE_INFO_RESP     = IR_MONITOR_DEL_CRUISE_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_SET_JPG_INFO_RESP        = IR_MONITOR_SET_JPG_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GET_JPG_INFO_RESP        = IR_MONITOR_GET_JPG_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_DEL_JPG_INFO_RESP        = IR_MONITOR_DEL_JPG_INFO_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_START_CRUISE_RESP        = IR_MONITOR_START_CRUISE_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_STOP_CRUISE_RESP         = IR_MONITOR_STOP_CRUISE_REQ + MSG_TYPE_RESP_OFFSET,

    IR_MONITOR_SET_PRESET_RESP  	 	= IR_MONITOR_SET_PRESET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_DEL_PRESET_RESP  	 	= IR_MONITOR_DEL_PRESET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_TO_PRESET_RESP  	 	    = IR_MONITOR_TO_PRESET_REQ + MSG_TYPE_RESP_OFFSET,

    /*����Ԥ�õ�������*/
    IR_MONITOR_PRESET_ANA_ADD_RESP      = IR_MONITOR_PRESET_ANA_ADD_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PRESET_ANA_CHG_RESP      = IR_MONITOR_PRESET_ANA_CHG_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_PRESET_ANA_DEL_RESP      = IR_MONITOR_PRESET_ANA_DEL_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_MOD_PRESET_RESP          = IR_MONITOR_MOD_PRESET_REQ + MSG_TYPE_RESP_OFFSET,

    /* �û����뼰Ȩ���޸� */
    IR_MONITOR_ADD_ACCOUNT_RESP            = IR_MONITOR_ADD_ACCOUNT_REQ + 0x8000,
    IR_MONITOR_CHG_ACCOUNT_PREMISSION_RESP = IR_MONITOR_CHG_ACCOUNT_PREMISSION_REQ + 0x8000,
    IR_MONITOR_CHG_ACCOUNT_PASSWORD_RESP   = IR_MONITOR_CHG_ACCOUNT_PASSWORD_REQ + 0x8000,
    IR_MONITOR_DEL_ACCOUNT_RESP            = IR_MONITOR_DEL_ACCOUNT_REQ + 0x8000,
    IR_MONITOR_QUERY_ACCOUNT_RESP          = IR_MONITOR_QUERY_ACCOUNT_REQ + 0x8000,

    /*alarm condition*/
    IR_MONITOR_SET_ALARM_CONDITION_RESP  = IR_MONITOR_SET_ALARM_CONDITION_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GET_ALARM_CONDITION_RESP  = IR_MONITOR_GET_ALARM_CONDITION_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_ALARM_PIC_LIST_RESP       = IR_MONITOR_ALARM_PIC_LIST_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_ALARM_PIC_GET_RESP        = IR_MONITOR_ALARM_PIC_GET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RECORD_CONDITION_RESP     = IR_MONITOR_RECORD_CONDITION_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_RECORD_CONDITION_GET_RESP = IR_MONITOR_RECORD_CONDITION_GET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_DEL_ALARM_CONDITION_RESP  = IR_MONITOR_DEL_ALARM_CONDITION_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_TIMER_PIC_GET_RESP        = IR_MONITOR_TIMER_PIC_GET_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_DEV_GET_CUR_PIC_RESP      = IR_MONITOR_DEV_GET_CUR_PIC_REQ + MSG_TYPE_RESP_OFFSET,

    /* ��ȡ��ǰѲ����Ϣ���� */
    IR_MONITOR_GET_CUR_CRUISE_INFO_RESP = IR_MONITOR_GET_CUR_CRUISE_INFO_REQ + 0x8000,

    IR_MONITOR_FRAME_ANA_ADD_RESP 		= IR_MONITOR_FRAME_ANA_ADD_REQ + 0x8000,
    IR_MONITOR_FRAME_ANA_DEL_RESP 		= IR_MONITOR_FRAME_ANA_DEL_REQ + 0x8000,
    IR_MONITOR_FRAME_CLR_SET_RESP 		= IR_MONITOR_FRAME_CLR_SET_REQ + 0x8000,
    IR_MONITOR_FRAME_ADJUST_RANGE_RESP 	= IR_MONITOR_FRAME_ADJUST_RANGE_REQ + 0x8000,
    IR_MONITOR_FRAME_ADJUST_TYPE_RESP 	= IR_MONITOR_FRAME_ADJUST_TYPE_REQ + 0x8000,

    IR_MONITOR_SET_JPG_RESP             = IR_MONITOR_SET_JPG_REQ + 0x8000,
    IR_MONITOR_SET_E_PIC_RESP     		= IR_MONITOR_SET_E_PIC_REQ + 0x8000,
    IR_MONITOR_SET_E_PIC_INFO_RESP     	= IR_MONITOR_SET_E_PIC_INFO_REQ + 0x8000,
    IR_MONITOR_GET_E_PIC_LIST_RESP     	= IR_MONITOR_GET_E_PIC_LIST_REQ + 0x8000,
    IR_MONITOR_GET_E_PIC_INFO_RESP     	= IR_MONITOR_GET_E_PIC_INFO_REQ + 0x8000,
    IR_MONITOR_DEL_E_PIC_RESP     		= IR_MONITOR_DEL_E_PIC_REQ + 0x8000,


    IR_MONITOR_START_FIXED_POINT_CRUISE_RESP = IR_MONITOR_START_FIXED_POINT_CRUISE_REQ + 0x8000,
    IR_MONITOR_STOP_FIXED_POINT_CRUISE_RESP = IR_MONITOR_STOP_FIXED_POINT_CRUISE_REQ + 0x8000,
    IR_MONITOR_REPLAY_FIXED_POINT_CRUISE_LIST_RESP = IR_MONITOR_REPLAY_FIXED_POINT_CRUISE_LIST_REQ + MSG_TYPE_RESP_OFFSET,
    IR_MONITOR_GET_FIXED_POINT_CRUISE_STATE_RESP = IR_MONITOR_GET_FIXED_POINT_CRUISE_STATE_REQ + 0x8000,

    IR_MONITOR_PICTURE_SET_RADIATION_RESP = IR_MONITOR_PICTURE_SET_RADIATION_REQ + 0x8000,
    IR_MONITOR_PICTURE_SET_DISTANCE_RESP = IR_MONITOR_PICTURE_SET_DISTANCE_REQ + 0x8000,
    IR_MONITOR_PICTURE_SET_ENVIRONMENT_TEMP_RESP = IR_MONITOR_PICTURE_SET_ENVIRONMENT_TEMP_REQ + 0x8000,
    IR_MONITOR_PICTURE_SET_OPPOHITE_HUMIDITY_RESP = IR_MONITOR_PICTURE_SET_OPPOHITE_HUMIDITY_REQ + 0x8000,
    IR_MONITOR_PICTURE_GET_RADIATION_RESP = IR_MONITOR_PICTURE_GET_RADIATION_REQ + 0x8000,
    IR_MONITOR_PICTURE_GET_DISTANCE_RESP = IR_MONITOR_PICTURE_GET_DISTANCE_REQ + 0x8000,
    IR_MONITOR_PICTURE_GET_ENVIRONMENT_TEMP_RESP = IR_MONITOR_PICTURE_GET_ENVIRONMENT_TEMP_REQ + 0x8000,
    IR_MONITOR_PICTURE_GET_OPPOHITE_HUMIDITY_RESP = IR_MONITOR_PICTURE_GET_OPPOHITE_HUMIDITY_REQ + 0x8000,
    IR_MONITOR_SLIDE_SET_RESP = IR_MONITOR_SLIDE_SET_REQ + 0x8000,
    IR_MONITOR_SLIDE_DEL_RESP = IR_MONITOR_SLIDE_DEL_REQ + 0x8000,

    IR_MONITOR_FRAME_SET_RADIATION_RESP         = IR_MONITOR_FRAME_SET_RADIATION_REQ + 0x8000,
    IR_MONITOR_FRAME_SET_DISTANCE_RESP          = IR_MONITOR_FRAME_SET_DISTANCE_REQ + 0x8000,
    IR_MONITOR_FRAME_SET_ENVIRONMENT_TEMP_RESP  = IR_MONITOR_FRAME_SET_ENVIRONMENT_TEMP_REQ + 0x8000,
    IR_MONITOR_FRAME_SET_OPPOHITE_HUMIDITY_RESP = IR_MONITOR_FRAME_SET_OPPOHITE_HUMIDITY_REQ + 0x8000,
    IR_MONITOR_RLY_SET_RADIATION_RESP         = IR_MONITOR_RLY_SET_RADIATION_REQ+0x8000,
    IR_MONITOR_RLY_SET_DISTANCE_RESP          = IR_MONITOR_RLY_SET_DISTANCE_REQ+0x8000,
    IR_MONITOR_RLY_SET_ENVIRONMENT_TEMP_RESP  = IR_MONITOR_RLY_SET_ENVIRONMENT_TEMP_REQ+0x8000,
    IR_MONITOR_RLY_SET_OPPOHITE_HUMIDITY_RESP = IR_MONITOR_RLY_SET_OPPOHITE_HUMIDITY_REQ+0x8000,
    IR_MONITOR_RLY_GET_RADIATION_RESP         = IR_MONITOR_RLY_GET_RADIATION_REQ+0x8000,
    IR_MONITOR_RLY_GET_DISTANCE_RESP          = IR_MONITOR_RLY_GET_DISTANCE_REQ+0x8000,
    IR_MONITOR_RLY_GET_ENVIRONMENT_TEMP_RESP  = IR_MONITOR_RLY_GET_ENVIRONMENT_TEMP_REQ+0x8000,
    IR_MONITOR_RLY_GET_OPPOHITE_HUMIDITY_RESP = IR_MONITOR_RLY_GET_OPPOHITE_HUMIDITY_REQ+0x8000,

    IR_MONITOR_GET_ANALYS_TEMP_RESP		    = IR_MONITOR_GET_ANALYS_TEMP_REQ+0x8000,
    IR_MONITOR_CURVE_UP_FRAME_RESP		    = IR_MONITOR_CURVE_UP_FRAME_REQ+0x8000,
    IR_MONITOR_CURVE_DOWN_FRAME_RESP		= IR_MONITOR_CURVE_DOWN_FRAME_REQ+0x8000,

    IR_MONITOR_GET_TIME_RESP   = IR_MONITOR_GET_TIME_REQ+0x8000,

    IR_MONITOR_28181_QUERY_DEVICE_INFO_ACK  = IR_MONITOR_28181_QUERY_DEVICE_INFO + 0x8000,
    IR_MONITOR_28181_DEVICE_BOOT_ACK = IR_MONITOR_28181_DEVICE_BOOT + 0x8000,
    IR_MONITOR_28181_SEND_FRAME_ACK = IR_MONITOR_28181_SEND_FRAME + 0x8000,
    IR_MONITOR_28181_STOP_SEND_FRAME_ACK = IR_MONITOR_28181_STOP_SEND_FRAME + 0x8000,

    IR_MONITOR_MSG_TYPE_BUTT = 0xFFFF
}IR_MONITOR_MSG_TYPE_E;

#pragma pack(push)
#pragma pack(1)
typedef struct tagMsgHeader_S{
    UINT32   magic_type;
    UINT32   msg_type;
    UINT32   total_len;
    UINT32   seq_id;
    INT32    i_ret;
    UINT8    auc_data[0];
    tagMsgHeader_S()
    {
        magic_type = 0;
        msg_type = 0;
        total_len = 0;
        seq_id = 0;
        i_ret = 0;
    }
}MsgHeader_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagMSAlarmPic_S
{
    UINT32  ui_pic_class;//1:if, 2:visiblelight
    std::string pic_data;//
}MSAlarmPic_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveUp_S{
    UINT32 ui_frame_type;
    std::string str_frame;
}CurveUp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveDown_S{
    UINT32 ui_frame_type;
    std::string str_frame;
}CurveDown_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDevCurPicData_S {
    std::string s_sn;
    std::string s_pic_data;
}DevCurPicData_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDevDataInfo_S {
    std::string  str_dev_name;
    UINT32       ui_dev_type;                /* 0��ʾ�ɼ��⣬1��ʾ���� */
    std::string  str_dev_manufacturers;      /* �豸���� */
    std::string  str_dev_model;              /* �豸�ͺ� */
    std::string  str_dev_owner;              /* �豸���� */
    std::string  str_dev_administrative_area;/* �������� */
    std::string  str_precinct;               /* ���� */
    std::string  str_dev_install_address;
    UINT32       ui_sub_dev;                 /* �Ƿ������豸.��1��û��0 */
    UINT32       ui_singal_safe_mode;        /* ���ȫģʽ */
    UINT32       ui_registry_mode;           /* ע�᷽ʽ */
    std::string  str_certificate_serial_num; /* ֤�����к� */
    UINT32       ui_certificate_valid;       /* ֤����Ч��ʶ */
    UINT32       ui_invalid_reason_code;     /* ��Чԭ���� */
    std::string  str_certificate_termination_valid;/* ֤����ֹ��Ч�� */
    UINT32       ui_confidentiality_properties;    /* �������� */
    std::string  str_ip_address;
    UINT32       ui_port;                    /* �豸/����/ϵͳ�˿ں� */
    std::string  str_dev_pwd;                /* �豸���� */
    UINT32       ui_dev_stat;                /* �豸״̬ */
    float        f_longitude;                /* ���� */
    float        f_latitude;                 /* γ�� */
    std::string  str_dev_firmware_version;   /* �豸�̼��汾 */
    UINT32       ui_vid_input_num;           /* ��Ƶ����ͨ������������1 ����ѡ��*/
}DevDataInfo_S;
#pragma pack(pop)

/* �ط���Ƶ����֪ͨ��Ϣ */
#pragma pack(push)
#pragma pack(1)
typedef struct tagVodStopNotice_S{
    MsgHeader_S st_head;
    INT32  i_errno;
    UINT32 ui_session;
}VodStopNotice_S;
#pragma pack(pop)


/* �汾��֤ */
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32    ui_major_ver;
    UINT32    ui_sub_ver;
}VerReq_S;
#pragma pack(pop)

//#pragma pack(push)
//#pragma pack(1)
//typedef struct {
//    MsgHeader_S header;
//}CommReq_S;
//#pragma pack(pop)

typedef MsgHeader_S  CommReq_S;

//#pragma pack(push)
//#pragma pack(1)
//typedef struct tagCommResp{
//    MsgHeader_S header;
//    INT32 i_ret;
//    tagCommResp() {
//        header.magic_type = MSG_HEAD_MAGIC;
//        header.msg_type = 0;
//        header.total_len = 0;
//        header.seq_id = 0;
//        i_ret = -1;
//    }
//}CommResp_S;
//#pragma pack(pop)

typedef MsgHeader_S  CommResp_S;

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetAllUsrResp_S{
    MsgHeader_S st_header;
    INT32 i_ret;
    INT32 i_usr_num;
    UINT8 aucData[0];
}GetAllUsrResp_S;
#pragma pack(pop)

/* �û���Ȩ */
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT8      ac_login_id[32];
    UINT8     auc_digest[16];
    UINT32    ui_time_stamp;
    UINT32    ui_rand;
    UINT32    ui_client_type;
}AuthReq_S;

typedef struct {
    MsgHeader_S header;
    INT32 i_ret;
    UINT32 ui_client_id;
    INT32  i_client_type;
    UINT64 ul_out_dev_premission;
    UINT32 ui_group_count;
    UINT8 auc_data[0];
    //UINT32 ui_client_level;
}AuthResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 client_id;
}XmlReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    INT8 xml_buf[1024*100];
}XmlResp;
#pragma pack(pop)

typedef MsgHeader_S HeartBeatResp;

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 ui_client_id;
}HeartBeatReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT8 device_sn[128];
}VodPlayReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    INT8 url_buf[1024];
}VodPlayResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT8 device_sn[128];
}VodStopReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}VodStopResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 cmd;
    UINT32 val;
}PlatformCtrlReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}PlatformCtrlResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 num;
    UINT32 ui_member_len;
    INT8 members[1024];
    UINT32 alias_len;
    INT8 alias[1024];
}GroupCfgAddReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 id;
}GroupCfgAddResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 id;
}GroupCfgDelReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}GroupCfgDelResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 id;
    UINT32 num;
    UINT32 ui_member_len;
    INT8 members[1024];
    UINT32 alias_len;
    INT8 alias[1024];
}GroupCfgChgReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}GroupCfgChgResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 ui_group;
    UINT32 ui_restart_flag;
    UINT32 ui_time;
}SetCruiseRestartFlagReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 ui_result;
}SetCruiseRestartFlagResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 ui_group;
}GetCruiseRestartFlagReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 ui_result;
    UINT32 ui_restart_flag;
    UINT32 ui_time;
}GetCruiseRestartFlagResp;
#pragma pack(pop)

////////////// REPLAY ////////////////
#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S st_head;
    UINT32 ui_session;
    UINT32 ui_group;
    UINT32 ui_second;
    UINT32 ui_msecond;
}ReplayGetJpgOnTimeReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagReplayPicAttr_S{
    UINT32 ui_pic_type; // 1��ʾ���⣬2��ʾ�ɼ���
    UINT32 ui_pic_len;
    UINT8  auc_data[0];
}ReplayPicAttr_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S st_head;
    INT32 i_ret;
    UINT32 ui_pic_num;
    ReplayPicAttr_S st_pic_attr;
}ReplayGetJpgOnTimeResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 ui_cname_len;
    INT8   ac_cname[MONITOR_CNAME_LEN];
}ReplaySetupReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagReplaySetupRespBodyHead_S{
    MsgHeader_S header;
    INT32 result;
    UINT32 session;
    UINT32 num;
}ReplaySetupRespBodyHead_S;

typedef struct {
    ReplaySetupRespBodyHead_S st_Setup_resp_body_head;
    INT8 urls[2048];
}ReplaySetupResp;
#pragma pack(pop)

#define REPLAY_TIME_LEN 14

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32 ui_session;
    UINT32 ui_group;
    UINT16 us_slide;
    UINT16 us_ptz;
    UINT32 ui_speed;
    INT8 begin[REPLAY_TIME_LEN];
    INT8 end[REPLAY_TIME_LEN];
}ReplayPlayReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S st_head;
    INT32 result;
    UINT32 session;
}ReplayPlayResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32 session;
}ReplayPauseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S st_head;
    INT32 result;
    UINT32 session;
}ReplayCommResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32 session;
}ReplayResumeReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 session;
}ReplayResumeResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32 session;
}ReplayStopReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 session;
}ReplayStopResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32 session;
    UINT32 frameno;
}ReplayFrameReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 session;
}ReplayFrameResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S st_head;
    UINT32      ui_session;
}ReplayHeartBeat;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 ctl;
    UINT32 session;
    UINT16 slide;
    UINT16 ptz;
    INT8 begin[REPLAY_TIME_LEN];
    INT8 end[REPLAY_TIME_LEN];
}ReplayFilesReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 ctl;
    UINT32 session;
    UINT32 filesnum;
    UINT8 fileinfo[0];
}ReplayFilesResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 num;
}ReplaySetNumReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 capacity;
}SetReserveHardCapacityReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 session;
    UINT32 group;
    UINT16 slide;
    UINT16 ptz;
    INT8 begin[REPLAY_TIME_LEN];
    INT8 end[REPLAY_TIME_LEN];
    UINT32 alias_len;
    INT8 alias[1024];
}ReplayPresetListReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 session;
    UINT32 group;
    INT8 begin[REPLAY_TIME_LEN];
    INT8 end[REPLAY_TIME_LEN];
}ReplayPointListReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}ReplaySetNumResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    INT32 session;
    INT32 filesnum;
    INT32 type;
    INT8 fileinfo[10*10*1024];
}ReplayPresetListResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    INT32 session;
    INT32 filesnum;
    INT32 type;
    INT8 fileinfo[10*10*1024];
}ReplayPointListResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}SetReserveHardCapacityResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    std::string sn;
    std::map<UINT32, std::string> str_ana_map;
}AddPrivateAnalyseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    std::string sn;
    std::set<UINT32> ana_no_set;
}DelPrivateAnalyseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}PrivateAnalyseResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    std::string str_sn;
    std::map<UINT32, std::string> str_condition_map;
}AddPrivateAlarmConditionReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct{
    MsgHeader_S header;
    std::string str_sn;
    std::set<UINT32> ui_condition_set;
}DelPrivateAlarmConditionReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct{
    MsgHeader_S header;
    UINT32 ui_result;
}PrivateAlarmConditionResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S st_head;
    UINT32 ui_session;
    UINT32 ui_frame_no;
    UINT32 ui_ana_len;
    INT8 ac_ana[0];
}ReplayAnaReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
}GetCruiseInfoReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT16 slide;
    UINT16 ptz;
    UINT32 second;
}CmdPreset;
typedef struct {
    UINT16 if_cur;
    UINT16 no;
    UINT32 alias_len;
    INT8 alias[256];
    UINT32 num;
    CmdPreset presets[64];
}CmdCruise;
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 num;
    CmdCruise cruises[256];
}GetCruiseInfoResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
}GetPresetInfoReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
//typedef struct {
    //UINT16 ptz;
    //UINT16 alias_len;
    //INT8 alias[256];
//}CmdPtzPreset;
//typedef struct {
    //UINT16 slide;
    //UINT16 ptz_num;
    //UINT32 alias_len;
    //INT8 alias[256];
    //CmdPtzPreset ptzs[64];
//}CmdSlidePreset;
typedef struct {
    UINT16 slide;
    UINT16 ptz;
    UINT32 alias_len;
    INT8 alias[256];
}CmdSlidePreset;

typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 num;
    UINT8 auc_data[0];
}GetPresetInfoResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 no;
    UINT32 alias_len;
    INT8 alias[256];
    UINT32 num;
    CmdPreset presets[64];
}SetCruiseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 no;
}SetCruiseResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 no;
}DelCruiseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}DelCruiseResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT16 slide;
    UINT16 ptz;
    UINT16 x;
    UINT16 y;
}PicPreset;
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 num;
    PicPreset presets[256];
    //UINT32 pic_len;
    //UINT8 *p_pic;
}SetPicReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}SetPicResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 pic_len;
    UINT8 *p_pic;
}SetPicInfoReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}SetPicInfoResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
}GetPicReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
    UINT32 num;
    PicPreset presets[64];
    UINT32 pic_len;
    UINT8 pic[0];
}GetPicResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
}DelPicReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}DelPicResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 no;
}StartCruiseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}StartCruiseResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    UINT32 group;
    UINT32 no;
}StopCruiseReq;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    MsgHeader_S header;
    INT32 result;
}StopCruiseResp;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagAddUserReq_S{
    MsgHeader_S st_header;
    INT32  i_name_len;
    INT8   ac_name[128];
    UINT8  auc_digest[16];
    INT32  i_client_type;
    UINT32 ui_client_level;
}AddUserReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDelUserReq_S{
    MsgHeader_S st_header;
    INT32  i_name_len;
    INT8   ac_name[128];
}DelUserReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagChgUserPwdReq_S{
    MsgHeader_S st_header;
    INT32  i_name_len;
    INT8   ac_name[128];
    UINT8  auc_digest[16];
}ChgUserPwdReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagChgUserLevelReq_S{
    MsgHeader_S st_header;
    INT32  i_name_len;
    INT8   ac_name[128];
    INT32  i_client_type;
    UINT32 ui_client_level;
}ChgUserLevelReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagQueryAllUserReq_S{
    MsgHeader_S st_header;
}QueryAllUserReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
/* ��ɫ���ɫ����Ϣ�ṹ�� */
/* ʵʱ��ɫ����Ϣ */
typedef struct tagLiveGetClrIdxReq_S{
    MsgHeader_S header;
    INT8 acSn[128];
}LiveGetClrIdxReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagLiveSetClrIdxReq_S{
    MsgHeader_S header;
    INT32 i_color_idx;
    INT8 acSn[128];
}LiveSetClrIdxReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagLiveSetAdjustRangeReq_S{
    MsgHeader_S header;
    float f_max;
    float f_min;
    INT8 acSn[128];
}LiveSetAdjustRangeReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagLiveSetAdjustTypeReq_S{
    MsgHeader_S header;
    INT32 i_adjust_type;
    INT8 acSn[128];
}LiveSetAdjustTypeReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
/* �طŵ�ɫ����Ϣ */
typedef struct tagReplayGetClrIdxReq_S{
    MsgHeader_S header;
    UINT32 ui_session;
}ReplayGetClrIdxReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagReplaySetClrIdxReq_S{
    MsgHeader_S header;
    INT32 i_color_idx;
    UINT32 ui_session;
    UINT32 ui_frame_no;
}ReplaySetClrIdxReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagReplaySetAdjustRangeReq_S{
    MsgHeader_S header;
    float f_max;
    float f_min;
    UINT32 ui_session;
    UINT32 ui_frame_no;
}ReplaySetAdjustRangeReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagReplaySetAdjustTypeReq_S{
    MsgHeader_S header;
    INT32  i_adjust_type;
    UINT32 ui_session;
    UINT32 ui_frame_no;
}ReplaySetAdjustTypeReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetClrIdxResp_S{
    MsgHeader_S header;
    INT32 i_ret;
    INT32 i_clr_idx;
}GetClrIdxResp_S;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
/************* ���콻����Ϣ *************/
typedef struct tagRailDelPresetReq_S{
    MsgHeader_S header;
    INT8   ac_rail_sn[32];
    INT32  i_sn_len;
    UINT32 ui_set_point;
}RailDelPresetReq_S;

typedef struct tagRailDelPresetResp_S{
    MsgHeader_S header;
    INT32  i_ret;
}RailDelPresetResp_S;
/****************************************/
typedef struct tagRailSetReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
    UINT32 ui_set_point;
    UINT32 ui_cname_len;
    INT8   ac_cname[128];
}RailSetReq_S;

typedef struct tagRailSetResp_S{
    MsgHeader_S header;
    INT32  i_ret;
    UINT32 ui_set_point;
}RailSetResp_S;

typedef struct tagRailCallBackReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
}RailCallBackReq_S;

typedef struct tagRailCallBackResp_S{
    MsgHeader_S header;
    INT32  i_ret;
}RailCallBackResp_S;

typedef struct tagRailDirectCommReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
}RailDirectCommReq_S;

typedef struct tagPtzDirectCommReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_ptz_sn[64];
}PtzDirectCommReq_S;

typedef struct tagRailDirectCommResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailDirectCommResp_S;

typedef struct tagPtzDirectCommResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}PtzDirectCommResp_S;

typedef struct tagRailStepRightReq_S{
    MsgHeader_S header;
    UINT32 ui_group_num;
}RailStepRightReq_S;

typedef struct tagRailStepRightResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailStepRightResp_S;

typedef struct tagRailLeftReq_S{
    MsgHeader_S header;
    UINT32 ui_group_num;
}RailLeftReq_S;

typedef struct tagRailLeftResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailLeftResp_S;

typedef struct tagRailRightReq_S{
    MsgHeader_S header;
    UINT32 ui_group_num;
}RailRightReq_S;

typedef struct tagRailRightResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailRightResp_S;

typedef struct tagRailStopReq_S{
    MsgHeader_S header;
    UINT32 ui_group_num;
}RailStopReq_S;

typedef struct tagRailStopResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailStopResp_S;

typedef struct tagRailToSetPointReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
    UINT32 ui_set_point;
}RailToSetPointReq_S;

typedef struct tagRailToSetPointResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailToSetPointResp_S;

typedef struct tagRailToDistanceReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
    UINT32 ui_distance;
}RailToDistanceReq_S;

typedef struct tagRailToDistanceResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailToDistanceResp_S;

typedef struct tagRailSetSpeedReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
    UINT32 ui_speed;
}RailSetSpeedReq_S;

typedef struct tagRailSetSpeedResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}RailSetSpeedResp_S;

typedef struct tagRailCalibrationReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
}RailChkReq_S;

typedef struct tagRailCalibrationResp_S{
    MsgHeader_S header;
    INT32     i_ret;
    UINT32    ui_len;
}RailChkResp_S;

typedef struct tagRailPosReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
}RailPosReq_S;

typedef struct tagRailPosResp_S{
    MsgHeader_S header;
    INT32     i_ret;
    UINT32    ui_cur_pos;
    UINT32    ui_cur_speed;
    UINT32    ui_total_len;
}RailPosResp_S;

typedef struct tagRaiTransReq_S{
    MsgHeader_S header;
    INT32  i_sn_len;
    INT8   ac_rail_sn[32];
}RaiTransReq_S;

typedef struct tagRaiTransResp_S{
    MsgHeader_S header;
    INT32     i_ret;
    UINT32    ui_data_len;
    UINT8     auc_data[0];
}RaiTransResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////
typedef struct tagDirectionReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
}DirectionReq_S;

typedef struct tagDirectionResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}DirectionResp_S;
/***********************************************/
typedef struct tagSingleAnaInfo_S{
    std::string    s_ana_cname;
    std::string    s_ana;
}SingleAnaInfo_S;

typedef struct {
    INT16 us_slide;
    INT16 us_ptz;
    std::string alias;
    std::list<SingleAnaInfo_S> ana_info_list;
}PresetPoint;

typedef struct {
    INT16 slide;
    INT16 ptz;
    UINT32 ui_cname_len;
    UINT8 auc_cname[0];
}PresetPointResp_S;

typedef struct tagPresetReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
    UINT16    us_slide_no;
    UINT16    us_ptz_no;
    std::string    s_cname;
    UINT32    ui_ana_num;
    std::list<SingleAnaInfo_S> ana_list;
}PresetReq_S;

typedef struct tagPresetResp_S{
    MsgHeader_S header;
    INT32     i_ret;
    UINT16    us_slide_no;
    UINT16    us_ptz_no;
}PresetResp_S;
/***********************************************/
typedef struct tagPlatFormPresetReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
    UINT16    ui_slide_no;
    UINT16    ui_preset_no;
    UINT32    ui_cname_len;
    INT8      auc_cname[128];
}PlatFormPresetReq_S;

typedef struct tagPlatFormPresetResp_S{
    MsgHeader_S header;
    INT32     i_ret;
    UINT16    ui_slide_no;
    UINT16    ui_preset_no;
}PlatFormPresetResp_S;
//////////////////////////////////////////
typedef struct tagPlatFormSpeedReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
    UINT32    ui_speed;
}PlatFormSpeedReq_S;

typedef struct tagPlatFormSpeedResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}PlatFormSpeedResp_S;
//////////////////////////////////////////
typedef struct tagPlatFormToPresetReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
    UINT16    us_slide_no;
    UINT16    us_preset_no;
}PlatFormToPresetReq_S;

typedef struct tagPlatFormToPresetResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}PlatFormToPresetResp_S;
//////////////////////////////////////////
typedef struct tagDelPresetReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
    UINT16    us_slide_no;
    UINT16    us_preset_no;
}DelPresetReq_S;

typedef struct tagDelPresetResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}DelPresetResp_S;
//////////////////////////////////////////
typedef struct tagPlatFormPresetInfo_S{
    UINT16    us_rail_preset;
    UINT16    us_platform_preset;
    UINT32    ui_stop_time;
}PlatFormPresetInfo_S;

typedef struct tagPlatFormCruiseReq_S{
    MsgHeader_S header;
    UINT32    ui_group_no;
    UINT32    ui_Cruise_num;
    PlatFormPresetInfo_S ast_preset_info[PRESET_MAX_NUM];
}PlatFormCruiseReq_S;

typedef struct tagPlatFormCruiseResp_S{
    MsgHeader_S header;
    INT32     i_ret;
}PlatFormCruiseResp_S;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct tagFocusChkReq_S{
    MsgHeader_S header;
    UINT32 ui_client_id;
    UINT32 sn_len;
    INT8 sn[128];
}FocusChkReq_S;
#pragma pack(pop)

// add by huhua
#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAnaAddReq_S{
    MsgHeader_S header;
    UINT32 ui_group_no;
    UINT16 ui_slide_no;
    UINT16 ui_ptz_no;
    UINT32 ui_ana_cname_len;
    std::string str_ana_cname;
    UINT32 ui_ana_len;
    std::string str_ana;
}PresetAnaAddReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAnaAddResp_S{
    MsgHeader_S header;
    INT32       i_Result;//succ:0, Err:other
    UINT32      ui_no;
}PresetAnaAddResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAnaChgReq_S{
    MsgHeader_S header;
    UINT32 ui_group_no;
    UINT16 ui_slide_no;
    UINT16 ui_ptz_no;
    UINT32 ui_preset_ana_no;
    UINT32 ui_ana_cname_len;
    std::string str_ana_cname;
    UINT32 ui_ana_len;
    std::string str_ana;
}PresetAnaChgReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAnaChgResp_S{
    MsgHeader_S header;
    INT32       i_Result;//succ:0, Err:other
}PresetAnaChgResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAnaDelReq_S{
    MsgHeader_S header;
    UINT32 ui_group_no;
    UINT16 ui_slide_no;
    UINT16 ui_ptz_no;
    UINT32 ui_preset_ana_no;
}PresetAnaDelReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAnaDelResp_S{
    MsgHeader_S header;
    INT32       i_Result;//succ:0, Err:other
}PresetAnaDelResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagModPresetReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      ui_slide;
    UINT16      ui_ptz;
}ModPresetReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagModPresetResp{
    MsgHeader_S header;
    INT32       i_Result; //succ:0, Err:other
}ModPresetResp_S;
#pragma pack(pop)

//IR_MONITOR_SET_ALARM_CONDITION_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagSetAlarmConditionReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_alarm_no;
    UINT32      ui_cname_len;
    std::string      str_cname;
    UINT32      ui_alarm_len;
    std::string      str_alarm_condition;
}SetAlarmConditionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSetAlarmConditionResp{
    MsgHeader_S header;
    INT32       i_Result;
    UINT32      ui_alarm_no;
}SetAlarmConditionResp_S;
#pragma pack(pop)

//IR_MONITOR_GET_ALARM_CONDITION_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagGetAlarmConditionReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_alarm_no;// 0: all, other: one
}GetAlarmConditionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32 ui_alarm_no;
    UINT32 ui_cname_len;
    INT8   auc_data[0];//sample as:
                       //string str_cname;
                       //UINT32 ui_alarm_len;
                       //string str_alarm_condition;
}AlarmCondition_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetAlarmConditionResp{
    MsgHeader_S header;
    INT32       i_Result;
    UINT32      ui_alarm_num;
    INT8        auc_data[0]; //AlarmCondition_S
}GetAlarmConditionResp_S;
#pragma pack(pop)

//IR_MONITOR_ALARM_PIC_LIST_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagAlarmPicListReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    std::string      str_begin;
    std::string      str_end;
    UINT32      ui_alarm_no_count;//0:all
    std::set<UINT32> ui_alarm_no_list;
}AlarmPicListReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32  ui_second;
    UINT32  ui_millisecond;
    UINT32  ui_alarm_num;
    INT8    auc_data[0];//sample as:
                        //SetAlarmConditionResp_S.ui_alarm_no list
}AlarmPicNo_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagAlarmPicListResp{
    MsgHeader_S header;
    INT32       i_Result;
    UINT32      ui_Pic_num;
    INT8        auc_data[0];//AlarmPicNo_S
}AlarmPicListResp_S;
#pragma pack(pop)

//IR_MONITOR_ALARM_PIC_GET_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagAlarmPicGetReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_alarm_no;
    UINT32      ui_utc_sec;
    UINT32      ui_utc_millsec;
}AlarmPicGetReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32  ui_pic_class;//1:if, 2:visiblelight
    UINT32  ui_pic_len;
    UINT8   auc_data[0];//pic data
}AlarmPicData_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagAlarmPicGetResp{
    MsgHeader_S header;
    INT32       i_Result;
    INT32       ui_pic_num;
    INT8        auc_data[0];
}AlarmPicGetResp_S;
#pragma pack(pop)

//IR_MONITOR_TIMER_PIC_GET_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagTimerPicGetReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT32      ui_utc_sec;
    UINT32      ui_utc_millsec;
}TimerPicGetReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINT32  ui_pic_class;//1:if, 2:visiblelight
    UINT32  ui_pic_len;
    UINT8   auc_data[0];//pic data
}TimerPicData_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagTimerPicGetResp{
    MsgHeader_S header;
    INT32       i_Result;
    INT32       ui_pic_num;
    INT8        auc_data[0];
}TimerPicGetResp_S;
#pragma pack(pop)

//IR_MONITOR_RECORD_CONDITION_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagSetRecordConditionReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_record_time_interval;
    float       ft_max_temp_interval;//-9999 means invalid
    float       ft_max_temp;//-9999 means invalid
}SetRecordConditionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSetRecordConditionResp{
    MsgHeader_S header;
    INT32       i_Result;
}SetRecordConditionResp_S;
#pragma pack(pop)

//IR_MONITOR_RECORD_CONDITION_GET_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagGetRecordConditionReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
}GetRecordConditionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetRecordConditionResp{
    MsgHeader_S header;
    INT32       i_Result;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_record_time_interval;
    float       ft_max_temp_interval;//-9999 means invalid
    float       ft_max_temp;//-9999 means invalid
}GetRecordConditionResp_S;
#pragma pack(pop)

//IR_MONITOR_DEL_ALARM_CONDITION_REQ
#pragma pack(push)
#pragma pack(1)
typedef struct tagDelAlarmConditionReq{
    MsgHeader_S header;
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_alarm_no;
}DelAlarmConditionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDelAlarmConditionResp{
    MsgHeader_S header;
    INT32       i_Result;
}DelAlarmConditionResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetCurCruiseInfoReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}GetCurCruiseInfoReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetCurCruiseInfoResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    UINT32 ui_cruise_no;
    UINT16 us_slide;
    UINT16 us_ptz;
    UINT32 ui_time;
}GetCurCruiseInfoResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSetEPicReq_S{
    MsgHeader_S st_head;
    UINT32      ui_pic_id;
    UINT32      ui_pic_cname_len;
    INT8        ac_pic_cname[128];
    UINT32      ui_pic_len;
    UINT8       auc_pic_data[0];
}SetEPicReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSetEPicResp_S{
    CommResp_S  st_resp;
    UINT32      ui_pic_id;
}SetEPicResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSetEPicInfoReq_S{
    MsgHeader_S st_head;
    UINT32      ui_pic_id;
    UINT32      ui_pic_info_len;
    UINT8       auc_pic_info_data[0];
}SetEPicInfoReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSetEPicInfoResp_S{
    CommResp_S  st_resp;
}SetEPicInfoResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetEPicListReq_S{
    MsgHeader_S st_head;
}GetEPicListReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetEPicListResp_S{
    CommResp_S  st_resp;
    UINT32      ui_pic_num;
    UINT8       auc_e_pic_name[0];
}GetEPicListResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagEPicName_S{
    UINT32      ui_e_pic_id;
    UINT32      ui_pic_cname_len;
    INT8        ac_pic_cname[0];
}EPicName_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetEPicInfoReq_S{
    MsgHeader_S st_head;
    UINT32      ui_e_pic_id;
}GetEPicInfoReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagEPicData_S{
    UINT32      ui_data_len;
    UINT8       auc_data[0];
}EPicData_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetEPicInfoResp_S{
    CommResp_S  st_resp_head;
    EPicData_S  st_data;
}GetEPicInfoResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDelEPicReq_S{
    MsgHeader_S st_head;
    UINT32      ui_e_pic_id;
}DelEPicReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDelEPicResp_S{
    CommResp_S  st_resp_head;
}DelEPicResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagStartFixedPointCruiseReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}StartFixedPointCruiseReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagStartFixedPointCruiseResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}StartFixedPointCruiseResp_S;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct tagStopFixedPointCruiseReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}StopFixedPointCruiseReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagStopFixedPointCruiseResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}StopFixedPointCruiseResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetFixedPointCruiseStateReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}GetFixedPointCruiseStateReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetFixedPointCruiseStateResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    UINT32 ui_state;
    UINT32 ui_time;
}GetFixedPointCruiseStateResp_S;
#pragma pack(pop)

//picture manager
//set
#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetRadiationReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    float  f_radiation;
}PicSetRadiationReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetRadiationResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}PicSetRadiationResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetDistanceReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    float  f_distance;
}PicSetDistanceReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetDistanceResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}PicSetDistanceResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetEnvTempReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    float  f_envtemp;
}PicSetEnvTempReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetEnvTempResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}PicSetEnvTempResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetOppohiteHumidityReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    float  f_humidity;
}PicSetOppohiteHumidityReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicSetOppohiteHumidityResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}PicSetOppohiteHumidityResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepSetRaDiTeHuResp_S{
    MsgHeader_S header;
    INT32 i_result;
}RepSetRaDiTeHuResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepSetRadiationReq_S{
    MsgHeader_S header;
    float f_radiation;
    UINT32 ui_session;
    UINT32 ui_frameno;
}RepSetRadiationReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepSetDistanceReq_S{
    MsgHeader_S header;
    float f_distance;
    UINT32 ui_session;
    UINT32 ui_frameno;
}RepSetDistanceReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepSetEnvTmpReq_S{
    MsgHeader_S header;
    float f_envtmp;
    UINT32 ui_session;
    UINT32 ui_frameno;
}RepSetEnvTmpReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepSetHumidityReq_S{
    MsgHeader_S header;
    float f_humidity;
    UINT32 ui_session;
    UINT32 ui_frameno;
}RepSetHumidityReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepGetRaDiTeHuReq_S{
    MsgHeader_S header;
    UINT32 ui_session;
}RepGetRaDiTeHuReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepGetRadiationResp_S{
    MsgHeader_S header;
    INT32 i_result;
    float f_radiation;
}RepGetRadiationResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepGetDistanceResp_S{
    MsgHeader_S header;
    INT32 i_result;
    float f_distance;
}RepGetDistanceResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepGetEnvTmpResp_S{
    MsgHeader_S header;
    INT32 i_result;
    float f_envtmp;
}RepGetEnvTmpResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagRepGetHumidityResp_S{
    MsgHeader_S header;
    INT32 i_result;
    float f_humidity;
}RepGetHumidityResp_S;
#pragma pack(pop)

//get
#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetRadiationReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}PicGetRadiationReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetRadiationResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    float  f_radiation;
}PicGetRadiationResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetDistanceReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}PicGetDistanceReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetDistanceResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    float  f_distance;
}PicGetDistanceResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetEnvTempReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}PicGetEnvTempReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetEnvTempResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    float  f_envtemp;
}PicGetEnvTempResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetOppohiteHumidityReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
}PicGetOppohiteHumidityReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetGetAllAnaReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    UINT16 ui_slide;
    UINT16 ui_ptz;
    UINT32 ui_start_time;
    UINT32 ui_end_time;
}PresetGetAllAnaReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetAllAnaInfo_S{
    UINT32 ui_start_time;
    UINT32 ui_file_size;
    UINT8 ac_file_data[0];
}PresetAllAnaInfo_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPresetGetAllAnaResp_S{
    MsgHeader_S header;
    INT32 i_Result;
    UINT32 ui_file_num;
    PresetAllAnaInfo_S st_file_info[0];
}PresetGetAllAnaResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveUpFrameReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    UINT16 ui_slide;
    UINT16 ui_ptz;
    UINT32 ui_cur_frame_utcs;
    UINT32 ui_cur_frame_utcms;
}CurveUpFrameReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveUpInfo_S{
    UINT32 ui_frame_type;
    UINT32 ui_frame_size;
    UINT8 ac_frame_data[0];
}CurveUpInfo_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveUpFrameResp_S{
    MsgHeader_S header;
    INT32 i_Result;
    UINT32 ui_frame_utcs;
    UINT32 ui_frame_utcms;
    UINT32 ui_frame_num;
    CurveUpInfo_S st_frame_info[0];
}CurveUpFrameResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveDownFrameReq_S{
    MsgHeader_S header;
    UINT32 ui_group;
    UINT16 ui_slide;
    UINT16 ui_ptz;
    UINT32 ui_cur_frame_utcs;
    UINT32 ui_cur_frame_utcms;
}CurveDownFrameReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveDownInfo_S{
    UINT32 ui_frame_type;
    UINT32 ui_frame_size;
    UINT8 ac_frame_data[0];
}CurveDownInfo_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurveDownFrameResp_S{
    MsgHeader_S header;
    INT32 i_Result;
    UINT32 ui_frame_utcs;
    UINT32 ui_frame_utcms;
    UINT32 ui_frame_num;
    CurveDownInfo_S st_frame_info[0];
}CurveDownFrameResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetServerTimeReq_S{
    MsgHeader_S header;
}GetServerTimeReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGetServerTimeResp_S{
    MsgHeader_S header;
    INT32 i_Result;
    UINT32 ui_cur_time_utcs;
    UINT32 ui_cur_time_utcms;
}GetServerTimeResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagPicGetOppohiteHumidityResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    float  f_humidity;
}PicGetOppohiteHumidityResp_S;
#pragma pack(pop) // add by huhua end


#pragma pack(push)  // add by chents about add slide
#pragma pack(1)
typedef struct tagSlideSetReq_S{
    MsgHeader_S header;
    UINT8 uac_ip[16];
    UINT8 uac_port[2];
}SlideSetReq_S;
#pragma pack(pop)

//premission begin
#define  ABOUT_PREMISSION_OK  0
#define  ABOUT_PREMISSION_PROTROL_ERROR  1
#define  ABOUT_PREMISSION_USER_NOT_EXIST 2
#define  ABOUT_PREMISSION_NO_PREMISSION 3
#define  ABOUT_PREMISSION_USER_EXIST  4
#define  ABOUT_PREMISSION_WRONG_PASSWD 7

#pragma pack(push)
#pragma pack(1)
typedef struct tagAddAccountReq_S{
    MsgHeader_S header;
    UINT32 ui_passwd_len;
    UINT8  auc_digest[16];
    UINT32 ui_name_len;
    std::string str_user_name;
    UINT32 ui_user_premission;
    UINT64 ul_out_dev_premission;
    UINT32 ui_group_count;
    std::map<UINT32, UINT64> ui_group_premission;
    UINT32 ui_extend_len;
    std::string str_extend;
}AddAccountReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagAddAccountResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}AddAccountResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagChgAccountPremissionReq_S{
    MsgHeader_S header;
    UINT32 ui_passwd_len;
    UINT8  auc_digest[16];
    UINT32 ui_name_len;
    std::string str_user_name;
    UINT32 ui_user_premission;
    UINT64 ul_out_dev_premission;
    UINT32 ui_group_count;
    std::map<UINT32, UINT64> ui_group_premission;
    UINT32 ui_extend_len;
    std::string str_extend;
}ChgAccountPremissionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagChgAccountPremissionRessp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}ChgAccountPremissionResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagChgAccountPasswdReq_S{
    MsgHeader_S header;
    UINT32 ui_passwd_len;
    //std::string str_passwd;
    UINT8  auc_digest[16];
    UINT32 ui_user_len;
    std::string str_user_name;
    UINT32 ui_user_passwd_len;
    //std::string str_user_passwd;
    UINT8  auc_user_digest[16];
}ChgAccountPasswdReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagChgAccountPasswdResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}ChgAccountPasswdResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDelAccountReq_S{
    MsgHeader_S header;
    UINT32 ui_passwd_len;
    UINT8  auc_digest[16];
    UINT32 ui_user_len;
    std::string str_user_name;
}DelAccountReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagDelAccountResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}DelAccountResp_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagQueryAccountPremissionReq_S{
    MsgHeader_S header;
    UINT32 ui_user_len;
    std::string str_user_name;
    UINT32 ui_get_other_flag;//1 all
}QueryAccountPremissionReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct queryAccountPremission{
    UINT32 ui_user_len;
    UINT8  auc_data_q[0];
    //std::string str_user_name;
    //UINT32 ui_user_premission;
    //UINT64 ul_out_dev_premission;
    //UINT32 ui_group_count;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagQueryAccountPremissionResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
    UINT32 ui_account_count;
    UINT8  auc_data[0];
}QueryAccountPremissionResp_S;
#pragma pack(pop)

//premission end

// add by huhua end

#pragma pack(push)
#pragma pack(1)
typedef struct tagSlideSetResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}SlideSetResp_S;
#pragma pack(pop)

#pragma pack(push) //add by tchen about del slide
#pragma pack(1)
typedef struct tagSlideDelReq_S{
    MsgHeader_S header;
    UINT8 uac_ip[16];
    UINT8 uac_port[2];
}SlideDelReq_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagSlideDelResp_S{
    MsgHeader_S header;
    UINT32 ui_Result;
}SlideDelResp_S;
#pragma pack(pop)


typedef struct tagCtrlReq_S{
//    ReqUnion req;
    UINT32 req_type;
    INT32 i_conn_fd;
    UINT32 ui_client_id;
    struct in_addr st_cli_in_addr;
    void *pv_req;
}CtrlReq_S;

typedef struct tagCtrlReqPara_S{
//    ReqUnion req;
    UINT32 req_type;
    INT32 i_conn_fd;
    UINT32 ui_client_id;
    struct in_addr st_cli_in_addr;
}CtrlReqPara_S;

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_STOP_CRUISE_S{
    UINT32 ui_group;
}NOTICE_STOP_CRUISE_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_START_CRUISE_S{
    UINT32 ui_group;
    UINT32 ui_cruise_no;
}NOTICE_START_CRUISE_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PRESET_ARRIVAL_S{
    UINT32 ui_group;
    UINT16 us_slide;
    UINT16 us_ptz;
}NOTICE_PRESET_ARRIVAL_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PRESET_LEAVE_S{
    UINT32 ui_group;
    UINT32 ui_leave_preset;
    UINT32 ui_goto_preset;
}NOTICE_PRESET_LEAVE_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PRESET_DEL_S{
    UINT32 ui_group;
    UINT16 us_del_slide;
    UINT16 us_del_ptz;
}NOTICE_PRESET_DEL_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_CRUISE_STOP_S{
    UINT32 ui_group;
}NOTICE_CRUISE_STOP_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_FIXED_POINT_CRUISE_START_S{
    UINT32 ui_group;
}NOTICE_FIXED_POINT_CRUISE_START_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_FIXED_POINT_CRUISE_STOP_S{
    UINT32 ui_group;
}NOTICE_FIXED_POINT_CRUISE_STOP_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PIC_SET_RADIATION_S{
    UINT32 ui_group;
    float  f_radition;
}NOTICE_PIC_SET_RADIATION_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PIC_SET_DISTANCE_S{
    UINT32 ui_group;
    float  f_distance;
}NOTICE_PIC_SET_DISTANCE_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PIC_SET_ENV_TEMP_S{
    UINT32 ui_group;
    float  f_envtemp;
}NOTICE_PIC_SET_ENV_TEMP_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_PIC_SET_OPPOHITE_HUMIDITY_S{
    UINT32 ui_group;
    float  f_humidity;
}NOTICE_PIC_SET_OPPOHITE_HUMIDITY_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_CRUISE_RESTART_FLAG_S{
    UINT32 ui_group;
    UINT32 ui_restart_flag;
    UINT32 ui_time;
}NOTICE_CRUISE_RESTART_FLAG_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_INFRA_RECORD_CONDITION_S{
    UINT32      ui_group;
    UINT16      us_slide;
    UINT16      us_ptz;
    UINT32      ui_record_time_interval;
    float       ft_max_temp_interval;//-9999 means invalid
    float       ft_max_temp;//-9999 means invalid
}NOTICE_INFRA_RECORD_CONDITION_S;
#pragma pack(pop)


#define DISCONNECT_UNKNOWN_REASON  0
#define DISCONNECT_PREMISSION_CHG  1
#define DISCONNECT_PASSWORD_CHG    2
#define DISCONNECT_USER_DELETED    3
#define DISCONNECT_OTHER_CLIENT_LOGIN  4

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_LINK_DISCONNECT_S{
    UINT32 ui_reason;
}NOTICE_LINK_DISCONNECT_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_ADD_GROUP_S{
    UINT32 ui_group_no;
    std::string str_group_alias;
    std::string str_slide_sn;
    std::string str_ptz_sn;
    std::string str_infra_sn;
    std::string str_visi_sn;
}NOTICE_ADD_GROUP_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_CHG_GROUP_S{
    UINT32 ui_group_no;
    std::string str_group_alias;
    std::string str_slide_sn;
    std::string str_ptz_sn;
    std::string str_infra_sn;
    std::string str_visi_sn;
    UINT32 ui_clear_flag;
}NOTICE_CHG_GROUP_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagGETDEVCURPICTURE_S {
    std::string s_sn;
    std::string s_pic_data;
}NOTICE_GET_DEV_CUR_PIC_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNotice_Alarm_S{
    std::string str_sn;
    UINT32 ui_alarm_level;
    UINT32 ui_alarm_method;
    std::string str_time;
}Notice_Alarm_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_DEL_GROUP_S{
    UINT32 ui_group_no;
}NOTICE_DEL_GROUP_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagNOTICE_GET_GROUP_CRUISE_INFO_S{
    UINT32 ui_group_no;
}NOTICE_GET_GROUP_CRUISE_INFO_S;
#pragma pack(pop)

typedef struct tagMd5Val_S
{
    UINT32 n_a;
    UINT32 n_b;
    UINT32 n_c;
    UINT32 n_d;
}Md5Val_S;

typedef struct tagUserSecret_S{
    std::string str_user_name; //�û���
    UINT8  auc_digest[16]; //����
    UINT32 i_user_premission; //�û��ȼ�
    UINT64 ul_out_group_premission; //Ȩ��֮�豸�޹�
    std::map<UINT32, UINT64> ui_group_premission;//Ȩ��֮�豸��� ���,Ȩ��
    std::string str_father_name; //���û���
    std::string str_extend; //��ע
}UserSecret_S;

typedef struct {
    std::string classstr;
    std::string typestr;
    std::string snstr;
    std::string ipstr;
    std::string portstr;
    std::string macstr;
    std::string statusstr;
    INT32  i_miss_times; /* ����3��δ���ֲ�������豸ʧЧ�ˣ���������Ǽ����õ� */
}XmlDeviceInfo_S;

typedef struct {
    UINT32 ui_type;
    UINT32 ui_len;
    UINT32 ui_seq;
}CmdHead_S;

#pragma pack(push)
#pragma pack(1)
typedef struct tagVOD_RESP_DATA_HEAD_S{
    UINT32 ui_Magic; /* ��ֵ�̶�Ϊ0xA1A11A1A */
    UINT32 ui_type;
    UINT32 ui_total_len;
    UINT32 ui_seq;
    UINT8 auc_data[0];
}VOD_RESP_DATA_HEAD_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagVOD_RESP_CLR_DATA_S{
    VOD_RESP_DATA_HEAD_S st_resp_head;
    INT32                i_clr;
}VOD_RESP_CLR_DATA_S;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct tagMONITOR_LIVE_HEAD{
    UINT32 ui_Magic; /* ��ֵ�̶�Ϊ0XA0A05A5A */
    UINT32 ui_type;
    UINT32 ui_Length;
    UINT32 ui_seq;
    UINT8 auc_data[0];
}MONITOR_LIVE_HEAD_S;
#pragma pack(pop)

typedef struct tagMONITOR_FRAME_TIME_HEAD{
    UINT32 ui_Magic;  // ��ֵ�̶�Ϊ0XA0A05A5A
    UINT32 ui_type;   // 0x21
    UINT32 ui_Length; // sizeof( unsigned int ) * 6
    UINT32 ui_seq;    // 0
    UINT32 ui_sec;    //sec
    UINT32 ui_usec;   //usec
}MONITOR_FRAME_TIME_HEAD;

typedef struct tagMONITOR_CLIENT_ID_S{
    UINT32 ui_client_id;
    INT32  i_type;  /* 0��ʾ��������1��ʾ������ */
}MONITOR_CLIENT_ID_S;


typedef struct tagIR_ANA_DATA_S {
    UINT8 *puc_data;
    INT32 i_len;
    UINT32 ui_frame_no;
}IR_ANA_DATA_S;

typedef struct tagCOMM_INFRA_DATA_S {
    UINT8 *puc_data;
    INT32 i_len;
    UINT32 ui_frame_no;
}COMM_INFRA_DATA_S;

typedef struct tagCOMM_DATA_S {
    UINT8 *puc_data;
    INT32 i_len;
}COMM_DATA_S;

typedef struct tagCOMM_DATA_C_S {
    INT8 *pc_data;
    INT32 i_len;
}COMM_DATA_C_S;

#pragma pack(push)
#pragma pack(1)
typedef struct tagMONITOR_TEMPERATURE_PKT_S{
    UINT32 ul_tmp;
    u_int8_t t2;    //�¶ȵ�С��λ  ��0~255)
}MONITOR_TEMPERATURE_PKT_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagTHREE_MONITOR_TEMPERATURE_S{
    float  f_max;
    float  f_min;
    float  f_avg;

    tagTHREE_MONITOR_TEMPERATURE_S(){
        f_max = 0.0f;
        f_min = 0.0f;
        f_avg = 0.0f;
    }
}THREE_MONITOR_TEMPERATURE_S;
#pragma pack(pop)

typedef struct tagMONITOR_TEMPERATURE_HEAD_S{
    UINT32 ui_num;  // ����ж��ٸ��¶Ȱ���
    UINT32 ui_analyse_str_Length; // �����ַ������ȣ�����'\0'��
    UINT8 auc_string[0]; //�����ַ���
}MONITOR_TEMPERATURE_HEAD_S;

/*********************** class������ *********************/
class ReferDataBuf{
public:
    ReferDataBuf(){mi_reference = 1; pthread_mutex_init( &m_refer_lock, NULL );}
    ~ReferDataBuf() {pthread_mutex_destroy( &m_refer_lock );}
    INT32 init_data(UINT8 *puc_data_buf, UINT32 ui_data_len) {
        mpuc_data_buf = new UINT8[ui_data_len]();
        if (mpuc_data_buf == NULL) {
            return -1;
        }
        memcpy(mpuc_data_buf, puc_data_buf, ui_data_len);
        mi_buf_len = ui_data_len;

        return 0;
    }
    INT32 deinit() {
        INT32 i_ret = 0;
        refer_lock();
        if (mi_reference > 1) {
            mi_reference -= 1;
            i_ret = 1;
        } else {
            i_ret = 0;
            delete []mpuc_data_buf;
        }
        refer_unlock();

        return i_ret;
    }
    void add_refer() {
        refer_lock();
        mi_reference++;
        refer_unlock();
    }
    INT32 get_refer() {
        return mi_reference;
    }
    void get_data_buf(UINT8 *&puc_data_buf, INT32 &i_buf_len){ puc_data_buf = mpuc_data_buf; i_buf_len = mi_buf_len; }

private:
    INT32 refer_lock() { return pthread_mutex_lock(&m_refer_lock); }
    INT32 refer_unlock() { return pthread_mutex_unlock(&m_refer_lock); }
private:
    INT32 mi_reference;
    UINT8 *mpuc_data_buf;
    INT32 mi_buf_len;
    pthread_mutex_t m_refer_lock;
};

typedef struct{
    UINT32      ui_alarm_no;
    std::string str_alias;
    std::string str_alarm_condition;
}AlarmConditionInfo_S;

typedef struct {
    ReferDataBuf *po_refer_data;
    INT32 i_frame_type;
    UINT32 ui_frame_no;
}VOD_DATA_SEND_INFO_S;

typedef struct {
    INT16 slide;
    INT16 ptz;
    INT32 second;
}CruisePoint;

typedef struct {
    UINT32 ui_ana_no;
    std::string ananame;
    std::string analyse;
}RecordAnalyseInfo;

typedef struct {
    UINT32 ui_group;
    UINT16 us_slide;
    UINT16 us_ptz;
    RecordAnalyseInfo st_preset_analyse;
}PresetAnalyseInfo;

typedef struct tagEPicInfo_S{
    UINT32 ui_pic_id;
    std::string s_pic_cname;
}EPicInfo_S;

typedef struct tagPresetAnaFileInfo_S{
    std::string str_file_name;
    UINT8  uc_file_stat;
    UINT32 ui_group_id;
    UINT32 ui_start_time;
    UINT32 ui_start_time_res;       //�����ظ����ݵ���ʼʱ��
    std::string str_file_data_res;       //�����ظ�������
}PresetAnaFileInfo_S;

#pragma pack(push)
#pragma pack(1)
typedef struct tagMONITOR_TEMP_POINT_S{
    UINT32 ui_ana_no; //Ԥ�õ������
    float f_max;
    float f_avg;
    float f_min;
    UINT32 ui_sec;  //UTCʱ�䣬��ȷ����
    UINT32 ui_usec; //UTCʱ�䣬��ȷ��΢��
}MONITOR_TEMP_POINT_S;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct tagCurFrameInfo_S{
    UINT32 ui_group;
    UINT16 ui_slide;
    UINT16 ui_ptz;
    UINT32 ui_cur_frame_num;
    UINT32 ui_irv_frame_num;
    std::string s_irv_file;
    tagCurFrameInfo_S(){
        ui_group = 0;
        ui_slide = 0;
        ui_ptz = 0;
        ui_cur_frame_num = 0;
        ui_irv_frame_num = 0;
        s_irv_file = "";
    }
}CurFrameInfo_S;
#pragma pack(pop)


/* ��ȫ�ֽṹ�壬������¼��ʵʱ�Ⱥ�˳��û����ô�ϸ�ı������ʲ��ü��� */
typedef struct tagCtrlPara_S {
    bool b_chk;
}CtrlPara_S;


#endif
