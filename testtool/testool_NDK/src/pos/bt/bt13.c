/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt13.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetPairingMode�Ƿ��ܹ���ȷ��������������ģʽ
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetPairingMode"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt13
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20141118		created
*****************************************************************/
void bt13(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = 0, tempairmode = g_pair_mode;
	
	/*process body*/
	//BLEֻ��just work ģʽ,��ƽ����������֤����
	if(cls_show_msg("����%s...�����ƻ���ֻ�6s���°汾����,�˰�������,[ESC]����,[����]����", TESTAPI)==ESC)
	{
		send_result("%s����,ƻ���ֻ�6s���²�֧��just work�����ģʽ,�û�ȡ������", TESTAPI);
		return;
	}
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
#if !K21_ENABLE
	//case1:��������δ�� ����ʧ��
	if((ret=NDK_BTSetPairingMode(PAIRING_MODE_PINCODE))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//BM77δ��������ģʽ ����ʧ��	
	if(g_at_enable)
	{
		if((ret=NDK_BTSetPairingMode(PAIRING_MODE_PINCODE))!=NDK_ERR_TIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	// ��������ģʽ����ܽ�������
	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�����쳣����ʧ��
	if((ret=NDK_BTSetPairingMode(PAIRING_MODE_PASSKEY+2))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTSetPairingMode(PAIRING_MODE_JUSTWORK-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
//PCI��֤�汾��֧��just workģʽ
#if JUSTWORK_ENABLE
	//case3:��ͬ��ģʽ�����ͬ	BLEЭ�鲻��
	//case3.1:just work mode ֱ������
	//cls_show_msg("�ֻ����������а�֮ǰ��Ե�����ȡ�������������");
	//cls_printf("JUSTWORK������,��ʹ���ֻ��������Գ���ɨ������POS����%s",BtName)
	g_pair_mode=PAIRING_MODE_JUSTWORK;
	if((ret=NDK_BTSetPairingMode(g_pair_mode))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTExitCommand())!=NDK_OK)//BM77��Ҫ���˳�����ģʽ������ѵ�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�������flash���·�������������������open���������ģʽ��ɾ����Ƶ����� 20160728 linwl
	NDK_PortClose(PORT_NUM_BT);
	NDK_PortOpen(PORT_NUM_BT, NULL);
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
		send_result("line %d:���ʧ��,���������(%d)", __LINE__, ret);
		GOTOERR;
	}
	if(cls_show_msg("�ֻ���POS�Ƿ���ֱ������,��[Enter]����,��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	
#if !JUSTWORK_FLAG
//����pincode��־λ����:оƬ20707-A2û��pincodeģʽ
#if PINCODE_ENABLE
	//case3.2:pincode mode  ��Ҫ��pin
	//cls_show_msg("�ֻ����������а�֮ǰ��Ե�����ȡ�������������");
	if((ret=NDK_BTEnterCommand())!=NDK_OK)//BM77Ҫ�ڽ�������ģʽ�����������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	g_pair_mode=PAIRING_MODE_PINCODE;
	if((ret=NDK_BTSetPairingMode(g_pair_mode))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTExitCommand())!=NDK_OK)//BM77��Ҫ���˳�����ģʽ������ѵ�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�������flash���·�������������������open���������ģʽ��ɾ����Ƶ����� 20160728 linwl
	NDK_PortClose(PORT_NUM_BT);
	NDK_PortOpen(PORT_NUM_BT, NULL);
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
		send_result("line %d:���ʧ��,���������(%d)", __LINE__, ret);
		GOTOERR;
	}
	if(cls_show_msg("�ֻ���POS����ʱ��PIN�������Ҫ����PIN,��[Enter]����,��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	NDK_BTEnterCommand();//�������˳����ʹ��
	if(!g_at_enable)//BM77��2 3 4 ģʽ
	{
		//case3.3:SSP mode
#if BTPCI_ENABLE
		cls_show_msg("�ֻ����������а�֮ǰ��Ե�����ȡ�������������");
#endif
		g_pair_mode=PAIRING_MODE_SSP;
		if((ret=NDK_BTSetPairingMode(g_pair_mode))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:���ʧ��,���������(%d)", __LINE__, ret);
			GOTOERR;
		}
		if(cls_show_msg("�ֻ���ʾSSP�����,��Ҫȷ�����,��[Enter]����,��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		//case3.4:Passkey mode
		//�˴��н�����ģʽ�л�,Ӧ�ò���Ҫ��ʾȡ�����
		//cls_show_msg("�ֻ����������а�֮ǰ��Ե�����ȡ�������������");
		g_pair_mode=PAIRING_MODE_PASSKEY;
		if((ret=NDK_BTSetPairingMode(g_pair_mode))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:���ʧ��,���������(%d)", __LINE__, ret);
			GOTOERR;
		}
		if(cls_show_msg("�ֻ�����ʾPASSKEY�����,POS��Ҫ���������,��[Enter]����,��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}

#if JUSTWORK_SC_ENABLE
		if(cls_show_msg("����ǰ�׿�ֻ�����,�밴[ESC]����,ƻ��6s�����ֻ����԰�[����]����")==ESC)
		{ 
			send_result("%s����,��׿�ֻ���ƻ���ֻ�6s���°汾�ֻ���֧��just work and scģʽ,�û�ȡ������", TESTAPI);
			GOTOERR;
		}
		else
		{
			//case3.5:just work and scģʽ����ԶԻ���,�ֻ���Ҫ�����ȷ��
			g_pair_mode=PAIRING_MODE_JUSTWORK_SC;
			if((ret=NDK_BTSetPairingMode(g_pair_mode))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if((ret=lib_bt_pairing(1))!=NDK_OK)
			{
				send_result("line %d:���ʧ��,���������(%d)", __LINE__, ret);
				GOTOERR;
			}
			if(cls_show_msg("�ֻ���ʾ��ԶԻ���,��Ҫȷ�����,��[Enter]����,��[����]")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
			if((ret=NDK_BTDisconnect())!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
#endif
	}
#endif

	send_result("%s����ͨ��", TESTAPI);	
ERR:
	//�ָ������е�����
	g_pair_mode = tempairmode;
	NDK_BTSetPairingMode(g_pair_mode);
	//�˳�����ģʽ
	NDK_BTExitCommand();
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

