/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt2.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTGetLocalMAC�Ƿ����������ȡ������MAC��ַ
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTGetLocalMAC"
#define   MAXLEN	128

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt4
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt4(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}

	/*private & local definition*/
	char  outbuf[20] = {0};
	int  ret = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
#if !K21_ENABLE
	//case1:��������δ�򿪽��л�ȡMAC��Ӧ��ʧ��
	if((ret=NDK_BTGetLocalMAC(outbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:����%sʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif 	

	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	if(g_at_enable)//BM77
	{	
		//case3:δ��������ģʽ,���ܻ�ȡ��Mac
		if((ret=NDK_BTGetLocalMAC(outbuf))!=NDK_ERR_TIMEOUT)
		{
			send_result("line %d:����%sʧ��(%d)(%s)", __LINE__, TESTAPI, ret, outbuf);
			GOTOERR2;
		}
	}
	
	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR2;
	}
	
	//case 4:�쳣����:����mac����ΪNULL
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalMAC(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:����%sʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case 5:��������
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalMAC(outbuf))!=NDK_OK)
	{
		send_result("line %d:����%sʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	//��ʹ�ô˷�ʽ���
	send_result("%s�������,mac��ַ:%02x:%02x:%02x:%02x:%02x:%02x�����ֻ�ɨ�赽������MAC�˶��Ƿ�һ��!", TESTAPI, outbuf[5], outbuf[4], outbuf[3], outbuf[2], outbuf[1], outbuf[0]);//�����Լ��Ӧ����ʾMAC�ķ�ʽһ��,����ʹ���Լ��޷�ʶ�����÷������,�����ֻ�ɨ���MAC������Ƚ�
ERR:
	NDK_BTExitCommand();
ERR2:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


