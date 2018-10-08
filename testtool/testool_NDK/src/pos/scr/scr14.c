/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 14.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrBackLight�ܷ񿪹�LCD�������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrBackLight"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr14
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr14(void)
{
	/*private & local definition*/
	int ret = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//ǰ��:��ֹ��������
#if !K21_ENABLE
	NDK_SysSetSuspend(0);
#endif

	//case1:�����쳣:emBL�ֱ�Ϊ-1,5
	if((ret=NDK_ScrBackLight(-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrBackLight(5))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_ScrBackLight(BACKLIGHT_OFF))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(5, "�ѹرձ���,���𰴼��ȴ�5���򿪱���");
	
	//case2:�򿪱���	
	if((ret=NDK_ScrBackLight(BACKLIGHT_ON))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(MAXWAITTIME,"�Ѵ򿪱���!���ڱ���ر�ǰ���ٰ���������رձ���");

	//case3:�رձ���	
	sleep(1);
	if((ret=NDK_ScrBackLight(BACKLIGHT_OFF))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:���ⳣ��
#if !K21_ENABLE //K21ƽ̨��Ʒ��֧��BACKLIGHT_LOCKON�����Ա��ⳤ���������������
	cls_show_msg1(5, "�ѹرձ���,5������ñ��ⳣ�������ĵȴ�60��");
	if((ret=NDK_ScrBackLight(BACKLIGHT_LOCKON))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(60);//����60��,ֱ�������Զ��ر�ʱ�䵽��,��һֱ����˵�����ⳣ��������Ч
	if(cls_show_msg1(MAXWAITTIME,"����Ƴ���60��,[ENTER]ͨ��,[����]��ͨ��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case 5:��������ùر�Ӧ������ر�
	cls_show_msg1(5, "���𰴼�,5��󽫹رձ���");	
	if((ret=NDK_ScrBackLight(BACKLIGHT_OFF))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(5, "�ѹرձ���,5��󽫴򿪱�����������ĵȴ�60��");

	//case 6:��������ö���Ӧ�ûᱻ���óɶ���	
	if((ret=NDK_ScrBackLight(BACKLIGHT_LOCKON))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrBackLight(BACKLIGHT_ON))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(60);
	if(cls_show_msg1(MAXWAITTIME, "����ر�,[ENTER]ͨ��,[����]��ͨ��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif
	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
	//�ָ�Ĭ������
ERR://�����������
#if !K21_ENABLE
	NDK_SysSetSuspend(1);
#endif	
	NDK_ScrBackLight(BACKLIGHT_ON);
	return;
}

