/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��Ƶģ��
* file name			: 
* Author 			: jiangym
* version			: 
* DATE				: 20131028
* directory 			: 
* description			: ��Ƶģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"��Ƶģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: voice
* functional description 	: ɨ��ģ�����������,�ɸ�����Ҫ��չ
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        jiangym           20131028       created
*****************************************************************/
void voice(PF* list, unsigned char id[])
{
	/*private & local definition*/

	/*process body*/
	if(auto_flag==1)//��Ƶģ��֧���Զ����ԣ�����Ҫ�ֶ����Ը���
	{
		g_UCID = 0;
		send_result("%s�Զ����Բ�����Ϊ���ղ��Խ���������ֶ�������֤", TESTMODULE);
		//return;
	}
	
	cls_show_msg1(1, "%s����...", TESTMODULE);	
	process(list, id);
	
	return;
}

