/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc5.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmRead �ܷ����ͬ��ͨѶ�Ľ������ݲ�������SDLC������
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmRead"
#define MAXWAITTIME 60
#define	WUCHASEC	0.03 //���ֵ����
#define PCKTMAXLEN	350

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
void sdlc5(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};// 1K���ĵ��Ƽ�������,2K���������
	float tmp = 0.0;
	
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
	//case1:δ��ʼ�����
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);		
		RETURN;
	}
#endif
	//��������	
	if((ret=LinkUp())!=NDK_OK)
	{
		send_result("line %d:%s������·ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��ʼ�����ͻ���������
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ
	//case2:��ȷд����
	//case2.1:���ȴ�ʱ��Ϊ60��
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_OK)
	{		
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);
	//��
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ϶�ȡ����
	if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__,TESTAPI,  PCKTMAXLEN, rlen);
		GOTOERR;
	}
	//case2.2:���ȴ�ʱ��Ϊ0	
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_OK)
	{		
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__,TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);
	//��
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, 0))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ϶�ȡ����
	if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		GOTOERR;
	}
	
	//case3:����������
	if((ret=NDK_MdmRead(NULL, &rlen, MAXWAITTIME))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MdmRead(rbuf, NULL, MAXWAITTIME))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MdmRead(rbuf, &rlen, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case4: �ȴ���ʱ
	// ��ʱ��������û������,Ӧ���ض���ʱ
	lib_StartStopwatch();
	if(((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_TIMEOUT)||((tmp=(lib_ReadStopwatch()-MAXWAITTIME))>WUCHASEC))
	{
		send_result("line %d:%s�����ݲ��Գ�ʱʧ��(ret=%d,tm=%f)", __LINE__, TESTAPI, ret, tmp);
		GOTOERR;
	}
	
	//case5:��ȡ�ĵȴ�ʱ��Ϊ0ʱ��û������Ӧ����������
	if((ret=NDK_MdmRead(rbuf, &rlen, 0))!=NDK_ERR_TIMEOUT)
	{
		send_result("line %d:%s�����ݲ��Գ�ʱʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:���� ͬ����·�Ѷ�ʱ�Ƿ񷵻�δ���š�
	if((ret=NDK_MdmHangup())!=NDK_OK )
	{
		send_result("line %d:%s�һ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_MODEM_NOPREDIAL)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	LinkDown();
	send_result("%s����ͨ��", TESTAPI);//�򻹿����������Ϣ,�磺send_result("%s����ͨ��(��ʱ%f)", TESTAPI, fTimeElapsed);
	return;
ERR:
	LinkDown();
	return;
}

