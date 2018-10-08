/************************************************************************
* copyright(c)2005-2016 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name			: 32.c
* Author 			: zhengry
* version			: 
* DATE				: 20160511
* directory 			: 
* description			: NDK_LedSetBright�������������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_LedSetBright"
#define MAXWAITTIME  15

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scr32
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					    zhengry		    20160511		created
*****************************************************************/
void scr32(void)
{
	/*private & local definition*/
	int ret=0, i=0, nKeyin=0;
	char buf[7]="888888";
	
	/*process body*/
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_LedClr();

	//case1:�쳣��������:rate=-1/8ʱ
	if((ret=NDK_LedSetBright(-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedSetBright(8)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��������:������������Ϊ0~7��ʱ������Ӧ�����α���
	for(i=0; i<8; i++)
	{
		if((ret=NDK_LedSetBright(i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		if((ret=NDK_LedDisp(0, buf)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		NDK_SysDelay(10);
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "led��������α���.��[ENTER],����[����].");
	if( nKeyin != ENTER &&  nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_LedSetBright(3);
	NDK_LedClr();
	return;
}

