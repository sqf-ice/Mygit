/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util15.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ����NDK_MonthDays   �ӿڹ���
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
#define 	TESTAPI		"NDK_MonthDays"

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
void util15(void)
{
	//����Ϊ�ֲ�����
	int year = 2000;
	int mon = 1, day = 10;
	int ret = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_MonthDays(year,mon,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:nyearΪ-1 Ӧ����ʧ�� 
	if((ret=NDK_MonthDays(-1,mon,&day))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:nMonС��1���ߴ���12��Ӧ����ʧ��
	year = 2000;
	if((ret=NDK_MonthDays(year,0,&day))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_MonthDays(year,13,&day))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:��������:nYearΪ1996��nMonΪ6���򷵻�pnDays=30
	if((ret=NDK_MonthDays(1996,6,&day))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(day!=30)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case5:��������:nYearΪ2019��nMonΪ7���򷵻�pnDays=31
	if((ret=NDK_MonthDays(2019,7,&day))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(day!=31)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case6:��������:����2�� nYearΪ2008��nMonΪ2���򷵻�pnDays=29
	if((ret=NDK_MonthDays(2008,2,&day))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(day!=29)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case7:��������:������2�� nYearΪ2015��nMonΪ2���򷵻�pnDays=28
	if((ret=NDK_MonthDays(2015,2,&day))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(day!=28)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}

