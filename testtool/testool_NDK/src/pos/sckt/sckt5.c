/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name		: sckt5.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_TcpWrite�ӿڹ���
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
#define 	TESTAPI		"NDK_TcpWrite"
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
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: 
*****************************************************************/
void sckt5(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, j = 0;
	uint unSckt = 0, sendLen = 0, recvLen = 0, sendLen1 = 0, sendLen2 = 0;
	ushort usPort = 3400+rand()%256;	
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//����ǰ��(�²�����Ҫ�Ƚ���)
	if(NetUp()!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		return;
	}
	
	//��socket������
	if((ret=NDK_TcpOpen(&unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_TcpBind(unSckt, netsetting.LocalIP.Str, usPort)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1:����δ���ӣ�����д����Ӧ��ʧ�ܣ��Ͷ˷���NDK_ERR_TCP_NETWORK
	for(j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
#if K21_ENABLE
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen))  != NDK_ERR_TCP_NETWORK)
#else
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_ERR_TCP_SEND)
#endif
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
	//case2:����Ƿ����������в����ķǷ����Զ�Ҫ����
	if(((ret=NDK_TcpWrite(-1/*0*/, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) ==NDK_OK/*!= NDK_ERR_TCP_SEND*/)//�ж˷���NDK_ERR_TCP_SEND,�Ͷ˷���NDK_ERR_LINUX_TCP_NOT_OPEN,�ʷſ��ж�����
		||((ret1=NDK_TcpWrite(unSckt, NULL, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_ERR_PARA)
		//||((ret2=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, NULL)) != NDK_OK)//�޸ĳ�case6 20160325 zhengry
	   )
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
	
	//case6:���ͳ��ȴ�NULLʱ,��������Ӧ���ܳɹ�
#if !(defined ME50NX||defined ME68) //ME50Nx���һ��������NULLʱ����NDK_ERR_PARA �ʴ�case���� 20171124 sull
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sendLen=sizeof(sendBuf);
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
#endif

	//case3:����ʹ�ò���(������)
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
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

	//case5:�����������Էִ�(����)�������ݣ�һ�ν���linwl20130617
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf)/2, MAXWAITTIME, &sendLen1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(sendLen1 != sizeof(sendBuf)/2)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, sendLen1, sizeof(sendBuf)/2);
		GOTOERR;
	}
	if((ret=NDK_TcpWrite(unSckt, sendBuf+sizeof(sendBuf)/2, sizeof(sendBuf)/2, MAXWAITTIME, &sendLen2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(sendLen2 != sizeof(sendBuf)/2)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, sendLen2, sizeof(sendBuf)/2);
		GOTOERR;
	}
	sendLen=sendLen1+sendLen2;
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
#if ETH_ENABLE||WLAN_ENABLE//����ģ���޷�������߶���,������
	if(auto_flag==0||auto_flag==2)
	{
		//case7:�Ͽ�����������·,�Կ��Խ��з�������
		cls_show_msg1(MAXWAITTIME, "��Ͽ���̫����������,���������...");
		if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(MAXWAITTIME, "�������̫����������,���������...");
		//�Ͷ�WiFiģ����Ҫ��������,�����Ӱ��6������,������ƷҲ����Ӧ��Ҳ��Ӱ�� 20180705 add
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_TcpRead(unSckt,recvBuf,sendLen,MAXWAITTIME,&recvLen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	else
		send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
#endif	
	//case4:�ر�socket��д�ѹرյ�socketӦʧ��
	if((ret=NDK_TcpClose(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_TcpWait(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if K21_ENABLE  //�Ͷ˷���NDK_ERR_LINUX_TCP_NOT_OPEN
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_ERR_LINUX_TCP_NOT_OPEN)
#else
	if((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_ERR_TCP_SEND)
#endif
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

