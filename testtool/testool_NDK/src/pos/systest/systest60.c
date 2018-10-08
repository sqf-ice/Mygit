/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	BT/WLM���
* file name			: systest60.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: BT/WLM���
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
#define	TESTITEM	"BT/WLM���"
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
					 	jiangym			20140304	created
*****************************************************************/	
void BT_WLM(int nWlmType)
{
	/*private & local definition*/
	uint h_tcp = -1;
	int  ret = -1, i = 0, slen = 0, rlen = 0, recvLen = 0, succ=0, cmdlen = 0, nSq = 0, timeout = SO_TIMEO;
	char sendBuf[PACKMAXLEN] = {0}, recvBuf[PACKMAXLEN] = {0};//��BUFSIZE_BT-MIN_BTCMD_LEN�޸ĳ�PACKMAXLEN���������ں�������̫��ģ���ڳ�ʼ������ʱ����Ļ���С������ʱ�ĳ��������ܷ�
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
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "WLM����δ����");	
		return;
	}
	
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
	while (1)
	{	
		NDK_WlModemGetSQ(&nSq);
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�(�ź�ֵ:%d)", ++i, TESTITEM, succ, nSq)==ESC)
			break;
		Local2Global(BT);
		if(update_snd_packet(&sendpacket)!=SUCC)//ʹ��BT���ݰ����в���
			break;
		//sleep(10);//ÿ�ιҶ�֮����������Ҫ�ȴ�10��,������оƬ��ѹ��
		
		//WLM����
		Local2Global(nWlmType);
		if(__LinkUp_WLM()!=SUCC ||__NetUp_PPP()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM����ʧ��",__LINE__,i);
			goto ERR;
		}
		Global2Local(nWlmType);

		//����������
		Local2Global(BT);
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��(%d)", __LINE__, i, ret);
			goto ERR2;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, i, ret);
			goto ERR2;
		}
		Global2Local(BT);
		
		/*WLM���������*/
		Local2Global(nWlmType);
		if(__TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM TransUpʧ��", __LINE__, i);
			goto ERR2;
		}
		Global2Local(nWlmType);
		
		//�����建����
		Local2Global(BT);
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			goto ERR3;
		}	
		
		/*Wlm��������*/
		Local2Global(nWlmType);
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
			goto ERR3;
		}
		
		//������������
		Local2Global(BT);
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x05, sendpacket.header, sendpacket.len, cmdsendBuf);
		cmdlen = sendpacket.len+MIN_BTCMD_LEN;
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			goto ERR3;
		}
		
		/*wlm��������*/
		Local2Global(nWlmType);
		memset(recvBuf, 0, sizeof(recvBuf));
		if((rlen=sock_recv(h_tcp, recvBuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			goto ERR3;
		}
		//WLMУ���շ�����
		if (MemCmp(sendpacket.header, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM����У��ʧ��", __LINE__, i);
			goto ERR3;
		}
		
		//������������
		Local2Global(BT);
		memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���������մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, cmdlen);
			bt_comm_once(12,0);
			goto ERR3;
		}
		bt_cmd_uppacket(cmdrecvBuf, sendpacket.len, recvBuf);
		if(memcmp(sendBuf, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������У��ʧ��", __LINE__, i);
			goto ERR3;
		}
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������ʧ��(%d)", __LINE__, i, ret);
			goto ERR3;
		}
		
		//WLM TransDown
		Local2Global(nWlmType);
		if (__TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM TransDownʧ��", __LINE__, i);
			goto ERR3;
		}	
		
		//�ر���������
		Local2Global(BT);
		//�Ͽ���������
		memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
		bt_cmd_packet(0x03, NULL, 0, cmdsendBuf);
		NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf);
		packetnum = 0;//���ӶϿ������к�Ҫ��0
		cls_show_msg1(1, "POS�˹Ҷ�����������...");
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��", __LINE__, i);
			goto ERR2;
		}
		//�ر���������
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, i);
			goto ERR2;
		}
		
		//WLM NetDown
		Local2Global(nWlmType);
		if (__NetDown_PPP()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WLM NetDownʧ��", __LINE__, i);
			goto ERR;
		}					
		succ++;
		continue;
	ERR3:
		Local2Global(nWlmType);
		g_socktype = netsetting.socktype;
		__TransDown(h_tcp);	
	ERR2:
		Local2Global(BT);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
	ERR:
		Local2Global(nWlmType);
		__NetDown_PPP();
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"%s�������,�ѽ���%d��,�ɹ�:%d��",TESTITEM,i-1,succ);
	return;
}

void systest60(void)
{
	/*private & local definition*/
	int ret = 0, nWlmType = 0;; 
	
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
		if(conf_conn_WLM(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WLM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		nWlmType = getlinktype();
		Global2Local(nWlmType);
		nConfInit[1]=1;
		
		g_CycleTime = 10;
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);	
		//ִ�н������
		BT_WLM(nWlmType);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.��������\n"
							"9.WLM����");
		
		switch(ret)
		{
		case '1':
			BT_WLM(nWlmType);
			break;
		case '0':
			//cls_show_msg("���������BT��������...");
			switch(conf_conn_BT())
			{
			case SUCC:
				cls_show_msg1(2, "BT���óɹ�!");
				//����wlan����
				Global2Local(BT);
				nConfInit[0]=1;
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
			case NDK_ERR_QUIT://�û�����
			default:
				continue;//return;
				break;
			}
			break;
		case '9':	
			switch(conf_conn_WLM(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����wlm����
				nWlmType = getlinktype();
				Global2Local(nWlmType);
				nConfInit[1]=1;
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				continue;//return;
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


