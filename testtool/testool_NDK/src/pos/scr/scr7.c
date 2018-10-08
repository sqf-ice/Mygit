/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 7.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		:  ����NDK_ScrSetViewPort�ܷ�������Ļ����ʾ����, ����NDK_ScrGetViewPort�ܷ��ȡ��Ļ����ʾ����
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrSetViewPort,NDK_ScrGetViewPort"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr7
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr7(void)
{
	/*private & local definition*/
	int  ret = 0;	
	uint punX = 0, punY = 0, punWidth = 0, punHeight = 0;
	uint punX1 = 0, punY1 = 0, punWidth1 = 0, punHeight1 = 0;

	/*process body*/
	cls_show_msg1(2, "����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	NDK_ScrClrs();
	NDK_ScrRefresh();
	//����ǰ��:��ȡ��ǰ����Ļ����ʾ����//������Ӧ�õ��ڲ��Գ����ʼ��ʱ���ÿ���ʾ�������С����δ�����������ʾ����Ϊʵ����Ļ�ߴ�
	if((ret=NDK_ScrGetViewPort(&punX,&punY,&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//case1:NDK_ScrSetViewPort�����쳣��unX,unY,unWidth,unHeight�ֱ�Ϊ-1,>��Ļ,unX+unWidth>��Ļ,unY+unHeight>��Ļ
	if((ret=NDK_ScrSetViewPort(punX-1,0,100,100))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetViewPort(0,punY-1,100,100))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetViewPort(0,0,-1,100))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetViewPort(0,0,100,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���۱߽�ֵΪGUI_GetLCDWidth()+1;gp730��ʵ�ʱ߽�ֵΪGUI_GetLCDWidth()+5
	if((ret=NDK_ScrSetViewPort(0,0,GUI_GetLCDWidth()+5,100))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetViewPort(0,0,100,GUI_GetLCDHeight()+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���۱߽�ֵΪGUI_GetLCDWidth()+1;gp730��ʵ�ʱ߽�ֵΪGUI_GetLCDWidth()+5	
	if((ret=NDK_ScrSetViewPort(10+punX,10+punY,GUI_GetLCDWidth()-5,100))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetViewPort(10+punX,10+punY,100,GUI_GetLCDHeight()-9))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case2:����һ����������ʾ����Ӧ�óɹ�
	//������һ��,������һ������,���Ƿ����һ����Ч 20140603 added by jiangym
	if((ret=NDK_ScrSetViewPort( 15+punX,15+punY,punWidth-30,punHeight-30))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)(x=%d,y=%d,punWidth=%d,punHeight=%d)", __LINE__, TESTAPI, ret, 15+punX,15+punY,punWidth-30,punHeight-30);
		GOTOERR;
	}
	if((ret=NDK_ScrSetViewPort(10+punX,10+punY,punWidth-20,punHeight-20))!=NDK_OK)//�˴�����
	{
		send_result("line %d:%s����ʧ��(%d)(x=%d,y=%d,punWidth=%d,punHeight=%d)", __LINE__, TESTAPI, ret, 10+punX,10+punY,punWidth-20,punHeight-20);
		GOTOERR;
	}

	//case3:��֤��ʾ����ֻ�ڸ���������Ч�����Ե�������һ����ʾ����	
	if(ENTER != cls_show_msg1(MAXWAITTIME,"����Ϣ�Ǵ���Ļ��������(10,10)��ʼ��ʾ��,ENTER����ͨ��;������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case4:��������Ҳֻ����������꣬�����������ص���ʾ�������Ͻ�	
	NDK_ScrClrs();
	if(ENTER != cls_show_msg1(MAXWAITTIME,"|��꽫�ƶ���(10,10),ENTER����ͨ��;������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case5:NDK_ScrGetViewPortΪNULL,��ʾ���û�ȡ����ֵ�����سɹ�
	if((ret=NDK_ScrGetViewPort(NULL,&punY1,&punWidth1,&punHeight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetViewPort(&punX1,NULL,&punWidth1,&punHeight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetViewPort(&punX1,&punY1,NULL,&punHeight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetViewPort(&punX1,&punY1,&punWidth1,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:��ȡ��ʾ�����С��Ӧ�ú�case2��2�����õ�һ��
	if((ret=NDK_ScrGetViewPort(&punX1,&punY1,&punWidth1,&punHeight1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(punX1!=10+punX||punY1!=10+punY||punWidth1!=(punWidth-20)||punHeight1!=(punHeight-20))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d)", __LINE__, TESTAPI, punX1, punY1, punWidth1, punHeight1);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	
ERR:
	//�ָ�Ĭ������
	NDK_ScrSetViewPort(punX,punY,punWidth,punHeight);
	return;
}

