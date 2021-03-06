/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 工具模块
* file name		: util.c
* Author 			: jiangym
* version			:
* DATE				: 20130107
* directory 	:
* description	: 工具模块总入口,本模块的公共函数与公共定义也可以放于此处
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
#define	TESTMODULE	"工具模块"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: util
* functional description 	: 工具模块入口主函数,可根据需要扩展
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
		send_result("%s可自动测试，请使用自动测试验证", TESTMODULE);
		return;
	}
	//以下为函数体
	cls_show_msg1(1, "%s测试...", TESTMODULE);
	process(list, id);

	//模块测试结束
	return;
}
