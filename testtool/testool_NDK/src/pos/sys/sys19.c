/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys19.c
* Author 			: jiangym
* version			: 
* DATE				: 20130821
* directory 			: 
* description			: ����NDK_SysTime�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysTime"

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
*			  		     
*****************************************************************/
void sys19(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int nRet = 0;
	ulong ultime=0, ultime1=0, subtime=0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�����쳣����
	if((nRet = NDK_SysTime(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//case2:��������
	if((nRet = NDK_SysTime(&ultime)) != NDK_OK)//���ؾ�������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	sleep(5);
	if((nRet = NDK_SysTime(&ultime1)) != NDK_OK)//���ؾ�������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((subtime=ultime1-ultime)!=5)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, subtime);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

