/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 20.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrGetVer�ܷ��ȡ��ʾģ��汾
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrGetVer"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr20
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr20(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	char pszVer[20] = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣:pszVerΪNULL
	if((ret=NDK_ScrGetVer(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��ȡ��ʾģ��汾
	if((ret=NDK_ScrGetVer(pszVer))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin=cls_show_msg1(5, "�汾Ϊ%s,��[ȷ��]����ͨ��,[����]ʧ��",pszVer);
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

	return;
}

