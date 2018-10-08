/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth7.c
* Author 			: chensj
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_NetAddRouterTable�ӿڹ���
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
#define 	TESTAPI		"NDK_NetAddRouterTable"

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
*			  		chensj		   20131127	 		created
*****************************************************************/
void eth7(void)
{
	//����Ϊ�ֲ�����
	char  szDestIP[32]={"192.169.4.199"}; 
	int nRet = 0;

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//����Ϊ������,�ú���������������systest45����֤���ð���ֻ���쳣����
	//case1:�쳣����
	if((nRet = NDK_NetAddRouterTable(-1,szDestIP))!=NDK_ERR_NET_UNKNOWN_COMMTYPE/*NDK_ERR_PARA*/)//20140102 �������Ϳ����޸��˵ײ㷵��ֵ
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__ , TESTAPI, nRet);
		RETURN;
	}

	//case2: �쳣����
	if((nRet = NDK_NetAddRouterTable(COMM_TYPE_UNKNOW+1,szDestIP))!=NDK_ERR_NET_UNKNOWN_COMMTYPE/*NDK_ERR_PARA*/)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__ , TESTAPI, nRet);
		RETURN;
	}

	//case3: δ֪����
	if((nRet = NDK_NetAddRouterTable(COMM_TYPE_UNKNOW,szDestIP))!=NDK_ERR_NET_UNKNOWN_COMMTYPE)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__ , TESTAPI, nRet);
		RETURN;
	}

#if !K21_ENABLE   //�Ͷ�ME50C ֧�ִ�NULL 20180328  �ڶ���������Ч 20180428 modify
	//case4: �쳣����
	if((nRet = NDK_NetAddRouterTable(COMM_TYPE_ETH,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__ , TESTAPI, nRet);
		RETURN;
	}

	//case5:��ЧIP
	if((nRet = NDK_NetAddRouterTable(COMM_TYPE_ETH,"192.168.4"))!=NDK_ERR_NET_INVALIDIPSTR)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__ , TESTAPI, nRet);
		RETURN;
	}
#endif
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}
