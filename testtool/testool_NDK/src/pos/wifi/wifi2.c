/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��Wifi ģ��
* file name		: wifi2.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_WifiStart,NDK_WifiStop  wifi�������ر�wifi����
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
#define TESTAPI	"NDK_WifiStart,NDK_WifiStop"	

extern int scan_get_mac(uchar outmac[6]); 

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wifi2(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			jiangym		20160223   created 
*****************************************************************/
#if WIFI_PRIORITY_ENABLE
void wifi2(void)
{
	/*private & local definition*/
	int ret = 0, i = 0 ;
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_AP_ENTRY_T set_ap[32];
	list_s.stored_ap = set_ap;
	ST_WIFI_AP_INFO ap_info;
	ST_WIFI_STATUS status;
	char *ssid[] = {
					"AP1@#fartest",
					"AP2&4same",
					"`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.",
					"AP2&4same",
					};
	/*
	����Ҳ�������̨·�������Լ�����̨·�����Ļ���������·���������ҵ�mac���߽��������ҵ�mac Ȼ���ӦAP�޸�mac
	ͬʱ�������룬���ð�ȫ����:AP1Ϊ wpa2ģʽ��AP2Ϊ wpa2ģʽ��AP3ΪWEPģʽ��AP4ΪWPAģʽ������ʹ��64λ��hex
	�����ð�ȫ����ʱ�����Լ����ò�ͬ���͵ģ�ͬʱ���ú����룬Ȼ��Ѵ˴���������Ӧ�޸�
	
	uchar default_mac[][6] = {
					{0x78,0xEB,0x14,0xA3,0x68,0xF0},//FAST FW150R ·������mac
					{0xA8,0x57,0x4E,0x95,0x2E,0x88},//TP-Link TL-WR742N·������mac
					{0x78,0x54,0x2e,0x59,0xdd,0x3a},//D-Link DIR-601 ����N150·������mac
					{0x08,0x10,0x79,0x82,0xDB,0x41},//�ڿ�NW705P ·������mac
					};
	*/
	char *pwd[] = {
					"12345678",//wpa2 aes
					"12345678910",//wpa2 
					"1234567890",//wep
					"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef",//wpa
					};
	char *wifimacstr[] = {"WifiMac1","WifiMac2","WifiMac3","WifiMac4"};
	char pInfo[64] = {0}, mactemp[64] = {0};
	uchar apmac[4][6];
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//��Ҫ��ʾ׼����4��ap��ssid���������ú� ����ȥ����
	/*
	������Ҫ����һ�����Ի����Ĵ����:������Ҫ4��ap�ֱ�Ϊap1 2 3 4
	����ap2 4���Է����ڱȽϽ���λ��
	ap1 ��Զ(ͬʱҪ��������)
	ap3�м�λ��(����ʹ���ź�ֵap24>ap3>ap1)(ap1 ap3�������)
	ap1Ҫ��ap2Զ(ap2�ź�ǿ��ap1),ap3Ҫ��ap4Զ(ap4�ź�ǿ��ap3)
	*/
	//����ǰ��
	NDK_WifiStart();
	if((ret = lib_getwifistatus(&status)) != NDK_OK)
	{
		send_result("line %d:��ȡwifi״̬ʧ��(ret=%d,status=%d)", __LINE__, ret, status.status);
		return;
	}
	//�ȴ������ļ��л�ȡ��AP Mac  �������ļ���ͨ��ɨ���ȡ
	if(NDK_FsExist(PARACONF)!=NDK_OK)
	{	
		cls_show_msg1(3,"�޲��������ļ�,��ͨ��ɨ���ȡmac");
		//ͨ��ɨ���ȡMac���붨���Mac������
		for(i=0;i<4;i++)	
		{
			cls_show_msg1(5,"��ȡ��%d������AP:%s��Mac",i+1,ssid[i]);
			if((ret = scan_get_mac(apmac[i])) != SUCC)
			{
				send_result("line %d:��ȡ����AP Macʧ��(%d)", __LINE__, ret);
				return;
			}
		}
	}
	else
	{
		for(i=0;i<4;i++)
		{
			if(GetParaInfo(wifimacstr[i],pInfo)!=SUCC)
			{	
				send_result("line %d:����Mac��Ϣʧ��", __LINE__ );
				return;
			}
			if(trim_string(pInfo,mactemp)!=SUCC)
			{	
				send_result("line %d:��ȡMac��Ϣʧ��", __LINE__ );
				return;
			}
			if(trim_char2uchar(mactemp,apmac[i])!=SUCC)
			{	
				send_result("line %d:ת��Mac��ʽʧ��", __LINE__ );
				return;
			}
		}
	}
	NDK_WifiStop();//�պ�Ҳ��Ϊ��������
	
	//case1:���ÿ��б�����start���һ�ȡ״̬����WIFI_WLAN_STATUS_EMPTY�����Ҳ�����SSID
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
	memset(&status,0,sizeof(status));
	if( ((ret = lib_getwifistatus(&status)) != NDK_OK) || (status.status != WIFI_WLAN_STATUS_EMPTY) || strcmp(status.ssid,""))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�����б���ڶ�����ȼ��ģ�����start��ѡ�����ȼ��ߵ�����(��ʱ�����ź���Σ����ȼ������ź�)���سɹ���״̬��ssidΪԤ�ڵ�AP3
	//�������ź�ǿ��Ԥ��AP2>AP3>AP1�����ȼ�priority��ֵΪ0~255��ֵԽ���ʾ���ȼ�Խ�ߣ�����AP3���ȼ����
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i],6);//���������ڻ������������ͬ����SSID,��Ҫ��ȷ����������һ��AP����Ҫ����MAC��ַ���˴���Ҫ����ĵ�,Ҫ�󲹳����˵��(�ĵ�Ҫ��)
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET | WIFI_AP_FLAG_MAC_SET;
		list_s.stored_ap[i].priority = i;//�����ȼ��ߵķ������,����ײ㰴˳������,����������AP1��AP1����������ȼ������
	}
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;//��AP1���ó�����AP
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
	//Ԥ��Ҫ���������ȼ���ߵ�AP3
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[2]) )//����Ŀǰwifi���ȶ���ap3�и��ʳ������Ӳ��ϵ���������Դ˴����ܻ�����AP2(�ײ�BUG)
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
#if 0//������ʾ�����ϵ�AP��MAC��ַ
	memset(&ap_info,0,sizeof(ap_info));
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&ap_info)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("AP:%s MAC:%02x:%02x:%02x:%02x:%02x:%02x", ap_info.ssid, ap_info.mac.octet[0], ap_info.mac.octet[1], ap_info.mac.octet[2], ap_info.mac.octet[3], ap_info.mac.octet[4], ap_info.mac.octet[5]);
#endif
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.1:�ҶϺ��ȡ״̬��ʧ�ܣ�ͬʱ����WIFI_WLAN_STATUS_FAIL״̬(�Ҷ���������)
	memset(&status,0,sizeof(status));
	if( ((ret=NDK_WifiStatus(&status))!=NDK_ERR_WIFI_DEVICE_UNAVAILABLE) || (status.status!=WIFI_WLAN_STATUS_FAIL) || strcmp(status.ssid,""))//�������Ⱥ�������ֵ�ĵ�ȷ�������޸ķ���ֵ(�ĵ�Ҫ��)
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}

	//case3:�����Ƿ��������ȼ���Ӧ���ܹ��������ϴ������ϵ�ap�����԰�case2�������ӹ���ap��Ϊ�ϴ����ӵģ��޸������ȼ��ɸߵ��ͣ�Ӧ����Ȼ������
	//���������AP1��AP2��AP3�����ȼ�AP1>AP2>AP3��Ԥ�������ϴ����ӳɹ���AP3
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{	
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i],6);//����ͬ��AP2��AP4���������Ҫָ��mac����ȷ�����ӵ�ap
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET | WIFI_AP_FLAG_MAC_SET; 
		list_s.stored_ap[i].priority = 3-i;
	}
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;
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
	//Ԥ��Ҫ�������ϴ����ӵ�AP3����Ȼ��ʱAP3�����ȼ����
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[2]))//����Ŀǰwifi���ȶ���ap3�и��ʳ������Ӳ��ϵ���������Դ˴����ܻ�����AP2(�ײ�BUG)
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:���������ȼ����Ұ��ϴ����ӵ�apɾ���������µ�ap�����start��Ӧ�û��������ź�ǿ��ap
	//�������ӱ����ɾ����AP3(�ϴ����ӵ�)��ȡ�������ȼ�����,����AP1��AP2����������Ԥ���������ź�ǿ�Ƚ�ǿ��AP2(AP2�Ͻ�),�������ڴ����ʱҪȷ��AP2�ź�ǿ��AP1
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<2;i++)
	{	
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i],6);//�����д���ͬ��AP2��AP4�������ͬʱԤ��Ҫ������ͬ��APʱ��Ҫָ��mac����ȷ�����ӵ�ap
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
		list_s.stored_ap[i].priority = 0;
	}
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;//AP1���ر�־λ��1
	list_s.stored_num = 2;
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
	//���ջ����:AP2���ź���ǿ��AP1��,Ԥ��Ҫ�����ź�ǿ��AP2
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}//�˴��и�������AP1������ԭ����AP1���ź�ȷʵǿ��AP2�������ڴ�ʱAP1�����ص��޷���ȡ���ź�ֵ�������ɿ��������źŵ��Ժ��޸�����(δȷ���Ƿ�ΪBUG)
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if 0
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap[0].ssid,ssid[1]);
	strcpy(list_s.stored_ap[0].pwd,pwd[1]);
	memcpy(list_s.stored_ap[0].mac.octet,apmac[1],6);//�����д���ͬ��AP2��AP4�������ͬʱԤ��Ҫ������ͬ��APʱ��Ҫָ��mac����ȷ�����ӵ�ap
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
	list_s.stored_ap[0].priority = 0;
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
	//Ԥ��Ҫ����(AP2)
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	

	//ע��:������5 6 7 8 9 10�����������,�����ƶ���ע��
	//case5:�����б�����ap,������������Ϊ�գ�����start�������ӷ�������ʧ��WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED�Ҳ�����ssid
	//���������AP1��AP2��AP3�������ȼ�������ȫ�����ô���Ԥ������ʧ��
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{	
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,"1234123412");//�������ô���� 
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i],6);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET; 
		list_s.stored_ap[i].priority = 0;
	}
	strcpy(list_s.stored_ap[0].pwd,"");//AP1���������ÿ�
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;
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
	memset(&status,0,sizeof(status));
	if( ((ret = lib_getwifistatus(&status)) != NDK_OK) || (status.status != WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED)|| strcmp(status.ssid,""))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0//������������������֤��,Ϊ�Զ����Զ����ٲ��Ը����,���ɲ�����Ա�۲���������ó��б��е�AP�Ƿ񱻳�������
	if(auto_flag==0||auto_flag==2)
	{
		if(cls_show_msg("�������Ƿ�������%s��%s,��[Enter],��[����]",ssid[1],ssid[2])!=ENTER) //�˴���������:���ڰ�ȫģʽ��Ӧ�������г���Ҫ�����Եײ��жϵ�AP1�Ŀ����벻����Ҫ��Ͳ��᳢������(��BUG)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
#endif	
	//case6:��case5�е��б�����AP2�������޸���ȷ�󣬵���start��Ӧ���ܹ���������ȷ��AP2
	//���������AP1��AP2��AP3�������ȼ���ֻ��AP2����ȷ�ģ�Ԥ��������AP2
	strcpy(list_s.stored_ap[1].pwd,pwd[1]);//��AP2����������ȷ
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )//�˴�����WEP_OPENģʽ�����벻��ȷʱ�ײ�Ҳ�жϳɳɹ����ӵ�BUG(��ȻNDK�������Ӳ��ɹ�)����ʧ��(�ײ�BUG)
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}

	//case7:������������ĳ��AP������£�ɾ�����޸�AP�б��г�����������AP�������Ŀ��Ϣ��������Ӧ���ֻ����ٿ���ȥ�������Ա��֡�һ��
	//������ɾ��AP1����AP4��ʵ�ʴ��AP2��AP3��AP4�������ȼ���Ԥ���������ϴ����ӵ�AP2
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i+1]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i+1]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i+1],6);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
		list_s.stored_ap[i].priority = 0;
	}
	list_s.stored_num = 3;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//δ���������:Ӧ����Ȼ����������AP2
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//������:Ӧ����Ȼ������AP2
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	//����AP2��AP4ͬʱ����,��Ҫ�ж��Ƿ�ΪAP2��mac
	memset(&ap_info,0,sizeof(ap_info));
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&ap_info)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(ap_info.mac.octet, apmac[1], sizeof(apmac[1])))
	{
		send_result("line %d: %s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, ap_info.mac.octet[0], ap_info.mac.octet[1], ap_info.mac.octet[2], ap_info.mac.octet[3], ap_info.mac.octet[4], ap_info.mac.octet[5]);
		GOTOERR;
	}
	
	//case8:�޸ı���֮ǰ���ӹ���apɾ���󣬵���start���ܹ�������Ӧ�Ĺ���(��������ԭ��)����ap
	//������ɾ��AP2��ʵ�ʱ���AP3��AP4�����ȼ�AP4>AP3��Ԥ�ڸ������ȼ�ԭ������AP4
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<2;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i+2]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i+2]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i+2],6);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET | WIFI_AP_FLAG_MAC_SET;
		list_s.stored_ap[i].priority = i; //AP4�����ȼ����  (��Ϊ��ȷ�������Ĳ��ȶ���ɲ��ԵĴ���,�ź�������ǰ�������Ѿ����)
	}
	list_s.stored_num = 2;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//δ���������:Ӧ����Ȼ����������AP2
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������Ӧ�ø������ȼ�ԭ��������AP4
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[3]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	//����AP2��AP4ͬʱ����,��Ҫ�ж��Ƿ�ΪAP4��mac
	memset(&ap_info,0,sizeof(ap_info));
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&ap_info)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(ap_info.mac.octet, apmac[3], sizeof(apmac[3])))
	{
		send_result("line %d: %s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, ap_info.mac.octet[0], ap_info.mac.octet[1], ap_info.mac.octet[2], ap_info.mac.octet[3], ap_info.mac.octet[4], ap_info.mac.octet[5]);
		GOTOERR;
	}

	//case9:�޸�֮ǰ���ӳɹ���ap�����룬�ٴ�start������¸��ݹ���(��������ԭ��)����ap 
	//����������AP1��AP2��AP3��AP4��AP4Ϊ�ϴ����ӵ�AP���������Ԥ��:����AP4ʧ�ܺ�Ӧ�û᳢���������ȼ��ڶ���AP3��Ӧ���ܹ�������AP3
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<4;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i],6);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET | WIFI_AP_FLAG_MAC_SET;
		list_s.stored_ap[i].priority = i; //AP4���ȼ���� AP3�ڶ�
	}
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;
	strcpy(list_s.stored_ap[3].pwd,"00000000");//��֮ǰ�ɹ����ӵ�ap4�޸ĳɴ��������
	list_s.stored_num = 4;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//δ���������:Ӧ����Ȼ����������AP4
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[3]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�������Ӻ�Ӧ�����������ȼ��ڶ��ߵ�AP3
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[2]) )//�˴�����ͬ��AP2�Ĵ���,�ײ���AP2�����ϴ����ӵ�APȥ����,���Ի�����AP2(�ײ�BUG),Ҳ��������ΪAP3�����ϣ�����������AP2
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10.1:���ڵײ�AP�б��������DHCP����Ŀ��ʹ�þ�̬IP��ʽ����Ŀ������������Ԥ����������ӹ����磬�ϴ������ϵġ����ȼ����ȡ��ͽ�ԭ���ź����ȵȣ����ײ�ʵ�ֶ�������������ʹ�ö�̬IP���Ǿ�̬IP����Ӧ�����ӵ���Ӧ��AP�ϣ�
	//����������AP1(��̬IP)��AP2(��̬IP)�����ȼ�AP2>AP1��Ԥ��������AP2(��̬IP)
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap[0].ssid,ssid[0]);
	strcpy(list_s.stored_ap[0].pwd,pwd[0]);
	strcpy(list_s.stored_ap[0].ip.ip,"192.168.1.10");
	strcpy(list_s.stored_ap[0].ip.netmask,"255.255.255.0");
	strcpy(list_s.stored_ap[0].ip.gateway,"192.168.1.1");
	strcpy(list_s.stored_ap[0].ip.dns_primary,"218.85.157.99");
	strcpy(list_s.stored_ap[0].ip.dns_secondary,"192.168.30.1");
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_HIDDEN_SET | WIFI_AP_FLAG_PRI_SET;
	list_s.stored_ap[0].priority = 0;
	strcpy(list_s.stored_ap[1].ssid,ssid[1]);
	strcpy(list_s.stored_ap[1].pwd,pwd[1]);
	memcpy(list_s.stored_ap[1].mac.octet,apmac[1],6);
	list_s.stored_ap[1].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET | WIFI_AP_FLAG_MAC_SET;
	list_s.stored_ap[1].priority = 1; 
	list_s.stored_num = 2;
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
#if 1	
	//���ӳɹ����ж����ӵ���MAC��ַӦ����AP2��MAC��ַ
	memset(&ap_info,0,sizeof(ap_info));
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&ap_info)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(ap_info.mac.octet, apmac[1], sizeof(apmac[1])))
	{
		send_result("line %d: %s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, ap_info.mac.octet[0], ap_info.mac.octet[1], ap_info.mac.octet[2], ap_info.mac.octet[3], ap_info.mac.octet[4], ap_info.mac.octet[5]);
		GOTOERR;
	}
#endif	
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case10.2::���ڵײ�AP�б��������DHCP����Ŀ��ʹ�þ�̬IP��ʽ����Ŀ������������Ԥ����������ӹ����磬�ϴ������ϵġ����ȼ����ȡ��ͽ�ԭ���ź����ȵȣ����ײ�ʵ�ֶ�������������ʹ�ö�̬IP���Ǿ�̬IP����Ӧ�����ӵ���Ӧ��AP�ϣ�
	//����������AP1(��̬IP)��AP4(��̬IP)�����ȼ�AP1>AP4��Ԥ��������AP1(��̬IP)��ע��:������һ���ϴ����ӳɹ���APΪ������ȼ������Ա�������ɾ��AP2(�ϴ����ӵ�)������AP4
	list_s.stored_ap[0].priority = 1;
	strcpy(list_s.stored_ap[1].ssid,ssid[3]);
	strcpy(list_s.stored_ap[1].pwd,pwd[3]);
	memcpy(list_s.stored_ap[1].mac.octet,apmac[3],6);
	list_s.stored_ap[1].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
	list_s.stored_ap[1].priority = 0; 
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[0]) )//�˴��и���ʧ��:����ԭ������ж������ڸ�����������AP1������AP4����(ȷ����BUG)
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case11:����ap 
	//����������AP1Ϊ����AP��Ԥ�ڿ���������AP1
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap[0].ssid,ssid[0]);
	strcpy(list_s.stored_ap[0].pwd,pwd[0]);
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_HIDDEN_SET;
	list_s.stored_ap[0].priority = 0; 
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[0]) )//�˴�������ʧ��:ԭ������Ǹ��������Ӳ���AP1,��������ʧ��
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case12:����ͬ��ssid�����ò�ͬ���ȼ�������start�󣬻��������ȼ��ߵģ��жϿ��Ը���mac�ж�
	//����������AP2��AP3��AP4��AP2��AP4ͬ�������ȼ� AP4>AP3>AP2��Ԥ�ڿ���������AP4
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)  //�б�������  AP2 AP3 AP4  AP2��AP4ͬ��
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i+1]); 
		strcpy(list_s.stored_ap[i].pwd,pwd[i+1]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i+1],6);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET | WIFI_AP_FLAG_MAC_SET;
		list_s.stored_ap[i].priority = i;
	}
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
	//Ԥ�����������ȼ���ߵ�AP4
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[3]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	memset(&ap_info,0,sizeof(ap_info));
	if((ret = NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&ap_info)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	if(memcmp(ap_info.mac.octet, apmac[3], sizeof(apmac[3])))
	{
		send_result("line %d: %s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, ap_info.mac.octet[0], ap_info.mac.octet[1], ap_info.mac.octet[2], ap_info.mac.octet[3], ap_info.mac.octet[4], ap_info.mac.octet[5]);
		GOTOERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_WifiStop();
	return;
}
#else
void wifi2(void)
{
	/*private & local definition*/
	int ret = 0, i = 0 ;
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_AP_ENTRY_T set_ap[32];
	list_s.stored_ap = set_ap;
	ST_WIFI_AP_INFO ap_info;
	ST_WIFI_STATUS status;
	char *ssid[] = {"AP1@#fartest",
#if defined ME68 || defined ME50NX  //����ƽ̨��֧�ֶ���  20180515  modify
				"`~!\"@$#[^%]*() -_=+|\\&{}:;?/><.",
#else
				"`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.",
#endif
				};
	char *pwd[] = {"12345678",//wpa2 aes 
				   "1234567890", //wep
				   };
	char pInfo[64] = {0}, mactemp[64] = {0};
	uchar apmac[2][6];
	char *wifimacstr[] = {"WifiMac1", "WifiMac3"};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	NDK_WifiStart();
	if((ret = lib_getwifistatus(&status)) != NDK_OK)
	{
		send_result("line %d:��ȡwifi״̬ʧ��(ret=%d,status=%d)", __LINE__, ret, status.status);
		return;
	}
	//�ȴ������ļ��л�ȡ��AP Mac  �������ļ���ͨ��ɨ���ȡ
	if(NDK_FsExist(PARACONF)!=NDK_OK)
	{	
		cls_show_msg1(3,"�޲��������ļ�,��ͨ��ɨ���ȡmac");
		//ͨ��ɨ���ȡMac���붨���Mac������
		for(i = 0; i<2; i++)
		cls_show_msg1(5,"��ȡ��%d������AP:%s��Mac",i+1,ssid[i]);
		if((ret = scan_get_mac(apmac[i])) != SUCC)
		{
			send_result("line %d:��ȡ����AP Macʧ��(%d)", __LINE__, ret);
			return;
		}
	}
	else
	{
		for(i=0; i<2; i++)
		{
			if(GetParaInfo(wifimacstr[i],pInfo)!=SUCC)
			{	
				send_result("line %d:����Mac��Ϣʧ��", __LINE__ );
				return;
			}
			if(trim_string(pInfo,mactemp)!=SUCC)
			{	
				send_result("line %d:��ȡMac��Ϣʧ��", __LINE__ );
				return;
			}
			if(trim_char2uchar(mactemp,apmac[i])!=SUCC)
			{	
				send_result("line %d:ת��Mac��ʽʧ��", __LINE__ );
				return;
			}
		}
	}
	NDK_WifiStop();//�պ�Ҳ��Ϊ��������
	
	//case1:���ÿ��б�����start���һ�ȡ״̬����WIFI_WLAN_STATUS_EMPTY�����Ҳ�����SSID
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
	memset(&status,0,sizeof(status));
	if( ((ret = lib_getwifistatus(&status)) != NDK_OK) || (status.status != WIFI_WLAN_STATUS_EMPTY) || strcmp(status.ssid,""))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�ҶϺ��ȡ״̬��ʧ�ܣ�ͬʱ����WIFI_WLAN_STATUS_FAIL״̬(�Ҷ���������)(�˴�״̬������ı�,����δ��״ֵ̬����)
	//ע��:�˴�������(0, -1,  )Ҳ��ͨ��  20180606 modify
	memset(&status,0,sizeof(status));
	if( ((ret=NDK_WifiStatus(&status))!=NDK_ERR_WIFI_DEVICE_UNAVAILABLE) /*|| (status.status!=WIFI_WLAN_STATUS_FAIL)*/ || strcmp(status.ssid,""))
	{
		if(((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL))
		{
			send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
			GOTOERR;
		}
	}

	//case3:�����б������������start�������ӷ�������ʧ��WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED�Ҳ�����ssid
	memset(set_ap,0,sizeof(set_ap));	
	strcpy(list_s.stored_ap[0].ssid,ssid[1]);
	strcpy(list_s.stored_ap[0].pwd,"1234123412");//�������ô���� 
	memcpy(list_s.stored_ap[0].mac.octet,apmac[1],6);
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET; 
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
	memset(&status,0,sizeof(status));
	if( ((ret = lib_getwifistatus(&status)) != NDK_OK) || ((status.status != WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED) && (status.status != WIFI_WLAN_STATUS_FAIL))|| strcmp(status.ssid,""))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��case5�е��б�����AP2�������޸���ȷ�󣬵���start��Ӧ���ܹ���������ȷ��AP
	strcpy(list_s.stored_ap[0].pwd,pwd[1]);//������������ȷ
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}

	//case5:�޸ı���֮ǰ���ӹ���apɾ���󣬵���start���ܹ������µ��б�����ap
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap[0].ssid,ssid[0]);
	strcpy(list_s.stored_ap[0].pwd,pwd[0]);
	memcpy(list_s.stored_ap[0].mac.octet,apmac[0],6);
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;
	list_s.stored_num = 1;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//δ����start�����:Ӧ����Ȼ����������AP2
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������Ӧ�ø����µ��б�������AP1
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[0]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	
	//case6:����ap 
	//����������AP1Ϊ����AP��Ԥ�ڿ���������AP1
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap[0].ssid,ssid[0]);
	strcpy(list_s.stored_ap[0].pwd,pwd[0]);
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_HIDDEN_SET;
	list_s.stored_ap[0].priority = 0; 
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[0]) )//�˴�������ʧ��:ԭ������Ǹ��������Ӳ���AP1,��������ʧ��
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//�б�����Ӧ��ֻ�е�һ̨·����Ч
	//case7:��AP1����Ϊ��һ̨·�ɣ�Ӧ��������AP1
	memset(set_ap,0,sizeof(set_ap));
	for(i=0; i<2; i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		memcpy(list_s.stored_ap[i].mac.octet,apmac[i],6);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
	}
	list_s.stored_ap[0].flag |= WIFI_AP_FLAG_HIDDEN_SET;
	list_s.stored_num = 2;
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
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[0]) )//�˴�������ʧ��:ԭ������Ǹ��������Ӳ���AP1,��������ʧ��
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Ѿ���NDK_WifiStart���ٵ�NDK_WifiStart�����ȶϿ�������
	//case8:��һ�ε���NDK_WifiStart״̬���������ӻ�������ʱ���ڶ���Start��״̬Ӧ�������������ӣ�Ȼ��������
	memset(set_ap,0,sizeof(set_ap));
	strcpy(list_s.stored_ap[0].ssid,ssid[1]);
	strcpy(list_s.stored_ap[0].pwd,pwd[1]);
	memcpy(list_s.stored_ap[0].mac.octet,apmac[1],6);
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_MAC_SET;
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
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�������ӵ�����£��ٵ���Start��ȡ����״̬Ӧ�������������ӣ�Ȼ��������
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if(auto_flag==0 || auto_flag==2)
	{
		//case9:�ڵ�һ��start�󣬻�ȡ��wifiδ���ӵ�����£��ٴε���NDK_WifiStart״̬Ӧ������������
		if((ret = NDK_WifiStart()) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg("��ε�·����:%s��Դ�����������", ssid[1]);
		if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL) || strcmp(status.ssid, "") )
		{
			send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
			GOTOERR;
		}
		cls_show_msg("�����·����:%s��Դ���30������������", ssid[1]);
		if((ret = NDK_WifiStart()) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, ssid[1]) )
		{
			send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
			GOTOERR;
		}
		if((ret = NDK_WifiStop()) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_WifiStop();
	return;
}
#endif

