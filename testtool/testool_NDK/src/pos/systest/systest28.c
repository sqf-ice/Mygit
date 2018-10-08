/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: SMART/��ӡ����
* file name		: systest28.c
* Author 			: chensj
* version			: V1.0
* DATE			: 20130221
* directory 		: 
* description		: SMART/��ӡ����
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
#define	TESTITEM	"SMART/��ӡ����"

#define	DEFAULT_CNT_STR	"10"//"100"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
#define	MAXWAITTIME	(180)	//30->180 for ���

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
*					chensj			2013/02/21     created
*****************************************************************/
static void smart_print(SMART_t type, int i)
{
	/*private & local definition*/
#include "icsam.h"
	int ret = 0, ret1 = 0, succ = 0, cnt = 0, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR, *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"}, msg[32] = {0};
	uchar UID[LEN_UID] = {0};
	uint unLen=0;
	int nUIDlen=0;
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr, 1,sizeof(CntStr)-1,&unLen, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==ESC||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	cls_show_msg1(g_keeptime, "��ȷ���Ѱ�װ%s��,���������...", (type==SAM||type==IC)?CardNoStr[i]:card[type]);
	if (type==SAM||type==IC)
	{
		sprintf(msg, "%s/��ӡ����", CardNoStr[i]);
		UID[0] = CardNo[i];
	}
	else
		sprintf(msg, "%s/��ӡ����", card[type]);	
	
	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)//if (FAIL==clrprintbuf())
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ����ʹ������Чmodify 20131122 jiangym
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}
	
	while(cnt)
	{
		//����
		smart_deactive(type, CardNo[i], 10);
		
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;
		
		//ȡ��ӡ״̬��smartѰ��
		print_bill();
		if ((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC||(ret1=smart_detect(type, UID,&nUIDlen))!=SUCC)
		{
			cnt--;
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(0x%x,%d)", __LINE__, bak-cnt, msg, ret, ret1);
			continue;
		}
		
		//ȡ��ӡ״̬��smart����
		print_bill();
		if ((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC||(ret1=smart_active(type,UID,&nUIDlen))!=SUCC)
		{
			cnt--;
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(0x%x,%d)", __LINE__, bak-cnt, msg, ret, ret1);
			continue;
		}
		
		while(cnt)
		{
			if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
				break;
			cnt--;
			
			//ȡ��ӡ״̬��smart��д
			print_bill();
			if((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC||(ret1=smart_APDUrw(type, CardNo[i], UID))!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(0x%x,%d)", __LINE__, bak-cnt, msg, ret, ret1);
				break;
			}
			
			succ++;
		}
	}
	
	//smart�µ�
	print_bill();
	if ((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC||(ret1=smart_deactive(type, CardNo[i], 0))!=SUCC)//�������,�������ùر�
	{
		cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:%sʧ��(0x%x,%d)", __LINE__, msg, ret, ret1);
		//��ʱsmart�µ�ʧ��,��������ѹ�����Գɹ���return;
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

void systest28(void)
{
	/*private & local definition*/
//#include "icsam.h"
	int ret = 0, i = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	char *CardNoStr[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		conf_prn();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		
		switch(type=select_smarttype())
		{
		case CPU_A:
		case CPU_B:
		case MIFARE_1:
		case FELICA:
		case MIFARE_0:	
		case ISO15693:
			if ((ret=RFID_init(type))!=SUCC)
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��Ƶ����ʼ��ʧ��(%d)!���������Ƿ���ȷ.", __LINE__, ret);
				return;
			}
			//���������ļ��������Ƶ�����ͽ�����Ƶ����PRINT�Ľ���
			smart_print(type, i);
			break;
		case SAM:
		case IC:
			//type = CPU_A;
			break;
		default:
			break;
		}
		
		//��˳����������IC/SAM1/SAM2/SAM3��print�Ľ���
#if IC1_ENABLE
		type = IC;
		i = 0;
		smart_print(type, i);
#endif
#if SAM1_ENABLE
		type = SAM;
		i = 1;
		smart_print(type, i);
#endif
#if SAM2_ENABLE
		type = SAM;
		i = 2;
		smart_print(type, i);
#endif
#if SAM3_ENABLE
		type = SAM;
		i = 3;
		smart_print(type, i);
#endif
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.smart����\n"
							"9.��ӡ����");
		
		memcpy(g_RF_bak/*gen31_bak*/, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(ret)
		{
		case '1':
			smart_print(type, i);
			break;
		case '0':
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
			case SAM:
			case IC:
			//	conf_icsam_pps();
				if ((i=select_ICSAM())==NDK_ERR_QUIT)
					;//QUIT����ʾ
				else
					cls_show_msg1(2, "��ѡ��%s", CardNoStr[i]);
				break;
			default:
				break;
			}
			break;
		case '9':
			conf_prn();
			break;			
		case ESC:
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

