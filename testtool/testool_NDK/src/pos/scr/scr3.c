/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 3.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrPush�Ƿ���Ա��浱ǰ��Ļ��ʾ״̬,����NDK_ScrPop�����ָܻ������к�һ��pushscreen�������ʾ״̬
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_ScrPush,NDK_ScrPop"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr3
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr3(void)
{
	/*private & local definition*/
	uint PosX = 0, PosY = 0;
	int nKeyin = ENTER, i = 0, ret = 0;
	EM_TEXT_ATTRIBUTE iOldAttr = {0};
	uint ViewWidth = 0, ViewHeight = 0;

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	NDK_ScrClrs();
	GUI_DispStringEx(0, 0, "�߱����߹��ܵĻ���,��ȷ���ѿ�������!ENTER������...", TEXT_ATTRIBUTE_REVERSE);
	if(ENTER!=lib_getkeycode(MAXWAITTIME)&&auto_flag!=1)
	{
		send_result("line %d:%sδ�����޸ģ�������ֹ", __LINE__, TESTAPI);
		return;
	}
	
	//case1:����push���Σ������Ƿ�ȷʵֻ�������һ�ε�screen��Ϣ
	while(i<2)
	{
		NDK_ScrClrs();
		PosX = rand()%(ViewWidth-(GUI_GetFontWidth()/2));
		PosY = rand()%(ViewHeight-GUI_GetFontHeight());		//Ϊ��������ʾ��ʾ��Ϣ,����-Ӣ������Ŀ�͸ߵĲ���
		NDK_ScrGotoxy(PosX, PosY);
		NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,&iOldAttr);
		NDK_ScrPrintf("����NDK_ScrPush...");
		NDK_ScrSetAttr(TEXT_ATTRIBUTE_NORMAL,&iOldAttr);
		show_msg1(MAXWAITTIME, "�������NDK_ScrPush ��%d��", ++i);	
		if((ret=NDK_ScrPush())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
	}

	//case2:�ָ�����������һ�ε�screen��Ϣ
	cls_show_msg1(MAXWAITTIME,"��������ָ�[NDK_ScrPush ��2��].������������,���Ѻ�,���ڱ�����!");//��Ļ��Ϣ�Ѹı�
	if((ret=NDK_ScrPop())!=NDK_OK)//��Ļ�ı�Ϊ"pushscreen ��2��"����Ϣ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);//����һ���Ĺ۲�ʱ��

	//case3:��֤�ú������ܹ�Ƕ��ʹ��,��2��POP��Ȼֻ�ָܻ���2�α���Ľ���
	cls_show_msg1(MAXWAITTIME,"���������Ӧ�ָ�[NDK_ScrPush ��1��].");
	if((ret=NDK_ScrPop())!=NDK_OK)//��Ļ�ı�Ϊ"pushscreen ��2��"����Ϣ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);//����һ���Ĺ۲�ʱ��
	
	nKeyin = cls_show_msg1(MAXWAITTIME, "��ȷ�ϲ���ͨ��,������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	
	return;
}

