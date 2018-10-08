/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest75.c
* Author 			:huangjianb
* version			: 
* DATE				:20141009
* directory 			: 
* description			: NFC/BT�������
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
#define	TESTITEM		"NFC/BT����"

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
*			  		huangjianb		20141009	created
*****************************************************************/
static void nfc_BT(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0, ret = -1 ,nSeekCnt = 3 ,pnRecvlen = 0;
	int  rlen = 0, timeout = SO_TIMEO;
	char buf[BUFSIZE_BT] = {0}, rbuf[BUFSIZE_BT] = {0};
	Packet sendpacket;
	uchar psRecebuf[64] = {0};

	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);	
	while (1)
	{
		//��������
		NDK_PortClrBuf(PORT_NUM_BT);
		NDK_PortClose(PORT_NUM_BT);
		
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", ++i, TESTITEM, succ_count)==ESC)
			break;
		setlinktype(BT);//���� BT���ݰ�
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;

		//����������
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�δ���������ʧ��", __LINE__, i);
			continue;
		}
		if(g_pair_mode == 2 || g_pair_mode == 3)// SSP PIN��PassKey�������ģʽ ��Ҫ�������״̬��ѯ�����ȷ�Ϻ���������Բ���
		{	
			cls_show_msg("��֮ǰ����Ե�����ȡ����Ժ����������");
			cls_printf("�ֻ�ʹ��ISSC��POS�������");
			if((ret=lib_bt_pairing(1))!=NDK_OK)
			{
				cls_show_msg("line %d:���ʧ��,���������(%d)", __LINE__,ret);
				NDK_BTDisconnect();
				continue;
			}
		}
		else //Just Work �� PIN Code�������ģʽ����ֱ�Ӳ������
			cls_show_msg("ƻ���ֻ�ʹ��BleTest��׿�ֻ�ʹ��ISSC,��POS���Ӻ�(BleTest��Ҫ�㿪ʼ�ش�),���������");
		//cls_show_msg("ʹ���ֻ���������ISSC��POS���Ӻ�,���������");
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		//�����建����
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1(g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			continue;
		}	
		//����NFC����֮����Ҫ�и�2S������ʱ�䣬�����ֻ�����������Ӧ
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		
		//������������
		if((ret=NDK_PortWrite(PORT_NUM_BT, sendpacket.len, sendpacket.header))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		//������������
		memset(rbuf, 0, sizeof(rbuf));
		if((ret=NDK_PortRead(PORT_NUM_BT,sendpacket.len, rbuf, timeout*1000, &rlen)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d���������մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, rlen, sendpacket.len);
			continue;
		}
		//�Ƚ��շ�
		if(rlen != sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�����������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			continue;	
		}
		if(MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d����������У��ʧ��", __LINE__, i);
			continue;	
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		//�ر���������
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, i);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		succ_count++;
	}
			
	cls_show_msg("�������:%d\n�ɹ�����:%d", i-1, succ_count);
	return;
}

void systest75(void)
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
			nfc_BT();
			break;
		case '0':
			conf_conn_BT();
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

