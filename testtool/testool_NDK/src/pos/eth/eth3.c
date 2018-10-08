/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth3.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_GetDnsIp�ӿڹ���
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
#define 	TESTAPI		"NDK_GetDnsIp"

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
void eth3(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1;
	char DnsName[16] = "www.baidu.com";
	char DnsIP[16] = {0};

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
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
	if(((ret=NDK_GetDnsIp(NULL, DnsName)) != NDK_ERR_PARA)
		||((ret1=NDK_GetDnsIp(DnsIP, NULL)) != NDK_ERR_PARA)){
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
	
	//case2:����ʹ�ò���
	memset(DnsIP, 0, sizeof(DnsIP));
	if ((ret=NDK_GetDnsIp(DnsIP, DnsName)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��!����:%s IP:%s", TESTAPI, DnsName, DnsIP);
ERR:
	NetDown();
	return;
}

