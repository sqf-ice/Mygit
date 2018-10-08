/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 27.c
* Author 			: zhengry
* version			: 
* DATE			: 20170503
* directory 		: 
* description		: NDK_PrnSetTrueTypeFontEncodingFormat���������ʽ��NDK_PrnTrueTypeFontTextʹ���Ѽ��ص�TTF������ʾ����
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PrnSetTrueTypeFontEncodingFormat��NDK_PrnTrueTypeFontText"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
#define FILENAME37  "angsa.ttf"
#define FILENAME2  "msgothic.ttc"
#define FILENAME3 "arial.ttf"
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: prnt27
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhengry         20170503  	   created
*****************************************************************/
void prnt27(void)
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
	char uni_angsa_str[]={0x06, 0x0e, 0x10, 0x0e, 0x29, 0x0e, 0x37, 0x0e, 0x55, 0x0e, 0xbb, 0x00, 0xc8, 0x00, 0xe5, 0x00, 0xf1, 0x00, 0x20, 0x20};	uint rand_x=0;
	char utf8_msg_str[]={0xE3, 0x81, 0x93, 0xE3, 0x82, 0x93, 0xE3, 0x81, 0xAB, 0xE3, 0x81, 0xA1, 0xE3, 0x81, 0xAF, 0xE3,
						 0x80, 0x81, 0xE3, 0x81, 0x93, 0xE3, 0x82, 0x8C, 0xE3, 0x81, 0xAF, 0xE3, 0x83, 0x86, 0xE3, 0x82, 
						 0xB9, 0xE3, 0x83, 0x88, 0xE3, 0x81, 0xAE, 0xE4, 0xBE, 0x8B, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99 };//���İ��:���,���ǲ�������

	char utf8_arial_str[]={0xD8, 0xB3, 0xD9, 0x84, 0xD8, 0xA7, 0xD9, 0x85, 0xD8, 0x8C, 0x20, 0xDA, 0x86,
		                  0xDB, 0x8C, 0xD9, 0x86}; //��˹���:��ã��й�

	char uni_arial_str[]={0x33, 0x06, 0x44, 0x06, 0x27, 0x06, 0x45, 0x06, 0x0C, 0x06, 0x20, 0x00, 0x86, 0x06,
		                   0xCC, 0x06, 0x46, 0x06};  //��˹���:��ã��й�
						 
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	cls_show_msg("�뽫%s,%s��%s���ص�����Գ���ͬһ��·����,������ֽ�����������...", FILENAME37, FILENAME2,FILENAME3);
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto ERR;
	}

	//case1:δ����ttf���壬�������������ʽ����ʾ�ַ�������Ӧ��ʧ��
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, strlen(asc_str)))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:δ����������,Ӧ�޷���ӡ�ַ���
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME37, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
#if 0 //������Ϊ����Ҳ������������Ĭ��ֵΪ24*24,������ʾ�ַ��� 20180307 sull modify
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("Ӧ�����ݴ�ӡ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif
	
	//case2.1:�쳣������inputΪ��DISP_TTF_INPUT_ASCII/UNICODEʱ
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(48,48))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UTF8+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.2:�쳣������X=-1,XΪ�ش�ֵ?,str=NULL,strlen=-1
	if((ret=NDK_PrnTrueTypeFontText(-1, asc_str, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(384, asc_str, strlen(asc_str)))!=NDK_ERR_PARA) //������ӡ�����Ϊ384��(0~383��)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, NULL, strlen(asc_str)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:����ΪUNICODE��ʽʱ�������ַ������ȱ�����2�ı���������Ӧ����NDK_ERR_PARA
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, 1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3������ascii��ʽ��ʱ����ʾunicode�ַ�Ӧ�û��ж����0x00
	rand_x=rand()%384; 
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(rand_x, uni_str, sizeof(uni_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ���ݴ�x=%d���꿪ʼΪ%s,Ӧ����������,��ÿ���ַ�֮����пհ׿�,��[ENTER],��[����]", rand_x, asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case4:����ascii��ʽ��ʱ�򣬴�ӡutf8�ַ���������ӡ(UTF-8����ascii��ʽ)
	rand_x=rand()%384; 
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(rand_x, utf8_str, sizeof(utf8_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ���ݴ�x=%d���꿪ʼΪ%s,Ӧ����������,��[ENTER],��[����]", rand_x, asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case5������unicode��ʽ��ʱ����ʾascii���ַ�Ӧ�û�����
	rand_x=rand()%384; 
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(rand_x, asc_str, strlen(asc_str)+1))!=NDK_OK)//asc_strΪ51���ַ�,��unicode��ʽ��,�ַ���������Ϊ2��������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ����Ӧ��x=%d���꿪ʼΪ�׿��,��[ENTER],��[����]", rand_x) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6:Ӧ�޷���ʾ��ttf�����ļ��е���
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str1, sizeof(asc_str1)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ����Ϊhello+����,Ӧ��������ӡ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case7:����������ʾ�ַ�����,Ӧ����ȷ��ʾ��Ӧ����
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, uni_str1, 10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ����Ϊhello,Ӧ��������ӡ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case8:��������,�����������ʽӦ����������ӡ
	//case8.1:�����ʽΪascii��,Ӧ�ܴ�ӡ��Ӧ�ַ�
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ����Ϊ%s,Ӧ��������ӡ,��[ENTER],��[����]", asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case8.2:�����ʽΪunicode��,Ӧ�ܴ�ӡ��Ӧ�ַ�
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, uni_angsa_str, sizeof(uni_angsa_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("Ӧ��������ӡ,�Ҵ�ӡ����Ӧ��prn�ļ����µĲ��Խ��ͼƬ1һ��,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case8.3:�����ʽΪutf-8��,Ӧ�ܴ�ӡ��Ӧ�ַ�
	if((ret=NDK_PrnDestroyTrueTypeFont())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME2, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(48,48))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UTF8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, utf8_msg_str, sizeof(utf8_msg_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("Ӧ��������ӡ,�Ҵ�ӡ����Ӧ��prn�ļ����µĲ��Խ��ͼƬ2һ��,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case9:������˹�ascii��ֻ֧��Ӣ�ĺ����֣��ʲ���	add by sull 20171101
	//case9.1:�����ʽΪutf-8�룬Ӧ��������ӡ��˹��	
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME3, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(36,36))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UTF8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, utf8_arial_str, sizeof(utf8_arial_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("Ӧ��������ӡ,�Ҵ�ӡ˳��Ӧ���ҵ��������С:36*36������Ϊ��˹���:��ã��й�����[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case9.2:�����ʽΪunicode�룬Ӧ��������ӡ��˹��
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(72,72))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, uni_arial_str, sizeof(uni_arial_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("Ӧ��������ӡ,�Ҵ�ӡ˳��Ӧ���ҵ��������С:72*72������Ϊ��˹���:��ã��й�����[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_PrnInit(g_PrnSwitch);	
	NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII);
	NDK_PrnDestroyTrueTypeFont();
	return;
}
