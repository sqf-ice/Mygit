/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth2.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_EthGetMacAddr�ӿڹ���
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_EthGetMacAddr"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  						chensl		   20130105	 		created
*****************************************************************/
void eth2(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	char MacAddr[18] = {0};

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:����Ƿ�����NULL
	if ((ret=NDK_EthGetMacAddr(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:����ʹ�ò���
	memset(MacAddr, 0, sizeof(MacAddr));
	if ((ret=NDK_EthGetMacAddr(MacAddr)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (strlen(MacAddr) != 17)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, strlen(MacAddr));
		RETURN;
	}
	/*
	for(i=0;i<16;)
	{		
		macBuf[j++] = (MacAddr[i]-'0')*16 + (MacAddr[i+1]-'0');
		i+=3;
	}
	*/

	//���Խ���
	//send_result("%s����ͨ��!(mac:%03d:%03d:%03d:%03d:%03d:%03d),���[�Լ�]�˶��Ƿ�һ��!", TESTAPI, macBuf[0], macBuf[1], macBuf[2], macBuf[3], macBuf[4], macBuf[5]);
	send_result("%s����ͨ��!(mac:%s),���[�Լ�]�˶��Ƿ�һ��!", TESTAPI, MacAddr);
	return;
}

