/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt2.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnImage�ӿڹ���
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
#define TESTAPI	"NDK_PrnImage"	
#define MAXWAITTIME 180 //30->180 for ���
#define MAXCASECNT (10)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
//���º�����ɿ��ǰ�"ְ����"ģʽ�������,����Ŀǰ���ޱ�Ҫ(������֧������)
static int transfer_state(int *row_normal, int *col_normal, int *mode)
{
	/*private & local definition*/

	/*process body*/
	//��������˵,��˫����һ����,ֻҪ��������һ�ּ���
	if(*row_normal==DISABLE && *col_normal==DISABLE)
	{
		*col_normal = ENABLE;
		*mode = PRN_MODE_WIDTH_DOUBLE;
		return SUCC;
	}
	else if(*row_normal==DISABLE && *col_normal==ENABLE)
	{
		*row_normal = ENABLE;
		*col_normal = DISABLE;
		*mode = PRN_MODE_HEIGHT_DOUBLE;
		return SUCC;
	}
	else if(*row_normal==ENABLE && *col_normal==DISABLE)
	{
		*col_normal = ENABLE;
		*mode = PRN_MODE_NORMAL;
		return SUCC;
	}
	else
		return FAIL;//δ����״̬ת������FAIL
}

//big,small��#define	MAXPRLINEWIDE_MID	128Ϊ�н�
static int testcase_MAXPRLINEWIDE_big(int MAXPRLINEWIDE, int subcase)
{
	/*private & local definition*/
	char img[128] = {0};
	int nRet = 0;

	/*process body*/
	switch (subcase)
	{
	case 0:
		//���ڵ�ַ�ռ����������ԣ���ӡ������ͼƬCCB_BMP�󣬽��Ŵ�ӡ��ͼ�γ�������ͼҲ����������ͼ�Σ�ֻҪ���ܷɼ��ɡ�
		NDK_PrnStr("---���´�ӡCCB_BMP�͵ȴ�С������ͼ��ȴ������ͼ---\n");
		if ((nRet =NDK_PrnImage(128, 64*2, 0, CCB_BMP))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 1:
		NDK_PrnStr("---���´�ӡCCB_BMP_144---\n");
		if ((nRet = NDK_PrnImage(144, 64, 0, CCB_BMP_144))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 2:
		NDK_PrnStr("---���´�ӡCCB_BMP---\n");
		if ((nRet = NDK_PrnImage(128, 64, 0, CCB_BMP))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 3:
		NDK_PrnStr("---���´�ӡCCB_BMP(xsize<128��xsize%8!=0)---\n");
		if ((nRet = NDK_PrnImage(128-1, 64, 0, CCB_BMP))!=NDK_OK)//�ײ���size��(+7/8)*8�Ĵ���
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 4://�����°����ӡ
		NDK_PrnStr("---���·����°����ӡCCB_BMP---\n");
		if ((nRet = NDK_PrnImage(128, 32, 0, CCB_BMP))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		if ((nRet = NDK_PrnImage(128, 32, 0, CCB_BMP+128*32/8))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 5://���Ҵ�ӡ
		NDK_PrnStr("---���¿��Ҵ�ӡCCB_BMP---\n");
		if ((nRet = NDK_PrnImage(128, 64, MAXPRLINEWIDE-128, CCB_BMP))!=NDK_OK)
		{
			NDK_PrnStart();
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 6://���ִ�ӡ
		//xsize+xpos>MAXPRLINEWIDE(����:xsize��xpos����Խ��)
		if ((nRet = NDK_PrnImage(128, 64, MAXPRLINEWIDE-128/2, CCB_BMP))!=NDK_ERR_PARA)//==NDK_OK)
		{
			
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 7://���ִ�ӡ//xsize+xpos>MAXPRLINEWIDE(����:xpos��Խ��,xsizeԽ��)
		memset(img, 0xff, sizeof(img));
		memset(img, 0xcc, (MAXPRLINEWIDE+7)/8);
		if ((nRet = NDK_PrnImage(sizeof(img)*8, 1, 8, img))!=NDK_ERR_PARA)//==NDK_OK)
		{
			
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 8://zhangnw20131016�����������ȷ��:����Ҫ֧�ִ���8���� //xpos����8������ʱ��8������������ 
		print_Ex("---���´�ӡ2��CCB_BMP(��߽�%sӦ����)---\n", /*(PRN_TYPE_TP ==nType)?"":*/"��");
		if ((nRet = NDK_PrnImage(128, 64, 16, CCB_BMP)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		if ((nRet = NDK_PrnImage(128, 64, 16+7, CCB_BMP)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	default:    /**< �������*/
		//xpos>MAXPRLINEWIDE����ʼλ��xposԽ�罫����ӡ�κζ���
		//xpos��0��ʼ��,��ôMAX_PRLINEWIDE-1��Խ��,��MAX_PRLINEWIDE����Խ�����
		if ((nRet = NDK_PrnImage(128, 64, MAXPRLINEWIDE+1, CCB_BMP))!=NDK_ERR_PARA)//==NDK_OK)//zhangnw20100902:��MAXPRLINEWIDE����ΪMAXPRLINEWIDE+1,�ſ�����
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		if ((nRet = NDK_PrnImage(128, 64, 0, NULL)) !=NDK_ERR_PARA)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		
		break;
	}

	return SUCC;
}

#if 0
static int testcase_MAXPRLINEWIDE_small(int MAXPRLINEWIDE, int subcase)
{
	/*private & local definition*/
#if !(defined GP730||defined GP710||defined GP730CP)
	char img[128] = {0};
#endif

	/*process body*/
	switch (subcase)
	{
	case 0:
		print("---���´�ӡNL_Icon�͵ȴ�С������ͼ---\n");
		if (printimage(16, 16*2, 0, NL_Icon)==FAIL)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;
	case 1:
		break;
	case 2:
		print("---���´�ӡNL_Icon---\n");
		if (printimage(16, 16, 0, NL_Icon)==FAIL)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;
	case 3:
		print("---���´�ӡNL_Icon(xsize<16��xsize%8!=0)---\n");
		if (printimage(16-1, 16, 0, NL_Icon)==FAIL)//�ײ���size��(+7/8)*8�Ĵ���
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;
	case 4://�����°����ӡ
		print("---���·����°����ӡNL_Icon---\n");
		if (printimage(16, 8, 0, NL_Icon)==FAIL)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		if (printimage(16, 8, 0, NL_Icon+16*8/8)==FAIL)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;
	case 5://���Ҵ�ӡ
		print("---���¿��Ҵ�ӡNL_Icon---\n");
		if (printimage(16, 16, MAXPRLINEWIDE-16, CCB_BMP)==FAIL)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		break;			
	case 6://���ִ�ӡ
		//xsize+xpos>MAXPRLINEWIDE(����:xsize��xpos��Խ��)
#if defined GP730||defined GP710||defined GP730CP
		if (printimage(16, 16, MAXPRLINEWIDE-16/2, NL_Icon)==SUCC)
#else
		print("---���´�ӡ����NL_Icon---\n");
		if (printimage(16, 16, MAXPRLINEWIDE-16/2, NL_Icon)==FAIL)
#endif
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
		//xsize<=0,xpos�ֺϷ�ʱ(MAXPRLINEWIDE>xpos>=0),xsize����MAXPRLINEWIDE-xpos��������
#if defined GP730||defined GP710||defined GP730CP
		if (printimage(-1, 16, MAXPRLINEWIDE-16/2, NL_Icon)==SUCC)
#else
		print("---���´�ӡ����NL_Icon---\n");
		if (printimage(-1, 16, MAXPRLINEWIDE-16/2, NL_Icon)==FAIL)
#endif
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}		
		break;
	case 7://���ִ�ӡ//xsize+xpos>MAXPRLINEWIDE(����:xpos��Խ��,xsizeԽ��)
#if !(defined GP730||defined GP710||defined GP730CP)
		print("---���´�ӡ����(xpos=8)---\n");
		memset(img, 0xff, sizeof(img));
		memset(img, 0xcc, (MAXPRLINEWIDE+7)/8);
		if (printimage(sizeof(img)*8, 1, 8, img)==FAIL)
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
#endif
		break;
	default://�������
		//xpos>MAXPRLINEWIDE
		//xpos��0��ʼ��,��ôMAX_PRLINEWIDE-1��Խ��,��MAX_PRLINEWIDE����Խ�����
		if (printimage(16, 16, MAXPRLINEWIDE+1, NL_Icon)==SUCC)//zhangnw20100902:��MAXPRLINEWIDE����ΪMAXPRLINEWIDE+1,�ſ�����
		{
			send_result("line %d: %s����ʧ��(xpos>PRLINEWIDE)", __LINE__, TESTAPI);
			return FAIL;
		}				
		//ysize<0
		if (printimage(16, -1, 0, NL_Icon)==SUCC)
		{
			send_result("line %d: %s����ʧ��(ysize<0)", __LINE__, TESTAPI);
			return FAIL;
		}		
		break;
	}		

	return SUCC;
}
#endif

/****************************************************************
* function name 	 				: void prnt2(void)
* functional description 	: ��ӡͼ��
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt2(void)
{
	//����Ϊ�ֲ�����
	time_t nOldtime=0,nDiff=0;
	int nBusy = FALSE,i=0,nKeyin=0;
	int nRow_normal=DISABLE, nCol_normal=DISABLE, nMode=0;
	EM_PRN_STATUS  emStatus=0;
	int MAXPRLINEWIDE_old =0;
	int MAXPRLINEWIDE = 0;
	int nRet =0;
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
	
	MAXPRLINEWIDE_old =(PRN_TYPE_TP==nType)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP==nType)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);

BEGIN:
	cls_show_msg1(2, "����%s(����%s,����%s)...\n", TESTAPI, nRow_normal==DISABLE?"�Ŵ�":"����", nCol_normal==DISABLE?"�Ŵ�":"����");
	print_Ex("����%s(����%s,����%s)...\n", TESTAPI, nRow_normal==DISABLE?"�Ŵ�":"����", nCol_normal==DISABLE?"�Ŵ�":"����");
	NDK_PrnStart();

	NDK_PrnSetMode(nMode, 0);
	
	MAXPRLINEWIDE = MAXPRLINEWIDE_old/((nRow_normal==DISABLE)?2:1);//����MAXPRLINEWIDE
	
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
			//test begin
			nBusy=FALSE;
			switch(i)
			{
			default:
				if(/*(MAXPRLINEWIDE>=MAXPRLINEWIDE_MID)&&*/(testcase_MAXPRLINEWIDE_big(MAXPRLINEWIDE, i )==FAIL))
					return;
#if 0	//��Ŀǰ���ݿ�,MAXPRLINEWIDE�ܻ����MAXPRLINEWIDE_MID,���ݷ��,��СĿ������С
				else if((MAXPRLINEWIDE<MAXPRLINEWIDE_MID)&&(testcase_MAXPRLINEWIDE_small(MAXPRLINEWIDE, i)==FAIL))
					return;
#endif
				break;
			case MAXCASECNT:
				if(transfer_state(&nRow_normal, &nCol_normal, &nMode)==SUCC)
				{
					i = 0;
					goto BEGIN;
				}

				//���ڱ������ܻ��޸�Ĭ�ϵ�ѹ��ģʽ,���ԱϺ�Ҫ�ָ�һ��
				//�ָ�Ĭ�ϵ�����
				switch (nType)
				{
				case PRN_TYPE_HIP:
				case PRN_TYPE_FIP:
					if(NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE,1) != NDK_OK)
					{
						send_result("line %d:���ô�ӡģʽ[PRN_MODE_ALL_DOUBLE, 1]ʧ��", __LINE__);
						goto prnt_error;
					}
					break;
				case PRN_TYPE_TP:
					if(NDK_PrnSetMode(PRN_MODE_NORMAL ,0) != NDK_OK)
					{
						send_result("line %d:���ô�ӡģʽ[PRN_MODE_NORMAL, 0]ʧ��", __LINE__);
						goto prnt_error;
					}
					break;
				default:
					send_result("line %d:δ�ܻ�ȡ����ӡ������", __LINE__);
					goto prnt_error;
					break;
				}				
				
				nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,��ENTERͨ��,��������ͨ��");
				if (nKeyin!=ENTER && nKeyin!=0)//zhangnw20061031 modify
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;//return;
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
			goto prnt_error;//return;
			break;
		}
	}
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}

