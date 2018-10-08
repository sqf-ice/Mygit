/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 35.c
* Author 			: zhengry
* version			: 
* DATE			: 20170425
* directory 		: 
* description		: NDK_ScrInitTrueTypeFont����TTF�����NDK_ScrDestroyTrueTypeFont����ж��
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScrInitTrueTypeFont��NDK_ScrDestroyTrueTypeFont"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
#define FILENAME35  "angsa.ttf"
#define FILENAMEPATH1 "andlso.ttf"   //��Ϊ���ڵ�ǰ·����  20180502 modify
#define FILENAME1	  "andlso.ttf"
#define FILENAME2  "msgothic.ttc"
#define FILEERRNAME  "errfont.ttf"
#define FILEERRPATH   "/appfs/apps/angsa.ttf"
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr35
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhengry         20170425  	   created
*****************************************************************/
void scr35(void)
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
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	cls_show_msg1(15, "�뽫%s,%s��%s���ص�����Գ���ͬһ��·����", FILENAME35, FILENAME2, FILENAME1);

	//case1:δ����ttf���壬����ж�����庯��Ӧ��ʧ��
	if((ret=NDK_ScrDestroyTrueTypeFont())!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�쳣����:filepathname��NULL/filepathname�ļ�������/filepathname�ļ�·������
	if((ret=NDK_ScrInitTrueTypeFont(NULL, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrInitTrueTypeFont(FILEERRNAME, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrInitTrueTypeFont(FILEERRPATH, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:�����ļ�����������index��Ϊ�����ļ��еģ�Ӧ�޷�����TTF���� 
	if((ret=NDK_ScrInitTrueTypeFont(FILENAME35, -1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:����angsa���岢��ʾ����,����andlso���岢��ʾ����,Ӧ����ʾ��Ӧ����;����δ���������ʽ,Ĭ��Ϊascii����ʾ
	if(cls_show_msg("�Ƿ񿪻��״�����ttf���������ʾ��������,��[ENTER],��[����]") != ENTER)
	{
		NDK_ScrClrs();	
		if((ret=NDK_ScrInitTrueTypeFont(FILENAME35, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, strlen(asc_str)))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_ScrRefresh();
		NDK_KbGetCode(0,0);
		if( cls_show_msg("��ʾ����Ϊ%sӦ�����������룬������%s�����ļ��м�������СΪ24���ַ���ʽ�Ƿ�����һ��,��[ENTER],��[����]", asc_str, FILENAME35) != ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	
	NDK_ScrClrs();
	if((ret=NDK_ScrInitTrueTypeFont(FILENAMEPATH1, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0,0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ����Ϊ%sӦ�����������룬������%s�����ļ��м�������СΪ24���ַ���ʽ�Ƿ�����һ��,��[ENTER],��[����]", asc_str, FILENAME1) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_ScrDestroyTrueTypeFont())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:ж�������,�ٴΰ�װ������Ӧ���ܳɹ�;�ڲ�ֹһ�������е������ļ��У��ڵ�����ȷindex�£�Ӧ����������TTF����
	NDK_ScrClrs();
	if((ret=NDK_ScrInitTrueTypeFont(FILENAME2, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_UNICODE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, uni_str1, sizeof(uni_str1)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	
	NDK_ScrClrs();
	if((ret=NDK_ScrInitTrueTypeFont(FILENAME2, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 20, uni_str1, sizeof(uni_str1)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ����Ϊ%sӦ�����������룬��������ʾ�����岻һ��,��[ENTER],��[����]", asc_str1) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII);
	
	if((ret=NDK_ScrDestroyTrueTypeFont())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case6: ����ж�����庯���󣬵������������ʽӦ��ʧ��
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7: ����ж�����庯���󣬵������������СӦ��ʧ��
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24,24))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8: ����ж�����庯���󣬵��û�ȡ�����СӦ��ʧ��
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case9: ����ж�����庯���󣬵�����ʾ����Ӧ��ʧ��
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, strlen(asc_str)))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScrDestroyTrueTypeFont();
	return;
}
