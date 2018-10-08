/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt7.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_PortOpen�ܷ��ʼ��������,NDK_PortClose�ܷ������ر�����
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"��������"
#define   MAXWAITTIME		10
#define   BPS_NUM		8
#define   MAX_SIZE    BUFSIZE_BT-MIN_BTCMD_LEN

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt7
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt7(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1;
#if !K21_ENABLE
	int j = 0;
	char *bps[BPS_NUM] ={ "2400", "4800", "9600", "19200", "38400", "57600", "115200", "230400"};//"300", "1200",
#endif

	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

#if !K21_ENABLE
	//δ����������ʹ���ֻ��������ֽ�������POSӦ�Ѳ���
	//ǰ������Ҫ��֮ǰ��Ե�ȡ��,ûȡ����ʹ������Ҳ������
	/*//�������Ƶ�systest57 abnormal_test6 20151217 linwl
	cls_show_msg("����ǰ����Ե�POSȡ��,ȡ�������������");
	if(cls_show_msg("ʹ���ֻ���������%s,Ӧ��������,��ȷ[Enter],����[����]",BtName)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}*/
	//case1:Ӧ�ṩ���������ڲ��������ù���
	cls_printf("�����������ڲ�����������");
	for (; j<BPS_NUM; j++)
	{
		if((ret=NDK_PortOpen(PORT_NUM_BT, bps[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if((ret = NDK_PortClose(PORT_NUM_BT)) != NDK_OK)
		{				
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
#endif

#if K21_ENABLE
	//case2:�쳣��������(�˴���Ч)
	if((ret=NDK_PortOpen(PORT_NUM_BT, "115201"))!=NDK_OK)
#else
	if((ret=NDK_PortOpen(PORT_NUM_BT, "115201"))!=NDK_ERR_PARA)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_BT);
	
	//case3:�������ԣ��򿪺���Գɹ���д����
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
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
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

