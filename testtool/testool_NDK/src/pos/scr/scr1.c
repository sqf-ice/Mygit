/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 1.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrGotoxy �Ƿ������ȷ�ƶ����ڣ�y+1���еڣ�x+1���д�������NDK_ScrGetxy�Ƿ������ȷȡ���λ�õ��кź��к�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define	 MAXTIMEOUT	10
#define 	TESTAPI		"NDK_ScrGotoxy,NDK_ScrGetxy"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr1
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr1(void)
{
	/*private & local definition*/
	uint PosX = 0, PosY = 0;
	int nKeyin = ENTER, ret = 0;
	uint getx = 0, gety = 0, ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	lib_kbflush();		//�������������
	//Ϊʵ���Զ������ԣ�����������
	lib_StartStopwatch();
	while ((nKeyin=lib_kbhit())==0)
	{
		PosX = rand()%(ViewWidth-(GUI_GetFontWidth()/2));
		PosY = rand()%(ViewHeight-GUI_GetFontHeight());
		//NDK_ScrGotoxy :  0<= x <=(LCDCOL-1) ;0<= y <= (LCDROW-1) (��λ��CHAR)
		cls_show_msg1(3, "��꽫�ƶ���(%d,%d),�����������...", PosX, PosY);
		
		//case1:����scrgotoxy�����ƶ�
		NDK_ScrClrs();
		if((ret=NDK_ScrGotoxy(PosX, PosY))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}	
		
		//case2:����scrgetxy��ȡ����ֵ�����õ�ֵһ��
		if((ret=NDK_ScrGetxy(&getx, &gety))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if (getx!=PosX || gety!= PosY)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, getx, gety);
			RETURN;
		}
		NDK_ScrPrintf("|�������!");
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(20);
		cls_printf("����%s...��ȷ�ϲ���ͨ��,������ʧ��", TESTAPI);
		NDK_SysDelay(20);
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

	//case3:��Ӧ�ߵ�״̬��(730׷�Ӳ���)
#if  0 //STATUSBAR_ENABLE

	if((ret=NDK_ScrGotoxy(0, 18))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetxy(&getx, &gety))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(18==gety)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	if((ret=NDK_ScrGotoxy(0, 19))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetxy(&getx, &gety))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(19==gety)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif
	//���������Ĳ���ǰ��
	PosX = rand()%ViewWidth;
	PosY = rand()%ViewHeight;
	if((ret=NDK_ScrGotoxy(PosX, PosY))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:�����쳣���ԡ��ĵ�˵�������������Ƿ������걣�ֵ�ǰλ�ò��䡣
	// ��,���������ǲ������õġ�
	//case4.1:x�����쳣
	if((ret=NDK_ScrGotoxy(ViewWidth, 0))!=NDK_ERR_PARA)//!=���󷵻�ֵ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetxy(&getx, &gety))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(ViewWidth==getx || getx!=PosX|| gety!= PosY)
	{
		send_result("line %d:%s����ʧ��%d,(%d,%d)(%d,%d)", __LINE__, TESTAPI, ViewWidth, getx, gety, PosX, PosY);
		RETURN;
	}

	//case4.2:y�����쳣
	if((ret=NDK_ScrGotoxy(0, ViewHeight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetxy(&getx, &gety))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(ViewHeight==gety || getx!=PosX || gety!= PosY)
	{
		send_result("line %d:%s����ʧ��%d,(%d,%d)(%d,%d)", __LINE__, TESTAPI, ViewHeight, getx, gety, PosX, PosY);
		RETURN;
	}
	
	//case4.3:x,y�����쳣
	if((ret=NDK_ScrGotoxy(ViewWidth, ViewHeight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetxy(&getx, &gety))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(ViewWidth==getx || ViewHeight==gety || getx!=PosX || gety!= PosY)
	{
		send_result("line %d:%s����ʧ��(%d,%d)(%d,%d)(%d,%d)", __LINE__, TESTAPI, ViewWidth, ViewHeight, getx, gety, PosX, PosY);
		RETURN;
	}

	//��������
	send_result("%s����ͨ��", TESTAPI);
	return;
}

