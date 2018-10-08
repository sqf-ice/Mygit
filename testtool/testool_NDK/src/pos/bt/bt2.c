/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt2.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTEnterCommand,NDK_BTExitCommand�Ƿ����������������ģʽ
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define    TESTAPI		"NDK_BTEnterCommand,NDK_BTExitCommand"
#define MAXWAITTIME		10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt2
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt2(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = 0, nKeyin = ENTER;
	char pinbuf[7] = {0}, namebuf[15] = {0};//pin������6λ,�ڹ�оME30�����з������������ֻ��6��������,��Ҫ����һ�ֽڵĻ�����20170801

	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//�򿪴���
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:BM77�ڽ�������ģʽ��Ӧ������������,֧��AT����ģʽ��BTģ�������ͬ,����ģ����������
	if(g_at_enable)
	{
		cls_show_msg1(MAXWAITTIME,"ȡ��֮ǰ����Ե�POS��,���������");
		nKeyin = cls_show_msg1(MAXWAITTIME,"ʹ���ֻ���������POS����s%Ӧ��������,��ȷ[Enter],����[����]", BtName);
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
	}

	//case2:��������ģʽ ���Խ���PIN �Ļ�ȡ�Ȳ���
#if !BTPCI_ENABLE
	memset(pinbuf,0,sizeof(pinbuf));
	if((ret=NDK_BTGetPIN(pinbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	memset(namebuf,0,sizeof(namebuf));
	if((ret=NDK_BTGetLocalName(namebuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(BtName, namebuf, strlen(namebuf)) && memcmp(BtPin, pinbuf, strlen(pinbuf)))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#if !BTPCI_ENABLE
	if((ret=NDK_BTSetPIN(pinbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if((ret=NDK_BTSetLocalName(namebuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�˳�����ģʽ����������ģʽ,�ɽ������ݼ�Ĵ���
	if((ret=NDK_BTExitCommand())!=NDK_OK)
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
		send_result("line %d:%s�������ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	send_result("%s����ͨ��", TESTAPI);	
ERR:
	NDK_BTExitCommand();
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

