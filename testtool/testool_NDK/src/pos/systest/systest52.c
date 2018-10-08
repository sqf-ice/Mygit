/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	��Ƶ/��������
* file name			: systest52.c
* Author 			: 	chensj
* version			: 
* DATE				: 
* directory 			: 
* description			: ��Ƶ/�����������
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
#define	TESTITEM	"��Ƶ/��������"
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
					 chensj 		2013/12/24            created
*****************************************************************/
void rfid_touch(SMART_t type)
{
	int ret = 0, pUIDlen = 0, bak = 0,cnt = 0, succ = 0, rcvlen = 0;
	uint unLen = 0;
	uchar UID[LEN_UID] = {0};
	char  CntStr[8] = DEFAULT_CNT_STR;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};

	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	cls_show_msg("���ڸ�Ӧ������%s��,���������...", card[type]);
	while(1)
	{
		RFID_deactive(type, 0);
		if(bak++==cnt)
			break;
		if(ESC==cls_show_msg1(2, "���ڽ���%d��%s����,�ɹ�%d��,ESC�˳�", bak, TESTITEM, succ))
			break;

		//��������
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}
		
		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;		
		}	

		//��������
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}
		
		//����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{	
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}

		//��������
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}
		
		//��д
		if ((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
		{	
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}

		//��������
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}
		
		//�رճ�
		if ((ret=RFID_deactive(type, 0))!=SUCC)
		{	
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}

		//��������
		if(systest_touch()!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			goto END;
		}
		succ++;
	}
	cls_show_msg("%s�������,�ܹ�����%d�γɹ�%d��\n", TESTITEM, bak-1, succ);
	return;
END:
	RFID_deactive(type, 0);
	return;
}

void systest52(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0,ret = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);

	while(1)
       {
		keyin = cls_show_msg("1.����/��Ƶ\n"
							"0.��Ƶ������");
		switch(keyin) 
		{
		case '1':
			rfid_touch(type);
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
			default:
				cls_show_msg("line %d:���ô���!������RFID(%d).", __LINE__, type);
				break;
			}
			break;	
		case ESC:
			return;
		default:
			continue;
			break;
		}
	}
	
	//test over
	return;
}
