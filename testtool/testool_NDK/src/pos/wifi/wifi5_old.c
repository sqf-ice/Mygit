/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��Wifi ģ��
* file name		: wifi5.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_WifiSetFunc�Ƿ��ܹ���ȷ�����Ƿ񿪻��Զ����ӹ��ܺ��Զ������ȹ���
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
#define TESTAPI	"NDK_WifiSetFunc"	
#define WIFI_LEN 2047
#define MAXTIMEOUT 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
void set_auto_connect(void)
{
	/*private & local definition*/
	int nkey = 0, ret = 0, data = 0;

	/*process body*/
	while(1)
	{
		nkey = cls_show_msg("1.�����Զ�����\n"
							"2.�������Զ�����\n");
			switch(nkey)
			{	
				case '1':
					data = 1;
					if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START,&data)) != NDK_OK)
					{
						send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
						return;
					}
					cls_show_msg1(2,"���ÿ����Զ����ӳɹ�");
					return;
					break;
				case '2':
					data = 0;
					if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START,&data)) != NDK_OK)
					{
						send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
						return;
					}
					cls_show_msg1(2,"���ÿ������Զ����ӳɹ�");
					return;
					break;
				case ESC:
					return;
					break;
				default:
					continue;
					break;
			}
	}
}

void set_table_and_start(void)
{
	/*private & local definition*/
	int ret = -1;
	ST_WIFI_STATUS status;
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_AP_ENTRY_T st_ap;
	list_s.stored_ap = &st_ap;
	//�˴��������޸�AP���ּ�����
#if defined ME68 || defined ME50NX  //����ƽ̨��֧�ֶ���  20180515  modify
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?/><.";
#else
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.";
#endif
	char pwd[] = "1234567890";

	/*process body*/
	//���úñ��
	memset(&st_ap,0,sizeof(st_ap));
	strcpy(list_s.stored_ap->ssid,test_ap); 
	strcpy(list_s.stored_ap->pwd,pwd);
	//memcpy(list_s.stored_ap->mac.octet,ap_mac,6);
	list_s.stored_ap->flag = WIFI_AP_FLAG_DHCP_SET ;//| WIFI_AP_FLAG_MAC_SET
	list_s.stored_ap->priority = 0;
	list_s.stored_num = 1;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, test_ap) )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		return;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	cls_show_msg1(2,"����%s�ɹ�,����������",test_ap);
#if 0//Ӧ����Ҫ������������:�ڲ�����б��������֤�ܹ��Զ����� 20180604 modify
	memset(&st_ap,0,sizeof(st_ap));
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#endif
	return;
}
/*
�����汾:����б�����Զ�����,������б���ܹ��Զ�����,�ʲ��Զ�������֤����
����:�岻���б����ܹ��Զ�����
�ž�����:Ŀǰ������б�����Զ�����,������б���ܹ��Զ�����(δ���Ƿ������)
������˵������֤���Զ����������Ŀǰ��֤����,ֻ���л���������֤:
	�ڲ�����б������Ҫ��֤�ܹ��Զ�������
*/
void verify_after_set(void)
{
	/*private & local definition*/
	int nkey = 0, ret = 0;
	ST_WIFI_STATUS status;
#if defined ME68 || defined ME50NX  //����ƽ̨��֧�ֶ���  20180515  modify
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?/><.";
#else
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.";
#endif
	
	/*process body*/
#if 0  //��֤�����Զ����Ӳ���Ҫ����start,����ģ����֤,�ž����������β���֤
	//Ӧ����Ҫ������ǰ��NDK_WifiStart 20180503 sull modify
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		return;
	}
#endif
	while(1)
	{
		nkey = cls_show_msg("1.��֤�Զ�����\n"
							"2.��֤���Զ�����\n");
			switch(nkey)
			{
				case '1':
					cls_printf("������֤�Զ�����...");
					memset(&status,0,sizeof(status));
					if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, test_ap) )
					{
						send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
						return;
					}
					cls_show_msg1(2,"�Զ�������֤ͨ��");
					return;
					break;
				case '2':
					memset(&status,0,sizeof(status));
#if 0//WIFI_PRIORITY_ENABLE //��������ͨ��������ڲ�ͬģ�鲻����ͬ,��ʱ��֤����,������������Ը�����ȷ����20180604 modify
					if( ((ret=NDK_WifiStatus(&status))==NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL) || strcmp(status.ssid,""))//�������Ⱥ�������ֵ�ĵ�ȷ�������޸ķ���ֵ(�ĵ�Ҫ��)
#else
					if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!= WIFI_WLAN_STATUS_FAIL) )
#endif
					{
						send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
						return;
					}
					cls_show_msg1(2,"���Զ�������֤ͨ��");
					return;
					break;
				default:
					continue;
					break;
				case ESC:
					return;
					break;
			}
	}
}

void verify_relink(void)
{
	/*private & local definition*/
	int ret = 0, retime = -1, i = 0, data = 0;
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_AP_ENTRY_T st_ap;
	list_s.stored_ap = &st_ap;
	ST_WIFI_AP_LIST list_scan;
	ST_WIFI_AP_INFO ap[32];
	list_scan.list = ap;
	ST_WIFI_STATUS status;
	time_t oldtime;
	//�˴��������޸�AP���ּ�����Ͷ�Ӧ��MAC
#if defined ME68 || defined ME50NX  //����ƽ̨��֧�ֶ���  20180515  modify
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?/><.";
#else
	char  test_ap[] = "`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.";
#endif
	char pwd[] = "1234567890"; 
	uint unLen = 0;
	char sBuf[128] = {0};

	/*process body*/
	//����ǰ��
	//�ž�����������ģ����Ҫ���ÿ����Զ�����(��ʵ���Զ���������,��ԭ���Ŀ����Զ����ӹ���û��,ֻ������ģ���п����Զ����ӹ���)
#if !WIFI_PRIORITY_ENABLE
	data = 1;
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START,&data)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	//��ȡӲ����Ϣ �ж�WiFiģ�� 20180703 modify
	memset(sBuf,0xff,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	NDK_WifiStop();
	memset(&st_ap,0,sizeof(st_ap));
	strcpy(list_s.stored_ap->ssid,test_ap); 
	strcpy(list_s.stored_ap->pwd,pwd);
	list_s.stored_ap->flag = WIFI_AP_FLAG_DHCP_SET ;
	list_s.stored_ap->priority = 0;
	list_s.stored_num = 1;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if(sBuf[8]==0x03||sBuf[8]==0x02)  //����Ǿž�����ģ��\����ģ����Ҫ�Ƚ���һ��start��Ż��Զ�����
	{
		if((ret = NDK_WifiStart()) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		memset(&status,0,sizeof(status));
		if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, test_ap))
		{
			send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
			GOTOERR;
		}
	}		

	//case1:�����Զ�����ʱ��Ϊ5000ms
	//step1:·�����ȹر�
	cls_show_msg("��ر�AP:%s·�����ĵ�Դ�����������", test_ap);
	//step2:�����Զ�����ʱ�䣬�ú������õ�ֵ����һ��start����Ч
	retime = 5000;
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)//�˺���ʱ�����þž�����������ģ����Ч
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(sBuf[8]!=0x03&&sBuf[8]!=0x02)// �ž�����ģ��\����ģ�鲻��Ҫ�ٵ���start,����ģ����Ҫ����
	{	
		//step3:���µ���AP��������APû�п���Ԥ��Ӧ������ʧ��
		if((ret = NDK_WifiStart()) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		memset(&status,0,sizeof(status));
		if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL) || strcmp(status.ssid,""))
		{
			send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
			GOTOERR;
		}
	}
	//step4:��AP�ϵ磬�˴�ͨ��ɨ�赽AP�����ж�AP�����ɹ�
	cls_show_msg1(5,"�뿪��AP:%s·�����ĵ�Դ�����������", test_ap);
	oldtime=time(NULL);
	while(1)
	{
		cls_show_msg1(1,"ɨ��AP��Ϣ��...");
		memset(ap,0,sizeof(ap));
		list_scan.num = 128;//ϣ���������ó��㹻��,���Ի���ҲҪ���������㹻���AP����ɨ��,���Ը���
		ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan);
		if((ret==NDK_OK) && (list_scan.num>0))
		{
			for(i=0;i<list_scan.num;i++)
			{
				if(!strcmp(test_ap, ap[i].ssid))
					break;
			}
			if(i<list_scan.num)
				break;
		}
		//��ʱ�˳�
		if(time(NULL)-oldtime>MAXTIMEOUT)
		{	
			send_result("line %d:δɨ�赽AP:%s,��ʱ�˳�,%s����ʧ��", __LINE__, test_ap, TESTAPI); 
			break;
		}
	}
	//step5:AP�򿪺�Ӧ�ò㲻���ٵ���NDK_WifiStart�������ײ�Ӧ���ܹ��Զ����������������ӳɹ�
	NDK_SysMsDelay(retime+10000);//��ʱ5s  retime//����RDM��BUG2017030600726��������10����ʱ 20170306
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, test_ap))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:����δ����NDK_WifiStart������µ���NDK_WifiSetFuncӦ�óɹ�
	retime = 5000;
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�������ò��Զ�����-1
	//step1:·�����ȹر�
	cls_show_msg("��ر�AP:%s·�����ĵ�Դ�����������", test_ap);
	//step2:���ò�����
	retime = -1;
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//step3:��ʼ����AP��������APû�п���Ԥ��Ӧ������ʧ��
	if((ret = NDK_WifiStart()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL) || strcmp(status.ssid,""))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	//step4:��AP�ϵ磬�˴�ͨ��ɨ�赽AP�����ж�AP�����ɹ�
	cls_show_msg1(5,"�뿪��AP:%s·�����ĵ�Դ�����������", test_ap);
	oldtime=time(NULL);
	while(1)
	{
		cls_show_msg1(1,"ɨ��AP��Ϣ��...");
		memset(ap,0,sizeof(ap));
		list_scan.num = 128;//ϣ���������ó��㹻��,���Ի���ҲҪ���������㹻���AP����ɨ��,���Ը���
		if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan))!=NDK_OK) || (list_scan.num==0) )
		{
			send_result("line %d:%s����ʧ��(ret=%d,num=%d)", __LINE__, TESTAPI, ret, list_scan.num);
			GOTOERR;
		}
		if(list_scan.num>0)
		{
			for(i=0;i<list_scan.num;i++)
			{
				if(!strcmp(test_ap, ap[i].ssid))
					break;
			}
			if(i<list_scan.num)
				break;
		}
		//��ʱ�˳�
		if(time(NULL)-oldtime>MAXTIMEOUT)
		{	
			send_result("line %d:δɨ�赽AP:%s,��ʱ�˳�,%s����ʧ��", __LINE__, test_ap, TESTAPI); 
			break;
		}
	}
	//step5:AP�򿪺�Ӧ�ò㲻���ٵ���NDK_WifiStart�������ײ�Ӧ�ò��ᷢ���Զ�����������Ӧ��ʧ��
	memset(&status,0,sizeof(status));
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_FAIL) || strcmp(status.ssid,""))
	{
		send_result("line %d: %s����ʧ��(%d,%d,%s)", __LINE__, TESTAPI, ret, status.status, status.ssid);
		GOTOERR;
	}
	if((ret = NDK_WifiStop()) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:��������ʱ��Ϊ0 �������� ������ѹ��� ��Ϊ���������Ļ�ʵ������pos�ϵı��־ͻῨ���ǣ��Ῠ��ʲô�������?????
	
	cls_show_msg1(3,"��������ͨ��");
ERR:
	NDK_WifiStop();
	return;
}

/****************************************************************
* function name 	 				: void wifi5(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			jiangym		20160226   created 		 
*****************************************************************/
void wifi5(void)
{
	/*private & local definition*/
	int nkey = 0, ret = 0, data = 1;
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//case1:�����쳣 WIFI_SET_FUNC_AUTO_START-1 WIFI_SET_FUNC_UNDEFINE+1
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START-1,&data)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WifiSetFunc(WIFI_GET_FUNC_UNDEFINE,&data)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WifiSetFunc(WIFI_GET_FUNC_UNDEFINE+1,&data)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//����Ĭ�����
	data = 0;
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START,&data)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2: ������ϲ���
	while(1)
	{
		nkey = cls_show_msg("1.�����Ƿ񿪻��Զ�����\n"
							"2.����\n"
							"3.���ú����֤\n"
							"4.��������");
			switch(nkey)
			{	
				case '1':
					set_auto_connect();
					break;
				case '2':
					cls_show_msg1(2,"����ǰ�Ƚ���һ��ap����...");//���ݿ�����ƿ����Զ�����ֻ�᳢���ϴ����ӳɹ���AP������ϴ����ӳɹ���AP��ɾ��(APδ��,���߱���б�ɾ����)�ͻ��Զ�����ʧ�ܣ����Ҳ����ٸ����±��ȥ�������ӣ��˴���ҪҪ�󿪷������ĵ�˵��
					set_table_and_start();
					NDK_SysReboot();
					break;
				case '3':
					verify_after_set();
					break;
				case '4':
					verify_relink();
					break;
				case ESC:
					//�ָ�Ĭ��ֵ������Ч
					cls_show_msg("ע��:������ù������Զ��������������ٽ�������wifi�����Ĳ��ԣ�����������˳�");
					data = 0;
					if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_AUTO_START,&data)) != NDK_OK)
					{
						cls_show_msg1(2,"line %d: �ָ�����Ĭ��ֵʧ��(%d)", __LINE__, ret);
						return;
					}
					NDK_WifiStop();//�����겻���м��Ƿ���Գɹ�����Ҫ�ر�wifi
					return;
					break;
			}
	}
}

