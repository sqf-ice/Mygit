/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys12.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysDelay,NDK_SysMsDelay,NDK_SysStartWatch,NDK_SysStopWatch�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if defined ME15CHX||defined ME15B||defined ME66
#define	TESTAPI		"NDK_SysMsDelay,NDK_SysReadWatch"
#else
#define	TESTAPI		"NDK_SysDelay,NDK_SysMsDelay,NDK_SysStartWatch,NDK_SysStopWatch"
#endif

#define MAXWAITTIME 30
#if K21_ENABLE //K21ƽ̨���и�����ʾ�ͼ��㶼��Ч
#if defined ME15C||defined ME15CHX||defined ME15B||defined ME66
#define WUCHASEC  2 //���ֵ����
#else
#define WUCHASEC  1 //���ֵ����
#endif
#else
#define WUCHASEC  0.03 //���ֵ����
#endif
#define	MAX_WUCHASEC	1.0
#define	TESTTIME	60

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
*			  		      linwei	        20130121	 	created
*****************************************************************/
#if !(defined ME15CHX||defined ME15B||defined ME66)
void sys12(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int i,ret = -1;
	unsigned int unTime = 0, unTime2 = 0;

	//����Ϊ������
	cls_printf("������ʱAPI,���Թ����������������(��ҪһЩʱ��,��ȴ�3����)...");

	//case1: �쳣���ԣ��������
	if((ret=NDK_SysStopWatch(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysDelay(0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysMsDelay(0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2: NDK_SysDelay��ʱTESTTIME��
	NDK_SysStartWatch();
	NDK_SysDelay(TESTTIME*10);//100msΪ��λ
	NDK_SysStopWatch(&unTime);
#if K21_ENABLE
	if (abs(unTime/1000-60) >= WUCHASEC)
#else
	if (fabs(unTime/1000.0-60.0) >= WUCHASEC)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, unTime);
		RETURN;
	}
	
	//case3: NDK_SysMsDelay��ʱTESTTIME��
	NDK_SysStartWatch();
	NDK_SysMsDelay(TESTTIME*1000);// 1msΪ��λ
	NDK_SysStopWatch(&unTime);
#if K21_ENABLE
	if (abs(unTime/1000-60) >= WUCHASEC)
#else
	if (fabs(unTime/1000.0-60.0) >= WUCHASEC)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, unTime);
		RETURN;
	}

	//case4: �����ε���NDK_SysStartWatch��ÿ�ε��ö��ᵼ���ܱ����¿�ʼ��ʱ��
	for(i=0; i<1000; i++)
		NDK_SysStartWatch();
	NDK_SysStopWatch(&unTime);
#if K21_ENABLE
	if(unTime/1000 >= MAX_WUCHASEC)
#else
	if(unTime/1000.0 >= MAX_WUCHASEC)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, unTime);
		RETURN;
	}

	//case5:���NDK_SysStopWatch
	NDK_SysMsDelay(1);
	NDK_SysStopWatch(&unTime2);
#if !K21_ENABLE
	if(unTime2 <= unTime)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, unTime2, unTime);
		RETURN;
	}
#endif
	//���Խ���
	send_result("�ܱ�,��ʱAPI����ͨ��");
	return;
}
#else
void sys12(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int ret = -1;
	uint unTime = 0, unTime2 = 0, time = 0;

	//����Ϊ������
	cls_printf("������ʱAPI,���Թ����������������(��ҪһЩʱ��,��ȴ�1����)...");

	//case1: �쳣���ԣ��������
	if((ret=NDK_SysReadWatch(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysMsDelay(0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2: NDK_SysMsDelay��ʱTESTTIME��
	NDK_SysReadWatch(&unTime);
	NDK_SysMsDelay(TESTTIME*1000);// 1msΪ��λ
	NDK_SysReadWatch(&unTime2);
	time = unTime2-unTime;
	if (abs(time/1000-60) >= WUCHASEC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, time);
		RETURN;
	}

	send_result("%s����ͨ��%d,%d", TESTAPI,unTime,unTime2);
	return;
}
#endif

