/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 5.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrClrs�Ƿ���������Ļ,���ѹ���Ƶ�(0,0),������ʾ���Իָ�����
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrClrs"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr5
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr5(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	EM_TEXT_ATTRIBUTE OldAttr = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//case1:����NDK_ScrClrs
	NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,&OldAttr);
	NDK_ScrPrintf("����ǰ\n");
	NDK_ScrRefresh();
	show_msg1(MAXWAITTIME, "�������������,������Ϣ��(0,0)��������ʾ");
	if((ret=NDK_ScrClrs())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	nKeyin = show_msg1(MAXWAITTIME, "(0,0)��������ʾ,��ȷ�ϲ���ͨ��,������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	
	return;
}

