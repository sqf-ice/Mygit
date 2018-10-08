/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ſ�/NFC����
* file name		: systest80.c
* Author 			: sunwx
* version			: V1.0
* DATE			: 20141008
* directory 		:
* description		: �ſ�/NFC����
* related document	: NL_POS����Ա�ο��ֲ�
*				  
************************************************************************
* log			: 
* Revision 1.0  20130120 chenfm
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"�ſ�/NFC����"

#define	DEFAULT_CNT_STR	"10"//"100"
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
*						sunwx		20141008		created
*****************************************************************/

void systest80(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	
	int ret = 0, succ = 0, cnt = 0, bak = 0,  pnRecvlen = 0, nSeekCnt = 3;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uchar psRecebuf[64] = {0};

	cls_show_msg1(1, "%s������...", TESTITEM);
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	cls_show_msg("����ֻ�NFC���Թ��߲�����pos,���������...");
	
	while(cnt)
	{
		if(ESC==cls_show_msg1(2, "%s������...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�",TESTITEM,cnt,succ))
			break;
		
		//ˢ�ſ�
		if ((ret=systest_MagCard())!=STRIPE)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(0x%x)", __LINE__, bak-cnt, TESTITEM, ret);
			continue;
		}
		//NFC
		//����NFC����֮����Ҫ�и�2S������ʱ�䣬�����ֻ�����������Ӧ
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, TESTITEM,ret);
			continue;
		}
		
		cnt--;
		succ++;
		
	}
	
	cls_show_msg("%s���,��ִ�д���Ϊ%d,�ɹ�%d��",TESTITEM, bak-cnt, succ);
	return;
}



