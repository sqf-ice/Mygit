/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan13.c
* Author 		: 
* version		: 
* DATE			: 
* directory 		: 
* description		: ����NDK_WiFiConnect_8021x�����ӵ�ָ����802.1x��֤ģʽ��AP����
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
#define TESTAPI	"NDK_WiFiConnect_8021x"	

/*------------global variables definition-----------------------*/
#define MAXTIMEOUT 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan13(void)
* functional description 	        : ���ӵ�ָ����802.1x��֤ģʽ��AP  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			jiangym		20161202   created		
*****************************************************************/
void wlan13(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0, numList = 0;
	char key[20]={0};
	ST_WIFI_8021x_PARAM param;
	EM_WPA_CONSTATE state;
	time_t diff = 0, oldtime = 0;
	ST_WIFI_APINFO ESSIDlist[64];	

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	//�����ڴ����ò���802.1x����ʾ��������
	if(linksetting.WLANEncMode!=WIFI_NET_SEC_WPA_EAP)
	{
		send_result("%s����������WIFI����ģʽΪ802.1xģʽ���ѡ���������в���", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ڴ����õ�ȫ�ֱ������ݸ���������
	param.comm_param.pszKey = linksetting.WLANPwd;
	param.comm_param.emKeyType = linksetting.WLANKeyType;
	param.comm_param.emSecMode = linksetting.WLANEncMode;
	param.comm_param.psEthIp = netsetting.LocalIP.Str;
	param.comm_param.psEthGateway = netsetting.LocalGateway.Str;
	param.comm_param.psEthNetmask =  netsetting.LocalMask.Str;
	param.comm_param.ucIfDHCP = netsetting.DHCPenable;
	param.comm_param.psEthDnsPrimary = netsetting.DNSHost[0];
	param.comm_param.psEthDnsSecondary = netsetting.DNSHost[1];
	param.emEap = linksetting.WLAN8021XEapMode;
	param.pszIdentity = linksetting.WLAN8021XIdentity;
	param.pszca_cert = NULL;
	
	//case1:����WIFI�豸δ��ʼ���͵���NDK_WiFiConnect_8021x����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
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

	//case2:Ӧ���ܹ���ȡ�����е�AP��Ϣ��������Ļ����ʾ������Ӧ�����ɴ����Ե�WPA/WPA2ģʽ��AP��Ϣ:����ssid,��ȫģʽ,�ź�ǿ�ȵ�
	memset(ESSIDlist,0,sizeof(ESSIDlist));
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetInfo(ESSIDlist,64, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg("Line=%d\nAP[%d]->Essid:%s\nsKeyModeStr:%s\nnFrequency:%d\nsSignal:%s", __LINE__,i, ESSIDlist[i].sEssid,ESSIDlist[i].sKeyModeStr,ESSIDlist[i].nFrequency,ESSIDlist[i].sSignal);
				break;
			}
		}
		sleep(1);
	}
	if (j == 3)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case3:�쳣�������ԣ������������ΪNULL,Ӧ�÷��ز�������
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, NULL)) != NDK_ERR_WIFI_INVDATA )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiConnect_8021x(NULL, &param)) != NDK_ERR_WIFI_INVDATA )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(1,"������case3����ͨ��");

	//case4:��������:Ԥ��Ӧ���ܹ����ӳɹ�
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
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
	cls_show_msg1(1,"������case4����ͨ��");

	//case5:�Ͽ����� Ӧ���ܹ��ɹ��һ�ȡ״̬��δ����״̬
	if((ret = NDK_WiFiDisconnect()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED)
	{
	    send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    GOTOERR;
	}
	if((ret=NDK_WiFiShutdown()) != NDK_OK)
	{
		 send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    	GOTOERR;
	}
	cls_show_msg1(1,"������case5����ͨ��");
	
	//case6:���Կ�����������SSID��·����
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}
	cls_show_msg("�뽫������·�������ó�����SSID,���������������");
	memset(ESSIDlist,0,sizeof(ESSIDlist));
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetInfo(ESSIDlist,64, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg("Line=%d\nAP[%d]->Essid:%s\nsKeyModeStr:%s\nnFrequency:%d\nsSignal:%s", __LINE__,i, ESSIDlist[i].sEssid,ESSIDlist[i].sKeyModeStr,ESSIDlist[i].nFrequency,ESSIDlist[i].sSignal);
				break;
			}
		}
		sleep(1);
	}
	if (j == 3)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	/*//��֤Ӧ��ɨ�費�����ص�SSID������������֮ǰ�Ѿ����ӹ���������Ȼ�ܹ���ɨ�赽��һֱһ����������������Բ����ԣ�
	for(i=0; i<numList; i++)
	{
		if(!strcmp(ESSIDlist[i],WlanEssid))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}*/
	//��֤�ܹ���������
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime = time(NULL);
	while(1)
	{
		if ((ret=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		show_stopwatch(1, diff);
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
	//�ҶϺ�ִ�к�������
	if((ret=NDK_WiFiShutdown()) != NDK_OK)
	{
		 send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    	GOTOERR;
	}
	cls_show_msg("case6����ͨ��,�뽫������·�������óɹ㲥��ʽ(������)");

	//case7:���Ӳ���������������
	//case7.1:�������ģʽ���ڷ�Χ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}
	param.comm_param.emSecMode = WIFI_NET_SEC_WPA_EAP+1;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);//ֱ�ӷ��ز���������ʱ5s����
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.comm_param.emSecMode = linksetting.WLANEncMode;
	
	//case7.2:��������ΪNULLʱӦ��ʧ�� 
	param.comm_param.pszKey = NULL;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);//����ΪNULL,ֱ�ӷ��ز�������
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.comm_param.pszKey=linksetting.WLANPwd;

	//case7.3:�������֤�û���Ϊ�շ���ʧ��
	param.pszIdentity = NULL;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.pszIdentity = linksetting.WLAN8021XIdentity;

	//case7.4:�����802.1x��֤��ʽ���ڷ�Χ�ڷ���ʧ��
	param.emEap = EAP_MODE_TTLS+1;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.emEap = linksetting.WLAN8021XEapMode;
	
	//case7.5:�����802.1x��֤��ʽ��·�������õĲ���ȷʱӦ�����Ӳ���
	if(param.emEap == EAP_MODE_TTLS)//EAP_MODE_PEAP
		param.emEap = EAP_MODE_PEAP;
	else if(param.emEap == EAP_MODE_PEAP)
		param.emEap = EAP_MODE_MD5;
	else if(param.emEap == EAP_MODE_MD5)
		param.emEap = EAP_MODE_TTLS;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)//�����������������Ϸ������ֱ�ӷ��سɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(50);//��̬����ʱ������,��ʱ50sȷ�����Ӻ���ִ�����
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED)//��ʵ�����ӽ��Ӧ�������Ӳ��ɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.emEap = linksetting.WLAN8021XEapMode;
	cls_show_msg1(1,"������case7.5����ͨ��");

	//case7.6:�������֤�û�������ȷӦ�����Ӳ���
	strcpy(param.pszIdentity, "ERRidentity");
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)//�����������������Ϸ������ֱ�ӷ��سɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(50);//��̬����ʱ������,��ʱ50sȷ�����Ӻ���ִ�����
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED)//��ʵ�����ӽ��Ӧ�������Ӳ��ɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.pszIdentity = linksetting.WLAN8021XIdentity;
	cls_show_msg1(1,"������case7.6����ͨ��");
	
	//case7.7:�������벻��ȷӦ�����Ӳ���
	memset(key, 0x00, sizeof(key));
	strcpy(key, "ERRpwd1234");
	param.comm_param.pszKey = key;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)//�����������������Ϸ������ֱ�ӷ��سɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(50);//��̬����ʱ������,��ʱ50sȷ�����Ӻ���ִ�����
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED)//��ʵ�����ӽ��Ӧ�������Ӳ��ɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	param.comm_param.pszKey =  linksetting.WLANPwd;
	cls_show_msg1(1,"������case7.7����ͨ��");
	
	//���ڿ�����IP �����أ����룬DNS��ΪNULL������¶�δ�������жϣ������Ǿ�̬���Ƕ�̬��ʽ����
	//�����ɵײ����Ӻ󷵻أ���̬���ӵ��ϣ���̬���Ӳ��ϣ�
    if(param.comm_param.ucIfDHCP)//���ڶ�̬����
    {
		//case6.8:IP��ַΪNULL
    	param.comm_param.psEthIp=NULL;
    	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(30);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_OK) //���ӳɹ�
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	param.comm_param.psEthIp=netsetting.LocalIP.Str;
		cls_show_msg1(1,"������case7.8����ͨ��");
    	
    	//case6.9:����ΪNULL
    	param.comm_param.psEthGateway=NULL;
    	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(30);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_OK) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	param.comm_param.psEthGateway=netsetting.LocalGateway.Str;
		cls_show_msg1(1,"������case7.9����ͨ��");
    	
    	//case6.10:��������ΪNULL
    	param.comm_param.psEthNetmask =NULL;
    	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(30);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_OK) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	param.comm_param.psEthNetmask=netsetting.LocalMask.Str;
		cls_show_msg1(1,"������case7.10����ͨ��");
    }
    else//���ھ�̬����
    {
		//case6.8:IP��ַΪNULL
		param.comm_param.psEthIp= NULL;
    	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(10);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	param.comm_param.psEthIp=netsetting.LocalIP.Str;
		cls_show_msg1(1,"������case7.8����ͨ��");
    	
    	//case6.9:����ΪNULL
    	param.comm_param.psEthGateway= NULL;
    	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(10);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	param.comm_param.psEthGateway=  netsetting.LocalGateway.Str;
		cls_show_msg1(1,"������case7.9����ͨ��");
    	
    	//case6.10:��������ΪNULL
    	param.comm_param.psEthNetmask = NULL;
    	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(10);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	param.comm_param.psEthNetmask=  netsetting.LocalMask.Str;
		cls_show_msg1(1,"������case7.10����ͨ��");

		//case6.11:DNS������ΪNULL
		param.comm_param.psEthDnsPrimary=NULL;
		if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		sleep(10);
		if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		param.comm_param.psEthDnsPrimary= netsetting.DNSHost[0];
		param.comm_param.psEthDnsSecondary = NULL;
		if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		sleep(10);
		if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		param.comm_param.psEthDnsPrimary = netsetting.DNSHost[0];
		param.comm_param.psEthDnsSecondary = netsetting.DNSHost[1];
		cls_show_msg1(1,"������case7.11����ͨ��");
}
	
	sleep(10);
	//case8:�쳣���Ժ��������Կ����Ƿ�����
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime = time(NULL);
	while(1)
	{
		if ((ret=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		show_stopwatch(1, diff);
		
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
	//�ҶϺ�ִ�к�������
	if((ret=NDK_WiFiShutdown()) != NDK_OK)
	{
		 send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    	GOTOERR;
	}
	cls_show_msg1(1,"������case8����ͨ��");

 	//case8:Ӧ���������õı���IP���������������õ����ز���ͬһ���������,����Ӧ�óɹ� 
 	//WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}
	param.comm_param.psEthIp = "40.40.40.103";
	param.comm_param.psEthGateway = "172.16.60.1";
	param.comm_param.psEthNetmask =  "255.255.255.0";
	param.comm_param.ucIfDHCP = 0;
	if ((ret=NDK_WiFiConnect_8021x(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime = time(NULL);
	while(1)
	{
		if ((ret=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		show_stopwatch(1, diff);
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
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	//���Ժ���:WIFI�豸�Ҷ�
	NDK_WiFiShutdown();
	return;
}

