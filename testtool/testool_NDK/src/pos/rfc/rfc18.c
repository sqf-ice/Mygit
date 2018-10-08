/************************************************************************
*
* copyright(c)2012-2015 Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc18.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description			: ����NDK_Iso15693Write_SingleBlock,NDK_Iso15693Read_SingleBlock,NDK_Iso15693Read_MultipleBlocks�ܷ���ȷ���ж�д����(Iso15693Э��)
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_Iso15693Write_SingleBlock,NDK_Iso15693Read_SingleBlock,NDK_Iso15693Read_MultipleBlocks"

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
*			  		   	 jiangym			20180127		created
*****************************************************************/
void rfc18(void)
{
	int ret = -1, i = 0;
	uint cardcount = 0, maxcards = 20;
	uchar cmd = 0x00;
	uchar ucBlocknum = 0;
	uchar startblock = 0;
	uchar count = 0;
	iso15693ProximityCard_t cards[20];		//�������֧��20��ISO1569���Ķ�д
	iso15693PiccMemoryBlock_t w_memBlock, w2_memBlock, r_memBlock;
	iso15693PiccSystemInformation_t sysInfo;
	iso15693PiccMemoryBlock_t memBlock[64];
	
	//��ʼ��
	if((ret=NDK_Iso15693_init()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	
	//case1:�����쳣,����ʧ��
	if((ret=NDK_Iso15693Read_SingleBlock(cards[0],ucBlocknum,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case2:����ͻ�����δ����д�����ֱ�Ӷ�??
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
	ucBlocknum = 0x01;
	memset(&r_memBlock, 0, sizeof(iso15693PiccMemoryBlock_t));
	if((ret=NDK_Iso15693Read_SingleBlock(cards[0],ucBlocknum,&r_memBlock)) != NDK_OK)//����ʲô??
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���������??
	count = 5;
	if((ret=NDK_Iso15693Read_MultipleBlocks(cards[0],startblock,count,memBlock)) != NDK_OK)//����ʲô??
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case3:����д�����
	ucBlocknum = 0x01;
	w_memBlock.data[0] = 0x02;
	w_memBlock.data[1] = 0x08;
	w_memBlock.data[2] = 0x04;
	w_memBlock.data[3] = 0x09;	
	if((ret=NDK_Iso15693Write_SingleBlock(cards[0],w_memBlock,ucBlocknum)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case4:д�����������
	ucBlocknum = 0x01;
	memset(&r_memBlock, 0, sizeof(iso15693PiccMemoryBlock_t));
	if((ret=NDK_Iso15693Read_SingleBlock(cards[0],ucBlocknum,&r_memBlock)) != NDK_OK)//����ʲô??
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//������бȽ������Ƿ�һ��??
	if((r_memBlock.data[0]!=w_memBlock.data[0])||(r_memBlock.data[1]!=w_memBlock.data[1])||(r_memBlock.data[2]!=w_memBlock.data[2])||(r_memBlock.data[3]!=w_memBlock.data[3]))
	{
		send_result("line %d:%s����ʧ��(%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, r_memBlock.data[0], r_memBlock.data[1], r_memBlock.data[2], r_memBlock.data[3]);
		GOTOERR;
	}	
	
	//case5:��ͬһ��д2�ζ��������ǵڶ��ε�?
	ucBlocknum = 0x01;
	w_memBlock.data[0] = 0x02;
	w_memBlock.data[1] = 0x08;
	w_memBlock.data[2] = 0x04;
	w_memBlock.data[3] = 0x09;	
	if((ret=NDK_Iso15693Write_SingleBlock(cards[0],w_memBlock,ucBlocknum)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	w2_memBlock.data[0] = 0x01;
	w2_memBlock.data[1] = 0x02;
	w2_memBlock.data[2] = 0x03;
	w2_memBlock.data[3] = 0x04;
	if((ret=NDK_Iso15693Write_SingleBlock(cards[0],w2_memBlock,ucBlocknum)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	ucBlocknum = 0x01;
	memset(&r_memBlock, 0, sizeof(iso15693PiccMemoryBlock_t));
	if((ret=NDK_Iso15693Read_SingleBlock(cards[0],ucBlocknum,&r_memBlock)) != NDK_OK)//����ʲô??
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((r_memBlock.data[0]!=w2_memBlock.data[0])||(r_memBlock.data[1]!=w2_memBlock.data[1])||(r_memBlock.data[2]!=w2_memBlock.data[2])||(r_memBlock.data[3]!=w2_memBlock.data[3]))
	{
		send_result("line %d:%s����ʧ��(%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, r_memBlock.data[0], r_memBlock.data[1], r_memBlock.data[2], r_memBlock.data[3]);
		GOTOERR;
	}	
	
	//case6:�ٶ�1������һ��?
	ucBlocknum = 0x01;
	memset(&r_memBlock, 0, sizeof(iso15693PiccMemoryBlock_t));
	if((ret=NDK_Iso15693Read_SingleBlock(cards[0],ucBlocknum,&r_memBlock)) != NDK_OK)//����ʲô??
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((r_memBlock.data[0]!=w2_memBlock.data[0])||(r_memBlock.data[1]!=w2_memBlock.data[1])||(r_memBlock.data[2]!=w2_memBlock.data[2])||(r_memBlock.data[3]!=w2_memBlock.data[3]))
	{
		send_result("line %d:%s����ʧ��(%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, r_memBlock.data[0], r_memBlock.data[1], r_memBlock.data[2], r_memBlock.data[3]);
		GOTOERR;
	}	

	//case7:����д5������,Ȼ���5������,�ٽ��бȽ�??
	for(i=1;i<6;i++)
	{
		ucBlocknum = i;
		w_memBlock.data[0] = 0x02;
		w_memBlock.data[1] = 0x04;
		w_memBlock.data[2] = 0x06;
		w_memBlock.data[3] = 0x08;	
		if((ret=NDK_Iso15693Write_SingleBlock(cards[0],w_memBlock,ucBlocknum)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}	
	}
	count = 5;
	if((ret=NDK_Iso15693Read_MultipleBlocks(cards[0],startblock,count,memBlock)) != NDK_OK)//����ʲô??
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

