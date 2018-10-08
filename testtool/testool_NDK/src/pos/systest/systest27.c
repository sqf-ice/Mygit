/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest27.c
* Author 			: chensl
* version			: 
* DATE			: 20130124
* directory 	: 
* description	: SMART/LAN�������
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"SMART/LAN����"

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
* history 		 		: author			date			remarks
*			  		chensl		   20130124	 		created
*****************************************************************/
static void smart_LAN(SMART_t type, int i)
{
	/*private & local definition*/
#include "icsam.h"

	int ret = 0, ret1 = 0, timeout = SO_TIMEO, slen = 0, rlen = 0, icount = 0, isucc = 0, uidlen = 0;
	uint h_tcp = 0; 
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0}, *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"}, msg[32] = {0};
	uchar UID[LEN_UID] = {0};
	
	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	cls_show_msg1(5, "��ȷ���Ѱ�װ%s��,���������...", (type==SAM||type==IC)?CardNoStr[i]:card[type]);
	if(type==SAM||type==IC)
	{	
		sprintf(msg, "%s/LAN�������", CardNoStr[i]);
		UID[0] = CardNo[i];
	}
	else
		sprintf(msg, "%s/LAN�������", card[type]);
	
	//NetUp
	if (NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:NetUpʧ��", __LINE__);
		return;
	}	
	while(1)
	{
		TransDown(h_tcp);//����
		smart_deactive(type, CardNo[i], 10);
		
		if(ESC==cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", icount+1, msg, isucc))
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		icount++;

		//TransUp��smartѰ��
		if ((ret=TransUp(&h_tcp))!=SUCC||(ret1=smart_detect(type, UID, &uidlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d,%d)", __LINE__, icount, msg, ret, ret1);
			continue;
		}
		
		//LAN�������ݺ�smart����
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len||(ret1=smart_active(type, UID, &uidlen))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ʵ��%d, Ԥ��%d)(%d)", __LINE__, icount, msg, slen, sendpacket.len, ret1);
			continue;
		}
		
		//LAN�������ݺ�smart��д
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len||(ret1=smart_APDUrw(type, CardNo[i], UID))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ʵ��%d, Ԥ��%d)(%d)", __LINE__, icount, msg, rlen, sendpacket.len, ret1);
			continue;
		}
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, icount);
			continue;
		}
		
		//TransDown��smart�µ�
		if ((ret=TransDown(h_tcp))!=SUCC||(ret1=smart_deactive(type, CardNo[i], 10))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(%d,%d)", __LINE__, icount, msg, ret, ret1);
			continue;
		}		
		
		isucc++;
	}
	
	NetDown();
	smart_deactive(type, CardNo[i], 0);	
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, icount, isucc);
	return;
}


void systest27(void)
{
	/*private & local definition*/
//#include "icsam.h"
	int ret = 0, i = 0, tmpflag = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	char *CardNoStr[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=0)
		SetWifiStopflag(0);//ȷ����������ά��wifi���ӵ�����²���
	
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_LAN()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����LAN����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		
		g_CycleTime = 10;
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
				SetWifiStopflag(tmpflag);
				return;
			}
			//���������ļ��������Ƶ�����ͽ�����Ƶ����LAN�Ľ���
			smart_LAN(type, i);
			break;
		case SAM:
		case IC:
			//type = CPU_A;
			break;
		default:
			break;
		}
		
		//��˳����������IC/SAM1/SAM2/SAM3��LAN�Ľ���
#if IC1_ENABLE
		type = IC;
		i = 0;
		smart_LAN(type, i);
#endif
#if SAM1_ENABLE
		type = SAM;
		i = 1;
		smart_LAN(type, i);
#endif
#if SAM2_ENABLE
		type = SAM;
		i = 2;
		smart_LAN(type, i);
#endif
#if SAM3_ENABLE
		type = SAM;
		i = 3;
		smart_LAN(type, i);
#endif
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		SetWifiStopflag(tmpflag);
		return;
	}
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.LAN����\n"
							"9.smart����");		
		memcpy(g_RF_bak, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(ret)
		{
		case '1':
			smart_LAN(type, i);
			break;
		case '0':
			switch(conf_conn_LAN())
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				break;
			case NDK_ERR_QUIT://�û�����
			default:
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
		case ESC:
			cls_show_msg("��ѡ��������Ƭ���²���!");
			SetWifiStopflag(tmpflag);
			return;
			break;
		default:
			continue;
			break;
		}
	}

	SetWifiStopflag(tmpflag);
	return;
}

