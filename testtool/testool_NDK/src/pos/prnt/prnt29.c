/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ӡ
* file name		: 29.c
* Author 			: 
* version			: 
* DATE			: 
* directory 		: 
* description		: ����NDK_PrnSetMixMode�ܷ��������û���ģʽ
* related document	: NDK.chm
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define ABCBANKBMP "abclogo.bmp"
#define FILENAME3  "arial.ttf"
#define	TESTAPI		"NDK_PrnSetMixMode"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: prnt29
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	       sull           20180118  	    created
*
*****************************************************************/
void prnt29(void)
{
	/*private & local definition*/
	int ret = -1;
	char uni_arial_str[]={0x33, 0x06, 0x44, 0x06, 0x27, 0x06, 0x45, 0x06, 0x0C, 0x06, 0x20, 0x00, 0x86, 0x06,
		                 0xCC, 0x06, 0x46, 0x06};  //��˹���:��ã��й�

	
	/*process body*/
	//����ǰ��
	cls_show_msg("�뽫%s��%s���ص�����Գ���ͬһ��·����,��ȷ����ֽ�����������...", ABCBANKBMP,FILENAME3);
	
	//case1:��ȡ����Ĭ��ֵ(��ӡ������Ӧ��Ϊ�ǻ���ģʽ)
	NDK_PrnStr("NewLand�´�½00045");
	NDK_PrnImage(128, 64, 0, CCB_BMP);
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case2:����Ϊ0��Ϊ�ǻ���ģʽ
	if((ret = NDK_PrnSetMixMode(0)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	NDK_PrnStr("����Logo");
	if((ret = NDK_PrnPicture(0,ABCBANKBMP)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case3:����Ϊ1��Ϊ����ģʽ
	if((ret = NDK_PrnSetMixMode(1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStr("----------------NewLand�´�½00045");
	NDK_PrnImage(128, 64, 0, CCB_BMP);
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:����Ϊ-1��Ϊ����ģʽ
	if((ret = NDK_PrnSetMixMode(-1)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	NDK_PrnStr("------����Logo");
	if((ret = NDK_PrnPicture(0,ABCBANKBMP)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case5:��ӡinit����Զ��ָ�Ĭ��ֵ(������ģʽ)
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		GOTOERR;
	}
	NDK_PrnStr("NewLand�´�½00045");
	NDK_PrnImage(128, 64, 0, CCB_BMP);
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6:����Ϊ���ţ�ttf�����ͼƬ��ͬһ��
	if((ret = NDK_PrnSetMixMode(1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
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
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if((ret = NDK_PrnPicture(0,ABCBANKBMP)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case7:����Ϊ�ǻ��ţ�ttf�����ͼƬ����ͬһ��
	if((ret = NDK_PrnSetMixMode(0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
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
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	NDK_PrnImage(128, 64, 0, CCB_BMP);
	NDK_PrnStart();
	if(cls_show_msg("��ӡ�����ֺ�ͼƬ�Ƿ���ͬһ��?����[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
ERR:
	NDK_PrnSetMixMode(0);
	NDK_PrnInit(g_PrnSwitch);	
	NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII);
	NDK_PrnDestroyTrueTypeFont();
	return;
}

