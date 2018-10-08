/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt14.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTGetPairingStatus,NDK_BTConfirmPairing�����Ƿ��ܹ���ȷ��ȡ�������״̬��ȷ�����
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTGetPairingStatus,NDK_BTConfirmPairing"
#define   MAXWAITTIME		30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt14
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20141118		created
*****************************************************************/
void bt14(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = 0, flag = 0;
	uint len = 0;
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

	//case1:BM77����ģ�鲻֧�֣�����NDK_ERR_NOT_SUPPORT
	if(g_at_enable)//BM77��֧��
	{
		if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_BTConfirmPairing(pkey,1))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		cls_show_msg1(10,"������ģ�鲻֧��%s����", TESTAPI);
		return;
	}

#if !K21_ENABLE
	//case2:��������δ�� ��ȡ���״̬ʧ�� ȷ�����Ҳʧ��
	if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_BTConfirmPairing(pkey,1))!=NDK_ERR_OPEN_DEV)
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
	//case3:���ģʽ����2 3ģʽ�Ļ᷵��ʧ��
#if !BTPCI_ENABLE
	NDK_BTSetPairingMode(PAIRING_MODE_JUSTWORK);
	if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTConfirmPairing(pkey,1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_BTSetPairingMode(PAIRING_MODE_PINCODE);//����pin codeģʽ
	if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTConfirmPairing(pkey,1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case4:�쳣����:��������
	NDK_BTSetPairingMode(PAIRING_MODE_SSP);//������ȷ��ģʽ
	if((ret=NDK_BTGetPairingStatus(NULL,&flag))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTGetPairingStatus(pkey,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTConfirmPairing(NULL,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:δ�����ϻ�ȡ״̬����0,(�ĵ���������״̬������0)
	if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(flag)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	//δ���ʱ����ȷ����Է���ʧ��
	if((ret=NDK_BTConfirmPairing(pkey,1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������case��˳��:��������ʧ�ܵ���ִ�����ӳɹ��ģ�����ʡȥȡ����ԵĶ���
	//case8:sspģʽ���ܹ������յ����豸���ֻ�����������󣬲�ȡ����Ժ�Ӧ�÷������ʧ��
	NDK_BTSetPairingMode(PAIRING_MODE_SSP);//����SSPģʽ
#if BTPCI_ENABLE
	cls_show_msg("�ֻ����������а�֮ǰ��Ե�����ȡ�������������"); //������PCI��֤�汾���ǹ��ڰ汾 ƻ���ֻ��������sspģʽ���Ժ� ��߶���Ҫ����ȡ�����
#endif	
	cls_printf("SSP PINģʽ:ʹ���ֻ�����������POS��������,�ֻ��˳���������ȡ�����");
	oldtime = time(NULL);
	while(1)
	{
		if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		NDK_SysMsDelay(50);
		switch(flag)
		{
			case 0:
				if((diff=time(NULL)-oldtime)>60)
				{
					send_result("line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
					goto ERR;
				}
				continue;
				break;
			case 1:
				//������ȷ��pkey����ȡ�����
				if((ret=NDK_BTConfirmPairing(pkey, 0))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				continue;
				break;
			case 2:
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
				break;
			case 3:
				cls_show_msg1(5,"ȡ����Գɹ�,����������ͨ��");
				break;
			default:
				send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
				goto ERR;
				break;
		}
		if(flag==3)
			break;
	}
	//case9: ȡ���������»�ȡ��״ֵ̬0(�ĵ���������״̬������0)
	if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(flag)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	//�Ͽ�����
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:sspģʽ���ܹ������յ����豸���ֻ�����������󣬲�������Ժ��ܹ�������Գɹ�
	cls_printf("SSP PINģʽ:ʹ���ֻ�����������POS��������,�ֻ��˳���������ȷ�����");
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
					send_result("line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
					goto ERR;
				}
				continue;
				break;
			case 1:
				//������ȷ��pkey���н������
				if((ret=NDK_BTConfirmPairing(pkey, 1))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				continue;
				break;
			case 2:
				cls_show_msg1(5,"������Գɹ�,����������ͨ��");
				break;
			case 3:
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
				break;
			default:
				send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
				goto ERR;
				break;
		}
		if(flag==2)
			break;
	}
	
	//case7: �����ӵ�����»�ȡ��״ֵ̬0(�ĵ���������״̬������0)
	if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(flag)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	//�Ͽ�����
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case11:PassKeyģʽ���ܹ������յ����豸���ֻ�����������󣬲�������ȷkey��ȡ����Ժ�Ӧ�÷������ʧ��
	//cls_show_msg("��֮ǰ��Գɹ�������ȡ����Ժ�,���������");	
	NDK_BTSetPairingMode(PAIRING_MODE_PASSKEY);//����PassKeyģʽ
	cls_printf("PassKeyģʽ:ʹ���ֻ�����������POS��������,�ֻ��˳��������");
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
					send_result("line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
					goto ERR;
				}
				continue;
				break;
			case 1:
				if(pkey[0]!='\0')
				{
					send_result("line %d:%s����ʧ��(%c)", __LINE__, TESTAPI, pkey[0]);
					goto ERR;
				}
				cls_printf("����ȷ�����ֻ�����ʾ�������:\n");
				if((ret=lib_kbgetinput(pkey, 0, 6, &len, INPUTDISP_PASSWD, 0, INPUT_CONTRL_LIMIT_ERETURN)) == NDK_ERR || ret == NDK_ERR_TIMEOUT)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto ERR;
				}
				if((ret=NDK_BTConfirmPairing(pkey, 0))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				continue;
				break;
			case 2:
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
				break;
			case 3:
				cls_show_msg1(5,"ȡ����Գɹ�,����������ͨ��");
				break;
			default:
				send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
				goto ERR;
				break;
		}
		if(flag==3)
			break;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case12:PassKeyģʽ���ܹ������յ����豸���ֻ�����������󣬲��������key�ͽ�����Ժ�Ӧ�÷������ʧ��
	//cls_show_msg("��֮ǰ��Գɹ�������ȡ����Ժ�,���������");	
	cls_printf("PassKeyģʽ:ʹ���ֻ�����������POS��������,�ֻ��˳��������");
	oldtime = time(NULL);
	while(1)
	{	
		if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		switch(flag)
		{
			case 0:
				if((diff=time(NULL)-oldtime)>60)
				{
					send_result("line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
					goto ERR;
				}
				continue;
				break;
			case 1:
				if(pkey[0]!='\0')
				{
					send_result("line %d:%s����ʧ��(%c)", __LINE__, TESTAPI, pkey[0]);
					goto ERR;
				}
				cls_printf("�����������ֻ�����ʾ�������:\n");
				if((ret=lib_kbgetinput(pkey, 0, 6, &len, INPUTDISP_PASSWD, 0, INPUT_CONTRL_LIMIT_ERETURN)) == NDK_ERR || ret == NDK_ERR_TIMEOUT)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto ERR;
				}
				if((ret=NDK_BTConfirmPairing(pkey, 1))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				continue;
				break;
			case 2:
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
				break;
			case 3:
				cls_show_msg1(5,"���ʧ�ܵ�����������ͨ��");
				break;
			default:
				send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
				goto ERR;
				break;
		}
		if(flag==3)
			break;
	}

	//case10:PassKeyģʽ���ܹ������յ����豸���ֻ�����������󣬲�������ȷkey�ͽ�����Ժ�Ӧ�÷�����Գɹ�
	//cls_show_msg("��֮ǰ��Գɹ�������ȡ����Ժ�,���������");	
	cls_printf("PassKeyģʽ:ʹ���ֻ�����������POS��������,�ֻ��˳���PASSKEY");
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
					send_result("line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
					goto ERR;
				}
				continue;
				break;
			case 1:
				if(pkey[0]!='\0')
				{
					send_result("line %d:%s����ʧ��(%c)", __LINE__, TESTAPI, pkey[0]);
					goto ERR;
				}
				cls_printf("����ȷ�����ֻ�����ʾ�������:\n");
				if((ret=lib_kbgetinput(pkey, 0, 6, &len, INPUTDISP_PASSWD, 0, INPUT_CONTRL_LIMIT_ERETURN)) == NDK_ERR || ret == NDK_ERR_TIMEOUT)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto ERR;
				}
				if((ret=NDK_BTConfirmPairing(pkey, 1))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}	
				continue;
				break;
			case 2:
				cls_show_msg1(5,"������Գɹ�,����������ͨ��");
				break;
			case 3:
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
				break;
			default:
				send_result("line %d:%s����ʧ��:��ȡ��δ֪״̬(%d)", __LINE__, TESTAPI, flag);
				goto ERR;
				break;
		}
		if(flag==2)
			break;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);	
ERR:
	NDK_BTSetPairingMode(g_pair_mode);
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

