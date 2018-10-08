/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc8.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_GetRfidType�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_GetRfidType"

/*------------global variables definition-----------------------*/

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
*			  		    chenfm	        20130108	 	created
*****************************************************************/
void rfc8(void)
{
	//����Ϊ�ֲ�����
	int ret= -1, pnRfidtype= -1;
	char *Em_type[]={"RFID_Autoscan", "RFID_RC531", "RFID_PN512", "RFID_AS3911", "RFID_RC663", "RFID_MH1608"};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�쳣����
	if((ret=NDK_GetRfidType(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	} 

	//case2:������ȡ��Ƶ��оƬ����
	if((ret=NDK_GetRfidType(&pnRfidtype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(pnRfidtype<0||pnRfidtype>SZ_ARRAY(Em_type)-1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pnRfidtype);
		return;	
	}
	if(ENTER!=cls_show_msg("��ǰ��ƵоƬ����Ϊ:%s\n��ȷ��[ENTER],����������", Em_type[pnRfidtype]))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, Em_type[pnRfidtype]);
		RETURN;
	}  

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

