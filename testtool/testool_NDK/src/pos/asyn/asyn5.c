/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn5.c
* Author 			: chenjings
* version			: 
* DATE			: 20130114
* directory 		: src/pos/asyn
* description		: ����NDK_MdmRead �ܷ�����첽ͨѶ�Ľ������ݲ�������asyn������
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmRead"
#define MAXWAITTIME 60
#define MAXTIMEOUT  10
#define	WUCHASEC	0.03 //���ֵ����

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
void asyn5(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0, rlenother = 0;
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
	//��ʼ��buf
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	
#if 0		
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif

	//case2:��ȡ��ȷд����
	//д����
	if((ret=NDK_MdmWrite(buf, sizeof(buf)))!=NDK_OK)
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
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__,TESTAPI, sizeof(buf), rlen);
		GOTOERR;
	}
	
#if 0
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif

	//case3:���Զ��ȴ�ʱ��Ϊ0�������һ�ζ�����
	//д����
	if((ret=NDK_MdmWrite(buf, sizeof(buf)))!=NDK_OK)
	{		
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__,TESTAPI, ret);
		GOTOERR;
	}
	sleep(10);
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)+1;
	if((ret=NDK_MdmRead(rbuf, &rlen, 0))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ϶�ȡ����
	if((sizeof(buf)!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf), rlen);
		GOTOERR;
	}
	
#if 0
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif	
	//cas4:����ϣ����ȡ����С�ڷ��͵����ݳ���
	//д����
	if((ret=NDK_MdmWrite(buf, sizeof(buf)))!=NDK_OK)
	{		
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__,TESTAPI, ret);
		GOTOERR;
	}
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)-1;
	if ((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�����
	if(((sizeof(buf)-1)!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf)-1, rlen);
		GOTOERR;
	}
	//��ȡʣ�������
	memset(rbuf, 0, sizeof(rbuf));
	rlenother=sizeof(buf)-rlen;
	if ((ret=NDK_MdmRead(rbuf, &rlenother, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�����
	if(((sizeof(buf)-rlen)!=rlenother)||memcmp((buf+rlen), rbuf, rlenother))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,%x,R=%ld,%x)", __LINE__, TESTAPI, sizeof(buf)-rlen, rlenother);
		GOTOERR;
	}
	//case5:����������
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
	rlen=0;
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	rlen=-1;
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MdmRead(rbuf, &rlen, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

#if 0
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif	

	//case6: ���Զ��ȴ���ʱ
	//��ʱ��������û������,Ӧ���ض���ʱ
	rlen=sizeof(buf);
	lib_StartStopwatch();
	if(((ret=NDK_MdmRead(rbuf, &rlen, MAXTIMEOUT))!=NDK_ERR_TIMEOUT)||((tmp=(lib_ReadStopwatch()-MAXTIMEOUT))>WUCHASEC))
	{
		send_result("line %d:%s�����ݲ��Գ�ʱʧ��(ret=%d,tm=%f)", __LINE__, TESTAPI, ret, tmp);
		GOTOERR;
	}
	
	//case7:���Զ�ȡ�ĵȴ�ʱ��Ϊ0ʱ��û������Ӧ����������
	if((ret=NDK_MdmRead(rbuf, &rlen, 0))!=NDK_ERR_TIMEOUT)
	{
		send_result("line %d:%s�����ݲ��Գ�ʱʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:������·�Ѷ�ʱ�Ƿ񷵻�δ���š�
	if((ret=NDK_MdmHangup())!=NDK_OK )
	{
		send_result("line %d:%s�һ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	rlen=sizeof(buf);
	if((ret=NDK_MdmRead(rbuf, &rlen, 3))!=NDK_ERR_TIMEOUT/*NDK_ERR_MODEM_NOPREDIAL*/)//�첽MDM��дʱ�Ƕ����ڲ��������س�ʱ,linwl20130617
	{
		send_result("line %d:%s�����ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
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

