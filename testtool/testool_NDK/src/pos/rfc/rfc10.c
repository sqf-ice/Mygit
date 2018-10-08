/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc10.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ���� NDK_M1ExternalAuthen�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_M1ExternalAuthen"

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
void rfc10(void)
{
	/*private & local definition*/
	uchar psDatabuf[MAXLEN_DATA]={0}, UID[LEN_UID]={0}, psSakbuf[256]={0}, psBlockdata[MAXLEN_DATA]={0};
	int ret=0, ret1= -1, ret2= -1, ret3= -1, pnDatalen=0, UIDlen = 0;
	uchar inc[]={0x01, 0x00, 0x00, 0x00};//"\x01\x00\x00\x00";//uint inc = 0x01;

	/*process body*/
	cls_printf("����%s...", TESTAPI);
	NDK_RfidPiccDeactivate(0);//����

	//case1:iccrw,�����������rf5�в�����,�������������̵���������
	//��ʹ��CMD_Auth_Ex�������Ĵ�,װ,��֤��������
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");
	if((ret=NDK_M1Request(REQA, &UIDlen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Anti(&UIDlen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_M1Select(UIDlen, UID, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d,0x%2x)", __LINE__, TESTAPI, ret, psSakbuf[0]);
		GOTOERR;
	}

#if 1	//�����������ⲿ��֤,�ڲ���֤�ķ�ʽ����rf9�в�����
	if((ret=NDK_M1ExternalAuthen(-1, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x01))!=NDK_ERR_PARA
	||(ret1=NDK_M1ExternalAuthen(UIDlen, NULL, AUTHKEY_TYPE_B, AUTHKEY, 0x01))!=NDK_ERR_PARA
	||(ret2=NDK_M1ExternalAuthen(UIDlen, UID, 0xff, AUTHKEY, 0x01))!=NDK_ERR_PARA
	||(ret3=NDK_M1ExternalAuthen(UIDlen, UID, AUTHKEY_TYPE_B, NULL, 0x01))!=NDK_ERR_PARA)
	{
		send_result("line %d:����ʧ��(%d,%d,%d,%d)", __LINE__, ret, ret1, ret2, ret3);
		GOTOERR;
	}
	if((ret=NDK_M1ExternalAuthen(UIDlen, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x01))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	if((ret=NDK_M1KeyStore(AUTHKEY_TYPE_B, 0x00, AUTHKEY))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1KeyLoad(AUTHKEY_TYPE_B, 0x00))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1InternalAuthen(pnDatalen, UID, AUTHKEY_TYPE_B, 0x01))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
#endif
	pnDatalen=LEN_BLKDATA;//strlen(BLK02DATA_ORI);
	if((ret=NDK_M1Write(0x02, &pnDatalen, BLK02DATA_ORI))!=NDK_OK)//01����֤����,����������������Ҳ��֤��
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Increment(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Transfer(0x02))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Increment(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Transfer(0x02))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_M1Decrement(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Transfer(0x02))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//�����������õ�ʵ��Ч��Ϊ+1

#if 0	//ĿǰNDK���:inc/dec��,��ҪBLKTransferһ�²���Ч,BLKRestore������,����
	//ԭGPƽ̨���:CMD_BLKTransfer��CMD_BLKRestoreʵ��������,��ֱ�ӷ���00
	if((ret=NDK_M1Transfer(0x02))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Restore(0x02))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	if((ret=NDK_M1Read(0x02, &pnDatalen, psBlockdata))!=NDK_OK||pnDatalen!=LEN_BLKDATA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	else
	{
		if(memcmp(BLK02DATA_INC1, psBlockdata, LEN_BLKDATA))
		{
			send_result("line %d:���дʧ��", __LINE__);
			GOTOERR;
		}
	}
	
#if 0
	if((ret=CMD_Halt())!=MI_OK)
	{
		send_result("line %d:CMD_Haltʧ��(ret=%d)", __LINE__, ret);
		GOTOERR;
	}
	if((ret=CMD_Request(WUPA, out))!=MI_OK)
	{
		send_result("line %d:Ѱ��ʧ��(ret=%d)", __LINE__, ret);
		GOTOERR;
	}
#endif

	//case3:������֤����20130225add by zhangnw: ��֤һ��,��дһ��(��дĳ��ǰ����֤���������û���)
	if((ret=NDK_M1ExternalAuthen(UIDlen, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x04))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Read(0x04, &pnDatalen, psBlockdata))!=NDK_OK||pnDatalen!=LEN_BLKDATA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_M1ExternalAuthen(UIDlen, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x08))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Read(0x08, &pnDatalen, psBlockdata))!=NDK_OK||pnDatalen!=LEN_BLKDATA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:ʹ�ô����key������֤20130528 add by zhengdc
	if((ret=NDK_M1ExternalAuthen(UIDlen, UID, AUTHKEY_TYPE_B, (uchar *)"\x11\x11\x11\x11\x11\x11", 0x08))==NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_RfidPiccDeactivate(10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//msdelay(10);

	//case2Ϊԭgen31�����Բ����Ƶ�����//zhangnw20110829
	//case2:ʹ��4k M1�����Ա߽�
	cls_show_msg("���ڸ�Ӧ������1��4k M1��,���������...");
	//Ѱ��
	if((ret=NDK_M1Request(REQA, &UIDlen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Anti(&UIDlen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_M1Select(UIDlen, UID, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}		
	//��֤/����,�˴�ʹ���ⲿ��֤(�ڲ���֤����,����Կ��Ĵ洢��������Ӱ��)
	//���ڲ�֪��4k����B��Կ�Ƕ���,ʹ��A��Կ(������A��ԿΪȫ0xff)
	if((ret=NDK_M1ExternalAuthen(UIDlen, UID, AUTHKEY_TYPE_A, AUTHKEY, 0xfe))!=NDK_OK)//��֤�Ŀ��ڵ�4K�ڵ���
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//��д,���ǵ����Կ�����,����������
	if((ret=NDK_M1Read(0xfd, &pnDatalen, psBlockdata))!=NDK_OK)//0xfe���������Ŀ�Ӧ���ܶ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�رճ�
	if((ret=NDK_RfidPiccDeactivate(10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);//�����Թر�
	return;
}

