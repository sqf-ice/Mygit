/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt18.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetMaxBondedDevices �ܷ���ȷ���ü�ס�ֻ��ĸ���
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetMaxBondedDevices"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt18
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20150330		created
*****************************************************************/
void bt18(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	/*private & local definition*/
	int ret = -1,i = 0;	  
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	if(cls_show_msg("�״ν���˰���,ѡ����[Enter],���������˰���ѡ��[����]")==ENTER)
	{
		//case1:��������δ��,����������¼���ֻ���Ϣ��ĿӦ�÷���ʧ��
		/*���ڿ������ͣ������ڵ�һ���������ڴ�֮ǰ���ԣ�����Ѿ���ֲ��bt.c��*/
/*		if((ret=NDK_BTSetMaxBondedDevices(0))!=NDK_ERR_IOCTL)
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

		//case2:��������Ӧ�÷���ʧ��
		if((ret=NDK_BTSetMaxBondedDevices(6))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_BTSetMaxBondedDevices(-1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case3:�������� :����¼�ֻ�����
		if((ret=NDK_BTSetMaxBondedDevices(0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_BTDisconnect();//pos�˶Ͽ�����
		if(cls_show_msg("�ֻ���pos��������ʱ����ԶԻ���,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);				
			GOTOERR;
		}	
		/*
		�˴�����0������֤��������Ҫ������Ż������� 
		ԭ��������ģ���¼�����һ�����ӵ��ֻ� �����ٴ�����ʱ���������ԶԻ���
		����Ҫ����������ģ��ͰѼ�¼���ֻ�ɾ����,�����õ�0����K21��������ģ��
		��ʱ�ֻ���ȥ��posʱ�ͻ������ԶԻ�����
		 ####�Ƿ�������0����֤��??####
		*/
		cls_show_msg("�����������ٴ�����ʱ�������,���������");
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_BTDisconnect();
		if(cls_show_msg("�ֻ���pos��������ʱ����ԶԻ���,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);				
			GOTOERR;
		}

		//case4:��������:��¼�ֻ�����Ϊ5
		if((ret=NDK_BTSetMaxBondedDevices(5))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg("׼���岿�ֻ�(�ɰ�׿��ƻ���ɻ��)�ֱ���pos��������,���������");
		for(i=0;i<5;i++)
		{
			cls_show_msg1(3, "ʹ�õ�%d���ֻ������...", i+1);
			if((ret=lib_bt_pairing(1))!=NDK_OK)
			{
				send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			NDK_BTDisconnect(); 
		}
		if(cls_show_msg("�Ƿ�ÿ���ֻ���pos��������ʱ������ԶԻ���,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);				
			GOTOERR;
		}	
		cls_show_msg("���������ν������岿�ֻ�����������,���ӳɹ������������(��һ̨�ɹ���Ͽ����Ӻ������һ̨����)");	
		if(cls_show_msg("5���ֻ���������ܹ�ֱ������pos,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}	
		//ʹ�õ������ֻ���������ʱ��Ҫ���
		cls_show_msg("ʹ��δ��pos������ӹ����µ��ֻ���pos������������,���������");
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_BTDisconnect();
		if(cls_show_msg("�µ��ֻ���pos��������ʱ����ԶԻ���,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}	
		
		//case5:��ʱ���ӵĵ�һ���ֻ��ٽ�������ʱҲ��Ҫ���
		cls_show_msg("ʹ����ǰ���ӵĵ�һ���ֻ���pos������������,���������");
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_BTDisconnect();
		if(cls_show_msg("��һ���ֻ���pos��������ʱ����ԶԻ���,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}	
		cls_show_msg("pos�������ٽ���˰������е�һ�����ڶ����ֻ�����������,���������");
		if(cls_show_msg("�Ƿ�����pos,��[Enter],��[����]")==ENTER)
		{
			NDK_SysReboot();
		}
	}
	
	NDK_PortClose(PORT_NUM_BT);
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:��������֤��һ��������ԣ��ڶ�����Ҫ���
	cls_show_msg("��ʹ����ǰ���ӵĵ�һ���ֻ���pos�����������Ӻ�,���������");
	if(cls_show_msg("�˲��ֻ��������ֱ������pos,��[Enter],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	
	NDK_BTDisconnect();
	//�������ӵڶ����ֻ������
	cls_show_msg("ʹ����ǰ���ӵĵڶ����ֻ���pos������������,���������");
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("�˲��ֻ���pos��������ʱ����ԶԻ���,��[Enter],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}	

	send_result("%s����ͨ��", TESTAPI);
	
ERR:
	NDK_PortClose(PORT_NUM_BT);
	return;
}


