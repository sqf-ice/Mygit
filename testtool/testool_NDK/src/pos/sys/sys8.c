/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys8.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysTimeBeep�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysTimeBeep"

#define	MAXWAITTIME 30
#define WUCHASEC 0.015
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
*			  		        linwei	           20130121	 	created
*****************************************************************/
void sys8(void)
{
	if(auto_flag==1)
		send_result("%s�Զ����Բ�����Ϊ���ղ��Խ���������ֶ�������֤", TESTAPI);
	//����Ϊ�ֲ�����
	int ret = -1, nKeyin = 0;
	float fTimeElapsed = 0.0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
#if !K21_ENABLE
	NDK_SysSetBeepVol(5);
#endif
	//case1:�쳣���ԣ��������
	if((ret=NDK_SysTimeBeep(0,1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysTimeBeep(1,0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysTimeBeep(4001,1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:������С�߽�ֵ
	if((ret=NDK_SysTimeBeep(1, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:��������������ʱ���ֵ�������������
	cls_printf("��������������5��");
	lib_StartStopwatch();
	if((ret=NDK_SysTimeBeep(2000, 5000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	fTimeElapsed=lib_ReadStopwatch();
	if((fTimeElapsed-5)>=WUCHASEC)
	{
		send_result("line %d:%s����ʧ��(%f)", __LINE__, TESTAPI, fTimeElapsed);
		RETURN;
	}
	
	//case4: ��100,1000,4000Ƶ����1��
	cls_show_msg1(3, "���Ե��и�Ƶ�ʷ�����Ӧ���");
	if((ret=NDK_SysTimeBeep(100,1000)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysTimeBeep(1000,1000)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysTimeBeep(4000,1000)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if !K21_ENABLE
	nKeyin = cls_show_msg1(MAXWAITTIME, "���е�Ƶ�ʸ�����1��,[ȷ��]ͨ��,[����]ʧ��");
#else
	nKeyin = cls_show_msg1(MAXWAITTIME, "�Ͷ�ƽ̨��������3��,[ȷ��]ͨ��,[����]ʧ��");//�Ͷ�ƽ̨��֧��Ƶ�ʲ���������Ч��һ�� 20141022 linwl
#endif
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;	
}

