/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt13.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetUnderLine�ӿڹ���
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
#define TESTAPI	"NDK_PrnSetUnderLine"	
#define MAXWAITTIME 90

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt13(void)
* functional description 	:��ӡ�Ƿ����»��߹���
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt13(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet = 0, nRet1 = 0;
	
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
			//�����ع�  ��֤�Ƿ����һ����Ч 20140603 added by jiangym
			//case1:PRN_UNDERLINE_STATUS_OPEN
			NDK_PrnSetUnderLine(PRN_UNDERLINE_STATUS_OPEN);
			NDK_PrnStr("1.������仰���û���»��߲���ʧ��\n");
			NDK_PrnStart();
			NDK_SysDelay(10);
			if (cls_show_msg("1.����ӡЧ��(�Ƿ����»���),[ȷ��]��,[����]û��")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto prnt_error;
			}
			NDK_PrnSetUnderLine(PRN_UNDERLINE_STATUS_OPEN);
			NDK_PrnStr("2.������仰���û���»��߲���ʧ��\n");
			NDK_PrnStart();
			NDK_SysDelay(10);
			if (cls_show_msg("2.����ӡЧ��(�Ƿ����»���),[ȷ��]��,[����]û��")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto prnt_error;
			}
			
			//case2:PRN_UNDERLINE_STATUS_CLOSE
			NDK_PrnSetUnderLine(PRN_UNDERLINE_STATUS_CLOSE);
			NDK_PrnStr("3.������仰������»��߲���ʧ��\n");
			NDK_PrnStart();
			NDK_SysDelay(10);
			if (cls_show_msg("3.����ӡЧ��(�Ƿ����»���),[ȷ��]��,[����]û��")==ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto prnt_error;
			}
			NDK_PrnSetUnderLine(PRN_UNDERLINE_STATUS_CLOSE);
			NDK_PrnStr("4.������仰������»��߲���ʧ��\n");
			NDK_PrnStart();
			NDK_SysDelay(10);
			if (cls_show_msg("4.����ӡЧ��(�Ƿ����»���),[ȷ��]��,[����]û��")==ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto prnt_error;
			}

			//case3:�쳣
			if((nRet = NDK_PrnSetUnderLine(-1))!=NDK_ERR_PARA) 
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			if((nRet1 = NDK_PrnSetUnderLine(2))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet1);
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
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	NDK_PrnSetUnderLine(g_PrnUnderline);
	return;
}

