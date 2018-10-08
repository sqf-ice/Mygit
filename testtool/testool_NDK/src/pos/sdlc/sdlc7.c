/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc7.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmClrbuf �ܷ����ͬ��ͨѶ������
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmClrbuf"
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
void sdlc7(void)
{
	//����Ϊ�ֲ�����
	int ret = 0;
	int j = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};// 1K���ĵ��Ƽ�������,2K���������
	uint GetReadLen = -1;
	
	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�����
	
	//����ǰ��:
	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf));//���TPDU��ͷ
#if 0 //20141202 reset���������޸�ֱ�ӷ���NDK_OK�����α����� 
	//case1:δ��ʼ��,Ӧ�÷���NDK_ERR_MODEM_INIT_NOT
	if((ret=NDK_MdmClrbuf())!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ������ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
#endif
	//case2:��ʼ��Ӧ���سɹ�NDK_OK
	if((ret=NDK_MdmSdlcInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_MdmClrbuf())!=NDK_OK)
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=0))//��󻺳�������Ϊ0
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}
	
	//case3:����֮������建������ЧӦ�÷���NDK_OK�����Ҳ�Ӱ����������ͨ��
	if((ret=LinkUp())!=NDK_OK)
	{
		send_result("line %d:%s������·ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(buf, 0, sizeof(buf));
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;
	}
	sleep(5);//��ʱ
	//4.4.6NDK�汾֮��ֻҪ��ʼ���󣬶����Խ��ж�Ӧ��ͬ�����첽���ջ������  linwl20140304
	if((ret=NDK_MdmClrbuf())!=/*NDK_ERR_MODEM_SDLCCLRBUFFAIL*/NDK_OK)//ͬ��MODEM�建�����ڲ���֮����Ч���᷵��ʧ�� linwl20130710
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	sleep(5);
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0	
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_TIMEOUT/*NDK_OK*/)//����֮���建������Ч�����Զ�����Ӧ�ó�ʱ
	{
		send_result("line %d:%s������ʧ��(ret=%d,rlen=%d)", __LINE__, TESTAPI, ret, rlen);
		GOTOERR;
	}
	if((ret=NDK_MdmClrbuf())!=/*NDK_ERR_MODEM_SDLCCLRBUFFAIL*/NDK_OK)//ͬ��MODEM�建�����ڲ���֮����Ч���᷵��ʧ�� linwl20130710
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:���������շ�ǰ����ջ���������Ӱ�����������շ�
	if((ret=NDK_MdmClrbuf())!=NDK_OK)
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;
	}
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0	
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)//����֮���建������Ч�����Զ�����Ӧ�óɹ�
	{
		send_result("line %d:%s������ʧ��(ret=%d,rlen=%d)", __LINE__, TESTAPI, ret, rlen);
		GOTOERR;
	}
	if((ret=NDK_MdmClrbuf())!=NDK_OK)
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}		
	//�Ƚ϶�ȡ����
	if((PCKTMAXLEN!=rlen)||(MemCmp(buf, rbuf, rlen)))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		goto ERR;
	}
	
	//���Խ���
	LinkDown();
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	LinkDown();
	return;
}

