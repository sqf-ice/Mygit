/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		: alg.c
* Author 			: wangzy
* version			: 
* DATE				: 20130319
* directory 	: 
* description	: �㷨ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
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
#define	TESTMODULE	"�㷨ģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: alg
* functional description 	: �㷨ģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  						wangzy		   20130319	 		created
*****************************************************************/
void alg(PF* list, unsigned char id[])
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

