/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			:  ETH/WLAN����
* file name			: 22.c
* Author 			: chensj
* version			: 
* DATE				: 20130521
* directory 		: 
* description		:  ETH/WLAN�������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"ETH/WLAN����"
/*------------global variables definition-----------------------*/
//static char destipforwifi[16] = {0};
//static char destipforeth[16] = {0};

/*----------global variables declaration("extern")--------------*/
static int nConfInit[2] = {0};	 /*�Ƿ����ù��˲���:0δ���� 1������*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		      chensj	        20130524	 	created
*****************************************************************/
void wlan_ETH(LinkSetting *lnk, NetworkSetting *net)
{
	/*private & local definition*/
	int total[2] = {0, 0}, succ[2] = {0, 0}, i = 0, cnt = 0;
	int slen = 0, rlen = 0, timeout = SO_TIMEO;
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0}, *type = NULL;
	Packet sendpacket;
	
	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1(g_keeptime, "Wifi����δ����");
		return;
	}
	if(nConfInit[1] == 0)
	{
		cls_show_msg1(g_keeptime, "Eth����δ����");	
		return;
	}

	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	// ETH/WLAN�������(����ǰ��Ҫ����·�������������ȫ������)
	if(sendpacket.forever==FALSE)  //������ѭ�����ε��õ�update_snd_packet���ʵ�ʴ�������
	{
		cnt = sendpacket.lifecycle;
		sendpacket.lifecycle = 2*cnt;
	}
	while (1)
	{
		TransDown(h_tcp);
		NetDown();//����	
		
		i = (total[0]+total[1])%2;//i = rand()%2; linwl20131009δ��߽���Ч������ʹ������ķ�ʽ
		memcpy(&linksetting, &lnk[i], sizeof(LinkSetting));
		memcpy(&netsetting, &net[i], sizeof(NetworkSetting));
		type = getlinktype()==ETH?"ETH":"WLAN";
		//ͨѶǰ����·�ɱ�����Ϊ��ȷ��ÿ��ͨѶTCPʹ���������ӷ�ʽ��׼ȷ��20180208
		if(getlinktype()==ETH)
			NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);
		else
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);
		if(cls_show_msg1(2, "���ڽ��е�%d��%sͨѶ(�ѳɹ�%d��)", total[i]+1, type, succ[i])==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		++total[i];
		
		if(NetUp()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s NetUpʧ��", __LINE__, total[i], type);
			continue;
		}
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s TransUpʧ��", __LINE__, total[i], type);
			continue;
		}
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, total[i], type, slen, sendpacket.len);
			continue;
		}
		memset(rbuf, 0, sizeof(rbuf));
		if((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, total[i], type, rlen, sendpacket.len);
			continue;
		}
		if(MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sУ��ʧ��", __LINE__, total[i], type);
			continue;
		}
		if(TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s TransDownʧ��", __LINE__, total[i], type);
			continue;
		}
		if(NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s NetDownʧ��", __LINE__, total[i], type);
			continue;
		}
		++succ[i];
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"WLANͨѶ(%s)����:%d �ɹ�����:%d", lnk[0].WLANEssid, total[0], succ[0]);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"ETHͨѶ����:%d �ɹ�����:%d", total[1], succ[1]);
	return;
}

void systest22(void)
{
	/*private & local definition*/
	LinkSetting links[2];
	NetworkSetting nets[2];
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_WLAN()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WIFI����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//����wlan����
		memcpy(&links[0], &linksetting, sizeof(LinkSetting));
		memcpy(&nets[0], &netsetting, sizeof(NetworkSetting));
		//strcpy(destipforwifi,netsetting.SvrIP.Str);
		NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);//�ú���ΪWIFI����·�ɱ�
		nConfInit[0]=1;
		
		if(conf_conn_ETH(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,ETH����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//����eth����
		memcpy(&links[1], &linksetting, sizeof(LinkSetting));
		memcpy(&nets[1], &netsetting, sizeof(NetworkSetting));
		//strcpy(destipforeth,netsetting.SvrIP.Str);
		NDK_NetAddRouterTable(COMM_TYPE_ETH,netsetting.SvrIP.Str);//�ú���Ϊ��̫������·�ɱ�
		nConfInit[1]=1;

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		wlan_ETH(links, nets);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
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
			wlan_ETH(links, nets);
			break;
		case '0':
			//cls_show_msg("���������WLAN��������...");
			switch(conf_conn_WLAN())
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����wlan����
				memcpy(&links[0], &linksetting, sizeof(LinkSetting));
				memcpy(&nets[0], &netsetting, sizeof(NetworkSetting));
				//strcpy(destipforwifi,netsetting.SvrIP.Str);
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
			case NDK_ERR_QUIT://�û�����
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
				memcpy(&links[1], &linksetting, sizeof(LinkSetting));
				memcpy(&nets[1], &netsetting, sizeof(NetworkSetting));
				//strcpy(destipforeth,netsetting.SvrIP.Str);
				if((cls_show_msg("eth������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER ) 					
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
