/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt31.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ��������NDK_BTMasterConnect�������Ӽ���������(����汾)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterConnect"
#define MAXWAITTIME  10000  //��λms  ��10s
#define TIMEOUT  2  //2s
#define	WUCHASEC	0.03 //���ֵ����

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt31
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  
*****************************************************************/
void bt31(void)	 
{
	/*private & local definition*/
	int ret=0,status=0,i=0;
	/*������Ҫȥ��ȡ�ֻ���mac ����
	mac1Ϊ����2�ֻ�������mac  0x20,0x82,0xc0,0xc5,0x35,0x0c,     mac2Ϊ���Լ��ֻ���Mac 0x60,0xA3,0x7D,0x1E,0xC2,0x20
	mac ����д0x0c,0x35,0xc5,0xc0,0x82,0x20     iphone mac  0xD9,0x1C,0x49,0xAF,0x3B,0xBC*/
	char ph_mac1[6]={0x0c,0x35,0xc5,0xc0,0x82,0x20},ph_mac2[6]={0x20,0xC2,0x1E,0x7D,0xA3,0x60};
	char err_mac[5]={0x00,0x11,0x22,0x33,0x44},unpair_mac[6]={0x00,0x11,0x22,0x33,0x44,0x55};
	float tmp = 0.0;
	char str[32] = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1:��������δ�򿪣���������ʧ�ܣ���ȡ��������״̬ʧ��
	if((ret=NDK_BTMasterConnect(ph_mac1,1,MAXWAITTIME))!=NDK_ERR_OPEN_DEV)//-1
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
	
	//case2:��������һ��δ��Ե�mac��ַ���豸 Ӧ�÷���ʧ��
	if((ret=NDK_BTMasterConnect(unpair_mac,0,MAXWAITTIME))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	/* 
	�����ӳɹ���ǰ������:
	�����ü�¼�ֻ�����Ϊ2,Ȼ��ʹ��2���ֻ��ֱ���pos�������,��¼��2���ֻ���mac   
	*/
	if((ret=NDK_BTSetMaxBondedDevices(2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("׼��2���ֻ��ֱ���pos��������,���������");
	for(i=0;i<2;i++)
	{
		cls_show_msg1(3, "ʹ�õ�%d���ֻ������...", i+1);//����ʹ�õĹ�����Ҫ��֤����ȷ��
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_BTDisconnect(); 
	}
	
	//case3:�����Ƿ����԰���������֧�ֺͲ��������Լ�mac��ʽ����
	if((ret=NDK_BTMasterConnect(ph_mac1,2,MAXWAITTIME))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterConnect(ph_mac1,-1,MAXWAITTIME))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterConnect(ph_mac1,3,MAXWAITTIME))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterConnect(err_mac,1,MAXWAITTIME))!=NDK_ERR ) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case4:����mac����ΪNULL,���������һ�����ӵ��ֻ�(Ԥ��Ҳ�����ϵڶ����ֻ�)
	if((ret=NDK_BTMasterConnect(NULL,0,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//�ж�״̬��������״̬
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
	//��Ҫ�����˹��ж��Ƿ����ӵ��ǵڶ����ֻ�
	if(cls_show_msg("��pos�����ϵ��ֻ��Ƿ�Ϊ�ڶ����ֻ�,��[Enter],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ж�״̬��δ����״̬
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	
	//case5:��������һ����ȷ��mac��ַ���豸(macΪ��һ���ֻ�)Ӧ���ܹ����ӳɹ�
	//������һ���ֻ�
	if((ret=NDK_BTMasterConnect(ph_mac1,1,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//�ж�״̬��������״̬
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
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:��ʱʱ��̫�̵������ �����Ƿ�᲻�ɹ�,�������ó�ʱʱ��Ϊ0
	if((ret=NDK_BTMasterConnect(NULL,1,0))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case7:��ʱ���ԣ�׼ȷ�Բ��� (���ֻ�����)
	cls_show_msg("�ѵڶ����ֻ���Զ�����������...");
	lib_StartStopwatch();
	if(((ret=NDK_BTMasterConnect(ph_mac2,0,TIMEOUT*1000))!=NDK_ERR_TIMEOUT)||((tmp=lib_ReadStopwatch()-TIMEOUT)>WUCHASEC))
	{
		memset(str,0,sizeof(str));
		ftos(tmp,str);
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, str);
		GOTOERR;	
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

