/************************************************************************
*
* copyright(c)2012-2015 Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc13.c
* Author 			:  zhengry
* version			: 
* DATE				:  20150811
* directory 			: 
* description			: ����NDK_RfidFelicaPoll��felica��Ѱ�� 
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidFelicaPoll"

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
*			  		       zhengry	        20150811	 	created
*****************************************************************/
void rfc13(void)
{
	int ret=0, pUIDlen=0;
	uchar UID[300]={0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:��������
	if((ret=NDK_RfidFelicaPoll(NULL, &pUIDlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidFelicaPoll(UID, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�޿�����felica�������ڳ�ǿ��Χ��
	cls_show_msg("���ڸ�Ӧ���������κο��������������...");
	if((ret=NDK_RfidFelicaPoll(UID, &pUIDlen))!=NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�������ԣ���������felica��,Ѱ��Ӧ����
	cls_show_msg("���ڸ�Ӧ������Felica���������������...");
	if((ret=NDK_RfidFelicaPoll(UID, &pUIDlen))!=NDK_OK)
	{
		send_result("line %d:%sѰ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�쳣���ԣ��ò���felica��Ѱ��Ӧ��ʧ��
	cls_show_msg("���ڸ�Ӧ�����ñ�׼A���������������...");
	if((ret=NDK_RfidFelicaPoll(UID, &pUIDlen))!=NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:�쳣���ԣ��࿨��ȡһ��A����һ��felica��Ѱ����Ӧ�ܷ�
	cls_show_msg("���ڸ�Ӧ�����ñ�׼A����Felica���������������...");
	if((ret=NDK_RfidFelicaPoll(UID, &pUIDlen))!=NDK_OK)
	{
		send_result("line %d:%sѰ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�µ�
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;

}

