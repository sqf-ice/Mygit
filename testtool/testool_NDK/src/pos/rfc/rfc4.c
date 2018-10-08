/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc4.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_RfidPiccApdu�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidPiccApdu"

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
void rfc4(void)
{
	//����Ϊ�ֲ�����
	uchar psPicctype;
	int ret= -1, ret1= -1, ret2= -1, ret3= -1, rcvlen=0, pnDatalen=0;
	uchar psRecebuf[300]={0}, psDatabuf[256]={0};
	ushort sw=0;
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	uchar Select_1pay[]= {'\x00', '\x84', '\x00', '\x00', 8};
#else
	uchar Select_1pay[]={0x00, 0xA4, 0x04, 0x00, 0x0E, '1', 'P', 'A', 'Y', '.', 'S', 'Y', 'S', '.', 'D', 'D', 'F', '0', '1', 0x00};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case1:�쳣���ԣ��������     
	if((ret=NDK_RfidPiccApdu(-1, Select_1pay, &rcvlen, psRecebuf))!=NDK_ERR_PARA
	||(ret1=NDK_RfidPiccApdu(sizeof(Select_1pay), NULL, &rcvlen, psRecebuf))!=NDK_ERR_PARA
	||(ret2=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, NULL, psRecebuf))!=NDK_ERR_PARA
	||(ret3=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:"-7 δ�ϵ�(Ѱ������)"
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=-7)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2, "����������ͨ��!");//����������case����ͬ�Ĳ�����ʾ��Ϣ

	//case3:"-1 ��ʾ��дʧ��"
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_A)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	cls_show_msg("Ѱ���ɹ�!�뽫��Ӧ���ı�׼A���ƿ�,���������...");
	if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=-1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��Ȼiccrw_newʧ��,���Ѽ���ɹ�(�ײ㴦�ڼ���̬),����Ѱ������ʾ"�Ѿ�����(�ϵ�)"
	//��Ʊ����!(��EMV��ȫҪ��ִ��),����һ�������޷����Ե�	
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_ERR_RFID_NOCARD)//��Ϊ�ײ�����ΪAPDU��������,���µ�,�ص���ʼ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//(����EMVҪ��),Ŀǰ����������ɵײ����!
	//"�����µ磨����Ѽ���״̬��־����Ȼ����Ѱ�������"
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:A��APDU����
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_A)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=NDK_OK||((sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x9000&&(sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x6d00))
	{
		send_result("line %d:%s����ʧ��(%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
		GOTOERR;
	}
	//�����������Ҫ��B�����ԣ�Ӧ���µ�رճ�
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:B��APDU����
	cls_show_msg("���ڸ�Ӧ������1�ű�׼B��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_B)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=NDK_OK||((sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x9000&&(sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x6d00))
	{
		send_result("line %d:%s����ʧ��(%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
		GOTOERR;
	}
	//�����������Ҫ�������ԣ�Ӧ���µ�رճ�
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

