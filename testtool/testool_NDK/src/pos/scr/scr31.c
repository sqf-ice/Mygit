/************************************************************************
* copyright(c)2005-2016 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name			: 31.c
* Author 			: zhengry
* version			: 
* DATE				: 20160511
* directory 			: 
* description			: ����NDK_LedDisp�������ʾ��NDK_LedClr�����ȫ��
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_LedDisp,NDK_LedClr"
#define MAXWAITTIME  15

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct _LEDDISP_ALG
{
	uint justify;
	char *digits;
}LEDDISP_ALG;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scr31
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					    zhengry		    20160511		created
*****************************************************************/
void scr31(void)
{
	/*private & local definition*/
	int ret=0, i=0, nKeyin=0;
	LEDDISP_ALG abnormal[] ={
		{0, NULL},
		//{0, "12AB"}, //�ײ��޷��ж���ʾ�����Ƿ������ʲ���Ϊ�쳣��������20180425 modify
		{-1, "123"},
		{2, "123"},
	};
	LEDDISP_ALG normal[] = {
		{0, " . . . . . "},
		{0, "_._._._._._"},
		{0, "-.-.-.-.-.-"},
		{0, "0.0.0.0.0.0"},
		{0, "1.1.1.1.1.1"},
		{0, "2.2.2.2.2.2"},
		{1, "3.3.3.3.3.3"},
		{1, "4.4.4.4.4.4"},
		{1, "5.5.5.5.5.5"},
		{1, "6.6.6.6.6.6"},
		{1, "7.7.7.7.7.7"},
		{1, "8.8.8.8.8.8"},
		{1, "9.9.9.9.9.9"},
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

	//case1:�쳣��������:justify=-1/2ʱ
	for(i=0; i<SZ_ARRAY(abnormal); i++)
	{
		if((ret=NDK_LedDisp(abnormal[i].justify, abnormal[i].digits)) != NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		NDK_SysMsDelay(100);
	}	
	
	//case2:��������:justify=0 �����
	cls_printf("�����������...");
	if((ret=NDK_LedDisp(0, "1234")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "led������ϰ��������ʾ1234.��[ENTER],����[����].");
	if( nKeyin != ENTER &&  nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}

	//case3:��������:justify=1 �Ҷ���
	cls_printf("�����Ҷ�����...");
	if((ret=NDK_LedDisp(1, "1234")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "led������ϰ��Ҷ�����ʾ1234.��[ENTER],����[����].");
	if( nKeyin != ENTER &&  nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}

	//case4:��������:����������
	for(i=0; i<SZ_ARRAY(normal); i++)
	{
		if((ret=NDK_LedDisp(normal[i].justify, normal[i].digits)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		nKeyin=cls_show_msg1(MAXWAITTIME, "led������ϰ���ʾ:%s  ��[ENTER],����[����].", normal[i].digits);
		if( nKeyin != ENTER &&	nKeyin != 0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
			GOTOERR;
		}
	}

	//case5:��������:����6�����֣�Ӧ��������ʾ	
	cls_printf("�����ַ�������6������...");
	if((ret=NDK_LedDisp(0, "1234567")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "led���������ʾ123456  ��[ENTER],����[����].");
	if( nKeyin != ENTER &&  nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}

	//case6:��������:���ú�,�����Ӧȫ�����
	if((ret=NDK_LedClr())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "ԭ�����ŵ�led�����ȫ�����.��[ENTER],����[����].");
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
