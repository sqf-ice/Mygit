/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name		: sckt7.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_UdpOpen�ӿڹ���
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
#define 	TESTAPI		"NDK_UdpOpen"
#define		MAXNUM	4

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
* history 		 		: 
*****************************************************************/
void sckt7(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, i = 0;
	uint unSckt[MAXNUM] = {0};
	
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:����Ƿ�����	
	if((ret=NDK_UdpOpen(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:����ʹ�ò��ԣ��򿪶��(MAXNUM��)socket���ԣ���MAXNUM��UDP
	for(i=0; i<MAXNUM; i++)
	{
		if((ret=NDK_UdpOpen(&unSckt[i])) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
	}
	
	//case3:�ر�socket
	for(i=0; i<MAXNUM; i++)//����UDP�Ĺر�Ҳ��TCP�ĺ�����
	{
		if((ret=NDK_TcpClose(unSckt[i])) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}		
		if((ret=NDK_TcpWait(unSckt[i])) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			goto ERR;
		}
	}
		
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:	
	for(i=0; i<MAXNUM; i++)
	{
		NDK_TcpClose(unSckt[i]);
		NDK_TcpWait(unSckt[i]);
	}
	return;
}

