/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: PPPģ��
* file name		: ppp6.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_NetDnsResolv�ӿڹ���
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
#define 	TESTAPI		"NDK_NetDnsResolv"

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
*									huangjianb   20140617     modify
*****************************************************************/
void ppp6(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, ret2 = -1;
	char DnsName[16] = "www.baidu.com", DnsName126[16] = "www.126.com";
	char DnsIP[16] = {0}, DnsIP126[16] = {0};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��,������̫����ַ��������̫��ģ��
	//if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, DNS2)) != NDK_OK)
	//{
	//	send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
	//	return;
	//}
	if(NetUp()!=SUCC)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
	//case1:����Ƿ�����,����������
	if(((ret=NDK_NetDnsResolv(COMM_TYPE_PPP,NULL, DnsName)) != NDK_ERR_PARA)
		||((ret1=NDK_NetDnsResolv(COMM_TYPE_PPP,DnsIP, NULL)) != NDK_ERR_PARA)
		||((ret2=NDK_NetDnsResolv(COMM_TYPE_UNKNOW,DnsIP, DnsName)) != NDK_ERR_NET_UNKNOWN_COMMTYPE)){
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1,ret2);
		goto ERR;
	}
	
	//case2:����ʹ�ò���
	memset(DnsIP, 0, sizeof(DnsIP));
	if ((ret=NDK_NetDnsResolv(COMM_TYPE_PPP,DnsIP, DnsName)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//case3:�������Խ���www.126.com  20161019
	memset(DnsIP126, 0, sizeof(DnsIP126));
	if ((ret=NDK_NetDnsResolv(COMM_TYPE_PPP,DnsIP126, DnsName126)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��!\n�ٶ�����:%s IP:%s\n126����:%s IP:%s\n", TESTAPI, DnsName, DnsIP, DnsName126, DnsIP126);
ERR:
	NetDown();
	return;
}

