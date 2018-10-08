/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 1.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_KbFlush�ܷ�������̻�������NDK_KbGetCode�ܹ��ڳ�ʱʱ���ڶ�ȡ���̰���ֵ,NDK_KbHit�ܷ��ȡ�������е��׸����̼�ֵ����������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define  TESTAPI		"NDK_KbFlush,NDK_KbGetCode,NDK_KbHit"
#define  WUCHASEC 0.03//0.03 //���ֵ����
#define	 MAXTIMEOUT	10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: kb1
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*****************************************************************/
void kb1(void)
{
	/*private & local definition*/
	int cnt = 0,ret = 0;
	float tmp = 0;
	int nKeyin = 0;
#if K21_ENABLE
	char str[32] = {0};
#endif
	
#include "kbtable.h"
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:������Ĳ���:pnCodeΪNULL
#if !K21_ENABLE //K21�������ΪNULL
	if ((ret=NDK_KbHit(NULL))!=NDK_ERR_PARA)
#else
	if ((ret=NDK_KbHit(NULL))!=NDK_OK)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}
	
	//pnCodeΪNULLʱ,���سɹ�,��Ϊֻ���ڵȴ�ʱ�䰴��,��û��Ҫ��ȡ��ǰ�İ�������
	if ((ret=NDK_KbGetCode(2,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		RETURN;
	}	

	//case2:����NDK_KbFlush"�������������"���ܺ�NDK_KbHit"��鰴��������"
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��,NDK_KbHit��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = wait_anykey(MAXWAITTIME);
	if (nKeyin!=table[cnt].keycode && nKeyin!=0)
	{
		send_result("line %d: NDK_KbHit����ʧ��", __LINE__);
		RETURN;
	}

	//case3:����NDK_KbGetCode(30)
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����%ds�ڰ���%s��,NDK_KbGetCode(30)��������\n", MAXWAITTIME, table[cnt].keyname);
	nKeyin = lib_getkeycode(30);
	if (nKeyin!=table[cnt].keycode && nKeyin!=0)
	{
		send_result("line %d: NDK_KbGetCode����ʧ��", __LINE__);
		RETURN;
	}

	//case4:����NDK_KbGetCode(0)
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("�밴��%s��,NDK_KbGetCode(0)��������\n", table[cnt].keyname);
	nKeyin = lib_getkeycode(0);
	if (nKeyin!=table[cnt].keycode && nKeyin!=0)
	{
		send_result("line %d: NDK_KbGetCode����ʧ��", __LINE__);
		RETURN;
	}

	//case5:��ʱ����
	cls_printf("����%ds�ڲ������κμ�,�����ĵȴ�...", MAXTIMEOUT);
	lib_StartStopwatch();
	nKeyin = lib_getkeycode(MAXTIMEOUT);
#if defined ME30GX||defined ME32GX
	if (nKeyin!=0||(tmp=abs(INT_ReadStopwatch()-MAXTIMEOUT))>WUCHASEC)
#else
	if (nKeyin!=0||(tmp=fabs(lib_ReadStopwatch()-MAXTIMEOUT))>WUCHASEC)
#endif
	{
#if !K21_ENABLE
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, nKeyin, tmp);
#else
		memset(str,0,sizeof(str));
		ftos(tmp, str);
		send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI, nKeyin, str);
#endif
		RETURN;
	}

#if !K21_ENABLE	//�Ͷ˲�Ʒ��֧���Զ����߹���,��û�����߿���,������20160729 linwl
#if !(defined SP60||defined ME31S||defined ME31SLS||defined SP33||defined SP600||defined SP60G||defined SP33G||defined SP600G||defined ME51||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG)
	//case6:������������ʱ�����Ѱ�����Ӧ��Ӧ�û�ȡ��(Ӧ����)
	if(cls_show_msg("����Ʒ�Ƿ�֧�����߲��Ѵ����߿���,��:��[ENTER],��:������")==ENTER)
	{
		if(cls_show_msg("��ȴ��������ߺ󰴼�����,���Ѻ�,���ڱ�����!��:��[ENTER],��:������")!=ENTER)
		{
			send_result("line %d: NDK_KbGetCode����ʧ��", __LINE__);
			RETURN;
		}
	}
#endif

	//case7:���߻��Ѻ󣬰������Ѻ���ӵ���һ������Ӧ��Ӧ�û�ȡ��
	if(cls_show_msg("����Ʒ�Ƿ�֧�����߲��Ѵ����߿���,��:��[ENTER],��:������")==ENTER)
	{
		cls_show_msg("��ȴ��������ߺ󰴼�����,���Ѻ�������ڱ�����,�����������");
		cnt = rand()%(SZ_ARRAY(table));
		cls_printf("�밴��%s��,NDK_KbGetCode(0)��������\n", table[cnt].keyname);
		nKeyin = lib_getkeycode(0);
		if (nKeyin!=table[cnt].keycode && nKeyin!=0)
		{
			send_result("line %d: NDK_KbGetCode����ʧ��", __LINE__);
			RETURN;
		}
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
	return;
}

