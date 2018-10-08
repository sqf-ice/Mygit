/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	����/��ӡ����
* file name			: systest53.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ����/��ӡ�������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20131202  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"����/��ӡ����"
#define	MAXWAITTIME	(120)	//30->120 for ���
#define	DEFAULT_CNT_STR	"10"
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
* history 				: author			date			remarks
						jiangym			20131230	created
*****************************************************************/	
void print_touch(void)
{	
	int cnt = DEFAULT_CNT_VLE, bak = 0, ret = 0;
	uint len = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	//ע��:��λ�������ڽ��뺯��ѹ��ǰ��һ��,��֮��Ĳ�ͬ��ӡѹ�������в���(�������䲻����λ),�ԼӴ�ѹ��
	//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)
	{
		cls_show_msg("%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}	

	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "%s������...\n��ʣ%d��,��ע���ӡЧ��,ESC�жϲ���", TESTITEM, cnt))
			break;
		cnt--;
		
		print_bill();
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��,�ܹ�%d��,�ɹ�%d��", __LINE__, TESTITEM, bak, bak-cnt-1);
			return;
		}		
	}
	cls_show_msg("%s�������,�ܴ���Ϊ%d��,�ɹ�����Ϊ%d", TESTITEM, bak, bak-cnt);
	return;
}
void systest53(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);

	while(1)
    {
		keyin = cls_show_msg("1.����/��ӡ\n"
							 "0.��ӡ����");
		switch(keyin) 
		{
			case '1':
				print_touch();
				break;
			case '0':
				conf_prn();
				break;	
			case ESC:
				return;
			default:
				continue;
				break;
		}
	}	
	return;
}
