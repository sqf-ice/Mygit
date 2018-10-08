/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys31.c
* Author 			: zhengry
* version			: 
* DATE				: 20151019
* directory 		: 
* description		: ����NDK_SysTimeBeepUnblock������ʽʵ�ְ�ָ����Ƶ����ָ����ʱ��
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysTimeBeepUnblock"	 

#define MAX_FREQUENCY 4000
#define	MAXWAITTIME 30
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	      zhengry       20151019       created
*****************************************************************/
void sys31(void)
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
	if((ret=NDK_SysTimeBeepUnblock(0, 1000))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysTimeBeepUnblock(MAX_FREQUENCY+1, 1000))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysTimeBeepUnblock(100, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:������С�߽�ֵ
	if((ret=NDK_SysTimeBeepUnblock(1, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:���Է����������ýӿ�֮ǰ�͵��ýӿ�֮���ʱ���Ӧ��ԶС�����õ�ʱ��
	cls_printf("��������������5��");
	lib_StartStopwatch();
	if((ret=NDK_SysTimeBeepUnblock(2000, 5000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	fTimeElapsed=lib_ReadStopwatch();
	if(fTimeElapsed >= 0.03)
	{
		send_result("line %d:%s����ʧ��(%f)", __LINE__, TESTAPI, fTimeElapsed);
		RETURN;
	}
	sleep(5);

	//case4:������Ƶ�ʴӵ͵������������Σ��۲�ʵ�ʷ����������Ƿ�Խ��Խ��
	cls_show_msg1(3, "���Ե��и�Ƶ�ʷ�����Ӧ���");
	if((ret=NDK_SysTimeBeepUnblock(100, 1000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	if((ret=NDK_SysTimeBeepUnblock(1000, 1000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	if((ret=NDK_SysTimeBeepUnblock(4000, 1000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
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

