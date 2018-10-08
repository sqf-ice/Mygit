/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt27.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterRead�ܷ����趨��ʱ���ڴ�ָ���������豸�ж�ȡָ�����ȵ�����
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterRead"	
#define READ_MAX_SIZE 4096
#define SEND_SIZE 6000//ʵ�ʲ���
#define TIME_OUT 30000
#define WUCHASEC 0.03 //���ֵ������

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt27
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150611		created
*****************************************************************/
void bt27(void)
{
	/*private & local definition*/
	int ret=0, ret1=0, ret2=0, ret3=0, ret4=0,readlen=0,readlen1=0,j=0;
	char smac[6]={0};
	char recbuf[SEND_SIZE]={0},sendbuf[SEND_SIZE]={0};
	float tmp = 0.0;

	
	/*process body*/
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
	
	//���豸�������ӷ�ʽΪĬ�ϣ����豸Ϊ˫ģ�豸�������豸��Բ���������
	cls_show_msg("���˫ģ���������豸,�����������");  
	if(lib_btmaster_scan(smac, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	//��������豸
	cls_show_msg("���豸��[�����������ģʽ]�����[�ȴ����]�����������");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�������Ӵ��豸
	if((ret=NDK_BTMasterConnect(smac,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=lib_btmaster_getconstatus(smac, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case1:�����Ƿ�����
	if(((ret=NDK_BTMasterRead(NULL,sizeof(recbuf),recbuf,TIME_OUT,&readlen))!=NDK_ERR_PARA)||
		((ret1=NDK_BTMasterRead(smac,sizeof(recbuf),NULL,TIME_OUT,&readlen))!=NDK_ERR_PARA)||
		((ret2=NDK_BTMasterRead(smac,sizeof(recbuf),recbuf,-1,&readlen))!=NDK_ERR_PARA)||
		((ret3=NDK_BTMasterRead(smac,0,recbuf,TIME_OUT,&readlen))!=NDK_ERR_PARA)||
		((ret4=NDK_BTMasterRead(smac,READ_MAX_SIZE+1,recbuf,TIME_OUT,&readlen))!=NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2, ret3, ret4);//-6,-6,-6,0,0
		GOTOERR;
	}
	
	//case2:����豸дָ�����ȵ����ݣ�����4K�����ݣ������豸�����������ݷ��͸����豸���������Σ���У�����豸���ζ��������������豸���͵������Ƿ�һ��
	cls_show_msg("���д����ݶ�д,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,SEND_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ���豸����������
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,READ_MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterRead(smac,SEND_SIZE-READ_MAX_SIZE,recbuf+READ_MAX_SIZE,TIME_OUT,&readlen1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if(((readlen+readlen1)!=SEND_SIZE)||memcmp(sendbuf, recbuf, SEND_SIZE))	
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, readlen, readlen1);
		GOTOERR;	
	}

	//case3:����ʱ����:Ӧ�÷��س�ʱ,���ҳ�ʱʱ������Χ��
	cls_show_msg("����ʱ���ԣ�����豸���˳�[�����շ�],�����������");
	if((ret=NDK_BTMasterClrBuf(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	lib_StartStopwatch();
	if(((ret=NDK_BTMasterRead(smac,READ_MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_ERR_TIMEOUT)|| (tmp=fabs(lib_ReadStopwatch()-TIME_OUT/1000))>WUCHASEC)
	{
		send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
		GOTOERR;
	}

	//case4:�������϶Ͽ�����Ӧ��û���� 
	cls_show_msg("����д��Ͽ��������豸����,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<READ_MAX_SIZE; j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,READ_MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ���豸����������
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,READ_MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret = NDK_BTMasterDisconnect(smac)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:�Ͽ��������Ӻ󣬶�����Ӧ����δ���ӵ����豸
	if((ret=NDK_BTMasterRead(smac,READ_MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_ERR_BT_NOT_CONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;
	}

	//case6:	�ر��������ڣ���ȡ����Ӧ��ʧ��
	if((NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}	
	if((ret=NDK_BTMasterRead(smac,READ_MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTMasterDisconnect(smac);
	NDK_PortClose(PORT_NUM_BT);
	return;
	
}


