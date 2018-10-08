/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan4.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiConnect,NDK_WiFiIsConnected�ӿڹ���
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
#define TESTAPI	"NDK_WiFiConnect,NDK_WiFiIsConnected"	

/*------------global variables definition-----------------------*/
#define MAXTIMEOUT 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan4(void)
* functional description 	: ��ȡ����״̬��Ϣ ,�ж��Ƿ�������AP  
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					chensj   201305013	created
*****************************************************************/
void wlan4(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0, numList = 0;
	char key[20]={0};
	ST_WIFI_PARAM param;
    EM_WPA_CONSTATE state;
	time_t diff = 0, oldtime = 0;
	char szIp[32]={0};
	EM_WIFI_NET_SEC sec;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];
	
	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

    param.pszKey =  linksetting.WLANPwd;
	param.emKeyType = linksetting.WLANKeyType;
	param.emSecMode = linksetting.WLANEncMode;
	param.psEthIp = netsetting.LocalIP.Str;
	param.psEthGateway = netsetting.LocalGateway.Str;
	param.psEthNetmask =  netsetting.LocalMask.Str;
	param.ucIfDHCP = netsetting.DHCPenable;
	param.psEthDnsPrimary = netsetting.DNSHost[0];
	param.psEthDnsSecondary = netsetting.DNSHost[1];
	
	//case1:����WIFI�豸δ��ʼ���͵���NDK_WiFiConnect����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiConnect("XXX", &param)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//����ǰ��:WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}

	//ɨ��AP,����ɨ��3��
	for (j=0; j<3; j++) 
	{
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList)) == NDK_OK) 
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
	}

	//case2:�쳣�������ԣ������������ΪNULL,Ӧ�÷��ز�������
	if ((ret=NDK_WiFiConnect("XXX", NULL)) != NDK_ERR_WIFI_INVDATA )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiConnect(NULL, &param)) != NDK_ERR_WIFI_INVDATA )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(1,"������case2����ͨ��");

#if 1
	//case3:����WIFI֮ǰ����NDK_WiFiIsConnected����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif//�������ײ㷵��NDK_ERR_WIFI_DEVICE_UNAVAILABLE �豸��Ч�����ʣ��ײ��޸�

	//case4:��������:
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:�������Ӻ��״̬�仯
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
	
	//case8:����������֤WIFIģ��DHCP�����ܹ�������ȡ����ַ,��ȡ�ĵ�ַӦ�ø���̬���õĲ�һ��
	if(netsetting.DHCPenable==ENABLE)
	{
		memset(szIp, 0, sizeof(szIp));
		//if((ret = NDK_EthGetNetAddr(szIp,NULL,NULL,NULL)) != NDK_OK)
		if((ret=NDK_NetGetAddr(COMM_TYPE_WIFI,szIp,NULL,NULL,NULL))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		/*
		if(!strcmp(szIp,netsetting.LocalIP.Str))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}*/
		//20130813 linwl�޸��жϷ�ʽΪ�˹��жϣ���Ϊ����ڴ��������ж�ʱ�Ѿ��޸�netsetting.LocalIP.StrΪ��̬���ɵ���IPֵ��������ıȽϷ�����Ч
		if(cls_show_msg("WIFI��̬��ȡ����IP��ַΪ:%s,ȷ�ϼ�ͨ��,������ʧ��", szIp)!=ENTER)
		{
			send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szIp);
			GOTOERR;
		}
	}
	
	if((ret=NDK_WiFiShutdown()) != NDK_OK)
	{
		 send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    	GOTOERR;
	}
	cls_show_msg1(3,"������case3��case4����ͨ��");
	
	//WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}
	//case7:���Կ�����������SSID��·����
	cls_show_msg("�뽫������·�������ó�����SSID,���������������");
	//ɨ��AP,����ɨ��3��
	for (j=0; j<3; j++) 
	{
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList)) == NDK_OK) 
		{
			if (numList > 0) 
			{
				for (i=0; i<numList; i++)
					cls_show_msg("��ɨ�赽%d��AP,line:%d,AP[%d]=%s", numList,__LINE__, i, ESSIDlist1[i]);
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
	//��֤Ӧ��ɨ�費�������ص�AP
	for(i=0; i<numList; i++)
	{
		if(!strcmp(ESSIDlist1[i],WlanEssid))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	//��֤�ܹ���������
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

	//case8:���ӹ�һ��֮����ɨ��Ӧ����Ȼɨ�費��
	//ɨ��AP,����ɨ��3��
	for (j=0; j<3; j++) 
	{
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList)) == NDK_OK) 
		{
			if (numList > 0) 
			{
				for (i=0; i<numList; i++)
					cls_show_msg("��ɨ�赽%d��AP,line:%d,AP[%d]=%s", numList,__LINE__, i, ESSIDlist1[i]);
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
	//��֤Ӧ��ɨ�費�������ص�AP
	for(i=0; i<numList; i++)
	{
		if(!strcmp(ESSIDlist1[i],WlanEssid))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	
	//�ҶϺ�ִ�к�������
	if((ret=NDK_WiFiShutdown()) != NDK_OK)
	{
		 send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    	GOTOERR;
	}
	cls_show_msg("case7����ͨ��,�뽫������·�������óɹ㲥��ʽ(������)");

	//WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}

	//case6:���Ӳ���������������
	//case6.1:�������ģʽ���ں���Χ��
	param.emSecMode = WIFI_NET_SEC_WPA2+1;
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);//ֱ�ӷ��ز���������ʱ5s����
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(3,"������case6.1����ͨ��");

	//case6.2:�������ģʽ��·��������ģʽ��һ��
	//ɨ��AP,����ɨ��3��
	for (j=0; j<3; j++) 
	{
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList)) == NDK_OK) 
		{
			if (numList > 0) 
			{
				for (i=0; i<numList; i++)
					cls_show_msg("��ɨ�赽%d��AP,line:%d,AP[%d]=%s", numList,__LINE__, i, ESSIDlist1[i]);
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
	NDK_WiFiGetSec(WlanEssid, &sec);//���ӻ�ȡ·�������ܷ�ʽ
	if(sec == WIFI_NET_SEC_WEP_OPEN||sec == WIFI_NET_SEC_WEP_SHARED)//���·��������ģʽ��WEP �������WPA 20140221 linwl
		param.emSecMode = WIFI_NET_SEC_WPA2;
	else//·��������ģʽʹ��NONE��WPA����²����� WEPģʽ
		param.emSecMode = WIFI_NET_SEC_WEP_OPEN;
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)//�����������������Ϸ������ֱ�ӷ��سɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(50);//��̬����ʱ������,��ʱ50sȷ�����Ӻ���ִ�����
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/)//��ʵ�����ӽ��Ӧ�������Ӳ��ɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(3,"������case6.2����ͨ��");
	param.emSecMode=linksetting.WLANEncMode;

	//case6.3:�������벻��ȷ
	memset(key, 0x00, sizeof(key));
	strcpy(key, "ERRpwd1234");
	param.pszKey = key;
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)//�����������������Ϸ������ֱ�ӷ��سɹ�
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(param.emKeyType!=WIFI_KEY_TYPE_NOKEY)//���·�������������룬��ʵ������Ӧ��ʧ�� 20140221 linwl
	{
		sleep(50);//��̬����ʱ������,��ʱ50sȷ�����Ӻ���ִ�����
		if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/)//��ʵ�����ӽ��Ӧ�������Ӳ��ɹ�
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(3,"������case6.3����ͨ��");
	}
	else//���·���������룬��ʵ������Ӧ�ɹ�
	{
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
		//WIFI�豸��ʼ��
		if ((ret=NDK_WiFiInit()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			return;
		}
	}

	//case6.4:��������ΪNULL,·����������ʱӦ�óɹ���·����������ʱӦ��ʧ�� 20140221 linwl
	param.pszKey = NULL;
	if(param.emKeyType!=WIFI_KEY_TYPE_NOKEY)//���·�������������룬��ʵ������Ӧ��ʧ��
	{
		if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		sleep(10);//����ΪNULL,ֱ�ӷ��ز�������
		if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	else
	{
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
		//WIFI�豸��ʼ��
		if ((ret=NDK_WiFiInit()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			return;
		}
	}
	cls_show_msg1(3,"������case6.4����ͨ��");
	param.pszKey=linksetting.WLANPwd;
	
	//���ڿ�����IP �����أ����룬DNS��ΪNULL������¶�δ�������жϣ������Ǿ�̬���Ƕ�̬��ʽ����
	//�����ɵײ����Ӻ󷵻أ���̬���ӵ��ϣ���̬���Ӳ��ϣ�
	//case6.5:IP��ַΪNULL//���ﶯ̬��ʽIP�᷵��-3006 �����ص�ΪNULL����OK �в���
    if(param.ucIfDHCP)//���ڶ�̬����
    {
    	param.psEthIp=NULL;
    	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
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
    	cls_show_msg1(3,"������case6.5����ͨ��");
    	param.psEthIp=netsetting.LocalIP.Str;
    	
    	//case6.6:����Ϊ��
    	param.psEthGateway=NULL;
    	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
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
    	cls_show_msg1(1,"������case6.6����ͨ��");
    	param.psEthGateway=netsetting.LocalGateway.Str;
    	
    	//case6.7:��������Ϊ��
    	param.psEthNetmask =NULL;
    	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
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
    	cls_show_msg1(1,"������case6.7����ͨ��");
    	param.psEthNetmask=netsetting.LocalMask.Str;
    }
    else//���ھ�̬����
    {
		param.psEthIp= NULL;
    	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(10);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	cls_show_msg1(3,"������case6.5����ͨ��");
    	param.psEthIp=netsetting.LocalIP.Str;
    	
    	//case6.6:����Ϊ��
    	param.psEthGateway= NULL;
    	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(10);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	cls_show_msg1(1,"������case6.6����ͨ��");
    	param.psEthGateway=  netsetting.LocalGateway.Str;
    	
    	//case6.7:��������Ϊ��
    	param.psEthNetmask = NULL;
    	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	sleep(10);
    	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
    	{
    		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
    		GOTOERR;
    	}
    	cls_show_msg1(1,"������case6.7����ͨ��");
    	param.psEthNetmask=  netsetting.LocalMask.Str;

		//case6.10:DNS������ΪNULL�������
		param.psEthDnsPrimary=NULL;
		if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		sleep(10);
		if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(3,"������6.10����ͨ��");
		
		param.psEthDnsPrimary= netsetting.DNSHost[0];
		param.psEthDnsSecondary = NULL;
		if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_ERR_WIFI_INVDATA)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		sleep(10);
		if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED/*NDK_ERR_WIFI_DEVICE_UNAVAILABLE*/) 
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(3,"������6.11����ͨ��");
		
		param.psEthDnsPrimary = netsetting.DNSHost[0];
		param.psEthDnsSecondary = netsetting.DNSHost[1];
}


	//case6.8:�������Ͳ��ԣ���ȷ������������WIFI_KEY_TYPE_ASCII
    /*���մ��������һ������ASCII��,Ҳû�б�Ҫ���ж�ʮ�����Ƶ�ASCII��ת��
	param.emKeyType = WIFI_KEY_TYPE_HEX;
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(30);
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) 
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(1,"������case6.8����ͨ��");
    */

        
	//case6.9:�������Ͳ��ԣ���ȷ������������WIFI_KEY_TYPE_ASCII
	/*������������ΪNOKEY������֮ǰ��szKeyΪNULL�����
	param.emKeyType = WIFI_KEY_TYPE_NOKEY;
	if ((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(30);
	if ((ret=NDK_WiFiIsConnected()) != NDK_ERR_WIFI_NON_CONNECTED) //�˴������⣨���������ϣ�����������ʵ��ΪASCII
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(1,"������case6.9����ͨ��");
	param.emKeyType = WIFI_KEY_TYPE_ASCII;
	*/
	
	sleep(10);
	//case7:�쳣���Ժ��������Կ����Ƿ�����
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

	//case8:Ӧ���������õı���IP���������������õ����ز���ͬһ���������,����Ӧ�óɹ� 20160608
	//WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		return;
	}
	param.pszKey =  linksetting.WLANPwd;
	param.emKeyType = linksetting.WLANKeyType;
	param.emSecMode = linksetting.WLANEncMode;
	param.psEthIp = "40.40.40.103";
	param.psEthGateway = "172.16.60.1";
	param.psEthNetmask =  "255.255.255.0";
	param.ucIfDHCP = 0;
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
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		show_stopwatch(1, diff);
		//send_result("status=%d", state);
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



