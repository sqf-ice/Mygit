/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ�� 
* file name			: 
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: socketģ���������ϲ���
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"
#define   MAXWAITTIME 5
#if K21_ENABLE
#define		MAX_SIZE	2047//�Ͷ�����֧��2047
#else
#define		MAX_SIZE	5678
#endif


/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"socketģ����������"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
	TcpOpen=0 ,			 
	TcpClose,
	TcpWait,
	TcpConnect,
	TcpWrite,
	TcpRead,
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	TcpCheck,
	TcpSetKeepAlive,
#else
	TcpBind,
	TcpReset,
#endif
	MaxNum,//����ö��ֵʱ�ڴ�ֵǰ���,Ҫ��֤��ֵΪ���һ��ֵ
}EM_SCKT;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  sull	         20180108	   	    created
*****************************************************************/
static int sckt_randtest(int funnum)
{
	uint unSckt = 0, sendLen = 0, recvLen = 0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	int tcp_status = 0, errcode = 0;
#else
	ushort usPort = 3400+rand()%256;
#endif
	switch(funnum)
	{	
		case TcpOpen:
			NDK_TcpOpen(&unSckt);
			NDK_TcpClose(unSckt);//���ӹرվ��,Ϊ��ֹ�������Ծ����̫�ർ��������������ʧ�ܵ����� 20180801 modify
			break;
		case TcpClose:
			NDK_TcpClose(unSckt);
			break;
		case TcpWait:
			NDK_TcpWait(unSckt);
			break;
		case TcpConnect:
			NDK_TcpConnect(unSckt, netsetting.SvrIP.Str, netsetting.SvrTcpPort, MAXWAITTIME);                                                       
			break;
		case TcpWrite:
			memset(sendBuf, 0, sizeof(sendBuf));
			NDK_TcpWrite(unSckt, sendBuf, sizeof(sendBuf), MAXWAITTIME, &sendLen);
			break;
		case TcpRead:
			memset(recvBuf, 0, sizeof(recvBuf));
			NDK_TcpRead(unSckt, recvBuf, sendLen, MAXWAITTIME, &recvLen);
			break;
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
		case TcpCheck:
			NDK_TcpCheck(unSckt, &tcp_status, &errcode);
			break;
		case TcpSetKeepAlive:
			NDK_TcpSetKeepAlive(1,5,65,9);
			break;
#else
		case TcpBind:
			NDK_TcpBind(unSckt, netsetting.LocalIP.Str, usPort);
			break;
		case TcpReset:
			NDK_TcpReset(unSckt);
			break;
#endif
	}
	return SUCC;
}

void sckt200(void)
{
	/*private & local definition*/
	int  i = 0, bak = 0, num[30] ={0}, j = 0; 
	int ret = -1,cnt =50, len = 0;//cnt ����
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	char *scktstr[] = {"NDK_TcpOpen", "NDK_TcpClose", "NDK_TcpWait", "NDK_TcpConnect", "NDK_TcpWrite", "NDK_TcpRead", "NDK_TcpCheck", "NDK_TcpSetKeepAlive"};
#else
	char *scktstr[] = {"NDK_TcpOpen", "NDK_TcpClose", "NDK_TcpWait", "NDK_TcpConnect", "NDK_TcpWrite", "NDK_TcpRead", "NDK_TcpBind", "NDK_TcpReset"};
#endif

	/*process body*/
	//����ǰ��:
	NetUp();
	len = (MaxNum>20)?MaxNum:20;
	while(cnt)
	{	
		cnt--;
		bak++;
		
		//���������
		cls_printf("��%d�����˳��:",bak);
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
		
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret = sckt_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",scktstr[num[j]]);
				goto ERR;
			}
		}	
	}
	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR: 
	//���Ժ���
	NetDown();
	return;
}

