/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WLAN/MDM��Ƚ���
* file name		: systest46.c
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/08/08
* directory 		: 
* description		: WLAN/MDM��Ƚ���
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
#define	TESTITEM	"WLAN/MDM��Ƚ���"

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
*			  		  chensj    	20130808 		created
*****************************************************************/
void WLAN_MDM(int nMdmType)
{
	/*private & local definition*/
	int  i = 0, slen = 0, slen2 = 0, rlen = 0, rlen2 = 0, timeout = SO_TIMEO,succ=0;
	char buf[PACKMAXLEN] = {0},rbuf[PACKMAXLEN] = {0},rbuf2[PACKMAXLEN] = {0};
	Packet sendpacket;
	uint h_tcp = -1;
		
	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1(g_keeptime, "Wifi����δ����");
		return;
	}
	
	if(nConfInit[1] == 0)
	{
		cls_show_msg1(g_keeptime, "MDM����δ����");	
		return;
	}

	/*process body*/
	setlinktype(nMdmType);
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();
	while (1)
	{		
		if(cls_show_msg1(3, "���ڽ��е�%d��%sͨѶ",++i ,TESTITEM)==ESC)
			break;
		
		setlinktype(nMdmType);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		
		/*MDM����*/
		Local2Global(nMdmType);
		if(nMdmType == ASYN)
		{
			if(__LinkUp_ASYN()!=SUCC)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
				continue;
			}
		}
		else
		{
			if(__LinkUp_SDLC()!=SUCC)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
				continue;
			}
		}
		Global2Local(nMdmType);
		
		/*wifi����*/
		Local2Global(WLAN);
		if(__LinkUp_WLAN()!=SUCC||__NetUp_WLAN()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR;
		}
		Global2Local(WLAN);

		/*MDM���轨�������*/
		
		/*wifi���������*/
		Local2Global(WLAN);
		if(__TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}
		Global2Local(WLAN);

		/*MDM��������*/
		Local2Global(nMdmType);
		if((slen2=mdm_send(sendpacket.header, sendpacket.len))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}
		
		/*wifi��������*/
		Local2Global(WLAN);
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}

		/*MDM��������*/
		Local2Global(nMdmType);
		memset(rbuf2, 0, sizeof(rbuf2));
		if ((rlen2=mdm_rev(rbuf2, sendpacket.len, 30))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}
		if(MemCmp(sendpacket.header, rbuf2, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}
		
		/*wifi��������*/
		Local2Global(WLAN);
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;		
		}
		/*У���շ�����*/
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}

		/*wifi�Ͽ������*/
		Local2Global(WLAN);
		g_socktype = netsetting.socktype;
		if (__TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR2;
		}

		/*MDMû�д����,����Ͽ�*/

		/*wifi�Ͽ�����*/
		Local2Global(WLAN);
		if(__NetDown_WLAN()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR;
		}

		/*MDM�Ͽ�����*/
		if(MDM_NetDown(nMdmType)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%s����ʧ��",__LINE__,i,TESTITEM);
			goto ERR;
		}
		
		succ++;
		continue;
		
		ERR2:
			Local2Global(WLAN);
			g_socktype = netsetting.socktype;
			__TransDown(h_tcp);
		ERR:
			Local2Global(WLAN);
			__NetDown_WLAN();
			MDM_NetDown(nMdmType);
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������,�ѽ���%d��,�ɹ�:%d��",TESTITEM,i-1,succ);
	return;

}

void systest46(void)
{
	/*private & local definition*/
	int keyin = 0,nMdmType = 0, tmpflag = 0; /*��¼ͬ�������첽ͨѶ*/ 
		
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
		nConfInit[0]=1;
		
		if(conf_conn_MDM()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,ETH����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		//����mdm����
		nConfInit[1]=1;
		nMdmType = getlinktype();
		Global2Local(nMdmType);

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);		
		//ִ�н������
		WLAN_MDM(nMdmType);		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		SetWifiStopflag(tmpflag);
		return;
	}
	
	while (1)
	{	
		keyin = cls_show_msg("1.����ѹ��\n"
							"0.WIFI����\n"
							"9.MDM����\n");
		switch(keyin)
		{
		case '1':
			WLAN_MDM(nMdmType);
			break;
		case '0':
			//cls_show_msg("���������WLAN��������...");
			switch(conf_conn_WLAN())
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				//����wlan����
				Global2Local(WLAN);
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
				//����mdm����
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
