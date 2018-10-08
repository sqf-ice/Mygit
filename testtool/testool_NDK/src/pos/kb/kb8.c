/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name			: 8.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ����NDK_PS2Start,NDK_PS2Stop
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_PS2Start,NDK_PS2Stop"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: kb8
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 		:
* history 		 			: author			date			remarks
*			  	  	       
*****************************************************************/
void kb8(void)
{
	/*private & local definition*/
	int ret = 0 ,cnt = 0, key = 0;
	int nKeyin = ENTER;

#include "kbtable.h"
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	cls_show_msg("�뽫POS����PS2�������������������������");
	
	//case1:��������start����Զ�ȡ��Ӽ��̼�ֵ
	if ((ret=NDK_PS2Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	 cnt = rand()%(SZ_ARRAY(ps2_num));
	 cls_printf("����20s�ڰ��¼�����ߵ�%s��,NDK_PS2GetCode(20)��������\n", ps2_num[cnt].keyname);
	 if((ret = NDK_PS2GetCode(20,&key)) != NDK_OK)
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d)", __LINE__, ret);
		 GOTOERR;
	 }
	if(key!= ps2_num[cnt].keycode)
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(0x%02x)", __LINE__, key);
		 GOTOERR;
	}
	if ((ret=NDK_PS2Stop())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	
	//case2:���startӦ�÷��سɹ�
	if ((ret=NDK_PS2Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	if ((ret=NDK_PS2Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	if ((ret=NDK_PS2Stop())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	
	//case3:stop�󰴼���Ӧ��ȡ����ֵ
	cnt = rand()%(SZ_ARRAY(ps2_num));
	cls_printf("����20s�ڰ��¼�����ߵ�%s��,Ӧ���ñ�����\n", ps2_num[cnt].keyname);
	if ((ret=NDK_PS2GetCode(20,&key))!=NDK_OK || key!=0)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI,ret,key);
		RETURN;
	}

	//case4:���stopԤ�ڷ��سɹ�
	if ((ret=NDK_PS2Stop())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	if ((ret=NDK_PS2Stop())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

