/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 18.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ���Բ���NDK_ScrSetFontColor�ܷ�����������ɫ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrSetFontColor"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr18
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr18(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	color_t unColor = BLACK;	
	EM_TEXT_ATTRIBUTE iOldAttr[2] = {0};
	
	/*process body*/ 
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣:emTypeΪ-1,4
	if((ret=NDK_ScrSetFontColor(unColor,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetFontColor(unColor,4))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_ScrClrs();
	
	//case2:����ΪFONTCOLOR_NORMAL
	if((ret=NDK_ScrSetFontColor(unColor,FONTCOLOR_NORMAL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	if(ENTER != show_msg1(MAXWAITTIME,"����������ʾʱ������ɫΪ��ɫ,��ȷ�ϲ���ͨ����������ʧ��\n")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3:����ΪFONTCOLOR_REVERSE	
	NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,iOldAttr);
	if((ret=NDK_ScrSetFontColor(WHITE,FONTCOLOR_REVERSE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	if(ENTER != show_msg1(MAXWAITTIME,"���巴����ʾʱ������ɫΪ��ɫ,��ȷ�ϲ���ͨ����������ʧ��\n")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	NDK_ScrClrs();
	NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,iOldAttr);
	//case4:����ΪFONTCOLOR_BG_REVERSE
	if((ret=NDK_ScrSetFontColor(unColor,FONTCOLOR_BG_REVERSE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	NDK_ScrRefresh();
	nKeyin = show_msg1(MAXWAITTIME, "���巴����ʾʱ������ɫΪ��ɫ,��ȷ�ϲ���ͨ����������ʧ��\n");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

	//�ָ�Ĭ������
ERR:
	NDK_ScrSetAttr(iOldAttr[0],iOldAttr+1);
	NDK_ScrSetFontColor(BLACK,FONTCOLOR_NORMAL);
	NDK_ScrRefresh();
	return;
}

