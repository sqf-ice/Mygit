/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys99.c
* Author 			: linwl
* version			: 
* DATE				: 20140621
* directory 		: 
* description		: ���Զ����С����ܵ�ѹ����������
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"�Զ�����ѹ��"

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
*			  		        linwl	           20140621	 	created
*****************************************************************/
void sys99(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	
	//����Ϊ������
#if 0
	NDK_SysDelay(50);//�ȴ�PMS��ȫ����
	if((ret=NDK_SysReboot())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	cls_show_msg("������ʾ����Ϣ���Զ�����ʧ�ܣ�����ʧ��");
	return;
#else
	cls_printf("������...���Ժ�");
	while(1)
	{//��֣����ͨ�Ľ����NDK_SysReboot������PMSδ��������»�ʧ�ܣ�	��������ʧ������²��ϳ���������ĳ���ܹ���������������
		NDK_SysDelay(5);
		if((ret=NDK_SysReboot())!=NDK_OK)
		{
			send_result("����ʧ��(%d),����������,��������Ϊ�������,��ʱ��ͣ���ڱ�������Ϊ�쳣���", ret);//�ý������ʾ��
			continue;
		}
	}
#endif
}

