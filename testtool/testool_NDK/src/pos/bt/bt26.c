/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt26.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterWrite�ܷ��ָ�������豸��ָ�����ȵ�����
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterWrite"	
#define MASTER_MAX_SIZE 4096//û�����ֵ
#define TIME_OUT 30000
#define EXPMAXNUM 3
#define 	SNDCNT			(71)
#define 	SNDCNT1			(128)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt26
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150611		created
*****************************************************************/
//����Ϊ��������ں���
void bt26(void)	//�޲����޷���
{
	/*private & local definition*/
	int ret=0,j=0,real_len=0;
	char smac[6]={0};
	char sendbuf[MASTER_MAX_SIZE]={0},recbuf[MASTER_MAX_SIZE]={0};

	
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
	//ɨ��˫ģ��������
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
	cls_printf("���Ӵ�ģʽ�豸��");
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
	
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	/*����豸�Ľ��ջ�����*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1:	�����Ƿ�����
	if((ret=NDK_BTMasterWrite(NULL,sizeof(sendbuf),sendbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterWrite(smac,sizeof(sendbuf),NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case2:Ӧ������豸дָ�����ȵ����ݣ�����8�ֽ����ݣ����ٽ����豸���������ݣ����͸����豸��У�����豸���͵����������豸��ȡ�����Ƿ�һ��
	cls_show_msg("���з���8�ֽ����ݶ�д,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	/*����豸�Ľ��ջ�����*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,SNDCNT,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ���豸����������
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,SNDCNT, recbuf,TIME_OUT,&real_len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((real_len!=SNDCNT)||memcmp(sendbuf, recbuf, real_len))	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case2.1:Ӧ������豸дָ�����ȵ����ݣ�����64���������ֲ�����4K�����ݣ����ٽ����豸���������ݣ����͸����豸��У�����豸���͵����������豸��ȡ�����Ƿ�һ��
	cls_show_msg("����64���������ֲ�����4K�����ݶ�д,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	/*����豸�Ľ��ջ�����*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,SNDCNT1,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ���豸����������
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,SNDCNT1, recbuf,TIME_OUT,&real_len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((real_len!=SNDCNT1)||memcmp(sendbuf, recbuf, real_len))	
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, real_len, SNDCNT1);
		GOTOERR;	
	}

//#if 0
	//case3:���豸�����豸д���� Ȼ�����豸�Ͽ����Ӳ�Ӧ�����쳣
	cls_show_msg("����д��Ͽ��������豸����,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	/*����豸�Ľ��ջ�����*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,MASTER_MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}	
//#endif
	if((ret = NDK_BTMasterDisconnect(smac)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}


	//case4:���½������豸����豸���������Ӧ�����������������շ�
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
	cls_printf("���Ӵ�ģʽ�豸��");
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
	cls_show_msg("����4K�����ݶ�д,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	/*����豸�Ľ��ջ�����*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,MASTER_MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ���豸����������
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,MASTER_MAX_SIZE, recbuf,TIME_OUT,&real_len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((real_len!=MASTER_MAX_SIZE)||memcmp(sendbuf, recbuf, real_len))	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case5:�Ͽ��������ӣ�Ӧ����δ���ӵ����豸
	if((ret=NDK_BTMasterDisconnect(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d),���ӳ�ʱ", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterWrite(smac,sizeof(sendbuf),sendbuf))!=NDK_ERR_BT_NOT_CONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case6:��������δ�򿪣���ָ�������豸��ָ�����ȵ�����Ӧ��ʧ��
	if((NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterWrite(smac,sizeof(sendbuf),sendbuf))!=NDK_ERR_OPEN_DEV)
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

