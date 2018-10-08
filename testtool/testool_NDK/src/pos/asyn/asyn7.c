/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn7.c
* Author 			: chenjings
* version			: 
* DATE			: 20130114
* directory 		: src/pos/asyn
* description		: ����NDK_MdmClrbuf �ܷ�����첽ͨѶ������
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmClrbuf"
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
*			  	  	chenjings      20130114	created
*****************************************************************/
void asyn7(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};// 1K���ĵ��Ƽ�������,2K���������

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
	//case1:δ��ʼ��
	if((ret=NDK_MdmClrbuf())!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ������ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	
#endif
	//��������
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

	//д����
	if((ret=NDK_MdmWrite(buf,sizeof(buf)))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;
	}
	//��ʱ
	sleep(10);
	
	//case2:������ն��������Ƿ���Ч(��պ�Ӧ�ò���read������)
	if((ret=NDK_MdmClrbuf())!=NDK_OK)
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(rbuf)+1;
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_TIMEOUT)	//Ӧ�ò��ܶ�ȡ������
	{
		send_result("line %d:%s�����ݳ�ʱ����ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:������ȷ��ջ�����������ͨѶӦ�ɹ�
	if((ret=NDK_MdmClrbuf())!=NDK_OK)
	{
		send_result("line %d:%s��ջ���ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//д����
	if((ret=NDK_MdmWrite(buf, sizeof(buf)))!=NDK_OK)
	{		
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)+1;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�����
	if((sizeof(buf)!=rlen)||(MemCmp(buf, rbuf, rlen)))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf), rlen);
		goto ERR;
	}

	//case4:��д�������
	
	//���Խ���
	LinkDown();
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	LinkDown();
	return;


}

