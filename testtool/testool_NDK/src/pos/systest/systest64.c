/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest64.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: SMART/BT�������
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
#define	TESTITEM	"SMART/BT����"

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
*			  		
*****************************************************************/
static void smart_BT(SMART_t type, int j)
{
	/*private & local definition*/
#include "icsam.h"

	int ret = 0, ret1 = 0, i = 0, succ = 0, uidlen = 0, cmdlen = 0, recvLen = 0, datalen = 0; 
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"}, msg[32] = {0};
	char sendBuf[PACKMAXLEN] = {0}, recvBuf[PACKMAXLEN] = {0};
	char cmdBuf[BUFSIZE_BT] = {0};//, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	uchar UID[LEN_UID] = {0};
#if !(defined ME15CHX||defined ME15B) 
	Packet sendpacket;
#else
	uint len = 0;
	char tbuf[16] = {0};
	int cnt = 0;
#endif

	/*process body*/
#if !(defined ME15CHX||defined ME15B) 	
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
#else
	cls_printf("���ý������:\n");
	strcpy(tbuf, "10");
	if ((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len, INPUTDISP_OTHER, 10, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
		strcpy(tbuf, "10");
	cnt=atoi(tbuf);
	
	cls_printf("���ó�ʼ����:\n");
	strcpy(tbuf, "1000");
	if ((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len,INPUTDISP_OTHER, 10, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
		strcpy(tbuf, "1000");		
	datalen = atoi(tbuf);
	if(datalen>BUFSIZE_BT-MIN_BTCMD_LEN)
		datalen = BUFSIZE_BT-MIN_BTCMD_LEN;

	//ME15CHX�ڴ�ֻ��4K,����ʹ��sendpacket��غ���,ֱ��ʹ��sendBuf,�ҹ̶�����BUFSIZE_BT����������շ�
	for (i=0; i<sizeof(sendBuf); i++)
		sendBuf[i]=rand()%256;
#endif
	cls_show_msg1(g_keeptime, "��ȷ���Ѱ�װ%s��,���������...", (type==SAM||type==IC)?CardNoStr[j]:card[type]);
	if(type==SAM||type==IC)
	{	
		sprintf(msg, "%s/BT�������", CardNoStr[j]);
		UID[0] = CardNo[j];
	}
	else
		sprintf(msg, "%s/BT�������", card[type]);

	i=0;
	while(1)
	{
		//��������
		smart_deactive(type, CardNo[j], 10);
		//NDK_PortClrBuf(PORT_NUM_BT);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", ++i, msg, succ)==ESC)
			break;
#if !(defined ME15CHX||defined ME15B) 
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		datalen = sendpacket.len;
#else
		if(i > cnt)
			break;
#endif
		cmdlen = datalen+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���

		//����������
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		if(g_btdiscoverflag)
		{
			if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
				continue;
			}
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
#if BTMFI_ENABLE
		cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
		//smartѰ��
		if ((ret1=smart_detect(type, UID, &uidlen))!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, i, msg, ret1);
			continue;
		}
		
		//�����建���� //Ϊ���Եײ������Ϳ��ƴſ���IC����оƬ���ڻ�������ͻ������,����Ӧ��ʵ��ʹ�÷�ʽ�������建����� 20160912 linwl
		/*if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			continue;
		}*/
		
		//smart����
		if((ret1=smart_active(type, UID, &uidlen))!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, i, msg, ret1);
			continue;
		}
		
		//������������
		memset(cmdBuf, 0, sizeof(cmdBuf));
#if defined ME15CHX||defined ME15B 		
		bt_cmd_packet(0x05, sendBuf, datalen, cmdBuf);
#else
		bt_cmd_packet(0x05, sendpacket.header, datalen, cmdBuf);
#endif		
		//cmdlen = sendpacket.len+MIN_BTCMD_LEN;
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
		sleep(1);
		
		//smart��д
		if ((ret1=smart_APDUrw(type, CardNo[j], UID))!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, i, msg, ret1);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
		
		//������������
		memset(cmdBuf, 0, sizeof(cmdBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			//bt_comm_once(12,0);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
		bt_cmd_uppacket(cmdBuf, datalen, recvBuf);
		if(memcmp(sendBuf, recvBuf, datalen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������У��ʧ��", __LINE__, i);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
#if !BTMFI_ENABLE//MFI������ԭ���ͻصĹ��ߣ��������03��06����Ӱ���´β��ԣ������跢�ͣ�zhengry 20170104
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
#endif
		//smart�µ�
		if ((ret1=smart_deactive(type, CardNo[j], 10))!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, i, msg,ret1);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}	
		
		//�Ͽ���������(�Ͷ˲�Ʒ��Ҫ���ô˺����Ͽ�����,�ж˲�Ʒ���ϴ˵���Ҳ��Ӱ��)
#if !BTMFI_ENABLE//MFI������ԭ���ͻصĹ��ߣ��������03��06����Ӱ���´β��ԣ������跢�ͣ�zhengry 20170104
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x03, NULL, 0, cmdBuf);
		NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdBuf);
#endif
		packetnum = 0;//���ӶϿ������к�Ҫ��0
		cls_show_msg1(1, "POS�˹Ҷ�����������...");
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��", __LINE__, i);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}		
		
		//�ر���������
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, i);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
		succ++;
	}
	
	smart_deactive(type, CardNo[j], 0);	
	cls_show_msg_record(FILENAME,FUNCNAME,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��,ע��:��������֧��NLBluetoothTest����", msg, i-1, succ);
	return;
}


void systest64(void)
{
	/*private & local definition*/	
//#include "icsam.h"
	int ret = 0, i = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	char *CardNoStr[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
#if !defined ME15CHX//ME15CΪ��ʡ����ռ������������ 20170213 linwl
	if(auto_flag==1)//֧���Զ�����
	{
		//�Զ�����������
		//BT���ò�����ʧ�����˳�
		if(conf_conn_BT()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,BT����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		
		//ִ�н������
		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);

		//SMART����
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
			smart_BT(type, i);
			break;
		case SAM:
		case IC:
			//type = CPU_A;
			break;
		default:
			break;
		}
		
		//��˳����������IC/SAM1/SAM2/SAM3��BT�Ľ���
#if IC1_ENABLE
		type = IC;
		i = 0;
		smart_BT(type, i);
#endif
#if SAM1_ENABLE
		type = SAM;
		i = 1;
		smart_BT(type, i);
#endif
#if SAM2_ENABLE
		type = SAM;
		i = 2;
		smart_BT(type, i);
#endif
#if SAM3_ENABLE
		type = SAM;
		i = 3;
		smart_BT(type, i);
#endif
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
#endif

	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.BT����\n"
							"9.smart����");		
		memcpy(g_RF_bak, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(ret)
		{
		case '1':
			smart_BT(type, i);
			break;
		case '0':
			conf_conn_BT();
#if BTMFI_ENABLE
			NDK_BTSetBleStatus(0);//��MFIЭ���£��ر�BLE��ʽ
#endif
			break;
		case '9':
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
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

