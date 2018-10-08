/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn3.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/asyn
* description		: ����NDK_MdmWrite �ܷ�����첽ͨѶ�ķ������ݲ�������asynд����
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmWrite"
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
*			  	  	chenjings      20130114	created
*****************************************************************/
void asyn4(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[ASYNPCKTMAXLEN+1] = {0}, rbuf[512] = {0};// 1K���ĵ��Ƽ�������

	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);  //��Ļ��ʾ��ǰ���ڵĲ�����

	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
#if 0 //20141202 reset���������޸�ֱ�ӷ���NDK_OK�����α����� 
	//case1,δ��ʼ�����
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);		
		RETURN;
	}
#endif
	//��������
	if((ret=LinkUp())!=NDK_OK)
	{
		send_result("line %d:%s������·ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:����д��ȷ����
	//��ʼ��buf
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;

#if 0		
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif
	
	//д����
	if((ret=NDK_MdmWrite(buf,sizeof(rbuf)))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;
	}
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(rbuf)+1;//������ȡ�������ݳ���Ҫ���ڵ���ʵ���ܶ�ȡ�������ݳ��ȡ�
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�����
	if((sizeof(rbuf)!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(rbuf), rlen);
		GOTOERR;
	}
	
	//case3:д����4096+1,Ӧ����ʧ��
	if((ret=NDK_MdmWrite(buf, ASYNPCKTMAXLEN+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�������ݳ���Ϊ0��ҲӦ�÷���NDK_ERR_PARA
	memset(rbuf, 0, sizeof(rbuf));//���rbuf,������һ��rbuf���Ϳ�����
	if((ret=NDK_MdmWrite(rbuf, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5,��������ָ��ΪNULL
	if((ret=NDK_MdmWrite(NULL, PCKTMAXLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:���� �첽��·�Ѷ�ʱ���Ƿ�Ż�δ���š�
	//�һ�
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s�һ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK/*NDK_ERR_MODEM_NOPREDIAL*/)//�첽MDM��дʱ��д���ڲ�����������Ȼ���سɹ�,linwl20130617
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	LinkDown();
	send_result("%s����ͨ��",TESTAPI);
	return;
ERR:
	LinkDown();
	return;
}

