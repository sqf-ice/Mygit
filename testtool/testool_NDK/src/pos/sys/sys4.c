/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys4.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysReboot�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysReboot"

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
void sys4(void)
{
	if(auto_flag==2)
		return;
	//case1:���ܲ���
#if !K21_ENABLE //���ݻ������۽�� K21Ӳ������Ҫ���ŵ�Դ�߲�������
	send_result("���ڲ������˵���[<-]����%s��֤", TESTAPI);
#else
	send_result("����ϵ�Դ�ߺ�,�ڲ������˵���[�˸��]����%s��֤", TESTAPI);
#endif
	return;
}

