/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	����/�ſ�����
* file name			: systest76.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ����/NFC�������
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
#define	TESTITEM	"����/NFC����"
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
					 huangjianb		20141009	created
*****************************************************************/	
void systest76(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = -1, bak = 0,cnt = 0, succ = 0 , nSeekCnt = 3 ,pnRecvlen = 0;
	uint unLen = 0;
	char  CntStr[8] = DEFAULT_CNT_STR;
	uchar psRecebuf[64] = {0};

	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);

	cls_show_msg("����ֻ�NFC���Թ��߲�����pos...,���������");
	
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
		
		//NFC����
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����ʧ��(%d)", __LINE__, ret);
			continue;
		}

		succ++;
	}
	cls_show_msg("%s�������,�ܹ�����%d�γɹ�%d��\n", TESTITEM, bak-1, succ);
	return;
}

