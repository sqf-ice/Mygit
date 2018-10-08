/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn8.c
* Author 			: chenjings
* version			: 
* DATE			: 20130114
* directory 		: src/pos/asyn
* description		: ����NDK_MdmGetreadlen �ܷ�����첽ͨѶʱ��ȡ���������е����ݳ���
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmGetreadlen"
#define MAXWAITTIME 60

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
*			  	  	chenjings      20130114	created
*****************************************************************/
void asyn8(void)
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
	//case1 : δ��ʼ�����
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

	//����
	if((ret=LinkUp())!=NDK_OK)
	{
		send_result("line %d:%s������·ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��ʼ��buf
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
		
#if 0
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif	
	
	//case3:�������Ӻ��д֮ǰ��������������Ӧ��Ϊ0
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=0))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}
	
  //д����
	if((ret=NDK_MdmWrite(buf, sizeof(buf)))!=NDK_OK)
	{		
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��ʱ
	sleep(10);
	
	//case4:����д���ݺ��ж�modem�����Ƿ���ȷ
	GetReadLen=-1;
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=sizeof(buf)))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}
	
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)+1;
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{		
		send_result("line %d:%s������ʧ��(ret=%d,len=%d)", __LINE__, TESTAPI, ret, rlen);
		GOTOERR;
	}
	//�Ƚ����ݳ���
	if(sizeof(buf)!=rlen)
	{
		send_result("line %d:%s�շ����Ȳ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf), rlen);
		GOTOERR;
	}
	
	//case5:���������ݺ��ж�modem����Ӧ��Ϊ0
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=0))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
		GOTOERR;
	}

	//д����
	if((ret=NDK_MdmWrite(buf, 350))!=NDK_OK)
	{				
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��ʱ
	sleep(10);
	GetReadLen=-1;
	if((ret=NDK_MdmClrbuf())!=NDK_OK)//����ջ���
	{		
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//case6:���Ե���NDK_MdmClrbuf�ж�modem����Ӧ��Ϊ0
	if(((ret=NDK_MdmGetreadlen(&GetReadLen))!= NDK_OK)||(GetReadLen!=0))
	{
		send_result("line %d:%s��ȡmodem���ȴ���(ret=%d,len=%d)", __LINE__, TESTAPI, ret, GetReadLen);
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

