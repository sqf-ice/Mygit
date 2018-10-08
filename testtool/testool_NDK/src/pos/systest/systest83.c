/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WIFI AP�ۺ�
* file name		: systest83.c
* Author 			: 
* version			: V1.0
* DATE			: 20141210
* directory 		: 
* description		: WIFI AP�ۺ�
* related document	: NL_POS����Ա�ο��ֲ�
*				  
************************************************************************
* log			: 
* Revision 1.0  20130120 chenfm
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"WIFI AP�ۺ�"

#define	DEFAULT_CNT_STR	"10"//"1000"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 				: author			date			remarks
*						linwl		20141210		created
*****************************************************************/
static void open_wifi_ap(int flag)
{
	/*private & local definition*/
	ST_WIFIAP_PARAM stAPParam = {0};
	int ret=-1;
	uint h_tcp = -1;
	
	/*process body*/
	//�򿪹������磬����ETH����WLM����
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}
	
	//��ȡȫ��WiFi�ȵ����
	memset(&stAPParam, 0,sizeof(stAPParam));
	stAPParam.emShareDev = WifiApShareDev;
	stAPParam.emSecMode = WifiApSecMode;
	strcpy(stAPParam.szAPIP, netsetting.WifiApIp.Str);
	strcpy(stAPParam.szKey,WifiApKey);
	strcpy(stAPParam.szSsid , WifiApSsid);
	stAPParam.uchidden = WifiApHidden;
	
	//��WIFI�ȵ�
	if((ret=NDK_WiFiAPOpen(stAPParam))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(ret=%d)", __LINE__, TESTITEM, ret);
		return;
	}
	//cls_show_msg("DEV:%d MODE:%d\nAPIP:%s\nAPKEY:%s\nAPSSID:%s\nHIDDEN:%d",stAPParam.emShareDev, stAPParam.emSecMode,stAPParam.szAPIP,stAPParam.szKey,stAPParam.szSsid,stAPParam.uchidden);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"WIFI AP�򿪳ɹ�\nAPSSID:%s\nAPKEY:%s\nAPIP:%s\n���������", stAPParam.szSsid, stAPParam.szKey, stAPParam.szAPIP);

	if(flag==1)
	{
		if(TransUp(&h_tcp)!=SUCC)
		{
			NetDown();
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:TransUpʧ��", __LINE__);
			return;
		}
		send_recv_press(h_tcp);
		TransDown(h_tcp);
	}
	cls_show_msg_record(FILENAME ,FUNCNAME ,"WIFI AP�򿪳ɹ�\nAPSSID:%s\nAPKEY:%s\nAPIP:%s\n������˳�", stAPParam.szSsid, stAPParam.szKey, stAPParam.szAPIP);
	return;
}

static void close_wifi_ap(void)
{
	/*private & local definition*/
	int ret = -1;
	
	/*process body*/
	//�ر�WIFI�ȵ�
	if((ret=NDK_WiFiAPClose())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(ret=%d)", __LINE__, TESTITEM, ret);
		return;
	}
	//�Ͽ���������
	NetDown();
	cls_show_msg_record(FILENAME ,FUNCNAME ,"WIFI AP�رճɹ�,������˳�\n");
	return;
}

static void test_wifi_AP(void)
{
	/*private & local definition*/
	int ret = '0', apcommflag = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{	
		if(auto_flag==1)
		{
			if(( ++ret == '4')||( cls_show_msg1( 2, "��������%sAPѹ������,ESC�˳�",TESTITEM))==ESC)
				return;
		}
		else
		{
			ret = cls_show_msg("1.��AP��������\n"
								"2.�ر�AP��������\n"
								"3.����AP��ͨ��־\n");
		}
		switch(ret)
		{
		case '1':
			open_wifi_ap(apcommflag);
			break;
		case '2':
			close_wifi_ap();
			break;
		case '3':
			if(cls_show_msg1(10, "�Ƿ����AP���������շ�����:�ǰ���ȷ�ϡ�,����������\n")==ENTER)
				apcommflag=1;
			else
				apcommflag=0;
			break;
		case ESC:
			cls_show_msg("�˳����Գ���ǰ�����WIFI AP�رղ���\n");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

static void ap_abnormity(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while (1)
	{
		keyin = cls_show_msg("1.��̨�豸����AP\n" 
							"2.���߲���\n"
							"3.��ʱ�侲��\n"
							"4.�ϵ������\n");
		switch (keyin)
		{
		case '1':
			cls_show_msg("���������AP,��AP�򿪳ɹ�����ʹ�ö�̨�豸����AP����Ӧ���������ݴ���");
			break;
		case '2':
			cls_show_msg("���������AP,��AP�򿪳ɹ�����ȴ�AP��������,���߻��Ѻ�ʹ�������豸����APӦ�����������ݴ���");
			break;
		case '3':
			cls_show_msg("���������AP,��AP�򿪳ɹ����泤ʱ�����(����30����)��ʹ�������豸����APӦ�����������ݴ���");
			break;
		case '4':
			cls_show_msg("���������AP,��AP�򿪳ɹ�����ʹ�������豸����AP��POS������ϵ���ٽ���AP��������Ӧ�óɹ�");
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
		open_wifi_ap(0);
		cls_printf("�ر�AP����������...");
		close_wifi_ap();
	}
	return;
}


void systest83(void)
{
	/*private & local definition*/
	int ret = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		//��������
		if(conf_conn_AP()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,������������ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//AP��������
		conf_wifi_AP();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��APѹ������
		test_wifi_AP();
			
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.APѹ������\n"
							"2.AP�쳣����\n"
							"7.AP��������\n"
							"8.������������\n");
		
		switch(ret)
		{
		case '1':
			test_wifi_AP();
			break;
		case '2':
			ap_abnormity();
			break;
		case '7':
			conf_wifi_AP();
			break;
		case '8':
			switch(conf_conn_AP())
			{
			case SUCC:
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				break;
			case NDK_ERR_QUIT:
			default:
				break;
			}
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

