/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: event1ģ��
* file name	: event1.c
* Author 		: linwl
* version		: 
* DATE			: 20160912
* directory 		: 
* description		: ����Ndk_beginTransactions������������,��ռ��ȫ֧��ģ��.����Ndk_endTransactions�ر�����,������ռ��ȫ֧��ģ��.����Ndk_getStatus��ȡ��ȫ֧��ģ���״̬�Ĺ���
* related document	: �߶�ƽ̨ͳһAPI��̬���ӿ����ֲ�V2.00.doc
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"Ndk_beginTransactions,Ndk_endTransactions,Ndk_getStatus"
#define PINTIMEOUT_MAX	(200*1000)
#define PINTIMEOUT_MIN	(5*1000)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date		remarks
*			  	  	   		linwl       20160912	created
*****************************************************************/
void event1(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1, get_status = -1;

	/*process body*/
	cls_printf("����%s...", TESTAPI);
	
	//case1:�쳣���ԣ���ʱʱ��Ƿ����
	if((ret = Ndk_beginTransactions(-1))==NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��һ����ȷ����Ndk_beginTransactionsӦ�÷��سɹ�,��ʱʱ��Ϸ�10��
	if((ret = Ndk_beginTransactions(10))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:�������ǰ�ظ���������Ӧ�÷���NDK_ERR_POSNDK_TRANS_ALREADY
	if((ret = Ndk_beginTransactions(10))!=NDK_ERR_POSNDK_TRANS_ALREADY)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:���û�ȡ״̬����Ӧ�÷��سɹ�
	if((ret = Ndk_getStatus())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case5:������������Ӧ�óɹ�
	if((ret = Ndk_endTransactions())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case6:�������������,�ٵ�����������Ӧ�÷���NDK_ERR_POSNDK_TRANS_NOEXIST
	if((ret = Ndk_endTransactions())!=NDK_ERR_POSNDK_TRANS_NOEXIST)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case7:�������������,�ٴο���Ӧ�óɹ�,��ʱʱ��Ϸ�3��
	if((ret = Ndk_beginTransactions(3))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case8:����ʱ������,�ٴο�������Ӧ�óɹ�,��ʱʱ��Ϸ�3��
	sleep(4);
	if((ret = Ndk_beginTransactions(3))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case9:����ʱ������,�ٵ�����������Ӧ�÷���NDK_ERR_POSNDK_TRANS_NOEXIST
	sleep(4);
	if((ret = Ndk_endTransactions())!=NDK_ERR_POSNDK_TRANS_NOEXIST)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case10:���û�ȡ״̬����Ӧ�÷��سɹ�
	if((ret = Ndk_getStatus())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

