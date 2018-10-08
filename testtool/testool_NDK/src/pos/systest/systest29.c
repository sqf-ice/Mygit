/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest29.c
* Author 			: chenjs
* version			: 
* DATE			: 20130227
* directory 	: 
* description	: MDM/LAN�������
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
#define	TESTITEM		"MDM/LAN����"

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
*			  		chenjs		   20130227	 		created
*****************************************************************/

static int LAN_dial_comm(Packet *sendpacket)
{
	/*private & local definition*/
	int ret = 0, slen = 0, rlen = 0, timeout = SO_TIMEO;
	uint h_tcp = -1;
	char rbuf[PACKMAXLEN] = {0};
	
	/*process body*/
	if(NetUp()!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:NetUpʧ��", __LINE__);
		return FAIL;
	}
	if((ret=TransUp(&h_tcp))!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	//LAN��������
	if((slen=sock_send(h_tcp, sendpacket->header, sendpacket->len, timeout))!=sendpacket->len)
	{
		cls_show_msg1(g_keeptime, "line %d:LAN��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket->len);
		goto ERR;
	}
	//LAN��������
	if((rlen=sock_recv(h_tcp, rbuf, sendpacket->len, timeout))!=sendpacket->len||(ret=MemCmp(sendpacket->header, rbuf, sendpacket->len)))
	{
		cls_show_msg1(g_keeptime, "line %d:LAN��������ʧ��(ʵ��%d, Ԥ��%d)(%d)", __LINE__, rlen, sendpacket->len, ret);
		goto ERR;
	}
	if((ret=TransDown(h_tcp))!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransDown�Ҷ�ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if(NetDown()!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:NetDownʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;
ERR:
	TransDown(h_tcp);
	NetDown();	
	return FAIL;
}

static void mdm_LAN(LinkType *lnktype)
{
	/*private & local definition*/
	int i = 0, succ1 = 0, succ2 = 0,cnt = 0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	float dial = 0.0, hang = 0.0;
	
	/*process body*/
	//��ʼ��������MDM��LAN����
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	if(sendpacket.forever==FALSE)
	{
		cnt = sendpacket.lifecycle;
		sendpacket.lifecycle = 2*cnt;  //�˴�ѭ����������������Ϊѭ���������ε���update_snd_packet
	}
	
	mdm_clrportbuf_all();
	while(1)
	{
		if(ESC==cls_show_msg1(2, "���ڽ��е�%d��%s����,��ESC�˳�", i+1, TESTITEM))
			break;
		
		//��MDM���͵İ���ʽ����Ϊ����ͨѶ��ʽ�µķ���
		setlinktype(lnktype[1]);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		
		//����MDMͨѶ
		if(mdm_dial_comm(MDMDialStr, &sendpacket, FALSE, &dial, &hang)==SUCC)
			succ1++;

		//LANͨѶ
		setlinktype(lnktype[0]);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		if(LAN_dial_comm(&sendpacket)==SUCC)
			succ2++;
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"�������%d��.MDM�ɹ�%d��,LAN�ɹ�%d��.", i, succ1, succ2);
	return;
}

void systest29(void)
{
	/*private & local definition*/
	int ret = 0;
	LinkType type[2] = {NONE, NONE};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_LAN()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����LAN����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		type[0] = getlinktype();

		if(conf_conn_MDM()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����MDM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		type[1] = getlinktype();

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		mdm_LAN(type);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.LAN����\n"
							"9.mdm����");
		
		switch(ret)
		{
		case '1':
			mdm_LAN(type);
			break;
		case '0':
			switch(conf_conn_LAN())
			{
			case SUCC:
				type[0] = getlinktype();
				cls_show_msg1(2, "�������óɹ�!");
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
		case '9':
			switch(conf_conn_MDM())
			{
			case SUCC:
				type[1] = getlinktype();
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

