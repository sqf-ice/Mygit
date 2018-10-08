/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name			: sckt8.c
* Author 			:
* version			: 
* DATE				: 
* directory 	: 
* description	: ����NDK_TcpReset�ӿڹ���
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
#define 	TESTAPI		"NDK_TcpReset"
#define		MAXWAITTIME 5	//���յȴ�ʱ��

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
void sckt8(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	uint unSckt = 0;

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//����ǰ��(�²�����Ҫ�Ƚ���)
	if(NetUp()!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		return;
	}
		
	//case1:��һ��sock��������Ӻ������ر�
	if((ret=NDK_TcpOpen(&unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if K21_ENABLE
	if((ret=lib_tcp_checkstatus(unSckt, MAXWAITTIME))!=SOCKET_STATUS_CONNECT)
	{
		send_result("line %d:%sTCP��ȡ����ʧ��(%d))", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if((ret=NDK_TcpReset(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case2:�رվ����������Ӧ��ʧ��
#if K21_ENABLE
	if((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR_LINUX_TCP_NOT_OPEN)
#else
	if((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:�쳣����
	//case3.1�ر��ѹرվ����Ӧʧ�ܷ���
	if((ret=NDK_TcpReset(unSckt)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case3.2�ر�һ���յľ����Ӧʧ�ܷ���
	if((ret=NDK_TcpReset(-1)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_TcpClose(unSckt);
	NDK_TcpWait(unSckt);
	NetDown();
	return;
}

