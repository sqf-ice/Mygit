/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 37.c
* Author 			: zhengry
* version			: 
* DATE			: 20170427
* directory 		: 
* description		: NDK_ScrSetTrueTypeFontEncodingFormat���������ʽ��NDK_ScrDispTrueTypeFontTextʹ���Ѽ��ص�TTF������ʾ����
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScrSetTrueTypeFontEncodingFormat��NDK_ScrDispTrueTypeFontText"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
#define FILENAME37  "angsa.ttf"
#define FILENAME2  "msgothic.ttc"
#define FILENAME3 "arial.ttf"
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr37
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhengry         20170427  	   created
*****************************************************************/
void scr37(void)
{
	/*private & local definition*/
	int ret=0;
	char asc_str[]="Hello NewLand123! Please Enter Any Key to Continue!";// ֻ����ʾttf�����е�����
	char uni_str[]={0x48, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x4E, 0x00, 0x65, 0x00, 
					0x77, 0x00, 0x4C, 0x00, 0x61, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 
					0x21, 0x00, 0x20, 0x00, 0x50, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 
					0x20, 0x00, 0x45, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x41, 0x00, 
					0x6E, 0x00, 0x79, 0x00, 0x20, 0x00, 0x4B, 0x00, 0x65, 0x00, 0x79, 0x00, 0x20, 0x00, 0x74, 0x00, 
					0x6F, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6E, 0x00, 
					0x75, 0x00, 0x65, 0x00, 0x21, 0x00};//��asc_str�ַ�����unicode��ʽ
	char utf8_str[]={0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x4E, 0x65, 0x77, 0x4C, 0x61, 0x6E, 0x64, 0x31, 0x32, 0x33,
					 0x21, 0x20, 0x50, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20, 0x45, 0x6E, 0x74, 0x65, 0x72, 0x20, 0x41,
					 0x6E, 0x79, 0x20, 0x4B, 0x65, 0x79, 0x20, 0x74, 0x6F, 0x20, 0x43, 0x6F, 0x6E, 0x74, 0x69, 0x6E, 
					 0x75, 0x65, 0x21 };
	char asc_str1[]={0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0xD0, 0xC2, 0xB4, 0xF3, 0xC2, 0xBD};//"hello�´�½"ascii����16��������
	char uni_str1[]={0x48, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x20, 0x00, 0xB0, 0x65, 0x27, 0x59, 0x46, 0x96};
	char utf8_msg_str[]={0xE3, 0x81, 0x93, 0xE3, 0x82, 0x93, 0xE3, 0x81, 0xAB, 0xE3, 0x81, 0xA1, 0xE3, 0x81, 0xAF, 0xE3,
						 0x80, 0x81, 0xE3, 0x81, 0x93, 0xE3, 0x82, 0x8C, 0xE3, 0x81, 0xAF, 0xE3, 0x83, 0x86, 0xE3, 0x82, 
						 0xB9, 0xE3, 0x83, 0x88, 0xE3, 0x81, 0xAE, 0xE4, 0xBE, 0x8B, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99 };//���İ��:���,���ǲ�������

	char utf8_arial_str[]={0xD8, 0xB3, 0xD9, 0x84, 0xD8, 0xA7, 0xD9, 0x85, 0xD8, 0x8C, 0x20, 0xD8, 0xA7, 0xD8, 0x8C, 0xD9,
		                   0x86, 0x20, 0xDB, 0x8C, 0xDA, 0xA9, 0x20, 0xD9, 0x85, 0xD9, 0x88, 0xD8, 0xB1, 0xD8, 0xAF, 0x20, 
		                   0xD8, 0xA2, 0xD8, 0xB2, 0xD9, 0x85, 0xD9, 0x88, 0xD9, 0x86, 0x20, 0xD8, 0xA7, 0xD8, 0xB3, 0xD8, 0xAA}; //��˹���:��ã����ǲ�������

	char uni_arial_str[]={0x33, 0x06, 0x44, 0x06, 0x27, 0x06, 0x45, 0x06, 0x0C, 0x06, 0x20, 0x00, 0x86, 0x06,
		                  0xCC, 0x06, 0x46, 0x06};  //��˹���:��ã��й�
		                  
	uint ViewWidth = 0, ViewHeight = 0, rand_width=0, rand_height=0, getwidth=0, getheight=0;

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	cls_show_msg1(5, "�뽫%s,%s��%s���ص�����Գ���ͬһ��·����", FILENAME37, FILENAME2, FILENAME3);
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);

	//case1:δ����ttf���壬�������������ʽ����ʾ�ַ�������Ӧ��ʧ��
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, strlen(asc_str)))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:δ����������,Ӧ�޷���ʾ�ַ���
	if((ret=NDK_ScrInitTrueTypeFont(FILENAME37, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if 0 //������Ϊ����Ҳ������������Ĭ��ֵΪ24*24,������ʾ�ַ��� 20180307 sull modify
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	sleep(1);
	if( cls_show_msg("Ӧ��������ʾ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

	//case2.1:�쳣������inputΪ��DISP_TTF_INPUT_ASCII/UNICODEʱ
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UTF8+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.2:�쳣������X=-1/Y=-1/X>��Ļ�����ʾ��/Y>��Ļ�����ʾ�ߣ�str=NULL,strlen=-1
	if((ret=NDK_ScrDispTrueTypeFontText(-1, 0, asc_str, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, -1, asc_str, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(ViewWidth+1, 0, asc_str, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, ViewHeight+1, asc_str, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, NULL, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8:����ΪUNICODE��ʽʱ�������ַ������ȱ�����2�ı���������Ӧ����NDK_ERR_PARA
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, 1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3������ascii��ʽ��ʱ����ʾunicode�ַ�Ӧ�û��ж����0x00
	NDK_ScrClrs();
	rand_width=rand()%(ViewWidth-24);
	rand_height=rand()%(ViewHeight-24);
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(rand_width, rand_height, uni_str, sizeof(uni_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ����Ӧ��(%d,%d)���꿪ʼΪ%s,Ӧ����������,��ÿ���ַ�֮����пհ׿�,��[ENTER],��[����]", rand_width, rand_height, asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case4:����ascii��ʽ��ʱ����ʾutf8�ַ���������ʾ(UTF-8����ascii��ʽ);����������ʾ,����Ϊ��ɫ
	NDK_ScrClrs();
	rand_width=rand()%(ViewWidth-24);
	rand_height=rand()%(ViewHeight-24);
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_NORMAL,NULL))!=NDK_OK)//��������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetFontColor(RED,FONTCOLOR_NORMAL))!=NDK_OK)//��������Ϊ��ɫ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(rand_width, rand_height, utf8_str, sizeof(utf8_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	NDK_ScrSetFontColor(BLACK,FONTCOLOR_NORMAL);
	if( cls_show_msg("��ʾ����Ӧ��(%d,%d)���꿪ʼΪ%s,Ӧ����������������Ϊ��ɫ,��[ENTER],��[����]", rand_width, rand_height, asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4������unicode��ʽ��ʱ����ʾascii���ַ�Ӧ�û��ǿ��
	NDK_ScrClrs();
	rand_width=rand()%ViewWidth;
	rand_height=rand()%ViewHeight;
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(rand_width, rand_height, asc_str, strlen(asc_str)+1))!=NDK_OK)//asc_strΪ51���ַ�,��unicode��ʽ��,�ַ���������Ϊ2��������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ����Ӧ��(%d,%d)���꿪ʼΪ���ӷ���,��[ENTER],��[����]", rand_width, rand_height) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case5:Ӧ�޷���ʾ��ttf�����ļ��е���
	NDK_ScrClrs();
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str1, sizeof(asc_str1)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ������Ϊhello+����,Ӧ��������ӡ����ʾ�����޷���,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case6:����������ʾ�ַ�����,Ӧ����ȷ��ʾ��Ӧ����;�����ޱ���ɫ(ΪĬ��������ɫ)
	NDK_ScrClrs();
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_NOBACKCOLOR,NULL))!=NDK_OK)//�����ޱ���ɫ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, uni_str1, 10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ������Ϊhello,Ӧ��������ʾ,��������ɫΪ��ɫ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case7:��������,�����������ʽӦ����������ʾ
	//case7.1:�����ʽΪascii��,���÷�����ʾ,����������ɫΪ��ɫ,����ɫΪ��ɫ;
	//case7.2:�����ʽΪunicode��,�����ޱ���ɫ(ΪĬ��������ɫ),�������÷���������ɫû�б仯
	NDK_ScrClrs();
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,NULL))!=NDK_OK)//���÷���
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetFontColor(RED,FONTCOLOR_REVERSE))!=NDK_OK)//��������Ϊ��ɫ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrSetFontColor(GREEN,FONTCOLOR_BG_REVERSE))!=NDK_OK)//����ɫΪ��ɫ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_NOBACKCOLOR,NULL))!=NDK_OK)//�����ޱ���ɫ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetFontColor(GREEN,FONTCOLOR_REVERSE))!=NDK_OK)//��������Ϊ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, getheight*2, uni_str, sizeof(uni_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ������Ϊ%s,Ӧ��������ʾ�ҵ�һ������Ϊ������ʾ(����ɫΪ��ɫ,����Ϊ��ɫ),�ڶ�������Ϊ������ʾ,������ɫΪ��ɫ,��[ENTER],��[����]", asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case7.3:�����ʽΪutf-8,Ӧ����ʾ��Ӧ�������ַ�
	if((ret=NDK_ScrDestroyTrueTypeFont())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrInitTrueTypeFont(FILENAME2, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���scr�ļ����µĲ��Խ��ͼƬ,�������漴����ʾ���������Ա�.");
	NDK_ScrClrs();
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UTF8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, utf8_msg_str, sizeof(utf8_msg_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("Ӧ��������ʾ,����ʾ����Ӧ��scr�ļ����µĲ��Խ��ͼƬһ��,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case8:������˹����ʾ sull��� 20171103
	//case8.1:�����ʽΪunicode�룬Ӧ��������ʾ��˹��
	NDK_ScrClrs();
	if((ret=NDK_ScrInitTrueTypeFont(FILENAME3, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(72,72))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, uni_arial_str, sizeof(uni_arial_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("Ӧ��������ʾ,����ʾ˳��Ӧ���ҵ��������С:72*72������Ϊ��˹���:��ã��й�����[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case8.2:�����ʽΪutf-8�룬Ӧ��������ʾ��˹��	
	NDK_ScrClrs();
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(36,36))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UTF8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0,0, utf8_arial_str, sizeof(utf8_arial_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("Ӧ��������ʾ,����ʾ˳��Ӧ���ҵ��������С36*36������Ϊ��˹���:��ã����ǲ�����������[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScrSetFontColor(WHITE,FONTCOLOR_REVERSE);
	NDK_ScrSetFontColor(BLACK,FONTCOLOR_BG_REVERSE);
	NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII);
	NDK_ScrDestroyTrueTypeFont();
	return;
}
