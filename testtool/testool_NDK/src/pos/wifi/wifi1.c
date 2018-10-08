/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��Wifi ģ��
* file name		: wifi1.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_WifiProfileGet,NDK_WifiProfileSet�ܷ���ȷ�����б���ȡ�б���Ϣ�Ĺ���
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
#define TESTAPI	"NDK_WifiProfileGet,NDK_WifiProfileSet"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wifi1(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			jiangym		20160223   created 		 
*****************************************************************/
void wifi1(void)
{
	/*private & local definition*/
	int ret = 0, i = 0;
	uchar ap_mac[6] = {0x30,0x31,0x32,0x33,0x34,0x35};
	char ap_ip[16] = "192.168.1.10", ap_netmask[16] = "255.255.255.0", ap_gateway[16] = "192.168.1.254";
	char ap_dns_pri[16] = "218.85.157.99", ap_dns_sec[16] = "1982.168.30.1";
	ST_WIFI_PROFILE_LIST list_s;
	ST_WIFI_PROFILE_LIST list_g;
	ST_WIFI_AP_ENTRY_T set_ap[32], get_ap[32];
	memset(set_ap,0,sizeof(set_ap));//����
	memset(get_ap,0,sizeof(get_ap));
	list_s.stored_ap = set_ap;
	list_g.stored_ap = get_ap;
	char *ssid[] = {//�����ֱ��ʾ
					"test0000",
					"`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.",//�����ַ�
					"һ��������",//����
					};
	char *ssid1[] = {
					"0000",
					"wifi123@$#",
					};
	char *pwd[] = {
					"12345678",
					"1234",
					"",
					};

	/*process body*/
	if(auto_flag==2)
		return;
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_WifiStop();//�����򿪻��Զ�����wifi���������
	
	//case1:�����쳣
	if((ret = NDK_WifiProfileSet(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WifiProfileGet(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:(������һ�βŻ�Ϊ�գ�֮��ᱣ��֮ǰ��) ���ÿձ���ȡ��Ӧ���ǿձ�:���ô���Ϊ0  ��ô���ǿձ�,����ģ��"ɾ"�����
	list_s.stored_num = 0;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WifiProfileGet(&list_g)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(list_g.stored_num != 0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, list_g.stored_num);
		RETURN;
	}
	
	//case3:��������AP,Ӧ���ܹ��ɹ�,����ģ��"��"�ӱ����3��
	//���ú��ȡ����ȡ�����еĲ���stored_num�йأ������Ϊ�������Ҳ��Ϊ���������
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		list_s.stored_ap[i].priority = 0;
	}
	list_s.stored_ap[0].flag = WIFI_AP_FLAG_DHCP_SET;//��1��ssidΪӢ��,����DHCP,������Ȩ
	list_s.stored_ap[1].flag = WIFI_AP_FLAG_MAC_SET;//��2��APΪ�����ַ�,����MAC����̬IP,������Ȩ
	memcpy(list_s.stored_ap[1].mac.octet,ap_mac,6);
	strcpy(list_s.stored_ap[1].ip.ip,ap_ip);
	strcpy(list_s.stored_ap[1].ip.netmask,ap_netmask);
	strcpy(list_s.stored_ap[1].ip.gateway,ap_gateway);
	strcpy(list_s.stored_ap[1].ip.dns_primary,ap_dns_pri);
	strcpy(list_s.stored_ap[1].ip.dns_secondary,ap_dns_sec);
	list_s.stored_ap[2].flag = WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET;//��3��APΪ����,ʹ��DHCP,��������Ȩ
	list_s.stored_ap[2].priority = 1;
	list_s.stored_num = 3;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case3.1:�б��д������3����Ϣ��������ȡ2������ȡ��Ӧ����2��
	list_g.stored_num = 2;//�����������Ҳ��������� Ԥ�����������
	memset(get_ap,0,sizeof(get_ap));
	if( ((ret=NDK_WifiProfileGet(&list_g))!=NDK_OK) || (list_g.stored_num!=2) )//ÿ��get����һ��"��"�Ķ���
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, list_g.stored_num);
		RETURN;
	}
	//case3.2:�б�������3����Ϣ��������ȡ5������ȡ��Ӧ����3��
	list_g.stored_num = 5;
	memset(get_ap,0,sizeof(get_ap));
	if( ((ret=NDK_WifiProfileGet(&list_g))!=NDK_OK) || (list_g.stored_num!=3) )//ÿ��get����һ��"��"�Ķ���
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, list_g.stored_num);
		RETURN;
	}
	
	//case4:���Ի�ȡ��Ϣ����ȷ��
	for(i=0;i<3;i++)
	{
		if(strcmp(list_g.stored_ap[i].ssid,ssid[i]) || strcmp(list_g.stored_ap[i].pwd,pwd[i]))
		send_result("line %d: %s����ʧ��(%s,%s)", __LINE__, TESTAPI, list_g.stored_ap[i].ssid,list_g.stored_ap[i].pwd);
		RETURN;	
	}
	if(list_g.stored_ap[0].flag != WIFI_AP_FLAG_DHCP_SET || list_g.stored_ap[0].priority != 0 || 
		list_g.stored_ap[1].flag != WIFI_AP_FLAG_MAC_SET ||  list_g.stored_ap[1].priority != 0 ||
		list_g.stored_ap[2].flag != (WIFI_AP_FLAG_DHCP_SET | WIFI_AP_FLAG_PRI_SET)||  list_g.stored_ap[2].priority != 1 )
	{
		send_result("line %d: %s����ʧ��(%d,%d,%d,%d,%d,%d)", __LINE__, TESTAPI, list_g.stored_ap[0].flag, list_g.stored_ap[0].priority,
			 list_g.stored_ap[1].flag, list_g.stored_ap[1].priority, list_g.stored_ap[2].flag, list_g.stored_ap[2].priority);
		RETURN;	
	}
	if(memcmp(list_g.stored_ap[1].mac.octet,ap_mac,6))
	{
		send_result("line %d: %s����ʧ��(mac:%02x:%02x:%02x:%02x:%02x:%02x,)", __LINE__, TESTAPI,list_g.stored_ap[1].mac.octet[0],list_g.stored_ap[1].mac.octet[1],
			list_g.stored_ap[1].mac.octet[2],list_g.stored_ap[1].mac.octet[3],list_g.stored_ap[1].mac.octet[4],list_g.stored_ap[1].mac.octet[5]);
		RETURN;	
	}
	if(strcmp(list_g.stored_ap[1].ip.ip,ap_ip) || strcmp(list_g.stored_ap[1].ip.netmask,ap_netmask) || strcmp(list_g.stored_ap[1].ip.gateway,ap_gateway) 
		|| strcmp(list_g.stored_ap[1].ip.dns_primary,ap_dns_pri) || strcmp(list_g.stored_ap[1].ip.dns_secondary,ap_dns_sec))
	{
		send_result("line %d: %s����ʧ��(%s,%s,%s,%s,%s)", __LINE__, TESTAPI,list_g.stored_ap[1].ip.ip,list_g.stored_ap[1].ip.netmask,list_g.stored_ap[1].ip.gateway,
			list_g.stored_ap[1].ip.dns_primary,list_g.stored_ap[1].ip.dns_secondary);
		RETURN;	
	}
	
	//case5:�����������������ò�ͬ���ݣ���ȡ��Ӧ���Ǻ�һ�ε����
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<3;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET;
		list_s.stored_ap[i].priority = 0;
	}
	list_s.stored_num = 3;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)//"��"��3�� AP1,AP2,AP3
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(set_ap,0,sizeof(set_ap));
	for(i=0;i<2;i++)
	{
		strcpy(list_s.stored_ap[i].ssid,ssid1[i]);
		strcpy(list_s.stored_ap[i].pwd,pwd[i]);
		list_s.stored_ap[i].flag = WIFI_AP_FLAG_DHCP_SET;
		list_s.stored_ap[i].priority = 0;
	}
	list_s.stored_num = 2;
	if((ret = NDK_WifiProfileSet(&list_s)) != NDK_OK)//"��"�������3��,����2���µ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	list_g.stored_num = 3;//��Ȼ���ȡ3����ʵ��Ӧ��ֻ�ܻ�ȡ2��
	memset(get_ap,0,sizeof(get_ap));
	if( ((ret=NDK_WifiProfileGet(&list_g))!=NDK_OK) || (list_g.stored_num!=2) )//ÿ��get����һ��"��"�Ķ���
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, list_g.stored_num);
		RETURN;
	}
	//�ж�list_g Ӧ��ʣ��2���µ�AP��Ϣ
	for(i=0;i<2;i++)
	{
		if(strcmp(list_g.stored_ap[i].ssid,ssid1[i]) || strcmp(list_g.stored_ap[i].pwd,pwd[i])|| list_g.stored_ap[i].flag != WIFI_AP_FLAG_DHCP_SET || list_g.stored_ap[i].priority != 0)
		{
			send_result("line %d: %s����ʧ��(%s,%s,%d,%d)", __LINE__, TESTAPI, list_g.stored_ap[i].ssid,list_g.stored_ap[i].pwd, list_g.stored_ap[i].flag,list_g.stored_ap[i].priority);
			RETURN;	
		}
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

