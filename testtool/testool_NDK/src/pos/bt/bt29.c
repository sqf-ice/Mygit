/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt29.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterClrBuf  �ܷ����ָ���������ӵĽ��ջ�����
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterClrBuf"	
#define MAX_SIZE 1024
#define TIME_OUT 30000

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 		: bt29
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150612		created
*****************************************************************/
//����Ϊ��������ں���
void bt29(void)	//�޲����޷���
{
	/*private & local definition*/
	int ret = 0, readlen = 0, len = 0, j = 0;
	char smac[6] = {0},smac1[6] = {0};
	char recbuf[MAX_SIZE] = {0}, sendbuf[MAX_SIZE] = {0};
	time_t oldtime = 0;

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
	cls_show_msg("���˫ģ���������豸1,���������ʼɨ��");  
	if(lib_btmaster_scan(smac, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg("���˫ģ���������豸2,���������ʼɨ��");  
	if(lib_btmaster_scan(smac1, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	//������Ӵ��豸1
	cls_show_msg("���豸1��[�����������ģʽ]�����[�ȴ����]�����������");
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
	cls_printf("���Ӵ�ģʽ�豸1��");
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

	//������Ӵ��豸2
	cls_show_msg("���豸2��[�����������ģʽ]�����[�ȴ����]�����������");
	if((ret=NDK_BTMasterBond(smac1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("���Ӵ�ģʽ�豸2��");
	if((ret=NDK_BTMasterConnect(smac1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=lib_btmaster_getconstatus(smac1, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case1:�����Ƿ�����
	if((ret=NDK_BTMasterClrBuf(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:���һ̨�������ȥ��ȡ���ݣ�Ӧ�����������ˣ��ڶ�̨�����建����Կɶ�ȡ������
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_BTMasterClrBuf(smac1)) != NDK_OK)	
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	cls_show_msg("��̨���豸����[�����շ�]�����������");
	if((ret=NDK_BTMasterWrite(smac,MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterWrite(smac1,MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//�жϽ��ջ������Ѿ��յ��ͻ��������ݺ�
	oldtime=time(NULL);
	while(1)
	{	
		if((ret=NDK_BTMasterReadLen(smac,&len))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(len==MAX_SIZE)
			break;
		if(time(NULL)-oldtime>60)
		{
			send_result("line %d:%s����ʧ�ܣ����ݴ��䳬ʱ(%d)", __LINE__, TESTAPI, len);
			goto ERR;
		}
	}
	//������豸1�Ļ�����
	if((ret=NDK_BTMasterClrBuf(smac)) != NDK_OK)	
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,MAX_SIZE,recbuf,5000,&readlen))!=NDK_ERR_TIMEOUT||readlen!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, readlen);
		GOTOERR;	
	}

	//�ڶ�̨Ӧ����������
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac1,MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if(readlen!=MAX_SIZE||memcmp(sendbuf,recbuf,MAX_SIZE)) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;	
	}

	//case3:�Ͽ��������Ӻ�Ӧ����NDK_ERR_BT_NOT_CONNECTED
	if((ret = NDK_BTMasterDisconnect(smac)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_BTMasterDisconnect(smac1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMasterClrBuf(smac))!=NDK_ERR_BT_NOT_CONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:�ر��������ڣ�Ӧ�÷���NDK_ERR_OPEN_DEV
	if((NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}	
	if((ret=NDK_BTMasterClrBuf(smac))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTMasterDisconnect(smac);
	NDK_BTMasterDisconnect(smac1);
	NDK_PortClose(PORT_NUM_BT);
	return;
	
}

