/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys6.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysBeep�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysBeep"
#define	MAXWAITTIME 30

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
void sys6(void)
{
	if(auto_flag==1)
		send_result("%s�Զ����Բ�����Ϊ���ղ��Խ���������ֶ�������֤", TESTAPI);
	//����Ϊ�ֲ�����
	int i = 0;
	int nKeyin = ENTER;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
#if !K21_ENABLE
	NDK_SysSetBeepVol(5);
#endif

	//case 1:������10��
	for (i=0; i<10; i++)
	{
		NDK_SysBeep();
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}

