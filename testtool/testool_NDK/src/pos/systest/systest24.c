/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ſ�/SMART����
* file name		: systest24.c
* Author 			: chenfm
* version			: V1.0
* DATE			: 20130120
* directory 		:
* description		: �ſ�/SMART����
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
#define	TESTITEM	"�ſ�/SMART����"

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
*					chenfm			20130120			created
*****************************************************************/
static void mag_smart(SMART_t type, int i)
{
	/*private & local definition*/
#include "icsam.h"
	
	int ret = 0, ret1 = 0, succ = 0, cnt = 0, bak = 0,pUIDlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR, *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693" }, msg[32] = {0};
	uchar UID[LEN_UID] = {0};
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)		
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	cls_show_msg("��ȷ���Ѱ�װ%s��,���������...", (type==SAM||type==IC)?CardNoStr[i]:card[type]);
	if(type==SAM||type==IC)
	{	
		strcpy(msg, CardNoStr[i]);//sprintf(msg, "%s", CardNoStr[i]);
		UID[0] = CardNo[i];
	}
	else
		strcpy(msg, card[type]);
	
	while(cnt)
	{
		//smart_powerdown(type, CardNo[i], 10);//����
		smart_deactive(type, CardNo[i], 10);
		if(ESC==cls_show_msg1(2, "�ſ�/%s������...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;
		
		//ˢ�ſ���smart����
		if ((ret=systest_MagCard())!=STRIPE||(ret1=smart_detect(type,UID,&pUIDlen))!=SUCC)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d�δſ�/%s����ʧ��(0x%x,%d)", __LINE__, bak-cnt, msg, ret, ret1);
			continue;
		}
		
		//ˢ�ſ���smart��״̬
		if ((ret=systest_MagCard())!=STRIPE||(ret1=smart_active(type, UID,&pUIDlen))!=SUCC)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d�δſ�/%s����ʧ��(0x%x,%d)", __LINE__, bak-cnt, msg, ret, ret1);
			continue;
		}
		
		while(cnt)
		{
			if(ESC==cls_show_msg1(2, "�ſ�/%s������...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
				break;
			cnt--;
			
			//ˢ�ſ���smart��д
			if((ret=systest_MagCard())!=STRIPE||(ret1=smart_APDUrw(type, CardNo[i], UID))!=SUCC)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�δſ�/%s����ʧ��(%d,%d)", __LINE__, bak-cnt, msg, ret, ret1);
				break;
			}
			succ++;
		}
	}
	
	//smart�µ�
	if ((ret=smart_deactive(type, CardNo[i], 0))!=SUCC)//�������,�������ùر�
	{	
		cls_show_msg1(g_keeptime, "line %d:%s�µ�ʧ��(0x%x)", __LINE__, msg, ret);
		//��ʱsmart�µ�ʧ��,��������ѹ�����Գɹ���return;
	}
	
	cls_show_msg("�ſ�/%s�������,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

void systest24(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}
	/*private & local definition*/
#include "icsam.h"

	int ret = 0, i = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693" };
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.smart����");
		
		memcpy(g_RF_bak/*gen31_bak*/, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(ret)
		{
		case '1':
			mag_smart(type, i);
			break;
		case '0':
			switch(type=select_smarttype())
			{
#if !defined ME15CHX//ME15CΪ��ʡ����ռ������������ 20170213 linwl
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
#endif
			case SAM:
			case IC:
				//conf_icsam_pps();
				if ((i=select_ICSAM())==NDK_ERR_QUIT)
					;//QUIT����ʾ
				else
					cls_show_msg1(2, "��ѡ��%s",  CardNoStr[i]);
				break;
			default:
				break;
			}
			break;
		case ESC:
			smart_deactive(type, CardNo[i], 0);
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

