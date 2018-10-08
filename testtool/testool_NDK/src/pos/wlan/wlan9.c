/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan9.c
* Author 		: chensj
* version		: 
* DATE			: 20131127
* directory 		: 
* description		: ����NDK_NetGetAddr�ӿڹ���
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
#define TESTAPI	"NDK_NetGetAddr"	

/*------------global variables definition-----------------------*/
#define MAXTIMEOUT 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan9(void)
* functional description 	: ��ȡMac ��ַ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  				chensj   20131127	created
*****************************************************************/
void wlan9(void)
{
	int ret = 0,ret1=0,i=0;
	char szIPaddr[16] = {0}, szMaskaddr[16] = {0}, szGateway[16] = {0}, szDns[16*3+2] = {0}, tempDns1[16*3+2]={0}, tempDns2[16*3+2]={0};
	ST_WIFI_PARAM param;
	time_t diff = 0, oldtime = 0;
	EM_WPA_CONSTATE state;

	/*process body*/
	if(auto_flag==2)
		return;
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������	
	
	//case1:δ��ʼ����ȡ��ַӦ��ʧ��
	if((ret=NDK_NetGetAddr(COMM_TYPE_WIFI, szIPaddr, szMaskaddr, szGateway, NULL)) != NDK_ERR_NET_GETADDR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	

	//����ǰ��:WIFI�豸��ʼ��
	if((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
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

	if((ret=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	oldtime = time(NULL);
	while(1)
	{
		if((ret=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
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

	if((ret=NDK_WiFiIsConnected()) != NDK_OK)
	{
	    send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
	    GOTOERR;
	}

	//�쳣��������
	if((ret=NDK_NetGetAddr(COMM_TYPE_WIFI, NULL, NULL, NULL, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//�쳣��������
	if(((ret=NDK_NetGetAddr(COMM_TYPE_ETH-1, szIPaddr, szMaskaddr, szGateway, NULL)) != NDK_ERR_PARA)
		||((ret1=NDK_NetGetAddr(COMM_TYPE_UNKNOW, szIPaddr, szMaskaddr, szGateway, NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}

	//case2:�������ܲ��ԣ����Ƿ���Ի�ȡ��IP���������롢����
	if((ret=NDK_NetGetAddr(COMM_TYPE_WIFI, szIPaddr, szMaskaddr, szGateway, szDns)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(param.ucIfDHCP)//��̬��ַ
	{
		if(auto_flag==0)
		{
			cls_show_msg1(3,"��ȡwifi��ַ�ɹ�IP:%s",szIPaddr);
			cls_show_msg1(3,"��ȡwifi��ַ�ɹ���������:%s",szMaskaddr);
			cls_show_msg1(3,"��ȡwifi��ַ�ɹ�����:%s",szGateway);
			cls_show_msg1(3,"��ȡwifi��ַ�ɹ�DNS:%s",szDns);
			if(cls_show_msg("��ȡ��IP���������롢���ء�DNS�Ƿ����?[ENTER]���� [����]������")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
		}
		else
			send_result("��ȡwifi��ַ�ɹ�IP:%s ��������:%s ����:%s DNS:%s,������Ƿ����",szIPaddr,szMaskaddr,szGateway,szDns);
	}
	else//��̬��ַӦ�ú����Ӳ���һ��
	{
		sprintf(tempDns1,"%s;%s",netsetting.DNSHost[0],netsetting.DNSHost[1]);
		sprintf(tempDns2,"%s;%s",netsetting.DNSHost[1],netsetting.DNSHost[0]);
		if(strcmp(netsetting.LocalIP.Str,szIPaddr)||strcmp(netsetting.LocalGateway.Str,szGateway)||strcmp(netsetting.LocalMask.Str,szMaskaddr)||((strcmp(tempDns1,szDns)!=0)&&(strcmp(tempDns2,szDns)!=0)))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}

	//���Ժ���:WIFI�豸�Ҷ�
	if((ret = NDK_WiFiShutdown()) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3:�ҶϺ�,��ȡ��ַӦ�÷���ʧ��
	memset(szIPaddr,0,sizeof(szIPaddr));
	memset(szMaskaddr,0,sizeof(szMaskaddr));
	memset(szGateway,0,sizeof(szGateway));
	memset(szDns,0,sizeof(szDns));
	if((ret=NDK_NetGetAddr(COMM_TYPE_WIFI, szIPaddr, szMaskaddr, szGateway, szDns)) != NDK_ERR_NET_GETADDR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=0;i<16;i++)
	{	
		if(szIPaddr[i]!='\0'||szMaskaddr[i]!='\0'||szGateway[i]!='\0'||szDns[i]!='\0')
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_WiFiShutdown();
	return;
}

