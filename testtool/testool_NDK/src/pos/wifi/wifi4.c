/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��Wifiģ��
* file name		: wifi4.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_WifiGetFunc�Ƿ��ܹ���ȷ��ȡwifi�����Ϣ����
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
#define TESTAPI	"NDK_WifiGetFunc"	

extern int scan_get_mac(uchar outmac[6]); 

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
char * displaywifimode(int modevalue)
{
	/*private & local definition*/
	char *sectypestr[] = {"WIFI_AUTH_OPEN", "WIFI_AUTH_WEP_OPEN", "WIFI_AUTH_WEP_SHARED", "WIFI_AUTH_WPA_TKIP_PSK", "WIFI_AUTH_WPA_AES_PSK", "WIFI_AUTH_WPA_MIXED_PSK", "WIFI_AUTH_WPA2_AES_PSK", "WIFI_AUTH_WPA2_TKIP_PSK", "WIFI_AUTH_WPA2_MIXED_PSK", "WIFI_AUTH_WPS_OPEN", "WIFI_AUTH_WPS_SECURE"};
	char *tmp = "δ֪�ļ���ģʽ";
	
	/*process body*/
	if(modevalue<WIFI_AUTH_OPEN||modevalue>WIFI_AUTH_WPS_SECURE)
		return tmp;
	
	tmp = sectypestr[modevalue];
	return tmp;
}

char * displaywififrequency(int frequencyvalue)
{
	/*private & local definition*/
	char *frequencystr[] = {"5G", "2.4G"};
	char *tmp = "δ֪��Ƶ��";
	
	/*process body*/
	if(frequencyvalue<0 || frequencyvalue>1)
		return tmp;
	
	tmp = frequencystr[frequencyvalue];
	return tmp;
}
/****************************************************************
* function name 	 				: void wifi4(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  				jiangym		20160301	created	 
*****************************************************************/
void wifi4(void)
{
	/*private & local definition*/
	int ret = 0, i = 0;
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_AP_ENTRY_T st_ap;
	list_s.stored_ap = &st_ap;
	ST_WIFI_AP_LIST list_scan;
	ST_WIFI_AP_INFO ap[32];
	list_scan.list = ap;
	ST_ETHER_ADDR_T mac;
	ST_WIFI_AP_INFO succ_ap;
	ST_WIFI_STATUS status;
#if defined ME68 || defined ME50NX  //����ƽ̨��֧�ֶ���  20180515  modify
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?/><.";
#else
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.";
#endif
	char pwd[] = "1234567890"; //�˴��������޸�AP���ּ�����Ͷ�Ӧ��MAC
	//uchar default_mac[6] = {0x78,0x54,0x2e,0x59,0xdd,0x3a};
	uchar ap_mac[6] = {0};
	char pInfo[64] = {0}, mactemp[64] = {0};
	char ssidout[32][33];
	uint ssidoutlen = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_WifiStart();
	if((ret = lib_getwifistatus(&status)) != NDK_OK)
	{
		send_result("line %d:��ȡwifi״̬ʧ��(ret=%d,status=%d)", __LINE__, ret, status.status);
		return;
	}
	//�������ļ��л�ȡap Mac  �������ļ���ͨ��ɨ���ȡ
	if(NDK_FsExist(PARACONF)!=NDK_OK)
	{	
		cls_show_msg1(3,"�޲��������ļ�,��ͨ��ɨ���ȡmac");
		//ͨ��ɨ���ȡMac���붨���Mac������
		cls_show_msg1(5,"��ȡ����AP:%s��Mac",test_ap);
		if((ret = scan_get_mac(ap_mac)) != SUCC)
		{
			send_result("line %d:��ȡ����AP Macʧ��(%d)", __LINE__, ret);
			return;
		}
	}
	else
	{
		if(GetParaInfo("WifiMac3",pInfo)!=SUCC)
		{	
			send_result("line %d:����Mac��Ϣʧ��", __LINE__ );
			return;
		}
		if(trim_string(pInfo,mactemp)!=SUCC)
		{	
			send_result("line %d:��ȡMac��Ϣʧ��", __LINE__ );
			return;
		}
		if(trim_char2uchar(mactemp,ap_mac)!=SUCC)
		{	
			send_result("line %d:ת��Mac��ʽʧ��", __LINE__ );
			return;
		}
	}
	NDK_WifiStop();//�պ�Ҳ��Ϊ��������
	
	//case1:������װ��֮ǰ,���øú�����Ӧ�ó����ܷɵ��쳣
	if ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan)) != NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(ap,0,sizeof(ap));
	list_scan.num = 32;//����ֵΪ32
	if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num==0) )
	{
		send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
		GOTOERR;
	}
	
	//����ǰ��:ɨ�蹦����Ҫ��start(wifi��������)����״̬ҪΪ�ȶ�(�����ա�ʧ�ܡ��ɹ�����״̬)ʱ���ܷ��سɹ�
	NDK_WifiStart();
	if((ret = lib_getwifistatus(&status)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�����쳣����:WIFI_GET_FUNC_SCAN_RESULT-1,WIFI_GET_FUNC_UNDEFINE+1Ӧ�÷��ز�������
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT-1,&list_scan)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_UNDEFINE+1,&list_scan)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_UNDEFINE,&list_scan)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_MAC, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:���Ի�ȡɨ����Ϣ:����ֵ��0�����
	cls_printf("����ɨ��AP��Ϣ...");
	memset(ap,0,sizeof(ap));
	list_scan.num = 0;//����ֵΪ0
	if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num!=0) )
	{
		send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
		GOTOERR;
	}
	
	//case4:���Ի�ȡɨ����Ϣ:�ж��ap(���������ַ����������ַ���ap)ʱ���ؽ��,ϣ����ȡ3��С�ڵײ�ɨ�赽��ʵ�ʸ���ʱӦ����3��
	memset(ap,0,sizeof(ap));
	list_scan.num = 3;//����ֵΪ3
	if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num!=3) )
	{
		send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
		GOTOERR;
	}
	
	//case5:���Ի�ȡɨ����Ϣ:����ֵ128>ʵ�ʻ�ȡ������10,��Ӧ�÷���ʵ�ʻ�ȡ���ĸ���,��Ϣ����: num ssid auth_mode frequency rssi channel mac
	memset(ap,0,sizeof(ap));
	list_scan.num =32/*128*/;//ϣ���������ó��㹻��,���Ի���ҲҪ���������㹻���AP����ɨ��,���Ը���
	if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num==0) )
	{
		send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
		GOTOERR;
	}
	//Ԥ�ڵĽ��������ȷ��,����������ʾ,����ʾ������Ա������ʾ�Ľ���Ƿ���ȷ:�����ʾ���쳣������������SSID����,SSID����,��֤ģʽ����,�źŲ��ں���Χ�ڵ�
	if (list_scan.num > 0) 
	{	
		memset(ssidout,0,sizeof(ssidout));
		send_result("��ɨ�赽%d��AP����Ϣ����(�����ȡ��AP��Ϣ�Ƿ���ȷ):", list_scan.num); 
		for (i=0; i<list_scan.num; i++)
		{
			if(!strcmp(ap[i].ssid,""))//���������ap  ֱ�Ӵ������buf
				strcpy(ssidout[i],ap[i].ssid);
			else
			{
				if((ret=NDK_Utf8ToGbk((uchar *)ap[i].ssid,strlen(ap[i].ssid),(uchar *)ssidout[i],&ssidoutlen))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					GOTOERR;
				}
			}
			send_result("AP[%d]:%s,ģʽ:%s,�ŵ�:%d,Ƶ��:%s,�ź�:%d,mac:%02x:%02x:%02x:%02x:%02x:%02x", i, ssidout[i], displaywifimode(ap[i].auth_mode),
			ap[i].channel, displaywififrequency(ap[i].frequency), ap[i].rssi, ap[i].mac.octet[0], ap[i].mac.octet[1], ap[i].mac.octet[2], ap[i].mac.octet[3], ap[i].mac.octet[4], ap[i].mac.octet[5]);
		}
	}
	if(auto_flag==0||auto_flag==2)
	{
		if(cls_show_msg("������ʾ��AP��Ϣ�Ƿ�����,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}

	//case6:����ssid��AP��Ӧ�ñ�ɨ�赽
	if(auto_flag==0||auto_flag==2)
	{
		cls_show_msg("������ĳ̨AP,�䲻Ӧ�ٱ�ɨ�赽,����15�����Һ����������...");
		memset(ap,0,sizeof(ap));
		if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num==0) )
		{
			send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
			GOTOERR;
		}
		if (list_scan.num > 0)//Ԥ�ڵĽ��������ȷ��,����������ʾ,����ʾ������Ա���ɨ�赽��AP��Ӧ�ò����б����ص�SSID
		{
			memset(ssidout,0,sizeof(ssidout));
			send_result("����ĳ̨AP��ɨ�赽%d��AP����Ϣ����:", list_scan.num);
			for (i=0; i<list_scan.num; i++)
			{
				if(!strcmp(ap[i].ssid,""))//���������ap  ֱ�Ӵ������buf
					strcpy(ssidout[i],ap[i].ssid);
				else
				{
					if((ret=NDK_Utf8ToGbk((uchar *)ap[i].ssid,strlen(ap[i].ssid),(uchar *)ssidout[i],&ssidoutlen))!=NDK_OK)
					{
						send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
						GOTOERR;
					}
				}
				send_result("AP[%d]:%s,mac:%02x:%02x:%02x:%02x:%02x:%02x", i, ssidout[i], ap[i].mac.octet[0], ap[i].mac.octet[1], ap[i].mac.octet[2], ap[i].mac.octet[3], ap[i].mac.octet[4], ap[i].mac.octet[5]);
			}
		}
		if(cls_show_msg("����֮ǰ���ص�AP�Ƿ��ٱ�ɨ�赽,�ǰ�[ENTER],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}


	//case7:�رյ�AP��Ӧ���ٱ�ɨ�赽
	if(auto_flag==0||auto_flag==2)
	{
		cls_show_msg("��ر�ĳ̨AP,�䲻Ӧ�ٱ�ɨ�赽,�ر�15�����Һ����������...");
		memset(ap,0,sizeof(ap));
		if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num==0) )
		{
			send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
			GOTOERR;
		}
		if (list_scan.num > 0)//Ԥ�ڵĽ��������ȷ��,����������ʾ,����ʾ������Ա���ɨ�赽��AP��Ӧ�ò����б��رյ�SSID
		{
			memset(ssidout,0,sizeof(ssidout));
			send_result("�ر�ĳ̨AP��ɨ�赽%d��AP����Ϣ����:", list_scan.num);
			for (i=0; i<list_scan.num; i++)
			{
				if(!strcmp(ap[i].ssid,""))//���������ap  ֱ�Ӵ������buf
					strcpy(ssidout[i],ap[i].ssid);
				else
				{
					if((ret=NDK_Utf8ToGbk((uchar *)ap[i].ssid,strlen(ap[i].ssid),(uchar *)ssidout[i],&ssidoutlen))!=NDK_OK)
					{
						send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
						GOTOERR;
					}
				}
				send_result("AP[%d]:%s,mac:%02x:%02x:%02x:%02x:%02x:%02x", i, ssidout[i], ap[i].mac.octet[0], ap[i].mac.octet[1], ap[i].mac.octet[2], ap[i].mac.octet[3], ap[i].mac.octet[4], ap[i].mac.octet[5]);
			}	
		}
		if(cls_show_msg("����֮ǰ�رյ�AP�Ƿ��ٱ�ɨ�赽,�ǰ�[ENTER],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	
	//case8:����оƬmac,�ж��Ƿ���ȷ
	cls_printf("��֤��ȡWIFI��MAC��Ϣ...");
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_MAC,&mac)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	send_result("��ȡ��wifi��macΪ:%02x:%02x:%02x:%02x:%02x:%02x,����֤MAC��ַ�Ƿ���ȷ",mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]);
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,status=%d)", __LINE__, TESTAPI, ret, status.status);
		RETURN;
	}
	
	//case9:����һ��ap,�������,��ôӦ�����Ӳ���,��ȡ�����ӵ�AP��Ϣ����Ӧ�÷���ʧ��
	memset(&st_ap,0,sizeof(st_ap));
	strcpy(list_s.stored_ap->ssid,test_ap); 
	strcpy(list_s.stored_ap->pwd,"0000000000");
	list_s.stored_ap->flag = WIFI_AP_FLAG_DHCP_SET ;
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
	if( ((ret = lib_getwifistatus(&status)) != NDK_OK) || ((status.status != WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED) && (status.status != WIFI_WLAN_STATUS_FAIL)))
	{
		send_result("line %d: %s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status.status);
		GOTOERR;
	}
	memset(&succ_ap,0,sizeof(succ_ap));
	if( (ret=NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&succ_ap))==NDK_OK )
	{
		send_result("line %d: %s����ʧ��(ret=%d),�����ϴ����AP:%s", __LINE__, TESTAPI, ret, succ_ap.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case10:���Է���������ap��Ϣ:����һ����ȷ��AP��Ϣ,���гɹ����Ӻ�᷵����ȷ��ap��Ϣ
	cls_printf("��֤��ȡ�����ӵ�ap��Ϣ...");
	memset(&st_ap,0,sizeof(st_ap));
	strcpy(list_s.stored_ap->ssid,test_ap); 
	strcpy(list_s.stored_ap->pwd,pwd);
	list_s.stored_ap->flag = WIFI_AP_FLAG_DHCP_SET ;
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
	if( ((ret = lib_getwifistatus(&status)) != NDK_OK) || (status.status != WIFI_WLAN_STATUS_SUCC) )
	{
		send_result("line %d: %s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status.status);
		GOTOERR;
	}
	memset(&succ_ap,0,sizeof(succ_ap));
	if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&succ_ap))!=NDK_OK) || strcmp(succ_ap.ssid,test_ap) ||  memcmp(succ_ap.mac.octet, ap_mac, sizeof(ap_mac)) )
	{
		send_result("line %d: %s����ʧ��(ret=%d),�����ϴ����AP:%s,MAC:%02x:%02x:%02x:%02x:%02x:%02x", __LINE__, TESTAPI, ret, succ_ap.ssid, succ_ap.mac.octet[0], succ_ap.mac.octet[1], succ_ap.mac.octet[2], succ_ap.mac.octet[3], succ_ap.mac.octet[4], succ_ap.mac.octet[5]);
		goto ERR;
	}
	send_result("���������ӵ�AP��ģʽ:%s,�ŵ�:%d,Ƶ��:%s,�ź�:%d�Ƿ���ȷ", displaywifimode(succ_ap.auth_mode), succ_ap.channel, displaywififrequency(succ_ap.frequency), succ_ap.rssi);

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_WifiStop();
	return;
}

