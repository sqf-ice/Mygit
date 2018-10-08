/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt15.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetDiscoverableStatus�Ƿ������ȷ���ú��Ƿ�������������
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetDiscoverableStatus"
#define  MAXWAITTIME 10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt15
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20150205		created
*****************************************************************/
void bt15(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	/*private & local definition*/
	int ret = -1, status= -1;	  
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1:BM77����ģ�鲻֧�֣�����NDK_ERR_NOT_SUPPORT 20150415 jiangym
	if(g_at_enable)//BM77��֧��
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		
		cls_show_msg1(10,"������ģ�鲻֧��%s����", TESTAPI);
		return;
	}
/*
	//�����Ե���ֲ��bt.c��,��Ϊδ�򿪷���ʧ������δ��patchǰ����,֮��������ö��ܳɹ�  20150415 jiangym
	//case1:��������δ�� ����ʧ��
	if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_ERR_IOCTL)//0
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;
	}
*/
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	/*memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, outbuf);
		GOTOERR;
	}*/

	//case2:�������ò�Ϊ0��1��2Ӧ�÷���ʧ��
	if((ret=NDK_BTSetDiscoverableStatus(3))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTSetDiscoverableStatus(-1))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
//ME30MHǰ�������Ҫ���ÿ����㲥�����ú������û��ϵͳĬ�ϵ������
	if(!g_btdiscoverflag)
	{
		//case3:��������:ϵͳĬ��Ϊ��������
		if(cls_show_msg("���ֻ�������%s���������ȡ����Ժ�,�жϴ�ʱӦ����������%s,��[Enter],��[����]", BtName, BtName)!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}	
	//case4:��������:����Ϊ0 ���ܱ�������
	if((ret=NDK_BTSetDiscoverableStatus(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("��ʱ�ֻ�������������Ӧ��������������:%s,��[Enter],��[����]", BtName)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case5:����Ϊ1 �ܱ������� �����ӳɹ�����������շ�
	if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
	{
		send_result("line %d:%s����ʧ��(ret=%d,status=%d)", __LINE__, TESTAPI, ret, status);
		GOTOERR;
	}
	if((ret=bt_comm_once(12,0))!=NDK_OK)//��ʱ������disconnect,�����ֻ��˹��߻�ԭ����Ե�ȡ�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("��BTFWATS�����а�del(�������߽���:�Ͽ����ӵ���ȡ�����)������,�����������");

	//case6:����2,����ֻ�ܱ�����Ե��ֻ����������ܽ����շ�����,δ��Թ����ֻ������ѵ�,ƻ���ֻ�������Ը��ʹʻ���������
	if((ret=NDK_BTSetDiscoverableStatus(2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if defined ME20 //����ME20����2������0��һ����Ч�� ��������
	if(cls_show_msg("ʹ�ô��ֻ�������������%sӦ����������,��[Enter],��[����]", BtName)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#else
	//��ʱʹ������Թ����ֻ�Ӧ�ÿ���ֱ����������������ͨѶ
	if(cls_show_msg("ʹ�ð�׿�ֻ���[Enter],ʹ��ƻ���ֻ���[����]")==ENTER)
	{
		cls_show_msg("ʹ�ô˰�׿�ֻ�����������,��%s���Ӻ�,���������", BtName);
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
	}
	else
	{
		if(cls_show_msg("ʹ�ô�ƻ���ֻ�������������%sӦ����������,��[Enter],��[����]", BtName)!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	if(cls_show_msg("��ʱʹ��δ��Թ����ֻ�������������%sӦ����������,��[Enter],��[����]", BtName)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif
	if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7:�������ڹر���Ҳ�����óɹ�(δ��patchʱ��һ��δ�������Ż᷵��ʧ��,֮���ܷ��سɹ���)
	if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	

	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	//�ָ�Ĭ������
	NDK_BTSetDiscoverableStatus(1);
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


