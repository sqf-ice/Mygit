/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 26.c
* Author 			: zhengry
* version			: 
* DATE			: 20170502
* directory 		: 
* description		: NDK_PrnSetTrueTypeFontSizeInPixel����TTF����Ŀ�Ⱥ͸߶Ⱥ�NDK_PrnGetTrueTypeFontSize��ȡ������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PrnSetTrueTypeFontSizeInPixel��NDK_PrnGetTrueTypeFontSize"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
#define FILENAME36  "angsa.ttf"
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: prn26
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhengry         20170502  	   created
*****************************************************************/
void prnt26(void)
{
	/*private & local definition*/
	int ret=0, getwidth=0, getheight=0, getwidth1=0, getheight1=0, getwidth2=0, getheight2=0;
	char asc_str[]="Innovation in China 0123456789!";
	char ch1= 'A', ch2= 'a', ch3= 'h';

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	cls_show_msg("�뽫%s���ص�����Գ���ͬһ��·����,������ֽ�����������...", FILENAME36);
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto ERR;
	}

	//case1:δ����ttf���壬�������������ߺ������ȡ��ߺ���Ӧ��ʧ��
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(12, 12))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��δ���ÿ���������ȡ�ַ������Ӧ��Ϊ0
	if((ret=NDK_PrnInitTrueTypeFont(FILENAME36, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
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
	
	if((ret=NDK_PrnSetTrueTypeFontEncodingFormat(PRN_TTF_INPUT_ASCII))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.1:�쳣������width/height=-1
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(-1, 12))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(12, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3.2:�쳣������str��NULL,strlen=-1
	if((ret=NDK_PrnGetTrueTypeFontSize(NULL, strlen(asc_str), &getwidth, &getheight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, -1, &getwidth, &getheight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	
	//case4:���ÿ�߹���,���޷���ӡ(ʱ���ȽϾ�)
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(60000, 60000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if OVERSEAS_OS70_ENABLE  //�����Ʒ7.0�汾����Ĭ���������Ϊ1000(ԭ��5000)�����Դ�ӡ����̫���ռ�᲻��  20180423 sull modify
	if((ret=NDK_PrnGetTrueTypeFontSize("Inno 0!", strlen("Inno 0!"), &getwidth, &getheight))!=NDK_OK)

	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, "Inno 0!", strlen("Inno 0!")))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("Ӧ����������ӡ����Inno 0!,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#else
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
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
	if( cls_show_msg("Ӧ�����ݴ�ӡ,��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

	//case5:���÷�8��������С����,Ӧ��������ӡ
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(31, 45))!=NDK_OK)
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
	if( cls_show_msg("��ӡ%s,�����СӦ����31*45,Ӧ������������,��[ENTER],��[����]", asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6���������ο�ߣ�ͬһ��Ӧ�û�ȡ���Ŀ��Ӧ���ǵڶ������õ�
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24, 24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth, &getheight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(48, 48))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(asc_str, strlen(asc_str), &getwidth1, &getheight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	if( getwidth>=getwidth1 || getheight>=getheight1 )
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d)", __LINE__, TESTAPI, getwidth, getwidth1, getheight, getheight1);
		GOTOERR;
	}
	if((ret=NDK_PrnTrueTypeFontText(0, asc_str, strlen(asc_str)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStart();
	NDK_PrnFeedPaper(PRN_FEEDPAPER_AFTER);
	if( cls_show_msg("��ӡ%s,�����СӦ����48*48,Ӧ������������,��[ENTER],��[����]", asc_str) != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case7:��ȡ��ͬ�ַ����Ŀ��,Ӧ�ò�һ��,��A�Ŀ��ֵ��a�Ĵ�,h��a��,��aһ����
	if((ret=NDK_PrnSetTrueTypeFontSizeInPixel(24, 24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(&ch1, 1, &getwidth, &getheight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(&ch2, 1, &getwidth1, &getheight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PrnGetTrueTypeFontSize(&ch3, 1, &getwidth2, &getheight2))!=NDK_OK)
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
	NDK_PrnInit(g_PrnSwitch);	
	NDK_PrnDestroyTrueTypeFont();
	return;
}

