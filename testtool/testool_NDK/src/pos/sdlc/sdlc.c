/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: 
* Author 			: chenjings
* version			: 
* DATE			: 20130106
* directory 		: 
* description		: sdlcģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"ͬ��MODEM"
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: sdlc
* functional description 	: sdlcģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenjings      20130106	created
*****************************************************************/
void sdlc(PF* list, unsigned char id[])//��ں������������ļ���,Ҳ��ģ����
{
	//����Ϊ�ֲ�����
	int ret=-1;	
	
	//����Ϊ������
	cls_show_msg1(5,"%s����,��򿪺�̨�������Լ�ͷ,���������...", TESTMODULE);
 	if(auto_flag==1)//�Զ�����ʹ�ܵ��������ļ��ı�־��������ú����ͻ�ʹ�������ļ��������
 		g_conf_flag=1;
	else
		g_conf_flag=0;
	if(conf_link(SYNC)!=SUCC)
	{
		cls_show_msg("������������Ϊͬ��èʧ��!");
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

