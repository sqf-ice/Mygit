/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ۺϲ���
* file name		: 
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20060801
* directory 		: .\SRC\general\
* description		: �ۺϲ��������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: TCR690 API���Գ��� ��ϸ���˵����.doc
*				  TCR690&8200����Ա�ο��ֲᣨv1.05T)_2.doc
*				  TCR8280_������԰���(��˰)_V2.2.doc
*
************************************************************************
* log			: 
* Revision 1.1  20060921 11:20 zhangnw
* ��д�ۺϲ��Կ��
* Revision 1.0  20060801 14:10 zhangnw
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	ITEMPERPAGE	(4)
#define	MAXTIMEOUT	(60)
#define	MAXWAITTIME	(180)	//30->180 for ���

#if PRN_ENABLE
#define	PRNIMGWIDTH	MAX(MAX(MAXPRLINEWIDE_TP, MAXPRLINEWIDE_HIP), MAXPRLINEWIDE_FIP)	//(384)
#define	PRNIMGHEIGHT	(8)	//(8*8)
#define	PRNIMGBUFLEN	(PRNIMGWIDTH*PRNIMGHEIGHT/8)
#endif

#define	DEFAULT_KEEPTIME_STR	"0" //"2"
#define	DEFAULT_KEEPTIME_VLE	atoi(DEFAULT_KEEPTIME_STR)

//#define	DEBUG_CMD_ALL		(-1)		//for NL829
//��������CMD�Ͷ���DEBUG_CMDΪ����(����������뷵������);����ʹ��ʱ,�رմ˺�
//#define	DEBUG_CMD			NL829_CPU_ACTIVE	//NL829_APDURW
//#define	DEBUG_CMD			DEBUG_CMD_ALL	//ȫ���������//��ģʽ��NL829�������ݲ�֧��

/*------------global variables definition-----------------------*/
int g_keeptime = 0;// 2;//DEFAULT_KEEPTIME_VLE;//�ñ�����Ҫ��ѹ�����Դ�����ʾ�ȴ�ʱ��,��ѹ�����Ի�������Ҫȷ�ϵ�(����ʱ)
uchar g_RF_bak[/*MAXLEN_DATA*/LEN_BLKDATA+8] = DATA16;
#if ISO15693_ENABLE
iso15693ProximityCard_t cards[20];
#endif
/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
int g_CycleTime = 0;//ͨѶѹ���ͽ������
int g_PackLen = 0;//���ݰ�����
/* ���ڻ�ȡ���õĳ�ֵ: ����Դ���*/
int GetCycleValue(void)
{
	return g_CycleTime;
}

int GetCommPackLen(void)
{
	return g_PackLen;
}

static int IsAPartOfB(uchar *A, int Acnt, uchar *B, int Bcnt)
{
	/*private & local definition*/
 	int i = 0, j = 0;

 	/*process body*/
 	for(i=0; i<Acnt; i++)
 	{
		for(j=0; j<Bcnt&&A[i]!=B[j]; j++);
		if(j==Bcnt)
			return FALSE;
 	}
	
	return TRUE;
}

static int GetIDInCharArray(uchar value, uchar *array, int size)
{
	/*private & local definition*/
 	int i = 0;

 	/*process body*/
 	for(; i<size&&value!=array[i]; i++);

	if(i==size)
		return -1;
	else
	{
		sprintf(NowSystestName,"systest%d.c",array[i]);
	}
 	return i;
}

#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15C��֧�ļ�ϵͳΪ��ʡ����ռ������������ 20170213 linwl
//#if !K21_ENABLE
static void resultlog_conf(void)
{
	int nKeyin = 0,ret = 0,fp = 0,rdlen = 0;
	char result[1024]={0};
	
	while(1)
	{
		nKeyin = cls_show_msg("���Խ������\n"
							"1.���log�ļ�\n"
							"2.����log\n");
		switch(nKeyin)
		{
			case '1':
				NDK_FsDel(LOGFILE);
				return;
				break;
			case '2':
			//	if (g_com != INVALID_COM)
			//		NDK_PortClose(g_com);

			//	if(NDK_PortOpen(g_com, "115200") != NDK_OK )//PORT_NUM_COM1
			//	{
			//		cls_show_msg1(3,"line %d:�򿪴���1ʧ��", __LINE__);
			//		return;
			//	}
				if(cls_show_msg("��ȷ������������ڴ���ѡ�����ʹ���,���ް�ESC�˳�ѡ��,���а�����������")==ESC)
					return;
				if(g_com==PORT_NUM_USB)
					cls_show_msg("��PC����AccessPort�Ĵ��ں����������");
				if ((fp=NDK_FsOpen(LOGFILE, "r"))< NDK_OK)
				{
					cls_show_msg1(3,"line %d:��%sʧ��(%d)", __LINE__, LOGFILE,fp);
					return;
				}
				while((rdlen=NDK_FsRead(fp, result,sizeof(result)))==sizeof(result))
				{
					if((ret=NDK_PortWrite(g_com, rdlen, result))!=NDK_OK)//PORT_NUM_COM1
					{
						cls_show_msg1(3,"line %d: ����ʧ��ret=%d", __LINE__, ret);
						NDK_FsClose(fp);
						return;
					}
				}
				if(rdlen>0)
				{
					if((ret=NDK_PortWrite(g_com,rdlen, result))!=NDK_OK)//PORT_NUM_COM1
						cls_show_msg1(3,"line %d: ����ʧ��ret=%d", __LINE__, ret);	
				}
				NDK_FsClose(fp);
			//	NDK_PortClose(g_com);//�رմ���PORT_NUM_COM1

			//	if (g_com != INVALID_COM)
			//	{
			//		if ((ret=NDK_PortOpen(g_com, "115200,8,N,1")) != NDK_OK)
			//		{
			//			//��ʼ��ʧ����,Ҳ����send_result("line %d: ��ʼ�����ڴ�", __LINE__);
			//			cls_show_msg1(3,"line %d: ��ʼ�����ڴ�(%d)", __LINE__,ret);
			//			return;
			//		}
			//	}
				cls_show_msg1(2,"log���ͳɹ�");//���Ӹ���ʾ��Ϣ
				return;
				break;
			case ESC:
				return;
				break;
			default:
				break;
		}
	}
	return;
}
//#else
//static void resultlog_conf(void){cls_show_msg("����Ʒ��֧��д���Լ�¼����");return;}
//#endif

/*<
  * ���һЩҹ����е�ѹ������,ͬһ��ģ�������ѹ������,�������ߵ�
  * 		1.ͨѶѹ����2.����ѹ����3.ͨѶ����ѹ����4.��λ����ѹ��  ����һ����
*/
static void PressAndTime_conf(void)
{
	int nKeyin = 0,nRet  = 0;
	char CntStr[16]={"100"},PackageLen[16]={"10000"}, wait[4] = {0};
	uint unLen = 0, len = 0;

	//����g_keeptime ����ʱ��
	if(ENTER!=cls_show_msg1(MAXWAITTIME, "������Ϣ�Ƿ�һֱ����?\n"                                                   
												"[ȷ��]��[����]��\n"))
	{
		cls_printf("������Ϣ����ʱ��(s):");
		sprintf(wait, "%d", g_keeptime);
		if ((nRet=lib_kbgetinput(wait, 0, sizeof(wait)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)
			g_keeptime = DEFAULT_KEEPTIME_VLE;		
		else
			g_keeptime = atoi(wait);
	}
	else
		g_keeptime = 0;

	//������������ѹ����ز���
	while(1)
	{
		nKeyin = cls_show_msg("�Ƿ�����ѹ��\n"
							"1.��\n"
							"2.��\n");
		switch(nKeyin)
		{
			case '1':
				g_SequencePressFlag = 1;
								
				cls_printf("ȫ������ѹ�����Դ���:");
				if ((nRet=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)
					g_CycleTime = 100;
				else
					g_CycleTime = atoi(CntStr);

				cls_printf("ȫ������ѹ��ͨѶ����:");
				if ((nRet=lib_kbgetinput(PackageLen, 2, sizeof(PackageLen)-1, &unLen, INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)
					g_PackLen = 10000;
				else
					g_PackLen = atoi(PackageLen);
				return;
				break;
				
			case '2':
				g_SequencePressFlag = 0;
				return;
				break;
			default:
				g_SequencePressFlag = 0;
				return;
				break;
		}
	}
	return;
}

#if NEW_WIFI_ENABLE
static void conf_wifi_stop(void)
{
	/*private & local definition*/
 	int nKeyin = 0;

 	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("�Ƿ񱣳�wifi����\n"
							"1.��\n"
							"2.��\n");
		switch(nKeyin)
		{
			case '1':
				SetWifiStopflag(1);
				return;
				break;
			case '2':
				SetWifiStopflag(0);
				return;
				break;
			default:
				SetWifiStopflag(1);
				return;
				break;
		}
	}
	return;
}
#else
static void conf_wifi_stop(void){cls_show_msg("��֧����WIFI����");return;}
#endif

static void g_conf(void)
{
	/*private & local definition*/
 	int nKeyin = 0;

 	/*process body*/
 	do
	{
		nKeyin = cls_show_msg("1.��ӡ  5.wifi\n"
							"2.����ѹ����ʱ��\n"
							"3.���log����\n"
							"4.ѡ��NFC���ݸ�ʽ\n"
							);
#define MAXMENUITEM ('5')

		switch (nKeyin)
		{
		case '1':
#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)	
			conf_prn();
#endif
			break;
		case '2':
			PressAndTime_conf();
			break;
		case '3':
			resultlog_conf();
			break;
		case '4':
			conf_nfc_datatype();
			break;
		case MAXMENUITEM:
			conf_wifi_stop();
			break;
		case ESC:
			return;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
 } 

#if (ETH_ENABLE||WLAN_ENABLE||NEW_WIFI_ENABLE)
int conf_conn_LAN(void)
{
 	/*private & local definition*/
	int nKeyin = 0, lantype = -1;
 	char pInfo[64] = {0};

	/*process body*/
	do
	{
		if(g_SequencePressFlag)
		{
			if(GetParaInfo("LANType",pInfo)!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%s)", __LINE__, pInfo);
				return NDK_ERR_QUIT;//�����ļ����ô���ʱĬ���û�ȡ��
			}
			if(trim_string2int(pInfo,&lantype)!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%d)",__LINE__, lantype);
				return NDK_ERR_QUIT;
			}
			if(lantype==1)
				nKeyin='1';
			else if(lantype==2)
				nKeyin='2';
			else
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%d)",__LINE__, lantype);
				return NDK_ERR_QUIT;
			}
		}
 		else
		nKeyin = cls_show_msg("ѡ�����ӷ�ʽ:\n"
								"1.Eth 2.WLAN\n");
#define MAXMENUITEM ('2')

		switch (nKeyin)
		{
		//case 0://0Ϊ��ʱ,Ĭ������GPRS
		case '1':
#if ETH_ENABLE
			return conf_conn_ETH(TRUE);
			break;
#else
			cls_show_msg1(2,"����Ʒ��֧��ETH,��ѡ���������ӷ�ʽ");
			continue;
#endif
		case MAXMENUITEM://'2':
#if WLAN_ENABLE
			return conf_conn_WLAN();
			break;
#else
			cls_show_msg1(2,"����Ʒ��֧��WIFI,��ѡ���������ӷ�ʽ");
			continue;
#endif
		case ESC:
			return NDK_ERR_QUIT;//QUIT;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));

	return SUCC;//never reach
#undef MAXMENUITEM	
}

void create_recv_only_packet(Packet *packet, char *buf)
{
	/*private & local definition*/

	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len=PACKMAXLEN;// MIN(PACKMAXLEN,4096);//PACKMAXLEN;//��̨�������ֻ��5000 -->��̨������ 20180912
	packet->lifecycle = 1000;//PACKETLIFE;//1000;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

void send_recv_press(uint h_tcp)
{
	/*private & local definition*/
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	int i = 0, comm_succ_count = 0, timeout = SO_TIMEO;
	int slen = 0, rlen = 0;
	Packet sendpacket;

	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	while (1)
	{
		//������ͨѶ�����ж�
		cls_printf("��ʼ��%d��ͨѶ����(�ѳɹ�%d��)", i+1, comm_succ_count);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
	
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len)==ESC)
				break;
			else
				continue;
		}
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len)==ESC)
				break;
			else
				continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��ʧ��", __LINE__)==ESC)
				break;
			else
				continue;
		}
		
		comm_succ_count++;//ͨѶ�ɹ���������
	}		

	//���Ժ���
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d:�ɹ�����%d", i, comm_succ_count);
	return;
}

void recv_only_press(uint h_tcp)
{
	/*private & local definition*/
	int timeout = SO_TIMEO, rec_succ = 0, i = 0, slen = 0, rlen = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*process body*/
	create_recv_only_packet(&sendpacket, buf);
	update_snd_packet(&sendpacket);
	sendpacket.IsDataRnd = FALSE;//�������ѳ�ʼ����,͵͵�ı�Ϊ�����
	
	//��������
	if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
		return;
	}
	
	while (1)
	{
		//������ͨѶ�����ж�
		cls_printf("��ʼ��%d��ͨѶ����(�ѳɹ�%d��)", i+1, rec_succ);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
			
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len)==ESC)
				break;
			else
				continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��ʧ��", __LINE__)==ESC)
				break;
			else
				continue;
		}		
		
		rec_succ++;
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d,�ɹ�����%d", i, rec_succ);
	return;
}

void send_only_press(uint h_tcp)
{
	/*private & local definition*/
	int timeout = SO_TIMEO, i = 0;
	int send_succ = 0, slen = 0;
	char buf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	while (i<200)//while(1) �����������˳�Ӱ��Ч�� ���޸�ֻ����200�� linwl20131115
	{
		//������ͨѶ�����ж�
		cls_printf("��ʼ��%d��ͨѶ����(�ѳɹ�%d��)", i+1, send_succ);
		i++;

		//��������//�����ȷ���������������ͳ�ȥ��?��̨У��?��̨���֪��POS?
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len)==ESC)
				break;
			else
				continue;
		}
		NDK_SysDelay(10);//����̨һЩ����ʱ��
		send_succ++;
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d,�ɹ�����%d", i, send_succ);
	return;
}
#else
int conf_conn_LAN(void){return FAIL;}
void create_recv_only_packet(Packet *packet, char *buf){}
void send_recv_press(uint h_tcp){}
void recv_only_press(uint h_tcp){}
void send_only_press(uint h_tcp){}
#endif
#endif
/****************************************************************
* function name 	 	: general
* functional description 	: �ۺϲ������������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20060801 	 	created
*					zhangnw		   20060921		modify
*****************************************************************/
void systest(PF* list, unsigned char id[])
{
	/*private & local definition*/
	int nKeyin = 0, nPageSel = 0;//��0��ʼ����(��һҳ��Ϊ0)
	int cnt = 0, menuid = 0, maxpage = 0, sn =0, sn1 = 0;
	unsigned char select_id[MAXID] = {0};
	
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15C��֧�ļ�ϵͳΪ��ʡ����ռ������������ 20170213 linwl
	int nRet  = 0;
	char CntStr[16]={"100"},PackageLen[16]={"10000"};
	uint unLen = 0;
#endif	
	//zhangnw20060921 add �Ժ��ۺϲ��Ե������Ӳ˵����ڴ�������
	//��������Կ����Զ�����,����û�������Ҫ,���ݲ�ʵ��
	//���˵��:
	//"���ܡ�ѹ��"��ע��ģ��������ٶ�,�ɹ��ʼ���һ��ǿ���µı���
	//"����" ��עģ�����Ƚ��漰��һ��ǿ���µĽ����Ƿ�����
	//�Ƿ���н��������Ҫ����"�����������"
	char* menustr[] =
	{
		"��ӡ����,ѹ��",	// 1
		"MDM����,ѹ��",		// 2
		"ETH����,ѹ��",		// 3
		"��������,ѹ��",	// 4
		"IC������,ѹ��",	// 5
		"�ſ�����,ѹ��",	// 6
		"��ȫ����,ѹ��",	// 7
		"fsys����,ѹ��",	// 8
		"��������,ѹ��",	// 9
		"RTC�ۺϲ���",		// 10
		"WLAN����,ѹ��",	// 11
		"CPU���ܲ���",		// 12
		"��Ƶ������",		// 13		
		"�ſ�/���߽���",	// 14
		"�ſ�/��ӡ����",	// 15
		"�ſ�/MDM����",		// 16
		"�ſ�/LAN����",		// 17
		"��ӡ/���߽���",	// 18
		"��ӡ/MDM����",		// 19
		"��ӡ/LAN����",		// 20
		"����/MDM����",		// 21
		"ETH/WLAN����",		// 22
		"SMART/���߽���",	// 23
		"�ſ�/SMART����",	// 24
		"ICSAM/RF����",		// 25
		"SMART/MDM����",	// 26
		"SMART/LAN����",	// 27
		"SMART/��ӡ����",	// 28
		"MDM/LAN����",		// 29
		"����/LAN����",		// 30
		"Ӳ����Ϣ����",  	 //31
		"USB�����ۺ�",		// 32
		"ɨ��ͷ/����ͷ�ۺϲ���", // 33 
		"MEMORY������",		// 34
		"NL-PP60-1����",	// 35
		"NL829����",		// 36
		"(��)NL829����",	// 37
		"��ӡ/���̽���",	// 38
		"Ӳ��ͳ�Ʒ���",		// 39	
		"SD/U���ۺϲ���",	// 40
		"(SD/U��)/SMART", 	// 41
		"(SD/U��)/��ӡ",	// 42
		"��ӡ/��������",	//43
		"WLAN/ETH���",	//44
		"WLAN/WLM���",	//45
		"WLAN/MDM���",	//46
		"������ѹ������",	//47
		"�ſ�/���̽���",	//48
		"�����濨����",		//49  ���ſ�ˢ����IC����д����Ƶ��Ѱ����д�������뿨��4�ַ�ʽ���н���
		"��Ƶ/���̽���",   	//50
		"LCD��Ļ����",		//51
		"����/��Ƶ����",	//52
		"����/��ӡ����",	//53
		"����/�ſ�����",	//54
		"����/��ص���",	//55
		"����/���߽���",	//56
		"�����ۺ�",         //57
		"����/ȡ��ص���",  //58
		"BT/MDM���",		//59
		"BT/�������",	//60
		"BT/ETH���",		//61
		"BT/WLAN���",	//62
		"�ſ�/BT����",		//63
		"SMART/BT����",		//64
		"��ӡ/BT����",		//65
		"���߳������",		//66
		"״̬���ۺϲ���",	//67
		"����ѹ������",		//68
		"USB��/3g���",	//69
		"USB��/3g���",	//70
		"U��/3g���",		//71
		"MODEM/NFC���",	//72
		"LAN/NFC���",	//73
		"����/NFC���",	//74
		"BT/NFC����",		//75
		"����/NFC����",		//76
		"NFC�ۺ�",			//77
		"NFC/RF����",		//78
		"��ӡ/NFC����",		//79
		"�ſ�/NFC����",		//80
		"ȡ����/NFC����",	//81
		"ICSAM/NFC����",	//82
		"WIFI AP�ۺ�",		//83
		"WIFI/AP����",		//84
		"������ģʽ�ۺ�",	//85
		"����ͷ/WLM���",  //86
		"����ͷ/USB�ӽ���",  //87
		"����ͷ/USB������",  //88
		"����ͷ/U�̽���",  //89
		"������ӡ���ۺ�",	//90
		"��Ƶ/���߽���",	//91
		"SP10_Tǩ����",		//92
		"����/����ģ�鲢��",	//93
		"SP100�������",		//94
		"DBUS���߳�",		//95
		"K21_fs����,ѹ��",		//96
		"HID�����ۺ�",		//97
	};
	//ע���������Ŷ�Ӧ!

	char* tmpstr = NULL;
#if SCR_DISABLE
	char tmp[64] = {0};
#endif

	/*process body*/
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15C��֧�ļ�ϵͳΪ��ʡ����ռ������������ 20170213 linwl
#if  STABAR_ENABLE//20140626 linwl added
	int ret = -1;
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_WLSIGNAL|STATUSBAR_DISP_BATTERY|STATUSBAR_DISP_DATE|STATUSBAR_DISP_TIME|STATUSBAR_DISP_ETH|STATUSBAR_DISP_MODEM|STATUSBAR_DISP_WIFI|STATUSBAR_DISP_BLUETOOTH))!=NDK_OK)
	//if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_ALL))!=NDK_OK)//all ����ָ�������е�״̬ͼ��,���Բ�����all
	{
		cls_show_msg("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif

	if(auto_flag==1)//autotest_flag
	{
		cls_show_msg1(1, "�Զ����ۺϲ�����...");
		//�Զ�����
		g_keeptime = 3;//����ȴ�ʱ�����ó�1��
		g_SequencePressFlag = 1;
								
		cls_printf("ȫ������ѹ�����Դ���:");
		if ((nRet=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)
			g_CycleTime = 100;
		else
			g_CycleTime = atoi(CntStr);

		cls_printf("ȫ������ѹ��ͨѶ����:");
		if ((nRet=lib_kbgetinput(PackageLen, 2, sizeof(PackageLen)-1, &unLen, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)
			g_PackLen = 10000;
		else
			g_PackLen = atoi(PackageLen);
		process(list, id);
		return;
	}
	else
#endif
	{
	//�ֶ�����
#if K21_ENABLE
	cls_show_msg1(2, "�ۺϲ���...\n" "7 ��:���ط�ҳ\n" "8 ��:����ҳ\n" "ESC-�˳�0-����");
#else
	cls_show_msg1(2, "�ۺϲ���...\n" "����:���ط�ҳ\n" "����:����ҳ\n" "ESC-�˳�0-����");
#endif
	cls_show_msg1(5, "ע��:ѹ������������ĳɱ�Ĭ��ֵ��С!������ܲⲻ������!");

	strcpy((char *)select_id, (char *)id);
	while (1)
	{
		maxpage = (strlen((char *)select_id)-1)/ITEMPERPAGE;//��0��ʼ����(��һҳ��Ϊ0)
			
		//�ۺϲ���ѡ�����
		NDK_ScrClrs();//clrscr();
		for (cnt=0; cnt<ITEMPERPAGE; cnt++)
		{
			menuid = select_id[cnt+ITEMPERPAGE*nPageSel]-1;
			if (menuid==-1)
				break;
			else if (menuid>SZ_ARRAY(menustr)-1)
				tmpstr = "δ����Ĳ���";
			else
				tmpstr = menustr[menuid];
			
#if SCR_DISABLE	//�ض��򵽴���
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp, "%d.%s\n", cnt+1, tmpstr);
#if  0//ANDIROD_ENABLE
			NDK_AZPortWrite(g_com, strlen(tmp), tmp);
#else
			NDK_PortWrite(g_com, strlen(tmp), tmp);
#endif
#endif
			NDK_ScrPrintf("%d.%s\n", cnt+1, tmpstr);//printf("%d.%s\n", cnt+1, tmpstr);
		}
		NDK_ScrRefresh();

		//������
		switch ((nKeyin=lib_getch()))
		{
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
		case '0'://ȫ������
			g_conf();
			break;
#endif
		case '1':
		case '2':
		case '3':
		case '4':
			//ȷ��nKeyin��ѡ���Ӧ��select_id�����sn
			if((sn=nKeyin-'0'+ITEMPERPAGE*nPageSel)>strlen((char *)select_id))
				break;
			else
			{
				//ȷ��select_id[sn]��id[]�е�λ��sn1
				sn1 = GetIDInCharArray(select_id[sn-1], id, strlen((char *)id));
				//����sn1������
				(*(list+sn1))();//(*(list+nKeyin-'1'+ITEMPERPAGE*nPageSel))();
			}
			break;
		case '9'://zhangnw20111215���ӿ���ѡ�����
			if (BTN_OK==ShowMessageBox("������ʾ����\n���п�ѡ������?", BTN_OK|BTN_CANCEL, -1))
			{
				nPageSel = 0;
				sn = select_test(select_id, MAXID);//ֻ��ѡ��һ��ԭ�������ϵ��Ӽ�
				if(!IsAPartOfB(select_id, sn, id, strlen((char *)id)))//У��select_id�Ƿ�OK
				{
					cls_show_msg("��ѡ�����Ŵ��ڴ���!������˳��ۺϲ���...");
					return;
				}
			}
			break;
#if K21_ENABLE
		case '7':
#else
		case DOWNKEY:
#endif
			/*�·���*/
			if (nPageSel>=maxpage)
				nPageSel = 0;
			else
				nPageSel++;
			break;
#if K21_ENABLE
		case '8':
#else
		case UPKEY:
#endif
			/*�Ϸ���*/
			if (nPageSel==0)
				nPageSel = maxpage;
			else
				nPageSel--;
			break;
		case ESC:
#if STABAR_ENABLE	//20140626 linwl added
			NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
			return;
			break;
		default:
			continue;
			break;
		}
	}
	}
}

//===============���º���Ϊ�ۺ�ģ�鹲�õ�,�൱���ۺϵ�lib.c=========
#if !defined ME66 //ME66�ռ�С ��֧�� ������
#if PRN_ENABLE
//��ȡ��ӡ��״̬(��������æ״̬(����ʱ��Ϊtimeout)
//OK��OVERʱ,ת��ΪSUCC(0);�����Ĳ�ת��
//���й��ܲ�ʵ�֣�"æʱ����,����TIMEOUT(-3);����(����)״̬һ�ɷ���FAIL(-1)(���������Ϣ�������ѻ���ʾ��)"
int GetPrinterStatus(unsigned int timeout)
{
	/*private & local definition*/
	EM_PRN_STATUS prnstatus = 0;
	int busy = FALSE, ret = 0;
	time_t oldtime = 0, diff = 0;
	//time_t now = 0;

	 /*process body*/
	 //�쳣״̬����ʾ,����̬������ʾ
	while(1)
	{
		if((ret=NDK_PrnGetStatus(&prnstatus))!=NDK_OK)//prnstatus = getprinterstatus();
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ȡ��ӡ��״̬ʧ��%d", __FILE__, __LINE__, ret);
			return ret;
		}

		switch(prnstatus)
		{ 
		case PRN_STATUS_NOPAPER:
			if(cls_show_msg_record(FILENAME ,FUNCNAME ,"��ӡ��ȱֽ!װֽ��,���������,ESC����")==ESC)
			{
				NDK_ScrClrs();//clrscr();
				return prnstatus;//FAIL;
			}
			NDK_PrnStart();//print("\f");//ȱֽ��,���ݱ�����.Ϊ��װֽ��,�ص�������״̬(OK��OVER)
			NDK_ScrClrs();//clrscr();
			break;
		case PRN_STATUS_OVERHEAT:
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ������\n", __FILE__, __LINE__);
			NDK_ScrClrs();//clrscr();
			return prnstatus;//FAIL;
			break;
		case PRN_STATUS_BUSY:
			cls_printf("���ڴ�ӡ,���Ժ�...");
			if(busy==FALSE)
			{
				oldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
				busy = TRUE;
			}
			if((diff=time(NULL)-oldtime)>timeout)//if((diff=(now=time(NULL))-oldtime)>timeout)
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ������æ״̬ʱ�����,����\n", __FILE__, __LINE__);
				NDK_ScrClrs();//clrscr();
				return prnstatus;//TIMEOUT;
			}
			show_stopwatch(ENABLE, diff);
			break;
		case PRN_STATUS_OK:
			show_stopwatch(DISABLE, 0);
			return SUCC;
			break;
		case PRN_STATUS_VOLERR:
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����Դ����\n", __FILE__, __LINE__);
			NDK_ScrClrs();//clrscr();
			return prnstatus;//FAIL;
			break;
		case PRN_STATUS_DESTROYED:
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����\n", __FILE__, __LINE__);
			NDK_ScrClrs();
			return prnstatus;
			break;
		default:
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��������(%d)\n", __FILE__, __LINE__, prnstatus);
			NDK_ScrClrs();//clrscr();
			return prnstatus;//FAIL;
			break;
		}
	}

	show_stopwatch(DISABLE, 0);
	NDK_ScrClrs();//clrscr();
	return SUCC;
}

//��ӡ���Ҵ�б����
//ע��:���������н�ʹ��ѹ��ģʽ����ӡ
void print_triangle(int nPageWidth)
{
	/*private & local definition*/
#if !(defined ME31||ANDIROD_ENABLE)//||defined IM81||defined N900
	unsigned char buf[426*424/8] = {0}; //�����С384*384/8->426*424/8,��������������Խ��Ŀ��,�����岻���ܷ�
#else
	unsigned char buf[48*48] = {0}; //K21�ռ����ƻ�����ֻ��2K����
	nPageWidth = 48;//��K21���ǿ�ĸ߶����Ƴ�48�����򻺳�̫������� 20140520 linwl
#endif
	unsigned char mask = 0;
	unsigned int i = 0, j = 0, k = 0, bytesPerLine = 0;
	int ret = 0;
	
	/*process body*/
	nPageWidth = MIN(nPageWidth, 424);//��ֹ�������nPageWidth����ʱ,���𻺳������
	NDK_PrnSetMode(PRN_MODE_NORMAL, 0);//setprintmode(7, 0);
	NDK_PrnStr("---���´�ӡ���Ҵ�б����---\n");//print("---���´�ӡ���Ҵ�б����---\n");
	
	//��б����(��)---------
	memset(buf,0,sizeof(buf));	
	bytesPerLine =(nPageWidth+7)/8;	// ÿ���ֽ���
	for(i=0;i<nPageWidth;i++)
	{
		for(j=0;j<i/8;j++)
			buf[i*bytesPerLine+j]=0xFF;
		
		mask = 0x80;
		
		for(k=0;k<=(i&7);k++)
		{
			buf[i*bytesPerLine+j]|=mask;
			mask >>= 1;
		}
	}	
	if((ret=NDK_PrnImage(nPageWidth,nPageWidth,0,(char *)buf))!=NDK_OK)//if(printimage(nPageWidth,nPageWidth,0,(char *)buf)<0)
	{
		cls_show_msg("%s, line %d:��ӡ��б����ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	//��б����(��)---------
	memset(buf,0xff,sizeof(buf));
	for(i=0;i<nPageWidth;i++)
	{
		for(j=0;j<(nPageWidth-i)/8;j++)
			buf[i*bytesPerLine+j]=0x00;

		mask = 0x7F;
		
		for(k=0;k<((nPageWidth-i)&7);k++)
		{
			buf[i*bytesPerLine+j]&=mask;
			mask >>= 1;
		}
	}
	if((ret=NDK_PrnImage(nPageWidth,nPageWidth,0,(char *)buf))!=NDK_OK)//if(printimage(nPageWidth,nPageWidth,0,(char *)buf)<0)
	{	
		cls_show_msg("%s, line %d:��ӡ��б����ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}	
	NDK_PrnStr("\n");//print("\n");
	NDK_PrnStart();//print("\f");
	return;
}

void print_triangle_384(void)
{
	/*private & local definition*/
	int type = 0;//EM_PRN_TYPE type = 0;
	int MAXPRLINEWIDE = 0;

	/*process body*/
	type = lib_getprintertype();//NDK_PrnGetType(&type);
	MAXPRLINEWIDE = (PRN_TYPE_TP==type)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	print_triangle(MAXPRLINEWIDE);
}

//��ӡ����ҳ
void print_testpage(void)
{
	/*private & local definition*/
	const char *p = NULL;
	int ret = 0;

	/*process body*/
	if(time(NULL)&1)
		p = CCB_BMP;
	else 
		p = NL_BMP;//ԭ��ΪNL_Icon,���������!
	
	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE, 1);//setprintmode(0,0);   //����˫��
	NDK_PrnSetFont(PRN_HZ_FONT_32x32, PRN_ZM_FONT_16x32);//setprintfont(DOT16x32+(HZ32x32<<8));	
	NDK_PrnStr("---���´�ӡ����ҳ---\n");//print("---���´�ӡ����ҳ---\n");
	
	if((ret=NDK_PrnImage(128,64,0,p))!=NDK_OK)//if(printimage(128,64,0,p)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡͼ��ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	if((ret=NDK_PrnStr("��ӭʹ�ø����´�½���Թ�˾NLϵ��POS������ӡ��"))!=NDK_OK)//if(print("��ӭʹ�ø����´�½���Թ�˾NLϵ��POS������ӡ��")!=SUCC)
	{
		cls_show_msg("%s, line %d:�ͺ���ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	NDK_PrnSetFont(0, PRN_ZM_FONT_16x32);//setprintfont(DOT16x32);
	if((ret=NDK_PrnStr("\n[16x32] !\"#$%&()*./0123456789;<=>?@ABCDEFG\\^_`abcdejfgz{|}~"))!=NDK_OK)//if(print("\n[16x32] !\"#$%&()*./0123456789;<=>?@ABCDEFG\\^_`abcdejfgz{|}~")!=SUCC)
	{
		cls_show_msg("%s, line %d:��16x32�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
		
	NDK_PrnSetFont(0, PRN_ZM_FONT_16x16);//setprintfont(DOT16x16);
	if((ret=NDK_PrnStr("\n[16x16] !\"#$%&()*+,-./0123456789;<=>?@ABCDJKLPQRSTUVW\\^_`abcdejfgmnoxyz{|}~"))!=NDK_OK)//if(print("\n[16x16] !\"#$%&()*+,-./0123456789;<=>?@ABCDJKLPQRSTUVW\\^_`abcdejfgmnoxyz{|}~")!=SUCC)
	{
		cls_show_msg("%s, line %d:��16x16�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	NDK_PrnSetFont(0, PRN_ZM_FONT_6x8);//setprintfont(DOT6x8);
	if((ret=NDK_PrnStr("\n[6x8] !\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdejfghijklmnopqrstuvwyz{|}~"))!=NDK_OK)//if(print("\n[6x8] !\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdejfghijklmnopqrstuvwyz{|}~")!=SUCC)
	{
		cls_show_msg("%s, line %d:��6x8�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
		
	NDK_PrnSetFont(PRN_HZ_FONT_16x16, 0);//setprintfont(HZ16x16<<8); 
	if((ret=NDK_PrnStr("\n[16x16] ϧԵ ϧ�� ���� �ж�!"))!=NDK_OK)//if(print("\n[16x16] ϧԵ ϧ�� ���� �ж�!")!=SUCC)
	{
		cls_show_msg("%s, line %d:��16x16����ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	NDK_PrnSetFont(PRN_HZ_FONT_12x16, PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ12x16<<8));  
	if((ret=NDK_PrnStr("\n[12x16] ϧԵ ϧ�� ���� �ж�!\n"))!=NDK_OK)//if(print("\n[12x16] ϧԵ ϧ�� ���� �ж�!\n")!=SUCC)
	{
		cls_show_msg("%s, line %d:��12x16����ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	NDK_PrnSetFont(PRN_HZ_FONT_16x8, PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ16x8<<8));  
	if((ret=NDK_PrnStr("[16x8] ϧԵ ϧ�� ���� �ж�!\n"))!=NDK_OK)//if(print("[16x8] ϧԵ ϧ�� ���� �ж�!\n")!=SUCC)
	{
		cls_show_msg("%s, line %d:��16x8����ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	NDK_PrnSetFont(0, PRN_ZM_FONT_10x8);//setprintfont(DOT10x8);   //����ѹ����ӡ
	if((ret=NDK_PrnStr("[10x8]!\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdejfghijklmnopqrstuvwyz{|}~\n"))!=NDK_OK)//if(print("[10x8]!\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdejfghijklmnopqrstuvwyz{|}~\n")!=SUCC)
	{
		cls_show_msg("%s, line %d:��10x8�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}   

	//setprintmode(0,0);//setprintmode(6,1);    
	NDK_PrnSetFont(0, PRN_ZM_FONT_10x16);//setprintfont(DOT10x16);   //�ݺ���ѹ����ӡ
	if((ret=NDK_PrnStr("[10x16]!\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwyz{|}~\n"))!=NDK_OK)//if(print("[10x16]!\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwyz{|}~\n")!=SUCC)
	{
		cls_show_msg("%s, line %d:��10x16�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	} 
	
	//setprintmode(0,0);//setprintmode(3,1);   
	NDK_PrnSetFont(0, PRN_ZM_FONT_5x16);//setprintfont(DOT5x16);  //����ѹ��˫��
	if((ret=NDK_PrnStr("[5x16]!\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwyz{|}~\n"))!=NDK_OK)//if(print("[5x16]!\"#$%&()*+-/,.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwyz{|}~\n")!=SUCC)
	{
		cls_show_msg("%s, line %d:��5x16�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	
	NDK_PrnSetFont(0, PRN_ZM_FONT_5x7);//setprintfont(DOT5x7);
	if((ret=NDK_PrnStr("[5x7] !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwxyz{|}~\n"))!=NDK_OK)//if(print("[5x7] !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwxyz{|}~\n")!=SUCC)
	{
		cls_show_msg("%s, line %d:��5x7�ַ�ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
		
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡ�����
void print_rand(void)
{
	/*private & local definition*/
	char buf[1024] = {0};
	int i = 0, ret = 0;

	/*process body*/
	memset(buf,0,sizeof(buf));
	for (i=0;i<sizeof(buf);i++)
	{
		if(!(buf[i]=rand()%256))
			break;
	}
	
	print_Ex("---���´�ӡ%d�������---\n", i);
	if((ret=NDK_PrnStr(buf))!=NDK_OK)//if(print(buf)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡ�����ʧ��!(%d)", __FILE__, __LINE__, ret);
		return;
	}

	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡ20��"��"��
#define	NUM_GUO	(240)
void print_guo(void)
{
	/*private & local definition*/
	int i = 0, ret = 0;

	/*process body*/
	NDK_PrnSetFont(PRN_HZ_FONT_16x16, PRN_ZM_FONT_8x8);//setprintfont(DOT8x8+(HZ16x16<<8));//setprintfont(DOT8x8+(HZ24x24<<8));
	print_Ex("---���´�ӡ%d��\"��\"---\n", NUM_GUO);
	
	for(i=0;i<NUM_GUO;i++)
	{
		if((ret=NDK_PrnStr("��"))!=NDK_OK)//if(print("��")!=SUCC)
		{
			cls_show_msg("%s, line %d:��ӡ\"��\"��ҳʧ��!(%d)", __FILE__, __LINE__, ret);
			return;
		}
	}
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡ��䵥
void print_fill(void)
{
	/*private & local definition*/
	char printbuf[(128*64)/8] = {0};
	int ret = -1;
	
	/*process body*/
	NDK_PrnStr("---���´�ӡ��䵥---\n");//print("---���´�ӡ��䵥---\n");
	
	memset(printbuf,0xff,sizeof(printbuf));
	if((ret=NDK_PrnImage(128,64,0,printbuf))!=NDK_OK)//if(printimage(128,64,0,printbuf)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡ��䵥ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}

	NDK_PrnStr("\n");//print("\n");
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡƱ��
void print_bill(void)
{
	/*private & local definition*/
//	struct tm stTestTime; //struct postime stTestTime;
	char psBuf[125]={0};
	uint unLen = 0;
	//char szTmp[256] = {0};
	
	/*process body*/

#if 1//20131128,�������������۵�POS�ն�Ӳ���淶������������޸�bill����
	if(NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO, &unLen, psBuf)!=NDK_OK||psBuf[10]==0xff)
	{
		cls_show_msg("%s, line %d:����ʧ��", __FILE__, __LINE__);
		return;
	}
	if(psBuf[10]>0 && psBuf[10]<0x80) //����
	{
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_12x24A);
		NDK_PrnSetForm(0,0,6);
		NDK_PrnSetMode(PRN_MODE_NORMAL,1);
		NDK_PrnFeedByPixel(50); //����ֽ50���ص�(���ݸ߶˲�Ʒ���ֵĿ�����������) 20180428 add
		NDK_PrnStr("1:����POSǩ����\n");
		NDK_PrnStr("2: --------------------------\n");
		NDK_PrnStr("3:�ֿ��˴��  CUSTOMER COPY\n");
		NDK_PrnStr("4: ---------------------------\n");
		NDK_PrnStr("5:�̻�����(MERCHANT Name):\n 6:�����̻�����\n");
		NDK_PrnStr("7:�̻���(MERCHANT NO.):\n  8:1234567890123\n");
		NDK_PrnStr("9:�ն˱��(TERMINAL NO.):123456\n");
		NDK_PrnStr("10:����Ա��(Operator NO.):01\n");
		NDK_PrnStr("11: -------------------------\n");
		NDK_PrnStr("12:�յ���(ACQUIRER):\n 13:�յ����� 123456\n");
		NDK_PrnStr("14:������(ISSUER):��������12345\n");
		NDK_PrnStr("15:����(CARD NO.):\n");
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_16x32A);
		NDK_PrnStr("16:6200123456789012\n\n");
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_12x24A);
		NDK_PrnStr("17:��������(TRANS TYPE):����\n");
		NDK_PrnStr("18:��Ч��(EXP Date):2015/01\n");
		NDK_PrnStr("19:���κ�(BATCH NO.):12345\n");
		NDK_PrnStr("20:ƾ֤��(VOUCHER NO.):789\n");
		NDK_PrnStr("21:��Ȩ��(AUTH NO.):345678\n");
		NDK_PrnStr("22:���ײο���(REFER NO.):\n  23:9012345678\n");
		NDK_PrnStr("24:����/ʱ��(DATE/TIME):\n  25:2019/07/18 12:34:56\n");
		NDK_PrnStr("26:���׽��(AMOUNT):");
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_16x32A);
		NDK_PrnStr("12345.90\n");	
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_12x24A);
		NDK_PrnStr("27: -------------------------\n");
		NDK_PrnStr("28:Ӧ�ñ�ʶ(AID)123456789\n");
		NDK_PrnStr("29:��ע/REFERENCE\n");
		NDK_PrnStr("30:�����Ƿ������б�ע��Ϣ\n");
		NDK_PrnStr("31:�����Ƿ������б�ע��Ϣ\n");
		NDK_PrnStr("32:�����Ƿ������б�ע��Ϣ\n");
		NDK_PrnStr("33:֣������:�ñʽ���Ϊ����״̬,����Ч����!\n");
		NDK_PrnStr("34: -------------------------\n");
		NDK_PrnStr("35:�ֿ���ǩ��:\n\n\n\n\n");
		NDK_PrnStr("36:����ȷ�����Ͻ��ף�ͬ�⽫����뱾���˻�\n");
		NDK_PrnStr("37:I ACKNOWLEDGE SATISFACTORY RECEIPT OF RELATIVE GOODS/SERVICE\n");
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_8x16);
		NDK_PrnStr("\n\n");
		NDK_PrnStr("����: - - - - - X - - - - -- X - - - - --\n\n\n\n\n\n");
		NDK_PrnStart();
	}
	else
	{
		NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_8x16);
		NDK_PrnSetForm(0,0,2);
		NDK_PrnSetMode(PRN_MODE_NORMAL,1);
		NDK_PrnFeedByPixel(50); //����ֽ50���ص�
		NDK_PrnStr("����POSǩ����\n");
		NDK_PrnStr("--------------------------------\n");
		NDK_PrnStr("�ֿ��˴��CUSTOMER COPY\n");
		NDK_PrnStr("--------------------------------\n");
		NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);
		NDK_PrnStr("�̻�����(MERCHANT Name):\n  �����̻�����\n");
		NDK_PrnStr("�̻���(MERCHANT NO.):\n  123456789012345\n");
		NDK_PrnStr("�ն˱��(TERMINAL NO.):12345678\n");
		NDK_PrnStr("����Ա��(Operator NO.):01\n");
		NDK_PrnStr("--------------------------------\n");
		NDK_PrnStr("�յ���(ACQUIRER):\n  �յ����� 12345678\n");
		NDK_PrnStr("������(ISSUER):�������� 12345678\n");
		NDK_PrnStr("����(CARD NO.):\n");
		NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_10x16);
		NDK_PrnStr("  6200123456789012345\n");
		NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);
		NDK_PrnStr("��������(TRANS TYPE):����/SALE\n");
		NDK_PrnStr("��Ч��(EXP Date):2015/01\n");
		NDK_PrnStr("���κ�(BATCH  NO.):123456\n");
		NDK_PrnStr("ƾ֤��(VOUCHER  NO.):789012\n");
		NDK_PrnStr("��Ȩ��(AUTH  NO.):345678\n");
		NDK_PrnStr("���ײο���(REFER NO.):\n  901234567890\n");
		NDK_PrnStr("����/ʱ��(DATE/TIME):\n  2019/07/18  12:34:56\n");
		NDK_PrnStr("���׽��(AMOUNT):");
		NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_16x32);
		NDK_PrnStr("12345678.90\n");	
		NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);
		NDK_PrnStr("--------------------------------\n");
		NDK_PrnStr("Ӧ�ñ�ʶ(AID)1234567890123456\n");
		NDK_PrnStr("��ע/REFERENCE\n");
		NDK_PrnStr("�����Ƿ������б�ע��Ϣ\n");
		NDK_PrnStr("�����Ƿ������б�ע��Ϣ\n");
		NDK_PrnStr("�����Ƿ������б�ע��Ϣ\n");
		NDK_PrnStr("֣������:�ñʽ���Ϊ����״̬,������Ч����,ֻ������ʹ��\n");
		NDK_PrnStr("--------------------------------\n");
		NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_16x16);
		NDK_PrnStr("�ֿ���ǩ��:\n\n\n\n\n");
		NDK_PrnStr("����ȷ�����Ͻ��ף�ͬ�⽫����뱾���˻�\n");
		NDK_PrnStr("I ACKNOWLEDGE SATISFACTORY RECEIPT OF RELATIVE GOODS/SERVICE\n");
		NDK_PrnSetFont(PRN_HZ_FONT_24x24A,PRN_ZM_FONT_8x16);
		NDK_PrnStr("\n\n");
		NDK_PrnStr("- - - - - - - X - - - - - - - X - - - - - - - \n\n\n\n\n\n");
		NDK_PrnStart();
	}
	
#else
	NDK_SysGetPosTime(&stTestTime);//getpostime(&stTestTime);
	
	NDK_PrnSetMode(PRN_MODE_NORMAL,1);//setprintmode(6,2);
	NDK_PrnStr("---���´�ӡƱ��---\n");//print("---���´�ӡƱ��---\n");
	
#if 0
	NDK_PrnStr("\n\n����IC�����Ե���\n �´�½POS������");//print("\n\n����IC�����Ե���\n �´�½POS������");
	NDK_PrnStr("\n\nAID:   PBOC1\n    A00000000386980701"); //print("\n\nAID:   PBOC1\n    A00000000386980701");  
	NDK_PrnStr("\n\n  ���Ӵ���");//print("\n\n  ���Ӵ���");
	NDK_PrnStr("\r0002 1998 0817 0000 0001");//print("\r0002 1998 0817 0000 0001");
	NDK_PrnStr("\r ISSUER:11112222033330002");//print("\r ISSUER:11112222033330002");
	NDK_PrnStr("\n    Ȧ��");//print("\n    Ȧ��");
	sprintf(szTmp,"\n\n %4d��%02d��%02d��   %02dʱ%02d��%d��",
						stTestTime.tm_year+1900,
						stTestTime.tm_mon+1,
						stTestTime.tm_mday,			
						stTestTime.tm_hour,
						stTestTime.tm_min,
						stTestTime.tm_sec);			
	NDK_PrnStr(szTmp);//print(szTmp);
	NDK_PrnStr("\r\r    000041       000189"
		"\r\r    00000000\r\r    RMB:       147.85\r");//print("\r\r    000041       000189"
		"\r\r    00000000\r\r    RMB:       147.85\r");
	NDK_PrnStr("\r\r\r      ͬ��\r\r\r ǩ����________");//print("\r\r\r      ͬ��\r\r\r ǩ����________");
	NDK_PrnStart();//print("\f"); 
#else
	NDK_PrnSetFont(PRN_HZ_FONT_32x32, PRN_ZM_FONT_16x32);//setprintfont(DOT16x32+(HZ32x32<<8));
	NDK_PrnStr("      POSǩ����\r");//print("      POSǩ����\r");
	NDK_PrnSetFont(PRN_HZ_FONT_16x16, PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ16x16<<8));
	NDK_PrnStr("                �̻����/MERCHANT COPY\r");//print("                �̻����/MERCHANT COPY\r");
	//�̻�����
	NDK_PrnStr("�̻�����(MERCHANT NAME):�й�����\r\r");//print("�̻�����(MERCHANT NAME):�й�����\r\r");
	// �̻���
	NDK_PrnStr("�̻���(MERCHANT NO.:):\r");//print("�̻���(MERCHANT NO.:):\r");
	
	NDK_PrnSetFont(PRN_HZ_FONT_16x32,PRN_ZM_FONT_16x32);//setprintfont(DOT16x32+(HZ16x32<<8));
	NDK_PrnStr("  123456789123456\r");//print("  123456789123456\r");//print("  123456789123456\r");

	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ16x16<<8));
	// POS��� ����Ա���
	NDK_PrnStr("�ն˱��(TERMINAL NAME):12345678\r����Ա��(OPERSTOR NO):02\r");//print("�ն˱��(TERMINAL NAME):12345678\r����Ա��(OPERSTOR NO):02\r");
	//�յ���
	NDK_PrnStr("�յ���(ACQUIRER):23456789011\r");//print("�յ���(ACQUIRER):23456789011\r");
	//������
	NDK_PrnStr("������(ISSUER):01025678901\r");//print("������(ISSUER):01025678901\r");
	//pos���ĺ�
	NDK_PrnStr("���ĺ�(CENTER):87654321\r");//print("���ĺ�(CENTER):87654321\r");
	// ����
	NDK_PrnStr("����(CARD NO.):\r"); //print("����(CARD NO.):\r"); 
	NDK_PrnSetFont(PRN_HZ_FONT_16x32,PRN_ZM_FONT_16x32);//setprintfont(DOT16x32+(HZ16x32<<8));
	NDK_PrnStr("  6013821600029014090\r");//print("  6013821600029014090\r");
	
#if 0	//for 11cm
	//�̻�����
	NDK_PrnStr("�̻�����(MERCHANT NAME):�й�����\r\r");
	// �̻���
	NDK_PrnStr("�̻���(MERCHANT NO.:):\r");
	NDK_PrnSetFont(PRN_HZ_FONT_16x32,PRN_ZM_FONT_16x32);
	NDK_PrnStr("  123456789123456\r");

	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);
	// POS��� ����Ա���
	NDK_PrnStr("�ն˱��(TERMINAL NAME):12345678\r����Ա��(OPERSTOR NO):02\r");
	//�յ���
	NDK_PrnStr("�յ���(ACQUIRER):23456789011\r");
	//������
	NDK_PrnStr("������(ISSUER):01025678901\r");
	//pos���ĺ�
	NDK_PrnStr("���ĺ�(CENTER):87654321\r");
	// ����
	NDK_PrnStr("����(CARD NO.):\r"); 
	NDK_PrnSetFont(PRN_HZ_FONT_16x32,PRN_ZM_FONT_16x32);
	NDK_PrnStr("  6013821600029014090\r");	
#endif	

	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ16x16<<8));
	// ��������
	NDK_PrnStr("��������(TRANS TYPE):����\r");//print("��������(TRANS TYPE):����\r");
	// ��Ч��
	NDK_PrnStr("��Ч��(EXP DATE):2038/12\r");//print("��Ч��(EXP DATE):2038/12\r");
	
	//���κš�ƾ֤��
	NDK_PrnStr("���κ�(BATCH NO.):800011\rƾ֤��(VOUCHER NO.):000107\r");//print("���κ�(BATCH NO.):800011\rƾ֤��(VOUCHER NO.):000107\r");
	// ��Ȩ�롢ϵͳ�ο���
	NDK_PrnStr("��Ȩ��(AUTH NO.):author\r�ο���(REFER NO.):0123456788\r");//print("��Ȩ��(AUTH NO.):author\r�ο���(REFER NO.):0123456788\r");
	// �������ڡ�ʱ��		
	//print("����ʱ��(DATE/TIME):\r   2009/04/14   11:22:22\r");
	print_Ex("\n\n %4d��%02d��%02d��   %02dʱ%02d��%d��",
						stTestTime.tm_year+1900,
						stTestTime.tm_mon+1,
						stTestTime.tm_mday,			
						stTestTime.tm_hour,
						stTestTime.tm_min,
						stTestTime.tm_sec);
	// �⿨��֯����
	NDK_PrnStr("���ʿ���֯:DJJ\r");//print("���ʿ���֯:DJJ\r");
	// ��ӡ���׽��
	NDK_PrnSetFont(PRN_HZ_FONT_16x8,PRN_ZM_FONT_16x32);//setprintfont(DOT16x32+(HZ32x16<<8));
	NDK_PrnStr("���:RMB222.22\r");//print("���:RMB222.22\r");		
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ16x16<<8));
	NDK_PrnStr("��ע(REFERENCE):\r");//print("��ע(REFERENCE):\r");
	NDK_PrnStr("֣������:�ñʽ���Ϊ����״̬,����Ч����!\r");//print("֣������:�ñʽ���Ϊ����״̬,����Ч����!\r");
	NDK_PrnStr("�ֿ���ǩ��(CARD HOLDER SIGHATURE):\r\r\r\r\r\r");//print("�ֿ���ǩ��(CARD HOLDER SIGHATURE):\r\r\r\r\r\r");
	NDK_PrnStr("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\r");//print("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\r");
	NDK_PrnStr("����ȷ�����Ͻ��ף�ͬ�⽫����뱾���˻�\r\r");//print("����ȷ�����Ͻ��ף�ͬ�⽫����뱾���˻�\r\r");
	NDK_PrnStr("\r\r\r\r------- X-------X-------\r\r\r\r\r");//print("\r\r\r\r------- X-------X-------\r\r\r\r\r");
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
#endif

	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE, 1);//setprintmode(0, 0);
#endif
	return;
}

//��ӡ������
//ע��:����������ֻʹ��ѹ��ģʽ����ӡ
void print_verticalbill(void)
{
	/*private & local definition*/
	/*unsigned */char buf_img[PRNIMGBUFLEN] = {0};
	int type = 0;//EM_PRN_TYPE type = 0;
	char c = 0;
	int ret = -1, i = 0,xsize = 0;// type = getprintertype(), xsize = (PRINTTYPE_TP==type)?MAXPRLINEWIDE_TP:((PRINTTYPE_HIP==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	
	/*process body*/
	type = lib_getprintertype();//NDK_PrnGetType(&type);
	xsize = (PRN_TYPE_TP==type)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	
	NDK_PrnSetMode(PRN_MODE_NORMAL, 0);//setprintmode(7, 0);
	NDK_PrnStr("---���´�ӡ������---\n");//print("---���´�ӡ������---\n");

	c = (rand()&1)?0xaa:0x55;
	for(i=0;i<PRNIMGBUFLEN;i++)
	{
		buf_img[i]=c;
		c^=0xff;
	}
	if((ret=NDK_PrnImage(xsize,PRNIMGHEIGHT,0,buf_img))!=NDK_OK)//if(printimage(xsize,PRNIMGHEIGHT,0,(char *)buf_img)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡͼ��ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡ�հ׵�
void print_blank(void)
{
	/*private & local definition*/
	int type = 0;//EM_PRN_TYPE type = 0;
	/*unsigned */char buf_img[PRNIMGBUFLEN] = {0};
	//int type = getprintertype() ,xsize = (PRINTTYPE_TP==type)?MAXPRLINEWIDE_TP:((PRINTTYPE_HIP==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	int i = 0, xsize = 0;
	
	/*process body*/
	type = lib_getprintertype();//NDK_PrnGetType(&type);
	xsize = (PRN_TYPE_TP==type)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	
	NDK_PrnSetMode(PRN_MODE_NORMAL, 0);//setprintmode(7, 0);
	NDK_PrnStr("---���´�ӡ�հ׵�---\n");//print("---���´�ӡ�հ׵�---\n");

	memset(buf_img,0x00,PRNIMGBUFLEN);
	for(i=0;i<10;i++)
	{
		if(NDK_PrnImage(xsize,PRNIMGHEIGHT,0,buf_img)!=NDK_OK)//if(printimage(xsize,PRNIMGHEIGHT,0,(char *)buf_img)!=SUCC)
		{
			cls_show_msg("%s, line %d:��ӡ�հ׵�ʧ��\n", __FILE__, __LINE__);
			return;
		}
		NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	}
	return;
}

//��ӡ��������
void print_font(void)
{
	/*private & local definition*/
#include	"dotfont.h"
#include	"hzfont.h"

	int imode = 0, idot = 0, /*irow_space=2, */ihz = 0, i = 0 ,ret = -1;
	
	struct tm stTestTime;//struct postime stTestTime;
	//char szTmp[256] = {0};
	//char infoprint[1024] = {0};
	
	/*process body*/
	NDK_PrnStr("---���´�ӡ��������---\n");//print("---���´�ӡ��������---\n");
	
	for(imode=PRN_MODE_ALL_DOUBLE; imode<PRN_MODE_NORMAL; imode++)
	//for(irow_space=0;irow_space<8;irow_space++)    panxl 2007.7.18���Σ�irow_space��ֵΪ2
	for(; i<2; i++)
	{		
		for(ihz=0; ihz<SZ_ARRAY(hzfont); ihz++)   //for(ihz=0;ihz<strlen(hzfont);ihz++)   panxl 2007.7.18 modify
		{
			for(idot=0; idot<SZ_ARRAY(dotfont); idot++)    //for(idot=0;idot<strlen(dotfont);idot++)  panxl 2007.7.18 modify
			{	
				if((ret=NDK_PrnSetMode(imode,i))!=NDK_OK)//if(setprintmode(imode,irow_space)!=SUCC)
				{
					cls_show_msg("%s, line %d:���ô�ӡģʽʧ��(%d,%d,%d)", __FILE__, __LINE__, imode, i, ret);
					return;
				}
				
				if((ret=NDK_PrnSetFont(hzfont[ihz],dotfont[idot]))!=NDK_OK)//if(setprintfont(dotfont[idot]+(hzfont[ihz]<<8))!=SUCC)
				{
					cls_show_msg("%s, line %d:���ô�ӡ����ʧ��(%s,%s,%d)", __FILE__, __LINE__, dotfontstr[idot], hzfontstr[ihz], ret);
					return;
				}
				
				if(print_Ex("��ӡģʽ[%d,%d]\n��ӡ����[%s,%s]:\n(�����ȴ�ͼ���ٴ���)\n", imode, i, hzfontstr[ihz], dotfontstr[idot])!=SUCC)
				{
					cls_show_msg("%s, line %d:������ʧ��\n", __FILE__, __LINE__);
					return;                        
				}

				if(time(NULL)%2) 
				{
					if((ret=NDK_PrnImage(128,64,0,CCB_BMP))!=NDK_OK)//if(printimage(128,64,0,CCB_BMP)!=SUCC)
					{
						cls_show_msg("%s, line %d:��ͼ������ʧ��(%d)", __FILE__, __LINE__, ret);
						return;
					}
				}
				else 
				{
					if((ret=NDK_PrnImage(144,64,0,CCB_BMP_144))!=NDK_OK)//if(printimage(144,64,0,CCB_BMP_144)!=SUCC)
					{
						cls_show_msg("%s, line %d:��ͼ������ʧ��(%d)", __FILE__, __LINE__, ret);
						return;
					}
				}
				if((ret=NDK_PrnStr("01234�´�½��ӭ��\n0123456NewLandComputer Welcome you!\n~!@#$%^&*()_+|{[}]:;'<,>.?/\\=-dsfh\n"))!=NDK_OK)
				{
					cls_show_msg("%s, line %d:������ʧ��(%d)", __FILE__, __LINE__, ret);
					return;                       
				}

				NDK_SysGetPosTime(&stTestTime);//getpostime(&stTestTime);
				if(print_Ex("\n %4d��%02d��%02d��   %02dʱ%02d��%d��\n",
					stTestTime.tm_year+1900,
					stTestTime.tm_mon+1,
					stTestTime.tm_mday,			
					stTestTime.tm_hour,
					stTestTime.tm_min,
					stTestTime.tm_sec)!=SUCC)
				{
					cls_show_msg("%s, line %d:������ʧ��\n", __FILE__, __LINE__);
					return;
				}

				NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
				
#if 0
				///////////////////////////////////
				//panxl 2007.7.18 addѭ��������ʾ
				cls_show_msg1(1, "imode=%d,\nihz=%d,\nidot=%d",imode,ihz,idot);
				///////////////////////////////////
#endif
				if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
				{
					cls_show_msg("%s, line %d:��ӡ��״̬�쳣!(%d,%d,%d,%d)", __FILE__, __LINE__, imode, i, ihz, idot);
					return;                         
				}					

				//ʱ��̫��,��׽��������if(kbhit()==ESC)
				if(wait_anykey(1)==ESC)
					return;
			}
		}
	}

	return;
}

//��ӡͼ��
//ע��:���������н�ʹ��ѹ��ģʽ����ӡ
void print_image(int MageWide, int MageHigh, int MagePon, const char *p)
{
	/*private & local definition*/
	int ret = -1;
	
	/*process body*/
	NDK_PrnSetMode(PRN_MODE_NORMAL, 0);//setprintmode(7, 0);
	NDK_PrnStr("---���´�ӡͼ��---\n");//print("---���´�ӡͼ��---\n");
	
	//��ӡ��ͼ�κͲ���	
	print_Ex("���:%d,���:%d,�߶�:%d\n",MagePon,MageWide,MageHigh);
	if((ret=NDK_PrnImage(MageWide,MageHigh,MagePon,p))!=NDK_OK)//if(printimage(MageWide,MageHigh,MagePon,p)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡͼ��ʧ��(%d)", __FILE__, __LINE__, ret);
		return;
	}

	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

void print_image_CCB128(void)
{
	/*private & local definition*/

	/*process body*/
	NDK_PrnImage(128,64,0,CCB_BMP);//print_image(128, 64, 0, CCB_BMP);
	
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡ�س�
void print_enter(void)
{
	/*private & local definition*/
	int i = 0;

	/*process body*/
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);//setprintfont(HZ16x16<<8);
	NDK_PrnStr("---����������ӡ5���س�--\n");//print("---����������ӡ5���س�--\n");
	
	for(i=0; i<5; i++)
		NDK_PrnStr("\r");//print("\r");
	NDK_PrnStr("--����������ӡ20���س�--\n");//print("--����������ӡ20���س�--\n");
	for(i=0; i<20; i++)
		NDK_PrnStr("\r");//print("\r");

	NDK_PrnStr("----������ӡ�س�����----\n");//print("----������ӡ�س�����----\n");
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//ѹ����ӡ ��ż�հ��� ,��ż�ı���,��ż�س���			
//��ż�հ���
void print_compress(void)
{
	/*private & local definition*/
	int i = 0;
	
	/*process body*/
	NDK_PrnStr("---���´�ӡѹ��Ч��---\n");//print("---���´�ӡѹ��Ч��---\n");
	
	NDK_PrnStr("          \r");//print("          \r");
	
	NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE,1);//setprintmode(2,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_16x16);//setprintfont(HZ16x16<<8);   //panxl
	cls_printf("\n���ڴ�ӡ,���Ե�\n��ż��\n");
	for (i=1;i<=24;i++)
		NDK_PrnStr("��");	//print("��");	

	NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE,0);//setprintmode(3,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);//setprintfont(HZ16x16<<8);   //panxl
	NDK_PrnStr("          \r");//print("          \r");
	NDK_PrnStr("          \r");//print("          \r");
	for (i=1;i<=24;i++)
		NDK_PrnStr("��");	//print("��");	
	
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);//setprintfont(HZ16x16<<8);   //panxl
	NDK_PrnStr("          \r");//print("          \r");
	NDK_PrnStr("          \r");//print("          \r");
	NDK_PrnStr("          \r");//print("          \r");
	for (i=1;i<=24;i++)
		NDK_PrnStr("��");	//print("��");	

	NDK_PrnStr("\n��ӡǰ��ż�հ���\n");//print("\n��ӡǰ��ż�հ���\n");
	
	//��ż�ı���
	NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE,1);//setprintmode(2,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ16x16<<8));   //panxl
	//NDK_PrnStr("\n\nģʽ1,�м��1\n");//print("\n\nģʽ1,�м��1\n");	
	NDK_PrnStr("�м���ż�ո�\n");//print("�м���ż�ո�\n");
	NDK_PrnStr("��  ��   ��������  ���� \r");//print("��  ��   ��������  ���� \r");
	NDK_PrnStr(" �� ��  ������  ���� \r");//print(" �� ��  ������  ���� \r");
	
	//��ż�س���
	NDK_PrnStr("\r");//print("\r");
	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE, 1);//setprintmode(0,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);//setprintfont(HZ16x16<<8);   //panxl
	for (i=1;i<=24;i++)
		NDK_PrnStr("��");	//print("��");
	
	//NDK_PrnStr("\nģʽ0,�м��1\n");//print("\nģʽ0,�м��1\n");	
	NDK_PrnStr("��ӡǰһ���س���\n");//print("��ӡǰһ���س���\n");
	
	NDK_PrnStr("\r\r\r");//print("\r\r\r");
	NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE,1);//setprintmode(2,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);//setprintfont(HZ16x16<<8);   //panxl
	//    print("          \r");
	
	for (i=1;i<=24;i++)
		NDK_PrnStr("��");	//print("��");

	//NDK_PrnStr("\nģʽ2,�м��1\n");//print("\nģʽ2,�м��1\n");	
	NDK_PrnStr("��ӡǰ�����س���\n");//print("��ӡǰ�����س���\n");
	
	NDK_PrnStr("\r\r\r\r");//print("\r\r\r\r");
	NDK_PrnSetMode(PRN_MODE_HEIGHT_DOUBLE,0);//setprintmode(5,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);//setprintfont(HZ16x16<<8);   //panxl
	for (i=1;i<=24;i++)
		NDK_PrnStr("��");	//print("��");

	//NDK_PrnStr("\nģʽ5,�м��1\n");	//print("\nģʽ5,�м��1\n");
	NDK_PrnStr("��ӡǰ�ĸ��س���\n");//print("��ӡǰ�ĸ��س���\n");
	
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//while(getprinterstatus()==TPNOTREADY);
	{
		cls_show_msg("%s, line %d:��ӡ��״̬�쳣!", __FILE__, __LINE__);
		return;                         
	}
				
	NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE,1);//setprintmode(2,1);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ12x16<<8));   //panxl	
	cls_printf("����print����:\n"
				"\\r��������\n���ַ�"	
				"\n���ڴ�ӡ,���Ե�\n");
	//NDK_PrnStr("\nģʽ2,�м��1\n");//print("\nģʽ2,�м��1\n");		
	NDK_PrnStr("DOT8x16 HZ12x16\n");//print("DOT8x16 HZ12x16\n");
	for (i=1;i<=48;i++)		
		NDK_PrnStr("��");	//print("��");			

	NDK_PrnStr("\r\r	");	//print("\r\r	"); \r\r������TAB��,�����POS��������.
	NDK_PrnStr("\r\r "); //print("\r\r "); \r\r�����ǿո�,������ӡ.
	
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־		
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//while(getprinterstatus()==TPNOTREADY);
	{
		cls_show_msg("%s, line %d:��ӡ��״̬�쳣!", __FILE__, __LINE__);
		return;                         
	}						  
				
#if 0 //��NDK�в�����
	//setprintpagelen(0),�����POS���ǽ��д�ӡ��λ�Ĳ���.
	//�˺�����������,����û��ʲôӦ��.
	setprintpagelen(0);//setprintpagelen(300); 
	NDK_PrnSetMode(PRN_MODE_WIDTH_DOUBLE ,0);//setprintmode(3,6);
	NDK_PrnSetFont(PRN_HZ_FONT_12x16,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16+(HZ12x16<<8));   //panxl
	cls_printf("��pagelen���ó�0\n"
				"\n���ڴ�ӡ,���Ե�\n");
	NDK_PrnStr("\nģʽ3,�м��6\n");//print("\nģʽ3,�м��6\n");
	NDK_PrnStr("setprintpagelen(0)\n");//print("setprintpagelen(0)\n");		
	NDK_PrnStr("DOT8x16 HZ12x16\n");	//print("DOT8x16 HZ12x16\n");	
	for (i=1;i<=48;i++)		
		NDK_PrnStr("��");	//print("��");		

	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//while(getprinterstatus()==TPNOTREADY);
	{
		cls_show_msg("%s, line %d:��ӡ��״̬�쳣!", __FILE__, __LINE__);
		return;                         
	}
#endif
	
	return;
}

//��ӡ���ϵ���
void print_landi(void)
{
	/*private & local definition*/
	
	/*process body*/
	NDK_PrnSetFont(PRN_HZ_FONT_16x8,PRN_ZM_FONT_8x8);//setprintfont(DOT8x8+(HZ16x16<<8));//setprintfont(DOT8x8+(HZ24x24<<8));
	NDK_PrnSetMode(PRN_MODE_NORMAL,1);//setprintmode(6,2);
	NDK_PrnStr("���ݵ�أ�3.02V\n");//print("���ݵ�أ�3.02V\n");
	NDK_PrnStr("WELCOME TO POS EQUIPMENT\n");//print("WELCOME TO POS EQUIPMENT\n");	
	NDK_PrnStr("��������������������������������������������\n");//print("��������������������������������������������\n");
	NDK_PrnStr("��������������������������������������������\n");//print("��������������������������������������������\n");
	NDK_PrnSetMode(PRN_MODE_HEIGHT_DOUBLE,1);//setprintmode(4,2);
	NDK_PrnStr("��������������������������������������������\n");//print("��������������������������������������������\n");
	NDK_PrnStr("��������������������������������������������\n");//	print("��������������������������������������������\n");
	NDK_PrnStr("��λ��ӡЧ����\n");//print("��λ��ӡЧ����\n");
	NDK_PrnStr("��λ4(#2)\n");//print("��λ4(#2)\n");
	NDK_PrnStr("||  ||  ||  ||  ||\n");//print("||  ||  ||  ||  ||\n");
	NDK_PrnStr("||  ||  ||  ||  ||\n");//print("||  ||  ||  ||  ||\n");
	NDK_PrnStr("||  ||  ||  ||  ||\n");//print("||  ||  ||  ||  ||\n");
	NDK_PrnStr("�������Ч��\n");//print("�������Ч��\n");
	NDK_PrnStr(" 1 2 3 4 5 6 7 8 9 \n");//print(" 1 2 3 4 5 6 7 8 9 \n");
	NDK_PrnStr("--------------------------------------------------------------------\n");
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}

//��ӡ���ֿհ���ֽ������
//�߶˰�׿��Ʒʹ�ô�ӡ�ű�������֤ 20170609����
#if ANDIROD_ENABLE
void print_whitebill(void)
{
	/*private & local definition*/
	int ret = -1;
	//���ڴ�ӡ�ű���������һ�����ݺ�����NDK_PrnStart,�ʲ��ֳܷɼ���,����һ�������ݴ���,�������ָо����ٵ�����
	char buf_str[] = "!hz l\n !asc l\n !yspace 15\n !gray 10\n*text c ͨݸ���\n!hz s\n !asc s\n !yspace 5\n !gray 5\n*line \n*text l �̻����                             �����Ʊ���\n*line \n!hz n\n !asc n\n !yspace 10\n !gray 5\n*text l �̻�����:�����������޹�˾���ݷֹ�˾\n*text l �̻����:899330160120021\n*text l �ն˱��:05315830\n*text l ��������:�ֽ�����\n*text l ��������:����\n*text l ����ʱ��:2017/02/09 15:51:55\n*text l ֧�����:\n!hz l\n !asc l\n !yspace 15\n !gray 10\n*text l     RMB108\n!hz n\n !asc n\n !yspace 10\n !gray 5\n*text l ����״̬:����ɹ�\n*text l ������:9847355443331596288\n *barcode c 29847355443331596288\n!hz n\n !asc s\n !yspace 5\n !gray 5\n *text l ΢֧��������:ͨݸ���\n";
	
	/*process body*/
	if((ret = NDK_Script_Print(buf_str,strlen(buf_str)))!= NDK_OK)
	{
		cls_show_msg("%s, line %d:�ű���ӡʧ��(%d)", __FILE__, __LINE__,ret);
		return; 						
	}
	return;
}
#else
void print_whitebill(void)
{
	/*private & local definition*/
	
	/*process body*/
	NDK_PrnSetMode(PRN_MODE_NORMAL,1);
	NDK_PrnSetFont(PRN_HZ_FONT_32x32,PRN_ZM_FONT_16x32);
	NDK_PrnSetForm(0,0,15);
	NDK_PrnSetGreyScale(5);
	NDK_PrnStr("        ͨݸ���        \n");

	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16);
	NDK_PrnSetForm(0,0,5);
	NDK_PrnSetGreyScale(3);
	NDK_PrnStr("------------------------------------------------\n");
	NDK_PrnStr("�̻����                             �����Ʊ���\n");
	NDK_PrnStr("------------------------------------------------\n");

	NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_12x24A);
	NDK_PrnSetForm(0,0,10);
	NDK_PrnSetGreyScale(3);
	NDK_PrnStr("�̻�����:�����������޹�˾���ݷֹ�˾\n");
	NDK_PrnStr("�̻����:899330160120021\n");
	NDK_PrnStr("�ն˱��:05315830\n");
	NDK_PrnStr("��������:�ֽ�����\n");
	NDK_PrnStr("��������:����\n");
	NDK_PrnStr("����ʱ��:2017/02/09 15:51:55\n");
	NDK_PrnStr("֧�����:\n");

	NDK_PrnSetFont(PRN_HZ_FONT_32x32,PRN_ZM_FONT_16x32A);
	NDK_PrnSetForm(0,0,15);
	NDK_PrnSetGreyScale(5);
	NDK_PrnStr("    RMB 108\n");

	NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_12x24A);
	NDK_PrnSetForm(0,0,10);
	NDK_PrnSetGreyScale(3);
	NDK_PrnStr("����״̬:����ɹ�\n");
	NDK_PrnStr("������:9847355443331596288\n");
	
	//scriptBuilder.append("*barcode c ").append("29847355443331596288").append("\n");
	NDK_PrnImage(128,64,0,CCB_BMP);
	
	NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_12x12);
	NDK_PrnSetForm(0,0,5);
	NDK_PrnSetGreyScale(3);
	NDK_PrnStr("\n΢֧��������:ͨݸ���\n");
	
	NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	return;
}
#endif
//��ӡ�¹�������
void print_xinguodu(void)
{
	/*private & local definition*/
	char szTmp[256] = {0};	
	struct tm stTestTime;//struct postime stTestTime;
	
	/*process body*/
	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE, 1);//setprintmode(0,0);
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_6x8);//setprintfont(HZ16x16<<8|DOT6x8);
	NDK_PrnSetFont(0,PRN_ZM_FONT_6x8);//setprintfont(DOT6x8);
	NDK_PrnStr("\r\r");//print("\r\r");
	NDK_PrnStr("�����в�ɽ�����Է��ε�\r\r");//print("�����в�ɽ�����Է��ε�\r\r");
	NDK_PrnStr("            309350156911410\r");//print("            309350156911410\r");
	NDK_PrnStr("       11100495         01\r\r");//print("       11100495         01\r\r");
	NDK_PrnSetFont(0,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16);
	NDK_PrnStr("   622200*********7950\r\r");//print("   622200*********7950\r\r");
	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_6x8);//setprintfont(HZ16x16<<8|DOT6x8);
	NDK_PrnSetFont(0,PRN_ZM_FONT_6x8);//setprintfont(DOT6x8);
	NDK_PrnStr("     01020000    ��ҵ����\r\r");//print("     01020000    ��ҵ����\r\r");
	NDK_PrnStr("   ����/SALE      2000/00\r\r");//print("   ����/SALE      2000/00\r\r");
	NDK_PrnStr("   000003          000934\r\r");//print("   000003          000934\r\r");
	NDK_PrnStr("   012345\r");//print("   012345\r");
	NDK_SysGetPosTime(&stTestTime);
	sprintf(szTmp,"\n\n %4d��%02d��%02d��   %02dʱ%02d��%d��",
						stTestTime.tm_year+1900,
						stTestTime.tm_mon+1,
						stTestTime.tm_mday,			
						stTestTime.tm_hour,
						stTestTime.tm_min,
						stTestTime.tm_sec);	
	NDK_PrnStr(szTmp);//print(szTmp);
	NDK_PrnStr("   151019019346\r");
	NDK_PrnSetFont(0,PRN_ZM_FONT_8x16);//setprintfont(DOT8x16);
	NDK_PrnStr("   RMB 0.01\r\r");
	NDK_PrnStr("   RMB 0.01");
	NDK_PrnStart();//print("\f");
	return;
}

void print_signlfont(void)
{
	/*private & local definition*/
#include	"dotfont.h"
#include	"hzfont.h"

	int ret = -1, i = 0, j = 0, k = 0, l = 0;
	char tmp[100]={0};
	
	for(k= 32;k<=126;k++,l++)
		tmp[l]=k;//��ֵASCII��ֵ
		
	/*process body*/
	for(i=0;i<SZ_ARRAY(hzfont);i++)
	{
		if((ret=NDK_PrnSetFont(hzfont[i],PRN_ZM_FONT_8x16))!=NDK_OK)//if(setprintfont(hzfont[i]<<8)!=SUCC)
		{
			cls_show_msg("%s, line %d:���ô�ӡ����[%s]ʧ��(%d)", __FILE__, __LINE__, hzfontstr[i], ret);
			return;
		}
		if(print_Ex("HZ:[%s]�´�½\n", hzfontstr[i])!=SUCC)
		{
			cls_show_msg("%s, line %d:������ʧ��\n", __FILE__, __LINE__);
			return;                        
		}
		NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	}
	//NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡ��״̬�쳣!", __FILE__, __LINE__);
		return;                         
	}
	
	for(j=0;j<SZ_ARRAY(dotfont);j++)
	{
		if((ret=NDK_PrnSetFont(PRN_HZ_FONT_16x16,dotfont[j]))!=NDK_OK)//if(setprintfont(dotfont[j])!=SUCC)
		{
			cls_show_msg("%s, line %d:���ô�ӡ����[%s]ʧ��(%d)", __FILE__, __LINE__, dotfontstr[j], ret);
			return;
		}
		if(print_Ex("�ַ�:[%s]%s\n", dotfontstr[j], tmp)!=SUCC)
		{
			cls_show_msg("%s, line %d:������ʧ��\n", __FILE__, __LINE__);
			return;                        
		}
		NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	}
	//NDK_PrnStart();//print("\f");//\f�Ǵ�ӡ���������Ա�־
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
	{
		cls_show_msg("%s, line %d:��ӡ��״̬�쳣!", __FILE__, __LINE__);
		return;                         
	}
	return;
}

//���������ڴ�ӡѹ�����Լ��Դ�ӡΪ����Ľ������
//��Ҫ��ע����ִ�гɹ���ʵ��Ч��
//title:���Ա���˵��;
//func:����Ĳ��Զ���(��Ҫ���ڽ������,���Ǻ����б�,���û����з�װ��func�м���),����������ʱ,��NULL
void prn_press(const char *title, PF func)
{
	/*private & local definition*/
#define	DEFAULT_CNT_STR	"1"//"200"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(200)
	int cnt = DEFAULT_CNT_VLE, bak = 0, ret = 0;
	uint len = 0;
	char CntStr[8] = DEFAULT_CNT_STR;

	PF pfunc[] = {print_triangle_384, print_testpage, print_rand, print_guo, print_fill, print_bill, 
				print_verticalbill, print_blank, print_font, print_image_CCB128, print_enter, print_compress, print_landi, print_xinguodu};
	char *func_info[] = {"���ǿ�", "����ҳ", "�����", "����ҳ", "��䵥", "Ʊ��", "������", 
					"�հ׵�", "ģʽ�������", "ͼ�δ�ӡ", "�س�", "ѹ������", "���ϵ���", "�¹�������"};
	int id = 0;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
	if ((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	//ע��:��λ�������ڽ��뺯��ѹ��ǰ��һ��,��֮��Ĳ�ͬ��ӡѹ�������в���(�������䲻����λ),�ԼӴ�ѹ��
	//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)//if (FAIL==clrprintbuf())
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ��ʹ��ʼ��������Ч modify 20131122 jiangym
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
		if (ESC==cls_show_msg1(2, "%s������...\n��ʣ%d��,��ע���ӡЧ��,ESC�жϲ���", title, cnt))
			break;
		cnt--;

		for(id=0; id<SZ_ARRAY(pfunc); id++)
		{
			if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ��״̬�쳣!(��ǰ����:%s)", __FILE__, __LINE__, func_info[id]);
				return;                         
			}
			pfunc[id]();
			if(func!=NULL)//�������ӡ���н������
				func();
			if(wait_anykey(1)==ESC)
				goto OUT;
		}
	}

OUT:
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������,��ִ�д���Ϊ%d", title, bak-cnt);
	return;
}
#endif

#if MAG_ENABLE
//TK2_3�ز�,TK1��ѡ
//��������KEYIN��ת��ΪSTRIPE(�ݲ�֧��)
//ˢ���ĵ�����,�ͷ����ĵ��ĺ�(����0),�ʲ�����>0�����ɹ�����ж�����
int MagcardReadTest(int select_TK, int IsDisplayed, int waittime)
{
	/*private & local definition*/
	int ret = 0, errcode = 0, flg = 0x00;
	char buf2[MAXTRACKLEN] = {0}, buf3[MAXTRACKLEN] = {0}, buf1[MAXTRACKLEN] = {0}/*, c = 0*/;

	/*process body*/
	memset(buf2, 0, sizeof(buf2));
	memset(buf3, 0, sizeof(buf3));
	memset(buf1, 0, sizeof(buf1));

	cls_printf("��ˢ��(%02ds)--->>\n", waittime);
	ret = readcard3(select_TK, waittime, buf2, buf3, buf1, &errcode);
    //ע��:������Ϣ����IsDisplayed����.ֻҪ����,������Ϣ����ʾ!
	switch(ret)
	{
	case FAIL:
		cls_show_msg1(2, "%s, line %d:ˢ��ʧ��", __FILE__, __LINE__);
		break;
	case NDK_ERR_QUIT://QUIT:
		cls_show_msg1(2, "%s, line %d:�û�ȡ��ˢ��", __FILE__, __LINE__);
		break;
	case NDK_ERR_TIMEOUT://TIMEOUT:
		cls_show_msg1(2, "%s, line %d:ˢ����ʱ", __FILE__, __LINE__);		
		break;
	case STRIPE:
		//TK1
		if((select_TK&TK1)==TK1)
		{
			if(buf1[0]==0x7f)//������
			{
				cls_show_msg1(g_keeptime, "%s, line %d:1��������![0x7F]\nʵ��:%2x\nerrcode:0x%x\n", __FILE__, __LINE__, buf1[0], errcode);
				flg |= TK1;
			}
			else if(buf1[0]==0x7e)//������
			{
				cls_show_msg1(g_keeptime, "%s, line %d:1��������![0x7E]\nʵ��:%2x\nerrcode:0x%x\n", __FILE__, __LINE__, buf1[0], errcode);
			}
			else 
			{
				NDK_ScrClrs();//clrscr();
				if((errcode&MAGCARD_TK1_LRC_ERR)==MAGCARD_TK1_LRC_ERR)//У�����
				{    
					NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE, NULL);//settextattr(REVERSE_TEXT);//���Դ���ֵ
					flg |= TK1;
				}
				if(IsDisplayed || (flg&TK1)==TK1)
				{
					show_msg1(2, "1�ŵ�����(%d��):%s\n", strlen(buf1), buf1);//printf("1�ŵ�����(%d��):\n", strlen(buf1));
					//NDK_ScrDispString(0, 0, buf1, 1);
					//NDK_ScrRefresh();
					//wait_anykey(1);//getch();
				}
			}
		}
		//TK2
		if(buf2[0]==0x7f)//������
		{			
			cls_show_msg1(g_keeptime, "%s, line %d:2��������![0x7F]\nʵ��:%2x\nerrcode:0x%x\n", __FILE__, __LINE__, buf2[0], errcode);
			flg |= TK2;
		}
		else if(buf2[0]==0x7e)//������
		{
			cls_show_msg1(g_keeptime, "%s, line %d:2��������![0x7E]\nʵ��:%2x\nerrcode:0x%x\n", __FILE__, __LINE__, buf2[0], errcode);
		}
		else
		{
			NDK_ScrClrs();//clrscr();
			if((errcode&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR)//У�����
			{    
				NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE, NULL);//settextattr(REVERSE_TEXT);//���Դ���ֵ
				flg |= TK2;
			}
			if(IsDisplayed || (flg&TK2)==TK2)
			{
				show_msg1(2, "2�ŵ�����(%d��):%s\n", strlen(buf2), buf2);//printf("2�ŵ�����(%d��):\n", strlen(buf2));
				//NDK_ScrDispString(0, 0, buf2, 1);
				//NDK_ScrRefresh();
				//wait_anykey(1);//getch();
			}
		}
		//TK3
		if(buf3[0]==0x7f)//������
		{			
			cls_show_msg1(g_keeptime, "%s, line %d:3��������![0x7F]\nʵ��:%2x\nerrcode:0x%x\n", __FILE__, __LINE__, buf3[0], errcode);
			flg |= TK3;
		}
		else if(buf3[0]==0x7e)//������
		{
			cls_show_msg1(g_keeptime, "%s, line %d:3��������![0x7E]\nʵ��:%2x\nerrcode:0x%x\n", __FILE__, __LINE__, buf3[0], errcode);
		}
		else
		{
			NDK_ScrClrs();//clrscr();
			if((errcode&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR)//У�����
			{
				NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE, NULL);//settextattr(REVERSE_TEXT);//���Դ���ֵ
				flg |= TK3;
			}
			if(IsDisplayed || (flg&TK3)==TK3)
			{
				show_msg1(2, "3�ŵ�����(%d��):%s\n", strlen(buf3), buf3);//printf("3�ŵ�����(%d��):\n", strlen(buf3));
				//NDK_ScrDispString(0, 0, buf3, 1);
				//NDK_ScrRefresh();
				//wait_anykey(1);//getch();
			}
		}

		NDK_ScrClrs();//clrscr();
		ret |= flg<<8;
		break;
	default:
		cls_show_msg1(g_keeptime, "%s, line %d:ˢ������δ֪��:0x%x", __FILE__, __LINE__, ret);
		break;  
	}
	
	return ret;
}

int systest_MagCard(void){return MagcardReadTest(TK2_3, TRUE, 0);}
#endif

#if !(defined ME15CHX||defined ME15B||defined ME66)
void set_snd_packet(Packet *packet)
{
	/*private & local definition*/
	int nKeyin = 0, j = 0, ret = -1;
	uint len = 0;
	char tbuf[16] = {0}, *ptr = packet->header;
	LinkType type = getlinktype();

	/*process body*/
	cls_printf("���ð��ķ��ʹ���:\n");//һ���,��Ϊ���ʹ���==��������������+1
	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
	if(g_SequencePressFlag)
	{
		packet->lifecycle= g_CycleTime;//GetCycleValue();
	}
	else
	{
		strcpy(tbuf, "200"/*"300"*/);//zhangnw20110113 50->200,��Ϊ��Щ�������100�κ����
		if ((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len, INPUTDISP_OTHER, 10, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
			strcpy(tbuf, "200"/*"300"*/);
		
		//��Ӧ"����ǰ��update"if((packet->lifecycle=atoi(tbuf)-1)<0)
		if((packet->lifecycle=atoi(tbuf))<0)
			return;//�˰�����,ֱ�ӷ���(�����)
	}
	packet->forever = FALSE;
	
	cls_printf("���ó�ʼ����:\n");
	if(g_SequencePressFlag)
	{
		packet->len = packet->orig_len = g_PackLen;//GetCommPackLen();
	}	
	else
	{
		strcpy(tbuf, "13000");//zhangnw20110225����1000->10000,Ĭ�ϰ����İ�����// 20180905 ����10000->13000 
		if ((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len,INPUTDISP_OTHER, 10, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
			strcpy(tbuf, "13000");		
		packet->len = packet->orig_len = atoi(tbuf);
	}
	
	if(packet->len>PACKMAXLEN)
		packet->len = packet->orig_len = PACKMAXLEN;
	if((type==ASYN || type==SYNC) && packet->len<5)
		packet->len = packet->orig_len = 5;
	if(type==SYNC && packet->len>SDLCPCKTMAXLEN)
		packet->len = packet->orig_len = SDLCPCKTMAXLEN;
	if(type==ASYN && packet->len>ASYNPCKTMAXLEN)
		packet->len = packet->orig_len = ASYNPCKTMAXLEN;
	if(type==ETH && packet->len>ETHPACKMAXLEN)
		packet->len = MIN(packet->orig_len, ETHPACKMAXLEN);//����ETH�ﵽ��󳤶�ʱ,����������õĳ���(�еͶ�������) 20180806 modify
	if((type==GPRS||type==CDMA||type==TD)&& packet->len>WLMPACKMAXLEN)
		packet->len = MIN(packet->orig_len, WLMPACKMAXLEN);//����WLM�ﵽ��󳤶�ʱ,����������õĳ���(�еͶ�������) 20180806 modify
#if 0
	if( (type==SERIAL) && packet->len>BUFSIZE_SERIAL)
		packet->len = packet->orig_len = BUFSIZE_SERIAL;
#endif
	if( (type==BT) && packet->len>BUFSIZE_BT-MIN_BTCMD_LEN)//ʵ�ʿɷ��͵����ݳ���ΪBUFSIZE_BT-��Сָ���MIN_BTCMD_LEN
		packet->len = packet->orig_len = BUFSIZE_BT-MIN_BTCMD_LEN;

	if(g_SequencePressFlag)
	{
		if(ENTER==cls_show_msg1(5,"�����Ƿ����?\n"
								"[ȷ��]��[����]��\n"))
			packet->IsLenRec = TRUE;
		else
			packet->IsLenRec = FALSE;
	}
	else
	{
		if(ENTER==cls_show_msg("�����Ƿ����?\n"
							"[ȷ��]��[����]��\n"))
			packet->IsLenRec = TRUE;
		else
			packet->IsLenRec = FALSE;
	}
	
	do
	{
		if(g_SequencePressFlag)
		{
			nKeyin = cls_show_msg1(5,"_���÷�������_\n"
									"1.�̶�����\n"
									"2.�������\n");
		}
		else
		{
			nKeyin = cls_show_msg("_���÷�������_\n"
								"1.�̶�����\n"
								"2.�������\n");
		}
#define MAXMENUITEM ('2')		
		
		switch (nKeyin)
		{
		case '1':
			packet->IsDataRnd = FALSE;
			cls_printf("���ù̶�����:\n");
			strcpy(tbuf, "0");
			if((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len,INPUTDISP_OTHER, 20, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
				strcpy(tbuf, "0");
			memset(packet->header, atoi(tbuf), packet->len);
			break;
		case 0:  /*��ʱĬ�������*/
		case MAXMENUITEM://'2':
			packet->IsDataRnd = TRUE;
			for (j=0; j<packet->len; j++)
				*ptr++ = rand()%256;
			break;
		case ESC:
			return;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

	//��һ����������
#if SDLC_ENABLE
	if (type==SYNC)//\x60\x80\x00\x80\x00
		update_SDLC_packet(packet->header, packet->len);
#endif
#if ASYN_ENABLE
	if (type==ASYN)
		update_ASYN_packet(packet->header, packet->len);
#endif
#if BT_ENABLE
	if(type==BT)
		update_BT_packet(packet->header, packet->len);
#endif	
	return;
}

//ÿupdateһ��,���������Զ���1
int  update_snd_packet(Packet *packet)
{
	/*private & local definition*/
	int j = 0;
	char *ptr = packet->header;
	LinkType type = getlinktype();

	/*process body*/
	if(packet->forever==FALSE && packet->lifecycle<=0)
		return FAIL;

	if(packet->IsLenRec)
		packet->len++;

	if((packet->len>PACKMAXLEN))
		packet->len = MIN(packet->orig_len, PACKMAXLEN);//�ﵽ��󳤶�ʱ,����������õĳ���
#if 0		
	if( (type==SERIAL) && packet->len>BUFSIZE_SERIAL)
		packet->len = MIN(packet->orig_len, BUFSIZE_SERIAL);//�ﵽ��󳤶�ʱ,����������õĳ���
#endif	
	if(type==ETH && packet->len>ETHPACKMAXLEN)
		packet->len = MIN(packet->orig_len, ETHPACKMAXLEN);//����ETH�ﵽ��󳤶�ʱ,����������õĳ���(�еͶ�������) 20180806 modify
	if((type==GPRS||type==CDMA||type==TD)&& packet->len>WLMPACKMAXLEN)
		packet->len = MIN(packet->orig_len, WLMPACKMAXLEN);//����WLM�ﵽ��󳤶�ʱ,����������õĳ���(�еͶ�������) 20180806 modify
	if(type==BT && packet->len>BUFSIZE_BT-MIN_BTCMD_LEN)
		packet->len = MIN(packet->orig_len, BUFSIZE_BT-MIN_BTCMD_LEN);//�ﵽ��󳤶�ʱ,����������õĳ���
	if(type==SYNC && packet->len>SDLCPCKTMAXLEN)
		packet->len = MIN(packet->orig_len, SDLCPCKTMAXLEN);//�ﵽ��󳤶�ʱ,����������õĳ���
	if(type==ASYN && packet->len>ASYNPCKTMAXLEN)
		packet->len = MIN(packet->orig_len, ASYNPCKTMAXLEN);//�ﵽ��󳤶�ʱ,����������õĳ���
	if(packet->IsDataRnd)
	{
		for (j=0; j<packet->len; j++)
			*ptr++ = rand()%256;
	}

	//��һ����������
#if SDLC_ENABLE
	if (type==SYNC)//\x60\x80\x00\x80\x00
		update_SDLC_packet(packet->header, packet->len);
#endif
#if ASYN_ENABLE
	if (type==ASYN)
		update_ASYN_packet(packet->header, packet->len);
#endif
#if BT_ENABLE
	if(type==BT)
		update_BT_packet(packet->header, packet->len);
#endif
	if(packet->forever==FALSE)
		packet->lifecycle--;

	return SUCC;
}

//����ͳһ���ܴ�СΪPACKMAXLEN�Ļ�����
//������ֻ����һ������,Ҫͨ��set_snd_packet������,�ſ�����ʹ��
void init_snd_packet(Packet *packet, char *buf)
{
	/*private & local definition*/

	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len = 0;
	packet->lifecycle = 0;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = FALSE;
	return;
}
#else
void set_snd_packet(Packet *packet){return;}
int update_snd_packet(Packet *packet){return SUCC;}
void init_snd_packet(Packet *packet, char *buf){return;}
#endif
#endif

#if !(defined ME15CHX||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
#if (SDLC_ENABLE || ASYN_ENABLE)
int mdm_send(char *buf, int len)
{
	/*private & local definition*/
	int ret = 0;
	LinkType type = getlinktype();
	
	/*process body*/
	cls_printf("���ݷ�����...\n"
				"POS->%s", MDMDialStr);
	
	if(type==ASYN||type==SYNC)
		return ((ret=NDK_MdmWrite(buf, len))==NDK_OK)?len:ret;
	else 
		return FAIL;
}

int mdm_rev(char *buf, int len, int timeout)
{
	/*private & local definition*/
	int ret = 0;
	uint rlen = len;
	LinkType type = getlinktype();
	
	/*process body*/
	cls_printf("���ݽ�����...\n"
				"POS<-%s", MDMDialStr);
	
	if(type==ASYN)
	{
		if((ret=getmodemreadlenN())<=0)
			return ret;

		return ((ret=NDK_MdmRead(buf, &rlen, timeout))==NDK_OK)?rlen:ret;
	}
	else if(type==SYNC)
	{
		rlen = 0;//��ֹSDLC_readû�ж�rlen����0
		return ((ret=NDK_MdmRead(buf, &rlen, timeout))==NDK_OK)?rlen:ret;
	}
	else
		return FAIL;
}

void mdm_clrportbuf_all(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();
	
	/*process body*/
	if (type==ASYN||type==SYNC)
		NDK_MdmClrbuf();
	else
		return;
}

void mdm_reset(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();

	/*process body*/
	if(type==ASYN||type==SYNC)
		NDK_MdmReset();
	else
		return;
}

int mdm_init(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();
	//int ret = 0;

	/*process body*/
	if(type==ASYN)
		return NDK_MdmAsynInit(MDMPatchType);
	else if(type==SYNC)
		{
#if defined SP50||defined SP80
			if(sdlc_init_flag)
				return NDK_MdmSdlcSingleForceInit(MDMPatchType, g_sdlc_bps);
			else
#endif
				return NDK_MdmSdlcInit(MDMPatchType);
		}
	else
		return FAIL;
}

int mdm_dial_comm(char *dialstr, Packet *sendpacket, int rst, float *call_time_once, float *hangdowntime_once)
{
	/*private & local definition*/
	int send_len = 0, rec_len = 0, ret = 0;
	char rbuf[PACKMAXLEN] = {0};
	*call_time_once = *hangdowntime_once = 0.0;
	
   	/*process body*/
	if(rst==TRUE)
	{
		//��λMODEM
		cls_printf("��λMODEM��...");
		mdm_reset();
	}
	//��ʼ��MODEM
	cls_printf("��ʼ��MODEM��...");
	if((ret=mdm_init())!=SUCC)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��ʼ��MODEMʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;//FAIL;//ʹ��ret���԰�if�еĺ�������ֵ��һ�������ϲ�
	}
	//����
	cls_printf("MODEM��%s��...", dialstr);
	lib_StartStopwatch();
	if((ret=mdm_dial(dialstr))!=SUCC)
	{
		mdm_hangup();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:MODEM��%sʧ��", __FILE__, __LINE__, dialstr);
		return ret;//FAIL;
	}
	*call_time_once = lib_ReadStopwatch();
	//����ͨѶ
	cls_printf("MODEM����ͨѶ��...");
	//��������
	if((send_len=mdm_send(sendpacket->header, sendpacket->len))!=sendpacket->len)
	{
		mdm_hangup();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __FILE__, __LINE__, send_len, sendpacket->len);
		return FAIL;//���ﲻ����send_len,����FAIL����if�ж��е��߼�ֵΪ��(�߼��ж�ʧ��)!
	}
	//��������
	if((rec_len=mdm_rev(rbuf, sendpacket->len, 20))!=sendpacket->len)
	{
		mdm_hangup();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __FILE__, __LINE__, rec_len, sendpacket->len);
		return FAIL;
	}
	//�Ƚ�����
	if(MemCmp(sendpacket->header, rbuf, sendpacket->len))
	{
		mdm_hangup();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:����У��ʧ��", __FILE__, __LINE__);
		return FAIL;
	}
		
	//�Ҷ�
	cls_printf("MODEM�Ҷ���...");
	lib_StartStopwatch();
	if((ret=mdm_hangup())!=SUCC)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:MODEM�Ҷ�ʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	*hangdowntime_once = lib_ReadStopwatch()-(getlinktype()==SYNC?SLEEP_SDLC_HANGUP:0);

	return SUCC;
}

int MDM_NetDown(int nMdmType)
{
	EM_MDMSTATUS MdmStatus = 0;
	
	Local2Global(nMdmType);
	setlinktype(nMdmType);

	if(nMdmType==ASYN)
	{
		if(__LinkDown_ASYN()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:�Ͽ���·ʧ��", __LINE__);
			return FAIL;
		}
	}
	else
	{
		//�Ҷ�״̬����
		if(NDK_MdmCheck(&MdmStatus)!=NDK_OK|| MdmStatus != MDMSTATUS_CONNECT_AFTERPREDIAL)
		{
			__LinkDown_SDLC();
			cls_show_msg1(g_keeptime, "line %d:NDK_MdmCheck��������(MdmStatus=%d)", __LINE__, MdmStatus);
			return FAIL;
		}
		if(__LinkDown_SDLC()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:�Ͽ���·ʧ��", __LINE__);
			return FAIL;;
		}	
		if(NDK_MdmCheck(&MdmStatus)!=NDK_OK|| MdmStatus != MDMSTATUS_NOPREDIAL)
		{
			cls_show_msg1(g_keeptime, "line %d:NDK_MdmCheck��������", __LINE__);
			return FAIL;
		}
	}
	return SUCC;
}
#endif

//�ṩ��װ�������շ�����
#if (NDKSOCK_ENABLE ||NDKSSL_ENABLE)
int sock_recv(uint s, char  *buf, int len, int timeout)
{
	/*private & local definition*/

	/*process body*/
	cls_printf("���ݽ�����...\n"
				"%s:%d<-%s:%d\n", 
				netsetting.LocalIP.Str, netsetting.LocalPort, netsetting.SvrIP.Str, (netsetting.socktype==SOCK_NDKUDP)?netsetting.SvrUdpPort:netsetting.SvrTcpPort);
	if(netsetting.socktype==SOCK_NDKTCP||netsetting.socktype==SOCK_NDKUDP)
		return NDKTransReceiveN(s, buf, len, timeout);
#if NDKSSL_ENABLE
	else if(netsetting.socktype==SOCK_NDKSSL)
		return NDKSSLReceiveN((NDK_HANDLE)s, buf, len);
#endif
	else
		return FAIL;
}

int sock_send(uint s, char  *buf, int len, int timeout)
{
	/*private & local definition*/

	/*process body*/
	cls_printf("���ݷ�����...\n"
				"%s:%d->%s:%d\n", 
				netsetting.LocalIP.Str, netsetting.LocalPort, netsetting.SvrIP.Str, (netsetting.socktype==SOCK_NDKUDP)?netsetting.SvrUdpPort:netsetting.SvrTcpPort);
	if(netsetting.socktype==SOCK_NDKTCP||netsetting.socktype==SOCK_NDKUDP)
		return NDKTransSendN(s, buf, len, timeout);
#if NDKSSL_ENABLE
	else if(netsetting.socktype==SOCK_NDKSSL)
		return NDKSSLSendN((NDK_HANDLE)s, buf, len);
#endif
	else
		return FAIL;
}
#endif

#if (WLM_ENABLE && (NDKSOCK_ENABLE || NDKSSL_ENABLE))
int wireless_dial_comm(Packet *sendpacket, int flag)
{
	/*private & local definition*/
	uint h_tcp = -1;
	char rbuf[PACKMAXLEN] = {0};
	int slen = 0, rlen = 0, ret = 0, timeout = SO_TIMEO;
	
	/*process body*/
	//����ǰ��
	if((ret=NetUp())!=SUCC)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:NetUpʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	if(flag==RESET_PPPOPEN)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "PPP�Ѵ�(TCPδ��)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	if((ret=TransUp(&h_tcp))!=SUCC)
	{
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:TransUpʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	if(flag==RESET_TCPOPEN)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "TCP�Ѵ�(PPP�Ѵ�)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	//��������
	if((slen=sock_send(h_tcp, sendpacket->header, sendpacket->len, timeout))!=sendpacket->len)
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __FILE__, __LINE__, slen, sendpacket->len);
		return FAIL;
	}
#if 0//K21_ENABLE
	cls_show_msg1(5,"�����Ҫ���Ժ�̨�ҶϺ������յ������������ݵĹ���,����5���ڹر�TCP��̨");
#endif
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, sendpacket->len, timeout))!=sendpacket->len)
	{
		TransDown(h_tcp);//TransDown(netsetting.socktype, h_tcp);
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __FILE__, __LINE__, rlen, sendpacket->len);
		return FAIL;
	}
	if(MemCmp(sendpacket->header, rbuf, rlen))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:У��ʧ��", __FILE__, __LINE__);
		return FAIL;
	}	
	if ((ret=TransDown(h_tcp))!=SUCC)
	{
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:TransDownʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	if(flag==RESET_TCPCLOSE)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "TCP�ѹر�(PPPδ�ر�)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	NetDown();
	if(flag==RESET_PPPCLOSE)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "PPP�ѹر�(TCP�ѹر�)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	return SUCC;
}
#endif

#if RFID_ENABLE
#if K21_ENABLE //�Ͷ�ƽ̨��Ʒ��ʱ��֧��felica
int NDK_RfidFelicaApdu(int nSendlen, uchar *psSendbuf, int *pnRecvlen, uchar *psRecebuf){return NDK_ERR;}
int NDK_RfidFelicaPoll(uchar *psRecebuf,int *pnRecvlen){return NDK_ERR;}
#endif
//ע:������һ��ֻ�ڲ���������ʹ��һ�μ���,�ڲ�����һ�㲻����
int RFID_init(SMART_t type)
{
	/*private & local definition*/
	//uchar sta = 0;

	/*process body*/
	//NDK_RfidInit(NULL);//NDK_RfidInit(&sta);
	//NDK_RfidOpenRf();
	switch(type)
	{
	case CPU_A:
	case MIFARE_1:
	case CPU_B:
	case FELICA:
	case MIFARE_0:
		NDK_RfidInit(NULL);//NDK_RfidInit(&sta);
		NDK_RfidOpenRf();
		break;
#if ISO15693_ENABLE
	case ISO15693:
		NDK_ISO15693_init();
		break;
#endif
	default:
		return FAIL;
		break;
	}
	
	switch(type)
	{
	case CPU_A:
	case MIFARE_1:
		NDK_RfidPiccDeactivate(0);//icpowerdown(RFCARD);//����
		return NDK_RfidPiccType(RFCARD_TYPE_A);//SetICType(0xff, RFCARD_TYPE_A);
		break;
	case CPU_B:
		NDK_RfidPiccDeactivate(0);//icpowerdown(RFCARD);//����
		return NDK_RfidPiccType(RFCARD_TYPE_B);//SetICType(0xff, RFCARD_TYPE_B);
		break;
	case FELICA:
		//FELICA������Ҫ����Ѱ������
		NDK_RfidPiccDeactivate(0);//icpowerdown(RFCARD);//����
		return SUCC;
		break;
	case MIFARE_0:
		//����Ҫ����Ѱ������NDK_MifareActive  ���������
		NDK_RfidPiccDeactivate(0);//icpowerdown(RFCARD);//����
		return SUCC;
		break;
#if ISO15693_ENABLE
	case ISO15693:
		return SUCC;
		break;
#endif
	default:
		return FAIL;
		break;
	}
}

//��������϶���if�����Ǹ������������,Ӧ����ʾ��Ϣ,������ֱ��return���ϲ�,���������ⶨλ
int RFID_detect(SMART_t type, uchar *UID, int *pUIDlen)
{
	/*private & local definition*/
	uchar /*out[MAXLEN_DATA] = {0}, */SAK[LEN_SAK] = {0};
	int ret = 0;
	//uchar picctype = 0;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
#if ISO15693_ENABLE
	uint cardcount = 0, maxcards = 20;
#endif

	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
		if((ret=NDK_RfidPiccDetect(NULL))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sѰ��ʧ��(%d)",__FILE__ , __LINE__, card[type], ret);
			return (ret!=NDK_OK)?ret:FAIL;
		}
		return SUCC;
		break;
	case MIFARE_1:
		if((ret=NDK_M1Request(REQA, pUIDlen, UID))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sѰ��ʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}
		if((ret=NDK_M1Anti(pUIDlen, UID))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s����ͻʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}
		if((ret=NDK_M1Select(*pUIDlen, UID, SAK))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sѡ��ʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}
		return SUCC;
		break;
	case FELICA:
		//FELICA��Ѱ������
		//int NDK_RfidFelicaPoll(uchar *psRecebuf,int *pnRecvlen)
		if((ret=NDK_RfidFelicaPoll(UID, pUIDlen))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sѰ��ʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}
		return SUCC;
		break;
#if M0_ENABLE 
	case MIFARE_0:
		//M0��Ѱ������
		if((ret=NDK_MifareActive(REQA, UID, pUIDlen,SAK))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sѰ��ʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}
		return SUCC;
		break;	
#endif		
#if ISO15693_ENABLE
	case ISO15693:
		if((ret=NDK_ISO15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s����ͻʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}
		if((ret=NDK_iso15693SelectPicc(cards[0])) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sѡ��ʧ��(%d)",__FILE__ ,  __LINE__, card[type], ret);
			return ret;
		}	
		return SUCC;
		break;
#endif
	default:
		return FAIL;
		break;
	}	
}

//�˺����������Ǹ������Ĳ���������Ѱ��ʧ�ܲ������Ϸ��أ������ڳ�ʱ��Χ����ѯѰ��
int RFID_detect_poll(SMART_t type, uchar *UID, int *pUIDlen,int nTimeOut)
{
	/*private & local definition*/
	uchar /*out[MAXLEN_DATA] = {0}, */SAK[LEN_SAK] = {0};
	int ret = 0;
	//uchar picctype = 0;
	time_t oldtime = 0, diff = 0 ;
#if ISO15693_ENABLE
	uint cardcount = 0, maxcards = 20;
#endif

	/*process body*/

	oldtime=time(NULL);
	while((diff=time(NULL)-oldtime)<nTimeOut)
	{
		NDK_SysDelay(1);
		switch(type)
		{
		case CPU_A:
		case CPU_B:
			if((ret=NDK_RfidPiccDetect(NULL))!=NDK_OK)
				continue;
				
			return SUCC;
			break;
		case MIFARE_1:
			if((ret=NDK_M1Request(REQA, pUIDlen, UID))!=NDK_OK)
				continue;
			
			if((ret=NDK_M1Anti(pUIDlen, UID))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime,"%s,line %d:����ʧ��%d",__FILE__ ,  __LINE__,  ret);
				return ret;
			}
			if((ret=NDK_M1Select(*pUIDlen, UID, SAK))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime,"%s,line %d:����ʧ��%d",__FILE__ ,  __LINE__,  ret);
				return ret;
			}
			return SUCC;
			break;
		case FELICA:
			//FELICA��Ѱ������
			if((ret=NDK_RfidFelicaPoll(UID, pUIDlen))!=NDK_OK)
				continue;
			return SUCC;
			break;
#if M0_ENABLE 
		case MIFARE_0:
			//M0��Ѱ������
			if((ret=NDK_MifareActive(REQA, UID, pUIDlen,SAK))!=NDK_OK)
				continue;
			return SUCC;
			break;	
#endif
#if ISO15693_ENABLE
		case ISO15693:
			if((ret=NDK_ISO15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards)) != NDK_OK)
				continue;
			if((ret=NDK_iso15693SelectPicc(cards[0])) != NDK_OK)
			{
				cls_show_msg1(g_keeptime, "%s,line %d:ѡ��ʧ��(%d)",__FILE__ ,  __LINE__, ret);
				return ret;
			}	
			return SUCC;
			break;
#endif
		default:
			cls_show_msg("δ֪���͵Ŀ���");
			return FAIL;
			break;
		}	
	}
	return FAIL;
}

//UIDlen:�������������Ҳ�������������,��������
//��CPU������,�����������;
//��M-1������,�����������,�ʶ�Ӧ�ȸ�ֵ(��RFID_detect���)�ٽ�������ַ����
//FELICA������Ҫ�������
int RFID_active(SMART_t type, uchar *UID, int *pUIDlen)
{
	/*private & local definition*/
	int ret = 0;
	uchar picctype = 0;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	
	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
		if((ret=NDK_RfidPiccActivate(&picctype, pUIDlen, UID))!=NDK_OK||((type!=CPU_A||picctype!=RFID_TYPE_A)&&(type!=CPU_B||picctype!=RFID_TYPE_B)))
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s����ʧ��(%d,%d)",__FILE__ , __LINE__, card[type], ret, picctype);
			return (ret!=NDK_OK)?ret:FAIL;
		}
		return SUCC;
		break;
	case MIFARE_1:
		//���ڲ�֪��4k����B��Կ�Ƕ���,ʹ��A��Կ(������A��ԿΪȫ0xff)
#if 0	//������ʾ,ֱ�ӷ��ظ��ϲ㼴��
		if((ret=CMD_Auth_Ex(AUTHKEY_TYPE_A, 0x01, AUTHKEY, UID))!=MI_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�ⲿ��֤ʧ��(ret=%d)", __LINE__, ret);
			return ret;
		}
		return SUCC;
#else
		return NDK_M1ExternalAuthen(*pUIDlen, UID, AUTHKEY_TYPE_A, AUTHKEY, 0x01);//return ((ret=CMD_Auth_Ex(AUTHKEY_TYPE_A, 0x01, AUTHKEY, UID))!=MI_OK)?ret:SUCC;
#endif
		break;
	case FELICA:
		//FELICA������Ҫ�������
		//int NDK_RfidFelicaApdu(int nSendlen, uchar *psSendbuf, int *pnRecvlen, uchar *psRecebuf)
		return SUCC;
		break;
	case MIFARE_0:
		//Ϊ�˼��ݲ���C��M0��(����Ҫ��֤)�Ͳ�����֤
		return SUCC;
		break;
#if ISO15693_ENABLE
	case ISO15693:
		//15693������Ҫ�������
		return SUCC;
		break;
#endif
	default:
		return FAIL;
		break;
	}	
}

int RFID_APDUrw(SMART_t type, uchar *UID, int *cpurcvlen)
{
	/*private & local definition*/
	int ret = 0, rcvlen = 0;
	uchar atrbuf[300] = {0};
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	uchar Select_1pay[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
#else
	uchar Select_1pay[] = 
	{0x00, 0xA4, 0x04, 0x00, 0x0E,
	  '1',  'P',  'A',  'Y',  '.',  'S',  'Y',  'S',  '.',  'D',  'D',  'F',  '0',  '1',
	  0x00
	};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif
	uchar out[MAXLEN_DATA] = {0};
	ushort sw = 0;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
#if ISO15693_ENABLE
	iso15693PiccMemoryBlock_t w_memBlock;
	iso15693PiccMemoryBlock_t r_memBlock;
	uchar ucBlocknum = 0;
	//15693��Ƭд������
	ucBlocknum = 0x01;
	w_memBlock.data[0] = 0x02;
	w_memBlock.data[1] = 0x08;
	w_memBlock.data[2] = 0x04;
	w_memBlock.data[3] = 0x09;	
#endif

	//FELICA������֤������
	uchar  sendbuf[300] = {0};
	sendbuf[0] = 16;
	sendbuf[1] = 0x06;
	memcpy(&sendbuf[2], &UID[2], 8);
	sendbuf[10] = 0x01;
	sendbuf[11] = 0x09;
	sendbuf[12] = 0x00;
	sendbuf[13] = 0x01;
	sendbuf[14] = 0x80;
	sendbuf[15] = 0x00;
	
	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
		if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, cpurcvlen, atrbuf))!=NDK_OK||((sw=SMART_GETSW(atrbuf+*cpurcvlen-2))!=0x9000&&(sw&0x6000)!=0x6000))
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s��дʧ��(%d, 0x%04x)",__FILE__ , __LINE__, card[type], ret, sw);
			return (ret!=NDK_OK)?ret:FAIL;
		}
		return SUCC;
		break;
	case MIFARE_1:
#if 0	//���ǵ����Կ�������ֻ��������,��һ�ֲ��Բ���
		if((ret=CMD_BLKWrite(0x02, BLK02DATA_FF))!=MI_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��дʧ��(ret=%d)", __LINE__, ret);
			return ret;
		}
#endif
		if((ret=NDK_M1Read(0x02, &rcvlen, out))!=NDK_OK)//if((ret=CMD_BLKRead(0x02, out))!=MI_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s���ʧ��(%d)",__FILE__ , __LINE__, card[type], ret);
			return ret;
		}
		else
		{
#if 0	//���ǵ����Կ�������ֻ��������,��һ�ֲ��Բ���
			if(memcmp(BLK02DATA_FF, out, LEN_BLKDATA))
#else
			//Ԥ��rcvlenӦ����LEN_BLKDATA
			if(!memcmp(g_RF_bak, DATA16, rcvlen))//�״�
				memcpy(g_RF_bak, out, rcvlen);
			else if(memcmp(g_RF_bak, out, rcvlen))//������
#endif
			{
				cls_show_msg1(g_keeptime, "%s,line %d:%s����У��ʧ��(%d)",__FILE__ , __LINE__, card[type], rcvlen);
				return FAIL;
			}
		}
		return SUCC;
		break;
	case FELICA:
		//FELICA����д����
		if((ret=NDK_RfidFelicaApdu(sendbuf[0], sendbuf, &rcvlen, out))!=NDK_OK||rcvlen!=out[0])
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s����֤��ʧ��(%d)(%d,%d)",__FILE__ , __LINE__, card[type], ret, rcvlen, out[0]);
			return ret;
		}
		else
		{
			if(out[1]!=0x07||memcmp(sendbuf+2, out+2, 8))
			{
				cls_show_msg1(g_keeptime, "%s,line %d:%s����У��ʧ��(%02x)",__FILE__ , __LINE__, card[type], out[1]);
				return FAIL;
			}
		}
		return SUCC;
		break;
#if M0_ENABLE 
	case MIFARE_0:
		//���ǵ����Կ�������ֻ�������� 
		if((ret=NDK_M0Read(12,&rcvlen,out))!=NDK_OK) 
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s���ʧ��(%d)",__FILE__ , __LINE__, card[type], ret);
			return ret;
		}
		else
		{
			//Ԥ��rcvlenӦ����LEN_BLKDATA
			if(!memcmp(g_RF_bak, DATA16, rcvlen))//�״�
				memcpy(g_RF_bak, out, rcvlen);
			else if(memcmp(g_RF_bak, out, rcvlen))//������
			{
				cls_show_msg1(g_keeptime, "%s,line %d:%s����У��ʧ��(%d)",__FILE__ , __LINE__, card[type], rcvlen);
				return FAIL;
			}
		}
		return SUCC;
		break;	
#endif		
#if ISO15693_ENABLE
	case ISO15693:
		// write   read
		if((ret=NDK_iso15693Write_SingleBlock(cards[0],w_memBlock,ucBlocknum)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%sдʧ��(%d)",__FILE__ , __LINE__, card[type], ret);
			return FAIL;
		}
		memset(&r_memBlock, 0, sizeof(iso15693PiccMemoryBlock_t));
		if((ret=NDK_iso15693Read_SingleBlock(cards[0],ucBlocknum,&r_memBlock)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s,line %d:%s��ʧ��(%d)",__FILE__ , __LINE__, card[type], ret);
			return FAIL;
		}	
		//compare
		if((r_memBlock.data[0]!=w_memBlock.data[0])||(r_memBlock.data[1]!=w_memBlock.data[1])||(r_memBlock.data[2]!=w_memBlock.data[2])||(r_memBlock.data[3]!=w_memBlock.data[3]))
		{
			cls_show_msg1(g_keeptime,"%s,line %d:%s����У��ʧ��(%02x:%02x:%02x:%02x)", __FILE__ , __LINE__, card[type], r_memBlock.data[0], r_memBlock.data[1], r_memBlock.data[2], r_memBlock.data[3]);
			return FAIL;
		}
		return SUCC;
		break;
#endif
	default:
		return FAIL;
		break;
	}	
}

int RFID_deactive(SMART_t type, ushort msec)
{
	/*private & local definition*/
	//int ret = 0;

	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
	case FELICA:
	case MIFARE_0:
		return NDK_RfidPiccDeactivate(msec);
		break;
#if ISO15693_ENABLE
	case ISO15693:
		return NDK_ISO15693_Deinit();
		break;
#endif
	default:
		return FAIL;
		break;
	}
}
int RFID_finish(SMART_t type)
{
	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
	case FELICA:
	case MIFARE_0:
		return NDK_RfidCloseRf();
		break;
#if ISO15693_ENABLE
	case ISO15693:
		return NDK_ISO15693_Deinit();
		break;
#endif
	default:
		return FAIL;
		break;
	}
}

#else
int RFID_init(SMART_t type){return FAIL;}
int RFID_detect(SMART_t type, uchar *UID, int *pUIDlen){return FAIL;}
int RFID_detect_poll(SMART_t type, uchar *UID, int *pUIDlen,int nTimeOut){return FAIL;}
int RFID_active(SMART_t type, uchar *UID, int *pUIDlen){return FAIL;}
int RFID_APDUrw(SMART_t type, uchar *UID, int *cpurcvlen){return FAIL;}
int RFID_deactive(SMART_t type, ushort msec){return FAIL;}
int RFID_finish(SMART_t type){return FAIL;}
#endif
#endif

#if !(defined ME15CHX||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
#if (ICSAMNUM>0)
int select_ICSAM(void)
{
	/*private & local definition*/
//#include "icsam.h"
	char *CardNoStr[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};
	int i = 0;
	char pInfo[64] = {0};
#if SCR_DISABLE
	char tmp[32] = {0};
#endif
	/*process body*/
	if(g_SequencePressFlag)
	{
		if(GetParaInfo("ICSAMType",pInfo)!=SUCC)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%s)", __LINE__, pInfo);
			return 0;//�����ļ����ô���ʱĬ�ϲ���IC��
		}
		if(trim_string2int(pInfo,&i)!=SUCC)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%d)", __LINE__, i);
			return 0;
		}
		return i;
	}
	
	do
	{
		cls_printf("_ѡ����Կ�_\n");
		for(i=0; i<ICSAMNUM/*SZ_ARRAY(CardNo)-1*/; i++)
#if SCR_DISABLE//�ض��򵽴���
		{
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp, (i%2)?"%d.%s\n":"%d.%s ", i+1, CardNoStr[i]);
#if 0//ANDIROD_ENABLE
			NDK_AZPortWrite(g_com, strlen(tmp), tmp);
#else
			NDK_PortWrite(g_com, strlen(tmp), tmp);
#endif
		}
#endif
		NDK_ScrPrintf((i%2)?"%d.%s\n":"%d.%s ", i+1, CardNoStr[i]);
		NDK_ScrRefresh();
		if((i=lib_getch()-'1')==(ESC-'1'))
			return NDK_ERR_QUIT;//QUIT;
	}while(i<0 || i>ICSAMNUM-1/*SZ_ARRAY(CardNo)-2*/);

	return i;
}

int ICSAM_detect(EM_ICTYPE cardno)
{
	/*private & local definition*/
	int ret = 0, sta = 0;
#if !K21_ENABLE
	int lshift[] = {1, 4, 5, 6, 3};
#endif
	/*process body*/
	if((ret=NDK_IccDetect(&sta))!=NDK_OK)
		return ret;

#if K21_ENABLE   //K21ֻ֧�ֲ忨״̬�Ĳ�ѯ����֧���ϵ�״̬�Ĳ�ѯ
	return SUCC;
#else
	return (sta&0x000000ff&(0x01<<lshift[cardno]))?SUCC:FAIL;//return (sta&0x000000ff&(0x01<<(cardno+1)))?SUCC:FAIL
#endif

}

int ICSAM_APDUrw(EM_ICTYPE cardno)
{
	/*private & local definition*/
	int ret = 0, rlen = 0;
	uchar recv[128] = {0}, send[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	//���������Ŀ��Ĳ�ͬ,�еĿ���֧�����������,������޹�,��ʹ�õĲ��Կ��й�
	//char sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 0};
	ushort sw = 0;
	char *card[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};

	/*process body*/
	//ʹ��iccrw/iccrw_new��ȡ���������
	//if ((ret=iccrw(cardno, sizeof(send), send, recv))!=SUCC||recv[(int)recv[0]-1]!=0x90||recv[(int)recv[0]]!=0x00)
	if((ret=NDK_Iccrw(cardno, sizeof(send), send, &rlen, recv))!=NDK_OK||((sw=SMART_GETSW(recv+rlen-2))!=0x9000&&(sw=SMART_GETSW(recv+rlen-2))!=0x6d00))
	{
		cls_show_msg1(g_keeptime, "%s,line %d:%s��дʧ��(%d, 0x%04x)",__FILE__ ,__LINE__, card[cardno], ret, sw);
		return (ret!=NDK_OK)?ret:FAIL;
	}

	return SUCC;
}
#else
int select_ICSAM(void){return FAIL;}
int ICSAM_detect(EM_ICTYPE cardno){return FAIL;}
int ICSAM_APDUrw(EM_ICTYPE cardno){return FAIL;}
#endif

SMART_t select_smarttype(void)
{
	/*private & local definition*/
	int nKeyin = 0, rfctype=-1;
	SMART_t type[] = {CPU_A, CPU_B, MIFARE_1, FELICA, MIFARE_0, ISO15693, SAM, IC};
	char pInfo[64] = {0};

	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)
		{
			if(GetParaInfo("SMARTType",pInfo)!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%s)", __LINE__, pInfo);
				return CPU_A;//�����ļ�����ʧ��ʱĬ�ϲ���A��
			}
			if(trim_string2int(pInfo,&rfctype)!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%d)", __LINE__, rfctype);
				return CPU_A;
			}
			switch(rfctype)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				return type[rfctype-1];
				break;
			case ESC:
			default:
				cls_show_msg1(2, "������ΪCPU_A(Ĭ��)...");
				return CPU_A;
				break;
			}
		}

		nKeyin = cls_show_msg("ѡ������...\n"
							  "1.CPU_A 2.CPU_B\n"
							  "3.M-1 4.FELICA\n"
							  "5.M-0 6.15693\n"
							  "7.SAM 8.IC\n");

		switch(nKeyin)
		{
		case '3':
			//memcpy(gen31_bak, DATA16, LEN_BLKDATA);
		case '1':
		case '2':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			return type[nKeyin-'1'];
			break;
		case ESC:
			cls_show_msg1(2, "������ΪCPU_A(Ĭ��)...");
			return CPU_A;
			break;
		default:
			continue;
			break;
		}
	}
}

int smart_detect(SMART_t type, uchar *UID , int *pUIDlen)
{
	/*private & local definition*/
	uchar atr[300] = {0};
	int len = 0;

	/*process body*/
	switch (type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
	case FELICA:
	case ISO15693:
	case MIFARE_0:	
		return RFID_detect(type, UID, pUIDlen);
		break;
	case SAM:
	case IC:
		return ICSAM_powerup(UID[0], atr, &len);
		break;
	default:
		return FAIL;
		break;
	}
}

int smart_active(SMART_t type, uchar *UID, int *pUIDlen)
{
	/*private & local definition*/

	/*process body*/
	switch (type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
	case FELICA:
	case ISO15693:
	case MIFARE_0:	
		return RFID_active(type, UID, pUIDlen);
		break;
	case SAM:
	case IC:
		return  ICSAM_detect(UID[0]);
		break;
	default:
		return FAIL;
		break;
	}
}

int smart_APDUrw(SMART_t type, EM_ICTYPE cardno, uchar *UID)
{
	/*private & local definition*/
	int rcvlen = 0;
	
	/*process body*/
	switch (type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
	case FELICA:
	case ISO15693:
	case MIFARE_0:	
		return RFID_APDUrw(type, UID, &rcvlen);
		break;
	case SAM:
	case IC:
		return ICSAM_APDUrw(cardno);
		break;
	default:
		return FAIL;
		break;
	}
}

int smart_deactive(SMART_t type, EM_ICTYPE cardno, ushort msec)
{
	/*private & local definition*/
	
	/*process body*/
	switch (type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
	case FELICA:
	case ISO15693:
	case MIFARE_0:	
		return RFID_deactive(type, msec);
		break;
	case SAM:
	case IC:
		return ICSAM_powerdown(cardno);
		break;
	default:
		return FAIL;
		break;
	}
}
#else
int select_ICSAM(void)
{
	/*private & local definition*/
	
	/*process body*/
	return 0;//ME15Cֻ֧��IC����ֱ�ӷ���0��ʾѡ��IC��
}

int ICSAM_detect(EM_ICTYPE cardno)
{
	/*private & local definition*/
	
	/*process body*/
	return SUCC;
}

int ICSAM_APDUrw(EM_ICTYPE cardno)
{
	/*private & local definition*/
	int ret = 0, rlen = 0;
	uchar recv[128] = {0}, send[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	//���������Ŀ��Ĳ�ͬ,�еĿ���֧�����������,������޹�,��ʹ�õĲ��Կ��й�
	//char sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 0};
	ushort sw = 0;
	char *card[] = {"IC1", "SAM1", "SAM2", "SAM3", "SAM4"};

	/*process body*/
	//ʹ��iccrw/iccrw_new��ȡ���������
	if((ret=NDK_Iccrw(cardno, sizeof(send), send, &rlen, recv))!=NDK_OK||((sw=SMART_GETSW(recv+rlen-2))!=0x9000&&(sw=SMART_GETSW(recv+rlen-2))!=0x6d00))
	{
		cls_show_msg1(g_keeptime, "%s,line %d:%s��дʧ��(%d, 0x%04x)",__FILE__ ,__LINE__, card[cardno], ret, sw);
		return (ret!=NDK_OK)?ret:FAIL;
	}

	return SUCC;
}

SMART_t select_smarttype(void)
{
	/*private & local definition*/
	
	/*process body*/
	cls_show_msg1(2, "������ΪIC��(Ĭ��)...");
	return IC;
}

int smart_detect(SMART_t type, uchar *UID , int *pUIDlen)
{
	/*private & local definition*/
	uchar atr[150] = {0};
	int len = 0;

	/*process body*/
	return ICSAM_powerup(UID[0], atr, &len);
}

int smart_active(SMART_t type, uchar *UID, int *pUIDlen)
{
	/*private & local definition*/

	/*process body*/
	return  ICSAM_detect(UID[0]);
}

int smart_APDUrw(SMART_t type, EM_ICTYPE cardno, uchar *UID)
{
	/*private & local definition*/
	
	/*process body*/
	return ICSAM_APDUrw(cardno);
}

int smart_deactive(SMART_t type, EM_ICTYPE cardno, ushort msec)
{
	/*private & local definition*/
	
	/*process body*/
	return ICSAM_powerdown(cardno);
}
#endif

#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE||ANDIROD_ENABLE
//��С����תΪNL829�в��õĴ�����
void little2big_ushort(ushort src, ushort *des)
{
	/*private & local definition*/
	uchar tmp[8] = {0}, *addr = (uchar *)&src;
	int loop = sizeof(ushort)-1;
	
	/*process body*/
	for(; loop>=0; loop--)
		tmp[loop] = *addr++;
	memcpy((void *)des, tmp, sizeof(ushort));
}

//������ֻ��������Ĺ���(data->frame),����Դ���,���ϲ㷢�ͺ�,�Է�Ӧ���֪
int NL829_pack_data(uchar *data, int datalen, uchar *frame)
{
	/*private & local definition*/
	int LRCOffset = NL829_LEN_FRAMEHEAD+datalen;
	int framelen = LRCOffset+NL829_LEN_FRAMETAIL;
	
	/*process body*/
	if(data==NULL||datalen<NL829_LEN_CMD||datalen>NL829_MAXLEN_DATA)
		return FAIL;
	frame[NL829_OFFSET_STX] = STX;
	frame[NL829_OFFSET_DATALEN] = datalen/256;
	frame[NL829_OFFSET_DATALEN+1] = datalen%256;
	memcpy(frame+NL829_OFFSET_DATA, data, datalen);
	frame[LRCOffset] = MakeLRC(data, datalen);
	frame[LRCOffset+NL829_LEN_LRC] = ETX;
	return framelen;
}

//�����������������Ӧ�Ľ⹹(data--->status+append),�����س�״̬������ĸ������ݵĳ��ȸ�������
//�ϲ�ӦΪappend�����㹻�ռ�(����MAXLEN_DATA)
int NL829_unpack_data(uchar *data, int datalen, uchar *status, uchar *append)
{
	/*private & local definition*/
	
	/*process body*/
        //��Ƶ��Ѱ���Ͱ���������ѯָ��������һ���ֽڵ�״̬����״ֵ̬ΪAAʱ����������ɹ�����������Ϊ������ַ�����״ֵ̬Ϊ55ʱ���������볬ʱ����ȡ��������
        if(datalen<NL829_LEN_STATUS)
        {
            if(1==datalen)
            {
                if (0x55==data[0])
                    return NL829_PP60_FAIL;
                else if (0xAA!=data[0])
                    return FAIL;
            }
            else
                return FAIL;//-1��ʾ��������
        }
        if (PP60_SUCC==data[0])
        {
			memcpy(status, data, 1);
        	memcpy(append, &data[1], datalen-1);
        	return (datalen-1);//>=0��ʾ��������
        }
    	memcpy(status, data, NL829_LEN_STATUS);
    	memcpy(append, &data[NL829_LEN_STATUS], datalen-NL829_LEN_STATUS);
    	return (datalen-NL829_LEN_STATUS);//>=0��ʾ��������
}

ushort NL829_send_frame(uchar *cmdstr, int cmdlen)
{
	/*private & local definition*/
	
	/*process body*/
	serial_clrportbuf_all(g_devicecom);
	//���������֡����һ�㲻�ᳬ��������󻺳�,��portwriteһ�㶼��ɹ�,�ɲ��л���жϷ���
	return (NDK_PortWrite(g_devicecom, cmdlen, (char *)cmdstr)==NDK_OK)?NL829_SUCC:NL829_FAIL;//return (portwrite(g_devicecom, cmdlen, (char *)cmdstr)==SUCC)?NL829_SUCC:NL829_FAIL;
}

//���ڰ�Э���ֶν���,����ǰ�жϳ�data������ֶ��Ƿ���ȷ,����data��ŵ������ȥ������У��
ushort NL829_recv_frame(uchar *data, int *datalen, ushort timeout)
{
	/*private & local definition*/
	int len = 0, LRCOffset = 0, ret = 0, rlen = 0;
	uchar frame[NL829_MAXLEN_FRAME] = {0};
	
	/*process body*/
	//ע��:ʵ��������829Ҳ��һ��Ӧ��֡һ��ع���,����ʵ���ܶ���STXʱ,��������Ҳ���ѵ�����ջ����,
	//��ֻҪ����Ԥ�ڳ��Ⱥ���,���ճ�ʱ�������(������Ϊ0),����߽���Ч��
	//if((ret=portread(g_devicecom, NL829_LEN_STX, (char *)frame, NL829_TIMEOUT_BASE+timeout))!=NL829_LEN_STX||frame[NL829_OFFSET_STX]!=STX)//����STX�ĳ�ʱʱ��Ҫ���ǵ�829�Ĳ���ʱ�估���ڴ����ʱ��
	if((ret=NDK_PortRead(g_devicecom, NL829_LEN_STX, (char *)frame, (NL829_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=NL829_LEN_STX||frame[NL829_OFFSET_STX]!=STX)
	{
		//cls_show_msg("%s, line %d:%d, %02x", __FILE__, __LINE__, ret, frame[NL829_OFFSET_STX]);
		return NL829_ERR_FRAME_STX;
	}
	//if((ret=portread(g_devicecom, NL829_LEN_DATALEN, (char *)frame+NL829_OFFSET_DATALEN, /*0*/NL829_TIMEOUT_BASE+timeout))!=NL829_LEN_DATALEN)//���ǵ����泬ʱʱ��δ���ͷ�����,���˴���ʱΪ0������δ����ʱ(��BPS),�����.�ʳ�ʱʱ���Ϊ20+timeout
	if((ret=NDK_PortRead(g_devicecom, NL829_LEN_DATALEN, (char *)frame+NL829_OFFSET_DATALEN, /*0*/(NL829_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=NL829_LEN_DATALEN)	
		return NL829_ERR_FRAME_DATALEN1;
	len=NL829_2BYTESTOUSHORT(frame+NL829_OFFSET_DATALEN);
	if(len<1/*NL829_LEN_STATUS*/||len>NL829_MAXLEN_DATA)//������0x32 0x29ָ��status״̬����ֻ��1���ֽ�
		return NL829_ERR_FRAME_DATALEN2;
	//if((ret=portread(g_devicecom, len, (char *)frame+NL829_OFFSET_DATA, /*0*/NL829_TIMEOUT_BASE+timeout))!=len)
	if((ret=NDK_PortRead(g_devicecom, len, (char *)frame+NL829_OFFSET_DATA, /*0*/(NL829_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=len)			
		return NL829_ERR_FRAME_DATA;
	LRCOffset = NL829_LEN_FRAMEHEAD+len;
	//if((ret=portread(g_devicecom, NL829_LEN_LRC, (char *)frame+LRCOffset, /*0*/NL829_TIMEOUT_BASE+timeout))!=NL829_LEN_LRC||frame[LRCOffset]!=MakeLRC(frame+NL829_OFFSET_DATA, len))
	if((ret=NDK_PortRead(g_devicecom, NL829_LEN_LRC, (char *)frame+LRCOffset, /*0*/(NL829_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=NL829_LEN_LRC||frame[LRCOffset]!=MakeLRC(frame+NL829_OFFSET_DATA, len))					
		return NL829_ERR_FRAME_LRC;
	//if((ret=portread(g_devicecom, NL829_LEN_ETX, (char *)frame+LRCOffset+NL829_LEN_LRC, /*0*/NL829_TIMEOUT_BASE+timeout))!=NL829_LEN_ETX||frame[LRCOffset+NL829_LEN_LRC]!=ETX)
	if((ret=NDK_PortRead(g_devicecom, NL829_LEN_ETX, (char *)frame+LRCOffset+NL829_LEN_LRC, /*0*/(NL829_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=NL829_LEN_ETX||frame[LRCOffset+NL829_LEN_LRC]!=ETX)			
		return NL829_ERR_FRAME_ETX;

	//���϶���Ԥ�ڽ��յ��˲Ÿ����������������SUCC
	memcpy(data, frame+NL829_OFFSET_DATA, len);
	*datalen = len;
	return NL829_SUCC;
}

//dataΪӦ�÷��͵��������,statusΪӦ�ý��յ�״̬��,appendΪ��״̬������ĸ�������
//datalenΪ�ϲ�ָ�����͵�����������ܳ���
//appendlenΪ�ײ㷵�ظ��ϲ�ĳ�״̬������ĸ������ݵĳ���
//timeout:����һЩ�г�ʱҪ���ָ��(ִ����ϻ�ʱ�ŷ���)����ʹ��,�����Ĵ���0����
//����������NL829_SUCC��NL829_ERR_FRAME_BASEΪ����һ�������,����,���������жϻ�Ӧ֡��status�Ƿ���0x0000
ushort NL829_frame_factory(uchar *data, int datalen, uchar *status, uchar *append, int *appendlen, ushort timeout)
{
	/*private & local definition*/
	int ret = 0, len = 0;
	ushort ret1 = NL829_FAIL;
	uchar frame_cmd[NL829_MAXLEN_FRAME] = {0};
	uchar resp[NL829_MAXLEN_DATA] = {0};//frame_resp�е�data��
	
	/*process body*/
	//���
	len = NL829_pack_data(data, datalen, frame_cmd);
#if defined DEBUG_CMD//���������
	if(!memcmp(data, DEBUG_CMD, NL829_LEN_CMD))
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(frame_cmd+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif
	//����
	if((ret1=NL829_send_frame(frame_cmd, len))!=NL829_SUCC)
		return ret1;//return NL829_ERR_FRAME_SND;
	//�հ�
	if((ret1=NL829_recv_frame(resp, &len, timeout))!=NL829_SUCC)
		return ret1;//����NL829_ERR_FRAME_XXX(����Ľ��մ���)
#if defined DEBUG_CMD//���������
	if(!memcmp(data, DEBUG_CMD, NL829_LEN_CMD))
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(resp+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif		
	//���
	ret=NL829_unpack_data(resp, len, status, append);
	if(ret==FAIL)
		return NL829_ERR_FRAME_UNPACK;
    else if(ret==NL829_PP60_FAIL)
		return NL829_PP60_FAIL;
	*appendlen = ret;
	return NL829_SUCC;
}

//һ�����,����CMD�ӿ����ʱע������ĺϷ��Բ����ж�,͸������·����829Ӧ�𼴿�,�����ӿ��Բ��Ե��������ʱ829��Ӧ��
//���سɹ�״̬���������Ч�������棬��������57600
ushort NL829_SetBPS(uchar bps)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_SETBPS, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = bps;
	if((ret=NL829_frame_factory(data, NL829_SETBPS_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_SETBPS_APPLEN_RCV)
		return NL829_ERR_CMD_SETBPS;
	return NL829_GETSTAVAL(status);
}

ushort NL829_pack_PP60cmd(uchar *PP60cmd, int PP60cmdlen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0, datalen = NL829_LEN_CMD+1+PP60cmdlen;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/	
	memcpy(data, NL829_PP60_PACKCMD, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = DEVICE_PP60RF;
	memcpy(data+NL829_OFFSET_ARGS+1, PP60cmd, PP60cmdlen);
	if((ret=NL829_frame_factory(data, datalen, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(NL829_GETSTAVAL(status)!=NL829_GETSTAVAL(NL829_PP60_PACKCMD))
		return NL829_ERR_PP60_PACKCMD;
	if(appendlen!=NL829_PP60_PACKCMD_APPLEN_RCV)
		return NL829_ERR_PP60_PACKCMD1;
	return NL829_GETSTAVAL(append);
}

//secΪ�ȴ���ȡPP60����ص�ʱ�䣨�룩,==0:��ȡ���л�����������;>300,���谴300��
//inlenΪϣ���յ���PP60�������ݵĳ���,����512�����谴512��
//ע��:�������,ʵ���յ����������ض�С����ϣ���յ�����
ushort NL829_get_PP60rsp(ushort sec, ushort inlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, sec1 = 0, inlen1 = 0, outlen1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;//ע:staval������829��status���ֵ�˶�������PP60���ص�״̬���ֵ
	
	/*process body*/	
	memcpy(data, NL829_PP60_GETRSP, NL829_LEN_CMD);
	little2big_ushort(sec, &sec1);
	memcpy(data+NL829_OFFSET_ARGS, &sec1, sizeof(sec1));
	little2big_ushort(inlen, &inlen1);
	memcpy(data+NL829_OFFSET_ARGS+sizeof(sec1), &inlen1, sizeof(inlen1));
	if((ret=NL829_frame_factory(data, NL829_PP60_GETRSP_DATALEN_SND, status, append, &appendlen, sec))!=NL829_SUCC)
		return ret;
	if(NL829_GETSTAVAL(status)!=NL829_GETSTAVAL(NL829_PP60_GETRSP))
		return NL829_ERR_PP60_GETRSP;
	if((staval=NL829_GETSTAVAL(append))==NL829_PP60_SUCC)
	{
		outlen1 = NL829_2BYTESTOUSHORT(append+sizeof(staval));
		if(appendlen!=sizeof(staval)+sizeof(outlen1)+outlen1)
			return NL829_ERR_PP60_GETRSP1;
		if(outlen1==0)//��ʱδ�յ�����
			return NL829_ERR_PP60_GETRSP2;
		*outlen = outlen1;
		memcpy(out, append+sizeof(staval)+sizeof(outlen1), outlen1);
	}
	else
	{
		if(appendlen!=sizeof(staval))
			return NL829_ERR_PP60_GETRSP3;
	}
	return staval;
}

//�������������ж�nini829��PP60�ܷ�ͨѶ��
ushort NL829_cnnt_PP60(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;//ע:staval������829��status���ֵ�˶�������PP60���ص�״̬���ֵ
	
	/*process body*/
	memcpy(data, NL829_PP60_CNNT, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, NL829_PP60_CNNT_DATA);
	if((ret=NL829_frame_factory(data, NL829_PP60_CNNT_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(NL829_GETSTAVAL(status)!=NL829_GETSTAVAL(NL829_PP60_CNNT))
		return NL829_ERR_PP60_CNNT;
	if((staval=NL829_GETSTAVAL(append))==NL829_PP60_SUCC)
	{
		if(appendlen!=sizeof(staval)+1)
			return NL829_ERR_PP60_CNNT1;
	}
	else
	{
		if(appendlen!=sizeof(staval))
			return NL829_ERR_PP60_CNNT2;
	}
	return staval;
}

//����sp10_T����֡�������������������հ������
//�˺���������Ĺ���(data�����������������,������β��ETX��LRC)
int SP10_T_pack_data(uchar *data, int datalen, uchar *frame)
{
	int ETXOffset = SP10_T_LEN_FRAMEHEAD+datalen;
	int framelen = ETXOffset+SP10_T_LEN_FRAMETAIL;
	
	/*process body*/
	if(data==NULL||datalen<SP10_T_LEN_CMD||datalen>SP10_T_MAXLEN_DATA)
		return FAIL;
	frame[SP10_T_OFFSET_STX] = STX;
	frame[SP10_T_OFFSET_DATALEN] = (datalen+SP10_T_LEN_ETX)/256; //ʵ��len�ǰ���ETX
	frame[SP10_T_OFFSET_DATALEN+1] = (datalen+SP10_T_LEN_ETX)%256;
	memcpy(frame+SP10_T_OFFSET_DATA, data, datalen);
	frame[ETXOffset] =ETX ;
	frame[ETXOffset+SP10_T_LEN_LRC] = MakeLRC(frame+SP10_T_OFFSET_DATALEN, SP10_T_LEN_DATALEN+datalen+SP10_T_LEN_ETX);
	return framelen;
}

//�˺����Ƕ�������Ӧ�Ľ���(��data��Ľ���cmd+����:answer+append)
int SP10_T_unpack_data(uchar *data, int datalen, uchar *answer, uchar *append)
{
	//�����Fx\Ex�Զ������������״ֵ̬����,�����Bxϵ�е�������״ֵ̬��ֱ�Ӷ������ж�?
	int appendlen = 0;
	memcpy(answer, data, SP10_T_LEN_CMD);//��Ӧ����
	memcpy(append, data+SP10_T_LEN_CMD, datalen-SP10_T_LEN_CMD);
	appendlen=(datalen-1);
	return appendlen;
}

//����
ushort SP10_T_send_frame(uchar *cmdstr, int cmdlen)
{
	/*private & local definition*/
	int ret = 0;
	/*process body*/
	ret=serial_clrportbuf_all(g_devicecom);
	//���������֡����һ�㲻�ᳬ��������󻺳�,��portwriteһ�㶼��ɹ�,�ɲ��л���жϷ���
	return (NDK_PortWrite(g_devicecom, cmdlen, (char *)cmdstr)==NDK_OK)?SP10_T_SUCC:SP10_T_FAIL;
}

//���ڰ�Э���ֶν���,����ǰ�жϳ�data������ֶ��Ƿ���ȷ,����data��ŵ������ȥ������У��
ushort SP10_T_recv_frame(uchar *data, int *datalen, ushort timeout)
{
	/*private & local definition*/
	int len = 0, ETXOffset = 0, ret = 0, rlen = 0;
	uchar frame[SP10_T_MAXLEN_FRAME] = {0};
	
	/*process body*/
	//ע��:ʵ��������Ӧ��֡һ��ع���,����ʵ���ܶ���STXʱ,��������Ҳ���ѵ�����ջ����,
	//��ֻҪ����Ԥ�ڳ��Ⱥ���,���ճ�ʱ�������(������Ϊ0),����߽���Ч��
	if((ret=NDK_PortRead(g_devicecom, SP10_T_LEN_STX, (char *)frame, (SP10_T_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP10_T_LEN_STX||frame[SP10_T_OFFSET_STX]!=STX)
	{
		return SP10_T_ERR_FRAME_STX;
	}
	if((ret=NDK_PortRead(g_devicecom, SP10_T_LEN_DATALEN, (char *)frame+SP10_T_OFFSET_DATALEN, /*0*/(SP10_T_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP10_T_LEN_DATALEN)	
		return SP10_T_ERR_FRAME_DATALEN1;
	len=SP10_T_2BYTESTOUSHORT(frame+SP10_T_OFFSET_DATALEN)-1;	//���������len�ǰ���ETX ������Ҫ��1
	if(len<1||len>SP10_T_MAXLEN_DATA)
		return SP10_T_ERR_FRAME_DATALEN2;
	//len:CMD+��������ݲ�����ETX
	if((ret=NDK_PortRead(g_devicecom, len, (char *)frame+SP10_T_OFFSET_DATA, /*0*/(SP10_T_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=len)			
		return SP10_T_ERR_FRAME_DATA;
	ETXOffset = SP10_T_LEN_FRAMEHEAD+len;
	if((ret=NDK_PortRead(g_devicecom, SP10_T_LEN_ETX, (char *)frame+ETXOffset, /*0*/(NL829_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP10_T_LEN_ETX||frame[ETXOffset]!=ETX)	
		return SP10_T_ERR_FRAME_ETX;
	if((ret=NDK_PortRead(g_devicecom, SP10_T_LEN_LRC, (char *)frame+ETXOffset+SP10_T_LEN_ETX, /*0*/(SP10_T_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP10_T_LEN_LRC||frame[ETXOffset+SP10_T_LEN_ETX]!=MakeLRC(frame+SP10_T_OFFSET_DATALEN, SP10_T_LEN_DATALEN+len+SP10_T_LEN_ETX))					
		return SP10_T_ERR_FRAME_LRC;

	//���϶���Ԥ�ڽ��յ��˲Ÿ����������������SUCC
	memcpy(data, frame+SP10_T_OFFSET_DATA, len);
	*datalen = len;
	return SP10_T_SUCC;
}

//dataΪӦ�÷��͵��������,answerΪӦ�ý��յ���Ӧ����,appendΪ����������ĸ�������
//datalenΪ�ϲ�ָ�����͵�����������ܳ���
//appendlenΪ�ײ㷵�ظ��ϲ�ĳ���������ĸ������ݵĳ���
//timeout:����һЩ�г�ʱҪ���ָ��(ִ����ϻ�ʱ�ŷ���)����ʹ��,�����Ĵ���0����
//����������SP10_T_SUCC��SP10_T_ERR_FRAME_BASEΪ����һ�������
ushort SP10_T_frame_factory(uchar *data, int datalen, uchar *answer, uchar *append, int *appendlen, ushort timeout)
{
	/*private & local definition*/
	int ret = 0, len = 0;
	ushort ret1 = SP10_T_FAIL;
	uchar frame_cmd[SP10_T_MAXLEN_FRAME] = {0};
	uchar resp[SP10_T_MAXLEN_DATA] = {0};//frame_resp�е�data��
	
	/*process body*/
	//���
	len = SP10_T_pack_data(data, datalen, frame_cmd);
#if defined DEBUG_CMD//���������
	if(!memcmp(data, DEBUG_CMD, SP10_T_LEN_CMD))
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(frame_cmd+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif
	//����
	if((ret1=SP10_T_send_frame(frame_cmd, len))!=SP10_T_SUCC)
		return ret1;	
	//�հ�
	if((ret1=SP10_T_recv_frame(resp, &len, timeout))!=SP10_T_SUCC)
		return ret1;//���ؾ���Ľ��մ���
#if defined DEBUG_CMD//���������
	if(!memcmp(data, DEBUG_CMD, SP10_T_LEN_CMD))
	{	
		int i = 0;
		cls_printf("line %d: %d", __LINE__, len);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(resp+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif	

	//���
	ret=SP10_T_unpack_data(resp, len, answer, append);
	if((ret!=len-1) && (ret!=0))//���ص�append���ȿ���Ϊ0������len-1,���������߱���
		return SP10_T_ERR_FRAME_UNPACK;
	*appendlen = ret;
	return SP10_T_SUCC;
}

//����SP100������̺����ָ�֡�������������������հ������
//�˺���������Ĺ���(data�����������������,������β��ETX��LRC)
int SP100_pack_data(uchar *data, int datalen, uchar *frame)
{
	int ETXOffset = SP100_LEN_FRAMEHEAD+datalen;
	int framelen = ETXOffset+SP100_LEN_FRAMETAIL;
	int len =2;
	uchar bcdbuf[2] = {0};
	
	/*process body*/
	if(data==NULL||datalen<SP100_LEN_CMD||datalen>SP100_MAXLEN_DATA)
		return FAIL;
	frame[SP100_OFFSET_STX] = STX;
	NDK_IntToBcd(bcdbuf,&len,datalen);//lenʹ�õ���2�ֽڵ�BCD��,ʹ��intתbcd
	memcpy(frame+SP100_OFFSET_DATALEN, bcdbuf, 2);
	memcpy(frame+SP100_OFFSET_DATA, data, datalen);
	frame[ETXOffset] =ETX ;
	frame[ETXOffset+SP100_LEN_ETX] = MakeLRC(frame+SP100_OFFSET_DATALEN, SP100_LEN_DATALEN+datalen+SP100_LEN_ETX);
	return framelen;
}
//�˺����Ƕ�������Ӧ�Ľ���(��data��Ľ���cmd+����:answer+append)
int SP100_unpack_data(uchar *data, int datalen, uchar *answer, uchar *append)
{
	int appendlen = 0;
	memcpy(answer, data, SP100_LEN_CMD);//��Ӧ����
	memcpy(append, data+SP100_LEN_CMD, datalen-SP100_LEN_CMD);
	appendlen=(datalen-2);
	return appendlen;
}
//����
ushort SP100_send_frame(uchar *cmdstr, int cmdlen)
{
	/*private & local definition*/
	int ret = 0;
	/*process body*/
	ret=serial_clrportbuf_all(g_devicecom);
	//���������֡����һ�㲻�ᳬ��������󻺳�,��portwriteһ�㶼��ɹ�,�ɲ��л���жϷ���
	return (NDK_PortWrite(g_devicecom, cmdlen, (char *)cmdstr)==NDK_OK)?SP100_SUCC:SP100_FAIL;
}
//���ڰ�Э���ֶν���,����ǰ�жϳ�data������ֶ��Ƿ���ȷ,����data��ŵ������ȥ������У��
ushort SP100_recv_frame(uchar *data, int *datalen, ushort timeout)
{
	/*private & local definition*/
	int len = 0, ETXOffset = 0, ret = 0, rlen = 0;
	uchar frame[SP100_MAXLEN_FRAME] = {0};
	
	/*process body*/
	//ע��:ʵ��������Ӧ��֡һ��ع���,����ʵ���ܶ���STXʱ,��������Ҳ���ѵ�����ջ����,
	//��ֻҪ����Ԥ�ڳ��Ⱥ���,���ճ�ʱ�������(������Ϊ0),����߽���Ч��
	if((ret=NDK_PortRead(g_devicecom, SP100_LEN_STX, (char *)frame, (SP100_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP100_LEN_STX||frame[SP100_OFFSET_STX]!=STX)
	{
		return SP100_ERR_FRAME_STX;
	}
	
	if((ret=NDK_PortRead(g_devicecom, SP100_LEN_DATALEN, (char *)frame+SP100_OFFSET_DATALEN, /*0*/(SP100_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP100_LEN_DATALEN)	
		return SP100_ERR_FRAME_DATALEN1;
	NDK_BcdToInt(frame+SP100_OFFSET_DATALEN, &len);//len�ĸ�ʽ��2�ֽ�bcd��,Ҫת��int	
	if(len<1||len>SP100_MAXLEN_DATA)
		return SP100_ERR_FRAME_DATALEN2;
	//len:CMD+��������ݲ�����ETX
	if((ret=NDK_PortRead(g_devicecom, len, (char *)frame+SP100_OFFSET_DATA, /*0*/(SP100_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=len)			
		return SP100_ERR_FRAME_DATA;
	ETXOffset = SP100_LEN_FRAMEHEAD+len;
	if((ret=NDK_PortRead(g_devicecom, SP100_LEN_ETX, (char *)frame+ETXOffset, /*0*/(SP100_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP100_LEN_ETX||frame[ETXOffset]!=ETX)	
		return SP10_T_ERR_FRAME_ETX;
	if((ret=NDK_PortRead(g_devicecom, SP100_LEN_LRC, (char *)frame+ETXOffset+SP100_LEN_ETX, /*0*/(SP100_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP100_LEN_LRC||frame[ETXOffset+SP100_LEN_ETX]!=MakeLRC(frame+SP100_LEN_STX, SP100_LEN_DATALEN+len+SP100_LEN_ETX))					
		return SP100_ERR_FRAME_LRC;
	//���϶���Ԥ�ڽ��յ��˲Ÿ����������������SUCC
	memcpy(data, frame+SP100_OFFSET_DATA, len);
	*datalen = len;
	return SP100_SUCC;
}
//�жϽ��յ�����ACK����NAK
ushort SP100_recv_ACKorNAK(uchar *data, int *datalen, ushort timeout)
{
	/*private & local definition*/
	int   ret = 0,rlen = 0;
	uchar frame[2] = {0};
	
	//���յ�ACK����NAK��
	if((ret=NDK_PortRead(g_devicecom, SP100_LEN_ACK_NAK, (char *)frame, (SP100_TIMEOUT_BASE+timeout)*1000, &rlen))!=NDK_OK||rlen!=SP100_LEN_ACK_NAK)
	{
		return SP100_ERR_ACK_NAK;
	}
	memcpy(data, frame, rlen);
	*datalen = rlen;
	return SP100_SUCC;
}
//dataΪӦ�÷��͵��������,answerΪӦ�ý��յ���Ӧ����,appendΪ����������ĸ�������
//datalenΪ�ϲ�ָ�����͵�����������ܳ���
//appendlenΪ�ײ㷵�ظ��ϲ�ĳ���������ĸ������ݵĳ���
//timeout:����һЩ�г�ʱҪ���ָ��(ִ����ϻ�ʱ�ŷ���)����ʹ��,�����Ĵ���0����
//����������SP100_SUCC��SP100_ERR_FRAME_BASEΪ����һ�������
ushort SP100_frame_factory(uchar *data, int datalen, uchar *answer, uchar *append, int *appendlen, ushort timeout)
{
	/*private & local definition*/
	int ret = 0, len = 0, len1;
	ushort ret1 = SP100_FAIL;
	uchar frame_cmd[SP100_MAXLEN_FRAME] = {0};
	uchar resp[SP100_MAXLEN_DATA] = {0};//frame_resp�е�data��
	uchar recv_ackornak[2] = {0};
	
	/*process body*/
	//���
	len = SP100_pack_data(data, datalen, frame_cmd);
#if defined DEBUG_CMD//���������
	if(!memcmp(data, DEBUG_CMD, SP100_LEN_CMD))
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(frame_cmd+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif
	//����
	if((ret1=SP100_send_frame(frame_cmd, len))!=SP100_SUCC)
		return ret1;	
	//��Ӧ���ack or nak
	if((ret1=SP100_recv_ACKorNAK(recv_ackornak, &len1, timeout))!=SP100_SUCC)
		return ret1;
	//�����ack ˵���ɹ�������ϢȻ���ٽ����հ����
	if(recv_ackornak[0] == ACK)
	{
		//�հ�
		if((ret1=SP100_recv_frame(resp, &len, timeout))!=SP100_SUCC)
			return ret1;//���ؾ���Ľ��մ���
#if defined DEBUG_CMD//���������
		if(!memcmp(data, DEBUG_CMD, SP100_LEN_CMD))
		{	
			int i = 0;
			cls_printf("line %d: %d", __LINE__, len);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
			for(; i<len; i++)
				NDK_ScrPrintf(" %02X", *(resp+i));
			NDK_ScrRefresh();
			lib_getch();
		}
#endif	

		//���
		ret=SP100_unpack_data(resp, len, answer, append);
		//���ص�append ����
		//return SP100_ERR_FRAME_UNPACK;
		*appendlen = ret;
		//����ACK��pinpad
		if((ret1=SP100_send_frame("\x06", 1))!=SP100_SUCC)
			return ret1;
		return SP100_SUCC;
	}
}
//ֻ��ACK ���ص�ָ��֡����з�װ���� �հ�
ushort SP100_frame_factory_ACK(uchar *data, int datalen, uchar *answer, int *answerlen, ushort timeout)
{
	/*private & local definition*/
	int len = 0;
	ushort ret = SP100_FAIL;
	uchar frame_cmd[SP100_MAXLEN_FRAME] = {0};
	
	/*process body*/
	//���
	len = SP100_pack_data(data, datalen, frame_cmd);
#if defined DEBUG_CMD//���������
	if(!memcmp(data, DEBUG_CMD, SP100_LEN_CMD))
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(frame_cmd+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif
	//����
	if((ret=SP100_send_frame(frame_cmd, len))!=SP100_SUCC)
		return ret;	
	//��Ӧ���ack or nak
	if((ret=SP100_recv_ACKorNAK(answer, answerlen, timeout))!=SP100_SUCC)
		return ret;
	
	return SP100_SUCC;
}

#endif

#if USB_ENABLE||SD_ENABLE
#define	BUFFERSIZE	(1024*20)
#define	TESTFILE	"test.file"
int systest_SDCard( int disktype)
{
	/*private & local definition*/
	char rootdir[16] = {0}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	int ret = 0, fp = 0, loop = 0,state = 0, cnt = rand()%1023+2 /*�����ļ���С��Լ����[40K, 20M]*/, wrlen = BUFFERSIZE-rand()%8;
	time_t oldtime = 0, oldtime1 = 0, diff2 = 0;	
	
	/*process body*/
	if((ret=NDK_DiskOpen(disktype,MAXTIMEOUT))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:%s��ʧ��(%d)", __FILE__,  __LINE__,(disktype==UDISK)?"U��":"SD��", ret);
		return FAIL;
	}
	oldtime1=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&state))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s, line %d:��ȡ״̬ʧ��(%d)", __FILE__, __LINE__, ret);
			return FAIL;
		} 
		if(state==DISKMOUNTSUCC)
			break;
		if(time(NULL)-oldtime1>30)
		{
			cls_show_msg1(g_keeptime, "%s, line %d:��ȡ״̬��ʱ(%d)", __FILE__, __LINE__, ret);	
			return FAIL;
		}
		NDK_SysMsDelay(1);
	}
	if((ret=NDK_DiskGetRootDirName(disktype,(char **)&rootdir))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:��ȡ·��ʧ��(%d)", __FILE__, __LINE__, ret);
		goto ERR;
	}
	//cls_show_msg1(5, "���²���ʹ��%dB��Ϊÿ�ζ�д����!", wrlen);
	sprintf(fname, "%s/%s", rootdir, TESTFILE);
	NDK_FsDel(fname);
	if ((fp=NDK_FsOpen(fname, "w"))<0)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:�����ļ�ʧ��(%d)", __FILE__, __LINE__, ret);
		goto ERR;
	}
	
	for (loop=0; loop<wrlen; loop++)
		writebuf[loop] = rand()%256;
	for (loop=0; loop<cnt; loop++)
	{
		cls_printf("����%s�ļ���(Լ%dkB)������(Լ%dkB),���Ժ�...",(disktype==UDISK)?"U��":"SD��", cnt*wrlen/1024, loop*wrlen/1024);
		if ((ret=NDK_FsWrite(fp,writebuf, wrlen ))!=wrlen)
		{
			cls_show_msg1(g_keeptime, "%s, line %d:д�ļ�ʧ��(cnt=%d, loop=%d, ret=%d, wrlen=%d)", __FILE__, __LINE__, cnt, loop, ret, wrlen);
			goto ERR;
		}
	}
	
	if(NDK_FsClose(fp)!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:�ر��ļ�ʧ��(%d)", __FILE__, __LINE__, ret);
		goto ERR;
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:%s�ر�ʧ��(%d)", __FILE__,  __LINE__,(disktype==UDISK)?"U��":"SD��", ret);
		goto ERR;
	}
	if((ret=NDK_DiskOpen(disktype,MAXTIMEOUT))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:%s��ʧ��(%d)", __FILE__,  __LINE__,(disktype==UDISK)?"U��":"SD��", ret);
		goto ERR;
	}	
	oldtime1=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&state))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "%s, line %d:��ȡ״̬ʧ��(%d)", __FILE__, __LINE__, ret);
			goto ERR;
		} 
		if(state==DISKMOUNTSUCC)
			break;
		if(time(NULL)-oldtime1>30)
		{
			cls_show_msg1(g_keeptime, "%s, line %d:��ȡ״̬��ʱ(%d)", __FILE__, __LINE__, ret);	
			goto ERR;
		}
		NDK_SysMsDelay(1);	
	}
	if ((fp=NDK_FsOpen(fname, "r"))<0)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:���ļ�ʧ��(%d)", __FILE__, __LINE__, ret);
		goto ERR;
	}
	//�ļ���С�Ƿ����ı�
	//if ((ret=lib_filelength(fp))!=cnt*wrlen)
		
	if(NDK_FsFileSize(fname,(uint *)&ret)!=NDK_OK||ret!=cnt*wrlen)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:�ļ���СУ��ʧ��(ʵ��:%dB, Ԥ��:%dB)", __FILE__, __LINE__, ret, cnt*wrlen);
		goto ERR;
	}
	
	cls_printf("У��SD���ļ���(Լ%dkB),���Ժ�...", cnt*wrlen/1024);
	oldtime = time(NULL);
	for (loop=0; loop<cnt; loop++)
	{
		if ((ret=NDK_FsRead(fp,readbuf, wrlen ))!=wrlen||memcmp(writebuf, readbuf, wrlen))
		{
			cls_show_msg1(g_keeptime, "%s, line %d:���ݶ�д��һ��(cnt=%d, loop=%d, ret=%d, wrlen=%d)", __FILE__, __LINE__, cnt, loop, ret, wrlen);
			debug_buf("���ͻ���", writebuf, wrlen);
			debug_buf("���ջ���", readbuf, ret);
			cls_show_msg("�뱣�����ݲ��رմ���,���������...");//��ֹ�������UsbDiskClose��,Ҳ��USB������Ϣ��¼������������
			goto ERR;
		}
		show_stopwatch(ENABLE, (diff2=time(NULL)-oldtime));
	}
	show_stopwatch(DISABLE, 0);	
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:�ر��ļ�ʧ��(%d)", __FILE__, __LINE__, ret);
		goto ERR;
	}
	if((ret=NDK_FsDel(fname))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:ɾ���ļ�ʧ��(%d)", __FILE__, __LINE__, ret);
		goto ERR;
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "%s, line %d:%s�ر�ʧ��(%d)", __FILE__,  __LINE__,(disktype==UDISK)?"U��":"SD��", ret);
		goto ERR;
	}
	return SUCC;
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(fname);
	NDK_DiskClose(disktype);
	return FAIL;
}
#else
int systest_SDCard( int disktype){return FAIL;}
#endif

/*���ɰ�ť,��У����Ƶ���׹����Ƿ��Ӱ�쵽������׼ȷ��*/
#if TOUCH_ENABLE
int systest_touch(void)
{
	int start_x1_point = 0, start_y1_point = 0, ret = 0;
#if CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG
	int wide = 50, height = 50;
#else
	int wide = 20, height = 20;
#endif
	ST_PADDATA pad_data;
	uint unWidth = 0, unHeight = 0;

	NDK_ScrGetViewPort(NULL, NULL, &unWidth, &unHeight);
	//unWidth = GUI_GetLCDWidth();
	//unHeight = GUI_GetLCDHeight();
	
	start_x1_point = rand()%(unWidth-wide-2);
	start_y1_point = rand()%(unHeight-height-2);

	NDK_ScrClrs();
	cls_printf("������Ļ����ʾ�ĺڿ�����");
	if((ret = NDK_ScrRectangle(start_x1_point,start_y1_point,wide,height,RECT_PATTERNS_SOLID_FILL,0))!=NDK_OK)
	{
		cls_show_msg1(2, "%s,line %d����ʧ��(%d)", __FILE__,__LINE__,ret);
		return FAIL;
	}
	NDK_ScrRectangle(start_x1_point-2,start_y1_point-2,wide+4,height+4,RECT_PATTERNS_NO_FILL,0);//������ڲ����޹�,���жϷ���,linwl 20140126
	//if((ret = NDK_ScrRectangle(start_x1_point-2,start_y1_point-2,wide+4,height+4,RECT_PATTERNS_NO_FILL,0))!=NDK_OK)
	//{
		//cls_show_msg1(2, "%s,line %d����ʧ��(%d)", __FILE__,__LINE__,ret);
		//return FAIL;
	//}
	NDK_ScrRefresh();
	while(1)
	{
		memset(&pad_data,0,sizeof(pad_data));
		if((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
		{
			cls_show_msg1(2, "%s,line %d����ʧ��(%d)", __FILE__,__LINE__,ret);
			return FAIL;
		}
		if (pad_data.emPadState == PADSTATE_KEY  && pad_data.unKeycode == ESC)
		{
			cls_show_msg1(3,"�û�ȡ��");
			return SUCC;
		}
		if(pad_data.emPadState == PADSTATE_UP)
		{
			if(((pad_data.unPadX >= start_x1_point) && (pad_data.unPadX <= start_x1_point+wide))&&((pad_data.unPadY >= start_y1_point) && (pad_data.unPadY <= start_y1_point+height)))
			{	
				return SUCC;
			}	
			else
			{
				cls_show_msg("��Ļ��Ư��,Ԥ��(%d~%d,%d~%d),ʵ��(%d,%d)",start_x1_point,start_x1_point+wide,start_y1_point,start_y1_point+height,pad_data.unPadX,pad_data.unPadY);
				return FAIL;
			}
		}
	}
}
#else
int systest_touch(void){return FAIL;}
#endif

#if 0	//�ýӿڲ���֧��
//����ӿڲ�������PP60��BPS,����������PP60ͨѶ��829����BPS
ushort NL829_SetBPS_PP60(uchar type, uchar bps, uchar *ret_bps)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;//ע:staval������829��status���ֵ�˶�������PP60���ص�״̬���ֵ
	
	/*process body*/
	memcpy(data, NL829_PP60_SETBPS, NL829_LEN_CMD);
	if(type==DEVICE_PP60_1&&bps==PP60_BPS_SELFADAPT)
		type = 0x03;
	else if(type==DEVICE_PP60_2&&bps==PP60_BPS_SELFADAPT)
		type = 0x04;
	data[NL829_OFFSET_ARGS] = type;
	data[NL829_OFFSET_ARGS+1] = bps;
	if((ret=NL829_frame_factory(data, NL829_PP60_SETBPS_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(NL829_GETSTAVAL(status)!=NL829_GETSTAVAL(NL829_PP60_SETBPS))
		return NL829_ERR_PP60_SETBPS;
	if((staval=NL829_GETSTAVAL(append))=NL829_PP60_SUCC)
	{
		if(appendlen!=sizeof(staval)+1)
			return NL829_ERR_PP60_SETBPS1;
		*ret_bps = append[sizeof(staval)];
	}
	else
	{
		if(appendlen!=sizeof(staval))
			return NL829_ERR_PP60_SETBPS2;
	}
	return staval;
}
#endif

//��Ƚ��������������Ӳ����ĺ���
LinkSetting links[10];
NetworkSetting nets[10];

#if 1//���ݽ������Ͳ�������ȫ�ֱ����ͻָ�ȫ�ֱ���
void Local2Global(LinkType lnktype)
{
	memset(&linksetting,0,sizeof(LinkSetting));	
	memset(&netsetting,0,sizeof(NetworkSetting));	
	memcpy(&linksetting, &links[lnktype], sizeof(LinkSetting));	
	memcpy(&netsetting, &nets[lnktype], sizeof(NetworkSetting));
}

void Global2Local(LinkType lnktype)
{	
	memset(&links[lnktype],0,sizeof(LinkSetting));	
	memset(&nets[lnktype],0,sizeof(NetworkSetting));		
	memcpy(&links[lnktype], &linksetting, sizeof(LinkSetting));	
	memcpy(&nets[lnktype], &netsetting, sizeof(NetworkSetting));
}
#endif

#if (NDKSOCK_ENABLE || NDKSSL_ENABLE)
void tcp_abnormal(void)
{
	/*private & local definition*/
	uint h_tcp = 0;
	char buf[ETHPACKMAXLEN] = {0};
	int  j = 0, timeout = 1, nKey = 0;
	int slen = 0;
	//Packet sendpacket;

	/*process body*/
	//case1:�ֱ����ǿ�ƹҶϺ�Э��ҶϵĲ���
	for (j=0; j<sizeof(buf); j++)
		buf[j]=rand()%256;

	nKey = cls_show_msg("��ѡ��ҶϷ�ʽ\n"
						"1.ǿ�ƹҶ�\n"
						"2.Э��Ҷ�");
	if(nKey=='1')//ǿ�ƹҶ����޸ĹҶϱ�־Ϊ0
		TCPRESETFLAG=TRUE;
	
	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}

	if(TransUp(&h_tcp)!=SUCC)
	{
		NetDown();
		cls_show_msg("line %d:TransUpʧ��", __LINE__);
		return;
	}
		
	cls_show_msg("��ε�POS���̨������,���������");
	if((slen=sock_send(h_tcp, buf, ETHPACKMAXLEN, timeout))!=ETHPACKMAXLEN)
	{
		NetDown();
		cls_show_msg1(g_keeptime,"line %d:����ʧ��(ʵ��%d)", __LINE__, slen);
		return;
	}
		
	TransDown(h_tcp);

	if(TCPRESETFLAG==FALSE)//Э��Ҷ�ʱ��POS���ط����ݰ������²������ߺ󣬺�̨���յ��ط������ݰ�
	{
		if(cls_show_msg("���²���POS���̨�����ߺ�,�ȴ�20��鿴��̨�Ƿ����յ�����,��:[ȷ��],��:[����]")!=ENTER)
		{
			cls_show_msg1(g_keeptime,"line %d:Э��Ҷϲ���ʧ��", __LINE__);
		}
		else
		{
			cls_show_msg1(g_keeptime,"Э��Ҷϲ���ͨ��", __LINE__);
		}
	}
	else//ǿ�ƹҶ�ʱ��POS�����ط����ݰ������²������ߺ󣬺�̨�����յ��ط������ݰ�
	{
		if(cls_show_msg("���²���POS���̨�����ߺ�,�ȴ�20��鿴��̨�Ƿ����յ�����,��:[ȷ��],��:[����]")==ENTER)
		{
			cls_show_msg1(g_keeptime,"line %d:ǿ�ƹҶϲ���ʧ��", __LINE__);
		}
		else
		{
			cls_show_msg1(g_keeptime,"ǿ�ƹҶϲ���ͨ��", __LINE__);
		}
	}
	NetDown();
	TCPRESETFLAG = FALSE;//�ָ�Ĭ�Ϸ�ʽ:Э��Ҷ�
	
	return;
}
#endif
#endif
