/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt17.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetBleStatus �ܷ���ȷ�����Ƿ�֧��BLE����
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetBleStatus"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt17
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20150319		created
*****************************************************************/
void bt17(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	/*private & local definition*/
	int ret = -1;	  
	char outbuf[16] = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
	//case1:��������δ�� ���������Ƿ�֧��ble���ӷ���ʧ��
	/*���ڿ������ͣ������ڵ�һ���������ڴ�֮ǰ���ԣ�����Ѿ���ֲ��bt.c��*/
/*	if((ret=NDK_BTSetBleStatus(0))!=NDK_ERR_IOCTL)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return;
	}*/
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, outbuf);
		GOTOERR;
	}

	//case2:��������Ӧ�÷���ʧ��
	if((ret=NDK_BTSetBleStatus(2))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�ر�BLE��ʹ��lightbule ����ʱӦ����������
	if((ret=NDK_BTSetBleStatus(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("ʹ��LightBlueӦ����������POS����:%s,��[Enter],��[����]",outbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:����BLE��ʹ��lightbule ����ʱӦ����������	
	if((ret=NDK_BTSetBleStatus(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("ʹ��LightBlueӦ��������POS����:%s,��[Enter],��[����]",outbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
		
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_PortClose(PORT_NUM_BT);
	return;
}


