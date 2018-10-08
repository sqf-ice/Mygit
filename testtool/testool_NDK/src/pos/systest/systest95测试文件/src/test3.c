/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: test3.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ��ȫ/�����߳̽���
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "NDK.h"
#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/syscall.h>

pid_t gettid(void)
{

	return syscall(SYS_gettid);

}

/*---------------constants/macro definition---------------------*/

/*------------global variables definition-----------------------*/
int thread_run_flag1 = 0;
static pthread_mutex_t mutex;
static int pthread_retval = 0;
static char WlanEssid[] = "test";

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
*			  						sull        20180411   created
*****************************************************************/
static void *pthread_sec(void *arg)
{
	int  ret = -1;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	uchar udesout[31]={0}, udesin[31]={0};

	//��װ��Կ
	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]������Կʧ��\n",__LINE__);
		goto ERR1;
	}
	memset(udesin, 0x20, sizeof(udesin));
	//��װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]��װTMK��Կ����ʧ��",__LINE__);
		goto ERR1;
	}
	//��װTDK2(��Կ16�ֽ�),ID=2,���İ�װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x14\x5F\x5C\x6E\x3D\x91\x44\x57\x38\xBE\xDB\x24\xA6\xD3\x80\x18", stKeyInfo.nDstKeyLen); /*��Ӧ��key����Ϊ8���ֽڵ�0x15+8���ֽڵ�0x17���޸�ǰ8��8��Կ��һ��*/
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]��װ��Կ����ʧ��",__LINE__);
		goto ERR1;
	}
	//thread_run_flag ��1ʱ �߳��˳�
	fprintf(stderr,"%s,%d devpid: %d\n",__FUNCTION__,__LINE__,gettid());
	while(thread_run_flag1 == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_sec is runing\n",__LINE__);
		//���мӽ�������		
		if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
		{
			fprintf(stderr,"line:[%d]DES����ʧ��",__LINE__);
			goto ERR1;
		}	
		if(memcmp(udesout, "\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7", 16))
		{
			fprintf(stderr,"line:[%d]���У��ʧ��",__LINE__);
			goto ERR1;
		}
		NDK_SysMsDelay(10);
	}
ERR1:
	NDK_SecKeyErase();
	pthread_mutex_lock(&mutex);
	thread_run_flag1 = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}

static void *pthread_wlm(void *arg)
{
	EM_WLM_STATUS emStatus = -1;
	int err = 0, ret = -1;
	EM_PPP_STATUS actual = PPP_STATUS_DISCONNECT;
	uint rlen = 0, slen = 0;
	uint time = 0;
	ulong LocalIp = 0;
	uchar ip[16] = {0};
	uint sckt = 0;
	ushort port = 0;
	char sbuf[] = "111111111111111111112222222222222222222222222222222";
	char rbuf[1024] = {0};

	if ((ret = NDK_WlInit(2000,NULL,&emStatus))!=NDK_OK||emStatus!=WLM_STATUS_OK)
	{
		fprintf(stderr,"line:[%d]���߳�ʼ��ʧ��",__LINE__);
		NDK_PppHangup(1);
		goto ERR1;
	}
	if((ret=NDK_PppDial("card", "card"))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]��PPPͨ��ʧ��!",__LINE__);
		NDK_PppHangup(1);
		goto ERR1;
	}
	NDK_SysStartWatch();
	while(1)
	{
		if((ret=NDK_PppCheck(&actual, &err))!=NDK_OK)
		{
			fprintf(stderr,"line:[%d]��ȡppp״̬ʧ��!",__LINE__);
			NDK_PppHangup(1);
			goto ERR1;
		}
		if(actual==PPP_STATUS_CONNECTED)
		{
				break;
		}
		NDK_SysStopWatch(&time);
		if(time/1000>10)//if(ReadStopwatch()>MAXTIMEOUT)
		{
			fprintf(stderr,"line:[%d]��ȡppp״̬��ʱ!",__LINE__);
			NDK_PppHangup(1);
			goto ERR1;
		}
	}
	fprintf(stderr,"%s,%d devpid: %d\n",__FUNCTION__,__LINE__,gettid());
	while(thread_run_flag1 == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_wlm is runing\n",__LINE__);
		if((ret = NDK_TcpOpen(&sckt)) != NDK_OK)
		{
			fprintf(stderr,"line:[%d]��TCPͨ��ʧ��!",__LINE__);
			NDK_PppHangup(1);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			goto ERR1;
		}
		if((ret = NDK_TcpConnect(sckt, "218.66.48.230", 3459, 5)) != NDK_OK)
		{
			fprintf(stderr,"line:[%d]TCP����ʧ��!",__LINE__);
			NDK_PppHangup(1);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			goto ERR1;
		}
		if((ret = NDK_TcpWrite(sckt, sbuf, sizeof(rbuf), 5, &slen)) != NDK_OK)
		{
			fprintf(stderr,"line:[%d]���ݷ���ʧ��!",__LINE__);
			NDK_PppHangup(1);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			goto ERR1;
		}
		memset(rbuf, 0, sizeof(rbuf));
		if((ret = NDK_TcpRead(sckt, rbuf, sizeof(rbuf), 5, &rlen)) != NDK_OK)
		{
			fprintf(stderr,"line:[%d]���ݽ���ʧ��!",__LINE__);
			NDK_PppHangup(1);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			goto ERR1;
		}
		if((ret = NDK_TcpClose(sckt)) != NDK_OK)
		{
			fprintf(stderr,"line:[%d]�Ͽ�TCPʧ��!",__LINE__);
			NDK_PppHangup(1);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			goto ERR1;
		}
		if((ret = NDK_TcpWait(sckt)) != NDK_OK)
		{
			fprintf(stderr,"line:[%d]�Ͽ�TCPʧ��!",__LINE__);
			NDK_PppHangup(1);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			goto ERR1;
		}
	}
	if((ret = NDK_PppHangup(1)) != NDK_OK)
	{
		fprintf(stderr,"line:[%d]�Ͽ�PPPʧ��!",__LINE__);
		NDK_PppHangup(1);
		NDK_TcpClose(sckt);
		NDK_TcpWait(sckt);
		goto ERR1;
	}
	NDK_SysStartWatch();
	while(1)
	{
		if((ret=NDK_PppCheck(&actual, &err))!=NDK_OK)
		{
			fprintf(stderr,"line:[%d]��ȡppp״̬ʧ��!",__LINE__);
			NDK_PppHangup(1);
			goto ERR1;
		}
		if(actual==PPP_STATUS_DISCONNECT)
		{
				break;
		}
		NDK_SysStopWatch(&time);
		if(time/1000>5)
		{
			fprintf(stderr,"line:[%d]��ȡppp״̬��ʱ!",__LINE__);
			NDK_PppHangup(1);
			goto ERR1;
		}
	}
	NDK_SysMsDelay(10);
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag1 = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}

void main(void)
{	
	/*private & local definition*/
	pthread_t tidp,tidp1;
    	int error=-1, error1=-1;
	//pthread_attr_t attr;

	/*process body*/
	NDK_ScrInitGui();

	fprintf(stderr,"line:[%d]��Ӧ��������...",__LINE__);
	//NDK_ScrClrs();
	//NDK_ScrPrintf("��Ӧ��������...");
	//NDK_ScrRefresh();

	thread_run_flag1 = 0;
	pthread_mutex_init(&mutex, NULL);
	if((error=NDK_PthreadCreate(&tidp,NULL,pthread_wlm,NULL))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]�����߳�ʧ��",__LINE__);
		NDK_FsDel("/root/flag");
		return;
	}
	NDK_SysMsDelay(10);
	if((error1=NDK_PthreadCreate(&tidp1,NULL,pthread_sec,NULL))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]�����߳�ʧ��",__LINE__);
		NDK_FsDel("/root/flag");
		return;
	}
	NDK_SysMsDelay(30*1000);
	thread_run_flag1=1;
	NDK_PthreadJoin(tidp,NULL);
	NDK_PthreadJoin(tidp1,NULL);
	pthread_mutex_destroy(&mutex);
	NDK_FsDel("/root/flag");
	fprintf(stderr,"line:[%d]��Ӧ��ִ�гɹ�",__LINE__);
	return;
}


