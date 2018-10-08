/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:��������ȡ����
* file name			: kb6.c
* Author 			:
* version			: 
* DATE				: 
* directory 			: 
* description			: ���� NDK_TSKbd_Ctrl�Ƿ�����ȷ������K21�ӹܻ����ͷŴ������Լ�
*					����NDK_TSKbdGetXY�Ƿ���ȷ��ȡ�����ĺ�������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  TESTAPI		"NDK_TSKbd_Ctrl,NDK_TSKbdGetXY"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: kb6
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        jiangym		20150513	created
*****************************************************************/
void kb6(void)
{
	/*private & local definition*/
	int ret = -1;
	uint x = 1, y = 1;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:��K21�ͷŴ������Ŀ���ʱ������NDK_TSKbdGetXY�᷵��ʧ��
	if((ret=NDK_TSKbd_Ctrl(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TSKbdGetXY(2,&x,&y))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�����������һ����Ч
	if((ret=NDK_TSKbd_Ctrl(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TSKbd_Ctrl(2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TSKbdGetXY(2,&x,&y))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:��k21�ӹܴ�����,x y����ΪNULLʱ,���سɹ�,��Ϊֻ���ڵȴ�ʱ�䰴��,��û��Ҫ��ȡ��ǰ�İ�������
	if((ret=NDK_TSKbd_Ctrl(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TSKbdGetXY(2,NULL,&y))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TSKbdGetXY(2,&x,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TSKbdGetXY(2,NULL,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:��k21�ӹܴ�����,�涨ʱ����δ��������,x y����ֵΪ0
	//�ٴ�������Ч
	if((ret=NDK_TSKbd_Ctrl(3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_printf("��Ҫ��������,�ȴ�2s");
	if((ret=NDK_TSKbdGetXY(2,&x,&y))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(x!=0 || y!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, x, y);
		RETURN;
	}

	//case5:k21�ӹܴ�����,�涨ʱ���ڰ�������,�жϻ�ȡ��x yֵӦ����ȷ	
	cls_printf("��5s�ڰ�������������һ���ſ�");
	if((ret=NDK_TSKbdGetXY(5,&x,&y))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(cls_show_msg("�����������ֵΪ(%d,%d),[Enter]��ȷ,[����]����", x, y)!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, x, y);
		RETURN;
	}

	//case6:k21�ӹܴ�����,���Ȱ���,�жϻ�ȡ��x yֵӦ����ȷ
	cls_printf("���ڴ�������,��,��,��,��,����,����,����,���¾Ÿ�λ�����ⰴ�º���Ӧ���Ի�ȡ����Ӧ����,һ���Ӻ��˳�����");
	lib_StartStopwatch();
	while(1)
	{
		if (lib_ReadStopwatch()>60)//һ���Ӻ��˳�����
			break;
		if((ret=NDK_TSKbdGetXY(0,&x,&y))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
		cls_printf("�����������ֵΪ(%d,%d),������һ��λ�õĲ���", x, y);
	}
	if(cls_show_msg("�жϸջ�ȡ�ĸ���λ�õ�����ֵ�Ƿ���ȷ,[Enter]��ȷ,[����]����")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

