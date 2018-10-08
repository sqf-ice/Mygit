/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: LCD��Ļ����
* file name			: systest51.c
* Author 			: 	chensj
* version			: 
* DATE				: 
* directory 			: 
* description			:  LCD��Ļ����
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20131202  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"LCD��Ļ����"
#if 1//defined SP610
#define	PNGPATH	"" //ͳһ·�����ڵ�ǰmain����·����,���ʱ���һ���� 20170915
#else
#define	PNGPATH	"/appfs/apps/share/"
#endif
#if defined SP610  //Sp610ͼƬ��ʽ��.bin����β��������
#define	IMAGEFORMAT	".bin"
#else
#define	IMAGEFORMAT	".png"
#endif

/*------------global variables definition-----------------------*/
static int  DispTime = 1;
static int RunNum = 100;
static uint unWidth,unHeight;

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
* history 				: author			date			remarks
					 chensj 		2013/12/02            created
*****************************************************************/
//������ʾÿ��ͼƬ��ʱ��,0Ϊ����,������ҳ
void Scr_Config(void)
{
	char szBuf[3]={"1"},szNumBuf[4]={"100"};
	uint unLen=0;
	int ret = 0;
	int  keyin = 0;

	while(1)
	   {
		keyin = cls_show_msg("1.ͼƬ��ʾʱ��\n"
							  "2.ѹ������");
		switch(keyin) 
		{
		case '1':
			cls_printf("��������ʾÿ��ͼƬ��ʱ��:");
			if((ret = lib_kbgetinput(szBuf, 1, 2, &unLen, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			{	
				DispTime = 2;
				return;
			}
			DispTime = atoi(szBuf);
			break;
		case '2':
			cls_printf("������ѹ������:");
			if((ret = lib_kbgetinput(szNumBuf, 1, 3, &unLen, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			{	
				RunNum = 100;
				return;
			}
			RunNum = atoi(szNumBuf);
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

//�������
void Dead_Pixel(void)
{
	//char *szPng[]={"huidu_0.png","huidu_100.png","red.png","green.png","blue.png","pink.png","cyan.png","yellow.png"};
	char *szPng[]={"huidu_0","huidu100","red","green","blue","pink","cyan","yellow"};//Sp610ͼƬ��ʽ��.bin����β��������
	char szBuf[64]={0};
	int i=0,ret = 0,nCode=0,RealRunNum = 0;

	RealRunNum = RunNum;
	NDK_ScrClrs();
	while(RealRunNum--)
	{
		while(1)
		{
			if(i==SZ_ARRAY(szPng))
			{
				i=0;
				break;
			}
			memset(szBuf,0,sizeof(szBuf));
			sprintf(szBuf,"%s%s%s",PNGPATH,szPng[i],IMAGEFORMAT);
			if((ret = NDK_ScrDispPic(0,0,unWidth,unHeight,szBuf))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode==ESC)
				goto END;
			i++;
		}
	}
END:
	if(cls_show_msg("�����洿ɫͼƬ�л��������Ƿ��ֻ���,[ENTER] �� [����] ��")==ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��,������Ļ����", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("����ͨ��,δ���ֻ���");
	return;
}

//�������
void Light_Pixel(void)
{
	//char *szPng[]={"huidu_0.png","red.png","huidu_0.png","green.png","huidu_0.png","blue.png"};
	char *szPng[]={"huidu_0","red","huidu_0","green","huidu_0","blue"};
	char szBuf[64]={0};
	int i=0,ret =0,nCode=0,RealRunNum = 0;

	RealRunNum = RunNum;
	NDK_ScrClrs();
	while(RealRunNum--)
	{
		while(1)
		{
			if(i==SZ_ARRAY(szPng))
			{
				i = 0;
				break;
			}
			memset(szBuf,0,sizeof(szBuf));
			sprintf(szBuf,"%s%s%s",PNGPATH,szPng[i],IMAGEFORMAT);
			if((ret = NDK_ScrDispPic(0,0,unWidth,unHeight,szBuf))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode==ESC)
				goto END;
			i++;
		}
	}
END:
	if(cls_show_msg("������ں���̺���ͼƬ�л��������Ƿ�����Ƚ����ĵ�,[ENTER] �� [����] ��")==ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��,������Ļ����", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("����ͨ��,δ��������");
	return;
}

//�������
void Dark_Pixel(void)
{
	//char *szPng[]={"huidu_100.png","red.png","huidu_100.png","green.png","huidu_100.png","blue.png"};
	char *szPng[]={"huidu100","red","huidu100","green","huidu100","blue"};
	char szBuf[64]={0};
	int i=0,ret = 0,nCode=0,RealRunNum = 0;

	RealRunNum = RunNum;
	NDK_ScrClrs();
	while(RealRunNum--)
	{
		while(1)
		{
			if(i==SZ_ARRAY(szPng))
			{
				i=0;
				break;
			}
			memset(szBuf,0,sizeof(szBuf));
			sprintf(szBuf,"%s%s%s",PNGPATH,szPng[i],IMAGEFORMAT);
			if((ret =NDK_ScrDispPic(0,0,unWidth,unHeight,szBuf))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode==ESC)
				goto END;
			i++;
		}
	}
END:
	if(cls_show_msg("������׺���̰���ͼƬ�л��������Ƿ��ַ��ֱȽϰ��ĵ�,[ENTER] �� [����] ��")==ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��,������Ļ����", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("����ͨ��,δ���ְ���");
	return;
}

//��Ļ���ص����:���Ի��㣬���㣬����
void Screen_Pixel(void)
{
	int  keyin = 0;

	/*if(cls_show_msg("��ȷ����%sĿ¼����huidu_0.png huidu_100.png red.png green.png\
		blue.png pink.png cyan.png yellow.pngͼƬ,[ESC]�˳�",PNGPATH)==ESC)*/
	if(cls_show_msg("��ȷ����%sĿ¼����huidu_0 huidu100 red green blue pink cyan yellow %s ͼƬ,[ESC]�˳�",PNGPATH,IMAGEFORMAT)==ESC)
		return;
	
	while(1)
       {
		keyin = cls_show_msg("1.�������\n"
							  "2.�������\n"
							  "3.�������");
		switch(keyin) 
		{
		case '1':
			Dead_Pixel();
			break;
		case '2':
			Light_Pixel();
			break;
		case '3':
			Dark_Pixel();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
}

//�ҶȽ������:��0������100�ٵݼ���0Ϊһ�ֲ���
void Gray_Gradient_Test(void)
{
	//char *szPng[]={"huidu_0.png","huidu_25.png","huidu_50.png","huidu_75.png","huidu_100.png","huidu_75.png","huidu_50.png","huidu_25.png"};
	char *szPng[]={"huidu_0","huidu_25","huidu_50","huidu_75","huidu100","huidu_75","huidu_50","huidu_25"};
	char szBuf[64]={0};
	int i=0,ret = 0,nCode=0,RealRunNum = 0;
	
	RealRunNum = RunNum;
	NDK_ScrClrs();
	while(RealRunNum--)
	{
		while(1)
		{
			if(i==SZ_ARRAY(szPng))
			{
				i=0;
				break;
			}
			memset(szBuf,0,sizeof(szBuf));
			sprintf(szBuf,"%s%s%s",PNGPATH,szPng[i],IMAGEFORMAT);
			if((ret =NDK_ScrDispPic(0,0,unWidth,unHeight,szBuf))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode==ESC)
				goto END;
			i++;
		}
	}
END:
	if(cls_show_msg("�ҶȽ����Ƿ�����,[ENTER] �� [����] ��")!=ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("�ҶȽ������ͨ��");
	return;
}

//�Ҷȵȼ�����
void Grey_Scale_Test(void)
{
	//char *szPng[]={"huidu_h1.png","huidu_h2.png","huidu_v1.png","huidu_v2.png"};
	char *szPng[]={"huidu_h1","huidu_h2","huidu_v1","huidu_v2"};
	char szBuf[64]={0};
	int i=0,ret = 0,nCode=0,RealRunNum = 0;
	
	RealRunNum = RunNum;
	NDK_ScrClrs();
	while(RealRunNum--)
	{
		while(1)
		{
			if(i==SZ_ARRAY(szPng))
			{
				i=0;
				break;
			}
			memset(szBuf,0,sizeof(szBuf));
			sprintf(szBuf,"%s%s%s",PNGPATH,szPng[i],IMAGEFORMAT);
			if((ret =NDK_ScrDispPic(0,0,unWidth,unHeight,szBuf))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode==ESC)
				goto END;
			i++;
		}
	}
END:
	if(cls_show_msg("�Ҷȵȼ�ͼƬ��ʾ�Ƿ�����,[ENTER] �� [����] ��")!=ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("�Ҷȵȼ�ͼƬ����ͨ��");
	return;
}

//��Ļ�ҶȲ���
void Screen_Gray(void)
{
	int  keyin = 0;

	if(cls_show_msg("��ȷ����%sĿ¼����huidu_0 huidu_25 huidu_50 huidu_75 huidu100\
		huidu_h1 huidu_h2 huidu_v1 huidu_v2 huidu2 huidu44 %sͼƬ,[ESC]�˳�",PNGPATH,IMAGEFORMAT)==ESC)
		return;
	
	while(1)
       {
		keyin = cls_show_msg("1.�ҶȽ������\n"
							  "2.�Ҷȵȼ�����\n");
		switch(keyin) 
		{
		case '1':
			Gray_Gradient_Test();
			break;
		case '2':
			Grey_Scale_Test();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
}

//���Ų���:���Թ����а�ȷ�ϼ�ʹ��Ļ��ͣ��۲���Ļ�ڰױ߽紦�Ƿ���ڸ���
void Cross_talk(void)
{
	int start_x1_point = 0, start_y1_point = 0, start_x_len = 0,start_y_len = 0, flag = 1, ret = 0, nCode = 0;

	if(unHeight>= unWidth)	
	{
		start_x1_point = (unWidth/2)-1;
		start_y1_point = start_x1_point;
		start_x_len = 2;
		start_y_len = unHeight-(unWidth-2);
	}	
	else
	{
		start_x1_point = (unHeight/2)-1;
		start_y1_point = start_x1_point;
		start_x_len =  unWidth-(unHeight-2);
		start_y_len = 2;
	}
	
	NDK_ScrClrs();
	NDK_ScrRefresh();
	while(1)
	{
		NDK_KbFlush();
		while(flag)
		{
			if((ret = NDK_ScrRectangle(start_x1_point,start_y1_point,start_x_len,start_y_len,RECT_PATTERNS_NO_FILL,0))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��(%d)", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			NDK_SysMsDelay(10);
			if((start_x1_point--)==0)
			{
				flag = 0;
				++start_x1_point;
				if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode == ESC)
					goto END;
				break;
			}
			start_y1_point--;
			start_x_len+=2;
			start_y_len+=2;	
			if(NDK_KbHit(&nCode)==NDK_OK)
			{
				if(nCode==ENTER)
				{
					NDK_KbGetCode(0, NULL);
					nCode = 0;
				}
				else if(nCode == ESC)
				{
					goto END;
				}
			}
		}
		NDK_KbFlush();
		while(!flag)
		{
			if((ret = NDK_ScrRectangle(start_x1_point,start_y1_point,start_x_len,start_y_len,RECT_PATTERNS_NO_FILL,0xFFFF))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
				return;
			}
			NDK_ScrRefresh();
			NDK_SysMsDelay(10);
			if((start_x1_point++)==((unHeight>= unWidth)?((unWidth/2)-1):((unHeight/2)-1)))
			{
				flag = 1;
				--start_x1_point;
				if(NDK_KbGetCode(DispTime, &nCode)==NDK_OK&&nCode == ESC)
					goto END;
				break;
			}
			start_y1_point++;
			start_x_len-=2;
			start_y_len-=2;
			if(NDK_KbHit(&nCode)==NDK_OK)
			{
				if(nCode==ENTER)
				{
					NDK_KbGetCode(0, NULL);
					nCode = 0;
				}
				else if(nCode == ESC)
				{
					goto END;
				}
			}
		}
	}
END:
	if(cls_show_msg("CrossTalk������;��ENTERͣס��߽��Ƿ�����,[ENTER] �� [����] ��")!=ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��", TESTITEM, __LINE__);
		return;
	}
	cls_show_msg("CrossTalk����ͨ��");
	return ;
}

//����ͼ��Ϊ�����̸񡱣����û�����ʾ12Сʱ���ϣ����л���ȫ��50%�ҶȵĻ��棬���Ƿ��в��������̸񡱡�
void Ghosting_Test(void)
{
	//char *szPng[]={"qipan.png","huidu_50.png"};
	char *szPng[]={"qipan","huidu_50"};
	char szBuf[64]={0};
	int i=0,ret = 0,nCode=0,RealRunNum = 0;

	if(cls_show_msg("��������ر����߿���,Ϊ���������,���̵��Ҷ�ͼƬ��1,�Ҷ�ͼƬ���˳�������3")==ESC)
		return;

	RealRunNum = RunNum;
	NDK_ScrClrs();
	while(1)
	{
		if(i==SZ_ARRAY(szPng))
			break;
	
		memset(szBuf,0,sizeof(szBuf));
		sprintf(szBuf,"%s%s%s",PNGPATH,szPng[i],IMAGEFORMAT);
		if((ret =NDK_ScrDispPic(0,0,unWidth,unHeight,szBuf))!=NDK_OK)
		{
			cls_show_msg1(2, "%s,line %d����ʧ��%d", TESTITEM,__LINE__,ret);
			return;
		}
		NDK_ScrRefresh();
	START:
		if(NDK_KbGetCode(0, &nCode)==NDK_OK&&((i==0&&nCode=='1')||(i==1&&nCode == '3')))
			;
		else
			goto START;
		i++;
	}
	
	if(cls_show_msg("�������л����Ҷ�50�Ƿ������̲�Ӱ,[ENTER] �� [����] ��")==ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("��Ӱ����ͨ��");
	return;
}

//��Ӱ����:��ɫС�����ƶ���������Ļ�ϲ�Ӧ�ó��ְ�ɫ��β
void Ghosting_Test2(void)
{
	int start_x1_point = 0, start_y1_point = 0, len = 10, flagx = 1, flagy = 1, ret = 0, nCode = 0;

	NDK_ScrClrs();
	NDK_ScrRefresh();
	//fprintf(stderr,"xxx=%d,yyy=%d\n",GUI_GetLCDWidth(),GUI_GetLCDHeight());
	while(1)
	{
		//fprintf(stderr,"x=%d,y=%d\n",start_x1_point,start_y1_point);
		if((ret = NDK_ScrRectangle(0,0,unWidth,unHeight,RECT_PATTERNS_SOLID_FILL,0))!=NDK_OK)//ȫ����ɫ
		{
			cls_show_msg1(2, "%s,line %d����ʧ��(%d)", TESTITEM,__LINE__,ret);
			return;
		}
		if((ret = NDK_ScrRectangle(start_x1_point,start_y1_point,len,len,RECT_PATTERNS_SOLID_FILL,0xffff))!=NDK_OK)//��ɫС������(ģ�����)
		{
			cls_show_msg1(2, "%s,line %d����ʧ��(%d)", TESTITEM,__LINE__,ret);
			return;
		}
		if((ret = NDK_ScrLine(start_x1_point,start_y1_point,start_x1_point+len-1,start_y1_point+len-1,0))!=NDK_OK)//��ɫС�������еĺ�ɫб��
		{
			cls_show_msg1(2, "%s,line %d����ʧ��(%d)", TESTITEM,__LINE__,ret);
			//fprintf(stderr,"x=%d,y=%d\n",start_x1_point,start_y1_point);
			return;
		}

		NDK_ScrRefresh();
		NDK_SysMsDelay(6);
		//�޸ı����־
		if(start_x1_point==0)
			flagx = 1;
		if(start_x1_point==(unWidth-len))
			flagx = 0;
		if(start_y1_point==0)
			flagy = 1;
		if(start_y1_point==(unHeight-len))
			flagy = 0;
		//���ݱ����־�޸���ʼ����
		if(flagx==1&&flagy==1)//���Ͻ�->���½��ƶ�
		{
			start_x1_point++;
			start_y1_point++;
		}
		if(flagx==0&&flagy==1)//���Ͻ�->���½��ƶ�
		{
			start_x1_point--;
			start_y1_point++;
		}
		if(flagx==0&&flagy==0)//���½�->���Ͻ��ƶ�
		{
			start_x1_point--;
			start_y1_point--;
		}
		if(flagx==1&&flagy==0)//���½�->���Ͻ��ƶ�
		{
			start_x1_point++;
			start_y1_point--;
		}

		if(NDK_KbHit(&nCode)==NDK_OK)
		{
			if(nCode == ESC)
				break;//�˳�����
		}
	}

	if(cls_show_msg("��ɫС�����ƶ���������Ļ�Ƿ������Ӱ,[ENTER] �� [����] ��")==ENTER)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��", TESTITEM,__LINE__);
		return;
	}
	cls_show_msg("��Ӱ����ͨ��");
	return;
}

void Screen_Ghosting(void)
{
	int  keyin = 0;

	if(cls_show_msg("��ȷ����%sĿ¼����huidu_50��qipan %sͼƬ,[ESC]�˳�",PNGPATH,IMAGEFORMAT)==ESC)
		return;
	
	while(1)
       {
		keyin = cls_show_msg("1.Crosstalk����\n"
							  "2.��Ӱ����\n"
							  "3.��Ӱ����\n");
		switch(keyin) 
		{
		case '1':
			Cross_talk();
			break;
		case '2':
			Ghosting_Test();
			break;
		case '3':
			Ghosting_Test2();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
}

void systest51(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0, ret = -1;
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
#if !K21_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE))!=NDK_OK)
	{
		cls_show_msg("״̬���ر�ʧ��!(%d)", ret);
		return;
	}
#endif	
//	NDK_ScrGetViewPort(NULL, NULL, &unWidth, &unHeight);
	unWidth = GUI_GetLCDWidth();
	unHeight = GUI_GetLCDHeight();
	
	while(1)
       {
		keyin = cls_show_msg("1.���ص����\n"
							"2.�ҶȲ���\n"
							"3.��Ӱ����\n"
							"0.�ȴ�ʱ������");
		switch(keyin) 
		{
		case '1':
			Screen_Pixel();
			break;
		case '2':
			Screen_Gray();
			break;
		case '3':
			Screen_Ghosting();
			break;
		case '0':
			Scr_Config();
			break;
		case ESC:
#if !K21_ENABLE
			if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_WLSIGNAL|STATUSBAR_DISP_BATTERY|STATUSBAR_DISP_DATE|STATUSBAR_DISP_TIME|STATUSBAR_DISP_ETH|STATUSBAR_DISP_MODEM|STATUSBAR_DISP_WIFI))!=NDK_OK)
			{
				cls_show_msg("״̬����ʾʧ��!(%d)", ret);
				return;
			}
#endif
			return;
		default:
			continue;
			break;
		}
	}
	
	//test over
	return;
}
