/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WLAN/WLM����
* file name		: systest45.c
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/08/06
* directory 		: 
* description		: WLAN/WLM��Ƚ���
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
#define	TESTITEM	"WLAN/WLM��Ƚ���"

/*------------global variables definition-----------------------*/
static int nConfInit[2] = {0};	 /*�Ƿ����ù��˲���:0δ���� 1������*/
char destipforwifi[16] = {0};
char destipforwlm[16] = {0};

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
static int GetPowerVol(int nFlag,uint* unVol)
{
	if(nFlag)
	{
		return NDK_SysGetPowerVol(unVol);
	}
	return SUCC;
}

void WLAN_WLM(int nWlmType,int nSelect)
{
	/*private & local definition*/
	int  i = 0, slen = 0, slen2 = 0, rlen = 0, rlen2 = 0, timeout = SO_TIMEO, succ = 0, nElectricFlag = 1, ret = 0;
	char buf[PACKMAXLEN] = {0},rbuf[PACKMAXLEN] = {0},rbuf2[PACKMAXLEN] = {0};
	Packet sendpacket;
	uint h_tcp = -1,h_tcp2 = -1, unVol = 0;
	
	/*����ǰ��*/
	if(nSelect != 2)
	{
		if(nConfInit[0] == 0)
		{
			cls_show_msg1(g_keeptime, "Wifif����δ����");
			return;
		}
	}
	if(nSelect != 1)
	{
		if(nConfInit[1] == 0)
		{
			cls_show_msg1(g_keeptime, "Wlm����δ����");	
			return;
		}
	}
	//���ȡ��ص����Ŀ���,Ĭ�Ͽ���Ϊ1֧��ȡ����������,WIFI������
	if(nSelect == 0)
	{
		if(cls_show_msg1(g_keeptime, "�Ƿ�ȡ��ص���\n1.��\n2.��")=='2')
			nElectricFlag=0;
		else
			nElectricFlag=1;
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

		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR;
		}
		
		/*Wlm����*/
		if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
		{
			Local2Global(nWlmType);
			if(__LinkUp_WLM()!=SUCC ||__NetUp_PPP()!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����Wlm����ʧ��",__LINE__,i,TESTITEM);
				goto ERR;
			}
			Global2Local(nWlmType);
		}
			
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR;
		}
		
		/* wifi����*/
		if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
		{
			Local2Global(WLAN);
			if(__LinkUp_WLAN()!=SUCC||__NetUp_WLAN()!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifi����ʧ��",__LINE__,i,TESTITEM);
				goto ERR2;
			}
			Global2Local(WLAN);
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR2;
		}
		
		/*Wlm���������*/
		//NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);//Ϊ֧�����ߺ�WIFI��������Ĺ��ܺ��� 20131120 linwl
		if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);//Ϊ֧�����ߺ�WIFI��������Ĺ��ܺ��� 20131120 linwl
			Local2Global(nWlmType);
			if(__TransUp(&h_tcp2)!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����Wlm���������ʧ��",__LINE__,i,TESTITEM);
				goto ERR3;
			}
			Global2Local(nWlmType);
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR3;
		}
		
		/*wifi���������*/
		//NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
			Local2Global(WLAN);
			if(__TransUp(&h_tcp)!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifi���������ʧ��",__LINE__,i,TESTITEM);
				goto ERR4;
			}
			Global2Local(WLAN);
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR4;
		}
		
		/*Wlm��������*/
		//NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);
		if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);
			Local2Global(nWlmType);
			if((slen2=sock_send(h_tcp2, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����Wlm��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, slen2, sendpacket.len);
				goto ERR4;
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR4;
		}
		
		/*wifi��������*/
		//NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
			Local2Global(WLAN);
			if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifi��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, slen, sendpacket.len);
				goto ERR4;
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR4;
		}
		
		/*wlm��������*/
		//NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);
		if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);
			Local2Global(nWlmType);
			memset(rbuf2, 0, sizeof(rbuf2));
			if ((rlen2=sock_recv(h_tcp2, rbuf2, sendpacket.len, timeout))!=sendpacket.len)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wlm��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, rlen2, sendpacket.len);
				goto ERR4;
			}
		}
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR4;
		}
			
		/*wifi��������*/
		//NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
			Local2Global(WLAN);
			memset(rbuf, 0, sizeof(rbuf));
			if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifi��������ʧ��(ʵ��:%d,Ԥ��:%d)",__LINE__,i,TESTITEM, rlen, sendpacket.len);
				goto ERR4;		
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR4;
		}

		/*У���շ�����*/
		if(nSelect!=2)
		{
			if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifiУ������ʧ��",__LINE__,i,TESTITEM);
				goto ERR4;
			}
		}
		
		if(nSelect!=1)
		{
			if(MemCmp(sendpacket.header, rbuf2, sendpacket.len))
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wlmУ������ʧ��",__LINE__,i,TESTITEM);
				goto ERR4;
			}
		}
		/*wifi�Ͽ������*/
		//NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
		if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,destipforwifi);
			Local2Global(WLAN);
			g_socktype = netsetting.socktype;
			if (__TransDown(h_tcp)!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifi�Ͽ������ʧ��",__LINE__,i,TESTITEM);
				goto ERR4;
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR3;
		}

		/*wlm�Ͽ������*/
		//NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);
		if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
		{
			NDK_NetAddRouterTable(COMM_TYPE_PPP,destipforwlm);
			Local2Global(nWlmType);
			g_socktype = netsetting.socktype;
			if (__TransDown(h_tcp2)!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wlm�Ͽ������ʧ��",__LINE__,i,TESTITEM);
				goto ERR3;
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR2;
		}

		/*wifi�Ͽ�����*/
		if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
		{
			Local2Global(WLAN);
			if(__NetDown_WLAN()!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����wifi�Ͽ�����ʧ��",__LINE__,i,TESTITEM);
				goto ERR2;
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR;
		}

		/*Wlm�Ͽ�����*/
		if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
		{
			Local2Global(nWlmType);
			setlinktype(nWlmType);
			if(__NetDown_PPP()!=SUCC)
			{
				cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%s����Wlm�Ͽ�����ʧ��",__LINE__,i,TESTITEM);
				goto ERR;
			}
		}
		
		//ȡ����
		if((ret = GetPowerVol(nElectricFlag,&unVol))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sȡ����%dʧ��(%d)",__LINE__,i,TESTITEM,unVol,ret);
			goto ERR;
		}
		succ++;
		continue;
		
		ERR4:
			if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
			{
				Local2Global(WLAN);
				g_socktype = netsetting.socktype;
				__TransDown(h_tcp);
			}
		ERR3:
			if(nSelect!=1)//nSelect==1��ʾWIFI/ȡ����
			{
				Local2Global(nWlmType);
				g_socktype = netsetting.socktype;
				__TransDown(h_tcp2);
			}
		ERR2:
			if(nSelect!=2)//nSelect==2��ʾWLM/ȡ����
			{
				Local2Global(WLAN);
				__NetDown_WLAN();
			}
		ERR:
			if(nSelect != 1)
			{
				Local2Global(nWlmType);
				setlinktype(nWlmType);
				__NetDown_PPP();
			}
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������,�ѽ���%d��,�ɹ�:%d��",TESTITEM,i-1,succ);
	return;

}

int cross_conf(void)
{
	int nSelect = 0;
	
	while(1)
	{
		nSelect = cls_show_msg("����ѡ������\n"
					   "1.WIFI/WLM���\n"
					   "2.WIFI/ȡ��������\n"
					   "3.WLM/ȡ��������\n");
		switch(nSelect)
		{
		case '1':
			return 0;
			break;
		case '2':
			return 1;
			break;
		case '3':
			return 2;
			break;
		default:
			break;
		}
	}
}

void systest45(void)
{
	/*private & local definition*/
	int keyin = 0 ,nWlmType = 0; /*���ߵ����� GPRS��CMDA��*/
	int nSelect = 0, tmpflag = 0; /*����ѡ�� 0: WIFI/WLAN���   1:WIFI/ȡ���� 2:WLAN/ȡ����*/
	
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
		
		if(conf_conn_WLM(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,ETH����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		//����wlm����
		nWlmType = getlinktype();
		Global2Local(nWlmType);
		strcpy(destipforwlm,netsetting.SvrIP.Str);
		NDK_NetAddRouterTable(COMM_TYPE_PPP,netsetting.SvrIP.Str);//Ϊ֧�����ߺ�WIFI��������Ĺ��ܺ��� 20131120 linwl
		nConfInit[1]=1;

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);		
		//ִ�н������
		for(nSelect=0;nSelect<3;nSelect++)//ִ��3�ֽ������
			WLAN_WLM(nWlmType,nSelect);	
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		SetWifiStopflag(tmpflag);
		return;
	}
	
	while (1)
	{	
		keyin = cls_show_msg("1.����ѹ��\n"
							"0.WIFI����\n"
							"8.WLM����\n"
							"9.����ѡ������");
		switch(keyin)
		{
		case '1':
			WLAN_WLM(nWlmType,nSelect);
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
				if((cls_show_msg("wifi������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER ) 	
				{
					NDK_NetAddRouterTable(COMM_TYPE_WIFI,netsetting.SvrIP.Str);//Ϊ֧�����ߺ�WIFI��������Ĺ��ܺ��� 20131120 linwl
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
		case '8':	
			//cls_show_msg("���������WLM��������...");
			switch(conf_conn_WLM(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����wlm����
				nWlmType = getlinktype();
				Global2Local(nWlmType);
				strcpy(destipforwlm,netsetting.SvrIP.Str);
				if((cls_show_msg("wlm������IP:%s,�Ƿ���Ҫ��ӵ�·�ɱ�?[ENTER]�ǣ�[����]��", netsetting.SvrIP.Str)) == ENTER ) 					
				{
					NDK_NetAddRouterTable(COMM_TYPE_PPP,netsetting.SvrIP.Str);//Ϊ֧�����ߺ�WIFI��������Ĺ��ܺ��� 20131120 linwl
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
		case '9':
			nSelect = cross_conf();
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

