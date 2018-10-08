/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt5.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnStart�ӿڹ���
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
#define TESTAPI	"NDK_PrnStart"	
#define MAXWAITTIME 90

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt5(void)
* functional description 	: ��ʼ��ӡ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt5(void)
{
	//����Ϊ�ֲ�����
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet = 0;
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

	//case3:ȱֽ����� ֱ�ӵ���NDK_PrnStartӦ�û᷵��ȱֽ״̬
	cls_show_msg("ȷ��ȱֽ�����,���������...");
	if((nRet = NDK_PrnStart()) != PRN_STATUS_NOPAPER)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
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

			//start��init��ϵĹ�����init�������Ѿ�����,������������start���ԵĶ���
			
			//case1:�ܷ�ʼ������ӡ����ӡ����ֵΪ0���Լ���������		
			if((nRet = lib_initprn(0)) != NDK_OK)/*if((nRet = NDK_PrnInit(0)) != NDK_OK)*/
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			if((nRet = NDK_PrnStr("������ӡ\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n")) != NDK_OK )
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			if((nRet = NDK_PrnStart()) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			NDK_PrnGetStatus(&emStatus);
			if(emStatus!=PRN_STATUS_OK)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI, emStatus);
				goto prnt_error;//return;
			}
			if (cls_show_msg("���\"������ӡ\"��10��,��[ȷ��]����[����]")!=ENTER)
			{
				send_result("%s����ʧ��", TESTAPI);
				goto prnt_error;//return;
			}
			cls_show_msg1(2, "������ͨ��!");			
				
			//case2:δ����NDK_PrnStr��NDK_PrnImageֱ�ӵ���NDK_PrnStart
			cls_show_msg("������ӡ�κ�����,���������...");
			if((nRet = lib_initprn(1)) != NDK_OK)/*if((nRet = NDK_PrnInit(1)) != NDK_OK)*/
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			if((nRet = NDK_PrnStart()) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			if (cls_show_msg("������κ�����,��[ȷ��]����[����]")==ENTER)
			{
				send_result("%s����ʧ��", TESTAPI);
				goto prnt_error;//return;
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

