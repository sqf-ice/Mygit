/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt11.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetForm�ӿڹ���
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
#define TESTAPI	"NDK_PrnSetForm"	
#define	MAXWAITTIME	(180)
#define	LEFTBORDER_MIN		(0)
#define	LEFTBORDER_MAX_TP	(288)
#define	LEFTBORDER_MAX_HIP	(288)
#define	LEFTBORDER_MAX_FIP	(288)
#define	RANGE_MIN		(0)
#define	RANGE_MAX_TP	(255)
#define	RANGE_MAX_HIP	(255)
#define	RANGE_MAX_FIP	(255)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int testcase(int subcase, int LEFTBORDER_MAX, int RANGE_MAX)
{
	/*private & local definition*/
	int nRnd = rand()%LEFTBORDER_MAX, nRet = SUCC;
	int nColumn = rand()%RANGE_MAX, nRow = rand()%RANGE_MAX;
	
	/*process body*/
	switch (subcase)
	{
	case 0:		
		NDK_PrnSetForm(LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);
		print_Ex("---��߾�(%d)���м��(%d,%d)(���ֺ�ͼ��Ӧ��߽����,���м�಻Ӱ��ͼ��Ч��)---\n", LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);//��Ӱ��ͼ���м��  ��Ҫ���Ѿ���ʱ 20150403 linwl��linwȷ��
		NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n�´�½NewLand00045\n");
		NDK_PrnImage(128, 64, 0, CCB_BMP);
		NDK_PrnImage(16, 16, 0, NL_Icon);
		break;
	case 1:  
#if 1 /*���������ʱ���⣬�߽�ֵ�������ö�������+�ּ�೬������ӡ�ĵ����г����ܷ�*/
		NDK_PrnSetForm(LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);    
		print_Ex("---��߾�(%d)���м��(%d,%d)(ͼ��Ӧ��߽���Ϊ0,���м�಻Ӱ��ͼ��Ч��)---\n", LEFTBORDER_MAX, RANGE_MAX, RANGE_MAX);//��Ӱ��ͼ���м��
		if((nRet=NDK_PrnSetForm(LEFTBORDER_MAX,RANGE_MAX,RANGE_MAX))!=NDK_OK)
		{
			cls_show_msg1(5,"line %d:%s����ʧ��(nRet=%d)",__LINE__,TESTAPI,nRet);
			return -5;	
		}	
		NDK_PrnStr("NL�´�½045\n");//NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n�´�½NewLand00045\n");
		NDK_PrnImage(128, 64, 0, CCB_BMP);
		NDK_PrnImage(16, 16, 0, NL_Icon);
#endif
		break;	
	case 2:
		NDK_PrnSetForm(LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);   			
		print_Ex("---��߾�(%d)���м��(%d,%d)---\n",nRnd,nColumn,nRow);
		if((nRet=NDK_PrnSetForm(nRnd,nColumn,nRow))!=NDK_OK)
		{
			cls_show_msg1(5,"line %d:%s����ʧ��(nRet=%d)",__LINE__,TESTAPI,nRet);
			return -6;	
		}	
		NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n");//NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n�´�½NewLand00045\n");
		NDK_PrnImage(128, 64, 0, CCB_BMP);
		NDK_PrnImage(16, 16, 0, NL_Icon);//Сͼ��
		NDK_PrnImage(128, 64, MIN(288-nRnd, 30), CCB_BMP);
		NDK_PrnImage(16, 16, MIN(288-nRnd, 30), NL_Icon);//Сͼ��
		break;
	case 3://�쳣
#if ANDIROD_ENABLE//20170407 linqy�����޸�
		if(NDK_PrnSetForm(385,0,0)!=NDK_ERR_PARA)
#else
		if(NDK_PrnSetForm(299,0,0)!=NDK_ERR_PARA)
#endif
			return -2;
		if(NDK_PrnSetForm(50,34,256)!=NDK_ERR_PARA)
			return -3;
		if(NDK_PrnSetForm(50,256,30)!=NDK_ERR_PARA)
			return -4;
		break;
	case 4://�߽����:w�պ���ʾ�ڱ߽紦 20150425�����߽�ֵ����,linwl
		NDK_PrnSetForm(LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);   			
		print_Ex("---��߾�(155)���м��(63,0)---\n");
		if((nRet=NDK_PrnSetForm(155,63,0))!=NDK_OK)
		{
			cls_show_msg1(5,"line %d:%s����ʧ��(nRet=%d)",__LINE__,TESTAPI,nRet);
			return -6;	
		}	
		NDK_PrnStr("��NewLand\n");
		break;
	case 5://�߽����,w���û���
		NDK_PrnSetForm(LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);   			
		print_Ex("---��߾�(156)���м��(63,0)---\n");
		if((nRet=NDK_PrnSetForm(156,63,0))!=NDK_OK)
		{
			cls_show_msg1(5,"line %d:%s����ʧ��(nRet=%d)",__LINE__,TESTAPI,nRet);
			return -6;	
		}	
		NDK_PrnStr("��NewLand\n");	
		break;
	default://�ָ�Ĭ��
		NDK_PrnSetForm(LEFTBORDER_MIN,RANGE_MIN,RANGE_MIN);  
		print_Ex("---��߾�(%d)���м��(%d,%d)(Ĭ��)---\n", LEFTBORDER_MIN,RANGE_MIN, RANGE_MIN);
		NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n�´�½NewLand00045\n");	
		NDK_PrnImage(128, 64, 0, CCB_BMP);
		NDK_PrnImage(16, 16, 0, NL_Icon);
		nRet = FAIL;
		break;
	}	
	
	return nRet;
}

/****************************************************************
* function name 	 				:void prnt11(void)
* functional description 	:���ô�ӡ��߽硢�ּ�ࡢ�м�ࡣ�ڶԴ�ӡ����Ч���ú�һֱ��Ч��ֱ���´�����
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt11(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int i=0;
	int nLeftborder_max=0;
	int nRange_max = 0;
	int nRet = 0;
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

	nLeftborder_max=(PRN_TYPE_TP ==nType)?LEFTBORDER_MAX_TP:((PRN_TYPE_HIP ==nType)?LEFTBORDER_MAX_HIP:LEFTBORDER_MAX_FIP);
	nRange_max = (PRN_TYPE_TP==nType)?RANGE_MAX_TP:((PRN_TYPE_HIP==nType)?RANGE_MAX_HIP:RANGE_MAX_FIP);
	
	while(1)
	{
		if((nRet=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
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

			if((nRet = testcase(i++, nLeftborder_max,nRange_max))==SUCC)
			{
				NDK_PrnStart();
				//continue;
			}
			else
			{
				if(nRet !=FAIL)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				NDK_PrnStart();
				nKeyin = cls_show_msg("����ӡЧ��,[ȷ��]ͨ��,[����]ʧ��");
				if (nKeyin!=ENTER)
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;
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
	
	//���Խ���
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}

