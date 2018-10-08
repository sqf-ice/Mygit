/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest63.c
* Author 			:
* version			: 
* DATE				:
* directory 			: 
* description			: �ſ�/BT�������
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
#define	TESTITEM		"�ſ�/BT����"

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
static void card_BT(void)
{
	/*private & local definition*/
	int  ret = -1, i = 0, recvLen = 0, succ=0, cmdlen = 0, datalen = 0;
	char sendBuf[PACKMAXLEN] = {0}, recvBuf[PACKMAXLEN] = {0};
	char cmdBuf[BUFSIZE_BT] = {0};//, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
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

	i=0;
	while (1)
	{
		//��������
		//NDK_PortClrBuf(PORT_NUM_BT);	
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", ++i, TESTITEM, succ)==ESC)
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
		if(systest_MagCard()!=STRIPE)
			continue;
		
		//�����建���� //Ϊ���Եײ������Ϳ��ƴſ���IC����оƬ���ڻ�������ͻ������,����Ӧ��ʵ��ʹ�÷�ʽ�������建����� 20160912 linwl
		/*if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			continue;
		}	
		if(systest_MagCard()!=STRIPE)
			continue;*/
		
		//������������
		memset(cmdBuf, 0, sizeof(cmdBuf));
#if defined ME15CHX||defined ME15B 		
		bt_cmd_packet(0x05, sendBuf, datalen, cmdBuf);
#else
		bt_cmd_packet(0x05, sendpacket.header, sendpacket.len, cmdBuf);
#endif		
		//cmdlen = sendpacket.len+MIN_BTCMD_LEN;
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
		//���ˢ��Ҫ��,��������ܻᵼ�º����06ָ��δ��,�ֻ��˻�ش�9�ֽ����ݶ������´β������ݱȽ�ʧ��20170823
		if(systest_MagCard()!=STRIPE)
		{
			NDK_PortClrBuf(PORT_NUM_BT);//����ˢ�������������û����ն��ǵ��´�����У���
			continue;
		}
		sleep(1);
		
		//������������
		memset(cmdBuf, 0, sizeof(cmdBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
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
			continue;
		}
		if(systest_MagCard()!=STRIPE)
		{
			NDK_PortClrBuf(PORT_NUM_BT);
			continue;
		}
		
		//�Ͽ���������(�Ͷ˲�Ʒ��Ҫ���ô˺����Ͽ�����,�ж˲�Ʒ���ϴ˵���Ҳ��Ӱ��)
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
		if(systest_MagCard()!=STRIPE)
			continue;
		
		succ++;
	}
			
	cls_show_msg("�������:%d\n�ɹ�����:%d,ע��:��������֧��NLBluetoothTest����", i-1, succ);
	return;
}

void systest63(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{
		keyin = cls_show_msg("1.�������\n"
							 "0.BT����");
		switch(keyin)
		{
		case '1':
			card_BT();
			break;
		case '0':
			conf_conn_BT();
#if BTMFI_ENABLE
			NDK_BTSetBleStatus(0);//��MFIЭ���£��ر�BLE��ʽ
#endif
			break;
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

