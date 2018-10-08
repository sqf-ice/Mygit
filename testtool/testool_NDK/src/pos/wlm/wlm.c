/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module						: wlm����ͨѶģ��
* file name					: 
* Author 						: chensj
* version						: 
* DATE						: 
* directory 					: 
* description					: wlm����ͨѶģ�������
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
#define	TESTMODULE	"WLMģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: wlm
* functional description 	: wlmģ�����������,�ɸ�����Ҫ��չ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						: author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void wlm(PF* list, unsigned char id[])
{
	/*private & local definition*/
#if STABAR_ENABLE
	int ret=-1;
#endif

	/*process body*/
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_ALL))!=NDK_OK)
	{
		cls_show_msg("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif
	cls_printf("%s����...", TESTMODULE);

	if(conf_conn_WLM(FALSE)!=SUCC)//==QUIT)
		return;

	cls_printf("��ʼ������MODEM��...\n���Ժ�...\n");

//	NDK_WlShowSignalQuality(-1,-1);//��������(AT����)����ǰ,�ȹ��ź���ʾ(�رտ���̨,�����������̳�ͻ)

	process(list, id);
	
//	NDK_WlShowSignalQuality(0, 0);//��������(AT����)����ǰ,�ȹ��ź���ʾ(�رտ���̨,�����������̳�ͻ)
#if STABAR_ENABLE
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif

//ģ����Խ���
	return;
}

