/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: NFC/RF����
* file name		: systest78.c
* Author 			: sunwx
* version			: V1.0
* DATE			: 20130120
* directory 		: 
* description		: NFC/RF����
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
#define	TESTITEM	"NFC/RF����"

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
*					sunwx			20141121		created
*****************************************************************/
static void rfid_nfc(SMART_t type)
{
	/*private & local definition*/
	
	int ret = 0, succ = 0, cnt = 0, bak = 0,pUIDlen = 0,pnRecvlen = 0, nSeekCnt = 3, rcvlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR, *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"}, msg[32] = {0};
	uchar UID[LEN_UID] = {0}, psRecebuf[64] = {0};
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");	
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	sprintf(msg, "%s/nfc����", card[type]);
	
	while(cnt)
	{
		//����
		RFID_deactive(type, 10);
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;

		//NFC  		
		cls_show_msg("���ƿ���Ƶ�������ֻ�NFC���Թ��߲�����pos,���������...");
		cnt--;
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		cls_show_msg("���ƿ��ֻ���ȷ����װ%s��,���������...", card[type]);

		//RFѰ��
		if ((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}

		//RF����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		//RF��д
		if((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}	
		succ++;
	
	}
	
	if ((ret=RFID_deactive(type, 0))!=SUCC)//�������,�������ùر�
	{	
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(%d)", __LINE__, msg, ret);
		//��ʱrf�µ�ʧ��,��������ѹ�����Գɹ���return;
	}
	
	cls_show_msg("%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

void systest78(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/

	int ret = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"9.RFID����\n");
		
		memcpy(g_RF_bak/*gen31_bak*/, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(ret)
		{
		case '1':
			rfid_nfc(type);
			break;
		case '9':
			switch(type=select_smarttype())
			{
			case CPU_A:
			case CPU_B:
			case MIFARE_1:
			case FELICA:
			case MIFARE_0:
			case ISO15693:
				if ((ret=RFID_init(type))!=SUCC)
					cls_show_msg("line %d:��ʼ��ʧ��(%d)!���������Ƿ���ȷ.", __LINE__, ret);
				else
					cls_show_msg1(2, "%s��ʼ���ɹ�!", card[type]);
				break;
			default:
				cls_show_msg("line %d:���ô���!������RFID(%d).", __LINE__, type);
				break;
			}
			break;
		case ESC:
			RFID_finish(type);
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

