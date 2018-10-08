/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt6.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTStatus�Ƿ��ܹ���ȡ����ȷ����������״̬
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTStatus"
#define   MAXWAITTIME		30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt6
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt6(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = 0, status= 0, flag = 0, j = 0, accept = 1;
	uint ulen = 0;
	char pkey[7] = {0};
	time_t oldtime = 0, diff = 0;

	/*process body*/
	//BLEֻ��just work ģʽ,��ƽ����������֤����
	if(cls_show_msg("����%s...�����ƻ���ֻ�6s���°汾����,�˰�������,[ESC]����,[����]����", TESTAPI)==ESC)
	{
		send_result("%s����,ƻ���ֻ�6s���°汾��֧��just work�����ģʽ,�û�ȡ������", TESTAPI);
		return;
	}
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1:δ���������ڷ���ʧ��
#if !K21_ENABLE
	if((ret=NDK_BTStatus(&status))!=NDK_ERR_OPEN_DEV)
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
	//ME30MH������Ҫ�����㲥
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	//��������ģʽ
	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:�쳣����,����ΪNULL
	if((ret=NDK_BTStatus(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//���������ж�status,�ֱ������ģʽ���в���
	//case3: ��������ģʽ,����1(δ����)
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	//PCI ��֤�汾��֧��just workģʽ �����������ж�
#if JUSTWORK_ENABLE
	//case4:�������ģʽ0(Just Work)
	NDK_BTSetPairingMode(PAIRING_MODE_JUSTWORK);
	//�˳�����ģʽ,��������ģʽ,����ʵ���������status
	if((ret=NDK_BTExitCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//cls_show_msg("����ǰ����Ե�POSȡ��,ȡ�������������");
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	cls_show_msg("just workģʽ:�ֻ�������������ɨ������POS������:%s,����ʾ������ʱ����",BtName);
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case8:NDK_BTDisconnect֮��ȡ״̬Ӧ�÷���δ����״̬
	sleep(2);
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
#endif

#if !JUSTWORK_FLAG
//����pincodeģʽ��־λ
#if PINCODE_ENABLE
	//case5:�������ģʽΪ1(Pin Code)
	//cls_show_msg("����ǰ����Ե�POSȡ����,�����������");
	NDK_BTEnterCommand();//��������ģʽ
	NDK_BTSetPairingMode(PAIRING_MODE_PINCODE);//����
	NDK_BTExitCommand();//�˳�����ģʽ
	cls_show_msg("pin codeģʽ:�ֻ���������ɨ������POS������:%s,����pin�Ի���ʱ��ȡ��,����ʾδ����ʱ����",BtName);
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	cls_show_msg("pin codeģʽ:�ֻ�������������ɨ������POS������:%s,����pin�Ի���,��������pin,����ʾδ����ʱ����",BtName);
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	cls_show_msg("pin codeģʽ:�ֻ�������������ɨ������POS������:%s,����pin�Ի���,������ȷ��pin:%s,����ʾ������ʱ����",BtName,BtPin);
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if(!g_at_enable)
	{
		//case6:�������ģʽΪ2(SSP)
#if BTPCI_ENABLE
		cls_show_msg("��ƻ���ֻ�����������Ե�POSȡ����,�����������");
#endif		
		NDK_BTEnterCommand();
		NDK_BTSetPairingMode(PAIRING_MODE_SSP);//����SSPģʽ
		NDK_BTExitCommand();
		for(j=0;j<2;j++)
		{
			if(j==0)
				cls_printf("SSP PINģʽ:�ֻ�����������POS��������,������SSP��ԶԻ���ʱ,�ֻ�ȡ�����");
			else
				cls_printf("SSP PINģʽ:�ֻ�����������POS������������,������SSP��ԶԻ���ʱ,�ֻ�ȷ�����");
			oldtime = time(NULL);	
			while(1)
			{
				if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				switch(flag)
				{
					case 0:
						if((diff=time(NULL)-oldtime)>60)
						{
							send_result("line %d:%s��ʱ��δ��⵽�������(%d)", __LINE__, TESTAPI, flag);
							goto ERR;
						}
						break;
					case 1:
						//j=0ʱȡ����� j=1ʱȷ�����
						if((ret=NDK_BTConfirmPairing(pkey, j))!=NDK_OK)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
							goto ERR;
						}
						break;
					case 2://��Գɹ�
						cls_show_msg("��Գɹ�,���������");
						if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
						{
							send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
							goto ERR;
						}
						if((ret=bt_comm_once(12,0))!=NDK_OK)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
							goto ERR;
						}
						if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
						{
							send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
							goto ERR;
						}
						break;
					case 3://���ʧ��
						if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
						{
							send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
							goto ERR;
						}
						break;
					default:
						send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
						goto ERR;
						break;
				}
				if(flag ==2 || flag == 3)
					break;//�˳�while����ִ��
			}
		}
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		//case7:�������ģʽΪ3(PassKey)	
		//cls_show_msg("����ǰ����Ե�POSȡ����,�����������");
		NDK_BTEnterCommand();
		NDK_BTSetPairingMode(PAIRING_MODE_PASSKEY);//����PassKeyģʽ
		NDK_BTExitCommand();
		for(j=0;j<3;j++)
		{
			cls_printf("pass keyģʽ:ʹ���ֻ�����������POS��������");
			oldtime = time(NULL);
			while(1)
			{
				accept = 1;
				if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				switch(flag)
				{
					case 0:
						if((diff=time(NULL)-oldtime)>60)
						{
							send_result("line %d:%s��ʱ��δ��⵽�������(%d)", __LINE__, TESTAPI, flag);
							goto ERR;
						}
						break;
					case 1://�յ��ֻ��������
						if(pkey[0]!='\0')
						{
							send_result("line %d:%s����ʧ��(%c)", __LINE__, TESTAPI, pkey[0]);
							goto ERR;
						}
						if(j==0)
							cls_printf("POSȡ�����");
						else if(j==1)
							cls_printf("POS���������������:\n");
						else			
							cls_printf("POS����ȷ�����ֻ�����ʾ�������:\n");
						if((ret=lib_kbgetinput(pkey, 0, 6, &ulen, INPUTDISP_PASSWD, 0, INPUT_CONTRL_LIMIT_ERETURN)) == NDK_ERR || ret == NDK_ERR_TIMEOUT)
						{
							accept = 0;
						}		
						if((ret=NDK_BTConfirmPairing(pkey,accept))!=NDK_OK)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
							goto ERR;
						}	
							
						break;
					case 2://��Գɹ�
						cls_show_msg("��Գɹ�,���������");
						if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
						{
							send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
							goto ERR;
						}
						if((ret=bt_comm_once(12,0))!=NDK_OK)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
							goto ERR;
						}
						if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
						{
							send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
							goto ERR;
						}
						break;
					case 3://���ʧ��
						if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
						{
							send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
							goto ERR;
						}
						break;
					default:
						send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
						goto ERR;
						break;
				}
				if(flag ==2 || flag == 3)
					break;//�˳�while����ִ��
			}
		}
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}

		//��׿�ֻ���just work scģʽ;BLE4.2����Э���just work and scģʽ����ԶԻ���,��ȡ����ȷ��
#if JUSTWORK_SC_ENABLE
		if(cls_show_msg("����ǰ�׿�ֻ�����,�밴[ESC]����,ƻ��6s�����ֻ����԰�[����]����")==ESC)
		{ 
			send_result("%s����,��׿�ֻ���ƻ���ֻ�6s���°汾�ֻ���֧��just work and scģʽ,�û�ȡ������", TESTAPI);
			GOTOERR;
		}
		else
		{
			NDK_BTEnterCommand();//��������ģʽ
			NDK_BTSetPairingMode(PAIRING_MODE_JUSTWORK_SC);//����
			NDK_BTExitCommand();//�˳�����ģʽ
			cls_show_msg("just work scģʽ:�ֻ���������ɨ������POS������:%s,������ԶԻ���ʱ��ȡ��,����ʾδ����ʱ����",BtName);
			if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
			{
				send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
				GOTOERR;
			}
			cls_show_msg("just work scģʽ:�ֻ�������������ɨ������POS������:%s,������ԶԻ���ʱ��ȷ��,����ʾ������ʱ����",BtName);
			if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
			{
				send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
				GOTOERR;
			}
			if((ret=bt_comm_once(12,0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
			{
				send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
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
	NDK_BTEnterCommand();
	NDK_BTSetPairingMode(g_pair_mode);//�ָ������е�����ģʽ
	NDK_BTExitCommand();
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

