/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			:asynģ��
* file name		: 
* Author 			: chenjings
* version			: 
* DATE			: 20130114
* directory 		: 
* description		: asynģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"�첽MODEM"
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: asyn
* functional description 	: asynģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenjings      20130114	created
*****************************************************************/
void asyn(PF* list, unsigned char id[])//��ں������������ļ���,Ҳ��ģ����
{
	//����Ϊ�ֲ�����
	int ret=-1;
	
	//����Ϊ������
	cls_show_msg1(5,"%s����,���PC�˺�̨����������·,���������...", TESTMODULE);
	if(auto_flag==1)//�Զ�����ʹ�ܵ��������ļ��ı�־��������ú����ͻ�ʹ�������ļ��������
 		g_conf_flag=1;
	else
		g_conf_flag=0;
	if(conf_link(ASYN)!=SUCC)
	{
		cls_show_msg("������������Ϊ�첽èʧ��!");
		return;
	}
	//����process����ģ�鵥Ԫ����
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_MODEM))!=NDK_OK)
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

