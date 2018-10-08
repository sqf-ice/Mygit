/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt14.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnGetPrintedLen�ӿڹ���
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
#define TESTAPI	"NDK_PrnGetPrintedLen"	
#define MAXWAITTIME 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt14(void)
* functional description 	:��ȡ��ӡ����
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt14(void)
{
	//����Ϊ�ֲ�����
	int nRet = 0;
	uint unOldLen=0,unNewLen=0;
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	
	while(1)
	{
		if((nRet=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
		if(emStatus != PRN_STATUS_BUSY)
			show_stopwatch(DISABLE, 0);
		
		switch(emStatus)
		{
		case PRN_STATUS_BUSY:
			if(nBusy==FALSE)
			{
				nOldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
				nBusy = TRUE;
			}
			if((nDiff=time(NULL)-nOldtime)>MAXWAITTIME)
			{
				send_result("line %d:��ӡ������æ״̬ʱ�����", __LINE__);
				goto prnt_error;
			}
			show_stopwatch(ENABLE, nDiff);
			continue;
			break;

		case PRN_STATUS_OK:
			nBusy=FALSE;

			//case1:���Կ�����ʼͳ�Ƴ��ȣ�
			if (ENTER==cls_show_msg("�������Ƿ�Ϊ���߿��������еĵ�һ������?\n[ȷ��]��[����]��"))
			{
				if(NDK_PrnGetPrintedLen(&unOldLen)!=NDK_OK || unOldLen!=0)
				{
					send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
					goto prnt_error;
				}
			}
			
			//case2:���Ի�ȡ�ĳ����Ƿ���ȷ
			if(NDK_PrnGetPrintedLen(&unOldLen)!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
				goto prnt_error;
			}
			NDK_PrnStr("���´�ӡ128*64����LOGO\n");
			NDK_PrnImage(128, 64, 0, CCB_BMP);
			NDK_PrnStart();
			if((nRet = NDK_PrnGetPrintedLen(&unNewLen))!=NDK_OK || (unNewLen-unOldLen)!=64)
			{
				send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI, nRet, unNewLen-unOldLen);
				goto prnt_error;
			}

			//case3:����ֻ��������ӡ���λ�ȡ����Ӧ����
			lib_initprn(0);
			if(NDK_PrnGetPrintedLen(&unOldLen)!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
				goto prnt_error;
			}
			NDK_PrnStr("���������仰����ʧ��\n");
			if((nRet = NDK_PrnGetPrintedLen(&unNewLen))!=NDK_OK || (unNewLen-unOldLen)!=0)
			{
				send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI, nRet, unNewLen-unOldLen);
				goto prnt_error;
			}

			//case4:�����쳣
			if((nRet = NDK_PrnGetPrintedLen(NULL))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI, nRet);
				goto prnt_error;
			}			
			
			send_result("%s����ͨ��", TESTAPI);
			goto prnt_error;
			break;
		case PRN_STATUS_NOPAPER:
			cls_printf("��ӡ��ȱֽ...װֽ��,���������,ESC����");
			lib_kbflush(); //��KB BUF
			while ((nKeyin=lib_kbhit())==0)
			{
				NDK_SysBeep();
				//����װֽ
				NDK_PrnFeedByPixel(10);
				sleep(2);
			}
			if (nKeyin==ESC)
			{
				send_result("line %d:��ӡ��ȱֽ,�û�����", __LINE__);
				goto prnt_error;
			}
			NDK_PrnStart();
			cls_printf("����%s...", TESTAPI);
			break;
		case PRN_STATUS_OVERHEAT:
			send_result("line %d: ��ӡ������", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_VOLERR:
			send_result("line %d: ��ӡ����Դ����", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_DESTROYED:
			send_result("line %d: ��ӡ����", __LINE__);
			goto prnt_error;
			break;
		default:
			//δ֪״̬����Ϊ��
			send_result("line %d: UNKNOWN STATE(tpstatus=%d)", __LINE__, emStatus);
			goto prnt_error;
			break;
		}
	
	}
	
	//���Խ���
prnt_error:
	NDK_PrnInit(g_PrnSwitch);	
	return;
}

