/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util.c
* Author 			: jiangym
* version			:
* DATE				: 20130107
* directory 	:
* description	: ����ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: NDK.chm
*
************************************************************************
* log			:
* Revision
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"����ģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: util
* functional description 	: ����ģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	:
* return value		 	:
* history 		 		:
*****************************************************************/
void util(PF* list, unsigned char id[])
{
	if(auto_flag==2)
	{
		g_UCID = 0;
		send_result("%s���Զ����ԣ���ʹ���Զ�������֤", TESTMODULE);
		return;
	}
	//����Ϊ������
	cls_show_msg1(1, "%s����...", TESTMODULE);
	process(list, id);

	//ģ����Խ���
	return;
}
