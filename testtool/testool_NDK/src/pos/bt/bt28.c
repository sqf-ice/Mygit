/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt28.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterReadLen�ܷ���ȷ�����豸�����������ֽ�
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterReadLen"	
#define MAX_SIZE 4096
#define TIME_OUT 30000

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 		: bt28
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150612		created
*****************************************************************/
//����Ϊ��������ں���
void bt28(void)	//�޲����޷���
{
	/*private & local definition*/
	int ret=0,readlen=0,j=0;
	char smac[6]={0};
	char recbuf[MAX_SIZE]={0},sendbuf[MAX_SIZE]={0};

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
	cls_show_msg("������������豸����ѡ�����Ӧ�����ģʽ�����ԣ����������");
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
	if((ret=NDK_BTMasterReadLen(NULL,&readlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMasterReadLen(smac,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:��������:����豸дһ���ֽڵ����ݣ��ٵ��ö�ȡ�������ֽڵĺ������������ֽ���Ӧ��д���ֽ���һ��
	cls_show_msg("���ж�������,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ�ӻ����������ݳ���
	sleep(5);//ԭ��û��5S��ʱ�������ĳ���Ϊ0
	if((ret=NDK_BTMasterReadLen(smac,&readlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if(readlen!=sizeof(sendbuf)) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, readlen);//len=0
		GOTOERR;	
	}

	//case3:�建����ڵ��ã���ȡ�������ݳ���Ӧ��Ϊ0
	cls_show_msg("�建���������,���豸����[�����շ�]�����������");
	if((ret = NDK_BTMasterClrBuf(smac)) != NDK_OK)	
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMasterReadLen(smac,&readlen))!=NDK_OK || readlen!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, readlen);
		GOTOERR;	
	}
	
	//case4:����豸д���ݣ����豸��ȡ���ݺ󣬽������ݵĻ�������0������ֽ���Ϊ0
	cls_show_msg("��ȡ���ݺ󣬻�������0.���豸����[�����շ�]�����������");
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_BTMasterWrite(smac,MAX_SIZE,sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//���豸Ӧ��Ҫ��ȡ���ݣ����ҷ������ݸ����豸
	
	//���豸��ȡ�ӻ����������ݳ���
	memset(recbuf,0,sizeof(recbuf));
	if((ret=NDK_BTMasterRead(smac,MAX_SIZE,recbuf,TIME_OUT,&readlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((readlen!=sizeof(sendbuf))||memcmp(sendbuf,recbuf,readlen))	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterReadLen(smac,&readlen))!=NDK_OK || readlen!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, readlen);
		GOTOERR;	
	}
	
	//case5:�Ͽ��������Ӻ�Ӧ����δ���ӵ����豸
	if((ret = NDK_BTMasterDisconnect(smac)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMasterReadLen(smac,&readlen))!=NDK_ERR_BT_NOT_CONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:�ر��������ڣ�Ӧ��ʧ��
	if((NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}	
	if((ret=NDK_BTMasterReadLen(smac,&readlen))!=NDK_ERR_OPEN_DEV)
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


