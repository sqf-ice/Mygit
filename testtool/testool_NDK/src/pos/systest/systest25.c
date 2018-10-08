/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: IC��/RF����
* file name		: systest25.c
* Author 			: chenfm
* version			: V1.0
* DATE			: 20130120
* directory 		: 
* description		: IC��/RF����
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
#define	TESTITEM	"ICSAM/RF����"

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
*					zhenglw			20120314			created
*					zhangnw			20120321			modify
*****************************************************************/
static void iccard_rfcard(SMART_t type, int i)
{
	/*private & local definition*/
#include "icsam.h"
	
	int ret = 0, ret1 = 0, succ = 0, cnt = 0, bak = 0,pnAtrlen  = 0,pUIDlen = 0, rcvlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR, *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"}, msg[32] = {0};
	uchar UID[LEN_UID] = {0},buf[256] = {0};
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");	
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	cls_show_msg1(g_keeptime, "��ȷ���Ѱ�װ%s/%s��,���������...",  CardNoStr[i], card[type]);
	sprintf(msg, "%s/%s����", CardNoStr[i], card[type]);
	
	while(cnt)
	{
		//����
		ICSAM_powerdown(CardNo[i]);
		RFID_deactive(type, 10);

		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;

		//RFѰ��
		if ((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
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

		//RF����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		//APDU��д
		while(cnt)
		{
			if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
				break;
			cnt--;
			
			//RF��ICSAM��д
			if((ret=ICSAM_APDUrw(CardNo[i]))!=SUCC||(ret1=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d,%d)", __LINE__, bak-cnt, msg, ret, ret1);
				break;
			}
			
			succ++;
		}
	}
	
	if ((ret=ICSAM_powerdown(CardNo[i]))!=NDK_OK||(ret1=RFID_deactive(type, 0))!=SUCC)//�������,�������ùر�
	{	
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(%d,%d)", __LINE__, msg, ret, ret1);
		//��ʱsmart�µ�ʧ��,��������ѹ�����Գɹ���return;
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

static void ic_rf_active(SMART_t type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = 0, bak = 0,pnAtrlen  = 0,pUIDlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uchar UID[LEN_UID] = {0},buf[256] = {0};
	uchar SAK[LEN_SAK] = {0},picctype = 0;;
	
	/*process body*/
	if(cls_show_msg("�˲�����Ҫһ�ż���IC��������Ƶ�����ܵĶ๦�ܿ�,[ESC]�˳�")==ESC)
		return;

	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	cls_show_msg("�뽫�๦�ܿ�����IC������,���������");
	while(cnt)
	{
		//����
		ICSAM_powerdown(ICTYPE_IC);
		RFID_deactive(type, 10);

		cls_printf("%s������...\n��ʣ%d��(�ѳɹ�%d��)", TESTITEM, cnt, succ);
			
		//RFѰ��������
		switch(type)
		{
		case CPU_A:
		case CPU_B:
			NDK_RfidPiccDetect(NULL);
			NDK_RfidPiccActivate(&picctype, &pUIDlen, UID);
			break;
		case MIFARE_1:
			NDK_M1Request(REQA, &pUIDlen, UID);
			NDK_M1Anti(&pUIDlen, UID);
			NDK_M1Select(pUIDlen, UID, SAK);
			NDK_M1ExternalAuthen(pUIDlen, UID, AUTHKEY_TYPE_A, AUTHKEY, 0x01);
			break;
		case FELICA:
			NDK_RfidFelicaPoll(UID, &pUIDlen);
			break;
#if M0_ENABLE 
		case MIFARE_0:
			NDK_MifareActive(REQA, UID, &pUIDlen,SAK);			
			break;
#endif
		default:
			return ;
			break;
		}
		
		//IC�ϵ�
		if ((ret=NDK_IccPowerUp(ICTYPE_IC, buf,&pnAtrlen ))!=NDK_OK)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��IC���ϵ�ʧ��(%d)", __LINE__, bak-cnt,ret);
			continue;
		}	
#if 0
		//IC��д
		if((ret=ICSAM_APDUrw(ICTYPE_IC))!=SUCC)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��IC����дʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;
		}
#endif		
		//IC�µ�
		if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��IC���µ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;
		}

		succ++;
		cnt--;
	}
	
	cls_show_msg("%s���,��ִ�д���Ϊ%d,�ɹ�%d��", TESTITEM, bak-cnt, succ);
	return;
}

void systest25(void)
{
	/*private & local definition*/
#include "icsam.h"

	int ret = 0, i = 0;
	SMART_t type = CPU_A, type1 = IC;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	if(auto_flag==1)
	{
		//�Զ����ò�����ʧ�����˳�
		switch(type=select_smarttype())
		{
		case CPU_A:
		case CPU_B:
		case MIFARE_1:
		case FELICA:
		case MIFARE_0:
		case ISO15693:
			break;
		case SAM:
		case IC:
			type = CPU_A;
			break;
		default:
			break;
		}
		if ((ret=RFID_init(type))!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��Ƶ����ʼ��ʧ��(%d)!���������Ƿ���ȷ.", __LINE__, ret);
			return;
		}
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//���������ļ��������Ƶ�����ͽ�����Ƶ����IC/SAM�Ľ���
#if IC1_ENABLE
		i = 0;
		iccard_rfcard(type, i);
#endif
#if SAM1_ENABLE
		i = 1;
		iccard_rfcard(type, i);
#endif
#if SAM2_ENABLE
		i = 2;
		iccard_rfcard(type, i);
#endif
#if SAM3_ENABLE
		i = 3;
		iccard_rfcard(type, i);
#endif
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
						     "2.IC/RF���\n"
							"0.ICSAM����\n"
							"9.RFID����\n");
		
		memcpy(g_RF_bak/*gen31_bak*/, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(ret)
		{
		case '1':
			iccard_rfcard(type, i);
			break;
		case '2':
			ic_rf_active(type);
			break;
		case '0':
			switch(type1=select_smarttype())
			{
			case SAM:
			case IC:
				//conf_icsam_pps();
				if ((i=select_ICSAM())==NDK_ERR_QUIT)
					;//QUIT����ʾ
				else
					cls_show_msg1(2, "��ѡ��%s",  CardNoStr[i]);
				break;
			default:
				cls_show_msg("line %d:���ô���!������IC/SAM(%d).", __LINE__, type1);
				break;
			}
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
			ICSAM_powerdown(CardNo[i]);
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

