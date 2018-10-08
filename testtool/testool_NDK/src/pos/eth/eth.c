/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ��̫��ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"��̫��ģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: eth
* functional description 	: ��̫��ģ�����������
* input parameter	 	:
* output parameter	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  						chensl		   20130105	 		created
*****************************************************************/
void eth(PF* list, unsigned char id[])
{
	int ret=-1;
	
	//����Ϊ������
	cls_show_msg1(1, "%s����...", TESTMODULE);

	switch(conf_conn_ETH(FALSE))
	{
	case SUCC:
		break;
	case FAIL:
		cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
	default:
		return;
		break;
	}
	
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME/*|STATUSBAR_DISP_ETH*/))!=NDK_OK)
	{
		cls_show_msg("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif
	process(list, id);
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	
	//ģ����Խ���
	return;
}

