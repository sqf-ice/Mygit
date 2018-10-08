/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name		: sckt.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: socketģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
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
#define	TESTMODULE	"socketģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: sckt
* functional description 	: socketģ�����������
* input parameter	 	:
* output parameter	: 
* return value		 	:
* history 		 		: 
*****************************************************************/
void sckt(PF* list, unsigned char id[])
{
	//����Ϊ������
	cls_show_msg1(1, "%s����...", TESTMODULE);
	
	//����ǰ��
	switch(conf_conn_sock())
	{
	case SUCC:
		break;
	case FAIL:
		cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
	case NDK_ERR_QUIT://�û�����
	default:
		return;
		break;
	}
	
	process(list, id);
	
	//ģ����Խ���
	return;
}

