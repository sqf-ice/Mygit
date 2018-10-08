/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	BT/ETH���
* file name			: systest61.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: BT/ETH���
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
#define	TESTITEM	"BT/ETH���"
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
void BT_ETH(void)
{
	/*private & local definition*/
	uint h_tcp = -1;
	int  ret = -1, i = 0, slen = 0, rlen = 0, recvLen = 0, succ=0, cmdlen = 0, timeout = SO_TIMEO;
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
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "ETH����δ����");	
		return;
	}
	
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
	while (1)
	{	
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", ++i, TESTITEM, succ)==ESC)
			break;
		Local2Global(BT);
		if(update_snd_packet(&sendpacket)!=SUCC)//ʹ��BT���ݰ����в���
			break;
		
		/*ETH����*/
		Local2Global(ETH);
		if(__LinkUp_ETH()!=SUCC || __NetUp_ETH()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETH����ʧ��",__LINE__,i);
			goto ERR;
		}
		Global2Local(ETH);
		
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
	
		/*ETH���������*/
		Local2Global(ETH);
		if(__TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETH TransUpʧ��", __LINE__, i);
			goto ERR2;
		}
		Global2Local(ETH);
			
		//�����建����
		Local2Global(BT);
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret);
			goto ERR3;
		}
		
		//ETH��������
		Local2Global(ETH);
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETH����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
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
		
		//ETH��������
		Local2Global(ETH);
		memset(recvBuf, 0, sizeof(recvBuf));
		if((rlen=sock_recv(h_tcp, recvBuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETH����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			goto ERR3;
		}
		//ETHУ���շ�����
		if(MemCmp(sendpacket.header, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETHУ��ʧ��", __LINE__, i);
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
		
		//ETH�Ͽ������
		Local2Global(ETH);
		g_socktype = netsetting.socktype;
		if (__TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETH TransDownʧ��", __LINE__, i);
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
		
		//ETH�Ͽ�����
		Local2Global(ETH);
		if(__NetDown_ETH()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ETH NetDownʧ��", __LINE__, i);
			goto ERR;
		}				
		succ++;
		continue;
	ERR3:
		Local2Global(ETH);
		g_socktype = netsetting.socktype;
		__TransDown(h_tcp);	
	ERR2:
		Local2Global(BT);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
	ERR:
		Local2Global(ETH);
		__NetDown_ETH();
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"%s�������,�ѽ���%d��,�ɹ�:%d��",TESTITEM,i-1,succ);
	return;
}
void systest61(void)
{
	/*private & local definition*/
	int ret = 0; 
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	if(auto_flag==1)//֧���Զ�����
	{
		//�Զ�����������
		//BT���ò�����ʧ�����˳�
		if(conf_conn_BT()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,BT����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		Global2Local(BT);
		nConfInit[0]=1;
		
		//ETH���ò�����ʧ�����˳�
		if(conf_conn_ETH(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,ETH����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		Global2Local(ETH);
		NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);//�ú���Ϊ��̫������·�ɱ�
		nConfInit[1]=1;
		
		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		BT_ETH();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.��������\n"
							"9.ETH����");
		
		switch(ret)
		{
		case '1':
			BT_ETH();
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
			switch(conf_conn_ETH(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����ETH����
				Global2Local(ETH);				
				if((cls_show_msg("ETH������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER ) 	
				{
					NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);//�ú���Ϊ��̫������·�ɱ�
					nConfInit[1]=1;
				}
				else
				{
					cls_show_msg("���������ã�ѡ����ȷ��·�ɱ�!!!");
					nConfInit[1]=0;
				}	
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


