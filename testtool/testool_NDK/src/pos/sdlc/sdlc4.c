/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc4.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmWrite �ܷ����ͬ��ͨѶ�ķ������ݲ�������SDLCд����
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmWrite"
#define MAX_SDLC_WRITEDATALEN 2048//1024=>2048�û���Ҫ�ײ㻺��������2048
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
void sdlc4(void)
{
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[2048+1] = {0}, rbuf[512] = {0};// 1K���ĵ��Ƽ�������

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

	//case2 ,д��ȷ����
	//case2.1 ��ʼ��buf
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ
	//case2.2,��ȷд����
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;
	}
	//case2.3:�Ƿ�������������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2.4:�Ƚ϶�д�����Ƿ�һ��
	if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		GOTOERR;
	}

	//case3:д���ݳ���Ϊ2048��Ӧ�óɹ����ײ�Ҫ����֧��2K���ݻ�����//���ڲ��Ի������ƣ��������ͱȽ� 20130724
	if((ret=NDK_MdmWrite(buf, MAX_SDLC_WRITEDATALEN))!=NDK_OK)
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if 0	//linwl20130606,�ͻ�������Ҫ֧�ִ���1K���ϵ������շ��������θ����������ⲿ��֤�����Դ���1K�����շ���֧�� ���������case3��֤
	//case3,д����1024+1,Ӧ����ʧ��
	if((ret=NDK_MdmWrite(buf, MAX_SDLC_WRITEDATALEN+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%sд���ݷ���ֵ����(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	//case4,�������ݳ���Ϊ0��ҲӦ�÷���NDK_ERR_PARA
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
	
	//case6:���� ͬ����·�Ѷ�ʱ�Ƿ񷵻�δ���š�
	//�һ�
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s�һ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(5);
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_ERR_MODEM_NOPREDIAL)//�ſ�����==NDK_OK)
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

