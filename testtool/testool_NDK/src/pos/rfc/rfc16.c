/************************************************************************
*
* copyright(c)2012-2015 Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc16.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description			: ����NDK_Iso15693_Inventory�ܷ�����Ѱ������ͻ(Iso15693Э��)
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_Iso15693_Inventory"

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
*			  		    	 jiangym			20180127		created
*****************************************************************/
void rfc16(void)
{
	int ret = -1, i = 0, j = 0;
	uint cardcount = 0, maxcards = 20;
	iso15693ProximityCard_t cards[20];		//�������֧��20��ISO1569���Ķ�д
	
	//case1:δ��ʼ������ʧ��
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_ERR_RFID_INITSTA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��ʼ��
	if((ret=NDK_Iso15693_init()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�����쳣,����ʧ��
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_1-1,maxcards,&cardcount,cards)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_Iso15693_Inventory( ISO15693_NUM_SLOTS_16+1,maxcards,&cardcount,cards)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,NULL,cards)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:δ���ÿ�ƬӦ�÷����޿�
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_ERR_RFID_NOCARD)//NDK_ERR_MI_TRANSERR
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:��������:���ÿ�Ƭ,Ѱ�����������
	cls_show_msg("����һ��15693��Ƭ,���������");
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
/*	for(i = 0; i < 20; i++)
	{
		for(j = 0; j < ISO15693_UID_LENGTH; j++)
		{
			send_result("%02x ",cards[i].uid[j]);
		}
	}*/
	//case5:���ö��ſ�Ƭ,Ӧ�ܼ������ſ�Ƭ
	cls_show_msg("���ö���15693��Ƭ,���������");
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_ERR_RFID_MULTICARD)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_Iso15693_Deinit();
	return;
}


