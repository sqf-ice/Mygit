/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt17.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnIntegrate�ӿڹ���
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
#define TESTAPI	"NDK_PrnIntegrate"	
#define MAXWAITTIME 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct _Prntest18_mode
{
	int	mode;
	char *msg;
}Prntest18_mode;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt17(void)
* functional description 	:��ӡbmp��png�ȸ�ʽ��ͼƬ  
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt17(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet = 0;
	int i=0;
	char sText[1024] = {0};
	char sTextOver[1024*2] = {0};

	Prntest18_mode prntest18_mode[] =
	{
		{TPSET_AUTO, "�Զ���Ӧ�����ֻ���ͼ�񣬱�֤���غϴ�ӡ"},
		{TPSET_TEXTUP, "�������ϣ��������غϣ����ֽ�ֱ�Ӹ�д��ͼ����"},
		{TPSET_PICUP, "ͼ�����ϣ��������غϣ�ͼ��ֱ�Ӹ�д��������"},
		{TPSET_MIX, "����ͼ��Ƕ�ף����غϣ����ֺ�ͼ��Ƕ�״�ӡ"}
	};
	
	//����Ϊ������
	cls_show_msg1(2, "����%s...��ע���ӡЧ��", TESTAPI);
	memset(sText, 'A', sizeof(sText));
	memset(sTextOver, 'B', sizeof(sTextOver));

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
			nBusy = FALSE;
		
			switch(i){
			default:
				NDK_PrnStr(prntest18_mode[i].msg);
				if ((nRet = NDK_PrnIntegrate(128, 64, 64, 64, CCB_BMP, sText, prntest18_mode[i].mode))!=NDK_OK)
				{
					send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet,i);
					return;
				}
				break;
			case SZ_ARRAY(prntest18_mode)://˵������������OK��,���ڽ����쳣��������
				//���Դ���Ĵ�ӡģʽ
				NDK_PrnStr("���Դ���Ĵ�ӡģʽ����Ĭ�ϵĴ�ӡģʽ���д�ӡ");
				if ((nRet =NDK_PrnIntegrate(128, 64, 64, 64,CCB_BMP ,sText, 256))!=NDK_OK)	//��default��������SUCC
				{
					send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					return;
				}
				NDK_PrnStart();

				//������Բ��������ַ������ȳ���1K
				NDK_PrnStr("�����ַ������ȳ���1Kʱ,��ֻ��ӡ1K(������۲��ӡЧ��)");
				if ((nRet =NDK_PrnIntegrate(128, 64, 64, 64, CCB_BMP, sTextOver, TPSET_AUTO))!=NDK_OK)//��default��������SUCC
				{
					send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					return;
				}
				NDK_PrnStart();
				
				NDK_PrnStr("���Դ������Ӧ����ӡ�κ�ͼ�κ�����");
				//xsize -- ͼ����(����)�����ܴ���384����
				if ((nRet =NDK_PrnIntegrate(512, 16, 0 , 0, CCB_BMP, sText, TPSET_AUTO))!=FAIL)
				{
					send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					return;
				}
				NDK_PrnStart();
				
				//(xpos+xsize)���ܴ���384����
				if ((nRet =NDK_PrnIntegrate(128, 64, 385-128, 64, CCB_BMP, sText, TPSET_AUTO))!=FAIL)
				{
					send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					return;
				}
				NDK_PrnStart();
				break;
			case SZ_ARRAY(prntest18_mode)+1:
				//test over
				nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,��ENTERͨ��,��������ͨ��");
				if (nKeyin!=ENTER && nKeyin!=0)	//zhangnw20061031 modify
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				return;
				break;
			}
			NDK_PrnStart();
			i++;
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
				return;
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
	return;
}

