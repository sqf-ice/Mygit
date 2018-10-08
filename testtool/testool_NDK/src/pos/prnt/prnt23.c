/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt22.c
* Author 		: linwl
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnGetDotData�ӿڸ��������ַ�����ȡ��ϵͳָ����ӡ������Ӧ�ĵ��󻺳�
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
#define TESTAPI	"NDK_PrnGetDotData"	
#define MAXWAITTIME 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt21(void)
* functional description 	:���ء��������塢��ӡ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						linwl   20140105	created
*****************************************************************/
void prnt23(void)
{
	//����Ϊ�ֲ�����
	int nBusy = FALSE, nKeyin = 0, ret = 0;
	time_t nOldtime = 0, nDiff = 0;
	EM_PRN_STATUS emStatus=PRN_STATUS_BUSY;
	char str[]="����������:1234590!@#()*&";
	char prnbuf[4096];
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	
	//����ǰ��:
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	
	//case1:���������쳣����
	//emGetFont����ָ���ֿ⣬Ŀǰֻ֧��PRN_GETFONT_16X16������ֵӦ�÷��ز�֧��
	if((ret = NDK_PrnGetDotData(0,str,prnbuf,strlen(str)*8,16))!=NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�ַ�������ΪNULL��Ӧ�÷��ز�������
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,NULL,prnbuf,strlen(str)*8,16))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//���ջ������ΪNULL��Ӧ�÷��ز�������
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,str,NULL,strlen(str)*8,16))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�������ؿ����Ϊ0��Ӧ�÷��ز�������
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,str,prnbuf,0,16))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�������ظ߲���Ϊ0��Ӧ�÷��ز�������
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,str,prnbuf,strlen(str)*8,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�������ؿ�,Ҫ���Ȳ�С��pszBuf�ַ�������*�ַ�������(��8)�����򷵻ز�������
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,str,prnbuf,strlen(str)*8-1,16))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�������ظ�,Ҫ��С���ַ�����߶ȣ����򷵻ز�������
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,str,prnbuf,strlen(str)*8,16-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������
	memset(prnbuf, 0, sizeof(prnbuf));
	if((ret = NDK_PrnGetDotData(PRN_GETFONT_16X16,str,prnbuf,strlen(str)*8,16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, ret);
		RETURN;
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
			NDK_PrnImage(strlen(str)*8,16,0,prnbuf);
			NDK_PrnStart(); 
			NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
			if ((nKeyin = cls_show_msg("����:%s�Ĵ�ӡЧ��,[ȷ��]ͨ��,[����]ʧ��",str))!=ENTER)
				send_result("%s����ʧ��", TESTAPI);
			else
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

