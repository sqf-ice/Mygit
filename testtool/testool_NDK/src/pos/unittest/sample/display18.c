/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 18.c
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20070423
* directory 		: .\SRC\display\
* description		: ����TurnOnLCD/TurnOffLCD�ܷ��/�ر�LCD����
* related document	: 
*
************************************************************************
* log			: 
* Revision 1.0  20070423 11:10 zhangnw
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  UCID  18
#define  MAXWAITTIME (30)
#define 	TESTAPI		"TurnOnLCD/TurnOffLCD"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: display18
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20070423 	 	created
*****************************************************************/
void display18(void)
{
	/*private & local definition*/
	int nKeyin = 0;
		
	/*process body*/
	//cls_printf("TurnOnLCD/TurnOffLCD������...�뿴Ч��!");

	cls_show_msg1(MAXWAITTIME, "%d��󽫴򿪱���,�뿴Ч��!", MAXWAITTIME);
	TurnOnLCD();

	cls_show_msg1(MAXWAITTIME, "%d��󽫹رձ���,�뿴Ч��!", MAXWAITTIME);
	TurnOffLCD();
	
	nKeyin = cls_show_msg1(MAXWAITTIME, "����Ч��,��ȷ��ͨ��,��������ͨ��");
	if(nKeyin==ENTER||nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
}

