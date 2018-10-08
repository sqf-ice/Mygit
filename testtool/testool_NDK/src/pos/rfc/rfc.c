/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: rfcģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"rfcģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: rfc
* functional description 	:rfcģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenfm      20130107	created
*				  	
*****************************************************************/
void rfc(PF *list, unsigned char id[])//��ں������������ļ���,Ҳ��ģ����
{
	//����Ϊ�ֲ�����

	//����Ϊ������
	if(auto_flag==1)//��֧���Զ�����
	{
		g_UCID = 0;
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTMODULE);
		return;
	}
	cls_show_msg1(1, "%s����...", TESTMODULE);

	if(NDK_RfidInit(NULL)!=NDK_OK)
	{
		cls_show_msg("��Ƶ�ӿ�������ʼ��ʧ��,������˳�...");
		return;
	}
	if(NDK_RfidOpenRf()!=NDK_OK)
	{
		cls_show_msg("����Ƶ���ʧ��,������˳�...");
		return;
	}

	process(list, id);

	NDK_RfidPiccDeactivate(0);
	if(NDK_RfidCloseRf()!=NDK_OK)
	{
		cls_show_msg("����Ƶ���ʧ��,������˳�...");
		return;
	}
	return;
}

