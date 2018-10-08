/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name		: sckt6.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_TcpRead�ӿڹ���
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
#define 	TESTAPI		"NDK_TcpRead"
#define		MAXWAITTIME 60	//���յȴ�ʱ��
#define		WUCHASEC	0.03 //���ֵ����
#if K21_ENABLE
#define		MAX_SIZE	2047//�Ͷ�����֧��2047
#else
#define		MAX_SIZE	5678
#endif
#define		MAXTIMEOUT	3	//��ʱ����ʱ��

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
void sckt6(void)
{
	//����Ϊ�ֲ�����
	int ret = -1,ret1 = -1,ret2 = -1,j = 0;
	uint unSckt = 0, sendLen = 0, recvLen = 0, recvLen1 = 0, recvLen2 = 0;
	ushort usPort = 3400+rand()%256;	
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	float tmp = 0.0;
#if K21_ENABLE		
	char str[32] = {0};
#endif	

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
	
	//��socket����
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
	memset(recvBuf,0,sizeof(recvBuf));
#if K21_ENABLE
	if ((ret=NDK_TcpRead(unSckt, recvBuf, sizeof(recvBuf), MAXWAITTIME, &recvLen)) != NDK_ERR_TCP_NETWORK)
#else
	if ((ret=NDK_TcpRead(unSckt, recvBuf, sizeof(recvBuf), MAXWAITTIME, &recvLen)) != NDK_ERR_TCP_RECV)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//����socket
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
	//case2:����Ƿ��������������в���
	if(//((ret=NDK_TcpRead(-1, recvBuf, sizeof(recvBuf), MAXWAITTIME, &recvLen))  == NDK_OK /*!= NDK_ERR,NDK_ERR_PARA*/)||//recvBuf�������ܷ�,������֪�����޸ĵ�BUG,Ϊ���ٲ��Թ����еķ�����λ��ʱ�����Ե�һ�������Ƿ����//20160325�����еͶ˷���ֵ�޸ĳ�ֻҪ���ɹ������� linwl	
		((ret1=NDK_TcpRead(unSckt, NULL, sizeof(recvBuf), MAXWAITTIME, &recvLen)) != NDK_ERR_PARA)
		||((ret2=NDK_TcpRead(unSckt, recvBuf, sizeof(recvBuf), MAXWAITTIME, NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2);
		GOTOERR;
	}
	
	//case3:����ʹ�ò���(������)
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if ((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_OK)
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

	//case4:�����������Է������ݣ��ִ�(����)����linwl20130617
	if ((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_OK)
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
	if((ret=NDK_TcpRead(unSckt, recvBuf, sendLen/2, MAXWAITTIME, &recvLen1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case7:�����������ճ�������1��֤���ճ��ȳ���ʵ�����ݳ���ʱ����NDK_OK���ҽ��յ�ʵ�ʳ��ȵ�����  20170112
#if K21_ENABLE
	if((ret=NDK_TcpRead(unSckt, recvBuf+sendLen/2, MAX_SIZE-sendLen/2+1, 10, &recvLen2)) != NDK_OK)
#else
	if((ret=NDK_TcpRead(unSckt, recvBuf+sendLen/2, MAX_SIZE-sendLen/2, MAXWAITTIME, &recvLen2)) != NDK_OK)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	recvLen = recvLen1+recvLen2;
	if(recvLen != sendLen || memcmp(sendBuf, recvBuf, recvLen))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}
	
	//case5:��ʱ���ԣ�׼ȷ�Բ��ԣ��Ͷ�ƽ̨��NDK_ERR_TCP_TIMEOUT�������ֵ��ʹ��NDK_ERR_TIMEOUT
	memset(recvBuf,0,sizeof(recvBuf));
	lib_StartStopwatch();
#if K21_ENABLE
	if(((ret=NDK_TcpRead(unSckt, recvBuf, MAX_SIZE+1, MAXTIMEOUT, &recvLen)) != NDK_ERR_TIMEOUT)||((tmp=lib_ReadStopwatch()-MAXTIMEOUT)>WUCHASEC))
#else
	if(((ret=NDK_TcpRead(unSckt, recvBuf, MAX_SIZE+1, MAXTIMEOUT, &recvLen)) != NDK_ERR_TCP_TIMEOUT)||((tmp=lib_ReadStopwatch()-MAXTIMEOUT)>WUCHASEC))
#endif
	{
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(tmp,str);
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, str);
#else
		send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
#endif
		GOTOERR;
	}
	
	//case6:�ر�socket�����ѹرյ�socketӦʧ��
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
	if ((ret=NDK_TcpRead(unSckt, recvBuf, MAX_SIZE, MAXWAITTIME, &recvLen)) != NDK_ERR_LINUX_TCP_NOT_OPEN)
#else
	if ((ret=NDK_TcpRead(unSckt, recvBuf, MAX_SIZE, MAXWAITTIME, &recvLen)) != NDK_ERR)
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

