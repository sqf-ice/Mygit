/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth5.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_NetDHCP�ӿڹ���
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_NetDHCP"
#define		MAXDHCPTIME		12		//DHCPʱ������

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  						chensl		   20130105	 		created
*****************************************************************/
void eth5(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	char getIPaddr[16] = {0}, getMask[16] = {0}, getGateway[16] = {0};
	//char oldgetIPaddr[16] = {0}, oldgetMask[16] = {0}, oldgetGateway[16] = {0}, oldDns[16] = {0};
	float dhcptime = 0.0, dhcptime1 = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif

	if(auto_flag==1)
	{
		send_result("%s��֧���Զ�����,�������ֶ�����", TESTAPI);
		return;
	}
	
	//����Ϊ������
	cls_show_msg("����%s,�뽫POS���ӵ�һ̨֧��DHCP��·������,�������ʼ����", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	if(cls_show_msg("������Ҫ�󿪻���ֱ�Ӳ���,������ǿ��������б�������ESC���˳�")==ESC)
	{
		send_result("line %d:�ǿ�����һ������,�û��˳�", __LINE__);
		return;
	}
	//����ǰ�ã���ȡԭʼ�����ַ��Ϣ
	//NDK_EthGetNetAddr(oldgetIPaddr, oldgetMask, oldgetGateway, oldDns);

	//�������ַ��Ϣ���ó�NULL
	if((ret=NDK_EthSetAddress("1.0.0.1", "255.0.0.0", "1.0.0.254", NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��������DHCP������֤,�������ڻ�ȡ��һ�ο���DHCPʱ��
	lib_StartStopwatch();
	if((ret=NDK_NetDHCP()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���Զ�̬��ȡ���ܵ�ִ�в�Ӧ���ڹ涨ֵ��(����ֵ�涨)
	if((dhcptime1=lib_ReadStopwatch())>MAXDHCPTIME)
	{
		send_result("line %d:%s����ʧ��(DHCPʱ��:%f)", __LINE__, TESTAPI, dhcptime1);
		GOTOERR;
	}
	//��֤��̬������IPӦ�ú;�̬���õĲ�һ��
	if ((ret=NDK_EthGetNetAddr(getIPaddr, getMask, getGateway, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(!strcmp(getIPaddr,"1.0.0.1") || !strcmp(getMask,"255.0.0.0") || !strcmp(getGateway,"1.0.0.254"))//linwl20130606�޸�DHCP����������Ч����֤��ʽ
	//if (strlen(getIPaddr) == 0 || strlen(getMask) == 0 || strlen(getGateway) == 0)
	{
		//send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, strlen(getIPaddr), strlen(getMask), strlen(getGateway));
		send_result("line %d:%s����ʧ��(%s,%s,%s)", __LINE__, TESTAPI, getIPaddr, getMask, getGateway);
		GOTOERR;
	}
	
#if !K21_ENABLE  //�Ͷ���״̬���Ͳ���������ͼ����ж� 20180316
	//case4:NDK_NetDHCP�������óɹ����Ҳ����������,״̬��ͼ����ʾ:����������ͼ�� 20140618  jiangym
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾ����������ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾ����������", __LINE__);
		GOTOERR;
	}
#endif	

	//case1:���ε��򲻲�����ʱ������DHCPʹ�ܣ���ʱӦ�ڹ涨ʱ�����ڣ�ͬʱ����Ӧ��ȡ��������������
	cls_show_msg("��γ�����,���������������");
	lib_StartStopwatch();
#if K21_ENABLE  //�Ͷ˷���δ���� 20180328
	if((ret=NDK_NetDHCP()) != NDK_ERR_MODEM_NOLINE)
#else
	if((ret=NDK_NetDHCP()) != NDK_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((dhcptime=lib_ReadStopwatch())>MAXDHCPTIME)
	{
		send_result("line %d:%s����ʧ��(DHCPʱ��:%f)", __LINE__, TESTAPI, dhcptime);
		GOTOERR;
	}
	memset(getIPaddr, 0, sizeof(getIPaddr));
	memset(getMask, 0, sizeof(getMask));
	memset(getGateway, 0, sizeof(getGateway));
	if((ret=NDK_EthGetNetAddr(getIPaddr, getMask, getGateway, NULL)) != NDK_ERR_NET_GETADDR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if !K21_ENABLE  //�Ͷ���״̬���Ͳ���������ͼ����ж� 20180316
	//case5:NDK_NetDHCP�ɹ���,δ�����������,״̬��ͼ����ʾ:δ��������ͼ�� 20140618  jiangym
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾδ��������ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾδ��������", __LINE__);
		GOTOERR;
	}
#endif	

	//case2:������1֮���ز������ߣ��ٴν���DHCPʹ�ܣ���ʱӦ�ڹ涨ʱ�����ڣ�ͬʱ��Ӧ�ܻ�ȡ��������������
	//�������ַ��Ϣ���ó�NULL
	if((ret=NDK_EthSetAddress("1.0.0.1", "255.0.0.0", "1.0.0.254", NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���������,���������������");
	lib_StartStopwatch();
	if((ret=NDK_NetDHCP()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���Զ�̬��ȡ���ܵ�ִ�в�Ӧ���ڹ涨ʱ�䡣(����ֵ�涨)
	if((dhcptime=lib_ReadStopwatch())>MAXDHCPTIME)
	{
		send_result("line %d:%s����ʧ��(DHCPʱ��:%f)", __LINE__, TESTAPI, dhcptime);
		GOTOERR;
	}
	
	//��֤��̬������IPӦ�ú;�̬���õĲ�һ��
	memset(getIPaddr, 0, sizeof(getIPaddr));
	memset(getMask, 0, sizeof(getMask));
	memset(getGateway, 0, sizeof(getGateway));
	if ((ret=NDK_EthGetNetAddr(getIPaddr, getMask, getGateway, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if(!strcmp(getIPaddr,"1.0.0.1") || !strcmp(getMask,"255.0.0.0") || !strcmp(getGateway,"1.0.0.254"))//linwl20130606�޸�DHCP����������Ч����֤��ʽ
	//if (strlen(getIPaddr) == 0 || strlen(getMask) == 0 || strlen(getGateway) == 0)
	{
		//send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, strlen(getIPaddr), strlen(getMask), strlen(getGateway));
		send_result("line %d:%s����ʧ��(%s,%s,%s)", __LINE__, TESTAPI, getIPaddr, getMask, getGateway);
		GOTOERR;
	}
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1]))!=NDK_OK)
	{                                                                                            
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//���Խ���
#if K21_ENABLE
	memset(str,0,sizeof(str));
	ftos(dhcptime1,str);
	send_result("%s����ͨ��(DHCPʱ��:%s)", TESTAPI, str); 
#else	
	send_result("%s����ͨ��(DHCPʱ��:%f)", TESTAPI, dhcptime1); 
#endif
ERR:
	NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1]);
	return;
}

