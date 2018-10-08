/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		:6.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrClrLine�Ƿ�������������,�ѹ���Ƶ�(0,unStartY)��
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10
#define 	TESTAPI		"NDK_ScrClrLine"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr6
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr6(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin = 0;
	uint getx = 0, gety = 0, ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(&getx, &gety, &ViewWidth, &ViewHeight);
	NDK_SysDelay(10);
	
	//����ǰ��:��Ļ����
	if((ret=NDK_ScrRectangle(0,0,ViewWidth,ViewHeight,RECT_PATTERNS_SOLID_FILL,BLACK))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	NDK_ScrRefresh();
	
	//case1:������������Ƿ�ʱ��Ч,���ش�����Ϣ:unEndY<unStartY,unStartYΪ-1,unStartY>��Ļ����
	if((ret=NDK_ScrClrLine(10,5))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrClrLine(-1,5))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	
	if((ret=NDK_ScrClrLine(ViewHeight,5))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//unEndYΪ-1,unEndY>��Ļ����,��Ϊ=��Ļ���ش���,������Ҫ�޸����NDK�ĵ�
	if((ret=NDK_ScrClrLine(ViewHeight/3*2,-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	nKeyin=show_msg1(MAXWAITTIME,"����%d�е�,��ȷ��[ENTER],������ʧ��\n",ViewHeight/3*2);
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}	
	//���1��unEndY=unStartY
	if((ret=NDK_ScrClrLine(10,10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	if((ret=NDK_ScrClrLine(ViewHeight/3,ViewHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	nKeyin=show_msg1(MAXWAITTIME,"����%d�е�,��11�б�����,��ȷ��[ENTER],������ʧ��\n",ViewHeight/3);
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case2:�������//�Ϸ���������unEndY>unStartY
	if((ret=NDK_ScrClrLine(2,ViewHeight-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	nKeyin=show_msg1(MAXWAITTIME,"ֻ�������е�.��ȷ��[ENTER].������ʧ��\n");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	NDK_ScrRefresh();
	
	//case3:���2��unEndY=unStartY+1
	if((ret=NDK_ScrClrLine(0,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrClrLine(2,ViewHeight-1))!=NDK_OK)//������ΪΪ������Ļ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	nKeyin=show_msg1(MAXWAITTIME, "��һ���е��б�ɾ��.��ȷ��[ENTER].������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
#if STABAR_ENABLE
	//case4:����״̬���ر�״̬�£�NDK_ScrClrLine�����Ƿ����� 20130703chensj
	cls_show_msg1(MAXWAITTIME, "����������ڵײ���ʾ״̬��,3���ر�,��3������");
	if((ret=NDK_ScrRectangle(0,ViewHeight/2,ViewWidth,ViewHeight/2,RECT_PATTERNS_SOLID_FILL,BLACK))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI,ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_ScrStatusbar(STATUSBAR_DISP_ALL|STATUSBAR_POSITION_BOTTOM);
	NDK_ScrRefresh();
	NDK_KbGetCode(3,NULL);
	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
	NDK_ScrRefresh();
	NDK_KbGetCode(3,NULL);
	if((ret=NDK_ScrClrLine(0,ViewHeight-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=show_msg1(MAXWAITTIME, "��Ļ������.��ȷ��[ENTER].������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
#if STABAR_ENABLE
ERR:
	if(gety!=0)
		NDK_ScrStatusbar(STATUSBAR_DISP_ALL);
	else
		NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}

