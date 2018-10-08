/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 39.c
* Author 			:
* version			: 
* DATE			:
* directory 		: 
* description		: ����NDK_ScrSetBrightness�ܷ���������Һ������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrSetBrightness"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr39
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	      sull             20180425         created
*****************************************************************/
void scr39(void)
{
	/*private & local definition*/
	int ret = 0, i = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//case1:�����쳣:val�ֱ�Ϊ0,21
	if((ret = NDK_ScrSetBrightness(0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_ScrSetBrightness(21)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�رձ��������£����ñ������Ӧ���Զ�������������Ч
	cls_show_msg1(5,"5��󼴽��رձ��⣬�벻Ҫ�������رպ��Զ��򿪱���,����Ļ�𽥱���");
	if((ret=NDK_ScrBackLight(BACKLIGHT_OFF))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);
	for(i=1;i<=20;i++)
	{
		if((ret = NDK_ScrSetBrightness(i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		NDK_SysMsDelay(500);
	}
	if(cls_show_msg1(MAXWAITTIME,"�Ƿ��Զ���������,����Ļ�𽥱���,[ENTER]ͨ��,[����]��ͨ��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3:������������Ϊ�Ӵ�С��Ӧ��������ʾ
	cls_show_msg1(5,"5�����۲���Ļ�Ƿ��𽥱䰵");
	if((ret=NDK_ScrBackLight(BACKLIGHT_ON))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=20; i>=1; i--)
	{
		if((ret = NDK_ScrSetBrightness(i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		NDK_SysMsDelay(500);
	}
	if(cls_show_msg1(MAXWAITTIME,"��Ļ�����Ƿ��𽥱䰵,[ENTER]ͨ��,[����]��ͨ��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
	
ERR:
	NDK_ScrBackLight(BACKLIGHT_ON);
	NDK_ScrSetBrightness(12); //�ָ�Ĭ������
	return;
}


