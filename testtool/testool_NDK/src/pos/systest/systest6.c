/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 6.c
* Author 			: chenfm
* version			: V1.0
* DATE			: 20130116
* directory 		: 
* description		: �ſ�ˢ��ѹ������
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  20130116 9.36
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"�ſ�����,ѹ��"
#define MAXWAITTIME 30

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		chenfm  		   	20130116  	 	created
*****************************************************************/
static int testtime(void)
{
	/*private & local definition*/
#define	DEFAULT_CNT_STR	"10"//"60"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(60)
	int cnt = DEFAULT_CNT_VLE, ret = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR;

	/*process body*/
	while(1)
	{
		cls_printf("������Ե�ʱ��(����):\n");
		//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default 
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			cnt = DEFAULT_CNT_VLE;
		else
			cnt = atoi(CntStr);
		
		if(cnt>0)
			break;
		else
			show_msg1(2, "����Ӧ�ô���0!\n");
	}
	return cnt;
}

static int IsDisplayed(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	while(1)
	{
		ret = cls_show_msg("_ѡ����ʾģʽ_\n"
							"1.����ʾ2.��ʾ\n");
		
		switch(ret) 
		{
		case ESC://ESC,����Ĭ��
			show_msg1(2, "��ʹ��1.����ʾ(Ĭ��)...");
		case '1':
			return FALSE;
			break;
		case '2':
			return TRUE;
			break;
		default:
			continue;
			break;
		}
	}
}

static int select_TK(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	while(1)
	{
		ret  = cls_show_msg("_ѡ��Ҫ���ŵ�_\n"
							"1.23�� 2.123��\n");
		
		switch(ret) 
		{
		case ESC://ESC,����Ĭ��
			show_msg1(2, "����23��(Ĭ��)...");
		case '1':
			return TK2_3;
			break;
		case '2':
			return TK1_2_3;
			break;
		default:
			continue;
			break;
		}
	}
}

void mag_abnormal(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin = 0;
	
	/*process body*/
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�ſ���ʧ��(%d)", __LINE__, ret);
		return;
	}
	while(1)
	{
		nKeyin=cls_show_msg("��ס��Դ��,ѡ���л�Ӧ�á���,�������дſ�ѹ�����Լ��еĴſ�,Ӧ����������.");
		if(nKeyin == ESC)
		{
			NDK_MagClose();
			break;
		}
	}

	return;
}

static void mag_press(int SelTk, int IsDisp, float TestLong)
{
	/*private & local definition*/
	int ret = 0;
	float testalltime = 0.0;
#if defined ME15CHX
    float tmptime = 0.0;
#endif
#if K21_ENABLE
	char str[32] = {0},str1[32] = {0};
#endif
	int StrCountErr = 0, StrCountOK = 0, strTotle = 0;

	/*process body*/
#if defined ME15CHX
	tmptime =  lib_ReadStopwatch();
#else
	lib_StartStopwatch();
#endif
	do
	{
		switch((ret=MagcardReadTest(SelTk, IsDisp, /*99*/MAXWAITTIME)))
		{ 
		//case FUNCQUIT:
		case NDK_ERR_QUIT:
			continue;
			break;
		case STRIPE:
			StrCountOK++;
			break;
		case FAIL:
		case NDK_ERR_TIMEOUT:
		default://����TK1,TK2...
			StrCountErr++;
			break;  
		}
		strTotle++;
#if defined ME15CHX
		testalltime = lib_ReadStopwatch()-tmptime;
#else
		testalltime = lib_ReadStopwatch();
#endif
		if(TestLong<testalltime)
			break;
	}while(ret!=NDK_ERR_QUIT);
#if !K21_ENABLE
	cls_show_msg("ˢ���趨ʱ��:%fs\nˢ��ʵ��ʱ��:%fs\n", TestLong, testalltime);
#else
	memset(str,0,sizeof(str));
	memset(str1,0,sizeof(str1));
	ftos(TestLong,str);
	ftos(testalltime,str1);
	cls_show_msg("ˢ���趨ʱ��:%ss\nˢ��ʵ��ʱ��:%ss\n", str, str1);
#endif
	cls_show_msg_record(FILENAME,FUNCNAME,"������%d��\n"
				"ˢ�ſ��ɹ���:\n"
				"��ȷ%d��/��%d��\n", StrCountErr, StrCountOK, strTotle);	
}

//�ſ���ѹ��������Ӧ��Ҫ�����һ��ǿ����,ˢ���ɹ���(����)������
void systest6(void)
{
	
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}
	/*private & local definition*/
	int tk = 0, Isdisp = 0, keyin = 0;
	float testlong = 0.0;

	/*process body*/
	cls_show_msg1(1, "%s������...\n", TESTITEM);

	while (1)
	{	
		keyin = cls_show_msg("1.�ſ��ۺ�\n"
							"2.�쳣����\n"	
							"0.��������");
		switch(keyin)
		{
		case '1':
			mag_press(tk, Isdisp, testlong);
			break;
		case '2':
			mag_abnormal();
			break;
		case '0':
			testlong =  testtime()*60; //��������Ե�ʱ��(����),�ú�����֤����ʱ����Ч(>0)
			tk = select_TK();
			Isdisp = IsDisplayed();
			break;
		case ESC:
			cls_show_msg("�뻻����ǿ/��/���ſ����²���!���������...");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

