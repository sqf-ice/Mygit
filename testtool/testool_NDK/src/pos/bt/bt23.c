/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt23.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterGetBondStatus�ܷ��ȡ����ȷ���������״̬
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterGetBondStatus"	
#define SBTNAME "test23"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt23
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150605		created
*****************************************************************/
void bt23(void)	//�޲����޷���
{
	/*private & local definition*/
	char skey[7]={0};
	int status=0,ret=0;
	char smac[6]={0};
	EM_PAIRING_MODE pair_mode=0;

	/*process body*/
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
	//case1:��������δ�򿪣���ȡ�������״̬Ӧ��ʧ��
	if((ret=NDK_BTMasterGetBondStatus(&pair_mode,skey,&status))!=NDK_ERR_OPEN_DEV)
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
	
	//case2:δ����������ԣ���ȡ���״̬�ɹ�������ȡ״̬ӦΪ����״̬(0)����ȡ�������ģʽӦ��Ϊ255//����˵4������JUST WORK SCģʽ��δ������Զ�Ӧ��ֵ��255
	if((ret=NDK_BTMasterGetBondStatus(&pair_mode,skey,&status))!=NDK_OK||pair_mode!=255||status!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, pair_mode, status);
		GOTOERR;	
	}
	
	//case3:�����Ƿ�����
	if((ret=NDK_BTMasterGetBondStatus(NULL,skey,&status))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterGetBondStatus(&pair_mode,NULL,&status))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterGetBondStatus(&pair_mode,skey,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//ɨ�貢ѡ������豸
	cls_show_msg("��򿪴��豸�����󣬰����������");
	memset(smac,0,sizeof(smac));
	if(lib_btmaster_scan(smac, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case4:��������:���豸�����ģʽΪJUST WORK ��ȡ���ģʽӦ��ʵ�����ģʽһ�� �һ�ȡ��Գɹ���״̬
	cls_show_msg("���豸��[�����������ģʽ]ΪJUST WORKģʽ��,����[�ȴ����]�����������");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ոշ������,δ�յ��������ʱ,���ģʽӦ�÷���δ�������255�����״̬Ӧ�÷��ؿ���״̬0
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

	//case5.1:��������:���豸�����ģʽΪPIN CODE ��������pin����ȷ�����
	cls_show_msg("���豸��[�����������ģʽ]ΪPIN CODEģʽ��,����[�ȴ����]�����������,���ʱ���豸����������PINCODE");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=lib_btmaster_pairing(1))!=3)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"���ʧ��,����������ͨ��");
	
	//case5.2:���豸�����ģʽΪPIN CODE ������ȷ��pin����ȷ�����
	cls_show_msg("���豸��[�����������ģʽ]ΪPIN CODEģʽ��,����[�ȴ����]�����������,���ʱ���豸��������ȷ��PINCODE");
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
	
	//case5.3:���豸�����ģʽΪPIN CODE  ������ȷ��pin�뵫ȡ�����
	cls_show_msg("���豸��[�����������ģʽ]ΪPIN CODEģʽ��,����[�ȴ����]�����������,���ʱ���豸������PINֱ�Ӱ�ȡ��");
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

	//case6:ȡ����Ժ����״̬Ӧ���ǿ���״̬
	if((ret=NDK_BTMasterGetBondStatus(&pair_mode,skey,&status))!=NDK_OK||status!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, status);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_PortClose(PORT_NUM_BT);
	return;
}

