/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module						: prnt��ӡģ��
* file name					: 
* Author 						: chensj
* version						: 
* DATE							: 
* directory 				: 
* description				: prnt��ӡģ�������
* related document	: DNKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"prntģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:prnt
* functional description 	: prntģ�����������,�ɸ�����Ҫ��չ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						: author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt(PF* list, unsigned char id[])
{
	/*private & local definition*/
	if(auto_flag==1)//��֧���Զ�����
	{
		g_UCID = 0;
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTMODULE);
		return;
	}
	/*process body*/
	cls_show_msg1(1, "%s����...", TESTMODULE);

	//����process����ģ�����ǰ,Ҫ�ȶ���·������linktype��ֵ
	conf_prn();
	lib_initprn(g_PrnSwitch);//NDK_PrnInit(g_PrnSwitch);
	process(list, id);
	
	//ģ����Խ���
	return;
}

