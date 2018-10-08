/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest72.c
* Author 			: huangjianb
* version			: 
* DATE			: 20141009
* directory 	: 
* description	: NFC/MDM�������
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
#define	TESTITEM		"NFC/MDM����"

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
*			  		  huangjb		   20141009	 	created
*****************************************************************/
static void nfc_modem(void)
{
	/*private & local definition*/
	int icount = 0, isucc = 0, ret = -1 ,pnRecvlen = 0;
	int send_len = 0, rec_len = 0 ,nSeekCnt = 3;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	uchar psRecebuf[64] = {0};
	
  /*process body*/		
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();

	cls_show_msg("����ֻ�NFC���Թ��߲�����pos...,���������");
	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", icount+1, TESTITEM, isucc)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		icount++;

		mdm_hangup();//��ʼ��ǰ�ı���
		//��ʼ��MODEM
		cls_printf("��ʼ��MODEM��(��%d��)...", icount);
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��ʼ��MODEM(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}
			
		//����
		cls_printf("MODEM������(��%d��)...", icount);
		if(mdm_dial(MDMDialStr)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:MODEM����(��%d��)ʧ��", __LINE__, icount);
			continue;
		}
		//����NFC����֮����Ҫ�и�2S������ʱ�䣬�����ֻ�����������Ӧ
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}

		//����ͨѶ
		cls_printf("MODEM����ͨѶ��(��%d��)...", icount);
		//��������
		if((send_len=mdm_send(sendpacket.header, sendpacket.len))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, send_len, sendpacket.len);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}

		//��������
		if((rec_len=mdm_rev(rbuf, sendpacket.len, 20))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rec_len, sendpacket.len);
			continue;
		}
		//�Ƚ�����
		if(MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:����У��ʧ��", __LINE__);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}

		//�Ҷ�
		cls_printf("MODEM�Ҷ���(��%d��)...", icount);
		if(mdm_hangup()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:MODEM�Ҷ�(��%d��)ʧ��", __LINE__, icount);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}

		isucc++;
	}
	
	cls_show_msg("ѭ����%d��\n�ɹ���%d��\n", icount, isucc);
	return;
}

void systest72(void)
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
							"0.MDM����");
		switch(keyin)
		{
		case '1':
			nfc_modem();
			break;
		case '0':			
			switch(conf_conn_MDM())
			{
			case SUCC:
				cls_show_msg1(2, "����modem�ɹ�!");
				break;
			case FAIL:
				cls_show_msg("line %d:����modemʧ��!!!\n������˳�����...", __LINE__);	
				break;
			case NDK_ERR_QUIT://�û�����
			default:
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
	
	return;
}

