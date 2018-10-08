/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ICSAM/NFC����
* file name		: systest82.c
* Author 			: 
* version			: V1.0
* DATE			: 20130120
* directory 		: 
* description		: ICSAM/NFC����
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
#define	TESTITEM	"ICSAM/NFC����"

#define	DEFAULT_CNT_STR	"10"//"1000"
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
*						sunwx		20141121		created
*****************************************************************/
static void icsam_nfc(int i)
{
	/*private & local definition*/
#include "icsam.h"
	
	int ret = 0, succ = 0, cnt = 0, bak = 0, pnAtrlen  = 0,pnRecvlen,nSeekCnt = 3;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR, msg[32] = {0};
	uchar psRecebuf[64],buf[256] = {0};

	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");	
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	cls_show_msg("��ȷ���Ѱ�װ%s��,���ֻ�NFC���Թ��߲�����pos,���������...",  CardNoStr[i]);
	sprintf(msg, "%s/nfc����", CardNoStr[i]);
	
	while(cnt)
	{
		//����
		ICSAM_powerdown(CardNo[i]);
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;
		
		//NFC  
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		
		//ICSAM�ϵ�
		if ((ret=NDK_IccPowerUp(CardNo[i], buf,&pnAtrlen ))!=NDK_OK)
		{ 
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}	
		while(cnt)
		{	
			if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
				break;
			cnt--;		
			//NFC
			sleep(2);
			if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
				break;
			}
			//ICSAM��д
			if((ret=ICSAM_APDUrw(CardNo[i]))!=SUCC)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
				break;
			}	
			succ++;
		}
	}
	if ((ret=ICSAM_powerdown(CardNo[i]))!=NDK_OK)//�������,�������ùر�
	{	
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(0x%x)", __LINE__, msg, ret);
		//��ʱICSAM�µ�ʧ��,��������ѹ�����Գɹ���return;
	}
	cls_show_msg("%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

void systest82(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
#include "icsam.h"

	int ret = 0, i = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.ICSAM����\n");
		
		switch(ret)
		{
		case '1':
			icsam_nfc(i);
			break;
		case '0':
			if ((i=select_ICSAM())==NDK_ERR_QUIT)
				;//QUIT����ʾ
			else
				cls_show_msg1(2, "��ѡ��%s",  CardNoStr[i]);
			break;
		case ESC:
			ICSAM_powerdown(CardNo[i]);
			cls_show_msg("��ѡ��������Ƭ���²���!");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

