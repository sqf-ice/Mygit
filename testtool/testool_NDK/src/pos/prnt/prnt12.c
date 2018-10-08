/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt12.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnFrnFeedByPixel�ӿڹ���
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
#define TESTAPI	"NDK_PrnFeedByPixel"	
#define  MINSTEPCNT  (0)
#define  MAXSTEPCNT  (792)	
#define MAXSTEPCNT_TP (1024)
#define	MAXWAITTIME	(180)	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt12(void)
* functional description 	:�ô�ӡ����ֽ������Ϊ���ص�,���øú�������û��������ֽ�����Ǵ��ڻ������У��ȵ���start֮��ʹ�ӡ����һ��ִ�� 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  				 chensj   20130105	   created
*****************************************************************/
void prnt12(void)
{
	//����Ϊ�ֲ�����
	int nRet = 0;
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet1 = 0,nCnt = 0;//unOldLen=0,unNewLen=0;
	int nType = lib_getprintertype();
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*///init��modeΪ normal
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}

	NDK_PrnSetFont(PRN_HZ_FONT_12x16,PRN_ZM_FONT_8x8);
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
			
			if(nType == PRN_TYPE_TP)
			{
				cls_show_msg1(MAXWAITTIME/5, "%d���,����ֽ%d������", MAXWAITTIME/5, MAXSTEPCNT_TP);
				print_Ex("---%d���,����ֽ%d������---\n", MAXWAITTIME/5, MAXSTEPCNT_TP);
				if((nRet=NDK_PrnFeedByPixel(MAXSTEPCNT_TP))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
					goto prnt_error;//return;
				}
				print_Ex("---�ѽ�ֽ%d������---\n", MAXSTEPCNT_TP);
				NDK_PrnStart();

				//�쳣: ����
				if((nRet=NDK_PrnFeedByPixel(MAXSTEPCNT_TP+1))!=NDK_ERR_PARA)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
					goto prnt_error;//return;
				}
			}
			
			cls_show_msg1(MAXWAITTIME/5, "%d���,����ֽ%d������", MAXWAITTIME/5, MAXSTEPCNT);
			print_Ex("---%d���,����ֽ%d������---\n", MAXWAITTIME/5, MAXSTEPCNT);
			if((nRet=NDK_PrnFeedByPixel(MAXSTEPCNT))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet, nRet1);
				goto prnt_error;//return;
			}
			print_Ex("---�ѽ�ֽ%d������---\n", MAXSTEPCNT);
			NDK_PrnStart();
		
			nCnt = rand()%MAXSTEPCNT+1+MAXSTEPCNT_TP/*MAXSTEPCNT*/;	/* chensj modifie 2013/06/18 :MAXSTEPCNT�޸ĳ�MAXSTEPCNT_TP��ֹ�������Կ��ܳɹ�����ʧ�ܵ�BUG*/
			//cls_show_msg1(MAXWAITTIME/5, "%d���,����ֽ%d������", MAXWAITTIME/5, MINSTEPCNT);
			//print_Ex("---%d���,����ֽ%d������---\n", MAXWAITTIME/5, MINSTEPCNT);
			if((nRet=NDK_PrnFeedByPixel(MINSTEPCNT))!=NDK_OK||(nRet1=NDK_PrnFeedByPixel(nCnt))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet, nRet1);
				goto prnt_error;//return;
			}
			print_Ex("---�ѽ�ֽ%d������---\n", MINSTEPCNT);
			NDK_PrnStart();

			//NDK_PrnSetMode(PRN_MODE_NORMAL,0);
			nCnt = rand()%MAXSTEPCNT;
			cls_show_msg1(MAXWAITTIME/5, "%d���,�������ֽ%d������", MAXWAITTIME/5, nCnt);
			print_Ex("---%d���,�������ֽ%d������---\n", MAXWAITTIME/5, nCnt);
#if 0	//���޷��������ص���,������
			unOldLen = lib_getprinterlen();
#endif
			if((nRet=NDK_PrnFeedByPixel(nCnt))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
				goto prnt_error;//return;
			}
#if 0	//���޷��������ص���,������
			NDK_PrnStart();
			unNewLen = lib_getprinterlen();
			if(unNewLen-unOldLen!=nCnt)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI,unNewLen-unOldLen, nCnt);
				goto prnt_error;//return;
			}
#endif
			print_Ex("---�ѽ�ֽ%d������---\n", nCnt);			
			NDK_PrnStart();

			NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE,0);
			//nCnt = rand()%MAXSTEPCNT;
			cls_show_msg1(MAXWAITTIME/5, "%d���,�������ֽ%d������", MAXWAITTIME/5, nCnt);
			print_Ex("---%d���,�������ֽ%d������---\n", MAXWAITTIME/5, nCnt);
#if 0	//���޷��������ص���,������			
			unOldLen = lib_getprinterlen();
#endif
			if((nRet=NDK_PrnFeedByPixel(nCnt))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
				goto prnt_error;//return;
			}
#if 0	//���޷��������ص���,������
			NDK_PrnStart();
			unNewLen = lib_getprinterlen();
			if(unNewLen-unOldLen!=nCnt/**2*/)//lib_getprinterlen,NDK_PrnFeedByPixel��Ӧ��Ϊʵ��(��ӡ����ֽ)����,��ģʽ�޹�
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI,unNewLen-unOldLen, nCnt);
				goto prnt_error;//return;
			}
#endif			
			print_Ex("---�ѽ�ֽ%d������(Ӧ����һ����ͬ������)---\n", nCnt);	
			NDK_PrnStart();

			//test over
			nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,[ȷ��]ͨ��,[����]ʧ��");
			if (nKeyin==ENTER||nKeyin==0)
				send_result("%s����ͨ��", TESTAPI);
			else
				send_result("%s����ʧ��", TESTAPI);
				
			goto prnt_error;//return;
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
				goto prnt_error;//return;
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
			goto prnt_error;//return;
			break;
		}
	
	}
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}

