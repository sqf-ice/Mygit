/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: eth
* file name	: eth8.c
* Author 		: huangjianb
* version		: 
* DATE			: 20140616
* directory 		: 
* description		: ����NDK_EthDisConnect�����ر���̫������
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
#define 	TESTAPI		"NDK_EthDisConnect"

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
* history 		 		: author				date				 remarks
*									:	huangjianb    20140617     created
*****************************************************************/
void eth8(void)
{
	int ret=0;
	char szIPaddr[16] = {0}, szMaskaddr[16] = {0}, szGateway[16] = {0}, szDNS[16*3+2] = {0};
	
	//case1:�ھ�̬��ʽ�´���̫���ɹ��󣬿��Ի�ȡ����̫����Ϣ��
	//����NDK_EthDisConnectӦ���ܹ������رգ��ҹرպ��޷���ȡIP��ַ��
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_EthDisConnect()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_ERR_NET_GETADDR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(auto_flag==0||auto_flag==2)
	{
		//case4:��̫���رպ�,IP��ַδ����,��ʱ״̬��ͼ��Ӧ����ʾΪ:IPδ����ͼ�� 20140623  jiangym
		if(cls_show_msg("״̬��ͼ���Ƿ���ʾIPδ����ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
		{
			send_result("line %d:״̬��ͼ��Ӧ����ʾIPδ����ͼ��", __LINE__);
			GOTOERR;
		}
	}
	else
		send_result("line %d:%s״̬��IPδ����ͼ���������ֶ�����", __LINE__, TESTAPI);
	
	
	//case 2:�ڶ�̬��ʽ�´���̫���ɹ��󣬿��Ի�ȡ����̫����Ϣ��
	//����NDK_EthDisConnectӦ���ܹ������رգ��ҹرպ��޷���ȡIP��ַ��
	cls_show_msg1(30, "�뽫POS���ӵ�һ̨֧��DHCP��·������,�������ʼ����");
	if((ret=NDK_NetDHCP()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_EthDisConnect()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	};
	if((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_ERR_NET_GETADDR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:���л��ؾ�̬��ʽ����̫���ɹ��󣬿��Ի�ȡ����̫����Ϣ��
	//����NDK_EthDisConnectӦ���ܹ������رգ��ҹرպ��޷���ȡIP��ַ��
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_EthDisConnect()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	};
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_ERR_NET_GETADDR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1]);
	return;
}
