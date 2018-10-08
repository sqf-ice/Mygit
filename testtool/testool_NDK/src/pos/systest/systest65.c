/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest65.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ��ӡ/BT����
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
#define	TESTITEM		"��ӡ/BT����"
#define	MAXWAITTIME	(120)	//30->120 for ���

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
static void print_BT(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0, ret = -1, cmdlen=0, recvLen=0;
	char sendBuf[PACKMAXLEN] = {0}, recvBuf[PACKMAXLEN] = {0};
	char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	Packet sendpacket;

	/*process body*/
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);

	if(lib_initprn(g_PrnSwitch)!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	print_bill();
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return; 						
	}
	while (1)
	{
		//��������
		NDK_PortClrBuf(PORT_NUM_BT);	
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", ++i, TESTITEM, succ_count)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
				
		//����������
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		cls_printf("���ڽ��д�ӡ��BT�������...");
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;
		}
		//�����建����
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;
		}
		
		//������������
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x05, sendpacket.header, sendpacket.len, cmdsendBuf);
		cmdlen = sendpacket.len+MIN_BTCMD_LEN;
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;
		}
		
		//������������
		memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			bt_comm_once(12,0);
			continue;
		}
		bt_cmd_uppacket(cmdrecvBuf, sendpacket.len, recvBuf);
		if(memcmp(sendBuf, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������У��ʧ��", __LINE__, i);
			continue;
		}
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;
		}
		
		//�Ͽ���������(�Ͷ˲�Ʒ��Ҫ���ô˺����Ͽ�����,�ж˲�Ʒ���ϴ˵���Ҳ��Ӱ��)
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x03, NULL, 0, cmdsendBuf);
		NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf);
		packetnum = 0;//���ӶϿ������к�Ҫ��0
		cls_show_msg1(1, "POS�˹Ҷ�����������...");
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��", __LINE__, i);
			continue;
		}
		//�ر���������
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, i);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;
		}		
		succ_count++;	
	}
		
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����:%d\n�ɹ�����:%d", i-1, succ_count);
	return;
}

void systest65(void)
{
	if(auto_flag==1)//֧���Զ�����
	{
		//�Զ�����������
		//BT���ò�����ʧ�����˳�
		if(conf_conn_BT()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,BT����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//prnt����
		conf_prn();
		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		print_BT();
		
		//��˳����������IC/SAM1/SAM2/SAM3��BT�Ľ���
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	while (1)
	{	
		keyin = cls_show_msg("1.�������\n"
							"0.BT����\n"
							"9.��ӡ����");
		switch(keyin)
		{
		case '1':
			print_BT();
			break;
		case '0':
			conf_conn_BT();
			break;
		case '9':
			conf_prn();
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

