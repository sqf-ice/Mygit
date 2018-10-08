/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	����/�ſ�����
* file name			: systest54.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ����/�ſ��������
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
#define	TESTITEM	"����/�ſ�����"
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
void systest54(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = 0, bak = 0,cnt = 0, succ = 0;
	uint unLen = 0;
	char  CntStr[8] = DEFAULT_CNT_STR;

	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	while(1)
	{
		if(bak++==cnt)
			break;
		if(ESC==cls_show_msg1(2, "���ڽ���%d��%s����,�ɹ�%d��,ESC�˳�", bak, TESTITEM, succ))
			break;

		//��������
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			continue;
		}
		
		//�ſ�����
		if(systest_MagCard()!=STRIPE)
			continue;

		succ++;
	}
	cls_show_msg("%s�������,�ܹ�����%d�γɹ�%d��\n", TESTITEM, bak-1, succ);
	return;
}

