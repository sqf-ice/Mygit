/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��ģ��
* file name			: 
* Author 			: jiangym
* version			: 
* DATE				: 20130911
* directory 			: 
* description			: ɨ��ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"ɨ��ģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan
* functional description 	: ɨ��ģ�����������,�ɸ�����Ҫ��չ
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        jiangym           20130911       created
*****************************************************************/
void scan(PF* list, unsigned char id[])
{
	/*private & local definition*/

	/*process body*/
	if(auto_flag==1)
	{
		g_UCID = 0;
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTMODULE);
		return;
	}
	
	cls_show_msg1(1, "%s����...", TESTMODULE);	
	process(list, id);
	
	return;
}

