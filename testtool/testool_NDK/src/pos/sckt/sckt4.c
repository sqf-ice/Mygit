/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name		: sckt4.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_TcpConnect�ӿڹ���
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
#define 	TESTAPI		"NDK_TcpConnect"
#if defined ME68 || defined ME50NX //ME68����GPRS��ʽʱ��SOCKET_STATUS_DISCONNECT״̬���Ҫ80s���ܱ���ȡ��
#define		MAXWAITTIME 90//���յȴ�ʱ��
#else
#define		MAXWAITTIME 60	//���յȴ�ʱ��
#endif
#define		WUCHASEC	0.03 //���ֵ����
#if K21_ENABLE
#define		MAX_SIZE	2047//�Ͷ�����֧��2047
#else
#define		MAX_SIZE	5678
#endif
#define		MAXTIMEOUT	3//��ʱ����ʱ��

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
void sckt4(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, ret2 = -1, j = 0;
	uint unSckt = 0, sendLen = 0, recvLen = 0;
	ushort usPort = 3400+rand()%256;	
	char szRemoteIp[16] = "192.168.4.155", sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	float tmp = 0.0,connecttime=0;
	LinkType tmplinktype = getlinktype();
	uint unSckt1 = 0, sendLen1 = 0, recvLen1 = 0;
	char sendBuf1[MAX_SIZE] = {0}, recvBuf1[MAX_SIZE] = {0}, porttmp[6] = {0}, ip2[16] = {0};
	ushort port2;
	int len = 0;
#if K21_ENABLE
	int status=0;
	char str[32] = {0};
#endif

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//����ǰ��(�²�����Ҫ�Ƚ���)
	if(NetUp()!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		return;
	}
	
	//case1:�豸δ�򿪣��������Ӳ���Ӧ��ʧ��
	if((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR_LINUX_TCP_NOT_OPEN/*NDK_ERR_TCP_NETWORK*/)//ϸ������ֵ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�򿪲���TCP���
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
	
	//case2:����Ƿ�����,�յ�Sock���
	if(((ret=NDK_TcpConnect(0, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR_LINUX_TCP_NOT_OPEN/*NDK_ERR_TCP_NETWORK*/)
		||((ret1=NDK_TcpConnect(-1/*0*/, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) == NDK_OK /*!= NDK_ERR,NDK_ERR_TCP_NETWORK*/)//20160325�����еͶ˷���ֵ�޸ĳ�ֻҪ���ɹ������� linwl		
		||((ret2=NDK_TcpConnect(unSckt, NULL, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR_PARA)){
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2);
		GOTOERR;
	}

	//case3:Զ�̶˿ڴ����������
	//ret = NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, 123, MAXWAITTIME);//��̫������ֵ������ͨѶ��ʽ��һ��
	//if(((getlinktype()==ETH && ret!=NDK_ERR_LINUX_TCP_REFUSE)) || (getlinktype()!=ETH && ret!=NDK_ERR_LINUX_TCP_TIMEOUT))
#if K21_ENABLE    
	if((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, 123, MAXWAITTIME)) != NDK_OK)//�Ͷ������޸ĳɷ�����ʽ�� 20160323
#else
	if((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, 123, MAXWAITTIME)) == NDK_OK)//!= NDK_ERR_LINUX_TCP_TIMEOUT/*NDK_ERR_TCP_NETWORK*/)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if K21_ENABLE   //�˿ڴ������Ӳ��Ϸ���SOCKET_STATUS_DISCONNECT ״̬
	if((ret=lib_tcp_checkstatus(unSckt, MAXWAITTIME))!=SOCKET_STATUS_DISCONNECT/*SOCKET_STATUS_CONNECTERR*/)
	{
		send_result("line %d:%sTCP��ȡ����״̬����(%d))", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����case8:���Ӳ��Ϻ����д�᷵���������NDK_ERR_TCP_NETWORK
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if ((ret=NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_ERR_TCP_NETWORK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	
	if((ret=NDK_TcpClose(unSckt)) != NDK_OK)//����ΪʲôҪ���Ҷϣ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_TcpWait(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(auto_flag==0||auto_flag==2)//���ֶ�����
	{
	//case4:��ʱ���ԣ�������ʱʱ��׼ȷ��
	if((ret=NDK_TcpOpen(&unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	usPort=3656+rand()%256;//���ڹر�TCPͨ�����Ѹĳɷ�����ʽ,������´򿪰���ͬ�������ͬIP����ͬ�˿ںŻ�δ�����ͷ� 20160325
	if((ret=NDK_TcpBind(unSckt, netsetting.LocalIP.Str, usPort)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(tmplinktype!=ETH)//����̫���豸:������������IP�����Ӳ��϶���ʱ��WIFI��Ҫ�ε�·�����ϵ����߹������ӳ�ʱ 20140815 linwl
	{
		cls_show_msg("�����WIFI,��ε�·������WLAN�ڵ����ߺ����������");
		lib_StartStopwatch();
#if K21_ENABLE  
		if(((ret=NDK_TcpConnect(unSckt, szRemoteIp, netsetting.SvrTcpPort, MAXTIMEOUT)) != NDK_OK))//�Ͷ������޸ĳɷ�����ʽ�� 20160323
#else
		if(((ret=NDK_TcpConnect(unSckt, szRemoteIp, netsetting.SvrTcpPort, MAXTIMEOUT)) != NDK_ERR_TCP_TIMEOUT)||((tmp = lib_ReadStopwatch()-MAXTIMEOUT)>WUCHASEC))
#endif			
		{
			send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
			GOTOERR;
		}
#if K21_ENABLE   //��ʱ���Ӳ��Ϸ���SOCKET_STATUS_DISCONNECT
		if((ret=lib_tcp_checkstatus(unSckt, MAXWAITTIME))!=SOCKET_STATUS_DISCONNECT/*SOCKET_STATUS_CONNECTERR*/)//��ȡTCP״̬Ӧ��������ʧ��
		{
			send_result("line %d:%sTCP��ȡ����״̬����(%d))", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif
		cls_show_msg("�����WIFI,�����·�����ϵ����ߺ����������");
	}
	else//��̫��:��̫��Ӧ��Ҫ�ܹ��жϵ�δ����̫���ߣ�����NDK_ERR_ETH_PULLOUT���������˳���������ʱҪ����1��ʱ�����˳� 20140815 linwl
	{
		cls_show_msg("�������̫��,��ε����ߺ����������");
		sleep(2);//��������ʱ���ϵͳȥ�ж��������������δ�жϵ����������ȥ���ӷ��س�ʱ 20140815
		lib_StartStopwatch();
#if K21_ENABLE
		if(((ret=NDK_TcpConnect(unSckt, szRemoteIp, netsetting.SvrTcpPort, MAXTIMEOUT)) != NDK_OK))//�Ͷ������޸ĳɷ�����ʽ�� 20160323
#else
		if(((ret=NDK_TcpConnect(unSckt, szRemoteIp, netsetting.SvrTcpPort, MAXTIMEOUT)) != NDK_ERR_ETH_PULLOUT)||((tmp = lib_ReadStopwatch())>1))
#endif
		{
			send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
			GOTOERR;
		}
#if K21_ENABLE //�������̫��?�Ͷ���̫����δ֧�����޸�
		//ME50C��̫��δ��������»᷵��-523����modeom����ͬһ������  20180615  sull modify
		if((ret=NDK_TcpCheck(unSckt, &status, MAXWAITTIME)) != NDK_ERR_MODEM_NOLINE)
		//if((ret=lib_tcp_checkstatus(unSckt, MAXWAITTIME))!=SOCKET_STATUS_DISCONNECT/*SOCKET_STATUS_CONNECTERR*/)//��ȡTCP״̬Ӧ��������ʧ��
		{
			send_result("line %d:%sTCP��ȡ����״̬����(%d))", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif
		cls_show_msg("�������̫��,��������ߺ����������");
		sleep(2);
	}
	
	if((ret=NDK_TcpClose(unSckt)) != NDK_OK)//����ΪʲôҪ���Ҷϣ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_TcpWait(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	}
	else
		send_result("line %d:%s�����ֶ�������֤", __LINE__, TESTAPI);
	
	//case5:����ʹ�ò��ԣ�����һ��������ͨѶ
	if((ret=NDK_TcpOpen(&unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	usPort=3656+rand()%256;//���ڹر�TCPͨ�����Ѹĳɷ�����ʽ,������´򿪰���ͬ�������ͬIP����ͬ�˿ںŻ�δ�����ͷ� 20160325
	if((ret=NDK_TcpBind(unSckt, netsetting.LocalIP.Str, usPort)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	lib_StartStopwatch();
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
	connecttime=lib_ReadStopwatch();
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
	if ((ret=NDK_TcpRead(unSckt, recvBuf, sizeof(sendBuf), MAXWAITTIME, &recvLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(recvLen != sendLen || memcmp(sendBuf, recvBuf, recvLen))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}

#if 1
	//case6:���������ӵ�socket���
#if K21_ENABLE 
	if((ret=NDK_TcpConnect(unSckt, szRemoteIp, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR_TCP_NETWORK) //�Ͷ˲�֧�������������ϵľ��
#else
	if((ret=NDK_TcpConnect(unSckt, szRemoteIp, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_OK)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if K21_ENABLE //&&!(defined ME50NX||defined ME68) //����ƽ̨��֧��SOCKET_STATUS_SEND_OVER״̬  sull 20180416 modify
	if((ret=lib_tcp_checkstatus(unSckt, MAXWAITTIME))!=SOCKET_STATUS_SEND_OVER)//���������Ӿ�����ܳɹ�����ʧ�ܶ�Ӧ�ò�Ӱ��ʵ��״̬��case5���������ݣ��������ﷵ��SOCKET_STATUS_SEND_OVER
	{
		send_result("line %d:%sTCP��ȡ����ʧ��(%d))", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#elif 0//fined ME50NX||defined ME68)  //����ƽ̨��֧���˸�״̬  20180716 modify
	if((ret=lib_tcp_checkstatus(unSckt, MAXWAITTIME))!=SOCKET_STATUS_CONNECT)
	{
		send_result("line %d:%sTCP��ȡ����ʧ��(%d))", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#endif

	//case6:�ر�socket�������ѶϿ���socket
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
#if K21_ENABLE
	if ((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR_LINUX_TCP_NOT_OPEN)
#else
	if ((ret=NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	if(auto_flag==0||auto_flag==2)//���ֶ�����
	{
		//case7:����˫�˿ڲ��ԣ����������˿ڣ�����֮���Ƿ�ụ��Ӱ�� 20180503 sull add
		cls_printf("�ڶ���������IP:\n");
		strcpy(ip2, DEFAULT_SVR_IP_STR);
		if ((ret=lib_kbgetinput(ip2, 0, sizeof(ip2)-1, &len, INPUTDISP_OTHER, 30, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(ip2, DEFAULT_SVR_IP_STR);	
		//itoa(netsetting.SvrPort, tmp, 10);                                         
		cls_printf("�ڶ���������TCP�˿�:\n");
		port2 = 3460;
		sprintf(porttmp, "%d", port2);
		if ((ret=lib_kbgetinput(porttmp, 0, sizeof(porttmp)-1, &len, INPUTDISP_OTHER, 30, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			port2 = 3460;	                     
		else                                                                         
			port2 = atoi(porttmp);
		cls_printf("����˫�˿ڲ���...");
		if((ret = NDK_TcpOpen(&unSckt)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}	
		if((ret = NDK_TcpOpen(&unSckt1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret = NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME)) != NDK_OK)
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
	if((ret = NDK_TcpConnect(unSckt1, ip2, port2, MAXWAITTIME)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
#if K21_ENABLE
	if((ret=lib_tcp_checkstatus(unSckt1, MAXWAITTIME))!=SOCKET_STATUS_CONNECT)
	{
		send_result("line %d:%sTCP��ȡ����ʧ��(%d))", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	memset(sendBuf, 0,sizeof(sendBuf));
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if((ret = NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(3);
	for (j=0; j<sizeof(sendBuf1); j++)
		sendBuf1[j]=rand()%256;
	if((ret = NDK_TcpWrite(unSckt1, sendBuf1, sizeof(sendBuf1), MAXWAITTIME, &sendLen1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvBuf1,0,sizeof(recvBuf1));
	if ((ret=NDK_TcpRead(unSckt1, recvBuf1, sizeof(sendBuf1), MAXWAITTIME, &recvLen1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(recvLen1 != sendLen1 || memcmp(sendBuf1, recvBuf1, recvLen1))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen1, sendLen1);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if ((ret=NDK_TcpRead(unSckt, recvBuf, sizeof(sendBuf), MAXWAITTIME, &recvLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(recvLen != sendLen || memcmp(sendBuf, recvBuf, recvLen))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}
	if((ret = NDK_TcpClose(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}
	if((ret = NDK_TcpWait(unSckt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}
	if((ret = NDK_TcpClose(unSckt1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}	
	if((ret = NDK_TcpWait(unSckt1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, recvLen, sendLen);
		GOTOERR;
	}	
	}
	else
		send_result("line %d:%s�����ֶ�������֤", __LINE__, TESTAPI);

	//���Խ���
#if K21_ENABLE
	memset(str,0,sizeof(str));
	ftos(connecttime,str);
	send_result("%s����ͨ��(TCP ����ʱ��:%s)", TESTAPI, str); 
#else
	send_result("%s����ͨ��(TCP ����ʱ��:%f)", TESTAPI, connecttime); 
#endif
ERR:
	NDK_TcpClose(unSckt);
	NDK_TcpWait(unSckt);
	NetDown();
	return;
}

