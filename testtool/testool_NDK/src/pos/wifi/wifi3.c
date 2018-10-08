/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��Wifiģ��
* file name		: wifi3.c
* Author 		:  jiangym
* version		: 
* DATE			:  20160226
* directory 		: 
* description		: ����NDK_WifiStatus�Ƿ�����ȷ��ȡ�ײ����ڲ�����AP���ƺ�����״̬
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
#define TESTAPI	"NDK_WifiStatus"	

/*------------global variables definition-----------------------*/
#define MAXTIMEOUT 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wifi3(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			  jiangym	   20160226		created
*****************************************************************/
void wifi3(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, ret1 = 0;
	time_t oldtime;
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_AP_ENTRY_T set_ap[32];
	list_s.stored_ap = set_ap;
	ST_WIFI_STATUS status;
	char *ssid[] = {
					"AP1@#fartest",
					"AP2&4same",
#if defined ME68 || defined ME50NX  //����ƽ̨��֧�ֶ���  20180515  modify
					"`~!\"@$#[^%]*() -_=+|\\&{}:;?/><.",  //·��������Ϊ:`~!"@$#[^%]*() -_=+|\&{}:;?/><.
#else
					"`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.",
#endif
					};

	/*process body*/
	if(auto_flag==2)
		return;
	cls_printf("����%s...", TESTAPI);
	//�ṩ����ap ssid���������ú� �ٲ���
	
	//����ǰ��
	NDK_WifiStop();//�����򿪻��Զ�����wifi���������
	
	//case1:�����쳣����NULL
	if((ret = NDK_WifiStatus(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:wifiδ����ȥ��ȡ״̬,Ӧ�÷���ʧ��
#if 0 //�����ȷ�Ϲ������ֻҪ����ʧ�ܻ��ȡ��ʧ��״̬������ͨ��  20180605 sull modify
	if((ret = NDK_WifiStatus(&status)) == NDK_OK/*!= NDK_ERR*/)//-5
//#else
	//if( (((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!= WIFI_WLAN_STATUS_FAIL)) && ((ret1 = NDK_WifiStatus(&status)) == NDK_OK))
#endif
	if((ret = NDK_WifiStatus(&status)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{
		if(((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL))
		{
			send_result("line %d: %s����ʧ��(%d,%d,status=%d)", __LINE__, TESTAPI, ret, ret1, status.status);
			GOTOERR;
		}
	}

	//case3:�����б�1��ap,���������,����start�������ӷ�������ʧ��WIFI_WLAN_STATUS_FAIL
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap->ssid,ssid[1]);
	strcpy(list_s.stored_ap->pwd,"9876543210");
	list_s.stored_ap->flag = WIFI_AP_FLAG_DHCP_SET;
	list_s.stored_ap->priority = 0;
	list_s.stored_num = 1;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret = NDK_WifiStatus(&status)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status.status);
			goto ERR;
		}
		//�����ȷ�ϣ�ģ�鱾���޷�׼ȷʶ����������ʱ��ֻ�ܷ�������ʧ�ܣ��ʻ�ȡ��FAIL״̬Ҳ��ͨ��
		if(status.status == WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED || status.status == WIFI_WLAN_STATUS_FAIL)
			break;
		if((time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d: %s����ʧ��,δ��ȡ����ȷ��״̬,��ʱ�˳�(status=%d)", __LINE__, TESTAPI, status.status);
			goto ERR;
		}
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:���ÿ��б�,��ȡ״̬�᷵��WIFI_WLAN_STATUS_EMPTY
	memset(set_ap,0,sizeof(set_ap));
	list_s.stored_num = 0;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if( ((ret=NDK_WifiStatus(&status))!=NDK_OK) || (status.status!= WIFI_WLAN_STATUS_EMPTY) )
	{
		send_result("line %d: %s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status.status);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//����WIFI_WLAN_STATUS_INIT ���ù���,��״̬���ò�׽��,�������������˿����Զ����Ӻ�,���ÿ��б����û�ȡ״̬�ܹ���ȡ��
	
	//case5:�б�:3��ap�������Ǵ������ô�����ӹ����л᷵���������ӵ�״̬WIFI_WLAN_STATUS_LINKING
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,"1234123412");
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET;
		list_s.stored_ap[i].priority = 0;
	}
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_HIDDEN_SET;
	list_s.stored_num = 3;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret = NDK_WifiStatus(&status)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(status.status == WIFI_WLAN_STATUS_LINKING)
			break;
		if((time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d: %s����ʧ��,δ��ȡ����ȷ��״̬,��ʱ�˳�(status=%d)", __LINE__, TESTAPI, status.status);
			goto ERR;
		}
	}
#if !WIFI_PRIORITY_ENABLE
	if( (ret=lib_getwifistatus(&status))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:�б�:������ȷ��ap,����start��᷵�سɹ���״̬,ssidΪԤ�ڵ�apWIFI_WLAN_STATUS_SUCC
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap->ssid,ssid[2]);
	strcpy(list_s.stored_ap->pwd,"1234567890");
	list_s.stored_ap->flag = WIFI_AP_FLAG_DHCP_SET;
	list_s.stored_ap->priority = 0;
	list_s.stored_num = 1;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret = NDK_WifiStatus(&status)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(status.status == WIFI_WLAN_STATUS_SUCC)
			break;
		if((time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d: %s����ʧ��,δ��ȡ����ȷ��״̬,��ʱ�˳�(status=%d)", __LINE__, TESTAPI, status.status);
			goto ERR;
		}
	}
	if(status.status!= WIFI_WLAN_STATUS_SUCC || strcmp(status.ssid,ssid[2]))
	{
		send_result("line %d: %s����ʧ��(status=%d,ssid:%s)", __LINE__, TESTAPI, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:����stop���ȡ״̬ Ӧ�÷���NDK_ERR ����WiFiģ�鷵��ֵ��ΪҪ����-3004  
	//�ҶϺ��ȡstatusԤ����Ҫ����-3004,����ȡ״̬����ok��ҲҪȷ����ȡ��״ֵ̬Ϊ-1 20180622 modify
	if( ((ret=NDK_WifiStatus(&status))!=NDK_ERR_WIFI_DEVICE_UNAVAILABLE) )
	{
		if(((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL))
		{
			send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
			goto ERR;
		}
	}

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_WifiStop();
	return;
}

