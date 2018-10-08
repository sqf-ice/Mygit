/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 17.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrSetSpace�ܷ������м����ּ��
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrSetSpace"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr17
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr17(void)
{
	/*private & local definition*/
	int  ret = 0, i = 0, nKeyin = ENTER;	
	uint unWspace = 0, unHpace = 0;

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��:����ϵͳ����

	//case1:�����쳣:unWspace,unHpace�ֱ�Ϊ-1
	//�����������Ϊuint����-1���ƿ��Գɹ���Ӧ�û�������࣬������ֻΪ���Ӳ��Ը��Ƕ�
	if((ret=NDK_ScrSetSpace(-1,unHpace))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetSpace(unWspace,-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1:�����ּ��	
	//subcase1:������wspace���ڵ�ǰʹ�õ�Ӣ���������ؿ�ȵ�2��,Ԥ��:�ּ��ΪӢ���������ؿ�ȵ�2��
	NDK_ScrClrs();
	if((ret=NDK_ScrSetSpace(GUI_GetFontWidth()+10,unHpace))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",GUI_GetFontWidth(),unHpace);
	for(i=0; i<((GUI_GetLCDWidth()/GUI_GetFontWidth())/2-1); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();	
	NDK_SysDelay(20);
	
	//subcase2:������wspace���ڵ�ǰʹ�õ�Ӣ���������ؿ�ȵ�2��,�ּ��ΪӢ���������ؿ�ȵ�2��
	if((ret=NDK_ScrSetSpace(GUI_GetFontWidth(),unHpace))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",GUI_GetFontWidth(),unHpace);
	for(i=0; i<((GUI_GetLCDWidth()/GUI_GetFontWidth())/2-1); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();	
	NDK_SysDelay(20);

	NDK_ScrClrs(); //С����Ҫ����
	//subcase3:������wspaceС�ڵ�ǰʹ�õ�Ӣ���������ؿ�ȵ�2��,Ԥ��:�ּ��Ϊwspace
	if((ret=NDK_ScrSetSpace(GUI_GetFontWidth(),unHpace))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",GUI_GetFontWidth(),unHpace);
	for(i=0; i<((GUI_GetLCDWidth()/GUI_GetFontWidth())/2-1); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();
	NDK_SysDelay(40);
	
	if((ret=NDK_ScrSetSpace(GUI_GetFontWidth()-1,unHpace))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",GUI_GetFontWidth()-1,unHpace);
	for(i=0; i<((GUI_GetLCDWidth()/GUI_GetFontWidth())/2-1); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();
	NDK_SysDelay(40);
	if(ENTER != cls_show_msg1(MAXWAITTIME,"�ۿ�Ч��,��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s�����м�����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case2:�����м��	
	//subcase1:hspace���ڵ�ǰʹ�õ�Ӣ���������ظ߶�ʱ��Ԥ��:�м��ΪӢ���������ظ߶�
	NDK_ScrClrs();
	if((ret=NDK_ScrSetSpace(unWspace,GUI_GetFontHeight()+10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",unWspace,GUI_GetFontHeight());
	for(i=0; i<(GUI_GetLCDWidth()/GUI_GetFontWidth()); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	
	//subcase2:hspace���ڵ�ǰʹ�õ�Ӣ���������ظ߶�ʱ��Ԥ��:�м��ΪӢ���������ظ߶�
	if((ret=NDK_ScrSetSpace(unWspace,GUI_GetFontHeight()))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",unWspace,GUI_GetFontHeight());
	for(i=0; i<(GUI_GetLCDWidth()/GUI_GetFontWidth()); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	
	//subcase3:hspaceС�ڵ�ǰʹ�õ�Ӣ���������ظ߶�ʱ���м��Ϊhspace
	if((ret=NDK_ScrSetSpace(unWspace,GUI_GetFontHeight()-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
	NDK_ScrPrintf("Ԥ��:�ּ��(%d),�м��(%d)",unWspace,GUI_GetFontHeight()-1);
	for(i=0; i<(GUI_GetLCDWidth()/GUI_GetFontWidth()); i++)
		NDK_ScrPrintf("��");
	NDK_ScrRefresh();
	NDK_SysDelay(40);
	
	nKeyin = cls_show_msg1(MAXWAITTIME, "�ۿ�Ч��,��ȷ�ϲ���ͨ����������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);	
	
	//�ָ�Ĭ������
ERR:
	//��Ҫ������ȷ��Ĭ��ֵ
	NDK_ScrSetSpace(0,0);
	return;
}

