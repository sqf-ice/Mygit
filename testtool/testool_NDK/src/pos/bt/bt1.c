/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt1.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTReset�Ƿ����������λ
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define    TESTAPI		"NDK_BTReset"
#define MAXWAITTIME		10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt1
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt1(void) 
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1, get_status = -1;

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//case4:bt״̬��ͼ����ԣ�δ���������Ӧ�ò���ʾbtͼ��
#if STABAR_ENABLE
	int nKeyin = ENTER;
	nKeyin = cls_show_msg1(MAXWAITTIME,"��鿴POS״̬���ϵ��Ƿ���ʾ��������ͼ�꣬��[ȷ��]����[����]");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

	NDK_PortClose(PORT_NUM_BT);

#if !K21_ENABLE  //�ж�ƽ̨��Ҫ��open֮���������ʹ��
	//case1:δ���������ڷ���ʧ��
	if((ret=NDK_BTReset())!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	if(!g_at_enable)//���ΪBM77,��Ӳ��λ��������һ��
	{
		//case2:���������Ϻ���и�λ,��λ��������Ͽ�����
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//ME30MH������Ҫ�����㲥 YC1021оƬ
		if(g_btdiscoverflag)
		{
			if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_BTReset())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_BTStatus(&get_status))!= NDK_OK || get_status!= 1)
		{
			send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, get_status);
			GOTOERR;
		}
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	//case3:��������
	//����������,���и�λ֮����Խ������ݵĴ���
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTReset())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ME30MH������Ҫ�����㲥
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if STABAR_ENABLE
	//case5:bt״̬��ͼ����ԣ����ӳɹ���Ӧ��������ʾbtͼ��
	nKeyin = cls_show_msg1(MAXWAITTIME,"��鿴POS״̬���ϵ��Ƿ�������ʾ��������ͼ�꣬��[ȷ��]����[����]");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(!g_at_enable)//���ΪBM77,��Ӳ��λ��������һ��
	{
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//case6:�����Ͽ����Ӻ��ٽ���reset���Ƿ�����������������
		if((ret=NDK_BTReset())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//ME30MH������Ҫ�����㲥
		if(g_btdiscoverflag)
		{
			if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_BTStatus(&get_status))!= NDK_OK || get_status!= 0)
		{
			send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, get_status);
			goto ERR;
		}
	}
	send_result("%s����ͨ��", TESTAPI);	
ERR:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

