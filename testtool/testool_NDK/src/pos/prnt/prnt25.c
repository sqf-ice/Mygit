/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 25.c
* Author 			: zhengry
* version			: 
* DATE			: 20170502
* directory 		: 
* description		: NDK_PrnInitTrueTypeFont����TTF�����NDK_PrnDestroyTrueTypeFont����ж��
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PrnInitTrueTypeFont��NDK_PrnDestroyTrueTypeFont"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
#define FILENAME35  "angsa.ttf"
#define FILENAMEPATH1 "andlso.ttf"  //��Ϊ�ŵ���ǰ·��  20180502 modify
#define FILENAME1	  "andlso.ttf"
#define FILENAME2  "msgothic.ttc"
#define FILEERRNAME  "errfont.ttf"
#define FILEERRPATH   "/appfs/apps/angsa.ttf"
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: prn25
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhengry         20170502  	   created
*****************************************************************/
void prnt25(void)
{
	/*private & local definition*/
	int ret=0, getwidth=0, getheight=0;
	char asc_str[]="Innovation in China 0123456789!";//angsa��andlso��֧����������,��֧��������������
	char asc_str1[]="Innovation in China �й����죬�ݼ�ȫ�� 0123456789!";//msgothic֧�����ĺ�����
	char uni_str1[]={0x49, 0x00, 0x6E, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x76, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 
					 0x6F, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x43, 0x00, 0x68, 0x00, 
		        	 0x69, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x20, 0x00, 0x2D, 0x4E, 0xFD, 0x56, 0x36, 0x52, 0x20, 0x90,
		        	 0x0C, 0xFF, 0xE0, 0x60, 0xCA, 0x53, 0x68, 0x51, 0x03, 0x74, 0x20, 0x00, 0x30, 0x00, 0x31, 0x00,
		        	 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 
		        	 0x21, 0x00};// Innovation in China �й����죬�ݼ�ȫ�� 0123456789!��ʮ������

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	cls_show_msg("�뽫%s,%s��%s���ص�����Գ���ͬһ��·����,������ֽ�����������...", FILENAME35, FILENAME2, FILENAME1);
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto ERR;
	}
	/*�⼸���º������ܴ�ӡ�е����ú���Ӱ��*/
	
	//case1:δ����ttf���壬����ж�����庯��Ӧ��ʧ��
	if((ret=NDK_PrnDestroyTrueTypeFont())!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�쳣����:filepathname��NULL/filepathname�ļ�������/filepathname�ļ�·������
	if((ret=NDK_PrnInitTrueTypeFont(NULL, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnInitTrueTypeFont(FILEERRNAME, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnInitTrueTypeFont(FILEERRPATH, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:�����ļ�����������index��Ϊ�����ļ��еģ�Ӧ�޷�����TTF���� 
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME35, -1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:����angsa���岢��ʾ����,����andlso���岢��ӡ����,Ӧ�ܴ�ӡ��Ӧ����;����δ���������ʽ,Ĭ��Ϊascii����ʾ
	if(cls_show_msg("�Ƿ񿪻��״�����ttf������ش�ӡ��������,��[ENTER],��[����]") != ENTER)
	{
		if((ret=NDK_PrnInitTrueTypeFont(FILENAME35, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
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
		if( cls_show_msg("��ӡ����Ϊ%sӦ�����������룬������%s�����ļ��м�������СΪ24���ַ���ʽ�Ƿ�����һ��,��[ENTER],��[����]", asc_str, FILENAME35) != ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	
	if((ret=NDK_PrnInitTrueTypeFont(FILENAMEPATH1, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
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
	if( cls_show_msg("��ӡ����Ϊ%sӦ�����������룬������%s�����ļ��м�������СΪ24���ַ���ʽ�Ƿ�����һ��,��[ENTER],��[����]", asc_str, FILENAME1) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case5:ж�������,�ٴΰ�װ������Ӧ���ܳɹ�;�ڲ�ֹһ�������е������ļ��У��ڵ�����ȷindex�£�Ӧ����������TTF����
	if((ret=NDK_PrnDestroyTrueTypeFont())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME2, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, uni_str1, sizeof(uni_str1)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);

	if((ret=NDK_PrnInitTrueTypeFont(FILENAME2, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, uni_str1, sizeof(uni_str1)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ����Ϊ%sӦ�����������룬�Ҵ�ӡ���������岻һ��,��[ENTER],��[����]", asc_str1) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII);

	if((ret=NDK_PrnDestroyTrueTypeFont())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case6: ����ж�����庯���󣬵������������ʽӦ��ʧ��
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7: ����ж�����庯���󣬵������������СӦ��ʧ��
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24,24))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8: ����ж�����庯���󣬵��û�ȡ�����СӦ��ʧ��
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case9: ����ж�����庯���󣬵�����ʾ����Ӧ��ʧ��
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, strlen(asc_str)))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_PrnInit(g_PrnSwitch);
	NDK_PrnDestroyTrueTypeFont();
	return;
}
