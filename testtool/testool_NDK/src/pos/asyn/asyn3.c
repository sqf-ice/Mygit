/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc3.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/asyn
* description		: ����NDK_MdmCheck �ܷ���ȷ�����modem״̬
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmCheck"
#define	MAXWAITTIME	60	//60��

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
void asyn3(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, j = 0;
	EM_MDMSTATUS MdmStatus;	//mdm״̬
	char buf[512] = {0}, rbuf[512] = {0};
	uint rlen = 0;

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
	//case2:����modemΪ-11 MDMSTATUS_NOPREDIAL 
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NOPREDIAL))//δ����ȷ��ȡmodem״̬
	{
		send_result("line %d:��⵽�쳣״̬(ret=%d,status=%d)", __LINE__, ret, MdmStatus);
		RETURN;
	}
	
	//case3:����modemΪ-3 MDMSTATUS_MS_NOCARRIER����״̬20140119�޸ĺ��޷�����
	//����
	if((ret=NDK_MdmDial("123456"))!=NDK_OK)//case2.1: �жϲ����Ƿ�ɹ�
	{
		send_result("line %d:%s����ʧ��(ret%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ж�״̬
#if 1//NDK_MdmCheck�������첽��ʹ��ʱ�� ������Ϊ����������Ӧ�޸�Ӧ��ʹ�÷�ʽ linwl 20140618 
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
			if((MdmStatus==MDMSTATUS_MS_BUSY/*MDMSTATUS_MS_NOCARRIER*/))//20140119 ��ˮ���������޸��˳�ʼ����Ĺ������������������״̬���
			{// �ж�modem״̬�Ƿ�Ϊæ��
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
#else
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_MS_BUSY/*MDMSTATUS_MS_NOCARRIER*/))//20140119 ��ˮ���������޸��˳�ʼ����Ĺ������������������״̬���
	{
		send_result("line %d:��⵽�쳣״̬(ret=%d,status=%d)", __LINE__, ret, MdmStatus);
		NDK_MdmHangup();  
		RETURN;
	}
	NDK_MdmHangup();
#endif	
	//case4:����modemΪ2 MDMSTATUS_CONNECT_AFTERPREDIAL
	//����
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(DialNum=%s,ret=%d)", __LINE__, TESTAPI, MDMDialStr ,ret);
		GOTOERR;
	}
	//case5:��һ�ε���Ӧ�����Ϸ���δ���ϵ�״̬���ҷ����ٶȱȽϿ�(0.3��֮��)����˵�������Ƿ�����ʽ�� linwl added 20140618
	lib_StartStopwatch();
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NORETURN_AFTERPREDIAL)||(lib_ReadStopwatch()>0.3))//0.3����
	{
		send_result("line %d:��⵽�쳣״̬(ret=%d,status=%d)", __LINE__, ret, MdmStatus);
		GOTOERR;
	}
	//�ж�modem״̬
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sδ�ܽ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	
	//case5:������д���ж�״̬�Ƿ�����
	//��ʼ��������
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;

#if 0		
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif

	//д����
	if((ret=NDK_MdmWrite(buf,sizeof(buf)))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;
	}
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)+1;//������ȡ�������ݳ���Ҫ���ڵ���ʵ���ܶ�ȡ�������ݳ��ȡ�
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�����
	if((sizeof(buf)!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf), rlen);
		GOTOERR;
	}
	//�ж�״̬��ȻΪ����״̬
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_CONNECT_AFTERPREDIAL ))
	{
		send_result("line %d:%sè״̬�쳣(ret=%d,status=%d)", __LINE__, TESTAPI, ret, MdmStatus);
		GOTOERR;
	}
		
	//case6:�һ���ȡ״̬ΪMDMSTATUS_NOPREDIAL (-11)
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NOPREDIAL))
	{
		send_result("line %d:%sè�쳣״̬(ret=%d,status=%d)", __LINE__, TESTAPI, ret, MdmStatus);
		GOTOERR;
	}
	
	//case7: �������NULL
	if((ret=NDK_MdmCheck(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//MDMSTATUS_NORETURN_AFTERPREDIAL
	//MDMSTATUS_OK_AFTERPREDIAL 
	//MDMSTATUS_MS_NODIALTONE  
	//MDMSTATUS_MS_ERROR  
	//MDMSTATUS_MS_BUSY
	//MDMSTATUS_LINE_VOL_TOO_LOW
	
	//���Խ���
	NDK_MdmHangup();
	send_result("%s����ͨ��", TESTAPI);
	return;
	goto ERR;
ERR:
	NDK_MdmHangup();
	return;
}

