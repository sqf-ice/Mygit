/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 36.c
* Author 			: zhengry
* version			: 
* DATE			: 20170426
* directory 		: 
* description		: NDK_SrcSetTrueTypeFontSizeInPixel����TTF����Ŀ�Ⱥ͸߶Ⱥ�NDK_SrcGetTrueTypeFontSize��ȡ������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScrSetTrueTypeFontSizeInPixel��NDK_ScrGetTrueTypeFontSize"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
#define FILENAME36  "angsa.ttf"
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr36
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhengry         20170426  	   created
*****************************************************************/
void scr36(void)
{
	/*private & local definition*/
	int ret=0, getwidth=0, getheight=0, getwidth1=0, getheight1=0, getwidth2=0, getheight2=0;
	char asc_str[]="Innovation in China 0123456789!";
	char ch1= 'A', ch2= 'a', ch3= 'h';
	uint ViewWidth = 0, ViewHeight = 0;

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	cls_show_msg1(5, "�뽫%s���ص�����Գ���ͬһ��·����", FILENAME36);
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);

	//case1:δ����ttf���壬�������������ߺ������ȡ��ߺ���Ӧ��ʧ��
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(12, 12))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=NDK_ScrInitTrueTypeFont(FILENAME36, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrSetTrueTypeFontEncodingFormat(DISP_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2:��δ���ÿ���������ȡ�ַ������Ӧ��Ϊ0
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	#if 0 //������Ϊ����Ҳ������������Ĭ��ֵΪ24*24,��ȡ���ַ���߲�Ϊ0,��ͬ�����ȡ�Ŀ�߲�һ�£������ж� 20180307 sull modify
	if( getwidth!=0 || getheight!=0 ) 
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, getwidth, getheight);
		GOTOERR;
	}
	#endif
	
	//case3.1:�쳣������width/height����С�ڵ���-1ֵ,��ת����1
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(-1, 12))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(12, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3.2:�쳣������str��NULL,strlen=-1,��-1��ʱ���ת����1
	if((ret=NDK_ScrGetTrueTypeFontSize(NULL, strlen(asc_str), &getwidth, &getheight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, -1, &getwidth, &getheight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	
	//case4:���ÿ��Ϊ�����Ļ��������Ļ��,���޷���ʾ,��Ļ���Ϊ320,240,ʵ�ʻ�ȡ���Ŀ��Ϊ7147,279
	NDK_ScrClrs();
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(ViewWidth, ViewHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
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
	sleep(2);
	if( cls_show_msg("Ӧ��������ʾ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case5:���÷�8��������С����,Ӧ��������ʾ
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(31, 45))!=NDK_OK)
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
	if( cls_show_msg("��ʾ����Ϊ%s,�����СӦ����31*45,Ӧ������������,��[ENTER],��[����]", asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6���������ο�ߣ�ͬһ��Ӧ�û�ȡ���Ŀ��Ӧ���ǵڶ������õ�
	NDK_ScrClrs();
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(12, 12))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24, 24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth1, &getheight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	if( getwidth>=getwidth1 || getheight>=getheight1 )
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d)", __LINE__, TESTAPI, getwidth, getwidth1, getheight, getheight1);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, 0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispTrueTypeFontText(0, getheight1, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_KbGetCode(0,0);
	if( cls_show_msg("��ʾ����Ϊ%s,�����СӦ����24*24,����ʾ���ַ�֮�䲻Ӧ�û����ص�,��[ENTER],��[����]", asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case7:��ȡ��ͬ�ַ����Ŀ��,Ӧ�ò�һ��,��A�Ŀ��ֵ��a�Ĵ�,j��a��,��aһ����
	if((ret=NDK_ScrSetTrueTypeFontSizeInPixel(24, 24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(&ch1, 1, &getwidth, &getheight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(&ch2, 1, &getwidth1, &getheight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetTrueTypeFontSize(&ch3, 1, &getwidth2, &getheight2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if( getwidth<=getwidth1 || getheight!=getheight1 || getwidth1>=getwidth2 || getheight1!=getheight2)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d,%d,%d)", __LINE__, TESTAPI, getwidth, getheight, getwidth1, getheight1, getwidth2, getheight2);
		GOTOERR;
	}//��ȡ��A��a��h�ĸ߶ȶ�һ��;��ȴӴ�С����A>h>a

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScrDestroyTrueTypeFont();
	return;
}
