/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ/���̽���
* file name			: systest50.c
* Author 			: 	chensj
* version			: 
* DATE				: 
* directory 			: 
* description			:  ��Ƶ/���̽���
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130822  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��Ƶ/���̽���"

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
					 chensj 			2013/10/08     created
*****************************************************************/
void rfid_input(SMART_t type)
{
#include "kbtable.h"
	int nCode = 0, pUIDlen = 0, i = 0;
	uchar SAK[LEN_SAK] = {0}, UID[32] = {0};
#if ANDIROD_ENABLE
	uint cardcount = 0, maxcards = 20;
	iso15693ProximityCard_t cards[20];
#endif
	NDK_KbFlush();
	while(1)
	{
		cls_printf("������[%s]",table[i].keyname);
		if(NDK_KbHit(&nCode)!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(0x%2x)", __LINE__, nCode);
			return;
		}
		else
		{	
			if(nCode!=0)
			{
				if(nCode!=table[i].keycode)//���������Ԥ�ڲ�һ���򱨴�
				{
					cls_show_msg("line %d,%s����ʧ��,Ԥ��(0x%2x)ʵ��(0x%2x)", __LINE__, TESTITEM, table[i].keycode, nCode);
					return;
				}
				else
				{
					i++;//���������Ԥ��һ���������һ������
					NDK_KbFlush();
				}
			}
			if(i == SZ_ARRAY(table))
			{
				if(cls_show_msg("%s�������,�Ƿ��о�������\n[ENTER]��,[����]��", TESTITEM)==ENTER)
					cls_show_msg("line %d,%s����ʧ��", __LINE__, TESTITEM);
				else
					cls_show_msg("%s����ͨ��", TESTITEM);
				return;
			}
		}
			
		switch(type)
		{
		case CPU_A:
		case CPU_B:
			NDK_RfidPiccDetect(NULL);
			continue;
			break;
		case MIFARE_1:
			NDK_M1Request(REQA, &pUIDlen, UID);
			NDK_M1Anti(&pUIDlen, UID);
			NDK_M1Select(pUIDlen, UID, SAK);
			continue;
			break;
		case FELICA:
			NDK_RfidFelicaPoll(UID, &pUIDlen);
			continue;
			break;
#if M0_ENABLE 
		case MIFARE_0:			
			NDK_MifareActive(REQA, UID, pUIDlen,SAK);	
			continue;
			break;
#endif
#if ISO15693_ENABLE
		case ISO15693:
			NDK_Iso15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards);
			continue;
			break;
#endif
		default:
			cls_show_msg("δ֪���͵Ŀ���");
			break;
		}	
	}
	return;
}

void systest50(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	
	cls_show_msg1(1, "%s������...", TESTITEM);

	while (1)
	{	
		ret = cls_show_msg("1.��Ƶ���̽���\n"
							"0.��Ƶ������");
		switch(ret)
		{
		case '1':
			rfid_input(type);
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
			break;
		default:
			continue;
			break;
		}
	}
}

