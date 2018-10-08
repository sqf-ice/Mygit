/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾģ��
* file name			: 30.c
* Author 			: zhengry
* version			: 
* DATE				: 20160510
* directory 			: 
* description			: ����NDK_LedLight���������ȫ����ȫ��
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_LedLight"
#define MAXWAITTIME  15

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct _LEDLIGHT_ALG
{
	uchar nr;
	uint onoff;
}LEDLIGHT_ALG;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scr30
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					    zhengry		    20160510		created
*****************************************************************/
void scr30(void)
{
	/*private & local definition*/
	int ret=0, i=0, nKeyin=0;
	LEDLIGHT_ALG abnormal[]={
		{0, 0},
		{8, 0},
		{0, 1},
		{8, 1},
		{1, -1},
		{1, 2},
	};
		
	/*process body*/
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_LedClr();
	if((ret=NDK_LedSetBright(3)) != NDK_OK)//������������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1:�쳣��������:nr=0/8ʱ
	for(i=0; i<SZ_ARRAY(abnormal); i++)
	{
		if((ret = NDK_LedLight(abnormal[i].nr, abnormal[i].onoff)) != NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
	}
	
	//case2:��������:�����ȫ�������ȫ�����
	cls_printf("����ledȫ������������...");
	for(i=1; i<8; i++)
	{
		if((ret = NDK_LedLight(i, 1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		NDK_SysMsDelay(200);
	}
	for(i=1; i<8; i++)
	{
		if((ret = NDK_LedLight(i, 0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		NDK_SysMsDelay(200);
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "led�����ȫ�������ȫ���.��[ENTER],����[����].");
	if( nKeyin != ENTER &&  nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}

	//case3:��������:�����������һ����
	cls_printf("����led��һ������...");
	for(i=1; i<8; i++)
	{
		if((ret = NDK_LedLight(i, 1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		NDK_SysMsDelay(200);
		if((ret = NDK_LedLight(i, 0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		NDK_SysMsDelay(200);
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "led�������һ����.��[ENTER],����[����].");
	if( nKeyin != ENTER &&  nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_LedClr();
	return;
}

