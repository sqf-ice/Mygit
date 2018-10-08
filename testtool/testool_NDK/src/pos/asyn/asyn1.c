/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn1.c
* Author 			: chenjings
* version			: 
* DATE			: 20130106
* directory 		: src/pos/asyn
* description		: ����NDK_MdmAsynInit �ܷ������MODEM�����첽��ʽ�ĳ�ʼ��
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmAsynInit"
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
*			  	  	chenjings      20130106	created
*****************************************************************/
void asyn1(void)

{
	//����Ϊ�ֲ�����
	int i = -1, j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};
	
	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 

	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1: ����iΪ����ֵ��Ӧ�óɹ�	
	i = rand()%256;     
	if((ret = NDK_MdmAsynInit(i))!=NDK_OK)
	{
		send_result("line %d:%sʧ��(i=%d,ret=%d)", __LINE__, TESTAPI, i, ret);
		RETURN;
	}
	//����
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(DialNum=%s,ret=%d)", __LINE__, TESTAPI,  MDMDialStr,ret);
		GOTOERR;
	}
	//�ж�modem״̬���Ƿ����ӳɹ�
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%s modem״̬δ��ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
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
		send_result("line %d:%s д����ʧ��(ret=%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}
	//������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)+1;//������ȡ�������ݳ���Ҫ���ڵ���ʵ���ܶ�ȡ�������ݳ��ȡ�
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s ������ʧ��(ret=%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}
	//�Ƚ�����		
	if((sizeof(buf)!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf), rlen);
		GOTOERR;
	}
	//�һ�
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s�Ҷ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
	goto ERR;
ERR:
	NDK_MdmHangup();
	return;
}

