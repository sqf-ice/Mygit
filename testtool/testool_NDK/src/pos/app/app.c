/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: 
* Author 			: daitt
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: 
* related document	: NDK.chm
*
************************************************************************/
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: Ӧ�ù���ģ��
* file name		: 
* Author 			: daitao
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: Ӧ�ù���ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"appģ��"

/*------------global variables definition-----------------------*/
#define MAXWAITTIME 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr
* functional description 	: ��ʾģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        daitao         20121224  	   created
*****************************************************************/
void app(PF *list, unsigned char id[])
{
	/*private & local definition*/

	/*process body*/
#if !K21_ENABLE
	if(cls_show_msg1(MAXWAITTIME, "%s����...����POS/appfs/downloadĿ¼���ز���NLD��,ESC�˳���ģ�����", TESTMODULE)==ESC)
#else
	if(cls_show_msg1(MAXWAITTIME, "%s����...����POS/appfsĿ¼���ز���NLD��,ESC�˳���ģ�����", TESTMODULE)==ESC)
#endif
		return;
	process(list, id);
	return;
}

