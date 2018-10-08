/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn9.c
* Author 			: chenjings
* version			: 
* DATE			: 20130106
* directory 		: src/pos/asyn
* description		: ����NDK_MdmReset�ܷ������MODEM������Ч��λ
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmReset"
#define	MAXWAITTIME	60	//60��
#define	PCKTMAXLEN	350

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenjings      20130106	created
*****************************************************************/
void asyn9(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};
	EM_MDMSTATUS MdmStatus;	//mdm״̬
	
	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 
	
	//case1 : ��֤��λ��MODEM����δ��ʼ��״̬,���״̬Ӧ��ʧ��
	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_MdmCheck(&MdmStatus))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ��������ش���(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:��֤�������Ӻ�λ,Ӧ�óɹ�,�Ҹ�λ�����Ӧ��ʧ��
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����(%s)ʧ��(ret=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sδ�ܽ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	NDK_MdmHangup();//���첽�����޷��Զ��Ҷϣ���Ҫ����Ҷ� 20140221 linwl
	//��λ
	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MdmCheck(&MdmStatus))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ��������ش���(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��                                     		
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(rbuf);//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��֤����ͨ�Ź����и�λ,Ӧ�óɹ�,�Ҹ�λ�����Ӧ��ʧ��
	sleep(30);//��ˮ ��λ��ʱ���û�еȴ�30���첽MDM��̨û�йҶϣ���������Ż�ʧ�ܣ��������������Ҫ����30�ȴ���ʱ
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����(%s)ʧ��(ret=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sδ�ܽ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	//��ʼ�����ͻ���������
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ		
	//д
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��                                     		
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(rbuf);//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�
	if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		GOTOERR;
	}
	//����
	if(NDK_MdmHangup()!=NDK_OK)
	{
		send_result("line %d:%s�������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_MdmHangup();
	return;
}

