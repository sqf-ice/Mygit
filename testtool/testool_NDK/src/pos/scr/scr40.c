/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 40.c
* Author 			:
* version			: 
* DATE			: 
* directory 		: 
* description		: ����NDK_ScrClrsClors�Ƿ���԰���ɫ�����Ļ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScrClrsColor"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr40
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	    
*
*****************************************************************/
void scr40(void)
{
	/*private & local definition*/
	int i = 0, ret = 0;
	color_t color_tbl[] = {0x0000/*��*/, 0xFFFF/*��*/, 0xF800/*��*/, 0x07E0/*��*/, 0x001F/*��*/, 0x07FF/*��*/};
	char *color[] = {"��", "��", "��", "��", "��", "��"};

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//case1:����NDK_ScrClrsColor
	for(i=0; i<SZ_ARRAY(color_tbl); i++)
	{
		cls_show_msg("�������ȫ��Ļ��ʾ%sɫ������Ļ�ϳ�����ɫ��Ӧ����������",color[i]);
		if((ret = NDK_ScrClsColor(color_tbl[i])) != NDK_OK) 
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			RETURN;
		}
		NDK_SysMsDelay(1*1000); //��ʱ
	}
	if(cls_show_msg("������Ϣ����Ļ[0,0]��������ʾ?��[ENTER[,��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}



