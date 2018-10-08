/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc3.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmCheck �ܷ���ȷ�����modem״̬
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmCheck"
#define	MAXWAITTIME	60	//60��
#define PCKTMAXLEN 350

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
*			  	  	chenjings      20130108	created
*****************************************************************/
void sdlc3(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, j = 0;
	EM_MDMSTATUS MdmStatus;	//mdm״̬
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};

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
	//case1 : δ��ʼ��ʧ�����
	if((ret=NDK_MdmCheck(&MdmStatus))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ��������ش���(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	//case2:����modemΪ-11 MDMSTATUS_NOPREDIAL (��δ����,��SDLC��������ֹ,�ڴ��ٴβ���)״̬�����
	if((ret=NDK_MdmSdlcInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	sleep(5);
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NOPREDIAL))//δ����ȷ��ȡmodem״̬
	{
		send_result("line %d:%s��⵽�쳣״̬(ret=%d,status=%d)", __LINE__, TESTAPI, ret, MdmStatus);
		RETURN;
	}
	
	//case3:����modemΪ-4 MDMSTATUS_MS_BUSY
	//����
	if((ret=NDK_MdmDial("123456"))!=NDK_OK)//case2.1: �жϲ����Ƿ�ɹ�
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ж�״̬
	lib_StartStopwatch();
	while(1)
	{									
		if((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)//δ����ȷ��ȡmodem״̬
		{
			send_result("line %d:%s���״̬ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		else if(MdmStatus!=MDMSTATUS_NORETURN_AFTERPREDIAL)// �ж�modem״̬�Ƿ�ı�
		{
			if((MdmStatus==MDMSTATUS_MS_BUSY )){// �ж�modem״̬�Ƿ�Ϊæ��
				NDK_MdmHangup();   
				break;
			}
			else {
				send_result("line %d:%sè״̬�쳣(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, MdmStatus);
				goto ERR;
			}
		}
		else if(lib_ReadStopwatch()>MAXWAITTIME)//60 s ��ʱδ��⵽��ͨ
		{
			send_result("line %d:%sδ�ܲ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, MdmStatus);
			goto ERR;
		}									
	}		

	//case4:����modemΪ2 MDMSTATUS_CONNECT_AFTERPREDIAL
	//����
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(DialNum=%s,ret=%d)", __LINE__, TESTAPI, MDMDialStr ,ret);
		GOTOERR;
	}					
	//�ж�modem״̬
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sδ�ܽ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}

	//��д�����ж�״̬�Ƿ�����
	//��ʼ��������
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;	
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ
	//д
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK)
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
	//�ж�״̬
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_CONNECT_AFTERPREDIAL ))
	{
		send_result("line %d:%sè״̬�쳣(ret=%d,status=%d)", __LINE__, TESTAPI, ret, MdmStatus);
		GOTOERR;
	}
		
	//case5:�һ���ȡ״̬ΪMDMSTATUS_NOPREDIAL (-11)
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(5);
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NOPREDIAL))
	{
		send_result("line %d:%sè�쳣״̬(ret=%d,status=%d)", __LINE__, TESTAPI, ret, MdmStatus);
		GOTOERR;
	}
	
	//case6: �������NULL
	if((ret=NDK_MdmCheck(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//MDMSTATUS_OK_AFTERPREDIAL 
	//MDMSTATUS_MS_NODIALTONE  
	//MDMSTATUS_MS_ERROR  
	//MDMSTATUS_MS_NOCARRIER
	//MDMSTATUS_LINE_VOL_TOO_LOW
	
	//���Խ���
	NDK_MdmHangup();
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_MdmHangup();
	return;


}

