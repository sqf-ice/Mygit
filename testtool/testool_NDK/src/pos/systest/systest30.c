/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest30.c
* Author 			: chensl
* version			: 
* DATE			: 20130124
* directory 		: 
* description		: ����/LAN����
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
#define	TESTITEM		"����/LAN����"

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
* history 		 		: author			date			remarks
*			  		chensl		   20130124	 		created
*****************************************************************/
static void LAN_wlm(LinkSetting *lnk, NetworkSetting *net)
{
	/*private & local definition*/
	int ret = 0, timeout = SO_TIMEO, slen = 0, rlen = 0, total[2] = {0, 0}, succ[2] = {0, 0}, i = 0, cnt = 0;
	uint h_tcp = 0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0}, *msg[2] = {"LAN","WLM"};
	LinkType linktype1;
	
	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1(g_keeptime, "LAN����δ����");
		return;
	}
	if(nConfInit[1] == 0)
	{
		cls_show_msg1(g_keeptime, "Wlm����δ����");	
		return;
	}
	
	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	if(sendpacket.forever==FALSE)  //������ѭ�����ε��õ�update_snd_packet���ʵ�ʴ�������
	{
		cnt = sendpacket.lifecycle;
		sendpacket.lifecycle = 2*cnt;
	}
	//LAN/WLM�������(����ǰ��Ҫ����·�������������ȫ������)
	while (1)
	{
		TransDown(h_tcp);
		NetDown();//����
		
		if((i=(total[0]+total[1])%2)) //if(i=rand()%2) linwl20131009δ��߽���Ч������ʹ������ķ�ʽ
			sleep(10);
		memcpy(&linksetting, &lnk[i], sizeof(LinkSetting));
		memcpy(&netsetting, &net[i], sizeof(NetworkSetting));
		//��ÿ�ν���ͨѶǰ��Ҫ����·�ɱ�����,���ܱ�֤TCP���ͨ�������߻���WiFi������̫�� sull add 20180208
		linktype1 = getlinktype();
		switch(linktype1)
		{
			case ETH:
				if((ret = NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str)) != NDK_OK)	
				{
					cls_show_msg1(g_keeptime, "line %d:��%d��ETH����·�ɱ�ʧ��", __LINE__, total[i]);
					continue;
				}
				break;
			case WLAN:
				if((ret = NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str)) != NDK_OK)	
				{
					cls_show_msg1(g_keeptime, "line %d:��%d�δ���·�ɱ�ʧ��", __LINE__, total[i]);
					continue;
				}
				break;
			case GPRS:
			case CDMA:
			case ASYN:
			case TD:
				if((ret = NDK_NetAddRouterTable(COMM_TYPE_PPP,netsetting.SvrIP.Str)) != NDK_OK)	
				{
					cls_show_msg1(g_keeptime, "line %d:��%d�δ���·�ɱ�ʧ��", __LINE__, total[i]);
					continue;
				}
				break;
			default:
				continue;
				break;
		}		
		
		if(ESC==cls_show_msg1(2, "���ڽ��е�%d��%sͨѶ(�ѳɹ�%d��),��ESC�˳�", total[i]+1, msg[i], succ[i]))
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		++total[i];
		
		//NetUp
		if(NetUp()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s NetUpʧ��", __LINE__, total[i], msg[i]);
			continue;
		}
		
		//TransUp
		if((ret=TransUp(&h_tcp))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s TransUpʧ��(%d)", __LINE__, total[i], msg[i], ret);
			continue;
		}
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s sendʧ��(ʵ��%d, Ԥ��%d)", __LINE__, total[i], msg[i], slen, sendpacket.len);
			continue;
		}
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len||MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s recvʧ��(ʵ��%d, Ԥ��%d)", __LINE__, total[i], msg[i], slen, sendpacket.len);
			continue;
		}

		//TransDown
		if((ret=TransDown(h_tcp))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s TransDownʧ��(%d)", __LINE__, total[i], msg[i], ret);
			continue;
		}
		
		//NetDown
		if(NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s NetDownʧ��", __LINE__, total[i], msg[i]);
			continue;
		}		
		++succ[i];
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"LAN��ִ�д���Ϊ%d,�ɹ�%d��", total[0], succ[0]);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"WLM��ִ�д���Ϊ%d,�ɹ�%d��", total[1], succ[1]);
	return;
}

void systest30(void)
{
	/*private & local definition*/
	int ret = 0;
	LinkSetting lnks[2];
	NetworkSetting nets[2];
	
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
		//����lan����
		memcpy(&lnks[0], &linksetting, sizeof(LinkSetting));
		memcpy(&nets[0], &netsetting, sizeof(NetworkSetting));
		if(getlinktype()==ETH)
			NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);//�ú���Ϊ��̫������·�ɱ�
		else
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);//�ú���ΪWIFI����·�ɱ�
		nConfInit[0]=1;
		
		if(conf_conn_WLM(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����WLM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//����wlm����
		memcpy(&lnks[1], &linksetting, sizeof(LinkSetting));
		memcpy(&nets[1], &netsetting, sizeof(NetworkSetting));
		NDK_NetAddRouterTable(COMM_TYPE_PPP,netsetting.SvrIP.Str);//�ú���Ϊ���ߴ���·�ɱ�
		nConfInit[1]=1;

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);	
		//ִ�н������
		LAN_wlm(lnks, nets);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.LAN����\n"
							"9.WLM����");
		
		switch(ret)
		{
		case '1':
			LAN_wlm(lnks, nets);
			break;
		case '0':
			switch(conf_conn_LAN())
			{
			case SUCC:
				cls_show_msg1(2, "LAN���óɹ�!");
				//����lan����
				memcpy(&lnks[0], &linksetting, sizeof(LinkSetting));
				memcpy(&nets[0], &netsetting, sizeof(NetworkSetting));
				if(getlinktype()==ETH)
				{
					if((cls_show_msg("ETH������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER )	
					{
						NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);//�ú���Ϊ��̫������·�ɱ�
						nConfInit[0]=1;
					}
					else
					{
						cls_show_msg("���������ã�ѡ����ȷ��·�ɱ�!!!");
						nConfInit[0]=0;
					}	
				}
				else
				{
					if((cls_show_msg("WIFI������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER )	
					{
						NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);//�ú���ΪWIFI����·�ɱ�
						nConfInit[0]=1;
					}
					else
					{
						cls_show_msg("���������ã�ѡ����ȷ��·�ɱ�!!!");
						nConfInit[0]=0;
					}	
				}
				break;
			case FAIL:
				cls_show_msg("line %d:LAN����δ��ͨ!!!\n������˳�����...", __LINE__);
				//return;
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				//return;
				break;
			}
			break;
		case '9':
			switch(conf_conn_WLM(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "WLM���óɹ�!");
				//����wlm����
				memcpy(&lnks[1], &linksetting, sizeof(LinkSetting));
				memcpy(&nets[1], &netsetting, sizeof(NetworkSetting));
				if((cls_show_msg("wlm·�ɱ�:%s,�Ƿ�Ϊ��Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER ) 	
				{
					NDK_NetAddRouterTable(COMM_TYPE_PPP,netsetting.SvrIP.Str);//�ú���Ϊ���ߴ���·�ɱ�
					nConfInit[1]=1;
				}
				else
				{
					cls_show_msg("���������ã�ѡ����ȷ��·�ɱ�!!!");
					nConfInit[1]=0;
				}	
				break;
			case FAIL:
				cls_show_msg("line %d:WLM����δ��ͨ!!!\n������˳�����...", __LINE__);
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

