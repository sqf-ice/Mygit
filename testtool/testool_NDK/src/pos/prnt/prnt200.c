/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ӡģ��
* file name			: 
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ���Դ�ӡģ�����������
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define HANZI12 "hzk12"
#define ABCBANKBMP "abclogo.bmp"
#define FILENAME35  "angsa.ttf"

#define	TESTAPI	"��ӡģ����������"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
	PrnGetStatus ,		// 0
	PrnImage,		
	PrnStr,	
	PrnInit,//3           		
	PrnStart,		//4//
	PrnGetVersion,  
	PrnSetFont, 
	PrnSetMode,  
	PrnSetGreyScale, 
	PrnSetForm, 
	PrnFeedByPixel,  
	PrnSetUnderLine,  	 //11
#if !ANDIROD_ENABLE
	PrnPicture, 
#if !defined SP610
	PrnFeedPaper,
#endif
#if !defined GP720
	PrnFontRegister,   
	PrnSetUsrFont,  
#if CPU5892_ENABLE ||CPU5830X_ENABLE|| OVERSEAS_ENABLE
	PrnGetDotData, 
#endif
#else
	PrnSetPageLen,  //������֧��
#endif	
#if OVERSEAS_ENABLE
	PrnInitTrueTypeFont,
	PrnDestroyTrueTypeFont,
	PrnSetTrueTypeFontSizeInPixel,
	PrnGetTrueTypeFontSize,
	PrnSetTrueTypeFontEncodingFormat,
	PrnTrueTypeFontText,
#endif
#else
	Script_Print,		//12
#endif
	MaxNum,
}EM_PRNT;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						sull					20170902		created
*****************************************************************/
#if !ANDIROD_ENABLE
static int func12(char m[], ST_PRN_RECMSG *p)
{
	p->nOffset = (94*((m[0]-0xa0)-1) +(m[1]-0xa0)-1)*24;
	p->nFontByte = 24;
	return 1;
}
#endif
static int prnt_randtest(int funnum)
{
	uint grey = 0;
	EM_PRN_STATUS  emStatus=0;
	char VerBuf[32]={0};
	//char sText[1024] = {0};
#if !ANDIROD_ENABLE
#if !defined GP720
	ST_PRN_FONTMSG stMsg;
#if CPU5892_ENABLE ||CPU5830X_ENABLE|| OVERSEAS_ENABLE
	char str[]="����������:1234590!@#()*&";
	char prnbuf[4096] = {0};
#endif 
#endif
#if OVERSEAS_ENABLE
	int getwidth=0, getheight=0;
	char asc_str[]="Innovation in China 0123456789!";
	char asc_str1[]="Hello NewLand123! Please Enter Any Key to Continue!";
#endif
#else
	char buf_str[] = {"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�120����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n"};
#endif
	uint rand1 = 0, rand2 = 0, rand3 = 0;
	switch(funnum)
	{	
		case PrnGetStatus:	
			NDK_PrnGetStatus(&emStatus);
			break;
		case PrnImage:  
			NDK_PrnImage(128, 64, 0, CCB_BMP);
			break;
		case PrnStr:
			NDK_PrnStr("NEWLand�´�½!#@%$%^&*()__+|><:;");
			break;
		case PrnInit:
			NDK_PrnInit(rand()%2);
			break;
		case PrnStart:
			NDK_PrnStart();
			break;
		case PrnGetVersion:
			memset(VerBuf,0,sizeof(VerBuf));
			NDK_PrnGetVersion(VerBuf);
			break;
		case PrnSetFont:
			NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);
			break;
		case PrnSetMode:
			NDK_PrnSetMode(rand()%4,0);
			break;
		case PrnSetGreyScale:
			srand(time(NULL));
			grey = rand()%6;
			NDK_PrnSetGreyScale(grey);
			break;
		case PrnSetForm:
			srand((unsigned)time(NULL));
			rand1=rand()%289;
			rand2=rand()%256;
			rand3=rand()%256;
			NDK_PrnSetForm(0,0,0);
		//	NDK_PrnSetForm(rand1,rand2,rand3);//�˴����������������(���ص�bug)
			break;
		case PrnFeedByPixel:
			srand((unsigned)time(NULL));
			NDK_PrnFeedByPixel(rand()%793); //���������ۺ�
			break;
		case PrnSetUnderLine:
			NDK_PrnSetUnderLine((rand()%2));
			break;
#if !ANDIROD_ENABLE
		case PrnPicture:
			NDK_PrnPicture(0,ABCBANKBMP);
			break;
#if !defined SP610
		case PrnFeedPaper:
			NDK_PrnFeedPaper((rand()%2));
			break;
#endif
#if !defined GP720
		case PrnFontRegister:
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum = 0;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 0;
			stMsg.nIsHZ = 1;
			stMsg.pszName = HANZI12;
			stMsg.func = func12;
			NDK_PrnFontRegister(&stMsg);
			break;
		case PrnSetUsrFont:
			NDK_PrnSetUsrFont(0);
			break;
#if CPU5892_ENABLE ||CPU5830X_ENABLE|| OVERSEAS_ENABLE
		case PrnGetDotData:
			memset(prnbuf,0,sizeof(prnbuf));
			NDK_PrnGetDotData(PRN_GETFONT_16X16,str,prnbuf,strlen(str)*8,16);
			break;
#endif
#else
		case PrnSetPageLen:
			srand(time(NULL));
			NDK_PrnSetPageLen(rand()%793);
			break;
#endif
#if OVERSEAS_ENABLE
		case PrnInitTrueTypeFont:
			NDK_PrnInitTrueTypeFont(FILENAME35, 0);
			break;
		case PrnDestroyTrueTypeFont:
			NDK_PrnDestroyTrueTypeFont();
			break;
		case PrnSetTrueTypeFontSizeInPixel:
			NDK_PrnSetTrueTypeFontSizeInPixel(12, 12);
			break;
		case PrnGetTrueTypeFontSize:
			NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight);
			break;
		case PrnSetTrueTypeFontEncodingFormat:
			NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII);
			break;
		case PrnTrueTypeFontText:
			NDK_PrnTrueTypeFontText(0, asc_str1, asc_str1);
			break;	
#endif
#else
	case Script_Print:
		NDK_Script_Print(buf_str,strlen(buf_str));
		break;
#endif
	}
	return SUCC;
}
//��ӡ������Ի����ܷ�����:ԭ�򿭸��Ѳ������NDK_PrnSetForm�����������������������֮�����NDK_PrnSetUnderLine�ܷ�
void prnt200(void)
{
	/*private & local definition*/
	int num[50] ={0}, i = 0, bak = 0;
	int ret = -1,cnt =50, len = 0, j = 0;

	/*process body*/
	if(MaxNum>20)
		len = MaxNum;
	else
		len = 20;
	//����ǰ��
	NDK_PrnInit(g_PrnGrey);
	
	while(cnt)
	{	
		cnt--;
		bak++;
		//���������
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
		}
		cls_printf("��%d�����˳��:",bak);
		for(i=0;i<len;i++)
		{
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
			
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret = prnt_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%d ",num[j]);
				goto ERR;
			}
			NDK_PrnStr("��ӡģ���������");
		}	
	}
	send_result("%s����ͨ��", TESTAPI);

ERR:
	NDK_PrnInit(g_PrnSwitch);
	NDK_PrnSetGreyScale(g_PrnGrey);
#if OVERSEAS_ENABLE
	NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII);
	NDK_PrnDestroyTrueTypeFont();
#endif
	return;
}


