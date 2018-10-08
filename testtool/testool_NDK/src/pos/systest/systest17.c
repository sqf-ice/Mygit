/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest17.c
* Author 			: chensl
* version			: 
* DATE			: 20130124
* directory 	: 
* description	: �ſ�/LAN�������
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
#define	TESTITEM		"�ſ�/LAN����"

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
static void card_LAN(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0;
	uint h_tcp = -1;
	int slen = 0, rlen = 0, timeout = SO_TIMEO;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}
	systest_MagCard();
	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ_count)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;

		TransDown(h_tcp);//UPǰ�ı���
		//UP
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		if(systest_MagCard()!=STRIPE)
			continue;
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
			continue;
		}
		if(systest_MagCard()!=STRIPE)
			continue;		
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
			continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
			continue;
		}
		if(systest_MagCard()!=STRIPE)
			continue;		
		//DOWN
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:TransDownʧ��", __LINE__);
			continue;
		}
		if(systest_MagCard()!=STRIPE)
			continue;
		
		succ_count++;
	}
	
	NetDown();
	systest_MagCard();
			
	cls_show_msg("ͨѶ����:%d\n�ɹ�����:%d", i, succ_count);
	return;
}

void systest17(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0, tmpflag = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=0)
		SetWifiStopflag(0);//ȷ����������ά��wifi���ӵ�����²���
	
	while (1)
	{
		keyin = cls_show_msg("1.�������\n"
							"0.LAN����");
		switch(keyin)
		{
		case '1':
			card_LAN();
			break;
		case '0':
			switch(conf_conn_LAN())
			{
			case SUCC:
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				//return;		
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				//return;
				break;
			}
			break;
		case ESC:
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

