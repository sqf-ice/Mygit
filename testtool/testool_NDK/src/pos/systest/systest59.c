/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	BT/MDM���
* file name			: systest59.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: BT/MDM���
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
#define	TESTITEM	"BT/MDM����"
#define MAXWAITTIME		10

/*------------global variables definition-----------------------*/
static int nConfInit[2] = {0};	 /*�Ƿ����ù��˲���:0δ���� 1������*/

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
					 	jiangym			20140228	created
*****************************************************************/	
void BT_MDM(int nMdmType)
{
	/*private & local definition*/
	int  ret = -1, i = 0, slen = 0, rlen = 0, recvLen = 0, succ = 0, cmdlen = 0;
	char sendBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0}, recvBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0};
	char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	Packet sendpacket;
		
	/*process body*/
	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "BT����δ����");
		return;
	}
	
	if(nConfInit[1] == 0)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "MDM����δ����");	
		return;
	}
	
	setlinktype(nMdmType);
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
	while (1)
	{		
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", ++i, TESTITEM, succ)==ESC)
			break;
		
		setlinktype(nMdmType);
		if(update_snd_packet(&sendpacket)!=SUCC)//ʹ��MDM���ݰ����в���
			break;
		/*MDM����*/
		Local2Global(nMdmType);
		if(nMdmType == ASYN)
		{
			if(__LinkUp_ASYN()!=SUCC)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ASYN����ʧ��", __LINE__, i);
				continue;
			}
		}
		else
		{
			if(__LinkUp_SDLC()!=SUCC)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��SDLC����ʧ��", __LINE__, i);
				continue;
			}
		}
		Global2Local(nMdmType);
		
		//����������
		Local2Global(BT);
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		Global2Local(BT);
		
		/*MDM���轨�������*/
		
		//�建����
		mdm_clrportbuf_all();
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		
		/*MDM��������*/
		Local2Global(nMdmType);
		if((slen=mdm_send(sendpacket.header, sendpacket.len))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��MDM����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
			goto ERR;
		}
		
		//������������
		Local2Global(BT);
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x05, sendpacket.header, sendpacket.len, cmdsendBuf);
		cmdlen = sendpacket.len+MIN_BTCMD_LEN;
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		
		/*MDM��������*/
		Local2Global(nMdmType);
		memset(recvBuf, 0, sizeof(recvBuf));
		if((rlen=mdm_rev(recvBuf, sendpacket.len, 20))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��MDM����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			goto ERR;
		}
		/*MDMУ���շ�����*/
		if(MemCmp(sendpacket.header, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��MDM����У��ʧ��", __LINE__, i);
			goto ERR;
		}
		
		//������������
		Local2Global(BT);
		memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			goto ERR;
		}
		bt_cmd_uppacket(cmdrecvBuf, sendpacket.len, recvBuf);
		if(memcmp(sendBuf, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������У��ʧ��", __LINE__, i);
			goto ERR;
		}
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		
		/*MDM�Ͽ�����*/
		Local2Global(nMdmType);
		cls_printf("MODEM�Ҷ���(��%d��)...", i);
		if(MDM_NetDown(nMdmType)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��MODEM�Ҷ�ʧ��", __LINE__, i);
			goto ERR;
		}
		
		//�ر���������
		Local2Global(BT);
		//�Ͽ���������
		cls_show_msg1(1, "POS�˹Ҷ�����������...");
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��", __LINE__, i);
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
		//�ر���������
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, i);
			continue;
		}			
		succ++;
		continue;
	ERR:
		Global2Local(BT);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		Global2Local(nMdmType);
		MDM_NetDown(nMdmType);
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"%s�������,�ѽ���%d��,�ɹ�:%d��",TESTITEM,i-1,succ);
	return;
}

int bt_comm(Packet *sendpacket)
{
	int ret = -1;
	
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����������ʧ��", __LINE__);
		return ret;
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:���ʧ��(%d)", __LINE__,ret);
		return ret;
	}
	if((ret=bt_comm_once(sendpacket->len,1))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:ͨѶʧ��(%d)", __LINE__, ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return ret;
	}
	//�Ͽ���������
	if((ret=NDK_BTDisconnect())!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ͽ���������ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_BT);
		return ret;
	}	
	//�ر���������
	if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�ر���������ʧ��(%d)", __LINE__, ret);
		return ret;
	}			
	return SUCC;
}
//BT��MDM��ǳ����
void bt_mdm_cross(int nMdmType)
{
	/*private & local definition*/
	int i = 0, cnt = 0, succ = 0, succ2 = 0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	float dial = 0.0, hang = 0.0;	
	
	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "BT����δ����");
		return;
	}
	if(nConfInit[1] == 0)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "MDM����δ����");	
		return;
	}
	/*process body*/
	//��ʼ��������BT��MDM����
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();
	
	if(sendpacket.forever==FALSE)  //������ѭ�����ε��õ�update_snd_packet���ʵ�ʴ�������
	{
		cnt = sendpacket.lifecycle;
		sendpacket.lifecycle = 2*cnt;
	}
	while (1)
	{		
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����,��ESC�˳�", ++i, TESTITEM)==ESC)
			break;
		
		//����ͨѶ
		Local2Global(BT);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		if(bt_comm(&sendpacket)==SUCC)
		{
			succ++;
		}
		Global2Local(BT);
		
		//MDM ͨѶ
		Local2Global(nMdmType);
		if(update_snd_packet(&sendpacket)!=SUCC)//ʹ��MDM���ݰ����в���
			break;
		if(mdm_dial_comm(MDMDialStr, &sendpacket, FALSE, &dial, &hang)==SUCC)
		{
			succ2++;
		}
		Global2Local(nMdmType);
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"ǳ�������%d��,BT�ɹ�:%d��,MDM�ɹ�:%d��",i-1,succ,succ2);
	return;
}

void systest59(void)
{
	/*private & local definition*/
	int ret = 0, nMdmType = 0; 
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_BT()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,BT����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		Global2Local(BT);
		nConfInit[0]=1;

		//MDM��������
		if(conf_conn_MDM()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,ETH����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		nConfInit[1]=1;
		nMdmType = getlinktype();
		Global2Local(nMdmType);
		g_CycleTime = 10;
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);	
		//ִ�н������
		BT_MDM(nMdmType);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.���ѹ��\n"
							"2.ǳ����ѹ��\n"
							"0.��������\n"
							"9.MDM����");
		
		switch(ret)
		{
		case '1':
			BT_MDM(nMdmType);
			break;
		case '2':
			bt_mdm_cross(nMdmType);
			break;	
		case '0':
			//cls_show_msg("���������BT��������...");
			switch(conf_conn_BT())
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����BT����
				Global2Local(BT);
				nConfInit[0]=1;
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
			case ESC://�û�����
			default:
				continue;//return;
				break;
			}
			break;
		case '9':	
			//cls_show_msg("���������MDM��������...");
			switch(conf_conn_MDM())
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����MDM����
				nConfInit[1]=1;
				nMdmType = getlinktype();
				Global2Local(nMdmType);
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
			case ESC://�û�����
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


