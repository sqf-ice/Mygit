/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan12.c
* Author 		: linwl
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiDisconnect�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_WiFiDisconnect"	

/*------------global variables definition-----------------------*/
#define MAXTIMEOUT 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan8(void)
* functional description 	: ��ȡMac ��ַ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					linwl   20151113	created
*****************************************************************/
void wlan12(void)
{
	/*private & local definition*/
	int nRet=0;
	time_t diff = 0, oldtime = 0;
	ST_WIFI_PARAM param;
	EM_WPA_CONSTATE state;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������	

	/*����ǰ��*/
	param.pszKey =  linksetting.WLANPwd;
	param.emKeyType = linksetting.WLANKeyType;
	param.emSecMode = linksetting.WLANEncMode;
	param.psEthIp = netsetting.LocalIP.Str;
	param.psEthGateway = netsetting.LocalGateway.Str;
	param.psEthNetmask =  netsetting.LocalMask.Str;
	param.ucIfDHCP = netsetting.DHCPenable;
	param.psEthDnsPrimary = netsetting.DNSHost[0];
	param.psEthDnsSecondary = netsetting.DNSHost[1];

	//case1:��ʼ��֮��,δ��������µ��öϿ����Ӳ�Ӧ�ó����쳣
	if((nRet = NDK_WiFiDisconnect()) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		GOTOERR;
	}
	
	if((nRet = NDK_WiFiInit()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		GOTOERR;
	}
	
	//case2:��ʼ��֮��,δ��������µ��öϿ����Ӳ�Ӧ�ó����쳣
	if((nRet = NDK_WiFiDisconnect()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		GOTOERR;
	}

	//case3:���Ӻ�,���öϿ����Ӻ���,Ӧ���ܹ��Ͽ��ɹ�
	if ((nRet=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	oldtime = time(NULL);
	while(1)
	{
		if ((nRet=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
			goto ERR;
		}
		show_stopwatch(1, diff);
		
		if (state == WIFI_WPA_CONSTATE_CONTEXT)
			break;
		else
			NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
	}
	if ((nRet=NDK_WiFiIsConnected()) != NDK_OK)
	{
	    send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
	    GOTOERR;
	}
	if(auto_flag==0||auto_flag==2)
	{
		if(cls_show_msg("״̬���Ƿ���ʾWIFI������ͼ��,[��]:ȷ��,[��]:����")!=ENTER)
		{	
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			GOTOERR;
		}
	}
	if((nRet = NDK_WiFiDisconnect()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	if ((nRet=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED)//����Ҫ�жϹҶ��Ƿ���Ҫʱ��
	{
	    send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
	    GOTOERR;
	}
	if(auto_flag==0||auto_flag==2)
	{
		if(cls_show_msg("״̬���Ƿ���ʾWIFIͼ��,[��]:ȷ��,[��]:����")!=ENTER)
		{	
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			GOTOERR;
		}
	}
	
	//case4:�Ͽ����Ӻ�,�������Ӻ���,Ӧ���ܹ��������ӳɹ�
	if ((nRet=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	oldtime = time(NULL);
	while(1)
	{
		if ((nRet=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
			goto ERR;
		}
		show_stopwatch(1, diff);
		
		if (state == WIFI_WPA_CONSTATE_CONTEXT)
			break;
		else
			NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
	}
	if ((nRet=NDK_WiFiIsConnected()) != NDK_OK)
	{
	    send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
	    GOTOERR;
	}
	
	//case5:�����öϿ����Ӻ���,ֱ�ӹҶ�оƬ��,���³�ʼ�����Ӳ���Ӧ�óɹ�
	if((nRet = NDK_WiFiShutdown()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	if((nRet = NDK_WiFiInit()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		GOTOERR;
	}
	if ((nRet=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	oldtime = time(NULL);
	while(1)
	{
		if ((nRet=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
			goto ERR;
		}
		show_stopwatch(1, diff);
		
		if (state == WIFI_WPA_CONSTATE_CONTEXT)
			break;
		else
			NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
	}
	if ((nRet=NDK_WiFiIsConnected()) != NDK_OK)
	{
	    send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
	    GOTOERR;
	}
	if((nRet = NDK_WiFiDisconnect()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	if((nRet = NDK_WiFiShutdown()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//test over
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_WiFiShutdown();
	return;
}
