/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys100.c
* Author 			: linwl
* version			: 
* DATE				: 20140621
* directory 		: 
* description		: ����������Ļ��ʾ���桱��������
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"��������Ļ��ʾ����"

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
void sys100(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int nCode = 0;
	
	//����Ϊ������
	NDK_ScrAutoUpdate(0,NULL);
	NDK_ScrStatusbar(STATUSBAR_DISP_ALL);
	while (1)
	{
		cls_printf("���ҹ��ҹ��ҹ������ҹ��ҹ��ҹ������ҹ��ҹ��ҹ����ҹ��ҹ��ҹ����ҹ��ҹ��ҹ����ҹ��ҹ��ҹ����ҹ��ҹ��ҹ����ҹ��ҹ��ҹ����ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ��ҹ���");
		NDK_SysMsDelay(10);
		cls_printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
		NDK_SysMsDelay(10);
		if(NDK_KbHit(&nCode)==NDK_OK)
		{
			if(nCode==ESC)
				return;
		}
	}
}

