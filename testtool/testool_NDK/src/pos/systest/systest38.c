/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: systest38.c
* Author 			: chensj
* version			: V1.0
* DATE			: 20130225
* directory 		: 
* description		: ��ӡ/���̽���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 1.0  20130225 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM		"��ӡ/���̽���"

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					chensj		20130225       	created
*****************************************************************/
//��д����ע�⣺�������Ҫ������������Ƚ���!����,ҲҪ��һ����ǿ�ȡ�
void systest38(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	
	/*process body*/
	cls_show_msg("%s����,���ڴ�ӡʱ��������,��ע���ӡЧ��", TESTITEM);
	conf_prn();
	prn_press(TESTITEM, NULL);
	return;
}

