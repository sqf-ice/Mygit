/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WLAN/ETH����
* file name		: systest44.c
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/08/07
* directory 		: 
* description		: WLAN/ETH��Ƚ���
* related document	: NL-GP730��Ա�ο��ֲ�.doc
*				  
************************************************************************
* log			: 
* Revision 1.0  2013/08/07 chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"WLAN/ETH��Ƚ���"

/*------------global variables definition-----------------------*/
static int nConfInit[2] = {0};	 /*�Ƿ����ù��˲���:0δ���� 1������*/
static char destipforwifi[16] = {0};
static char destipforeth[16] = {0};

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
*			  		  chensj    	20130807 		created
*****************************************************************/
void WLAN_ETH(void)
{
	/*private & local definition*/
	int  i = 0,slen = 0, slen2 = 0, rlen = 0, rlen2 = 0, timeout = SO_TIMEO,succ=0;
	uint h_tcp = -1,h_tcp2 = -1;
	char buf[PACKMAXLEN] = {0},rbuf[PACKMAXLEN] = {0},rbuf2[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1(g_keeptime, "Wifi����δ����");
		return;
	}
	
	if(nConfInit[1] == 0)
	{
		cls_show_msg1(g_keeptime, "ETH����δ����");	
		return;
	}

	/*process body*/
	//setlinktype(WLAN);
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	while (1)
	{		
		if(cls_show_msg1(3, "���ڽ��е�%d��%sͨѶ",++i ,TESTITEM)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		
		/*ETH����*/
		NDK_NetAddRouterTable(COMM_TYPE_ETH,destipforeth);
		Local2Global(ETH);
		if(__LinkUp_ETH()!=SUCC || __NetUp_ETH()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ETH����ʧ��",__LINE__,i,TESTITEM);
			goto ERR;
		}
		Global2Local(ETH);
		
		/* wifi����*/
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		Local2Global(WLAN);
		if(__LinkUp_WLAN()!=SUCC||__NetUp_WLAN()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifi����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}
		Global2Local(WLAN);
	
		/*ETH���������*/
		NDK_NetAddRouterTable(COMM_TYPE_ETH,destipforeth);
		Local2Global(ETH);
		if(__TransUp(&h_tcp2)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ETH���������ʧ��",__LINE__,i,TESTITEM);
			goto ERR3;
		}
		Global2Local(ETH);
		
		/*wifi���������*/
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		Local2Global(WLAN);
		if(__TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifi���������ʧ��",__LINE__,i,TESTITEM);
			goto ERR4;
		}
		Global2Local(WLAN);

		/*ETH��������*/
		NDK_NetAddRouterTable(COMM_TYPE_ETH,destipforeth);
		Local2Global(ETH);
		if((slen2=sock_send(h_tcp2, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ETH��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, slen2, sendpacket.len);
			goto ERR4;
		}
		
		/*wifi��������*/
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		Local2Global(WLAN);
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifi��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, slen, sendpacket.len);
			goto ERR4;
		}

		/*ETH��������*/
		NDK_NetAddRouterTable(COMM_TYPE_ETH,destipforeth);
		Local2Global(ETH);
		memset(rbuf2, 0, sizeof(rbuf2));
		if ((rlen2=sock_recv(h_tcp2, rbuf2, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ETH��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, rlen2, sendpacket.len);
			goto ERR4;
		}

		/*wifi��������*/
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		Local2Global(WLAN);
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifi����ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, rlen, sendpacket.len);
			goto ERR4;		
		}

		/*У���շ�����*/
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifiУ������ʧ��",__LINE__,i,TESTITEM);
			goto ERR4;
		}

		if(MemCmp(sendpacket.header, rbuf2, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wlmУ������ʧ��",__LINE__,i,TESTITEM);
			goto ERR4;
		}

		/*wifi�Ͽ������*/
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		Local2Global(WLAN);
		g_socktype = netsetting.socktype;
		if (__TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifi�Ͽ������ʧ��",__LINE__,i,TESTITEM);
			goto ERR4;
		}

		/*ETH�Ͽ������*/
		NDK_NetAddRouterTable(COMM_TYPE_ETH,destipforeth);
		Local2Global(ETH);
		g_socktype = netsetting.socktype;
		if (__TransDown(h_tcp2)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ETH�Ͽ������ʧ��",__LINE__,i,TESTITEM);
			goto ERR3;
		}

		/*wifi�Ͽ�����*/
		Local2Global(WLAN);
		if(__NetDown_WLAN()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����wifi�Ͽ�����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}

		/*ETH�Ͽ�����*/
		Local2Global(ETH);
		if(__NetDown_ETH()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ETH�Ͽ�����ʧ��",__LINE__,i,TESTITEM);
			goto ERR;
		}
		succ++;
		continue;
		
		ERR4:
			Local2Global(WLAN);
			g_socktype = netsetting.socktype;
			__TransDown(h_tcp);
		ERR3:
			Local2Global(ETH);
			g_socktype = netsetting.socktype;
			__TransDown(h_tcp2);
		ERR2:
			Local2Global(WLAN);
			__NetDown_WLAN();
		ERR:
			__NetDown_ETH();
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������,�ѽ���%d��,�ɹ�:%d��",TESTITEM,i-1,succ);
	return;

}

void systest44(void)
{
	/*private & local definition*/
	int keyin = 0, tmpflag = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=0)
		SetWifiStopflag(0);//ȷ����������ά��wifi���ӵ�����²���
		
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_WLAN()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WIFI����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		//����wlan����
		Global2Local(WLAN);
		strcpy(destipforwifi,netsetting.SvrIP.Str);
		//����ǰ������·�ɱ�
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);//�ú���ΪWIFI����·�ɱ�
		nConfInit[0]=1;
		
		if(conf_conn_ETH(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,ETH����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		//����eth����
		Global2Local(ETH);
		strcpy(destipforeth,netsetting.SvrIP.Str);
		NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);//�ú���Ϊ��̫������·�ɱ�
		nConfInit[1]=1;
		
		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		WLAN_ETH();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		SetWifiStopflag(tmpflag);
		return;
	}

	while (1)
	{	
		keyin = cls_show_msg("1.����ѹ��\n"
							"0.WIFI����\n"
							"9.ETH����\n");
		switch(keyin)
		{
		case '1':
			WLAN_ETH();
			break;
		case '0':
			//cls_show_msg("���������WLAN��������...");
			switch(conf_conn_WLAN())
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����wlan����
				Global2Local(WLAN);
				strcpy(destipforwifi,netsetting.SvrIP.Str);
				//����ǰ������·�ɱ�
				if((cls_show_msg("wifi������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER ) 	
				{
					NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);//�ú���ΪWIFI����·�ɱ�
					nConfInit[0]=1;
				}
				else
				{
					cls_show_msg("���������ã�ѡ����ȷ��·�ɱ�!!!");
					nConfInit[0]=0;
				}		
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
			//cls_show_msg("���������ETH��������...");
			switch(conf_conn_ETH(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����eth����
				Global2Local(ETH);
				strcpy(destipforeth,netsetting.SvrIP.Str);
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
			case ESC://�û�����
			default:
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

