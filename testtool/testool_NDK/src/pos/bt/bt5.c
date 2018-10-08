/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt5.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetPIN,NDK_BTGetPIN�Ƿ�����������û�ȡPIN
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetPIN,NDK_BTGetPIN"
#define   MAXPIN    	6
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt5
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt5(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}

	/*private & local definition*/
	char *inbuf[] ={"654321", 
					"123",
					"ne#1^3"};
	char abnorname[MAXPIN+3] = "1234567";
	char outbuf[MAXPIN+1] = {0}, oldbuf[MAXPIN+1] = {0};
	int ret = -1, i=0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
#if BTPCI_ENABLE
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR3;
	}
	if((ret=NDK_BTSetPIN(inbuf[0]))!=NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_BTGetPIN(oldbuf))!=NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	send_result("PCI�汾������֧��%s,����ͨ��", TESTAPI);
	return;
#endif

#if !K21_ENABLE
	//case1:��������δ�򿪽�������PINӦ��ʧ��
	if((ret=NDK_BTSetPIN(inbuf[0]))!=NDK_ERR_OPEN_DEV)//linwl ��inbuf�޸ĳ�inbuf[0] 20140603 ȥ�����뾯��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_BTGetPIN(outbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR3;
	}
	
	if(g_at_enable)//BM77
	{	
		//case3:δ��������ģʽ���û�ȡ��ʧ��
		if((ret=NDK_BTSetPIN(inbuf[0]))!=NDK_ERR_TIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR3;
		}
		if((ret=NDK_BTGetPIN(outbuf))!=NDK_ERR_TIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR3;
		}
	}
	
	//����ǰ��:��������ģʽ����ܽ��л�ȡĬ�ϵ�PIN
	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	//��ȡ��ǰ��PIN
	memset(oldbuf,0,sizeof(oldbuf));
	if((ret=NDK_BTGetPIN(oldbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR2;
	}
	
	//case 4:�쳣����
	//case 4.1:pinstr=NULL�����
	if((ret=NDK_BTSetPIN(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTGetPIN(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case 4.2:PIN���ȳ���6�ֽ�
	if((ret=NDK_BTSetPIN(abnorname))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case 5:���Բ�ͬ���õ�PIN�����,��������С��6,������ĸ�Լ�����6λ����
	for(i=0;i<SZ_ARRAY(inbuf);i++)
	{
		if((ret=NDK_BTSetPIN(inbuf[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//�������flash���·�������������������open������PIN��ɾ����Ƶ����� 20160728 linwl
		NDK_PortClose(PORT_NUM_BT);
		NDK_PortOpen(PORT_NUM_BT, NULL);
		NDK_BTEnterCommand();
		memset(outbuf,0,sizeof(outbuf));
		if((ret=NDK_BTGetPIN(outbuf))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(strcmp(outbuf, inbuf[i]))
		{
			send_result("line %d:%s����ʧ��%s,%s", __LINE__, TESTAPI, outbuf, inbuf[i]);
			GOTOERR;
		}
	}
	
	//case6:������һ��pin,�����ø�pin������ȡ���Ƿ��ǵڶ������õ� 20140603 added by jiangym
	if((ret=NDK_BTSetPIN(inbuf[0]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTSetPIN(inbuf[1]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetPIN(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(outbuf, inbuf[1]))
	{
		send_result("line %d:%s����ʧ��%s,%s", __LINE__, TESTAPI, outbuf, inbuf[1]);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_BTSetPIN(oldbuf);
ERR2:
	NDK_BTExitCommand();
ERR3:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


