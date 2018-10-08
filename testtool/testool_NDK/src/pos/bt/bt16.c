/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt16.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTMfiParamSet�Ƿ������ȷ���� ��Ӧ��mfi����
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTMfiParamSet"
#define  MAXLEN  63
#define  MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt16
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20150318		created
*****************************************************************/
void bt16(void)
{
	/*private & local definition*/
	int ret = -1, i = 0;	  
	char ab_len_para[MAXLEN+2] = {"abnormal_length_param_1234567890_abnormal_length_param_1234567890"};
	char outbuf[16] = {0};
	char *IphoneApp[] = {"CardEase Mobile", 
					    "NEWLAND", 
					    "NLBluetoothTest"}; //���Ե�����iphone App 
	/* factory����Ŀǰ�����ÿ�������,��������ʾ���������ַ������Ļ���ʾΪnull   */
	/*factoryӰ���ֻ����ʱ�����appʱ�����ʾ���ֿ��Բ�֧������*/
	char *factory[] =  {"`~!@#$%^&*() -_=+][{}:;����|\\?/><.,", //�����ַ� 
					    "NEWLAND",//Ӣ��  
					    "NLBluetoothTest"};//����Ӣ��  
	char *bundleid[] = {"com.creditcall.CardEaseMobile",
						"com.newland.mpos",
						"com.newland.inhouse.NLBluetoothTest"};
	char *protocol[] = {"com.thyron",
						"com.newland.payment",
						"com.ne.payment"};
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//δ��������������Ӧ�÷���ʧ�� 
	/*�˴�����bug,�᷵�سɹ�,��˱���ֱ���˳�,����Ӱ�����Ĳ���Ч��*/
	if((ret=NDK_BTMfiParamSet(factory[2],protocol[2],bundleid[2]))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
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

	//case1:��������ΪNULL Ӧ�÷���ʧ��
	if((ret=NDK_BTMfiParamSet(NULL,protocol[1],bundleid[1]))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMfiParamSet(factory[1],NULL,bundleid[1]))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMfiParamSet(factory[1],protocol[1],NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�������ó��ȳ���64�ֽ� Ӧ�÷���ʧ��
	if((ret=NDK_BTMfiParamSet(ab_len_para,protocol[1],bundleid[1]))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMfiParamSet(factory[1],ab_len_para,bundleid[1]))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTMfiParamSet(factory[1],protocol[1],ab_len_para))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	/******���� case4�����óɹ�  Ӱ����case5(�ǲ�Ĭ�����������)�Ĳ��Թ��ᵽǰ��*******/
	//case 5:��������:���� û������ΪĬ�ϵ�NLBluetoothTest(���������󲻱���֮ǰ����,�ʲ�����ʹ��Ĭ�ϵ�)
	//������Ĭ�ϵ�factoryΪNEWLAND	��ʾ������"NLBluetoothTest"����NEWLAND "��������"����
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("��ʱ�ֻ�����\"NLBluetoothTest\"����NEWLAND\"%s\"ͨ��,��[Enter]����������,��[����]",outbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//cls_show_msg("NLBluetoothTest������%s�����������",outbuf);
	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(g_pair_mode==2||g_pair_mode==3) 
	{
		cls_show_msg("���ֻ������а�����Ե������豸���Դ��豸�����������");
	}
	
	/*�˴�����bug,case3 case4���᷵��ʧ�ܾ��ܹ����óɹ�*/
	/*case3 case4��������:ֻҪ������ʽû�����⣬������������Ƿ���ȷ�������Ƿ���ȷ��Ӧ�����аѿ�*/
	//case3:id����Э�����ò��ǹ涨��Ӧ��Ҫ���ز���ʧ��----��Ϊ���سɹ�
	//idʹ�������ַ�
	if((ret=NDK_BTMfiParamSet(factory[2],protocol[1],factory[0]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Э��ʹ�������ַ�
	if((ret=NDK_BTMfiParamSet(factory[2],factory[0],bundleid[1]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:�쳣���:����id��Э�鲻һ��ʱӦ��Ҫ����ʧ��---��Ϊ���سɹ�
	if((ret=NDK_BTMfiParamSet(factory[2],protocol[0],bundleid[2]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�ֱ���������ģʽ���в���
	for(i=0;i<3;i++)
	{
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_BTMfiParamSet(factory[i],protocol[i],bundleid[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg("ע���ֻ��˵������ʾ���ݲ������ж�,���������");
		if(cls_show_msg("�ֻ��Ƿ���ʾ\"%s\"����%s\"%s\"ͨ��,��[Enter],��[����]",IphoneApp[i],factory[i],outbuf)!=ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(g_pair_mode==2||g_pair_mode==3) 
		{
			cls_show_msg("���ֻ������а�����Ե������豸���Դ��豸�����������");
		}
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	//����Ĭ�ϵ�MFI����
	NDK_BTMfiParamSet(factory[2],protocol[2],bundleid[2]);
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


