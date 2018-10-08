/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WIFI/AP����
* file name		: systest84.c
* Author 			: 
* version			: V1.0
* DATE			: 20141210
* directory 		: 
* description		: WIFI/AP����
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
#define	TESTITEM	"WIFI/AP����"

#define	DEFAULT_CNT_STR	"10"
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
static int test_AP(LinkType type)
{
	/*private & local definition*/
	ST_WIFIAP_PARAM stAPParam = {0};
	int ret=-1;
	
	/*process body*/
	//�򿪹������磬����ETH����WLM����
	setlinktype(type);
	if(NetUp()!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:NetUpʧ��", __LINE__);
		NetDown();
		return FAIL;
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
		NDK_WiFiAPClose();
		NetDown();
		return FAIL;
	}
	//cls_show_msg("DEV:%d MODE:%d\nAPIP:%s\nAPKEY:%s\nAPSSID:%s\nHIDDEN:%d",stAPParam.emShareDev, stAPParam.emSecMode,stAPParam.szAPIP,stAPParam.szKey,stAPParam.szSsid,stAPParam.uchidden);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"WIFI AP�򿪳ɹ�\nAPSSID:%s\nAPKEY:%s\nAPIP:%s\nʹ������һ̨�豸����һ��WIFI����ͨ�ź����������", stAPParam.szSsid, stAPParam.szKey, stAPParam.szAPIP);
	
	//�ر�WIFI�ȵ�
	if((ret=NDK_WiFiAPClose())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(ret=%d)", __LINE__, TESTITEM, ret);
		NDK_WiFiAPClose();
		NetDown();
		return FAIL;
	}
	//�Ͽ���������
	NetDown();
	return SUCC;
}

static int test_wifi(void)
{
	/*private & local definition*/
	int slen = 0, rlen = 0, timeout = SO_TIMEO;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	uint h_tcp = -1;
	
	/*process body*/
	setlinktype(WLAN);
	if(NetUp()!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:NetUpʧ��", __LINE__);
		NetDown();
		return FAIL;
	}
	
	//UP
	if(TransUp(&h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return FAIL;
	}
		
	//��������
	if((slen=sock_send(h_tcp, buf, PACKMAXLEN, timeout))!=PACKMAXLEN)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, PACKMAXLEN);
		goto ERR;
	}
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, PACKMAXLEN, timeout))!=slen)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, slen);
		goto ERR;
	}
	//�Ƚ��շ�
	if (MemCmp(buf, rbuf, PACKMAXLEN))
	{
		cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
		goto ERR;
	}

	if (TransDown(h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransDownʧ��", __LINE__);
		goto ERR;
	}
	NetDown();
	return SUCC;
ERR:
	TransDown(h_tcp);
	NetDown();
	return FAIL;
}

void WLAN_AP_cross(LinkType type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, i = 0, cnt = DEFAULT_CNT_VLE;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen=0;
		
	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:");
	if ((ret= lib_kbgetinput(CntStr,2, sizeof(CntStr)-1, &unLen,2, 5,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//����WIFI����ͨ��һ��
	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "%s������\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", TESTITEM, cnt, succ))
			break;
		cnt--;
		i++;

		//����WIFI AP���ܲ���
		Local2Global(type);
		if(test_AP(type)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��", __LINE__, i, TESTITEM);
			continue;
		}
		Global2Local(type);
		
		//����WIFI���ܲ���
		Local2Global(WLAN);
		if(test_wifi()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��", __LINE__, i, TESTITEM);
			continue;
		}
		Global2Local(WLAN);
		succ++;
	}

	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������\n�ܹ�%d��(�ɹ�%d��)", TESTITEM, cnt, succ);
	return;
}

void systest84(void)
{
	/*private & local definition*/
	int ret = 0, tmpflag = 0;
	LinkType apsharelinktype = ETH;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=1)
		SetWifiStopflag(1);//ȷ������������ʱwifiÿ�ζ�����stopȥ�Ҷ�

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		//AP��������
		conf_wifi_AP();
		//������������
		if(conf_conn_AP()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,������������ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		apsharelinktype = getlinktype();//���úù�������󽫹�����������(ETH,GPRS,CDMA)���ֺ��ڴ򿪹�������ʱʹ��
		Global2Local(apsharelinktype);//���ݹ�������Ĳ���
		//WLAN����
		if(conf_conn_WLAN()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WLAN����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		Global2Local(WLAN);

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��APѹ������
		WLAN_AP_cross(apsharelinktype);
			
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		SetWifiStopflag(tmpflag);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.WIFI/AP����\n"
							"7.AP��������\n"
							"8.������������\n"
							"9.WLAN����\n");
		
		switch(ret)
		{
		case '1':
			WLAN_AP_cross(apsharelinktype);
			break;
		case '7':
			conf_wifi_AP();
			break;
		case '8':
			switch(conf_conn_AP())
			{
			case SUCC:
				apsharelinktype = getlinktype();//���úù�������󽫹�����������(ETH,GPRS,CDMA)���ֺ��ڴ򿪹�������ʱʹ��
				Global2Local(apsharelinktype);//���ݹ�������Ĳ���
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				break;
			case NDK_ERR_QUIT:
			default:
				break;
			}
			
			break;
		case '9':
			switch(conf_conn_WLAN())
			{
			case SUCC:
				//����wlan����
				Global2Local(WLAN);
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
			SetWifiStopflag(tmpflag);
			return;
			break;
		default:
			continue;
			break;
		}
	}

	SetWifiStopflag(tmpflag);
	return;
}

