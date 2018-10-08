/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ӡ
* file name		: 28.c
* Author 			: 
* version			: 
* DATE			: 
* directory 		: 
* description		: ����NDK_PrnSetTrueTypeFontAttr�Ƿ������ȷȡԭ������ʾ���ԣ��������µ���ʾ����
* related document	: NDK.chm
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	MAXWAITTIME 10//30
#define FILENAME2  "msgothic.ttc"
#define FILENAME3 "arial.ttf"
#define ABCBANKBMP "abclogo.bmp"
#define	TESTAPI		"NDK_PrnSetTrueTypeFontAttr"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: prnt28
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	       sull           20180112  	    created
*
*****************************************************************/
void prnt28(void)
{
	/*private & local definition*/
	int ret = -1;
	EM_PRN_TTF_ATTRIBUTE OldAttr = PRN_TTF_ATTRIBUTE_NORMAL;
	char utf8_msg_str[]={0xE3, 0x81, 0x93, 0xE3, 0x82, 0x93, 0xE3, 0x81, 0xAB, 0xE3, 0x81, 0xA1, 0xE3, 0x81, 0xAF, 0xE3,
						 0x80, 0x81, 0xE3, 0x81, 0x93, 0xE3, 0x82, 0x8C, 0xE3, 0x81, 0xAF, 0xE3, 0x83, 0x86, 0xE3, 0x82, 
						 0xB9, 0xE3, 0x83, 0x88, 0xE3, 0x81, 0xAE, 0xE4, 0xBE, 0x8B, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99 };//���İ��:���,���ǲ�������
	char uni_arial_str[]={0x33, 0x06, 0x44, 0x06, 0x27, 0x06, 0x45, 0x06, 0x0C, 0x06, 0x20, 0x00, 0x86, 0x06,
		                 0xCC, 0x06, 0x46, 0x06, 0x0D, 0x00};  //��˹���:��ã��й�
	
	/*process body*/
	//����ǰ��
	cls_show_msg("�뽫%s,%s��%s���ص�����Գ���ͬһ��·����,������ֽ�����������...",ABCBANKBMP, FILENAME2,FILENAME3);
	//case1:�����Ƿ�
	if((ret = NDK_PrnSetTrueTypeFontAttr(0, &OldAttr)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}


	//case2:����Ϊ����ģʽ������������ȡ������Ĭ��ֵ����ģʽ(��ȡ��������֮ǰ��ģʽ)
	if((ret = NDK_PrnSetTrueTypeFontAttr(PRN_TTF_ATTRIBUTE_REVERSE, &OldAttr)) != NDK_OK || OldAttr != PRN_TTF_ATTRIBUTE_NORMAL)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, OldAttr);
		GOTOERR;
	}

	//case3:����NDK_PrnInit��ᱻ�Զ�����Ϊ����ģʽ
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
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
	if( cls_show_msg("Ӧ��������ӡ,�Ҵ�ӡ����Ӧ��prn�ļ����µĲ��Խ��ͼƬ2һ��,��ӡ����Ϊ����ģʽ����[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3:����Ϊ����ģʽ������������ȡ����ģʽ����ӡ������Ч��Ϊ����ģʽ
	if((ret = NDK_PrnSetTrueTypeFontAttr(PRN_TTF_ATTRIBUTE_REVERSE, &OldAttr)) != NDK_OK || OldAttr != PRN_TTF_ATTRIBUTE_NORMAL)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, OldAttr);
		GOTOERR;
	}
	if((ret = NDK_PrnDestroyTrueTypeFont()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME3, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}
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
	if( cls_show_msg("Ӧ��������ӡ,������Ϊ��˹���:��ã��й�,��ӡ����Ϊ����ģʽ?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3.1:����ģʽ�Դ�ӡ�ַ�����ͼƬ��Ӧ��Ч,ֻ��ttf������Ч
	if((ret = NDK_PrnSetTrueTypeFontAttr(PRN_TTF_ATTRIBUTE_REVERSE, &OldAttr)) != NDK_OK || OldAttr != PRN_TTF_ATTRIBUTE_REVERSE)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, OldAttr);
		GOTOERR;
	}
	NDK_PrnStr("����Logo");
	if((ret = NDK_PrnPicture(0,ABCBANKBMP)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 	
	NDK_PrnStart();
	if(cls_show_msg("Ӧ��������ӡ,�Ҵ�ӡ����Ϊ����ģʽ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:�������ԭģʽ����ΪNULL����ʱû�з���ֵ
	if((ret = NDK_PrnSetTrueTypeFontAttr(PRN_TTF_ATTRIBUTE_NORMAL, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PrnDestroyTrueTypeFont()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME3, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
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
	if(cls_show_msg("Ӧ��������ӡ,������Ϊ��˹���:��ã��й�,��ӡ����Ϊ����ģʽ?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);

ERR:
	NDK_PrnSetTrueTypeFontAttr(PRN_TTF_ATTRIBUTE_NORMAL, &OldAttr);
	NDK_PrnInit(g_PrnSwitch);	
	NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII);
	NDK_PrnDestroyTrueTypeFont();
	return;
}
