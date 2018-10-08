/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name			: sckt9.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_TcpCheck��ȡtcp״̬
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_TcpCheck"	
#define		MAXWAITTIME 60	//���յȴ�ʱ��
#if K21_ENABLE
#define		MAX_SIZE	2047//�Ͷ�����֧��2047
#else
#define		MAX_SIZE	5678
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 				: void wifi6(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			   zhengry      20160321    created 		 
*****************************************************************/
void sckt9(void)
{
	/*private & local definition*/
	int ret=0, errcode=0, j=0;
	char ErrIp[16] = "192.168.4.155";
	uint unSckt = 0, sendLen = 0, recvLen = 0;
	int tcp_status;
	ushort usPort = 3400+rand()%256;	
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//����ǰ��(�²�����Ҫ�Ƚ���)
	if(NetUp() != SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		return;
	}
	if((ret = NDK_TcpOpen(&unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_TcpBind(unSckt, netsetting.LocalIP.Str, usPort)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7:�쳣����:δ����NDK_NetAddRouterTable,�᷵������ӿڣ�wifi ����PPP������NDK_ERR_TCP_NETWORK.Ŀǰ�ݲ��ܲ�
	
	//case1:����δ�������ӷ���������,��ȡ״̬Ӧ����δ����SOCKET_STATUS_DISCONNECT
	if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode) ) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(tcp_status != SOCKET_STATUS_DISCONNECT)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���ӷ�����
	if((ret = NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2:�����쳣����,�������ղ���ͬʱΪNULL�ŷ���NDK_ERR_PARA
	if((ret = NDK_TcpCheck(unSckt, NULL, NULL) ) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		GOTOERR;  
	}
	
	//case3:socket�������,��ȡTCP״̬Ӧ����NDK_ERR_LINUX_TCP_NOT_OPEN
	if((ret = NDK_TcpCheck(unSckt+1, &tcp_status, &errcode) ) != NDK_ERR_LINUX_TCP_NOT_OPEN)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

 	//case4:��������,���Ի�ȡ״̬Ϊ������SOCKET_STATUS_CONNECT
	lib_StartStopwatch();
	while(1)
	{
		if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(tcp_status == SOCKET_STATUS_CONNECT)
			break;		
		if(tcp_status!=SOCKET_STATUS_CONNECTING)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
			goto ERR;
		}
		if(lib_ReadStopwatch()>MAXWAITTIME)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
			goto ERR;
		}
	}

	//case5:���ʹ�����,��ȡ״̬Ӧ�������ڷ���SOCKET_STATUS_READY_TO_SEND,������ɺ󷵻�SOCKET_STATUS_SEND_OVER
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0   //����ʱ��̫�� ��׽����SOCKET_STATUS_READY_TO_SEND���״̬,�ݲ�����
	lib_StartStopwatch();
	while(1)
	{
		if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode) ) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(tcp_status == SOCKET_STATUS_READY_TO_SEND)
			break;	
		if(lib_ReadStopwatch()>MAXWAITTIME)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
			goto ERR; 
		}
	}
#endif
	if(sendLen != sizeof(sendBuf))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, sendLen, sizeof(sendBuf));
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret=NDK_TcpRead(unSckt, recvBuf, sendLen, MAXWAITTIME, &recvLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(recvLen != sendLen || memcmp(sendBuf, recvBuf, recvLen))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR; 
	}
	if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode) ) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0//(defined ME50NX||defined ME68)&&getlinktype()!=GPRS  //����ƽ̨��֧��SOCKET_STATUS_SEND_OVER״̬  20180416 sull modify
	//����ƽ̨��֧���˸�״̬  20180716 modify
	if(tcp_status != SOCKET_STATUS_CONNECT)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
		GOTOERR;
	}
#else
	if(tcp_status != SOCKET_STATUS_SEND_OVER)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
		GOTOERR;
	}
#endif
	if((ret=NDK_TcpClose(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8:�ر�TCP���ͨ����,���û�ȡTCP״̬Ӧ�÷���NDK_ERR_LINUX_TCP_NOT_OPEN socket�������
	if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode) ) != NDK_ERR_LINUX_TCP_NOT_OPEN)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}

	//case6:���Ӵ���IP�ķ�����,����TcpConnect�Ƿ�����ʽ,Ӧ���سɹ�,��ȡTCP״̬ʱ�ȷ�����������SOCKET_STATUS_CONNECTING,���Ӧ�÷�������ʧ��SOCKET_STATUS_DISCONNECT
	if((ret = NDK_TcpOpen(&unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	usPort=3656+rand()%256;//���ڹر�TCPͨ�����Ѹĳɷ�����ʽ,������´򿪰���ͬ�������ͬIP����ͬ�˿ںŻ�δ�����ͷ� 20160325
	if((ret = NDK_TcpBind(unSckt, netsetting.LocalIP.Str, usPort)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}
	if((ret = NDK_TcpConnect(unSckt, ErrIp, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_OK)

	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode) ) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
 	if(tcp_status != SOCKET_STATUS_CONNECTING)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
		GOTOERR;
	}
	lib_StartStopwatch();
	while(1)
	{
		if((ret = NDK_TcpCheck(unSckt, &tcp_status, &errcode) ) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(tcp_status == SOCKET_STATUS_DISCONNECT)
			break;		
		if( tcp_status != SOCKET_STATUS_CONNECTING)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
			goto ERR;
		}
		if(lib_ReadStopwatch()>MAXWAITTIME)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, tcp_status);
			goto ERR;
		}
	}
	if((ret=NDK_TcpClose(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case8:NDK_ERR_OPEN_DEV:socket������ʧ�ܺ�NDK_ERR_IOCTL����������ʧ����ʱ�޷�����
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_TcpClose(unSckt);
	NDK_TcpWait(unSckt);
	NetDown();
	return;
}

