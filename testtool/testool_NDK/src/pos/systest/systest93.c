/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest93.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����/��ģ�齻�����
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"
#include <pthread.h>

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"����/��ģ�齻��"
#if !K21_ENABLE || OVERSEAS_ENABLE || defined SP610
#define  TESTFILE "FT6"
#else	//K21ƽֻ̨֧�־���·��
#define  TESTFILE "/appfs/FT6"
#endif
#define WAKE_UP_TIME 128
//#define MAXWAITTIME  600
#define	ITEMPERPAGE	(4)

/*------------global variables definition-----------------------*/
int thread_run_flag = 0;
static pthread_mutex_t mutex;
static int pthread_retval = 0;
SMART_t type;

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
*			  			jiangym			20171121 		created
*****************************************************************/
//�������ӡ�߳�,��ע��ӡЧ��,��עһ��ֽ�����ʹ��ѭ����ӡֽ����ѹ������,�������������ӡ������ص�
static void *pthread_prnt(void *arg)
{
	int ret = -1 ;
	EM_PRN_STATUS pemStatus;

	//��ӡ��ʼ��
	if((ret=lib_initprn(g_PrnSwitch))!= SUCC)//��ʼ��
	{
		cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
		goto ERR1;
	}
	NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_12x24A);
	NDK_PrnSetForm(0,0,6);
	NDK_PrnSetMode(PRN_MODE_NORMAL,1);
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_prnt is runing\n",__LINE__);
		NDK_PrnStr("�̻�����(MERCHANT Name):\n  �����̻�����\n");
		NDK_PrnStr("�̻���(MERCHANT NO.):\n  123456789012345\n");
		NDK_PrnStr("�ն˱��(TERMINAL NO.):12345678\n");
		NDK_SysMsDelay(4);
		NDK_PrnStr("�յ���(ACQUIRER):\n  �յ����� 12345678\n");
		NDK_PrnStr("������(ISSUER):�������� 12345678\n");
		NDK_PrnStr("��������(TRANS TYPE):����/SALE\n");
		NDK_PrnStart();
		if((ret = NDK_PrnGetStatus(&pemStatus))!=NDK_OK)
		{
			cls_show_msg1(3,"��ȡ��ӡ״̬ʧ��(%d)",ret);
			goto ERR1;
		}
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}
//��ʵ�� �����̻߳�һֱ��ռ,ˢ���̺߳���ִ�е�ˢ��,���ǽ�����ˢ���Ż�ͣ�ڴſ��߳���
static void *pthread_mag(void *arg)
{
	int  ret = -1, nErrorCode = 0;
	char pszTk1[MAXTRACKLEN] = {0}, pszTk2[MAXTRACKLEN] = {0}, pszTk3[MAXTRACKLEN] = {0};
	//�򿪴ſ�	
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		cls_show_msg1(3,"line %d:�ſ���ʧ��",__LINE__);
		goto ERR1;
	}
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_mag is runing\n",__LINE__);
		NDK_MagReset();
		cls_show_msg("��ˢ�������������");
		memset(pszTk1,0,sizeof(pszTk1));
		memset(pszTk2,0,sizeof(pszTk2));
		memset(pszTk3,0,sizeof(pszTk3));
		NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &nErrorCode);
	/*	//��ʾ�ŵ���Ϣ
		if((ret=check_stripe_result_tk1(pszTk1))!=0x00)
		{
			cls_show_msg("line %d:����ʧ��(0x%x)", __LINE__, ret);
			goto ERR1;
		}
		if((ret=check_stripe_result_tk2(pszTk2))!=0x00)
		{
			cls_show_msg("line %d:����ʧ��(0x%x)", __LINE__, ret);
			goto ERR1;
		}
		if((ret=check_stripe_result_tk3(pszTk3))!=0x00)
		{
			cls_show_msg("line %d:����ʧ��(0x%x)", __LINE__, ret);
			goto ERR1;
		}	*/
		NDK_SysMsDelay(4);
	}
ERR1:
	NDK_MagClose();
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}
//������ic���߳�,һ��Ҳ������Ϊ��,��Ҫͨ���˲�����֤�����Ƿ��������
static void *pthread_ic(void *arg)
{
	int  ret = -1, buflen = 0;
	uchar buf[256] = {0};

	cls_show_msg("�����IC1��,�������������");
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_ic is runing\n",__LINE__);
		memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
		{
			cls_show_msg1(3,"line %d: IC1�ϵ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{
			cls_show_msg1(3,"line %d: IC1�µ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}
//��������Ƶ���߳���Ҫ�������ж���Ƶ�����ú��ٽ���
static void *pthread_rfid(void *arg)
{
	int ret = -1;
	uchar UID[LEN_UID] = {0}, pUIDlen = 0, rcvlen = 0;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "15693"};

	/*process body*/
	cls_show_msg1(g_keeptime, "���ڸ�Ӧ������%s��,���������...", card[type]);
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_rfid is runing\n",__LINE__);
		//��ʼ������Ѱ������
		if(RFID_init(type)!=SUCC)
		{
			cls_show_msg1(3,"line %d:��ʼ��ʧ��!���������Ƿ���ȷ.", __LINE__);
			goto ERR1;
		}
		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg1(3,"line %d:Ѱ��ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}	
		//����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{	
			cls_show_msg1(3,"line %d:����ʧ��(%d)", __LINE__,  ret);
			goto ERR1;
		}
		//��д
		if ((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
		{	
			RFID_deactive(type, 10);
			cls_show_msg1(3,"line %d:��дʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		//�رճ�
		if ((ret=RFID_deactive(type, 0))!=SUCC)
		{	
			cls_show_msg1(3, "line %d:�رճ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		RFID_finish(type);
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}
/*������ͨѶ�߳�,ͨѶ������Ҳ�п��ܽ�������,��Ϊ��ֹ�������ǽ�ֹ�Զ�����,�˴���Ч,���Կ��ǲ���˵�
�����Թ����г���ͨѶʧ������Ҳ�п���,ʵ��Ӧ�ù������ǽ�ֹ������*/
#if 0  //Ϊ�˱��������Ա��,�˲�������ʱ����
static void *pthread_comm(void *arg)
{
	int slen = 0,  rlen = 0, i = 0, timeout = SO_TIMEO, ret = -1;
	uint h_tcp = 0;
	char sendbuf[PACKMAXLEN] = {0}, recbuf[PACKMAXLEN] = {0};

	//���ɲ������� 8k����
	for (i=0; i<sizeof(sendbuf); i++)
		sendbuf[i]=rand()%256;
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_comm is runing\n",__LINE__);
		if(LinkUp()!=SUCC)
		{
			cls_show_msg1(3,"line %d:LinkUpʧ��", __LINE__);
			goto ERR1;
		}
		if(NetUp()!=SUCC)
		{
			LinkDown();
			cls_show_msg1(3,"line %d:NetUpʧ��", __LINE__);
			goto ERR1;
		}
		if(TransUp(&h_tcp)!=SUCC)
		{
			NetDown();
			LinkDown();
			cls_show_msg1(3,"line %d:TransUpʧ��", __LINE__);
			goto ERR1;
		}
		//��������
		if ((slen=sock_send(h_tcp, sendbuf, PACKMAXLEN, timeout))!=PACKMAXLEN)
		{
			NetDown();
			LinkDown();
			cls_show_msg1(3,"line %d:��������ʧ��(%d)", __LINE__, slen);
			goto ERR1;
		}
		//��������
		memset(recbuf, 0, sizeof(recbuf));
		if((rlen=sock_recv(h_tcp, recbuf, PACKMAXLEN, timeout))!=PACKMAXLEN)	
		{
			NetDown();
			LinkDown();
			cls_show_msg1(3,"line %d:��������ʧ��(%d)", __LINE__, rlen);
			goto ERR1;
		}
		if (TransDown(h_tcp)!=SUCC)
		{
			NetDown();
			LinkDown();
			cls_show_msg1(3,"line %d:TransDownʧ��", __LINE__);
			goto ERR1;
		}
		if(NetDown()!=SUCC)
		{
			LinkDown();
			cls_show_msg1(3,"line %d:NetDownʧ��", __LINE__);
			goto ERR1;
		}
		if(LinkDown()!=SUCC)
		{
			cls_show_msg1(3,"line %d:LinkDownʧ��", __LINE__);
			goto ERR1;
		}
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}
#endif

static void *pthread_bt(void *arg)
{
#if BT_ENABLE
	int  ret = -1;

	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_bt is runing\n",__LINE__);
		//����������
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:������ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		//ME30MH������Ҫ�����㲥
#if 0//defined ME30MH||defined ME30GX
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:�������ÿɼ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
#endif
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:��������ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		if((ret=bt_comm_once(12,0))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:����ͨѶʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:�����Ͽ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		NDK_PortClose(PORT_NUM_BT);
		NDK_SysMsDelay(4);
	}
ERR1:
	NDK_PortClose(PORT_NUM_BT);
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
#else 
	cls_show_msg("��֧������,��������˳�");
	return;
#endif
}

//ɨ������պ������Ҫ�Ż�,���������ͷ��صĺ���ʱ��Ҫ�Ĵ���

static void *pthread_scan(void *arg)
{
#if SCAN_ENABLE||SCANASYCN_ENABLE
	int ret = 0;

	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�豸��ʼ��ʧ��(%d)", __LINE__, ret);
		return;
	}
#if SCAN_ENABLE
	NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT, g_Floodlight);
	NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT, g_Focuslight);
	NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY, g_Sensitivity);
#endif
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_scan is runing\n",__LINE__);
		cls_printf("��ɨ��");
		if((ret=lib_doscan(g_ScanType, 15))!=NDK_OK)
		{
			cls_show_msg1(3,"line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
#if SCAN_SOFT_ENABLE
	NDK_ScanExit();
#endif
	return ((void *) &pthread_retval);
#else
	cls_show_msg("��֧��ɨ�裬��������˳�");
	return;
#endif
}

static void *pthread_usb(void *arg)
{
	int  ret = -1;

	cls_show_msg("�����U��,���������");
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_usb is runing\n",__LINE__);
		if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:USB��ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		NDK_PortClose(PORT_NUM_USB);
		
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}

static void *pthread_udisk(void *arg)
{
	int  ret = -1;

	cls_show_msg("�����U��,���������");
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_udisk is runing\n",__LINE__);
		if((ret=NDK_DiskOpen(UDISK,10))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:P�̴�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		//�ر��豸
		if((ret=NDK_DiskClose(UDISK))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:U�̹ر�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}

static void *pthread_fs(void *arg)
{
	int  ret = -1;
	int fp=0, writelen=0, readlen=0;
	char writebuf[200]={0}, readbuf[200]={0};

	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_fs is runing\n",__LINE__);
		NDK_FsDel(TESTFILE);
		if((fp=NDK_FsOpen(TESTFILE, "w"))<0)
		{
			cls_show_msg1(3, "line %d:�ļ���ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		memset(writebuf, 0x08, sizeof(writebuf));
		if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
		{
			cls_show_msg1(3, "line %d:д�ļ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		NDK_FsSeek(fp, 0l, SEEK_SET);
		memset(readbuf, 0, sizeof(readbuf));
		if((readlen=NDK_FsRead(fp, readbuf, 200))!=200)
		{
			cls_show_msg1(3, "line %d:���ļ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		if((ret=NDK_FsClose(fp))!=NDK_OK)
		{
			cls_show_msg1(3, "line %d:�ر��ļ�ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}
		NDK_SysMsDelay(4);
	}
ERR1:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
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
		cls_show_msg1(3,"line %d:������Կʧ��(%d)", __LINE__, ret);
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
		cls_show_msg1(3,"line %d:��װTMK��Կ����ʧ��(%d)", __LINE__, ret);
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
		cls_show_msg1(3,"line %d:��װ��Կʧ��(%d)", __LINE__, ret);
		goto ERR1;
	}
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_sec is runing\n",__LINE__);
		//���мӽ�������		
		if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
		{
			cls_show_msg1(3,"line %d:DES����ʧ��(%d)", __LINE__, ret);
			goto ERR1;
		}	
		if(memcmp(udesout, "\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7", 16))
		{
			cls_show_msg1(3,"line %d:���У��ʧ��", __LINE__);
			goto ERR1;
		}
		NDK_SysMsDelay(4);
	}
ERR1:
	NDK_SecKeyErase();
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);
}

static void *pthread_sleep(void *arg)
{
	int  ret = -1 ;
	
	//thread_run_flag ��1ʱ �߳��˳�
	while(thread_run_flag == 0)
	{	
		//Ϊ��ȷ��ȷʵ���߳�����
		fprintf(stderr,"line:[%d]thread_sleep is runing\n",__LINE__);
		if((ret = NDK_SysSetSuspendDuration(WAKE_UP_TIME))!=NDK_OK)
		{
			cls_show_msg1(3,"�������߻���ʱ��ʧ��(%d)",ret);
			goto ERR;
		}
	/*	if((ret = NDK_SysGoSuspend())!=NDK_OK && ret != NDK_ERR_PRN_BUSY && ret != NDK_ERR_USB_BUSY)  //���ش�ӡæ����
		{			
			cls_show_msg1(3,"��������ʧ��(%d)",ret);
			goto ERR;
		}*/
		NDK_SysGoSuspend();	
		NDK_SysMsDelay(3);//��ʱ3ms	
	}
ERR:
	pthread_mutex_lock(&mutex);
	thread_run_flag = 1;
	pthread_retval = ret;
	pthread_mutex_unlock(&mutex);
	return ((void *) &pthread_retval);

}
static void sleep_other_test(int casekey)
{	
	/*private & local definition*/
	pthread_t tidp,tidp1;
    	int error=-1, error1=-1;
	//pthread_attr_t attr;
	time_t oldtime;
	int nkey = 0;
	PTHREAD pfunc[] = {pthread_prnt,pthread_mag,pthread_ic,pthread_rfid,/*pthread_comm,*/pthread_bt,pthread_scan,
				pthread_usb,pthread_udisk,pthread_fs,pthread_sec};
	char *func_info[] = {"�������ӡ", "������ſ�", "������IC��", "��������Ƶ��", /*"����������/WIFI",*/ "����������", "������ɨ��", 
					"������USB", "������U��", "�������ļ�ϵͳ", "�����밲ȫ"};

	/*process body*/
	cls_printf("����%s���߳�,���������߿ɻ���",func_info[casekey]);	
	thread_run_flag = 0;
	pthread_mutex_init(&mutex, NULL);
	if((error=NDK_PthreadCreate(&tidp,NULL,pfunc[casekey],NULL))!=0)
	{
		cls_show_msg1(g_keeptime,"line %d:������ӡ�߳�ʧ��(%d)", __LINE__, error);
		return;
	}
	if((error1=NDK_PthreadCreate(&tidp1,NULL,pthread_sleep,NULL))!=0)
	{
		cls_show_msg1(g_keeptime,"line %d:���������߳�ʧ��(%d)", __LINE__, error1);
		return;
	}
	oldtime=time(NULL);
	while(1)
	{
		if(thread_run_flag==1)	
			break;
	/*	if((time(NULL)-oldtime)>MAXWAITTIME)
		{
			thread_run_flag=1;
			break;
		}*/
		cls_printf("%s˫�߳�ִ����,�ɰ�ESC�˳�",func_info[casekey]);
		NDK_KbGetCode(1, &nkey);
		if(nkey == ESC)
		{
			thread_run_flag=1;
			break;
		}
	}
	NDK_PthreadJoin(tidp,NULL);
	NDK_PthreadJoin(tidp1,NULL);
	pthread_mutex_destroy(&mutex);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����ͨ��",func_info[casekey]);
	return;
}

//������ʵ��
void  select_sleep_func(void)
{
	/*private & local definition*/	
	int nKeyin = 0, nPageSel = 0;//��0��ʼ����(��һҳ��Ϊ0)
	int cnt = 0, menuid = 0, maxpage = 0, sn =0;
	char *menustr[] = {
		"�������ӡ",
		"������ſ�",
		"������IC��",
		"��������Ƶ��",
		/*"����������/WIFI",*/
		"����������",
		"������ɨ��", 
		"������USB",
		"������U��",
		"�������ļ�ϵͳ",
		"�����밲ȫ",
		};
	/*process body*/
#if SCR_DISABLE
	char str[1024]={0};
#endif
	char* tmpstr = NULL;

	/*process body*/
	maxpage = (SZ_ARRAY(menustr)-1)/ITEMPERPAGE;//��0��ʼ����(��һҳ��Ϊ0)
	cls_show_msg1(2, "ѡ�������..\n" "����:���ط�ҳ\n" "����:����ҳ\n");

	while (1)
	{
		NDK_ScrClrs();
		for (cnt=0; cnt<ITEMPERPAGE; cnt++)
		{
			menuid = cnt+ITEMPERPAGE*nPageSel;
			if (menuid==-1)
				break;
			else if (menuid>SZ_ARRAY(menustr)-1)
				break;
			else
				tmpstr = menustr[menuid];

#if SCR_DISABLE	//�ض�λ��������ʾ
			sprintf(str,"%d.%s\n", cnt+1, tmpstr);
			NDK_PortWrite(g_com, strlen(str), str);
#endif
			NDK_ScrPrintf("%d.%s\n", cnt+1, tmpstr);
			NDK_ScrRefresh();
		}

		//������
		switch ((nKeyin=lib_getch()))
		{
		case '1':
		case '2':
		case '3':
		case '4':
			if((sn=nKeyin-'1'+ITEMPERPAGE*nPageSel)>SZ_ARRAY(menustr)-1)
				break;
			else
				sleep_other_test(sn);
			break;
#if K21_ENABLE||defined ME50S
		case '7':
#else
		case DOWNKEY:
#endif
			/*�·���*/
			if (nPageSel>=maxpage)
				nPageSel = 0;
			else
				nPageSel++;
			break;
#if K21_ENABLE||defined ME50S
		case '8':
#else
		case UPKEY:
#endif
			/*�Ϸ���*/
			if (nPageSel==0)
				nPageSel = maxpage;
			else
				nPageSel--;
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

//���ú���
void set_config(void)
{
	int keyin = 0;

	/*process body*/
	while(1)
       {
		keyin = cls_show_msg(/*"1.WLM����\n"
						     "2.WLAN����\n"*/
						     "3.BT����\n"
						     "4.��ӡ����\n"
						     "5.��Ƶ������\n");
		switch(keyin) 
		{
	/*	case '1':
			conf_conn_WLM(TRUE);
			break;
		case '2':
			conf_conn_WLAN();
			break;*/
		case '3':
			conf_bt();
			break;
		case '4':
			conf_prn();
			break;
		case '5':
			type = select_smarttype();
			break;			
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return ;
}

void systest93(void)
{
	/*private & local definition*/	
	int keyin = 0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	
	while(1)
	{
		keyin = cls_show_msg("1.����������ģ�鲢��\n"
						      "0.����\n");
		switch(keyin) 
		{
		case '1':
			select_sleep_func();
			break;
		case '0':
			set_config();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}	
	return;
}


