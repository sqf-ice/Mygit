/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan8.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiShutdown�ӿڹ���
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
#define TESTAPI	"NDK_WiFiShutdown"	

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
*			  	  	  					chensj   201305013	created
*****************************************************************/
void wlan8(void)
{
	/*private & local definition*/
	int i= 0, numList = 0,nRet=0;
	time_t diff = 0, oldtime = 0;
	ST_WIFI_PARAM param;
	EM_WPA_CONSTATE state;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];

	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];
	
	/*process body*/
	if(auto_flag==2)
		return;
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������	

	/*����ǰ��*/
	if((nRet = NDK_WiFiInit()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		GOTOERR;
	}
	
	//case1:����NDK_WiFiInit��NDK_WiFiGetNetListӦ�ɹ�,��NDK_WiFiShutdown��NDK_WiFiGetNetListӦ�ܷ���ʧ��
	/*
	int j = 0;
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetList(ESSIDlist, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
					cls_show_msg1(3, "��ɨ�赽%d��AP,line:%d,AP[%d]=%s", numList,__LINE__, i, ESSIDlist1[i]);
				break;
			}
		}
		sleep(1);
	}
	if (j == 3)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}*/
	
	if((nRet = NDK_WiFiShutdown()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	for (i=0; i<3; i++)
	{
		if (NDK_WiFiGetNetList(ESSIDlist, &numList) == NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		sleep(1);
	}
	if (i == 3)
	{
		cls_printf("case1����ͨ��");
	}

	param.pszKey =  linksetting.WLANPwd;
	param.emKeyType = linksetting.WLANKeyType;
	param.emSecMode = linksetting.WLANEncMode;
	param.psEthIp = netsetting.LocalIP.Str;
	param.psEthGateway = netsetting.LocalGateway.Str;
	param.psEthNetmask =  netsetting.LocalMask.Str;
	param.ucIfDHCP = netsetting.DHCPenable;
	param.psEthDnsPrimary = netsetting.DNSHost[0];
	param.psEthDnsSecondary = netsetting.DNSHost[1];
	
	//case2:����NDK_WiFiInit��NDK_WiFiConnectӦ�ɹ�,��NDK_WiFiShutdown��NDK_WiFiConnectӦ�ܷ���ʧ��
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

	if((nRet = NDK_WiFiShutdown()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	if ((nRet=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}

	//case3:���ԹҶϺ����³�ʼ����,��������Ӧ�óɹ����ر�Ӧ��ʧ��
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
			send_result("line %d:%s����ʧ��ret=%d,diff=%d", __LINE__, TESTAPI, nRet, diff);
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

	if((nRet = NDK_WiFiShutdown()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	if ((nRet=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	
	//test over
	send_result("%s����ͨ��", TESTAPI);
	//return;
	
ERR:
	NDK_WiFiShutdown();
	return;
}
