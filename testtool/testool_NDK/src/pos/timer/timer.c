/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: timerģ��
* file name		: 
* Author 			: huangbs
* version			: 
* DATE			: 20120830
* directory 		: 
* description		: timerģ�������
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define	TESTMODULE	"timerģ��"

/****************************************************************
* function name 	 	: timer
* functional description 	: timerģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  huangbs         20120905	created
*				  	
*****************************************************************/
void timer(PF *list, unsigned char id[])
{
	if(auto_flag==1)//��֧���Զ�����
	{
		g_UCID = 0;
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTMODULE);
		return;
	}
	cls_show_msg1(1, "%s����...", TESTMODULE); 
	process(list, id);
	return;
}

