/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 10.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrRectangle�Ƿ���ָ�������ڻ�����
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_ScrRectangle"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr10
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr10(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0, i = 0;
	uint x0 = 0, y0 = 0, width = 0, height = 0;
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	//case1:�����쳣:unStartX,unStartY,unWidth,unHeight,��䷽ʽ�ֱ�Ϊ-1,>��Ļ
	if((ret=NDK_ScrRectangle(-1, 0, GUI_GetLCDWidth(),GUI_GetLCDHeight(),RECT_PATTERNS_NO_FILL, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrRectangle(0, -1, GUI_GetLCDWidth(),GUI_GetLCDHeight(),RECT_PATTERNS_NO_FILL, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrRectangle(0, 0, -1,GUI_GetLCDHeight(),RECT_PATTERNS_NO_FILL, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrRectangle(0, 0, GUI_GetLCDWidth(),-1,RECT_PATTERNS_NO_FILL, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//������䷽ʽΪ-1��Ӧ�÷���ʧ��//�ײ㽫����ö��������һ������ֵ���в��������سɹ�
	if((ret=NDK_ScrRectangle(0, 0, GUI_GetLCDWidth(),GUI_GetLCDHeight(),-1, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrRectangle(0, 0, GUI_GetLCDWidth()+1,GUI_GetLCDHeight(),RECT_PATTERNS_NO_FILL, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrRectangle(0, 0, GUI_GetLCDWidth(),GUI_GetLCDHeight()+1,RECT_PATTERNS_NO_FILL, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	NDK_ScrClrs();

	//case2:���⻭����
	lib_kbflush();		//�������������
	//Ϊʵ���Զ������ԣ�����������
	lib_StartStopwatch();
	while ((nKeyin=lib_kbhit())==0)
	{
		x0 = rand()%(ViewWidth-1);
		y0 = rand()%(ViewHeight-1);
		width = rand()%(ViewWidth-x0);
		height = rand()%(ViewHeight-y0);
		NDK_ScrRectangle(x0, y0, width, height, RECT_PATTERNS_SOLID_FILL, BLACK);
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(10);
		NDK_ScrRectangle(x0, y0, width, height, RECT_PATTERNS_SOLID_FILL, WHITE);
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(10);
		NDK_ScrRectangle(x0, y0, width, height, RECT_PATTERNS_NO_FILL, BLACK);
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(10);
		NDK_ScrRectangle(x0, y0, width, height, RECT_PATTERNS_NO_FILL, WHITE);
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(10);
		cls_printf("����%s...��ȷ�ϲ���ͨ��,������ʧ��", TESTAPI);
		NDK_SysDelay(30);
		if (MAXWAITTIME<lib_ReadStopwatch())
			break;
	}

	if (nKeyin==ENTER || nKeyin==0)
		;
	else
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case3:��Ƕ�׵ľ���
	cls_show_msg1(5, "���������Ƕ�׵ľ���");
	NDK_ScrClrs();
	for (i=0; i<=(ViewHeight/2); i++)
	{
		NDK_ScrRectangle(i*2, i, ViewWidth-i*2*2, ViewHeight-i*2, RECT_PATTERNS_NO_FILL, BLACK);
		NDK_ScrRefresh();
	}
	NDK_SysDelay(20);

	//case4:�����ͱ߿�(���Խ��)
	cls_show_msg1(5, "������������ͱ߿�(���Խ��),"
			"����ʾ����.");
	NDK_ScrClrs(); 
	NDK_ScrRectangle(ViewWidth, 0, 10, 10, RECT_PATTERNS_NO_FILL, BLACK);   //��������ȡֵ��Χ
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	NDK_ScrClrs(); 
	NDK_ScrRectangle(0, ViewHeight, 10, 10, RECT_PATTERNS_SOLID_FILL, BLACK);   //��������ȡֵ��Χ,�������ģʽΪSOLID_FILL�������串����
	NDK_ScrRefresh();
	NDK_SysDelay(20);	
	nKeyin = cls_show_msg1(5, "ENTER����ͨ��,��������ͨ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}

