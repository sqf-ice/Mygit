/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc12.c
* Author 			:  jiangym
* version			: 
* DATE				:  20140811
* directory 			: 
* description			: ����NDK_RfidTypeARats�ӿ��ܷ���M1��Ѱ������ͻ��ѡ���Ļ����ϼ���A�����ж�д����
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidTypeARats"

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
*			  		       jiangym	        20140811	 	created
*****************************************************************/
void rfc12(void)
{
	//����Ϊ�ֲ�����
	int ret=0, pnDatalen=0, rcvlen=0;
	uchar psRecebuf[256]={0}, psDatabuf[256]={0}, UID[LEN_UID]={0}, psSakbuf[10]={0};
	ushort sw=0;
	uchar Select_1pay[]= {'\x00', '\x84', '\x00', '\x00', 8};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);

	//case1:����Ѱ������ͻѡ��ǰ�ĵ���,Ӧ�÷���ʧ��
	if((ret=NDK_RfidTypeARats(0,&pnDatalen, psDatabuf))!=NDK_ERR_MI_WriteFifo)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//����M1������
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���ڸ�Ӧ������һ�ű�׼A��,���������...");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_M1Anti(&pnDatalen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Select(pnDatalen, UID, psSakbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����pssakbuf ֵ�ж���A������M������A��ִ�л�ȡA����ATS
	if((psSakbuf[0]&0x20)!=0x20)//bit5Ϊ1ʱ���ж�ΪA��,Ȼ��ִ��A������ apdu
	{
		send_result("line %d:%s����ʧ��(0x%02x)", __LINE__, TESTAPI, psSakbuf[0]);
		GOTOERR;
	}
	
	//case2:����ΪNULL
	if((ret=NDK_RfidTypeARats(0,NULL, psDatabuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidTypeARats(0,&pnDatalen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3:��������
	if((ret=NDK_RfidTypeARats(0,&pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=NDK_OK||((sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x9000&&(sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x6d00))
	{
		send_result("line %d:%s����ʧ��(%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;
}
