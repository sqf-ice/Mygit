/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ������ѹ������
* file name			: systest44.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ������ѹ������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130221  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"������ѹ������"

#define	DEFAULT_CNT_STR	"1500"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
#define	MAXWAITTIME	(30)	

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
* history 				: author			date			remarks

*****************************************************************/
void SysBeep_press(void)
{	
	int ret = 0;
	int  i = 0, cnt = DEFAULT_CNT_VLE, succ = 0;
	char CntStr[8] = DEFAULT_CNT_STR; 
	uint unLen = 0;
	
	//����ǰ��
#if !K21_ENABLE
	NDK_SysSetBeepVol(5);
#endif
	
	// ���Է�������������
	cls_printf("Ĭ�ϲ��Դ���:\n");
	//Ϊ�˼����޼��̵Ĳ�Ʒʹ�÷�װ�ĺ���20141031 jiangym modify
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)   
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	while(1)
	{
		cls_printf("NDK_SysBeepѹ��������\n�ܹ�:%d��,�ѽ���:%d��",cnt,i);
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;
		if((ret=NDK_SysBeep())!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:SysBeepѹ������ʧ��(%d)", __LINE__, ret);
			return;
		}
		succ++;
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SysBeepѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succ);	
	return;
}
	
void SysTimeBeep_press(void)
{	
	int ret = 0;
	int  i = 0, cnt = DEFAULT_CNT_VLE, succ = 0;
	char CntStr[8] = DEFAULT_CNT_STR; 
	uint unLen = 0;
	
	//����ǰ��
#if !K21_ENABLE
	NDK_SysSetBeepVol(5);
#endif
	
	//�����ڸ���ʱ�����ڷ���һ��Ƶ�ʵ�����
	cls_printf("Ĭ�ϲ��Դ���:\n");
	//Ϊ�˼����޼��̵Ĳ�Ʒʹ�÷�װ�ĺ���20141031 jiangym modify
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	while(1)
	{
		cls_printf("NDK_SysTimeBeepѹ��������\n�ܹ�:%d��,�ѽ���:%d��",cnt,i);
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;
		if((ret=NDK_SysTimeBeep(1000,500)) != NDK_OK)//Ƶ��1000,���0.5s
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:SysTimeBeepѹ������ʧ��(%d)", __LINE__, ret);
			return;
		}
		succ++;
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"SysTimeBeepѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succ);	
	return;
}

#if 0//!K21_ENABLE
void SysTimeBeepUnblock_press(void)
{
	int ret = 0;
	int  i = 0, cnt = DEFAULT_CNT_VLE, succ = 0;
	char CntStr[8] = DEFAULT_CNT_STR; 
	uint unLen = 0;
	
	//����ǰ��
#if !K21_ENABLE
	NDK_SysSetBeepVol(5);
#endif
	
	//�����ڸ���ʱ�����ڷ���һ��Ƶ�ʵ�����
	cls_printf("Ĭ�ϲ��Դ���:\n");
	//Ϊ�˼����޼��̵Ĳ�Ʒʹ�÷�װ�ĺ���20141031 jiangym modify
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	while(1)
	{
		cls_printf("SysTimeBeepUnblockѹ��������\n�ܹ�:%d��,�ѽ���:%d��",cnt,i);
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;
		if((ret=NDK_SysTimeBeepUnblock(1000,500)) != NDK_OK)//Ƶ��1000,���0.5s
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:SysTimeBeepѹ������ʧ��(%d)", __LINE__, ret);
			return;
		}
		sleep(0.5);
		succ++;
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"SysTimeBeepѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succ); 
	return;
}
#else
void SysTimeBeepUnblock_press(void){cls_show_msg1(5, "�Ͷ˲�Ʒ��֧��SysTimeBeepUnblock,����Ѿ�֧��,����ϵ���Բ���������");return;}
#endif

void systest47(void)
{	
	/*private & local definition*/
	int keyin = 0;
		
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)//autotest_flag
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��SysBeepѹ������
		SysBeep_press();

		//ִ��SysTimeBeepѹ������
		SysTimeBeep_press();
		
		//ִ��SysTimeBeepUnblockѹ������
		//SysTimeBeepUnblock_press();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		keyin = cls_show_msg("1.SysBeepѹ������\n"
							 "2.SysTimeBeepѹ������\n"
							 "3.SysTimeBeepUnblockѹ������");
		switch(keyin)
		{
			case '1':
				SysBeep_press();
				break;
			case '2':
				SysTimeBeep_press();
				break;	
			case '3':
				SysTimeBeepUnblock_press();
				break;	
			case ESC:
				return;
				break;
			default:
				continue;
				break;
		}
	}
	return; 
}


