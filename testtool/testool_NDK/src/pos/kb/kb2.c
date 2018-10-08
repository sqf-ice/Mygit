/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 2.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_KbSwitch,NDK_KbGetSwitch
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_KbSwitch,NDK_KbGetSwitch"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: kb2
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*****************************************************************/
void kb2(void)
{
	/*private & local definition*/
	int ret = 0 ,cnt = 0;
	int nMode = 0, nMode2 = 0;
	int pnState = 0;	
	int pnState2 = 0;
	int nKeyin = ENTER;

#include "kbtable.h"
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣:nSelect=-1,2;nMode=-1,2;pnState=NULL
	if ((ret=NDK_KbSwitch(-1,0,&pnState))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_KbSwitch(1,-1,&pnState))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_KbSwitch(1,0,NULL))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if ((ret=NDK_KbGetSwitch(-1,&pnState2))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN; 
	}
	if ((ret=NDK_KbGetSwitch(nMode,NULL))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}


	//case2:����NDK_KbSwitch��nSelectΪ0,Ĭ�Ϲرտ���ͨ���������ķ��ؼ���������������֤	
	if ((ret=NDK_KbSwitch(0,0,&pnState))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//subcase1:������NDK_KbHit
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��(1������),NDK_KbHit��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = wait_anykey(MAXWAITTIME);
	if (nKeyin!=table[cnt].keycode && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}
	//subcase2:������NDK_KbGetCode(30)
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��(1������),NDK_KbGetCode(30)��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = lib_getkeycode(MAXWAITTIME);
	if (nKeyin!=table[cnt].keycode && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}
	
	//subcase3:������NDK_KbGetCode(0)
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��(1������),NDK_KbGetCode(0)��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = lib_getkeycode(0);
	if (nKeyin!=table[cnt].keycode && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}

	//case3:����NDK_KbGetSwitch,����֤
	if ((ret=NDK_KbGetSwitch(nMode2,&pnState2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (pnState2!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pnState2);
		RETURN;
	}

	//case4:����NDK_KbSwitch��nSelectΪ1,�����ó�������
	if ((ret=NDK_KbSwitch(1,0,&pnState))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//subcase1:������NDK_KbHit���Ի�ȡ��������
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��(1������),NDK_KbHit��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = wait_anykey(MAXWAITTIME);
	if (nKeyin!=(table[cnt].keycode+0x4000) && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}
	
	//subcase2:������NDK_KbGetCode(30)���Ի�ȡ��������	
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��(1������),NDK_KbGetCode(30)��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = lib_getkeycode(MAXWAITTIME);
	if (nKeyin!=(table[cnt].keycode+0x4000) && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}
	//subcase3:������NDK_KbGetCode(0)���Ի�ȡ��������
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("�밴��%s��(1������),NDK_KbGetCode(0)��������\n", table[cnt].keyname);	
	nKeyin = lib_getkeycode(0);
	if (nKeyin!=(table[cnt].keycode+0x4000) && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}
	cls_printf("�밴��F1��(1������),NDK_KbGetCode(0)��������\n");	
	nKeyin = lib_getkeycode(0);
	if (nKeyin!=KEY_F1 && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}


	//case5:����NDK_KbGetSwitch,����֤	
	if ((ret=NDK_KbGetSwitch(nMode2,&pnState2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (nMode2!=0||pnState2!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, nMode2, pnState);
		RETURN;
	}
	
	//case6::����NDK_KbSwitch��nSelectΪ1,��������ϼ���
	if ((ret=NDK_KbSwitch(0,1,&pnState))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//subcase1:������NDK_KbHit���Ի�ȡ����ϼ�
	//subcase2:������NDK_KbGetCode(30)���Ի�ȡ����ϼ�
	
	//subcase3:������NDK_KbGetCode(0)���Ի�ȡ����ϼ�
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("�밴��ϼ�F1+%s��\n", table[cnt].keyname);
	nKeyin = lib_getkeycode(0);
	if (nKeyin!=(table[cnt].keycode+0x8000) && nKeyin!=0)
	{
		send_result("line %d: %s����ʧ��(%04x)", __LINE__, TESTAPI, nKeyin);
		GOTOERR;
	}

	//case7:����NDK_KbGetSwitch,����֤
	if ((ret=NDK_KbGetSwitch(nMode2,&pnState2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (nMode2!=1 || pnState2!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, nMode2, pnState);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	
ERR:
	NDK_KbSwitch(1, 0, &pnState);
	return;
}

