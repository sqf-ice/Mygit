/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: nfcģ��
* file name		: 
* Author 			: huangjianb
* version			: 
* DATE			: 20141020
* directory 		: 
* description		: nfcģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"nfcģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: nfc
* functional description 	:nfcģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date			remarks
*			  	  			huangjianb 	20130107	created
*				  	
*****************************************************************/
void nfc(PF *list, unsigned char id[])//��ں������������ļ���,Ҳ��ģ����
{
	/*private & local definition*/
	
	/*process body*/
	cls_show_msg1(1, "%s����...", TESTMODULE);
	
	process(list, id);
	
	//ģ����Խ���
	return;
}
