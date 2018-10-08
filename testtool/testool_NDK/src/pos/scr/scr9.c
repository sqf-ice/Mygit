/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 9.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrLine�Ƿ�����ָ��������仭�߶�
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrLine"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr9
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr9(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int i = 0, j = 0; 
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);

	//case1:�����쳣:unStartX,unStartY,unEndX,unEndY�ֱ�Ϊ-1,>��Ļ
	if((ret=NDK_ScrLine(-1, 10, GUI_GetLCDWidth()-1, GUI_GetLCDHeight()-1, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLine(10, -1, GUI_GetLCDWidth()-1,GUI_GetLCDHeight()-1, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLine(10, 10, -1, GUI_GetLCDHeight()-1, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLine(10, 10, GUI_GetLCDWidth()-1, -1, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLine(10, 10, GUI_GetLCDWidth(), GUI_GetLCDHeight()-1, BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLine(10, 10, GUI_GetLCDWidth()-1, GUI_GetLCDHeight(), BLACK))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	
	//case2:���⻭��
	cls_printf("�����ֱ��.��ȷ�ϲ���ͨ����������ʧ��"); //ME50NX��֧�ֻ�б�� 20171113
	lib_kbflush();		//�������������
	//Ϊʵ���Զ������ԣ�����������
	lib_StartStopwatch();
	while ((nKeyin=lib_kbhit())==0)
	{
		x0 = rand()%ViewWidth;
		y0 = rand()%ViewHeight;
		x1 = rand()%ViewWidth;
		y1 = rand()%ViewHeight;
		NDK_ScrLine(x0, y0, x1, y1, BLACK);
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(10);
		NDK_ScrLine(x0, y0, x1, y1, WHITE);
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(10);
		if (MAXWAITTIME*6<lib_ReadStopwatch())
			break;
	}
	if (nKeyin==ENTER || nKeyin==0)
		;//send_result("%s����ͨ��" ,TESTAPI);
	else
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case3:�����һ�ֱ��,Ȼ����ҵ���ֱ��
	cls_show_msg1(5, "�����һ�ֱ��,Ȼ����ҵ���ֱ��");
	NDK_ScrClrs();
	for (i=0; i<ViewWidth; i+=2)
	{
		for (j=0; j<ViewHeight; j++)
			NDK_ScrLine(i, j, i, j, BLACK);
		NDK_ScrRefresh();//ÿ��ˢ����ʵһ��
	}	
	NDK_SysDelay(3);
	NDK_ScrClrs();
	NDK_ScrRectangle(0, 0, ViewWidth, ViewHeight, RECT_PATTERNS_SOLID_FILL, BLACK);//���ɺ�ɫ��
	NDK_ScrRefresh();
	NDK_SysDelay(3);	
	for (i=ViewWidth-1;i>=0;i-=2)
	{
		for (j=0; j<ViewHeight; j++)
			NDK_ScrLine(i, j, i, j, WHITE);
		NDK_ScrRefresh();//ÿ��ˢ����ʵһ��
	}
	NDK_SysDelay(20);

	//case4:���Խ���
	cls_show_msg1(5, "����������Խ���");
	NDK_ScrClrs();
	NDK_ScrLine(0,0,ViewWidth-1,ViewHeight-1,BLACK);
	NDK_ScrLine(ViewWidth-1,0,0,ViewHeight-1,BLACK);
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	if(cls_show_msg1(MAXWAITTIME, "����Ļ���ֶԽ���,��[ȷ��];����������.")!=ENTER&&auto_flag!=1)  //ME50NX��֧�ֻ�б�� 20171113
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ 	

	//case5:Խ�����
	NDK_ScrLine((ViewWidth-1)/2, 0, (ViewWidth-1)/2, ViewHeight+2, BLACK);	 //��������ȡֵ��Χ
	NDK_SysDelay(20);// 2s
	if(cls_show_msg1(MAXWAITTIME, "����Ļ�������д���,��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ 	

	//test over
	nKeyin = cls_show_msg1(MAXWAITTIME, "ENTER����ͨ��,��������ͨ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	
	return;
}

