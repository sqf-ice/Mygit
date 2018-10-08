/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 11.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrPutPixel�Ƿ�����LCD��Ļ������һ���㣬����NDK_ScrGetPixel�ܷ�ȡ��ǰLCD��Ļ�ϵ�һ��(x,y)����ɫ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_ScrPutPixel,NDK_ScrGetPixel"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr11
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr11(void)
{
	/*private & local definition*/
	uint x = 0, y = 0, i = 0, j = 0;
	int nKeyin = ENTER, ret = 0;
	color_t punColor = BLACK;
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	//case1:�����쳣:x,y�ֱ�Ϊ-1,>��Ļ,NDK_ScrPutPixelʧ��;punColorΪNULL,NDK_ScrGetPixelʧ��
	if((ret=NDK_ScrPutPixel(ViewWidth, 0, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrPutPixel(0, ViewHeight, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrPutPixel(-1, 0, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrPutPixel(0, -1, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetPixel(ViewWidth, 0, &punColor))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetPixel(0, ViewHeight, &punColor))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetPixel(-1, 0, &punColor))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetPixel(0, -1, &punColor))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetPixel(0, 0, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:���⻭һ��	
	lib_kbflush();//�������������
	//Ϊʵ���Զ������ԣ�����������
	lib_StartStopwatch();
	while ((nKeyin=lib_kbhit())==0)
	{
		x = rand()%ViewWidth;
		y = rand()%ViewHeight;
		
		cls_show_msg1(3, "����(%d, %d)��һ�ڵ�,����ٻ�һ�׵�.��������,�ɰ�ENTER;���쳣,�밴������", x, y);
		NDK_ScrClrs();
		NDK_ScrPutPixel(x, y, BLACK);
		NDK_ScrRefresh();
#if !(defined ME50NX ||defined ME68||CPU5810X_ENABLE)//ME50NX\CPU5810X_ENABLEƽ̨��Ʒ��֧��NDK_ScrGetPixel 20171113
		NDK_ScrGetPixel(x, y, &punColor);
		if (BLACK!=punColor)
		{
			send_result("line %d:getpixelӦΪBLACK", __LINE__);
			RETURN;
		}
#endif
		NDK_SysBeep();
		NDK_SysDelay(10);
		NDK_ScrPutPixel(x, y, WHITE);
		NDK_ScrRefresh();
#if !(defined ME50NX ||defined ME68||CPU5810X_ENABLE) //ME50NX\CPU5810X_ENABLEƽ̨��Ʒ��֧��NDK_ScrGetPixel 20171113
		NDK_ScrGetPixel(x, y, &punColor);
		if (WHITE!=punColor)
		{
			send_result("line %d:getpixelӦΪWHITE(ʵ��:0x%4x)", __LINE__, punColor);
			RETURN;
		}
#endif
		NDK_SysBeep();
		NDK_SysDelay(10);
		if (MAXWAITTIME<lib_ReadStopwatch())
			break;
	}
	if (nKeyin==ENTER || nKeyin==0)//nKeyin==0˵����ʱ����ô��Ĭ�ϲ���ͨ��
		;
	else
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		
	//case3:ȫ��Ļ���ڵ�
	cls_show_msg1(5, "�밴�����ȫ��Ļ���ڵ�,��ȫ��Ļ���׵�");
	for (i=0; i<ViewWidth; i++)
	{
		for (j=0; j<ViewHeight; j++)
			NDK_ScrPutPixel(i, j, BLACK);
		NDK_ScrRefresh();//ÿ��ˢ����ʵһ��
	}
	NDK_SysDelay(20);
	
	//case4:ȫ��Ļ���׵�
	for (i=0; i<ViewWidth; i++)
	{
		for (j=0; j<ViewHeight; j++)
			NDK_ScrPutPixel(i, j, WHITE);
		NDK_ScrRefresh();//ÿ��ˢ����ʵһ��
	}
	NDK_SysDelay(20);
	
	//case5:���и��л��ڵ�
	cls_show_msg1(5, "�밴��������и��л��ڵ�");
	NDK_ScrClrs();
	for (i=0; i<ViewWidth; i+=2)
	{   
		for (j=0; j<ViewHeight; j+=2)
			NDK_ScrPutPixel(i, j, BLACK);			
		NDK_ScrRefresh();//ÿ��ˢ����ʵһ��
	}	  	
	NDK_SysDelay(20);
	
	//case6:���и��л��׵�
	cls_show_msg1(5, "�밴��������и��л��׵�");
	NDK_ScrRectangle(0,0,ViewWidth,ViewHeight,RECT_PATTERNS_SOLID_FILL,BLACK);//���ɺ�ɫ��
	for (i=0; i<ViewWidth; i+=2)
	{   
		for (j=0; j<ViewHeight; j+=2)
			NDK_ScrPutPixel(i, j, WHITE);	
		NDK_ScrRefresh();//ÿ��ˢ����ʵһ��
	}
	NDK_SysDelay(20);
	nKeyin = cls_show_msg1(5, "ENTER����ͨ��,��������ͨ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

	return;
}

