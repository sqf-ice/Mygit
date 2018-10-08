/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest68.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����ѹ������
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"����ѹ������"

#define	DEFAULT_CNT_STR	"100"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  						huangjianb	20140625	created
*****************************************************************/
static void SysGoSuspend_ot(void)
{
	/*process body*/
	while(1)
	{
		if(ESC==cls_show_msg1(60, "��ȴ��Զ��������ߺ󰴵�Դ�����ѣ���ȴ�180���Զ�����..."))
			break;		
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME, "�Զ����߻���ѹ���������");
	return;
}

static void SysGoSuspend_hm(void)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = 0, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen = 0;
	
	/*process body*/
	cls_printf("Ĭ�����߲��Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr, 1,sizeof(CntStr)-1,&unLen, INPUTDISP_OTHER, 5,INPUT_CONTRL_LIMIT))==ESC||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default
			bak = cnt = DEFAULT_CNT_VLE;
	else
			bak = cnt = atoi(CntStr);	
			
	while(cnt)
	{
		if(ESC==cls_show_msg1(5, "�������ߣ���Դ�����ѻ�180����Զ����Ѱ���Դ�����ѣ�ESC�˳�" ))
			break;	
		cnt--;

#if ANDIROD_ENABLE
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
		if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
		{
			cls_show_msg1(g_keeptime, "line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
		succ++;
		
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"�������߲�����ɣ��ɹ�%d��", succ);
	return;
}


void systest68(void)
{
	int ret=0, keyin = 0, wtime = 180;
	
	if((ret=NDK_SysSetSuspendDuration(wtime)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if(auto_flag==1)//autotest_flag
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTAPI);

		//ִ�к�������ѹ��
		cls_show_msg1(2, "�������߲���...");
		SysGoSuspend_hm();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���",TESTAPI);
		NDK_SysSetSuspendDuration(0);
		return;
	}
	while(1)
	{
		keyin = cls_show_msg("1.�Զ�����ѹ��\n"
							"2.��������ѹ��");
		
		switch(keyin) 
		{
		case '1':// 1.�Զ�����;
			cls_show_msg1(2, "�Զ����߲���...");
			SysGoSuspend_ot();
			break;	
		case '2':// 2.�������ߣ�
			cls_show_msg1(2, "�������߲���...");
			SysGoSuspend_hm();
			break;
		case ESC:
			NDK_SysSetSuspendDuration(0);
			return;
			break;	
		default:
			continue;
			break;
		}
	}
	return;
}
