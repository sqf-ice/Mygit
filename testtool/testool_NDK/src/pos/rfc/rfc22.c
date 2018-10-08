/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc22.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ����NDK_M1Anti_SEL��NDK_M1Select_SEL����ͻ��ѡ���ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_M1Anti_SEL,NDK_M1Select_SEL"

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
*			  		        linying		    20180420		created 
*****************************************************************/
//��ȡ��Ƭuid
static int get_mifare_UID(int *UIDlen,uchar* UID)
{
	uchar rece_buf[30];
	int ret=0,i=0,pnDatalen=0,len=0;
	unsigned char sak;
	unsigned char sel_cmd[3] = {0x93, 0x95, 0x97};
	//Ѱ��
	if((ret=NDK_M1Request(REQA, &pnDatalen, rece_buf))!=NDK_OK)
	{
		cls_show_msg("line %d:M1��Ѱ��ʧ��(%d)", __LINE__, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	len = (rece_buf[0] >> 6) + 1;//0x00 4  0x01 7 0x02 10
	i = 0;
    	do
	{
		//M1������ͻ
		if((ret=NDK_M1Anti_SEL(sel_cmd[i],&pnDatalen,rece_buf))!=NDK_OK)
		{
		    cls_show_msg("line %d:M1������ͻʧ��(%d)", __LINE__,  ret);
		    NDK_RfidPiccDeactivate(0);
		    return FAIL;
		}
		//M1ѡ��
		if(((ret=NDK_M1Select_SEL(sel_cmd[i],pnDatalen,rece_buf,&sak))!=NDK_OK)||((sak&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
		{
		    cls_show_msg("line %d:����ʧ��(%d,0x%2x)", __LINE__,  ret, sak);
		    NDK_RfidPiccDeactivate(0);
		    return FAIL;
		}
		memcpy((UID + (i << 2)), rece_buf, 4);
		if((sak & 0x04) == 0x00)
			break;	//UID transfer complete
	}while(i++ < 3);
	
	if((len == 1) && (*UID == 0x88)){
		cls_show_msg(" Ԥ��Ϊ4�ֽڵ�UID��Ϣ����");
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((len == 2) && (*(UID+4) == 0x88)){
		cls_show_msg(" Ԥ��Ϊ7�ֽڵ�UID��Ϣ����");
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	switch(len){
		case 1:
			*UIDlen = 4;
		break;
		case 2:
			*UIDlen = 7;
			memcpy(UID,&UID[1],3);/*first*/
			memcpy(&(UID[3]),&UID[4],4);
		break;
		case 3:
			*UIDlen = 10;
			memcpy(UID,&UID[1],3);/*first*/
			memcpy(&(UID[3]),&UID[5],3);
			memcpy(&(UID[6]),&UID[8],4);

		default:
			*UIDlen = 0;
			UID = 0x00;
			return NDK_ERR_MI_PROTOCOLERR;
	}	
    return SUCC;
}

void rfc22(void)
{
	//����Ϊ�ֲ�����
	int ret=0, pnDatalen,UIDlen=0;
	uchar UID[LEN_UID]={0},UID1[LEN_UID]={0},psDatabuf[256]={0},psSakbuf[256]={0},out[MAXLEN_DATA]={0};
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	
	NDK_RfidPiccType(RFCARD_TYPE_A);	 
	//case1:�����쳣����
	if((ret=NDK_M1Anti_SEL(0xff,&pnDatalen,UID1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_M1Select_SEL(0xff,pnDatalen,UID1,psSakbuf))!=NDK_ERR_PARA)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d,0x%2x)", __LINE__, TESTAPI, ret, psSakbuf[0]);
		GOTOERR;
	}
	
	//case2:δ����Ѱ���ͽ��з���ͻ��ѡ������Ӧ�÷���ʧ��
	cls_show_msg("���ڸ�Ӧ������1��Mifare-1��,���������...");
	if((ret=NDK_M1Anti_SEL(0x93,&pnDatalen,UID1))!=NDK_ERR_MI_WriteFifo)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_M1Select_SEL(0x93,4,UID1,psSakbuf))!=NDK_ERR_MI_WriteFifo)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��,��uid����ֱ�Ӹ�Ϊ4
	{
		send_result("line %d:%s����ʧ��(%d,%02x)", __LINE__, TESTAPI, ret, psSakbuf[0]);
		GOTOERR;
	}
	
	//case3:Ѱ���ɹ�����з���ͻʱ�ѿ��ƿ�,�����Ƿ���ͻ�ɹ���ѿ��ƿ�
	//cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("�뽫��Ӧ����Mifare-1���ƿ�,���������...");
	if((ret=NDK_M1Anti_SEL(0x93,&pnDatalen,UID))==NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	cls_show_msg("���ڸ�Ӧ������ͬһ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Anti_SEL(0x93,&pnDatalen,UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	cls_show_msg("�뽫��Ӧ����Mifare-1���ƿ�,���������...");
	if((ret=NDK_M1Select_SEL(0x93,4,UID,psSakbuf))==NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:ͬ�ſ���ȡ��uidӦ��һ��,��ͬ����ȡ��uid��ͬ
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");
	if((ret=get_mifare_UID(&UIDlen,UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���ڸ�Ӧ������ͬһ��Mifare-1��,���������...");
	if((ret=get_mifare_UID(&UIDlen,UID1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(UID, UID1, sizeof(UID)))
	{
		send_result("line %d:ͬ�ſ�UID��һ��", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���ڸ�Ӧ������һ��4K Mifare-1��,���������...");
	if((ret=get_mifare_UID(&UIDlen,UID1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(!memcmp(UID, UID1, sizeof(UID)))
	{
		send_result("line %d:��ͬ�ſ�UID��һ��", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:ʹ��4�ֽڵ�M1�������������̲���
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");
	if((ret=get_mifare_UID(&UIDlen,UID1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1ExternalAuthen(UIDlen, UID1, AUTHKEY_TYPE_A, AUTHKEY, 0x01))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	pnDatalen=LEN_BLKDATA; 
	if((ret=NDK_M1Write(0x02, &pnDatalen, BLK02DATA_FF))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Read(0x02, &pnDatalen, out))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	else
	{
		if(memcmp(BLK02DATA_FF, out, LEN_BLKDATA))
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:��ȡ7�ֽڵ�UID���жϳ���,����ʹ��M0��������֤,ֻ���ж�uid����
	cls_show_msg("���ڸ�Ӧ������1��UItralight C��,���������...");
	if((ret=get_mifare_UID(&UIDlen,UID1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(UIDlen!=7)
	{
		send_result("��ȡUID��������%d",UIDlen);
		goto ERR;
	}
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;
}

