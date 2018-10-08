/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt10.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetGreyScale�ӿڹ���
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
#define TESTAPI	"NDK_PrnSetGreyScale"
#define	MAXWAITTIME	(180)	//30->180 for ���

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt10(void)
* functional description 	:���ô�ӡ�Ҷ�
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt10(void)
{
	//����Ϊ�ֲ�����
	char sPrintbuf[(128*64)/8] = {0};
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int i = GREY_MINVALID;
	int nRet=0;
	
	//Ŀǰ����������ӡ֧�ֻҶ�����,��������д��
	int MAXPRLINEWIDE_old = MAXPRLINEWIDE_TP, MAXPRLINEWIDE=MAXPRLINEWIDE_old/2;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	memset(sPrintbuf,0xff,sizeof(sPrintbuf));
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}

	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);	
	NDK_PrnSetMode(PRN_MODE_NORMAL/*PRN_MODE_ALL_DOUBLE*/,0);
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

			if(i==GREY_MAXVALID+1)
			{
				//˵��[MIN,MAX]������Χ�Ѳ���,���ڽ����쳣��������
				if(NDK_PrnSetGreyScale(GREY_MAXVALID+1) != NDK_ERR_PARA)
				{
					send_result("line %d:���ûҶ�%dӦʧ��!\n", __LINE__, GREY_MAXVALID+1);
					goto  prnt_error;
				}
#if 0 //���ز�����
				if(NDK_PrnSetGreyScale(GREY_MINVALID-1) != NDK_ERR_PARA)
				{
					send_result("line %d:���ûҶ�%dӦʧ��!\n", __LINE__, GREY_MINVALID-1);
					goto  prnt_error;
				}
#endif

				//test over
				//NDK_PrnSetGreyScale(g_PrnGrey);
				nKeyin = cls_show_msg("��֤��ӡЧ��,��ENTERͨ��,��������ͨ��");
				if (nKeyin!=ENTER)
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;//return;
			}
			else
			{
				if(NDK_PrnSetGreyScale(i)==NDK_OK)
				{
					print_Ex("���ô�ӡ�Ҷ�ֵ%d�ɹ�%c��ע���ӡ�Ҷ�Ч��", i, (i%2)?'\n':',');
					if((nRet=NDK_PrnImage(128, 64, (i%2)?0:(MAXPRLINEWIDE-128),sPrintbuf))!=NDK_OK)
					{
						send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI, nRet, i);//cls_show_msg("line %d:��ӡ��䵥ʧ��!\n", __LINE__);
						goto prnt_error;//return;
					}
					
					NDK_PrnStr("\n");
					NDK_PrnStart();  
				}
				else
				{
					send_result("line %d:���ô�ӡ�Ҷ�ֵ%dʧ��", __LINE__, i);
					goto prnt_error;
				}
				i++;//������i,����Ϊ��һ����������
			}
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
	NDK_PrnSetGreyScale(g_PrnGrey);
	return;
}

