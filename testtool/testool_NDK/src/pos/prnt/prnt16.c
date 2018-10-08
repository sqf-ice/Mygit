/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt16.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnPicture�ӿڹ���
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
#define TESTAPI	"NDK_PrnPicture"
#define TIMEOUT 30
#define MAXWAITTIME 180
#define MAXCASECNT	8

#define NOEXIST "fnnnnnn.bmp"

#define ICOTUBIAO "tubiao.ico"
#define IMAGEPATH  "" //����ͼƬ���ڵ�ǰ·���£�������
#define	BMPFORMAT ".bmp"
#define PNGFORMAT ".png"
#define JPGFORMAT ".jpg"

//������bmp��png��jpgͼƬ���й������ϣ����ٺ궨�弰���� 20180301
//#define GUANGFABANKPNG "guangfabank.png"
//#define ABCBANKJPG "ABC.jpg"
#define ABCBANKBMP "abclogo.bmp"
//#define MEITUANPNG     "png02.png"
//#define FONTPNG           "png03.png"
//#define CARDPNG           "png04.png"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
static int testcase(int subcase)
{
	int nRet = 0, i = 0;
	char *szBmp[]={"abclogo","bmp01","bmp02"};
	char *szPng[]={"color1", "color2", "color3", "color4", "color5", "color6", "IHDR1", "IHDR2", "IHDR3", 
		"IHDR4", "IHDR5", "IHDR6", "IHDR7", "IHDR8", "IHDR9", "IHDR10", "IHDR11", "IHDR12", "IHDR13",
		"IHDR14", "IHDR15", "other1", "other2", "other3", "other4", "other5", "other6", "other7", "other8", 
		"other9", "other10", "other11", "other12", "other13", "other14", "other15", "other16", "ysz1",
		"ysz2", "ysz3", "ysz4", "ysz5", "ysz6", "ysz7", "guangfabank"};
	char *szJpg[]={"jsyh", "kbd", "ldcr", "logo", "png", "stw", "tcd", "xdl", "ABC"};
	char szBuf[64]={0};
	char hintBuf[128]={0},hintBuf1[64]="���´�ӡ:";
	
	switch(subcase)
	{
	case 1://case1:·��Ϊ��
		if((nRet = NDK_PrnPicture(0,NULL)) != NDK_ERR_PARA)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 2://case2:ͼƬ������
		NDK_FsDel(NOEXIST);
		if((nRet = NDK_PrnPicture(0,NOEXIST)) != NDK_ERR_PATH)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 3://case3:��֧�ֵ�ͼƬ��ʽico
		if((nRet = NDK_PrnPicture(0,ICOTUBIAO)) != NDK_ERR_DECODE_IMAGE)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 4://case4:xpos+xsize(�����ͼƬ�Ŀ��ֵ) > ndk_PR_MAXLINEWIDE������ģʽΪ384��
		if((nRet = NDK_PrnPicture(350, ABCBANKBMP)) != NDK_ERR_PARA)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		break;
	case 5://case5:xpos+xsize(�����ͼƬ�Ŀ��ֵ) > ndk_PR_MAXLINEWIDE  ����Ŵ�ʱΪ384/2
		NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE  ,0);
		if((nRet = NDK_PrnPicture(240, ABCBANKBMP)) != NDK_ERR_PARA)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		NDK_PrnSetMode(PRN_MODE_NORMAL,0);
		break;
		
	case 6://case6:��ӡbmp
	#if 0 //�����˲������ݣ�������bmpͼƬ���ϵ�һ�𣬴˲����������������Ĳ������� 20180301 sull modify
		NDK_PrnStr("---���´�ӡ����Logo---");
		if((nRet = NDK_PrnPicture(0,ABCBANKBMP)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
	#endif
		NDK_PrnStr("--���´�ӡ����bmpͼƬ(���в�ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ)--\n");
		for(i=0; i<SZ_ARRAY(szBmp); i++)
		{
			memset(szBuf, 0, sizeof(szBuf));
			memset(hintBuf, 0, sizeof(hintBuf));
			sprintf(szBuf,"%s%s%s",IMAGEPATH,szBmp[i],BMPFORMAT);
			sprintf(hintBuf,"%s%s",hintBuf1,szBuf);
			NDK_PrnStr(hintBuf);
			if((nRet = NDK_PrnPicture(0,szBuf)) != NDK_OK)
			{
				send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI,nRet,szBuf);
				return FAIL;
			}
		}
		break;
		
	case 7://case7:��ӡpng
	#if 0 //�����˲������ݣ�������pngͼƬ���ϵ�һ�𣬴˲����������������Ĳ������� 20180301 sull modify
		NDK_PrnStr("---���´�ӡ�㷢Logo---");
		if((nRet = NDK_PrnPicture(0,GUANGFABANKPNG)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		NDK_PrnStr("---���´�ӡ��������Logo(��ɫͼƬǳ��ɫ��ӡ�ɰ�ɫ,����ɫ��ӡ�ɺ�ɫ)---");
		if((nRet = NDK_PrnPicture(0,MEITUANPNG)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		NDK_PrnStr("---���´�ӡ���ƾ����(��ɫͼƬǳ��ɫ��ӡ�ɰ�ɫ,����ɫ��ӡ�ɺ�ɫ)---");
		if((nRet = NDK_PrnPicture(0,FONTPNG)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
		NDK_PrnStr("---���´�ӡС����ͼ��(��ɫͼƬǳ��ɫ��ӡ�ɰ�ɫ,����ɫ��ӡ�ɺ�ɫ)---");
		if((nRet = NDK_PrnPicture(0,CARDPNG)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
	#endif
		NDK_PrnStr("--���´�ӡ����pngͼƬ(���в�ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ)--\n");
		for(i=0; i<SZ_ARRAY(szPng); i++)
		{
			memset(szBuf, 0, sizeof(szBuf));
			memset(hintBuf, 0, sizeof(hintBuf));
			sprintf(szBuf,"%s%s%s",IMAGEPATH,szPng[i],PNGFORMAT);
			sprintf(hintBuf,"%s%s",hintBuf1,szBuf);
			NDK_PrnStr(hintBuf);
			if((nRet = NDK_PrnPicture(0,szBuf)) != NDK_OK)
			{
				send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI,nRet,szBuf);
				return FAIL;
			}
		}
		break;
	
	case 8://case8:��ӡjpg
	#if 0 //�����˲������ݣ�������pngͼƬ���ϵ�һ�𣬴˲����������������Ĳ������� 20180301 sull modify
		NDK_PrnStr("---���´�ӡ����ͨLogo---");
		if((nRet = NDK_PrnPicture(0,ABCBANKJPG)) != NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			return FAIL;
		}
	#endif
		NDK_PrnStr("--���´�ӡ����jpgͼƬ(���в�ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ)--\n");
		for(i=0; i<SZ_ARRAY(szJpg); i++)
		{
			memset(szBuf, 0, sizeof(szBuf));
			memset(hintBuf, 0, sizeof(hintBuf));
			sprintf(szBuf,"%s%s%s",IMAGEPATH,szJpg[i],JPGFORMAT);
			sprintf(hintBuf,"%s%s",hintBuf1,szBuf);
			NDK_PrnStr(hintBuf);
			if((nRet = NDK_PrnPicture(0,szBuf)) != NDK_OK)
			{
				send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI,nRet,szBuf);
				return FAIL;
			}
		}
		break;
	}
	
	return SUCC;
}

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt16(void)
* functional description 	:��ӡbmp��png�ȸ�ʽ��ͼƬ  
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt16(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int i = 1;
	int nRet=0/*,nRet1=0,nRet2=0*/,nRet3=0/*,nRet4=0,nRet5=0,nRet6=0*/;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;
	}
	if(cls_show_msg("��ȷ����ǰ·���´�������ͼƬ,[ȷ��]����[����]����")==ENTER)
	{
		if((nRet = NDK_FsExist(ABCBANKBMP))!=NDK_OK/*||(nRet1=NDK_FsExist(GUANGFABANKPNG))!=NDK_OK||(nRet2=NDK_FsExist(ABCBANKJPG))!=NDK_OK \*/
			||(nRet3 = NDK_FsExist(ICOTUBIAO))!=NDK_OK/*||(nRet4 = NDK_FsExist(MEITUANPNG))!=NDK_OK||(nRet5 = NDK_FsExist(FONTPNG))!=NDK_OK||(nRet6 = NDK_FsExist(CARDPNG))!=NDK_OK*/)//�����˲������ݣ�������ͼƬ���ϵ�һ�� 20180301 sull modify
		{
			send_result("line %d:%s����ʧ��(%d%d)",__LINE__,TESTAPI,nRet,nRet3);
			goto prnt_error;
		}
	}
	else
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
				if(testcase(i)!=SUCC)	
					goto prnt_error;
				break;
			case MAXCASECNT+1:
				//test over
				nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,���ӡͼƬ���бȽ�,��ENTERͨ��,��������ͨ��");
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

