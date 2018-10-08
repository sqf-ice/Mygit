/************************************************************************
*
* copyright(c)2012-2015 Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc17.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description			: ����NDK_Iso15693SelectPicc,NDK_Iso15693GetPicc_SystemInfo�ܷ��������з���ͻ֮���ѡ��������������л�ȡ��Ƭ�����Ϣ(Iso15693Э��)
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_Iso15693SelectPicc,NDK_Iso15693GetPicc_SystemInfo"

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
void rfc17(void)
{
	int ret = -1, i = 0, j = 0;
	uint cardcount = 0, maxcards = 20;
	iso15693ProximityCard_t cards[20];		//�������֧��20��ISO1569���Ķ�д
	iso15693PiccSystemInformation_t sysinfo;

	//��ʼ��
	if((ret=NDK_Iso15693_init()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case1:δ���з���ͻӦ�÷���ʧ��
	if((ret=NDK_Iso15693SelectPicc(cards[0])) != NDK_ERR_RFID_SEEKING)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	cls_show_msg("����һ��15693��Ƭ,���������");
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//for(i = 0; i < 20; i++)
	{
		for(j = 0; j < ISO15693_UID_LENGTH; j++)
		{
			send_result("%02x ",cards[0].uid[j]);
		}
	}
	//case2:����ͻ�ɹ���δ���ÿ�ƬӦ�÷����޿�
	cls_show_msg("��15693��Ƭ�ƿ�");//??
	if((ret=NDK_Iso15693SelectPicc(cards[0])) != NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case3:��������:���ÿ�Ƭ,Ѱ����������ݼ���ɹ����ȡ��Ƭ����
	cls_show_msg("����һ��15693��Ƭ,���������");
	if((ret=NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_Iso15693SelectPicc(cards[0])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case4:�����쳣
	if((ret=NDK_Iso15693GetPicc_SystemInfo(cards[0],NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case5:������ȡ��Ƭ����
	if((ret=NDK_Iso15693GetPicc_SystemInfo(cards[0],&sysinfo)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//�����Ƭ����
	send_result("flag:%d,uid:%02x",sysinfo.flags,sysinfo.uid);

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	
ERR:
	NDK_Iso15693_Deinit();
	return;
}

