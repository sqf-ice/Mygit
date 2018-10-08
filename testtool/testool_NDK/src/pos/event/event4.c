/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: eventģ��
* file name		: event4.c
* Author 		:  
* version		: 
* DATE			: 20180521
* directory 		: 
* description		: ���Ը�λ��Ƭ(NDK_RfidPiccResetCard(uchar usDelayms)):����0Ϊ�رճ�ǿ������6-10Ϊ�Ϸ�ֵ
* related document	: �߶�ƽ̨ͳһAPI��̬���ӿ����ֲ�V2.00.doc
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_RfidPiccResetCard"
#define MAXWAITTIME	30
#define PINTIMEOUT_MAX	(200*1000)

/*------------global variables definition-----------------------*/
static EM_SYS_EVENT flag = 0;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int notifyEvent(EM_SYS_EVENT eventNum, int msgLen, char * msg)
{
	flag = eventNum;
	NDK_SYS_UnRegisterEvent(eventNum);

	return SUCC;
}
static int notifyEvent_noexit(EM_SYS_EVENT eventNum, int msgLen, char * msg)
{
	flag = eventNum;
	//cls_printf("0x%08x",eventNum);
	return SUCC;
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date		remarks
*			  	  	 	jiangym		20180521 	created
*****************************************************************/
void event4(void)
{
	if(auto_flag==1)
	{
		send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1, i = 0, pnDatalen=0, rcvlen = 0;
	int errpara[] = {0,-1,11,8 };
	uchar psPicctype;
	uchar psRecebuf[300]={0}, psDatabuf[256]={0}, UID[LEN_UID]={0}, psSakbuf[256]={0}, psBlockdata[MAXLEN_DATA]={0};	
	uchar Select_1pay[]= {'\x00', '\x84', '\x00', '\x00', 8};

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//����ǰ��:
	for(i=SYS_EVENT_MAGCARD;i<=SYS_EVENT_PRNTER;i++)
		NDK_SYS_UnRegisterEvent(i);
	NDK_RfidPiccDeactivate(0);//��Ƶ���µ�

	//case1:�����쳣����(����NDK_RfidPiccDeactivate ����)
	// ����Ƿ������Լ�6-10��Χ�ڵĲ������Լ�0�����ϱ������޸�Ϊ��ȷ��ֵ�������ֵ<6ʱ���޸�ֵΪ6�������ֵ>10ʱ���޸�Ϊ10
	if(NDK_RfidInit(NULL)!=NDK_OK)
	{
		cls_show_msg("��Ƶ�ӿ�������ʼ��ʧ��,������˳�...");
		RETURN;
	}
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=0;i<4;i++)
	{	
		if((ret=NDK_RfidPiccResetCard(errpara[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	// �µ�����APDU����Ӧʧ�ܣ�����-7����Ϊ�������µķ���ֵ
	if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=-7)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_RfidPiccDeactivate(0);//��������

	//case2:�ǽӿ�δ�ϵ�ǰ����[��λ��Ƭ]Ӧ���سɹ�(��������0���0)
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccResetCard(100))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�ǽӿ��ϵ���ٽ���[��λ��Ƭ]����ʱ����APDU����Ӧʧ��
	for(i=0;i<2;i++) 
	{
		cls_show_msg("���ڸ�Ӧ������1�ű�׼%s��,���������",i==0?"A":"B");
		if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}	
		if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=(i==0?RFCARD_TYPE_A:RFCARD_TYPE_B))
		{
			send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))==NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_RfidPiccDeactivate(0);//��������
	}
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");
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
	if(((ret=NDK_M1Select(pnDatalen, UID, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_M1ExternalAuthen(pnDatalen, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x01))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Read(0x02, &pnDatalen, psBlockdata))==NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_RfidPiccDeactivate(0);//��������
	
	// case4.1:�¼�����ע��ǰ����[��λ��Ƭ]��������Ӱ���¼���������
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_RFID, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����ڸ�Ӧ������A�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	cls_show_msg("����A�������������");

	// case4.2:�¼�����ע������[��λ��Ƭ]��������Ӱ���¼���������(JNI_Rfid_PiccResetCard�µ��Ҫ�����¼��ſ��Լ������ǽӿ��¼�)
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_RFID, MAXWAITTIME*1000, notifyEvent_noexit))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�����¼�
	if((ret=NDK_SysResumeEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����ڸ�Ӧ������A�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	cls_show_msg("����A�������������");
	
	// case5:�������¼������[��λ��Ƭ]����Ӧ����
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	// case6.1:�����¼�ǰ����[��λ��Ƭ]����Ӧ������Ӧ�������������¼�
	if((ret=NDK_SysResumeEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����ڸ�Ӧ������A�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	cls_show_msg("����A�������������");

	// case6.2:�����¼������[��λ��Ƭ]����Ӧ������Ӧ�������������¼�(�ǽӿ��µ�֮��Ӧ�޷���������Ƶ�¼�����Ӱ�������¼��ļ���)
	if((ret=NDK_SysResumeEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����ڸ�Ӧ������A�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	cls_show_msg("����A�������������");

	// case7:ע�������¼������[��λ��Ƭ]��������Ӱ�������¼�����(����IC���ļ���)
#if IC1_ENABLE
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccResetCard(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����IC�������������");
	if(flag != SYS_EVENT_ICCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		goto ERR;
	}	
#endif
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);//��Ƶ���µ�
	for(i=SYS_EVENT_MAGCARD;i<=SYS_EVENT_PRNTER;i++)
		NDK_SYS_UnRegisterEvent(i);	
	return;	
}

