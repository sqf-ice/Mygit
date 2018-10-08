/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt24.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterBondConfirm�ܷ���PINCODE��SSPģʽ�½������ȷ��
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterBondConfirm"	
#define SBTNAME "test24"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 		: bt24
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150608		created
*****************************************************************/
void bt24(void)
{
	/*private & local definition*/
	char skey[7]={0}, smac[6]={0};
	int ret=0, status=0;
	EM_PAIRING_MODE pair_mode=0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
	//case1:��������δ�򿪣���ȡ�������״̬�����ȷ��Ӧ��ʧ�ܣ�����ֵΪ��NDK_ERR_OPEN_DEV��	
	if((ret=NDK_BTMasterBondConfirm(skey,1))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	

	//case2:δ��� ����ȷ����� ����ʧ��
	if((ret=NDK_BTMasterBondConfirm(skey,1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case3:�����Ƿ�����
	if((ret=NDK_BTMasterBondConfirm(NULL,1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterBondConfirm(skey,2))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterBondConfirm(skey,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//ɨ����豸MAC��ַ
	cls_show_msg("��򿪴��豸�����󣬰����������");
	memset(smac,0,sizeof(smac));
	if(lib_btmaster_scan(smac, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:���豸��JUST WORK/PASSKEYģʽ�£��������ȷ��Ӧ�ò���ʧ��
	cls_show_msg("���豸��[�����������ģʽ]ΪJUST WORKģʽ��,����[�ȴ����]�����������");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(ret=(NDK_BTMasterBondConfirm(skey,1))!=NDK_ERR)
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ոշ������ʱ���ģʽӦ�÷���δ�������255�����״̬Ӧ�÷��ؿ���״̬0//����˵4������JUST WORK SCģʽ��δ������Զ�Ӧ��ֵ��255
	if((ret=NDK_BTMasterGetBondStatus(&pair_mode,skey,&status))!=NDK_OK||pair_mode!=255||status!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, pair_mode, status);
		GOTOERR;	
	}
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"��Գɹ�,����������ͨ��");
	
	//case5.1:���豸��SSP���ģʽ�£�����ȷ�����Ԥ����ʾ��Գɹ�
	cls_show_msg("���豸��[�����������ģʽ]ΪSSPģʽ��,����[�ȴ����]�����������,���豸����ȷ����Բ���");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
	 	send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//cls_show_msg("����ʾ�Ƿ�ȷ�����ʱ,�����豸����ȷ�����,���������");
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"��Գɹ�,����������ͨ��");
	
	//case5.2:���豸��SSP���ģʽ�£�����ȡ�����Ԥ����ʾ���ʧ��
	cls_show_msg("���豸��[�����������ģʽ]ΪSSPģʽ��,����[�ȴ����]�����������,���豸����ȡ����Բ���");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
	 	send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=lib_btmaster_pairing(0))!=3)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"���ʧ��,����������ͨ��");

	//case6:��������:���豸ѡ��passkeyģʽ�����豸������ȷkeyԤ�ڻ���ʾ��Գɹ�
	cls_show_msg("���豸��[�����������ģʽ]Ϊpasskeyģʽ��,����[�ȴ����]�����������,���豸������ȷ��KEY");
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
	cls_show_msg1(2,"��Գɹ�,����������ͨ��");

	//case7:��������:���豸ѡ��passkeyģʽ�����豸�������keyԤ�ڻ���ʾ���ʧ��
	cls_show_msg("���豸��[�����������ģʽ]Ϊpasskeyģʽ��,����[�ȴ����]�����������,���豸��������KEY");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=lib_btmaster_pairing(1))!=3)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	cls_show_msg1(2,"���ʧ��,����������ͨ��");
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_PortClose(PORT_NUM_BT);
	return;
}

