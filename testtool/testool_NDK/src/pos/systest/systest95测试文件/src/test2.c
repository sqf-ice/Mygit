/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: test2.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ȡ����/��ȫ�߳̽���
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
static void *pthread_vol(void *arg)
{
		int  ret = -1;
	uint unVol = 0;

	//thread_run_flag ��1ʱ �߳��˳�
	fprintf(stderr,"%s,%d libpms volpid: %d\n",__FUNCTION__,__LINE__,gettid());
	while(thread_run_flag1 == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		//fprintf(stderr,"line:[%d]thread_vol is runing\n",__LINE__);
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK)
		{
			fprintf(stderr,"line:[%d]����ʧ��\n",__LINE__);
			goto ERR1;
		}
		NDK_SysMsDelay(10);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag1 = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}

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
	fprintf(stderr,"%s,%d secp devpid: %d\n",__FUNCTION__,__LINE__,gettid());
	while(thread_run_flag1 == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		//fprintf(stderr,"line:[%d]thread_sec is runing\n",__LINE__);
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
	if((error=NDK_PthreadCreate(&tidp,NULL,pthread_sec,NULL))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]�����߳�ʧ��",__LINE__);
		NDK_FsDel("/root/flag");
		return;
	}
	NDK_SysMsDelay(10);
	if((error1=NDK_PthreadCreate(&tidp1,NULL,pthread_vol,NULL))!=NDK_OK)
	{
		fprintf(stderr,"line:[%d]�����߳�ʧ��",__LINE__);
		NDK_FsDel("/root/flag");
		return;
	}
	NDK_SysMsDelay(5*1000);
	thread_run_flag1=1;
	NDK_PthreadJoin(tidp,NULL);
	NDK_PthreadJoin(tidp1,NULL);
	pthread_mutex_destroy(&mutex);
	NDK_FsDel("/root/flag");
	fprintf(stderr,"line:[%d]��Ӧ��ִ�гɹ�\n",__LINE__);
	return;
}


