/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt22.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterBond�ܷ�����������
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterBond"	//����API��API�Ե����֣�����API�ԣ���ʽ����api1/api2����

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt22
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150605		created
*****************************************************************/
//����Ϊ��������ں���
void bt22(void)	//�޲����޷���
{
	/*private & local definition*/
	char smac[6]={0}, //smac[6]={0x08,0x7a,0x4c,0x08,0xd8,0xb9},//���豸��MAC��ַ
		 smac1[6]={0x11,0x22,0x33,0x44,0x55,0x66};
	int ret=0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
	//case1:��������δ�򿪣���ȡ���ӽ��Ӧ��ʧ��	
	if((ret=NDK_BTMasterBond(smac))!=NDK_ERR_OPEN_DEV)
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
	
	//case2:�����Ƿ����ԣ���Ե��豸mac��ַΪnull��Ϊ7���ַ���5���ַ������ȡ���ӽ��Ӧ��ʧ��
	if((ret=NDK_BTMasterBond(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:����һ������ɨ��������豸��MAC��ַ�������ʧ�ܡ�
	if((ret=NDK_BTMasterBond(smac1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=lib_btmaster_pairing(1))!=3)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:���һ�������ڵ�MACȻ����case5�����һ�����ڵ�MAC���ȡ���״̬�ĳ�ʼ״̬Ӧ��Ϊ0����Ӧ�ó��ֳ�ʼ״̬Ϊ3�����
	if((ret=NDK_BTMasterBond(smac1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:��������:ѡ���ض���MAC��ַ�Ĵ�ģʽ�����ģʽ��Ϊ����һ�֣�������������
	//****���iPhone�ֻ�,ͬʱ��iPhone�ֻ���mac��ַ����smac,Ȼ����֤�Ƿ��ܹ���Գɹ� 
	//****���ɹ������ʹ��ɨ��ָ�����ƵĴ��豸,Ȼ���mac��ַ����smac�������
	cls_show_msg("��򿪴��豸�����󣬰����������");
	memset(smac,0,sizeof(smac));
	if(lib_btmaster_scan(smac, 0)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���豸��[�����������ģʽ]�����[�ȴ����]�����������");
	if((ret=NDK_BTMasterBond(smac))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//���Ժ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_PortClose(PORT_NUM_BT);
	return;
}
