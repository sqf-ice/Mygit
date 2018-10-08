/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt15.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnGetDotLine�ӿڹ���
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
#define TESTAPI	"NDK_PrnGetDotLine"	
#define MAXWAITTIME 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt15(void)
* functional description 	:��øôδ�ӡ�ĵ�������. 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt15(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet;
	uint unLine=0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	//if(NDK_PrnInit(0/*g_PrnSwitch*/) != NDK_OK)//��API�����ڹرյ����ͱߴ�����²���
	if(lib_initprn(0) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
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

			//case1:�쳣
			if(NDK_PrnGetDotLine(NULL) != NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
				goto prnt_error;
			}	
			
			//case2:��ȡ��ӡ�ַ�����( NDK_PrnInit ��ʼ������:PRN_MODE_NORMAL  ������������������ģʽ)
			NDK_PrnSetFont(PRN_HZ_FONT_16x16, PRN_ZM_FONT_16x16);
			NDK_PrnStr("�����ӡ����\n����ŶAAA!\n");
			//NDK_PrnGetDotLine���� ��start֮ǰ��ȡ����,�Ƶ�����ȥ.NDK_PrnStart();
			if((nRet=NDK_PrnGetDotLine(&unLine)) != NDK_OK ||(unLine!=32 && unLine!=32+56 && unLine!=32+48))//���԰汾�������"ע�⣺����Ϊ���԰汾������ӡ��ҳ��Ч"����ʾ��������48���У��������56��  linwl20140505
			{
				send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,unLine);
				NDK_PrnStart();
				goto prnt_error;
			}
			NDK_PrnStart();
			
			//case3:��ȡ��ӡͼ�ε���( NDK_PrnInit ��ʼ������:PRN_MODE_NORMAL  ������������������ģʽ)
			NDK_PrnImage(128, 64, 0, CCB_BMP);
			//NDK_PrnGetDotLine���� ��start֮ǰ��ȡ����,�Ƶ�����ȥ.NDK_PrnStart();
			if(NDK_PrnGetDotLine(&unLine) != NDK_OK ||(unLine!=64 && unLine!=64+56 && unLine!=64+48))
			{
				send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,unLine);
				NDK_PrnStart();
				goto prnt_error;
			}
			NDK_PrnStart();

			//test over
			if (cls_show_msg("����ӡЧ��,[ȷ��]ͨ��,[����]ʧ��")==ENTER)
				send_result("%s����ͨ��", TESTAPI);
			else
				send_result("%s����ʧ��", TESTAPI);
			
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

