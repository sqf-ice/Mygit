/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util14.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ����NDK_IsLeapYear�ӿڹ���
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
#define 	TESTAPI		"NDK_IsLeapYear"

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
*			  	  	     jiangym         20130110  	   created
*****************************************************************/
void util14(void)
{
	//����Ϊ�ֲ�����
	int ret = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:nYearΪ�Ƿ���-1����ʧ��
	if((ret=NDK_IsLeapYear(-1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��������:nYearΪ2006 ��2008��������
	if((ret=NDK_IsLeapYear(2006))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_IsLeapYear(2008))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	 	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

