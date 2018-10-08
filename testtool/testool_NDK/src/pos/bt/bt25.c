/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt25.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterConnect��NDK_BTMasterGetConStatus�ܷ��������Ӻͻ�ȡ����״̬
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterConnect,NDK_BTMasterGetConStatus"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt25
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150609		created
*****************************************************************/
//����Ϊ��������ں���
void bt25(void)	//�޲����޷���
{
	/*private & local definition*/
	char ble_mac[6]={0},
		 spp_mac[6]={0},
		 smac[6]={0};
	int ret=0,dev_type=0,c_status=0,i=0;

	/*process body*/
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1:��������δ�򿪣���������ʧ�ܣ���ȡ��������״̬ʧ��
	if((ret=NDK_BTMasterConnect(ble_mac,dev_type))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;	
	}
	if((ret=NDK_BTMasterGetConStatus(ble_mac,&c_status))!=NDK_ERR_OPEN_DEV)
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

	//ɨ���������������������ݣ�����׼��3̨�豸�ֱ���˫ģ�豸����ģBLE�豸����ģSPP�豸
	//ɨ���MAC�ֱ𴫵ݸ�ble_mac��spp_mac��smac
	memset(spp_mac,0,sizeof(spp_mac));
	memset(smac,0,sizeof(smac));
	
	cls_show_msg("��򿪵�ģBLE���������豸,�����������");
	if(lib_btmaster_scan(ble_mac, 2)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg("��򿪵�ģSPP���������豸,�����������");
	if(lib_btmaster_scan(spp_mac, 1)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg("���˫ģ���������豸,�����������");
	if(lib_btmaster_scan(smac, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	
	//��3̨�豸
	cls_show_msg("��ģBLE���豸ѡ�����ģʽ�����������");
	if((ret=NDK_BTMasterBond(ble_mac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
/*	if((ret=lib_btmaster_pairing(1))!=2)  //BLE�������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}*/

	cls_show_msg("��ģSPP���豸ѡ�����ģʽ�����������");
	if((ret=NDK_BTMasterBond(spp_mac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	cls_show_msg("˫ģ���豸��[�����������ģʽ]��,����[�ȴ����]�����������");
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

	//case2:�����Ƿ�����
	if((ret=NDK_BTMasterConnect(NULL,dev_type))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterConnect(smac,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterConnect(smac,3))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterGetConStatus(NULL,&c_status))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterGetConStatus(smac,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case3:δ�������ӣ���ȡ����״̬ӦΪ������
	if((ret=NDK_BTMasterGetConStatus(smac,&c_status))!=NDK_OK||c_status!=1)//����ret=31
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,c_status);
		GOTOERR;	
	}
	
	//case4:MAC�����ӷ�ʽͬ�豸֧�ֵ�ģʽ��ƥ����������ӳɹ�����������״̬ӦΪδ����
	if((ret=NDK_BTMasterConnect(ble_mac,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(1);//������Ҫʱ��,�����Ƿ��ܹ�������,֮�����ж�״̬
	if((ret=lib_btmaster_getconstatus(ble_mac, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterConnect(spp_mac,2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(1);//������Ҫʱ��,�����Ƿ��ܹ�������,֮�����ж�״̬
	if((ret=lib_btmaster_getconstatus(spp_mac, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//�������� (�Կ��������ϵ������豸���жϿ�,�Է��Ժ�����Ӱ��)
	NDK_BTMasterDisconnect(ble_mac);
	NDK_BTMasterDisconnect(spp_mac);
	
	//case6:�����Դ��豸ʹ��˫ģ�豸(���˫ģ�豸����������������ģӦ��Ҳ����)��˫ģʱ�������ӷ�ʽ��Ӧ�óɹ������豸���ӵ���Է�ʽ����ѡ��4���е���������
	for(i=0;i<3;i++)
	{	//i=2���Խ�� ��Գɹ��������ӳ�ʱ ����ʱ���˳������ٲ���ż�������������������-1�����
		cls_show_msg("˫ģ���豸��[�����������ģʽ]��,����[�ȴ����]�����������(���ӷ�ʽ:%d)",i);
		if(lib_btmaster_scan(smac, i)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
		if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
		if(i!=2)  //i=2ΪBLE��ʽ,BLE����Ҫ���,��˾��Ʒ����� ,����������  20170704
		{
			if((ret=lib_btmaster_pairing(1))!=2)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
		if((ret=NDK_BTMasterConnect(smac,i))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
		if((ret=lib_btmaster_getconstatus(smac, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
		if((ret=NDK_BTMasterDisconnect(smac))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTMasterDisconnect(smac);
	NDK_PortClose(PORT_NUM_BT);
	return;
}


