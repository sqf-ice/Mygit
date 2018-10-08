/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt12.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetLocalMAC�Ƿ������������������MAC��ַ
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetLocalMAC"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt12
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20141118		created
*****************************************************************/
void bt12(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	char  inbuf[20] = {0x30,0x31,0x32,0x33,0x34,0x35}, outbuf[20] = {0};
	char  inbuf1[20] = {0x35,0x34,0x33,0x32,0x31,0x30}, oldbuf[20] = {0};
	int  ret = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1:BM77����ģ�鲻֧�����ñ���MAC������NDK_ERR_NOT_SUPPORT
	if(g_at_enable)
	{
		if((ret=NDK_BTSetLocalMAC(inbuf))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		send_result("������ģ�鲻֧��%s����", TESTAPI);
		return;
	}

#if !K21_ENABLE
	//case1:��������δ�򿪽�������MAC��Ӧ��ʧ�ܷ���NDK_ERR_OPEN_DEV
	if((ret=NDK_BTSetLocalMAC(inbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//����������:��ȡԭ����mac��ַ
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(oldbuf,0,sizeof(oldbuf));
	if((ret=NDK_BTGetLocalMAC(oldbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case 2:�쳣����:�������õ�mac����ΪNULL
	if((ret=NDK_BTSetLocalMAC(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:������������:��ȡ��Ϊ���һ�����õ�ֵ
	if((ret=NDK_BTSetLocalMAC("112233"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTSetLocalMAC(inbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�������flash���·�������������������open������MAC��ɾ����Ƶ����� 20160728 linwl
	NDK_PortClose(PORT_NUM_BT);
	NDK_PortOpen(PORT_NUM_BT, NULL);
	NDK_BTEnterCommand();
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalMAC(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(outbuf,inbuf,6))
	{
		send_result("line %d:%s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, outbuf[5],outbuf[4],outbuf[3],outbuf[2],outbuf[1],outbuf[0]);
		GOTOERR;
	}

	//case4:�������� mac:35:34:33:32:31:30
	if((ret=NDK_BTSetLocalMAC(inbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalMAC(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(outbuf,inbuf1,6))
	{
		send_result("line %d:%s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, outbuf[5],outbuf[4],outbuf[3],outbuf[2],outbuf[1],outbuf[0]);
		goto ERR;
	}

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_BTSetLocalMAC(oldbuf);
	NDK_PortClose(PORT_NUM_BT);
	return;
}

