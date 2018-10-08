/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �����ۺ�	
* file name			: systest57.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: �����ۺϲ���
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
#define	TESTITEM	"�����ۺ�"
#define MAXWAITTIME		30
#define DEFAULT_CNT_VLE	100
#define	DEFAULT_CNT_STR	"100"
#define	PACKETLIFE		(10)

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
					 	jiangym			20140227	created
*****************************************************************/	
static void create_bt_ability_packet(Packet *packet, char *buf)
{
	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len= BUFSIZE_BT-MIN_BTCMD_LEN;
	packet->lifecycle = PACKETLIFE;//10;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}
static void bt_ability()
{
	int ret = -1,i = 0, recvLen = 0, cmdlen = 0, datalen = 0;
	char sendBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0}, recvBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0};//ԭʼ����
	char cmdBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	float commtimes = 0, rate = 0;
#if K21_ENABLE
	char str[32] = {0};
#endif
#if !(defined ME15CHX||defined ME15B)
	Packet sendpacket;
#else
	int cnt = 0;
	float time1 = 0, time2 = 0;
#endif
	
	/*process body*/
#if !(defined ME15CHX||defined ME15B) 
	create_bt_ability_packet(&sendpacket, sendBuf);
#else
	datalen = BUFSIZE_BT-MIN_BTCMD_LEN;
	cnt = PACKETLIFE;
	for (i=0; i<sizeof(sendBuf); i++)
		sendBuf[i]=rand()%256;
#endif
	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_BT);
	if((ret = NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:����������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			return ;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:�������ʧ��(%d)", __LINE__,ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
	cls_printf("����������д���ܲ���...");
	//ѭ����д10�� �����дͨѶ����
	i=0;
	while(1)
	{
		i++;
#if !(defined ME15CHX||defined ME15B)
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		datalen = sendpacket.len;
#else		
		if(i>cnt)
			break;
#endif
		cmdlen = datalen+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
		
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)//����ջ���
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//�������ݻش�����
		cls_printf("�����շ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x05, sendBuf, datalen, cmdBuf);
#if !(defined ME15CHX||defined ME15B)
		lib_StartStopwatch();
#else
		if(i==1)
			time1 = lib_ReadStopwatch();//��ȡ��һ�ο�ʼ����ʱ��ʱ��
#endif
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			goto DISCONNECT;
		}
		memset(cmdBuf, 0, sizeof(cmdBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT,cmdlen, cmdBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))//MAXWAITTIME
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			goto DISCONNECT;
		}
#if !(defined ME15CHX||defined ME15B)
		commtimes+=lib_ReadStopwatch();
#else
		time2=lib_ReadStopwatch();//���ϸ���ʱ��,���һ�θ���ʱ����ʱ������ͨѶʱ��
		commtimes=time2-time1;
#endif
		bt_cmd_uppacket(cmdBuf, datalen, recvBuf);
		if(memcmp(sendBuf, recvBuf, datalen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i);
			goto DISCONNECT;
		}

DISCONNECT:
		//������Ӧȷ�ϱ���
		cls_printf("������Ӧ���ݱ�����...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
	}
	if((ret = NDK_BTDisconnect()) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:�Ͽ���������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	NDK_PortClose(PORT_NUM_BT);
	rate = (float)cmdlen/(commtimes/PACKETLIFE);
#if !K21_ENABLE
	cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "������д����ͨѶ����Ϊ%fB/s", rate);
#else
	{
		memset(str,0,sizeof(str));
		ftos(rate,str);
		cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "������д����ͨѶ����Ϊ%sB/s", str);
	}
#endif
	return;
}

static void bt_read_write(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0, succconut = 0, cmdlen = 0, datalen = 0;
	char sendBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0}, recvBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0};//ԭʼ����
	char cmdBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
#if !(defined ME15CHX||defined ME15B)
	Packet sendpacket;
#else
	uint len = 0;
	char tbuf[16] = {0};
	int cnt = 0;
#endif

	
	/*process body*/
#if !(defined ME15CHX||defined ME15B)  //ME15B�ڴ�7K,ջ4K�ֲ��������ܳ���4K �ʲ���sendpacket��غ���
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
#else
	cls_printf("���ð��ķ��ʹ���:\n");
	strcpy(tbuf, "200");
	if ((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len, INPUTDISP_OTHER, 10, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
		strcpy(tbuf, "200");
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
	
	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_BT);
	if((ret = NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:����������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			return ;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:�������ʧ��(%d)", __LINE__,ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
	//��дѹ������
	i=0;
	while(1)
	{
		if(cls_show_msg1(3,"��ʼ��%d���������ڶ�дѹ��(�ѳɹ�%d��),ESC�˳�", i+1, succconut)==ESC)
			break;
		i++;
#if !(defined ME15CHX||defined ME15B)
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		datalen = sendpacket.len;
#else
		if(i > cnt)
			break;
#endif
		cmdlen = datalen+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
		
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)//����ջ���
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		//�������ݻش�����
		cls_printf("�������ݰ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x05, sendBuf, datalen, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			goto DISCONNECT;
		}

		cls_printf("�������ݰ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT,cmdlen, cmdBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))//MAXWAITTIME
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			goto DISCONNECT;
		}
		
		if((ret = bt_cmd_uppacket(cmdBuf, datalen, recvBuf))!=BTCMD_OK)
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�������ݰ�ʧ��%d", __LINE__, ret);
		
		cls_printf("�Ƚ�������...");
		if(memcmp(sendBuf, recvBuf, datalen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i);
			goto DISCONNECT;
		}
		succconut++;//ͨѶ�ɹ���������
		
DISCONNECT:
		//������Ӧȷ�ϱ���
		cls_printf("������Ӧ���ݱ�����...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
	}
	//�Ͽ���������
	//֪ͨ�ֻ��Ͽ�����
	//cls_printf("���͹Ҷ�֪ͨ��...");
	//memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	//bt_cmd_packet(0x03, NULL, 0, cmdsendBuf);
	//NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf);
	packetnum = 0;//���ӶϿ������к�Ҫ��0
	
	if((ret = NDK_BTDisconnect()) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ͽ���������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	NDK_PortClose(PORT_NUM_BT);
	
	cls_show_msg_record(FILENAME,FUNCNAME,"�������ڶ�дѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	return;
}

static void bt_open_close()
{
	int ret = 0, i = 0, cnt = 0, succ = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen = 0;
	
	/*process body*/
	if(g_SequencePressFlag) 
	{
		cnt = GetCycleValue();  //����ѹ�����ԣ���Ҫ�ֹ�����ѭ�������Ķ�ֱ������Ĭ��ֵ
	}
	else
	{
		cls_printf("Ĭ�ϲ��Դ���:\n");
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			cnt = DEFAULT_CNT_VLE;
		else
			cnt = atoi(CntStr);
	}
		
	NDK_PortClose(PORT_NUM_BT);
	
	while(1)
	{
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;
		if(cls_show_msg1(2,"�������ڿ���ѹ��������\n�ܹ�:%d��,��ִ��:%d��,�ѳɹ�:%d��,��ESC�˳�",cnt,i-1,succ)==ESC)
			break;
		
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:��%d�δ���������ʧ��", __LINE__, i);
			continue;
		}
		if(g_btdiscoverflag)
		{
			if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
				continue ;
			}
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			NDK_BTDisconnect();
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
		cls_show_msg1(3,"���ӳɹ�,3���ʼ�Ҷϲ���");//�˴��������ʱ,�ֻ����ж����Ӳ��ɹ�,����
		
		//POS�����Ͽ�����
		if((ret = NDK_BTDisconnect()) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, i);
			continue;
		}
		succ++;
	}

	NDK_PortClose(PORT_NUM_BT);
	cls_show_msg_record(FILENAME,FUNCNAME,"�ܹ�����%d����������ѹ������,�ѳɹ�%d��",i-1,succ);
	return;
}

static void mobilephone_press()
{
	int ret = 0, i = 0, cnt = 0, succ = 0;//link_status = 0
	char CntStr[8] = DEFAULT_CNT_STR, cmdsendBuf[MIN_BTCMD_LEN] = {0};
	uint unLen = 0;
	
	/*process body*/
	if(g_SequencePressFlag) 
	{
		cnt = GetCycleValue();  //����ѹ�����ԣ���Ҫ�ֹ�����ѭ�������Ķ�ֱ������Ĭ��ֵ
	}
	else
	{
		cls_printf("Ĭ�ϲ��Դ���:\n");
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			cnt = DEFAULT_CNT_VLE;
		else
			cnt = atoi(CntStr);
	}
		
	NDK_PortClose(PORT_NUM_BT);
	
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����������ʧ��", __LINE__);
		return;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			return ;
		}
	}
	while(1)
	{
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;
		if(cls_show_msg1(7,"�ֻ�����ѹ��������\n�ܹ�:%d��,��ִ��:%d��,�ѳɹ�:%d��,��ESC�˳�",cnt,i-1,succ)==ESC)//�����õ�ƻ���ֻ���ʱ�������ݱȽ�������Ҫ�ӳ��ȴ�ʱ��
			break;

		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			NDK_BTDisconnect();
			continue;
		}
		//cls_show_msg1(3,"���ӳɹ�,3���֪ͨ�ֻ��Ҷϲ���");//�˴��������ʱ,�ֻ����ж����Ӳ��ɹ�,����
#if BTMFI_ENABLE
		cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
		//����ָ��֪ͨ�ֻ��Ҷ�
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x03, NULL, 0, cmdsendBuf);
		NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf);
		packetnum = 0;//���ӶϿ������к�Ҫ��0
/*
		//�ж��ֻ��Ͽ��ɹ�
		sleep(3);
		if((ret=NDK_BTStatus(&link_status))!=NDK_OK||link_status!=1)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:�ֻ������Ͽ�ʧ��(%d,%d)", __LINE__, ret, link_status);
			NDK_BTDisconnect();//�ֻ������Ͽ�ʧ��,�����POS�Ͽ�
			continue;
		}
		*/
		succ++;
	}

	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	cls_show_msg_record(FILENAME,FUNCNAME,"�ܹ�����%d����������ѹ������,�ѳɹ�%d��",i-1,succ);
	return;
}

#if 0	//������������������֤BTFWATS���Թ��ߵ�ָ���
static void bt_set_time(void)
{
	/*private & local definition*/
	int ret = -1, i = 0;
	char cmdsendBuf[BUFSIZE_BT] = {0};
	char delaytime1[2]={0x00,0x05},delaytime2[2]={0x00,0x10};
	
	/*process body*/	
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��", __LINE__, i);
		return;
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�������ʧ��(%d)", __LINE__,ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)	//����ջ���
	{				
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
		goto DISCONNECT;
	}
	
	//�������ݻش�����
	cls_show_msg("���������ʼ����ɨ��ʱ����t1ֵΪ5��...");
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x01, delaytime1, 2, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN+2, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
		goto DISCONNECT;
	}

	cls_show_msg("���������ʼ����ɨ���豸�������豸��ʱ����t2ֵΪ10��...");
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x02, delaytime2, 2, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN+2, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
		goto DISCONNECT;
	}
	
DISCONNECT:
	cls_show_msg("���������ʼ֪ͨ�ֻ�ֹͣ���ԣ�������...");
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x04, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN+2, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	
	//�Ͽ���������
	cls_show_msg1(1,"POS�˹Ҷ�����������...");
	if((ret = NDK_BTDisconnect()) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��(%d)", __LINE__, i, ret);
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	NDK_PortClose(PORT_NUM_BT);
		
	return;
}

static void bt_read(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0, cmdlen = 0;
	char recvBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0};//ԭʼ����
	char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	
	/*process body*/	
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��", __LINE__, i);
		return;
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�������ʧ��(%d)", __LINE__,ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)	//����ջ���
	{				
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
		goto DISCONNECT;
	}

	cls_show_msg1(1,"�ȴ��������ݰ���,�����ֻ��˷���8���ֽڵ�0x38...");
	cmdlen = MIN_BTCMD_LEN+8;
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT,cmdlen, cmdrecvBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))//MAXWAITTIME
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
		goto DISCONNECT;
	}

	cls_show_msg1(1,"�����������,�������ݰ���...");
	if((ret = bt_cmd_uppacket(cmdrecvBuf, 8, recvBuf))!=BTCMD_OK)
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�������ݰ�ʧ��%d", __LINE__, ret);

	cls_show_msg1(1,"�������ݰ���...");
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x07, recvBuf, 8, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
		goto DISCONNECT;
	}
	
DISCONNECT:
	cls_show_msg1(1,"����0x03֪ͨ�ֻ��Ҷϣ�������...");
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x03, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN+2, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	
	//�Ͽ���������
	cls_show_msg1(1,"POS�˹Ҷ�����������...");
	if((ret = NDK_BTDisconnect()) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��(%d)", __LINE__, i, ret);
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	NDK_PortClose(PORT_NUM_BT);
		
	return;
}
#endif

static void bt_open_comm_close(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0, succconut = 0, cmdlen = 0, datalen = 0;
	char sendBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0}, recvBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0};//ԭʼ����
	char cmdBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
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
	cls_printf("���ð��ķ��ʹ���:\n");
	strcpy(tbuf, "200");
	if ((ret=lib_kbgetinput(tbuf, 0, sizeof(tbuf)-1, &len, INPUTDISP_OTHER, 10, INPUT_CONTRL_LIMIT))==NDK_ERR ||ret == NDK_ERR_TIMEOUT)
		strcpy(tbuf, "200");
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

	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_BT);
	i=0;
	while(1)
	{
		if(cls_show_msg1(2,"��ʼ��%d��������������ѹ��(�ѳɹ�%d��),ESC�˳�", i+1, succconut)==ESC)
			break;
		i++;
#if !(defined ME15CHX||defined ME15B)
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		datalen = sendpacket.len;
#else
		if(i > cnt)
			break;
#endif
		cmdlen = datalen+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���

		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��", __LINE__, i);
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
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			NDK_BTDisconnect();
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
		//cls_show_msg1(3,"���ӳɹ�,3���ʼ��������");//�˴��������ʱ,�ֻ����ж����Ӳ��ɹ�,����
#if BTMFI_ENABLE
		cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)	//����ջ���
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			goto DISCONNECT;
		}

		//�������ݻش�����
		//cls_show_msg1(1, "�������ݰ���...");
		cls_printf("�������ݰ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x05, sendBuf, datalen, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			goto DISCONNECT;
		}

		//cls_show_msg1(2, "�������ݰ���...");
		cls_printf("�������ݰ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT,cmdlen, cmdBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))//MAXWAITTIME
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			bt_comm_once(12,0);
			goto DISCONNECT;
		}
		
		cls_printf("�Ƚ�������...");
		bt_cmd_uppacket(cmdBuf, datalen, recvBuf);
		if(memcmp(sendBuf, recvBuf, datalen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i);
			goto DISCONNECT;
		}
		
		//������Ӧȷ�ϱ���
		cls_printf("������Ӧ���ݱ�����...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			goto DISCONNECT;
		}
		succconut++;//ͨѶ�ɹ���������
		
DISCONNECT:
		cls_show_msg1(1, "POS�˹Ҷ�����������...");
		if((ret = NDK_BTDisconnect()) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		NDK_PortClose(PORT_NUM_BT);
	}

	cls_show_msg_record(FILENAME,FUNCNAME,"������������+����ѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	return;
}

static void bt_press(void)
{
	/*private & local definition*/
	int key='0';
	
	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			if( ++key == '5' || cls_show_msg1(3, "������������ѹ������,ESC�˳�")==ESC)
				return ;			
		}
		else
		{
			key = cls_show_msg("1.��дѹ��\n"
								"2.����ѹ��\n"
								"3.����ѹ��\n"
								"4.�ֻ�����ѹ��\n"
#if 0//����ָ���
								"5.�ֶ�����\n"
								"6.����ָ�����\n"
#endif
								);
		}
		
		switch(key)
		{
		case '1':
			bt_read_write();
			break;
		case '2':
			bt_open_close();
			break;	
		case '3':
			bt_open_comm_close();
			break;
		case '4':
			mobilephone_press();
			break;
#if 0
		case '5':
			bt_read();
			break;
		case '6':
			bt_set_time();
			break;
#endif			
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

//case1:�ڣ��ֻ��ˣ���ʾ���ʱ���ܾ���ԣ���Ӧ�����ϡ��ٴ����ӣ�������ԣ�Ӧ�����ӳɹ���
void abnormal_test1(void)
{
	int ret = -1,status = 1;
	if((ret = NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)//"115200,8,N,1"
	{
		cls_show_msg("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTITEM,ret);
		goto END;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg( "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			goto END; ;
		}
	}
	if(g_pair_mode == 2 || g_pair_mode == 3)
	{
		cls_show_msg("���BTFWATS������[�ֶ�����],���������ʼ����,��POS��ʾ�ȴ�����ǽ���[ȡ��]��ԵĲ���");
		if((ret=lib_bt_pairing(0))!=NDK_ERR)
		{
			cls_show_msg("line %d:���Ӧ��ʧ��(ret:%d)", __LINE__, ret);
			goto END;
		}
	}
	else
		cls_show_msg("���BTFWATS������[�ֶ�����],��POS����,����PIN�Ի���ʱ��ȡ��,����ʾδ����ʱ����");
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=1)
	{
		cls_show_msg("line %d:%s����ʧ��(ret:%d,status:%d)", __LINE__, TESTITEM,ret,status);
		goto END;
	}
	if(cls_show_msg("POS���ֻ���������ʧ��,��[Enter],��[����]")!=ENTER)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
		goto  END;
	}
	
	cls_show_msg("���������ʼ����,��ʹ���ֻ��������߽��������������,��POS��ʾ�ȴ����ʱ�����[ȷ��]��ԵĲ���");
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:���Ӧ�óɹ�(%d)", __LINE__, ret);
		goto END;
	}
	if((ret=NDK_BTStatus(&status))!=NDK_OK||status!=0)
	{
		cls_show_msg("line %d:%s����ʧ��(ret:%d,status:%d)", __LINE__, TESTITEM, ret, status);
		goto END;
	}
	if(cls_show_msg("POS���ֻ��������ӳɹ�,��[Enter],��[����]")!=ENTER)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
		goto  END;
	}
	NDK_LedStatus(LED_RFID_RED_FLICK);
	cls_show_msg("����������ͨ��,���������");
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	NDK_LedStatus(LED_RFID_RED_OFF);//�ر�led��
	return;
}

//case2:����Чͨ�ŷ�Χ�ڣ���ʱ����û�����ĳЩ�����ڳ�ʱ���޲����Ĵ���̬�£�������������ߣ�����ʹ���������ܣ�Ӧ�����ӳɹ���
void abnormal_test2(void)
{
	int ret = -1;
	
#if !K21_ENABLE//��������BT7�Ƶ��˴� 20151217 linwl
	if(cls_show_msg("ʹ���ֻ�������������%s,Ӧ��������,��ȷ[Enter],����[����]",BtName)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		return;
	}
#endif
	cls_show_msg("��ʱ����û�����,�ٰ��������ʼ,��ʹ���ֻ��������߽��������������");
	if((ret = NDK_PortOpen(PORT_NUM_BT,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg( "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			goto END; ;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg("line %d:�������ʧ��(%d)", __LINE__, ret);
    	goto END;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
	if((ret=bt_comm_once(12,0))!=NDK_OK)
	{
		cls_show_msg("line %d:�����շ�ʧ��(%d)", __LINE__,ret);
		goto END;
	}
	cls_show_msg("����������ͨ��,���������");
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//case3:�ڣ��ֻ��ˣ���ʾ���ʱ��POS������ػ�������������Ӧ��������������
void abnormal_test3(void) 
{
	int ret = -1;

	cls_show_msg("���BTFWATS������[�ֶ�����],�������POS����ȴ���Բ���,����Թ������뽫POS�ػ���������,������ִ��������������,����Ӧ����");
	if((ret = NDK_PortOpen(PORT_NUM_BT,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg( "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			goto END; ;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg("line %d:�������ʧ��(%d)", __LINE__,ret);
		goto END;
	}
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//case4:�ڳɹ����ӵ��ֻ���POS������ػ�������������Ӧ��������������
void abnormal_test4(void)
{
	int ret = -1;

	cls_show_msg("���BTFWATS������[�ֶ�����],���������ʼ����,��ʹ���ֻ��������߽��������������");
	if((ret = NDK_PortOpen(PORT_NUM_BT,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg( "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			goto END; ;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg("line %d:�������ʧ��(%d)", __LINE__,ret);
		goto END;
	}
	cls_show_msg("�뽫POS�ػ���������,������ִ��������������,����Ӧ����");	
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//case5:�������ߺ󣨲������������Ƿ�ر������������ֻ��ˣ�Ӧ���������豸�������������ɹ�������������� �� ���ֻ��ˣ�ͨ��������ģ��ĸ��ֽ���������������ԣ����ӡ�����ͨѶ���Ͽ��ȷ�ʽ��Ӧ�ܻ���������
void abnormal_test5(void)
{
	int ret = -1;
	char sendbuf[12] = {0}, cmdrecbuf[MIN_BTCMD_LEN+12]= {0};
	int len = 0;
#if !BTMFI_ENABLE
	char recbuf[12] = {0}, cmdsendbuf[MIN_BTCMD_LEN+12] = {0};
	int cmdlen = 0;
#endif
	
	//NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if((ret = NDK_PortOpen(PORT_NUM_BT,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg( "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			goto END; ;
		}
	}
	cls_show_msg("POS�ȴ�Լ5s������������ߺ�,ʹ��BTFWATS���ֶ����Խ��������������(Ԥ���ܹ������ϲ���������)");//Ԥ�ھͲ�Ҫ�˰� Ҫ����ʾ̫����ѽ
	//BM77��Գɹ��󲻻ỽ�ѡ�ԭ����BM77�����ȫ����ģ������ɣ�POSû���յ��������û���жϲ������ỽ�� ���������� 20150416
	NDK_SysDelay(5);
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	sleep(1);//ME15�Ƚ���������Ҫһ��ʱ��
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg("line %d:�������ʧ��(%d)", __LINE__,ret);
		goto END;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__,ret);
		goto END;
	}
	cls_show_msg("POS�ȴ�Լ5s������������ߺ�,ʹ���ֻ��������߷�������:12��8��鿴POS(Ԥ���ܹ���������)");
	NDK_SysDelay(5);
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	sleep(1);//ME15�Ƚ���������Ҫһ��ʱ��
	cls_printf("POS�Ѿ�����,����������");
	memset(cmdrecbuf,0,sizeof(cmdrecbuf));
	memset(sendbuf,0x38,sizeof(sendbuf));
#if BTMFI_ENABLE
	//MFI���������Զ����������ݴ���ʽ��ͬ�����޸�
	if(((ret=NDK_PortRead(PORT_NUM_BT, 12, cmdrecbuf, 5*1000, &len)) != NDK_OK) || (len != 12))
	{
		cls_show_msg("line %d:��������ʧ��(%d,%d)", __LINE__, ret, len);
		goto END;
	}
	//�Ƚ�����
	if(memcmp(sendbuf, cmdrecbuf, 12))
	{
		cls_show_msg("line %d:����У��ʧ��", __LINE__);
		goto END;
	}
#else
	cmdlen=MIN_BTCMD_LEN+12;
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecbuf, 5*1000, &len)) != NDK_OK) || (len != cmdlen))
	{
		cls_show_msg("line %d:��������ʧ��(%d,%d)", __LINE__, ret, len);
		goto END;
	}
	//��������
	memset(recbuf,0,sizeof(recbuf));
	if((ret = bt_cmd_uppacket(cmdrecbuf, 12, recbuf))!=BTCMD_OK)
	{
		cls_show_msg("line %d:�������ݰ�ʧ��%d", __LINE__, ret);
		goto END;
	}
	//�Ƚ�����
	if(memcmp(sendbuf, recbuf, 12))
	{
		cls_show_msg("line %d:����У��ʧ��", __LINE__);
		goto END;
	}
	//����07ָ���յ��ֻ�����
	memset(cmdsendbuf, 0, sizeof(cmdsendbuf));
	bt_cmd_packet(0x07, recbuf, 12, cmdsendbuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendbuf))!=NDK_OK)
	{
		cls_show_msg("line %d:POS��������ʧ��(%d)", __LINE__, ret);
		goto END;
	}	
#endif
	cls_show_msg("POS�ȴ�Լ5s������������ߺ�,ʹ���ֻ��������߽��жϿ�������鿴POS(Ԥ���ܹ���������)");
	NDK_SysDelay(5);
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	sleep(1);//ME15�Ƚ���������Ҫһ��ʱ��
	if(cls_show_msg("pos�ѱ�����,��[Enter],��[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		goto END;
	}
	cls_show_msg("����������ͨ��,���������");
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//case6:�������߻��Ѻ�(�����Ǻ��ֻ��ѷ�ʽ,��:�������ѡ��������ѡ���ʱ���ѵ�),�����½�����������Ӧ�ɹ�(�������߹ر����������) �� ֱ�ӽ�����������ͨѶ�ɹ�(�������߲��ر����������)
void abnormal_test6(void)
{
	int ret = -1, status = 0;
	int keyin = -1;
	char sendbuf[12] = {0}, cmdrecbuf[MIN_BTCMD_LEN+12]= {0};
	int len = 0;
#if !BTMFI_ENABLE
	char recbuf[12] = {0}, cmdsendbuf[MIN_BTCMD_LEN+12] = {0};
	int cmdlen = 0;
#endif

	NDK_PortClose(PORT_NUM_BT);
	if((ret = NDK_PortOpen(PORT_NUM_BT,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			cls_show_msg( "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
			goto END; ;
		}
	}
	cls_show_msg("ʹ��BTFWATS���ֶ����Խ������������������,���������");	
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg("line %d:�������ʧ��(%d)", __LINE__,ret);
		goto END;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__,ret);
		goto END;
	}
	cls_show_msg("��������ѡ�����ߺ�Ļ��ѷ�ʽ,�ȴ�Լ5s�����������");
	
	while(1)
    {
		keyin = cls_show_msg("1.��������\n"
							 "2.��������\n"
#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE	
							 "3.��ʱ����"
#endif
							 );
		
		switch(keyin) 
		{
			case '1':
				cls_show_msg("���������������,���ߺ󰴼�����");
				NDK_SysDelay(5);
#if ANDIROD_ENABLE
				if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
				if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
				{
					cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
					goto END;
				}
				sleep(1);//ME15�Ƚ���������Ҫһ��ʱ��
				if((ret=NDK_BTStatus(&status)) != NDK_OK)
				{
					cls_show_msg("line %d:��ȡ����״̬ʧ��(%d)", __LINE__, ret);
					goto END;
				}	
				if(status)//���߻��Ѻ����ǶϿ�״̬�������������Ӧ�óɹ�������������ͨѶ
				{
					cls_printf("ʹ���ֻ��������߽�����������");
					if((ret=lib_bt_pairing(1))!=NDK_OK)
					{
						cls_show_msg("line %d:�����������ʧ��(%d)", __LINE__,ret);
						goto END;
					}
				}
				//���߻��Ѻ�����δ�Ͽ�״̬ �������������ͨѶ	
				if((ret=bt_comm_once(12,0))!=NDK_OK)
				{
					cls_show_msg("line %d:�����շ�ʧ��(%d)", __LINE__,ret);
					goto END;
				}
				break;
			case '2':
				cls_show_msg("���������������,���ߺ��ֻ��������߷�������:12��8����");
				NDK_SysDelay(5);
#if ANDIROD_ENABLE
				if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
				if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
				{
					cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
					goto END;
				}
				sleep(1);//ME15�Ƚ���������Ҫһ��ʱ��
				if((ret=NDK_BTStatus(&status)) != NDK_OK)
				{
					cls_show_msg("line %d:��ȡ����״̬ʧ��(%d)", __LINE__, ret);
					goto END;
				}	
				if(status)//���߻��Ѻ����ǶϿ�״̬�������������Ӧ�óɹ�������������ͨѶ
				{
					cls_printf("ʹ���ֻ��������߽�����������");
					if((ret=lib_bt_pairing(1))!=NDK_OK)
					{
						cls_show_msg("line %d:�����������ʧ��(%d)", __LINE__,ret);
						goto END;
					}
				}
				memset(cmdrecbuf,0,sizeof(cmdrecbuf));
				memset(sendbuf,0x38,sizeof(sendbuf));
#if BTMFI_ENABLE
				//�������ϵ���������ֻ����͵�����
				if(((ret=NDK_PortRead(PORT_NUM_BT, 12, cmdrecbuf, 5*1000, &len)) != NDK_OK) || (len != 12))
				{
					cls_show_msg("line %d:��������ʧ��(%d,%d)", __LINE__, ret, len);
					goto END;
				}
				//�Ƚ�����
				if(memcmp(sendbuf, cmdrecbuf, 12))
				{
					cls_show_msg("line %d:����У��ʧ��", __LINE__);
					goto END;
				}			
#else
				//�������ϵ���������ֻ����͵�����
				cmdlen=MIN_BTCMD_LEN+12;
				if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecbuf, 5*1000, &len)) != NDK_OK) || (len != cmdlen))
				{
					cls_show_msg("line %d:��������ʧ��(%d,%d)", __LINE__, ret, len);
					goto END;
				}
				//��������
				memset(recbuf,0,sizeof(recbuf));
				if((ret = bt_cmd_uppacket(cmdrecbuf, 12, recbuf))!=BTCMD_OK)
				{
					cls_show_msg("line %d:�������ݰ�ʧ��%d", __LINE__, ret);
					goto END;
				}
				//�Ƚ�����
				if(memcmp(sendbuf, recbuf, 12))
				{
					cls_show_msg("line %d:����У��ʧ��", __LINE__);
					goto END;
				}
				//����07ָ���յ��ֻ�����
				memset(cmdsendbuf, 0, sizeof(cmdsendbuf));
				bt_cmd_packet(0x07, recbuf, 12, cmdsendbuf);
				if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendbuf))!=NDK_OK)
				{
					cls_show_msg("line %d:POS��������ʧ��(%d)", __LINE__, ret);
					goto END;
				}
#endif
				break;	
#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE
			case '3':
				cls_show_msg("���������������,���ߺ�ȴ�180s�Զ�����");
				NDK_SysSetSuspendDuration(180);
				if((ret=NDK_SysGoSuspend()) != NDK_OK)
				{
					cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
					NDK_SysSetSuspendDuration(0);//�ָ�Ĭ��
					goto END;
				}
				sleep(1);//ME15�Ƚ���������Ҫһ��ʱ��
				if((ret=NDK_BTStatus(&status)) != NDK_OK)
				{
					cls_show_msg("line %d:��ȡ����״̬ʧ��(%d)", __LINE__, ret);
					NDK_SysSetSuspendDuration(0);//�ָ�Ĭ��
					goto END;
				}
				if(status)//���߻��Ѻ����ǶϿ�״̬�������������Ӧ�óɹ�������������ͨѶ
				{
					cls_printf("ʹ���ֻ��������߽�����������");
					if((ret=lib_bt_pairing(1))!=NDK_OK)
					{
						cls_show_msg("line %d:�����������ʧ��(%d)", __LINE__,ret);
						NDK_SysSetSuspendDuration(0);//�ָ�Ĭ��
						goto END;
					}
#if BTMFI_ENABLE
					cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
				}
				//���߻��Ѻ�����δ�Ͽ�״̬ �������������ͨѶ	
				if((ret=bt_comm_once(12,0))!=NDK_OK)
				{
					cls_show_msg("line %d:�����շ�ʧ��(%d)", __LINE__,ret);
					NDK_SysSetSuspendDuration(0);//�ָ�Ĭ��
					goto END;
				}
				NDK_SysSetSuspendDuration(0);//�ָ�Ĭ��
				break;
#endif
			case ESC:
				cls_show_msg("��ѡ���ѷ�ʽ");
				continue;
			default:
				continue;
				break;
		}
		break;
	}	
	NDK_LedStatus(LED_RFID_RED_FLICK);
	cls_show_msg("����������ͨ��,���������");
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	NDK_LedStatus(LED_RFID_RED_OFF);//�ر�led��
	return;
}

static void bt_abnormal(void)
{
  int keyin = 0;
  
  while(1)
    {
		keyin = cls_show_msg("1.�쳣1 2.�쳣2\n"
							 "3.�쳣3 4.�쳣4\n"
							 "5.�쳣5 6.�쳣6");
		switch(keyin) 
		{
			case '1':
				abnormal_test1();//case1:�ڣ��ֻ��ˣ���ʾ���ʱ���ܾ���ԣ���Ӧ�����ϡ��ٴ����ӣ�������ԣ�Ӧ�����ӳɹ���Ӧ��3��PIN���ģʽ�����PIN������PIN��SSP���·ֱ���֤��
				break;
			case '2':	
				abnormal_test2();//case2:����Чͨ�ŷ�Χ�ڣ���ʱ����û�����ĳЩ�����ڳ�ʱ���޲����Ĵ���̬�£�������������ߣ�����ʹ���������ܣ�Ӧ�����ӳɹ���
				break;	
			case '3':
				abnormal_test3();//case3:�ڣ��ֻ��ˣ���ʾ���ʱ��POS������ػ�������������Ӧ��������������
				break;
			case '4':
				abnormal_test4();//case4:�ڳɹ����ӵ��ֻ���POS������ػ�������������Ӧ��������������
				break;	
			case '5':
				abnormal_test5();//case5:�������ߺ󣨲������������Ƿ�ر������������ֻ��ˣ�Ӧ���������豸�������������ɹ�������������� �� ���ֻ��ˣ�ͨ��������ģ��ĸ��ֽ���������������ԣ����ӡ�����ͨѶ���Ͽ��ȷ�ʽ��Ӧ�ܻ���������
				break;
			case '6':
				abnormal_test6();//case6:�������߻��Ѻ�(�����Ǻ��ֻ��ѷ�ʽ,��:�������ѡ��������ѡ���ʱ���ѵ�),�����½�����������Ӧ�ɹ�(�������߹ر����������) �� ֱ�ӽ�����������ͨѶ�ɹ�(�������߲��ر����������)
				break;
			case ESC:
				return;
			default:
				continue;
				break;
		}
	}	
	return;
}

#if !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15�ռ���Դ���ƣ����β���Ҫ�ĺ���
//�������õ�������˽�к���
int wati_paraing()
{
	int ret = -1, link_status =0, accept = 1, flag = 0;
	uint ulen = 0;
	char pkey[7] = {0};
	time_t diff = 0, oldtime = time(NULL);
	
	if(g_pair_mode==2||g_pair_mode==3)
	{	
		while(1)
		{
			if(cls_show_msg1(1, "���豸�ȴ������...,�˳��밴[ESC]") ==  ESC)
				break;
			accept = 1;
			NDK_BTStatus(&link_status);
			if(!link_status)
			{
				//NDK_BTDisconnect();//�Ͽ��������豸�� 20170828 linying modify
				//cls_show_msg("����������,��ȡ����Ժ����������");
				cls_show_msg1(5,"��pos���POS������,������������");
				//cls_show_msg1(5,"��Գɹ�,���������");
				return 2;
			}
			ret = NDK_BTGetPairingStatus(pkey,&flag);
			if(ret == NDK_ERR_NOT_SUPPORT)//������֧�ָù���
			{
				cls_show_msg("��ȷ�ϴ�ģʽ����ģ���Ƿ�֧�ִ����ģʽ");
				return ret;
			}
			else if(ret == NDK_ERR)//���ģʽ��ҪΪSSP��PassKeyģʽ
			{
				cls_show_msg("���õ����ģʽ��֧�ֱ�����,������SSP��PassKeyģʽ");
				return ret;
			}
			else if(ret == NDK_OK)
			{
				switch(flag)
				{
					case 0:
						if((diff=time(NULL)-oldtime)>60)
						{
							cls_show_msg("line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
							return flag;
						}
						continue;
						break;
					case 1:
						if(pkey[0] == '\0')
						{
							cls_printf("�������豸����ʾ�������:\n");
							if((ret=lib_kbgetinput(pkey, 0, 6, &ulen, INPUTDISP_PASSWD, 0, INPUT_CONTRL_LIMIT_ERETURN)) == NDK_ERR || ret == NDK_ERR_TIMEOUT)
							{
								accept = 0;
							}
						}
						else
						{
							if(cls_show_msg("[Enter]ȷ�����,[����]ȡ�����")==ENTER)
								accept = 1;
							else
								accept = 0;
						}
						NDK_BTConfirmPairing(pkey, accept);
						break;
					case 2:
						cls_show_msg1(5,"��Գɹ�,���������");
						return flag;
						break;
					case 3:
						cls_show_msg("���ʧ��,���������");
						return flag;
						break;
					default:
						cls_show_msg("��⵽δ֪�����״̬(%d)",flag);
						return flag;
						break;
				}
			}
			else
			{
				cls_show_msg1(5,"δ֪�ķ���ֵ(%d)",ret);
				return ret;
			}
		}
	}
	else	
		cls_show_msg("��POS�ȴ���POS��֮���,��Գɹ������������");	
	return NDK_OK;
}

void bt_client_ras(void)
{
	int ret = -1,len = 0;
	char recbuf[BUFSIZE_BT]={0};
	
	cls_printf("�ȴ����豸����������..");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)	 
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)",__LINE__,ret);
		//NDK_PortClose(PORT_NUM_BT);
		return;
	}
	while(1)
	{
		if(cls_show_msg1(2,"�����շ���.ESC�˳�")==ESC)
			break;
		//������
		len = 0;
		memset(recbuf,0,sizeof(recbuf));
		ret = NDK_PortRead(PORT_NUM_BT,sizeof(recbuf),recbuf,0,&len);//�������ճ��ȴ���2047 ��ʱʱ��Ϊ0 �������ý�����еͶ��ϴ��ڲ��� �ж��ж������ݻ᷵��NDK_OK���Ͷ��ж�������һ������NDK_ERR_TIMEOUT
		//cls_show_msg1(1,"���յ����ݳ���Ϊ:%d,%d",len, ret);
		if(ret!=NDK_OK&&ret!=NDK_ERR_TIMEOUT)
		{
			cls_show_msg("line %d:���豸��������ʧ��(%d)", __LINE__, ret);
			//NDK_PortClose(PORT_NUM_BT);
			return;
		}
		else if(len!=0)//if(ret!==NDK_OK&&len!=0)
		{
			//cls_show_msg1(1,"���������У�����Ϊ:%d",len);
			//������
			//update_BT_packet(recbuf,len);
			if((ret=NDK_PortWrite(PORT_NUM_BT, len, recbuf)) != NDK_OK)	
			{
				cls_show_msg("line %d:���豸��������ʧ��(%d)", __LINE__, ret);
				//NDK_PortClose(PORT_NUM_BT);
				return; 	
			}
		}					
	}
}

void bt_client(void)
{
	int ret = -1, nkey =0;
	uint ulen = 0;
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while(1)
    {
		keyin = cls_show_msg("1.�������ƺ�PIN\n"
							 "2.�����������ģʽ\n"
							 "3.�ȴ����"
							 "4.�����շ�");
		switch(keyin) 
		{
		case '1':
			//������������ģʽ
			if((ret=NDK_BTEnterCommand())!=NDK_OK)
			{
				cls_show_msg("line %d:��������ģʽʧ��(%d)", __LINE__, ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			//������������
			cls_printf("��ǰ��������:\n");
			//Ϊ�˼��ݵͶ˲�ƷME15ʹ�÷�װ����
			if ((ret=lib_kbgetinput(BtName,0, sizeof(BtName)-1, &ulen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				strcpy(BtName, DEFAULT_BT_NAME);//�������BtName,����ȡ������,ҲҪ����Ĭ��ֵ
			if((ret=NDK_BTSetLocalName(BtName))!=NDK_OK)
			{
				cls_show_msg("file:%s,line:%d������������ʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			//��������PIN
			cls_printf("��ǰ����PIN:\n");
			//Ϊ�˼��ݵͶ˲�ƷME15ʹ�÷�װ����
			if ((ret=lib_kbgetinput(BtPin,0, sizeof(BtPin)-1, &ulen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				strcpy(BtPin, DEFAULT_BT_PIN);//�������BtPin,����ȡ������,ҲҪ����Ĭ��ֵ
			if((ret=NDK_BTSetPIN(BtPin))!=NDK_OK)
			{
				cls_show_msg("file:%s,line:%d������������ʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			//�˳���������ģʽ
			if((ret=NDK_BTExitCommand())!=NDK_OK)
			{
				cls_show_msg("line %d:�˳�����ģʽʧ��(%d)", __LINE__, ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			break;
		case '2':
			//������������ģʽ
			if((ret=NDK_BTEnterCommand())!=NDK_OK)
			{
				cls_show_msg("line %d:��������ģʽʧ��(%d)", __LINE__, ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			cls_show_msg1(2,"�������������ģʽ(��ʾ:ƻ���ֻ�ֻ������just workģʽ)");
			nkey = cls_show_msg("1.Just Work\n"
								"2.PIN Code\n"
								"3.SSP PIN\n"
								"4.PassKey");
			switch(nkey)
			{	
				case '1':
					NDK_BTSetPairingMode(PAIRING_MODE_JUSTWORK);
					g_pair_mode = 0;
					cls_show_msg1(1,"����ΪJust Workģʽ");
					break;
				case '2':
					NDK_BTSetPairingMode(PAIRING_MODE_PINCODE);
					g_pair_mode = 1;
					cls_show_msg1(1,"����ΪPIN Codeģʽ");
					break;
				case '3':
					NDK_BTSetPairingMode(PAIRING_MODE_SSP);
					g_pair_mode = 2;
					cls_show_msg1(1,"����ΪSSP PINģʽ");
					break;
				case '4':
					NDK_BTSetPairingMode(PAIRING_MODE_PASSKEY);
					g_pair_mode = 3;
					cls_show_msg1(1,"����ΪPassKeyģʽ");
					break;
				default:
					NDK_BTSetPairingMode(PAIRING_MODE_JUSTWORK);
					g_pair_mode = 0;
					cls_show_msg1(1,"Ĭ��Just Workģʽ");
					break;
			}
			//�˳���������ģʽ
			if((ret=NDK_BTExitCommand())!=NDK_OK)
			{
				cls_show_msg("line %d:�˳�����ģʽʧ��(%d)", __LINE__, ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			break;
		case '3':
			if((ret = wati_paraing()) != NDK_OK && ret != 2)
			{				
				cls_show_msg("line %d:�������ʧ��(%d)",__LINE__,ret);
			}	
			break;
		case '4':
			bt_client_ras();
			break;
		case ESC:
			NDK_PortClose(PORT_NUM_BT);
			return;
			break;
		default:
			continue;
			break;
		}
	}
	NDK_SysMsDelay(1000);//��ʱ100ms  Ϊ�˱�����豸����Ͽ�  2017028 linying added
    if((ret=NDK_BTDisconnect()) != NDK_OK)
	{
		cls_show_msg("line %d:�Ͽ�����ʧ��(%d)", __LINE__, ret);
		return ;
	}
	if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		send_result("line %d:�������ڹر�ʧ��(%d)", __LINE__, ret);
		return;
	}
	return;
}
#endif

void systest57(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
#if 1//K21_ENABLE
	cls_show_msg1(5, "��ʾ:����ǰ�Ƚ�������,�쳣����������Ҳ������,���������");
#endif
	setlinktype(BT);//����ͨѶ����ΪBT���������ݰ�̫������ʧ��

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_BT()!=SUCC)
			return;
		g_CycleTime = 100;

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��ѹ������
		bt_press();

		//ִ�����ܲ���
		bt_ability();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while(1)
    {
		keyin = cls_show_msg("1.ѹ�� 2.����\n"
							 "0.���� 3.�쳣\n"
#if BT_MASTER_ENABLE
							 "4.�����ͻ���"
#endif
							 );
		switch(keyin) 
		{
			case '1':
				bt_press();
				break;
			case '2':	
				bt_ability();
				break;	
			case '3':
				bt_abnormal();
				break;
			case '0':
				conf_conn_BT();
#if BTMFI_ENABLE
			NDK_BTSetBleStatus(0);//��MFIЭ���£��ر�BLE��ʽ
#endif
				break;
#if BT_MASTER_ENABLE
			case '4':
				bt_client();
				break;
#endif
			case ESC:
				cls_show_msg("�������ô��޸��������ģʽ�����½��б���������");
				return;
			default:
				continue;
				break;
		}
	}	
	return;
}

