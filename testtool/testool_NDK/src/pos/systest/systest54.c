/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	触屏/磁卡交叉
* file name			: systest54.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: 触屏/磁卡交叉测试
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20131202  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"触屏/磁卡交叉"
#define	DEFAULT_CNT_STR	"10"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)

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
* history 				: author			date			remarks
					 	jiangym			20131230	created
*****************************************************************/	
void systest54(void)
{
	if(auto_flag==1)//不支持自动测试
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s不支持自动测试，请使用手动测试验证", TESTITEM);
		return;
	}

	int ret = 0, bak = 0,cnt = 0, succ = 0;
	uint unLen = 0;
	char  CntStr[8] = DEFAULT_CNT_STR;

	cls_printf("默认交叉测试次数:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//按取消,则使用default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	while(1)
	{
		if(bak++==cnt)
			break;
		if(ESC==cls_show_msg1(2, "正在进行%d次%s测试,成功%d次,ESC退出", bak, TESTITEM, succ))
			break;

		//触屏测试
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s测试失败", __LINE__, TESTITEM);
			continue;
		}
		
		//磁卡测试
		if(systest_MagCard()!=STRIPE)
			continue;

		succ++;
	}
	cls_show_msg("%s测试完成,总共测试%d次成功%d次\n", TESTITEM, bak-1, succ);
	return;
}

