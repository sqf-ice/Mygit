/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc6.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmHangup �ܷ�Ͽ�MODEM��ͬ��ͨѶ��·����SDLC�һ�����
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmHangup"
#define	MAXWAITTIME 60
#define PCKTMAXLEN 350

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
//�Զ���Ĳ������Ӻ���������linkupʹ��linkdown ���ر�ͨѶ�����ʹ��NDK_MdmHangup�����
static int SdlcModemLink(void)
{
	int ret =0 ;
	
	if((ret=NDK_MdmSdlcInit(MDMPatchType))!=SUCC)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����(%s)ʧ��(ret=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		goto END;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sδ�ܽ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		goto END;
	}
	return SUCC;
END:
	NDK_MdmHangup();
	return FAIL;
}
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenjings      20130108	created
*****************************************************************/
void sdlc6(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};// 1K���ĵ��Ƽ�������,2K���������
	EM_MDMSTATUS MdmStatus;	//mdm״̬

	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�����
	
	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
#if 0 //20141202 reset���������޸�ֱ�ӷ���NDK_OK�����α����� 
	//case1,δ��ʼ�����
	if((ret=NDK_MdmHangup())!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ���������ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	//case2:������д��Ҷ�
	//����ǰ��
	if(SdlcModemLink()!=SUCC)
	{
		send_result("line %d:%s������·ʧ��", __LINE__,  TESTAPI);
		RETURN;
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
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
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
		RETURN;
	}

	//case3:�Ҷϳɹ����ж�MDM״̬Ӧ���ǹҶ�״̬,����δ����״̬
	sleep(5);
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NOPREDIAL))
	{
		send_result("line %d:%sè�쳣״̬(ret=%d,status=%d)", __LINE__, TESTAPI, ret, MdmStatus);
		goto ERR;
	}
	
	//���Խ���
	NDK_MdmHangup();
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_MdmHangup();
	return;
}

