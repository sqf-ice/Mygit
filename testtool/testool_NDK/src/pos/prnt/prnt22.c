/************************************************************************
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt22.c
* Author 		: jiangym
* version		: 
* DATE			: 20130821
* directory 		: 
* description		: ����NDK_PrnFeedPaper�ӿڹ���
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
#define TESTAPI	"NDK_PrnFeedPaper"	
#define MAXWAITTIME 180

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt22(void)
* functional description 	: ����������ӡ���ڴ�ӡ��ҳǰ���ӡ��ɺ�Ľ���ֽ���� 
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			
*****************************************************************/
void prnt22(void)
{
	//����Ϊ�ֲ�����
	int nBusy = FALSE, nKeyin = 0;
	time_t nOldtime = 0, nDiff = 0;
	int ret = 0;
	EM_PRN_STATUS emStatus=PRN_STATUS_BUSY;
	
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
		if((ret=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,ret);
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
			
			//case1:�����쳣-1,PRN_FEEDPAPER_AFTER+1
			if((ret=NDK_PrnFeedPaper(-1))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER+1))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}

			//case5:����������֤��ֽ�����ܹ�׼ȷ�Ľ�ֽ���߳�ֽ��
			NDK_PrnStr("��������Ӧ��ȫ��ֽ����");
			NDK_PrnStart();
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			if(cls_show_msg("ע��[��������Ӧ��ȫ��ֽ����]�Ƿ���ֽ����,[Enter]ͨ��,[����]ʧ��")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				return;	
			}
			
			//case2:���������Ƚ�ֽ����ֽ
			NDK_PrnStr("��ӡǰ�����ֽ����\n   Newland   \n123       321");
			NDK_PrnStart();
			//case3:֧��������ֽ��������ֽ
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_BEFORE))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_BEFORE))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			NDK_PrnStr("123       321\n   Newland   \n��������ݲ����໥����");
			NDK_PrnStart();
			//case4:��ֽ������ֽӦ���ܹ��ص�ԭ����ֽǰ��λ��,�Ӵ�ӡ�����Ͽ�����ӡЧ��������ָ��Ǵ�ӡ���м���̫�������
			if((ret=NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;	
			}
			if(cls_show_msg("��ӡ�ļ������ֲ����ָ���,[Enter]ͨ��,[����]ʧ��")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				return;	
			}

			send_result("%s����ͨ��", TESTAPI);
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
	
	//���Խ���	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);	
	return;
}
