/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: timerģ��
* file name		: timer3.c
* Author 			: huangbs
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_RegisterTimerRoutine�ӿڹ���
* related document	: NDK.chm
* 
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_RegisterTimerRoutine"

#if CPU5810X_ENABLE
static void func1(void)
{
	static int i=0;

	if(i<600)
		++i;//cls_printf("��%d�λص�����func(10ms�󽫸��´���)", ++i);
	else
		cls_printf("ֹͣ��ʱ������ʱʱ��ӦΪ6�룬���ж��Ƿ����ͨ��!");
}
#endif

static void func(void)
{
	static int i=0;

	if(i<5)
		cls_printf("��%d�λص�����func(3s�󽫸��´���)", ++i);
	else
		cls_printf("�������,���ж��Ƿ����ͨ��.��ػ�����!");
}

/****************************************************************
* function name 	 	: void timer3(void)
* functional description 	: ע����ʱ�ص�����
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  linrq      huangbs	created
*****************************************************************/
void timer3(void)
{
	int ret= -1;

	cls_printf("����%s...", TESTAPI); 

	//case 1 ����Ƿ�����
	if((ret=NDK_RegisterTimerRoutine(10, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	
#if CPU5810X_ENABLE   //���5810xƽ̨��Ʒ�˺������Ȳ������Ӳ��Ե�
	//case 3:�ص����ȴﵽ10ms	2018003 sull add
	cls_show_msg1(30,"��׼��������������ʼ��ʱ");
	cls_printf("��ʱ��...");
	if(NDK_OK!=(ret=NDK_RegisterTimerRoutine(7, func1)))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(7);
#endif

	//case 2,ע����ʱ�ص�����,֮����time_interval��������Զ��ص�
	if(NDK_OK!=(ret=NDK_RegisterTimerRoutine(ONESECOND*3, func)))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	while(1);
	//send_result("%s����ͨ��", TESTAPI);
	//return;
}

