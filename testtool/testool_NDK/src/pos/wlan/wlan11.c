/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WIFIAPģ��
* file name			: wifiap1
* Author 			: jiangym
* version			: 
* DATE				: 20141209
* directory 			: 
* description			: ����NDK_WiFiAPOpen��NDK_WiFiAPClose�Ƿ������������ر�wifi�ȵ㹦��
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"NDK_WiFiAPOpen,NDK_WiFiAPClose"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: wifiap1
* functional description 	:����NDK_WiFiAPOpen��NDK_WiFiAPClose
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	       jiangym			20141209		created
*****************************************************************/
void wlan11()
{
	/*private & local definition*/
	int ret = -1, i = 0;
	ST_WIFIAP_PARAM apparam = {0};
	char *secmodestr[] = {"NONE","WEP_OPEN","WEP_SHARED","WPA","WPA2"};
	char *errkey[] = 
	{
		"1234567",//���ּ���ģʽ���Ȳ���ȷ������
		"wifi12mima",//WIFI_NET_SEC_WEP_OPEN:10��ʮ�������ַ���0-9��A-F��:�Ǻ����ַ�����
		"wifi12mima1234567890wifi12",//WIFI_NET_SEC_WEP_ SHARED: 26��ʮ�������ַ���0-9��A-F��:�Ǻ����ַ�����
		"01234567890123456789012345678901234567890123456789012345678901234",//WIFI_NET_SEC_WPA��65�������ַ������ȷǷ�
		"01234567890123456789012345678901234567890123456789012345678901234",//WIFI_NET_SEC_WPA2��65�������ַ������ȷǷ�		
	};
    EM_WIFI_NET_SEC secmode[6] = 
	{
		WIFI_NET_SEC_WEP_OPEN,
		WIFI_NET_SEC_WEP_OPEN,
		WIFI_NET_SEC_WEP_SHARED,
		WIFI_NET_SEC_WEP_SHARED,
		WIFI_NET_SEC_WPA,
		WIFI_NET_SEC_NONE,
	};
	char *normalkey[] = 
	{
		"12345",//WIFI_NET_SEC_WEP_OPENģʽ����5λASCII����
		"12345678ab",//WIFI_NET_SEC_WEP_OPENģʽ����10λ16��������
		"0123456abc#%*",//WIFI_NET_SEC_WEP_SHAREDģʽ����13λASCII����
		"0123456789abcdef0123456789",//WIFI_NET_SEC_WEP_SHAREDģʽ����26λ16��������
		"12345678ab",//WIFI_NET_SEC_WPAģʽ����10λ16��������
		"12ab34#$%56wifi12mima01234567890wifi12",//WIFI_NET_SEC_NONEģʽ,���ж����볤�Ⱥ͸�ʽ
	};
	char *ssid[] = 
	{
		"wifi�ȵ�",
		"`~!\"@$#[^%]*() -",
		"_=+|\\&{}:;?,/><.",
	};
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ��:���ù�������
	if(conf_conn_AP()!=SUCC)
		return;
	//ʹ��ϵͳĬ�ϵ�AP�������г�ʼ��
	memset(&apparam, 0,sizeof(apparam));
	apparam.emShareDev = WifiApShareDev;
	apparam.emSecMode = WifiApSecMode;
	strcpy(apparam.szAPIP, netsetting.WifiApIp.Str);
	strcpy(apparam.szKey,WifiApKey);
	strcpy(apparam.szSsid , WifiApSsid);
	apparam.uchidden = WifiApHidden;

	if(cls_show_msg("�Ƿ�Ϊ������һ�����б�����:��[ENTER],��[����]")==ENTER)
	{
		//case1:��������δ���Ӵ������openʧ�ܣ�������ֻ�ܿ�����һ�β��ԣ����������豸�򿪺��һֱ�Ǵ�״̬
		if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_SHAREDEV_NOOPENED)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
	}

	//��������
	NetUp();
	
	//case2:�쳣��������
	//case2.1:��������粻��ö��ֵ��Χ��
	apparam.emShareDev = WIFIAP_SHARE_DEV_ETH-1;
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_NOSHAREDEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	apparam.emShareDev = WIFIAP_SHARE_DEV_PPP+1;
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_NOSHAREDEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	apparam.emShareDev = WifiApShareDev;
	
	//case2.2:���ܷ�ʽ����ö��ֵ��Χ��
	apparam.emSecMode = WIFI_NET_SEC_NONE-1;
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_SECMODE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	apparam.emSecMode = WIFI_NET_SEC_WPA2+1;
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_SECMODE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	apparam.emSecMode = WifiApSecMode;
	
	//case2.3:���õ�wifi�ȵ�����Ϊ��
	memset(apparam.szSsid,0,sizeof(apparam.szSsid));
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_NOSSID)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	strcpy(apparam.szSsid , WifiApSsid);
	
	//case2.4:���õ�wifi�ȵ����Ƴ��ȳ���32λ
	strcpy(apparam.szSsid,"aaaaaaaaaabbbbbbbbbbccccccccccddd");//33λ
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_SSIDLEN)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;
	}	
	strcpy(apparam.szSsid , WifiApSsid);

	//case2.5:���Ը������ģʽ���볤�Ȳ���ȷ���ַ�������Ҫ������£���APӦ��ʧ��
	//WIFI_NET_SEC_WEP_OPEN: 5��ASCII�ַ�����10��ʮ�������ַ���0-9��A-F������Կ����Ϊ10ʱ��ģ����Զ���Ϊ��ʮ�������ַ���
	//WIFI_NET_SEC_WEP_ SHARED: 13��ASCII�ַ�����26��ʮ�������ַ���0-9��A-F������Կ����Ϊ26ʱ��ģ����Զ���Ϊ��ʮ�������ַ���
	//WIFI_NET_SEC_WPA����WIFI_NET_SEC_WPA2����������8-64�������ַ���
	for(i=WIFI_NET_SEC_WEP_OPEN;i<=WIFI_NET_SEC_WPA2;i++)
	{
		apparam.emSecMode = i;
		memset(apparam.szKey,0,sizeof(apparam.szKey));
		strcpy(apparam.szKey,errkey[0]);
		if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_KEYLEN)
		{
			send_result("line %d:%s����ʧ��(i=%d)(%d)", __LINE__, TESTAPI, i, ret);
			GOTOERR;
		}
		memset(apparam.szKey,0,sizeof(apparam.szKey));
		strcpy(apparam.szKey,errkey[i]);
		if(i==1||i==2)
		{
			if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_KEYTYPE)
			{
				send_result("line %d:%s����ʧ��(i=%d)(%d)", __LINE__, TESTAPI, i, ret);
				GOTOERR;
			}
		}
		else
		{
			if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_KEYLEN)
			{
				send_result("line %d:%s����ʧ��(i=%d)(%d)", __LINE__, TESTAPI, i, ret);
				GOTOERR;
			}
		}
	}
	apparam.emSecMode = WifiApSecMode;
	strcpy(apparam.szKey,WifiApKey);

	//case2.6:���ô����wifi�ȵ�IP
	strcpy(apparam.szAPIP,"192.168.1");
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_IPTYPE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;
	}
	strcpy(apparam.szAPIP, netsetting.WifiApIp.Str);

	//case3:�������������̫������������ΪPPPӦ�÷���ʧ��;�����������PPP,������̫��Ӧ��ʧ��
	if(WifiApShareDev==WIFIAP_SHARE_DEV_ETH)
		apparam.emShareDev = WIFIAP_SHARE_DEV_PPP;	
	else
		apparam.emShareDev = WIFIAP_SHARE_DEV_ETH;	
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_ERR_WIFIAP_SHAREDEV_NOOPENED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	apparam.emShareDev = WifiApShareDev;

	//case4:��������:���ò�ͬ�ļ���ģʽ���в��Ը��ֺ�����������µĲ��ԣ�Ԥ��Ӧ�÷��سɹ�
	//case4.1:WIFI_NET_SEC_WEP_OPENģʽ����5λASCII����
	//case4.2:WIFI_NET_SEC_WEP_OPENģʽ����10λ16��������
	//case4.3:WIFI_NET_SEC_WEP_SHAREDģʽ����13λASCII����
	//case4.4:WIFI_NET_SEC_WEP_SHAREDģʽ����26λ16��������
	//case4.5:WIFI_NET_SEC_WPAģʽ����10λ16��������
	//case4.6:WIFI_NET_SEC_NONEģʽ,���ж����볤�Ⱥ͸�ʽ
	memset(&apparam, 0,sizeof(apparam));
	apparam.emShareDev = WifiApShareDev;
	strcpy(apparam.szAPIP, netsetting.WifiApIp.Str);
	strcpy(apparam.szSsid , WifiApSsid);
	apparam.uchidden = WifiApHidden;
	
	for(i=0;i<SZ_ARRAY(normalkey);i++)
	{
		apparam.emSecMode = secmode[i];
		memset(apparam.szKey,0,sizeof(apparam.szKey));
		strcpy(apparam.szKey,normalkey[i]);
		//cls_show_msg("SSID:%s\n����ģʽ:%s\n����:%s\n",apparam.szSsid,secmodestr[apparam.emSecMode],apparam.szKey);
		if((ret=NDK_WiFiAPOpen(apparam))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)(i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		if(apparam.emSecMode==WIFI_NET_SEC_NONE)
			cls_show_msg("ʹ���ֻ����������ȵ�:%s,����ģʽ:%s,����:��������",apparam.szSsid,secmodestr[apparam.emSecMode]);
		else
			cls_show_msg("ʹ���ֻ����������ȵ�:%s,����ģʽ:%s,����:%s",apparam.szSsid,secmodestr[apparam.emSecMode],apparam.szKey);
		//�ж��Ƿ��ܹ�ɨ��������
		if(cls_show_msg("�ֻ�Ӧ���ܹ��ɹ������ȵ㲢����,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��(i=%d)", __LINE__, TESTAPI, i);
			GOTOERR;
		}
	}
	apparam.emSecMode = WifiApSecMode;
	strcpy(apparam.szKey,WifiApKey);

	//case5:�ر�ap��ɨ�費��,Ҳ���Ӳ���
	if((ret=NDK_WiFiAPClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("ʹ���ֻ������ȵ�:%s,Ӧ����������",apparam.szSsid);
	//�ж��Ƿ��ܹ�ɨ��������
	if(cls_show_msg("�ֻ����������ȵ�:%s,��ȷ[Enter],����[����]",apparam.szSsid)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	

	//case6:�رպ��ٵ��ùر� 
	if((ret=NDK_WiFiAPClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7:�������ص�wifi�ȵ�ʹ���ֻ�Ӧ���������������ܹ��������ɹ�
	apparam.uchidden = 1;
	if((ret=NDK_WiFiAPOpen(apparam))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ж��Ƿ��ܹ�������
	if(cls_show_msg("ʹ���ֻ�����wifi�ȵ�:%s,Ӧ����������,��ȷ[Enter],����[����]",apparam.szSsid)!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("ʹ���ֻ����wifi�ȵ�:%s,����ģʽ:%s,����:%s",apparam.szSsid,secmodestr[apparam.emSecMode],apparam.szKey);
	//�ж��Ƿ��ܹ���ӳɹ�
	if(cls_show_msg("ʹ���ֻ��ܹ��ɹ������ȵ㲢����,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}		
	apparam.uchidden = WifiApHidden;

	//case8:wifi�ȵ����������Ϊ������,Ӧ���ܳɹ�
	//case9:wifi�ȵ����������Ϊ�����ַ�,Ӧ���ܳɹ�
	for(i = 0;i<SZ_ARRAY(ssid);i++)
	{
		strcpy(apparam.szSsid,ssid[i]);
		if((ret=NDK_WiFiAPOpen(apparam))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg("ʹ���ֻ����������ȵ�:%s,����ģʽ:%s,����:%s",apparam.szSsid,secmodestr[apparam.emSecMode],apparam.szKey);
		//�ж��Ƿ��ܹ�ɨ��������
		if(cls_show_msg("�ֻ�Ӧ���ܹ��ɹ������ȵ㲢����,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��(i=%d)", __LINE__, TESTAPI, i);
			GOTOERR;
		}
	}
	strcpy(apparam.szSsid , WifiApSsid);

	send_result("%s����ͨ��", TESTAPI);
ERR:
	//�ָ�ȫ�ֱ���
	NDK_WiFiAPClose();
	NetDown();
	return;
}

