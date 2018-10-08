/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt20.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetPageLen �ӿڹ���
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
#define TESTAPI	"NDK_PrnSetPageLen"	
#define	MAXCASECNT		(6)
#define	MAXWAITTIME	(180)	//30->180 for ���
#define	MAXPAGELEN_TP		(2370)	//����
#define	MAXPAGELEN_HIP	(2370)	//�������
#define	MAXPAGELEN_FIP	(8*88)	//Ħ�����

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int testcase_TP(int subcase)
{
	/*private & local definition*/
	char sImage[(128*64)/8] = {0};
	int nRnd = rand()%49+1;

	/*process body*/
	memset(sImage, 0xff, sizeof(sImage));
	switch (subcase)
	{
	case 0:	//����������ӡ	
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;	
	case 1: //�������Ϊ���ֵ��2371����
		if(NDK_PrnSetPageLen(nRnd)==NDK_OK)
		{
			send_result("line %d:%s����ʧ��(rand:%d)", __LINE__, TESTAPI, nRnd);
			return FAIL;
		}
		if(NDK_PrnSetPageLen(MAXPAGELEN_TP+1)==NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;
	case 2://������ӡ�����������Ϊ0��ʱ����ҳ֮��ֻ��һ�հ���		
		if(NDK_PrnSetPageLen(0)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		NDK_PrnStr("����ӡ\"setprintpagelen:0\"��128x64��䵥");
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	case 3:	//�������Ϊ50����
		if(NDK_PrnSetPageLen(50)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		NDK_PrnStr("����ӡ\"setprintpagelen:50\"��128x64��䵥");
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	case 4://�������Ϊ2370����
		if(NDK_PrnSetPageLen(MAXPAGELEN_TP)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		print_Ex("����ӡ\"setprintpagelen:%d\"��128x64��䵥", MAXPAGELEN_TP);
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	default://�������Ϊ80(Ĭ��ֵ)
		if(NDK_PrnSetPageLen(80)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		NDK_PrnStr("����ӡ\"setprintpagelen:80\"��128x64��䵥");
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	}

	return SUCC;
}

static int testcase_xIP(int xIPtype, int subcase)
{
	/*private & local definition*/
	char sImage[(128*64)/8] = {0};
	int MAXPAGELEN_xIP = (xIPtype==PRN_TYPE_HIP )?MAXPAGELEN_HIP:MAXPAGELEN_FIP;

	/*process body*/
	memset(sImage, 0xff, sizeof(sImage));
	switch (subcase)
	{
	case 0:	//����������ӡ	
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;	
	case 1: //Խ�����
		if(NDK_PrnSetPageLen(-1)==NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		if(NDK_PrnSetPageLen(MAXPAGELEN_xIP+1)==SUCC)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;
	case 2://�߽����	
		if(NDK_PrnSetPageLen(0)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		NDK_PrnStr("����ӡ\"setprintpagelen:0\"��128x64��䵥");
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	case 3://�߽����	
		if(NDK_PrnSetPageLen(MAXPAGELEN_xIP)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		print_Ex("����ӡ\"setprintpagelen:%d\"��128x64��䵥", (xIPtype==PRN_TYPE_HIP )?MAXPAGELEN_HIP:MAXPAGELEN_FIP);
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	default://�������ΪĬ��ֵ
		if(NDK_PrnSetPageLen(396)!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		print_Ex("����ӡ\"setprintpagelen:396\"��128x64��䵥");//NDK_PrnStr("����ӡ\"setprintpagelen:396\"��128x64��䵥");
		if (NDK_PrnImage(128, 64, 0, sImage)!=NDK_OK)
		{
			send_result("line %d:��ӡͼ��ʧ��", __LINE__);
			return FAIL;
		}
		break;
	}

	return SUCC;
}


/****************************************************************
* function name 	 				:void prnt20(void)
* functional description 	:����
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt20(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet=0,i=0;
	int nType = lib_getprintertype();
	
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
			switch(i)
			{
			default:
				//��������ʾ�ŵ�����testcaseȥ��ʾ(��testcase������send_result),����ֱ�ӷ���
				if((nType==PRN_TYPE_TP )&&(testcase_TP(i)==FAIL))
						goto prnt_error;//return;
				else if((nType==PRN_TYPE_HIP ||nType==PRN_TYPE_FIP )&&(testcase_xIP(nType, i)==FAIL))
						goto prnt_error;//return;
				else if(nType==PRN_TYPE_END )
				{
					send_result("line %d:δ�ܻ�ȡ����ӡ������", __LINE__);
					goto prnt_error;//return;
				}
				break;
			case MAXCASECNT:
				nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,��ENTERͨ��,��������ͨ��");
				if (nKeyin!=ENTER && nKeyin!=0)//zhangnw20061031 modify
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;//return;				
				break;
			}
			NDK_PrnStr("\f");//������һ��\f����ҳ������Ч
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

