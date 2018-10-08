/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: SMART/SD����
* file name		: 
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/06/18
* directory 		: 
* description		: (SD/U��)/SMART����
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130618
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"(SD/U��)/SMART����"

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
*					chensj			2013/06/18		creat
*****************************************************************/
static void sdcard_rfcard(SMART_t type, int disktype,int i)
{
	/*private & local definition*/
#include "icsam.h"
	int ret = 0, succ = 0, cnt = 0, bak = 0,UIDlen=0;
	char CntStr[8] = DEFAULT_CNT_STR, *card[] = {"CPU_A", "CPU_B"/*, "HS_B"*/, "M-1"/*, "RFID-SIM"*/, "FELICA", "M-0", "15693"}, msg[32]={0};
	uchar UID[LEN_UID] = {0};	
	uint unLen = 0;
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		bak  = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	cls_show_msg1(g_keeptime, "��ȷ���Ѱ�װ%s��,���������...", (type==SAM||type==IC)?CardNoStr[i]:card[type]);
	if (type==SAM||type==IC)
	{
		sprintf(msg, "%s/%s����", CardNoStr[i], (disktype==UDISK)?"U��":"SD");
		UID[0] = CardNo[i];
	}
	else
		sprintf(msg, "%s/%s����", card[type], (disktype==UDISK)?"U��":"SD");
	
	while(cnt)
	{
		NDK_DiskClose(disktype);
		smart_deactive(type, CardNo[i], 0);
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;
		cnt--;
		
		//RFѰ��
		
		if ((ret=smart_detect(type, UID,&UIDlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		//RF����
		if ((ret=smart_active(type, UID, &UIDlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		//RF��д
		if((ret=smart_APDUrw(type, CardNo[i], UID))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		//RF�µ�
		if ((ret=smart_deactive(type, CardNo[i], 0))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		//SD��
		if((ret=systest_SDCard(disktype))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

void systest41(void)
{
	/*private & local definition*/
//#include "icsam.h"
	int ret = 0, disktype = SDDISK,i=0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B"/*, "HS_B"*/, "M-1"/*, "RFID-SIM"*/, "FELICA", "M-0", "15693"};
	char *CardNoStr[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_disktype()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,SD/U������ʧ��,���������ļ�",TESTITEM);
			return;
		}
		disktype=get_disktype();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//smart����
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
			//���������ļ��������Ƶ�����ͽ�����Ƶ����SD/U�̵Ľ���
			sdcard_rfcard(type, disktype,i);
			break;
		case SAM:
		case IC:
			//type = CPU_A;
			break;
		default:
			break;
		}

		//��˳����������IC/SAM1/SAM2/SAM3��SD/U�̵Ľ���
#if IC1_ENABLE
		type = IC;
		i = 0;
		sdcard_rfcard(type, disktype,i);
#endif
#if SAM1_ENABLE
		type = SAM;
		i = 1;
		sdcard_rfcard(type, disktype,i);
#endif
#if SAM2_ENABLE
		type = SAM;
		i = 2;
		sdcard_rfcard(type, disktype,i);
#endif
#if SAM3_ENABLE
		type = SAM;
		i = 3;
		sdcard_rfcard(type, disktype,i);
#endif
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}

	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.SD/U������\n"
							"9.smart����\n");
		
		switch(ret)
		{
		case '1':
			sdcard_rfcard(type, disktype,i);
			break;
		case '0':
			if((ret=conf_disktype())!=SUCC)
			{
				cls_show_msg1(2, "�豸ѡ��ʧ��,������ѡ��", ret);
				return;
			}
			disktype=get_disktype();
			break;
		case '9':
			switch(type=select_smarttype())
			{
			case CPU_A:
			case CPU_B:
		//case HS_B:
			case MIFARE_1:
			case FELICA:
			case MIFARE_0:	
		//case RFID_SIM:
			case ISO15693:
				if ((ret=RFID_init(type))!=SUCC)
					cls_show_msg("line %d:��ʼ��ʧ��(%d)!���������Ƿ���ȷ.", __LINE__, ret);
				else
					cls_show_msg1(2, "%s��ʼ���ɹ�!", card[type]);
				break;
			case SAM:
			case IC:
				if ((i=select_ICSAM())==NDK_ERR_QUIT)
					;//QUIT����ʾ
				else
					cls_show_msg1(2, "��ѡ��%s", CardNoStr[i]);
				break;
			default:
				cls_show_msg("line %d:���ô���!������smart(%d).", __LINE__, type);
				break;
			}
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


