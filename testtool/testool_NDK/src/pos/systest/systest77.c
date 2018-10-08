/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	NFC�ۺ�
* file name		: 	systest77.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: 	NFC�ۺϲ���
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

#define	TESTITEM	"NFC�ۺϲ���"
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
					 huangjianb		20140926	created
*****************************************************************/	
static void nfc_test(void)
{
	int ret=0, succ = 0 ,cnt=0 ,i=0, bak = 0, nSeekCnt = 3 ,pnRecvlen = 0;
	uint unLen = 0;
	uchar psRecebuf[64] = {0};
	char CntStr[8] = DEFAULT_CNT_STR;
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
		
	cls_show_msg("��ȷ���Ѵ��ֻ�NFC���Թ��߲�����pos,���������...");
	
	while(cnt)
	{
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		i++;
		cnt--;
		//����NFC����֮����Ҫ�и�2S������ʱ�䣬�����ֻ�����������Ӧ
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)	
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����ʧ��(%d)", __LINE__, ret);
			continue;
		}

		succ++;
	}
	
	cls_show_msg("ѹ�����,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

void systest77(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int key = 0;
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	
	while (1)
	{
		key = cls_show_msg("NFC�ۺϲ���_\n"
							"1.NFCѹ������\n");
		switch(key)
		{
		case '1':
			nfc_test();
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
