/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: timerģ��
* file name			: timer4.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����NDK_DeRegisterTimer�����ܷ�ɾ����ʱ��
* related document	: NDK.chm
* 
************************************************************************/
#include "mode.h"
#define TESTAPI	"NDK_DeRegisterTimer"

/****************************************************************
* function name 	 	: void timer4(void)
* functional description 	: ��ʱ��ɾ������
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	 		jiangym		20140530	created
*****************************************************************/
static void func(void)
{
	static int i=0;

	if(i<3)
		cls_printf("��%d�λص�����func(2s�󽫸��´���)", ++i);
	else	
	{
		i = 0;
		NDK_DeRegisterTimer();
	}
	
}

void timer4(void)
{
	int ret= -1;

	cls_printf("����%s...", TESTAPI); 

	//case 1:ע����ʱ�ص�����,֮����time_interval��������Զ��ص�
	if((ret=NDK_RegisterTimerRoutine(2000, func))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	sleep(8);
	if(cls_show_msg("û�г�����ʾ:��4�λص�����func,[Enter]��ȷ,[����]ʧ��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

