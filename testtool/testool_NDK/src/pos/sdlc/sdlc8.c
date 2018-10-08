/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc8.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmGetreadlen �ܷ��ȡmodem����
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmGetreadlen"
#define MAXWAITTIME 60
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
void sdlc8(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};
	uint GetReadLen = -1;

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
	if((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%s��ȡmodemδ��ʼ������ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	//case2:NDK_MdmGetreadlen���صĳ���ֵΪNULLʱ�Ƿ񷵻ش��� 
	if((ret=NDK_MdmGetreadlen(NULL))!= NDK_ERR_PARA)
	{
		send_result("line %d:%s��ȡmodem���ȷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=LinkUp())!=NDK_OK)
	{
		send_result("line %d:%s������·ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ
	
#if 1
	//case3:�ղ��ųɹ�modem����������Ӧ��û������
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=0))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}
	
	//case4:����д���ݺ��ж�modem�����Ƿ���ȷ��Ӧ�ø�д���һ��
	//д����
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(10);
	GetReadLen=-1;
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=PCKTMAXLEN))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}
	
	//case5:��ȡ���ݺ󻺳���Ӧ��Ҫ�����GetReadLenӦ��Ϊ0
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{		
		send_result("line %d:%s������ʧ��(ret=%d,len=%d)", __LINE__, TESTAPI, ret, rlen);
		GOTOERR;
	}
	//�Ƚ϶�ȡ���ݳ���
	if(PCKTMAXLEN!=rlen)
	{		
		send_result("line %d:%s�շ����Ȳ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		GOTOERR;
	}
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=0))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}
#endif

#if 1
	//case6:ͬ��MODEM�建�����ڲ���֮����Ч������NDK_MdmGetreadlen����0
	//д����
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_OK)
	{				
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��ʱ
	sleep(10);
	if((ret=NDK_MdmClrbuf())!=/*NDK_ERR_MODEM_SDLCCLRBUFFAIL*/NDK_OK)//ͬ��MODEM�建�����ڲ���֮����Ч���᷵��ʧ�� l20130710->20140304 �°汾�е������建����Ч
	{		
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(10);
	GetReadLen=-1;
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=/*350*/0))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		goto ERR;
	}
#endif

	//���Խ���
	LinkDown();
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	LinkDown();
	return;
}

