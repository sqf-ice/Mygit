/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan5.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiConnectState�ӿڹ���
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
#define TESTAPI	"NDK_WiFiConnectState"	

/*------------global variables definition-----------------------*/
#define MAXTIMEOUT 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan5(void)
* functional description 	: ��ȡ����״̬��Ϣ ,�ж��Ƿ�������AP  
* input parameter	 				:��
* output parameter	 				:��
* return value		 				:��
* history 		 					:  author		date		remarks
*			  	  	  			   chensj   201305013	created
*****************************************************************/
void wlan5(void)
{
	/*private & local definition*/
	int ret = 0;
	ST_WIFI_PARAM param;
	EM_WPA_CONSTATE state;
	time_t diff = 0, oldtime = 0;
	
	/*process body*/
	if(auto_flag==2)
		return;
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�豸δ��ʼ������SDK_WiFiConnectState����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiConnectState(&state)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE) 
	{
    	send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//����ǰ��:WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}
	
	//case4:�豸δ��ʼ������SDK_WiFiConnectState����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiConnectState(&state)) != NDK_OK||state!=WIFI_WPA_CONSTATE_UNLINKED) 
	{
		send_result("line %d:%s����ʧ��ret=%d,state=%d", __LINE__, TESTAPI, ret, state);
		RETURN;
	}

	//case2:�쳣�������ԣ������������ΪNULL,Ӧ�÷��ز�������
	if ((ret=NDK_WiFiConnectState(NULL)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//LINKING,        ///< Linking to WiFi
	//LINKED,         ///< Selected but still not authenticated nor connected
    //AUTHENTICATED,  ///< After linked, we are authenticated but we have no context
    //CONTEXT   

	//case3:�������Թ�����ֿ��ܵ�״̬�ֱ������֤
	param.pszKey =  linksetting.WLANPwd;
	param.emKeyType = linksetting.WLANKeyType;
	param.emSecMode = linksetting.WLANEncMode;
	param.psEthIp = netsetting.LocalIP.Str;
	param.psEthGateway = netsetting.LocalGateway.Str;
	param.psEthNetmask =  netsetting.LocalMask.Str;
	param.ucIfDHCP = netsetting.DHCPenable;
	param.psEthDnsPrimary = netsetting.DNSHost[0];
	param.psEthDnsSecondary = netsetting.DNSHost[1];
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	oldtime = time(NULL);
	while(1) 
	{
    	if ((ret=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT) 
		{
    		send_result("line %d:%s����ʧ��ret=%d,state=%d", __LINE__, TESTAPI, ret, state);
			goto ERR;
    	}
		show_stopwatch(1, diff);
		if(state==WIFI_WPA_CONSTATE_UNLINKED)
			cls_printf("is UNLINKED...");
		if(state==WIFI_WPA_CONSTATE_LINKING)
			cls_printf("is LINKING...");
		if(state==WIFI_WPA_CONSTATE_LINKED)
			cls_printf("is LINKED...");
		if(state==WIFI_WPA_CONSTATE_AUTHENTICATED)
			cls_printf("is AUTHENTICATED...");
		if (state == WIFI_WPA_CONSTATE_CONTEXT)
			break;
		else
			NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
    }
	
	if ((ret=NDK_WiFiIsConnected()) != NDK_OK) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if((ret = NDK_WiFiDisconnect()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if ((ret=NDK_WiFiConnectState(&state)) != NDK_OK||state!=WIFI_WPA_CONSTATE_UNLINKED) 
	{
		send_result("line %d:%s����ʧ��ret=%d,state=%d", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}
    
	//���Ժ���:WIFI�豸�Ҷ�
	send_result("%s����ͨ��", TESTAPI);
ERR:
	//���Ժ���:WIFI�豸�Ҷ�
	NDK_WiFiShutdown();
	return;
}

