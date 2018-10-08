/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �����ۺϲ���
* file name		: systest4.c
* Author 			:  chensj
* version			: V1.0
* DATE			:20130204
* directory 		: 
* description		: �����ۺϲ���
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130204 chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define 	UCID  4
#define	TESTITEM	"��������,ѹ��"

#define	DEFAULT_CNT_STR	"100"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(200)

//#define	wireless_ability	WLM_ability
#define PACKETLIFE 30//�����������ѹ������

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
*			  		  chensj		   20130204  		created
*							
*****************************************************************/
#if CPU5810X_ENABLE
Network_t select_nettype(void)//ѡ������ģʽ����20180724 liny added
{
	/*private & local definition*/
	int nKeyin = 0, nettype=-1;
	char pInfo[64] = {0};
	Network_t type[] = {second_G,third_G,fourth_G};
	
	/*process body*/
	while (1)
	{
		if(auto_flag==1)
		{
			if(GetParaInfo("network",pInfo)!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%s)", __LINE__, pInfo);
				return fourth_G;//�����ļ�����ʧ��ʱĬ������4G����
			}
			if(trim_string2int(pInfo,&nettype)!=SUCC)
			{
				 cls_show_msg_record(FILENAME,FUNCNAME,"line =%d:�������ļ�ʧ��(%d)", __LINE__, nettype);
				 return fourth_G;
			}
			switch(nettype)
			{
				case 1:
				case 2:
				case 3:
					return type[nettype-1];
					break;
				case ESC:
				default:
					cls_show_msg1(2, "������Ϊ4G����(Ĭ��)...");
					return fourth_G;
					break;
			}
		}
		else
		{
			nKeyin = cls_show_msg("��������\n"
								"1.2G����\n"
								"2.3G����(�ƶ���3G)\n"
								"3.4G����\n"
								);
		
			switch(nKeyin)
			{
				case '1':
				case '2':
				case '3':
					return type[nKeyin-'1'];
					break;
				case ESC:
					cls_show_msg1(2, "������Ϊ4G(Ĭ��)...");
					return fourth_G;
					break;
				default:
					 continue;
					 break;
			}
		}
	}
}
 int conf_conn_setseg(Network_t type)//��������ģʽ����ATָ��20180727 liny added
{	
	int nRet=0;
	char *p = NULL,*q=NULL,*m=NULL,*style=NULL,*style1=NULL,*style2=NULL;
	char sRecvStr[100]={0};
	ST_ATCMD_PACK atm_cmd;
	EM_WLM_STATUS  pemStatus = 0;
	atm_cmd.AtCmdNo=WLM_CMD_UNDEFINE;
	atm_cmd.pcAddParam=NULL;
	time_t oldtime=0;
	
	oldtime=time(NULL);
	switch(type)
	{
		case second_G:
			//����ATָ���л�2G����
			//����ATָ�һ������Ҫ�ж��Ƿ��ǵ���
			atm_cmd.pcAtCmd="+CIMI";
			memset(sRecvStr,0,sizeof(sRecvStr));
			if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
			{ 
				 cls_show_msg1(g_keeptime,"line %d:��ȡ��Ӫ����Ϣʧ��(%d)", __LINE__, nRet);
				 return FAIL;
			}
			style=strstr(sRecvStr,"46003");
			style1=strstr(sRecvStr,"46005");
			style2=strstr(sRecvStr,"46011");
			memset(sRecvStr,0,sizeof(sRecvStr));
			if(style!=NULL||style1!=NULL||style2!=NULL)//�Ƿ�Ϊ����
			{
				atm_cmd.pcAtCmd="+qcfg=\"nwscanmode\",6";//����2G ATָ��
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
				{ 
					 cls_show_msg1(g_keeptime,"line %d:�л�3G����ʧ��(%d)", __LINE__, nRet);
					 return FAIL;
				}
			}
			else
			{
				atm_cmd.pcAtCmd="+qcfg=\"nwscanmode\",1";	
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
				{ 
					 cls_show_msg1(g_keeptime,"line %d:�л�2G����ʧ��(%d)", __LINE__, nRet);
					 return FAIL;
				}	
			}
			atm_cmd.pcAtCmd="+COPS?";			
			memset(sRecvStr,0,sizeof(sRecvStr));
			while(1)
			{
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,1000,&pemStatus))!=NDK_OK)
				{ 
					cls_show_msg1(g_keeptime,"line %d:�л�2G����ʧ��(%d)", __LINE__, nRet);
					return FAIL;
				 }
				 p=strstr(sRecvStr,"\",0");//�ƶ���ͨ2G��־
				 q=strstr(sRecvStr,"\",100");//����2G��־
				 if(p!=NULL||q!=NULL)
				 {
					cls_show_msg1(g_keeptime,"2G�������óɹ�");
					break;
				}
				if(time(NULL)-oldtime>10)
				{
					cls_show_msg1(g_keeptime,"line %d:�л�2G����ʧ��(%d)", __LINE__, nRet);
					return FAIL;
				 }	
			}
			break;
		case third_G:
			//�ƶ�û��3G�����ź���ͨATָ�һ������Ҫ�ж��ǵ��Ż�����ͨ
			atm_cmd.pcAtCmd="+CIMI";
			memset(sRecvStr,0,sizeof(sRecvStr));
			if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
			{ 
				 cls_show_msg1(g_keeptime,"line %d:��ȡ��Ӫ����Ϣʧ��(%d)", __LINE__, nRet);
				 return FAIL;
			}
			style=strstr(sRecvStr,"46003");
			style1=strstr(sRecvStr,"46005");
			style2=strstr(sRecvStr,"46011");
			memset(sRecvStr,0,sizeof(sRecvStr));
			if(style!=NULL||style1!=NULL||style2!=NULL)//�Ƿ�Ϊ����
			{
				atm_cmd.pcAtCmd="+qcfg=\"nwscanmode\",7";//����3G ATָ��
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
				{ 
					 cls_show_msg1(g_keeptime,"line %d:�л�3G����ʧ��(%d)", __LINE__, nRet);
					 return FAIL;
				}
			}
			else
			{
				atm_cmd.pcAtCmd="+qcfg=\"nwscanmode\",2";//��ͨ3G ATָ��
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
				{ 
					 cls_show_msg1(g_keeptime,"line %d:�л�3G����ʧ��(%d)", __LINE__, nRet);
					 return FAIL;
				}
			}
			atm_cmd.pcAtCmd="+COPS?";			
			memset(sRecvStr,0,sizeof(sRecvStr));
			while(1)
			{
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,1000,&pemStatus))!=NDK_OK)
				{ 
					cls_show_msg1(g_keeptime,"line %d:�л�3G����ʧ��(%d)", __LINE__, nRet);
					return FAIL;
				 }
				 p=strstr(sRecvStr,"\",2");//��ͨ3G��־
				 m=strstr(sRecvStr,"\",6");//��ͨ3G��־
				 q=strstr(sRecvStr,"\",100");//����3G��־
				 if(p!=NULL||q!=NULL||m!=NULL)
				{			
					cls_show_msg1(g_keeptime,"3G�������óɹ�");
					break;
				}
				if(time(NULL)-oldtime>10)
				{
					cls_show_msg1(g_keeptime,"line %d:�л�3G����ʧ��(%d)", __LINE__, nRet);
					return FAIL;
				 }	
			}
			break;						
		case fourth_G:
			atm_cmd.pcAtCmd="+qcfg=\"nwscanmode\",3";			
			if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,2000,&pemStatus))!=NDK_OK)
			{ 
				 cls_show_msg1(g_keeptime,"line %d:�л�4G����ʧ��(%d)", __LINE__, nRet);
				 return FAIL;
			}
			atm_cmd.pcAtCmd="+COPS?";				
			memset(sRecvStr,0,sizeof(sRecvStr));
			while(1)
			{
				if((nRet=NDK_WlSendATCmd(&atm_cmd,sRecvStr,100,1000,&pemStatus))!=NDK_OK)
				{ 
					cls_show_msg1(g_keeptime,"line %d:�л�4G����ʧ��(%d)", __LINE__, nRet);
					return FAIL;
				 }
				 p=strstr(sRecvStr,"\",7");
				if(p!=NULL)
				{			
					cls_show_msg1(g_keeptime,"4G�������óɹ�");
					break;
				}
				if(time(NULL)-oldtime>10)
				{
					cls_show_msg1(g_keeptime,"line %d:�л�4G����ʧ��(%d)", __LINE__, nRet);
					return FAIL;
				}	
			}
			break; 
		default:
			return FAIL;
			break;
	}
	return SUCC;
  }
#endif

static int select_rst_flag(void)
{
	/*private & local definition*/
	int nKeyin = 0, flg[] = {RESET_PPPOPEN, RESET_PPPCLOSE, RESET_TCPOPEN, RESET_TCPCLOSE, RESET_NO};

	/*process body*/
	while (1)
	{	
		nKeyin = cls_show_msg("_ѡ��λ����_\n"
							"PPP:1.��2.�ر�\n"
							"TCP:3.��4.�ر�\n");
		switch(nKeyin)
		{
		case '1':
		case '2':
		case '3':
		case '4':
			return flg[nKeyin-'1'];
			break;
		case ESC:
			cls_show_msg1(2, "�������и�λ����...");
			return RESET_NO;
			break;
		default:
			continue;
			break;
		}
	}	
}

static void wireless_dial_comm_press(int dialtype)
{
	/*private & local definition*/
	int i = 0, succ = 0, sq=0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	
	/*process body*/	
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	//����ͨѶ
	while(1)
	{
		NDK_WlModemGetSQ(&sq);
		if(cls_show_msg1(3, "��ʼ��%d�ζ�����ͨѶ(�ѳɹ�%d��),ESC�˳�...��ǰ�ź���:%d", i+1, succ, sq)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;

		if(wireless_dial_comm(&sendpacket, RESET_NO)==SUCC)
			succ++;
		else
			continue;

		if(dialtype==1)//��������ʱ��Ҫ�ȴ�
			cls_show_msg1(rand()%35+5, "ģ����Ϣ��,�����������Ϣ����ѹ��...");//ÿ�ιҶ�֮����������Ҫ�ȴ�10��,������оƬ��ѹ��
	}

	cls_show_msg_record(FILENAME,FUNCNAME,"������ͨѶѹ���������,ִ�д���Ϊ%d,�ɹ�%d��", i, succ);
	return;
}

//�����ӹ��ܲ���
static void ppp_outkeeptest(void)
{	
	/*private & local definition*/
	int timeout = SO_TIMEO, ret = 0, err = 0, i = 0;
	char buf[100] = {0}, rbuf[100] = {0};
	int slen = 0, rlen = 0;
	uint h_tcp = 0; 
	ST_PPP_CFG tmp_stPPPCfg;
	time_t diff = 0, oldtime = time(NULL);
	LinkType type = getlinktype();	
	
	/*process body*/
	for(i = 0;i<sizeof(buf);i++)
		buf[i]=rand()%255;

	memset(&tmp_stPPPCfg,0,sizeof(tmp_stPPPCfg));//20141201 3Gģ��֧�ֺ��޸�Ӧ�ò���봫����ȷ���Ŵ������������ʹ���ϴεĲ��Ŵ� 
	if(type==CDMA)
		strcpy(tmp_stPPPCfg.szDailNum, "#777");
	else
		strcpy(tmp_stPPPCfg.szDailNum, "*99***1#");
	tmp_stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
	tmp_stPPPCfg.nPPPFlag = LCP_PPP_KEEP;//������ // LCP_PPP_UNKEEPED;//������
	sprintf(tmp_stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(tmp_stPPPCfg.szApn, "\"%s\"", WLMAPN);
#else//�Ͷ˲�Ʒ��֧��LCP_PPP_KEEP,LCP_PPP_UNKEEPED���������Ӧ�ò�Ӱ��
	tmp_stPPPCfg.nPPPFlag = LCP_PPP_KEEP;//���ò��Զ�����
	sprintf(tmp_stPPPCfg.szApn, WLMAPN);
#endif
	tmp_stPPPCfg.ModemDial = NULL;
	tmp_stPPPCfg.PPPIntervalTimeOut = 29;///**<ά�ֳ����ӵ����ݰ����͵�ʱ����,<30S��������г����ӵ�ά��*/
	strcpy(tmp_stPPPCfg.nPPPHostIP, netsetting.SvrIP.Str);//��������IP���ø�������
	NDK_PppSetCfg(&tmp_stPPPCfg, sizeof(tmp_stPPPCfg));
	
	//����ǰ��
	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}

	if(TransUp(&h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	//��������
	if((slen=sock_send(h_tcp, buf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sizeof(buf));
		return;
	}
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sizeof(buf));
		return;
	}
	if(MemCmp(buf, rbuf, rlen))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
		return;	
	}
	if(TransDown(h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	
	cls_printf("��ȴ�600��");
	while(1)
	{
		if((diff=time(NULL)-oldtime)>600)//if(ReadStopwatch()>MAXTIMEOUT)
			break;
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}
	show_stopwatch(DISABLE, 0);
	
	if((ret=WaitPPPState(PPP_STATUS_DISCONNECT, &err))!=SUCC)//<30S������ʱ��С��30�벻���г�����ά����600���ppp���ӱ��ߵ�
	{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
		cls_show_msg1(10, "line %d:��ȡPHASE_NETWORKʧ��!(%d)", __LINE__, err);
		TransDown(h_tcp);
		NetDown();
		return;
	}
	if(TransUp(&h_tcp)==SUCC)//����Ӧ��ʧ��
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	TransDown(h_tcp);
	NetDown();
	cls_show_msg("���������ӵȴ�ʱ��������");
	return;
}

static void ppp_inkeeptest(void)
{	
	/*private & local definition*/
	int timeout = SO_TIMEO, ret = 0, err = 0, i = 0;
	char buf[100] = {0}, rbuf[100] = {0};
	int slen = 0, rlen = 0;
	uint h_tcp = 0; 
	ST_PPP_CFG tmp_stPPPCfg;
	time_t diff = 0, oldtime = time(NULL);
	LinkType type = getlinktype();
	
	/*process body*/
	for(i = 0;i<sizeof(buf);i++)
		buf[i]=rand()%255;

	memset(&tmp_stPPPCfg,0,sizeof(tmp_stPPPCfg));//20141201 3Gģ��֧�ֺ��޸�Ӧ�ò���봫����ȷ���Ŵ������������ʹ���ϴεĲ��Ŵ� 
	if(type==CDMA)
		strcpy(tmp_stPPPCfg.szDailNum, "#777");
	else
		strcpy(tmp_stPPPCfg.szDailNum, "*99***1#");
	memset(tmp_stPPPCfg.szDailNum,0,sizeof(tmp_stPPPCfg.szDailNum));//20141201 linwl ���������������벦�ź���ײ�����ģ������ѡ��Ĭ��ֵ
	tmp_stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
	tmp_stPPPCfg.nPPPFlag = LCP_PPP_KEEP;//������ // LCP_PPP_UNKEEPED;//������
	sprintf(tmp_stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(tmp_stPPPCfg.szApn, "\"%s\"", WLMAPN);
#else
	tmp_stPPPCfg.nPPPFlag = LCP_PPP_KEEP;//�Ͷ˲�Ʒ��֧��LCP_PPP_KEEP,LCP_PPP_UNKEEPED���������Ӧ�ò�Ӱ����������
	sprintf(tmp_stPPPCfg.szApn, WLMAPN);
#endif
	tmp_stPPPCfg.ModemDial = NULL;
	tmp_stPPPCfg.PPPIntervalTimeOut = 35;///**<ά�ֳ����ӵ����ݰ����͵�ʱ����,<30S��������г����ӵ�ά��*/
	strcpy(tmp_stPPPCfg.nPPPHostIP, netsetting.SvrIP.Str);//��������IP���ø�������
	NDK_PppSetCfg(&tmp_stPPPCfg, sizeof(tmp_stPPPCfg));
	
	//����ǰ��
	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}

	if(TransUp(&h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	//��������
	if((slen=sock_send(h_tcp, buf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sizeof(buf));
		return;
	}
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sizeof(buf));
		return;
	}
	if(MemCmp(buf, rbuf, rlen))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
		return;	
	}
	if(TransDown(h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	
	cls_printf("��ȴ�600��");
	while(1)
	{
		if((diff=time(NULL)-oldtime)>600)//if(ReadStopwatch()>MAXTIMEOUT)
			break;
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}
	show_stopwatch(DISABLE, 0);
	if((ret=WaitPPPState(PPP_STATUS_CONNECTED, &err))!=SUCC)//������ÿ35�뷢��һ��ά�ְ�,����������10���Ӻ��ܼ���ά������·
	{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
		cls_show_msg1(10, "line %d:��ȡPHASE_NETWORKʧ��!(%d)", __LINE__, err);
		TransDown(h_tcp);
		NetDown();
		return;
	}
	
	//pppδ�Ҷϣ�TransUpӦ�óɹ�
	if(TransUp(&h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	//��������
	if((slen=sock_send(h_tcp, buf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sizeof(buf));
		return;
	}
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sizeof(buf));
		return;
	}
	if(MemCmp(buf, rbuf, rlen))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
		return;	
	}
	if (TransDown(h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransDownʧ��", __LINE__);
		NetDown();
		return;	
	}
	
	NetDown();
	cls_show_msg("������ά��ʱ��������");
	return;
}


static void ppp_upkeeptest(void)
{	
	/*private & local definition*/
	int timeout = SO_TIMEO, ret = 0, err = 0, i = 0;
	char buf[100] = {0}, rbuf[100] = {0};
	int slen = 0, rlen = 0;
	uint h_tcp = 0; 
	ST_PPP_CFG tmp_stPPPCfg;
	time_t diff = 0, oldtime = time(NULL);
	LinkType type = getlinktype();
	
	/*process body*/
	for(i = 0;i<sizeof(buf);i++)
		buf[i]=rand()%255;

	memset(&tmp_stPPPCfg,0,sizeof(tmp_stPPPCfg));//20141201 3Gģ��֧�ֺ��޸�Ӧ�ò���봫����ȷ���Ŵ������������ʹ���ϴεĲ��Ŵ� 
	if(type==CDMA)
		strcpy(tmp_stPPPCfg.szDailNum, "#777");
	else
		strcpy(tmp_stPPPCfg.szDailNum, "*99***1#");
	memset(tmp_stPPPCfg.szDailNum,0,sizeof(tmp_stPPPCfg.szDailNum));//20141201 linwl ���������������벦�ź���ײ�����ģ������ѡ��Ĭ��ֵ
	tmp_stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
	tmp_stPPPCfg.nPPPFlag = LCP_PPP_UNKEEPED;//������
	sprintf(tmp_stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(tmp_stPPPCfg.szApn, "\"%s\"", WLMAPN);
#else//�Ͷ˲�Ʒ��֧��LCP_PPP_KEEP,LCP_PPP_UNKEEPED���������Ӧ�ò�Ӱ��
	tmp_stPPPCfg.nPPPFlag = LCP_PPP_UNKEEPED;//������
	sprintf(tmp_stPPPCfg.szApn, WLMAPN);
#endif
	tmp_stPPPCfg.ModemDial = NULL;
	//tmp_stPPPCfg.PPPIntervalTimeOut = 60;///**<ά�ֳ����ӵ����ݰ����͵�ʱ����,<30S��������г����ӵ�ά��*/
	
	NDK_PppSetCfg(&tmp_stPPPCfg, sizeof(tmp_stPPPCfg));
	
	//����ǰ��
	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}

	if(TransUp(&h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	//��������
	if((slen=sock_send(h_tcp, buf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sizeof(buf));
		return;
	}
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, sizeof(buf), timeout))!=sizeof(buf))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sizeof(buf));
		return;
	}
	if(MemCmp(buf, rbuf, rlen))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
		return;	
	}
	if(TransDown(h_tcp)!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}

	cls_printf("��ȴ�600��");
	while(1)
	{
		if((diff=time(NULL)-oldtime)>600)
			break;
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}
	show_stopwatch(DISABLE, 0);
	if((ret=WaitPPPState(PPP_STATUS_DISCONNECT, &err))!=SUCC)
	{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
		cls_show_msg1(10, "line %d:��ȡPHASE_NETWORKʧ��!(%d)", __LINE__, err);
		TransDown(h_tcp);
		NetDown();
		return;
	}
	if(TransUp(&h_tcp)==SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
		NetDown();
		return;
	}
	if((slen=sock_send(h_tcp, buf, sizeof(buf), timeout))<0)
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ʵ��%d)", __LINE__, slen);
		return;
	}
	TransDown(h_tcp);
	NetDown();
	cls_show_msg("�������Զ��Ҷϲ������");
	return;
}

//����ͨѶ,����flagΪ��0ʱ����ѹ������;����flagΪ0ʱ���ڻ�վ�л����쳣����
static void wireless_comm_press(int flag)
{	
	/*private & local definition*/
	int i = 0, last = -1, succ = 0, timeout = SO_TIMEO;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	int slen = 0, rlen = 0;
	uint h_tcp = 0; 
	
	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	//����ǰ��
PRE:
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		NetDown();
		cls_show_msg_record(FILENAME,FUNCNAME,"������ͨѶѹ���������,ִ�д���Ϊ%d,�ɹ�%d��", i, succ);
		return;
	}
	NDK_SysSetSuspend(0);//TCPͨѶǰ��ֹ���� 20140808
	//����ͨѶ
	while(1)
	{

		if(cls_show_msg1(3, "��ʼ��%d�γ�����ͨѶ(�ѳɹ�%d��),ESC�˳�...", i+1, succ)==ESC)
			break;		
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;

		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			if(flag)
			{
				if(IsContinuous(&i, &last))
				{
					NetDown();
					goto PRE;
				}
				else
					continue;
			}
			else
				continue;
		}
		
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η�������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
			if(flag)
			{
				if(IsContinuous(&i, &last))
				{
					NetDown();
					goto PRE;
				}
				else
					continue;
			}
			else
				continue;
		}
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν�������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			if(flag)
			{
				if(IsContinuous(&i, &last))
				{
					NetDown();
					goto PRE;
				}
				else
					continue;
			}
			else
				continue;
		}
		if(MemCmp(sendpacket.header, rbuf, rlen))
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��У��ʧ��", __LINE__, i);
			if(flag)
			{
				if(IsContinuous(&i, &last))
				{
					NetDown();
					goto PRE;
				}
				else
					continue;
			}
			else
				continue;
		}

		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransDownʧ��", __LINE__, i);
			if(flag)
			{
				if(IsContinuous(&i, &last))
				{
					NetDown();
					goto PRE;
				}
				else
					continue;
			}
			else
				continue;
		}
		succ++;
	}
	NDK_SysSetSuspend(1);//TCPͨѶǰ��ֹ���� 20140808
	NetDown();
	cls_show_msg_record(FILENAME,FUNCNAME,"������ͨѶѹ���������,ִ�д���Ϊ%d,�ɹ�%d��", i, succ);
	return;
}

static void wireless_dial_press(int dialtype)
{
	/*private & local definition*/
	int cnt = DEFAULT_CNT_VLE;
	char CntStr[8] = DEFAULT_CNT_STR;
	int i = 0, succ = 0, nRet = 0;
	uint unLen=0;
	
	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:");

	if(g_SequencePressFlag) 
	{
		cnt = GetCycleValue();  //����ѹ�����ԣ���Ҫ�ֹ�����ѭ�������Ķ�ֱ������Ĭ��ֵ
	}
	else
	{
		if((nRet = lib_kbgetinput(CntStr,0,sizeof(CntStr)-1,&unLen,INPUTDISP_OTHER,0,INPUT_CONTRL_LIMIT))==NDK_OK&&(nRet==ESC||nRet ==NDK_ERR_TIMEOUT))
			cnt = DEFAULT_CNT_VLE;
		else
			cnt = atoi(CntStr);
	}
	
	while(cnt)
	{
		if (ESC==cls_show_msg1(3, "���߰κ�ѹ����...\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", cnt, succ))
			break;
		cnt--;
		i++;

		if(NetUp()!=SUCC)//����ڲ�������ppp����,�ڲ���ͣ����ʾ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�����߰κ�ʧ��", __LINE__, i);
			continue;
		}
		if(NetDown()!=SUCC)//����ڲ�������ppp����,�ڲ���ͣ����ʾ
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�����߹Ҷ�ʧ��", __LINE__, i);
			continue;
		}
		succ++;

		if(dialtype==1)//��������ʱ��Ҫ�ȴ�
			cls_show_msg1(rand()%35+5, "ģ����Ϣ��,�����������Ϣ����ѹ��...");//ÿ�ιҶ�֮����������Ҫ�ȴ�10��,������оƬ��ѹ��
	}

	NetDown();
	cls_show_msg_record(FILENAME,FUNCNAME,"ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", i, succ);
	return;
}
static void create_WLM_reset_packet(Packet *packet, char *buf)
{
	/*private & local definition*/

	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len = WLMPACKMAXLEN;//����
	packet->lifecycle = 0;
	packet->forever = TRUE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

#if 0  //�ĳ�ʹ�õ����շ�ͨѶ�ɼ����� 20180706 linying modify
static void WLM_ability(void)
{
	/*private & local definition*/
	int i = 0, timeout = SO_TIMEO;
	uint h_tcp = 0;
	float hanguptime = 0, hangdowntime = 0, writetime = 0, readtime = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet snd_packet;
#if K21_ENABLE
	char str[32] = {0}, str1[32] = {0};
#endif

	/*process body*/
	create_WLM_ability_packet(&snd_packet, buf);
	
	while(1)
	{
		if(update_snd_packet(&snd_packet)!=SUCC)
			break;
		i++;
			
		//����NetUp����LinkUp(������NetUpȥ����LinkUp),�Բ�׼��PPP��ʱ��
		if(LinkUp()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��LinkUpʧ��", __LINE__, i);
			return;
		}
	
		lib_StartStopwatch();
		if(NetUp()!=SUCC)
		{
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��NetUpʧ��", __LINE__, i);
			return;
		}
		hanguptime += lib_ReadStopwatch();
		//���˻�ȡ�����������ʱ����
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			NetDown();
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��TransUpʧ��", __LINE__, i);
			return;
		}
		
		//�����շ���ʱ��	
		//memset(buf,0xff,sizeof(buf));
		lib_StartStopwatch();
		sock_send(h_tcp, snd_packet.header, snd_packet.len, timeout);
		writetime += lib_ReadStopwatch();
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		lib_StartStopwatch();
		sock_recv(h_tcp, rbuf, snd_packet.len, timeout);
		readtime += lib_ReadStopwatch();
		
		if (TransDown(h_tcp)!=SUCC)
		{
			NetDown();
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��TransDownʧ��", __LINE__, i);
			return;
		}
	
		lib_StartStopwatch();
		if(NetDown()!=SUCC)
		{
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��NetDownʧ��", __LINE__, i);
			return;			
		}
		hangdowntime += lib_ReadStopwatch();
		
		if(LinkDown()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��LinkDownʧ��", __LINE__, i);
			return;				
		}
	}//ѭ������20�κ���㲦�ź͹Ҷϵ�ʱ��,ȡƽ��ֵ
#if !K21_ENABLE
	cls_show_msg_record(FILENAME, FUNCNAME, "PPP����ʱ��:%fs PPP�Ҷ�ʱ��:%fs", hanguptime/PACKETLIFE, hangdowntime/PACKETLIFE);
	cls_show_msg_record(FILENAME, FUNCNAME, "����/����%dB���ݺ�ʱ:%fs/%fs", snd_packet.len, writetime/PACKETLIFE, readtime/PACKETLIFE);
#else
	memset(str,0,sizeof(str));
	memset(str1,0,sizeof(str1));
	ftos(hanguptime/PACKETLIFE,str);
	ftos(hangdowntime/PACKETLIFE,str1);
	cls_show_msg_record(FILENAME, FUNCNAME, "PPP����ʱ��:%ss PPP�Ҷ�ʱ��:%ss", str, str1);
	memset(str,0,sizeof(str));
	memset(str1,0,sizeof(str1));
	ftos(writetime/PACKETLIFE,str);
	ftos(readtime/PACKETLIFE,str1);
	cls_show_msg_record(FILENAME, FUNCNAME, "����/����%dB���ݺ�ʱ:%ss/%ss", snd_packet.len, str, str1);
#endif
	return;
}
#endif

//����������հ�
static void create_WLM_ability_packet(Packet *packet, char *buf)
{
	/*private & local definition*/

	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
#if K21_ENABLE
    packet->len = packet->orig_len= 1024;//�Ͷ�����ģ��ÿ�ζ��Ƿֳ�1K��1K���ݽ��з��ͣ�������Ҳ��1K��1K��
#else
	packet->len = packet->orig_len= 1024*4;
#endif
	packet->lifecycle =PACKETLIFE;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

//���������
static int  get_wlm_random_data(Packet *packet)
{
    /*private & local definition*/
	int j = 0;
	char *ptr = packet->header;
	/*process body*/
	if(packet->IsDataRnd)
	{
		for (j=0; j<packet->len; j++)
			*ptr++ = rand()%256;
	}
	return SUCC;
}
static void WLM_hang_up_down_ability(void)
{
	/*private & local definition*/
	int i = 0;
	uint h_tcp = 0;
	float hanguptime = 0, hangdowntime = 0;
	char buf[PACKMAXLEN] = {0};
	Packet snd_packet;
#if K21_ENABLE
	char str[32] = {0}, str1[32] = {0};
#endif

	/*process body*/
	create_WLM_ability_packet(&snd_packet, buf);
	
	while(1)
	{
		if(update_snd_packet(&snd_packet)!=SUCC)
			break;
		i++;
			
		//����NetUp����LinkUp(������NetUpȥ����LinkUp),�Բ�׼��PPP��ʱ��
		if(LinkUp()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��LinkUpʧ��", __LINE__, i);
			return;
		}
	
		lib_StartStopwatch();
		if(NetUp()!=SUCC)
		{
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��NetUpʧ��", __LINE__, i);
			return;
		}
		hanguptime += lib_ReadStopwatch();
		//���˻�ȡ�����������ʱ����
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			NetDown();
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��TransUpʧ��", __LINE__, i);
			return;
		}
		
		if (TransDown(h_tcp)!=SUCC)
		{
			NetDown();
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��TransDownʧ��", __LINE__, i);
			return;
		}
	
		lib_StartStopwatch();
		if(NetDown()!=SUCC)
		{
			LinkDown();
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��NetDownʧ��", __LINE__, i);
			return;			
		}
		hangdowntime += lib_ReadStopwatch();
		
		if(LinkDown()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��%d��LinkDownʧ��", __LINE__, i);
			return;				
		}
	}//ѭ������30�κ���㲦�ź͹Ҷϵ�ʱ��,ȡƽ��ֵ
#if !K21_ENABLE
	cls_show_msg_record(FILENAME, FUNCNAME, "PPP����ʱ��:%fs PPP�Ҷ�ʱ��:%fs", hanguptime/PACKETLIFE, hangdowntime/PACKETLIFE);
#else
	memset(str,0,sizeof(str));
	memset(str1,0,sizeof(str1));
	ftos(hanguptime/PACKETLIFE,str);
	ftos(hangdowntime/PACKETLIFE,str1);
	cls_show_msg_record(FILENAME, FUNCNAME, "PPP����ʱ��:%ss PPP�Ҷ�ʱ��:%ss", str, str1);
#endif
	return;
}

//����������ܲ���20180705 liny added
void WLM_recv_only_ability(uint h_tcp)
{
	/*private & local definition*/
	int i = 0, timeout = SO_TIMEO,  slen = 0, rlen = 0;
	float recvtime = 0,size=0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
#if K21_ENABLE
	char str[32] = {0};
#endif
	/*process body*/
	create_WLM_ability_packet(&sendpacket, buf);
	update_snd_packet(&sendpacket);
	sendpacket.IsDataRnd = FALSE;//�������ѳ�ʼ����,͵͵�ı�Ϊ�����
	sendpacket.lifecycle = PACKETLIFE;
	if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__,slen, sendpacket.len);
		NetDown();
		LinkDown();
		return;
	}
	cls_printf("���ߵ���������ܲ�����...");
    	while(1)
	{
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		lib_StartStopwatch();
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__,i, rlen, sendpacket.len);
			NetDown();
			LinkDown();
			return;
		}
		recvtime += lib_ReadStopwatch();
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��У��ʧ��", __LINE__,i);
			NetDown();
			LinkDown();
			return;
		}	
    }
	size=sendpacket.len/1024;
#if !K21_ENABLE
	cls_show_msg_record(FILENAME, FUNCNAME, "�����������:%fKB/S",size*PACKETLIFE/recvtime);
#else
	memset(str,0,sizeof(str));
	ftos(size*PACKETLIFE/recvtime,str);
	cls_show_msg_record(FILENAME, FUNCNAME, "�����������:%sKB/S",str);
#endif
    return;
}

//���������ܲ���20180705 liny added
void WLM_send_only_ability(uint h_tcp)
{
	/*private & local definition*/
	int i = 0, timeout = SO_TIMEO, slen = 0;
	float sendtime = 0,size=0;
	char buf[PACKMAXLEN] = {0};
	Packet sendpacket;
#if K21_ENABLE
	char str[32] = {0};
#endif
	/*process body*/
	create_WLM_ability_packet(&sendpacket, buf);
	cls_printf("���ߵ��������ܲ�����...");
	while (i<PACKETLIFE)
	{
		i++;
		get_wlm_random_data(&sendpacket);
        lib_StartStopwatch();
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i,slen, sendpacket.len);
		    	NetDown();
			LinkDown();
			return;
		}
		sendtime += lib_ReadStopwatch();
		NDK_SysDelay(20);//����̨һЩ����ʱ��2s(�Ͷ˷�����Ҫ2s��ʱ)
    }
	size=sendpacket.len/1024;
#if !K21_ENABLE
	cls_show_msg_record(FILENAME, FUNCNAME, "����������:%fKB/S",size*PACKETLIFE/sendtime);
#else
	memset(str,0,sizeof(str));
	ftos(size*PACKETLIFE/sendtime,str);
	cls_show_msg_record(FILENAME, FUNCNAME, "����������:%sKB/S",str);
#endif
    return;
}

//�Ż����ܲ��ԣ�ȡ����ͨѶ��������20180705 liny added
void WLM_ability(void)
{
	/*private & local definition*/
	int nKey = '0';
	void (*pf[2])(uint) = {WLM_recv_only_ability, WLM_send_only_ability};
	uint h_tcp = -1;

	cls_show_msg("ע��:���Ե����շ�����ʱ,��Ҫ�����ɵ����������̨,���������");
	
	nKey = cls_show_msg("1.���ź͹Ҷ�ʱ��\n"
						"2.�����������\n"
						 "3.����������\n");
	switch(nKey)
	{
		case '1':
			WLM_hang_up_down_ability();
			break;
		case '2':
		case '3':
			if(LinkUp()!=SUCC)
		        {
			        cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:LinkUpʧ��", __LINE__);
			        return;
		        }
		        if(NetUp()!=SUCC)
		        {
			        LinkDown();
			        cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:NetUpʧ��", __LINE__);
			        return;
		        }
		        if(TransUp(&h_tcp)!=SUCC)
		        {
			        NetDown();
			        LinkDown();
			        cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:TransUpʧ��", __LINE__);
			        return;
		        }
			pf[nKey-'2'](h_tcp);
			if (TransDown(h_tcp)!=SUCC)
		        {
			        NetDown();
			        LinkDown();
			        cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:TransDownʧ��", __LINE__);
			        return;
		        }
		        if(NetDown()!=SUCC)
		        {
			        LinkDown();
			        cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:NetDownʧ��", __LINE__);
			        return;			
		        }		
		        if(LinkDown()!=SUCC)
		        {
			        cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:LinkDownʧ��", __LINE__);
			        return;				
		        }
			break;
		case ESC:
			return;
			break;
		default:
			break;		
			
		}
}

static void wireless_press(void)
{
	/*private & local definition*/
	int nKeyin = '0', dialtype = 0;
	
	/*process body*/
	if(cls_show_msg1(5, "ѡ�񲦺�ѹ������:\n[ȷ��]��ͨ��\n[����]��ǿ��")==ENTER)
		dialtype = 1;//��������
	else
		dialtype = 0;//�쳣����

	while(1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			if( ++nKeyin == '4' ) //��Ӧ����Ĳ˵���+1���˵����Ӵ˴�ҲҪ��Ӧ������
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"����ѹ�����Խ���");
				return ;
			}
			if(cls_show_msg1(3, "������������ѹ������,ESC�˳�")==ESC)
				return;
		}
		else
		{
			nKeyin = cls_show_msg("1.����ѹ��\n2.������ѹ��\n3.������ѹ��");//���������Ƕ�PPP���Ե�
		}
		switch(nKeyin)
		{
		case '1':
			wireless_dial_press(dialtype);
			break;
		case '2':
			wireless_comm_press(1);
			break;
		case '3':
			wireless_dial_comm_press(dialtype);
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

static void wireless_reset(void)
{
	/*private & local definition*/
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	int rst_flag = RESET_NO;
	
	/*process body*/
	if((rst_flag=select_rst_flag())==RESET_NO)
		return;
	
	create_WLM_reset_packet(&sendpacket, buf);
	update_snd_packet(&sendpacket);

	wireless_dial_comm(&sendpacket, rst_flag);
	cls_show_msg("���߸�λ����ʧ��");//nener reach
	return;
}

static void wireless_PPP2AT(void)
{
	/*private & local definition*/
	int sq = 0, mux_support = 0;
	LinkType type = getlinktype();
	
	/*process body*/
	//������ý�Ϊ����,���ݷ��ڴ˴�
	mux_support = (cls_show_msg("����ģ��֧�ֶ�·����?\n[ȷ��]֧��[����]��֧��")==ENTER)?TRUE:FALSE;
	
	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}
	cls_printf("�Ѵ�����̬ͨ,�����л���AT����̬��,�����ĵȴ�...");

	if((mux_support!=TRUE||NDK_WlModemGetSQ(&sq)!=NDK_OK||!(((type==GPRS||type==CDMA)&&sq<=SQ_2G_MAX&&sq>=SQ_2G_MIN)||(type==TD&&sq<=SQ_3G_MAX&&sq>=SQ_3G_MIN)))
	&&(mux_support!=FALSE||NDK_WlModemGetSQ(&sq)==NDK_OK))//�ź�ֵ��ȡ�����ֵ���������= 20171013 modify by  linying
	{
		cls_show_msg("line %d:��·���ò���ʧ��,�ź�ֵΪ%d", __LINE__,sq);
		NetDown();
		return;
	}

	if(NetDown()!=SUCC)
	{
		cls_show_msg("line %d:NetDownʧ��", __LINE__);
		return;
	}

	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}
	if(NetDown()!=SUCC)
	{
		cls_show_msg("line %d:NetDownʧ��", __LINE__);
		return;
	}
	
	cls_show_msg("PPP->AT���Գɹ�");
	return;
}

//20140613 zhengdc �޸�����ǳ���ߡ������ߣ��Զ����ߺͺ������� ����������������pppopen������֤
static void wireless_sleep(void)
{
	/*private & local definition*/
	int i = 2, nRet = -1, timeout = SO_TIMEO, errcode = 0, count = 0;
	int sleepflag = 0;
	uint h_tcp = 0; 
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	int slen = 0, rlen = 0;
	EM_PPP_STATUS pppstatus;
	
	/*process body*/
#if CPU5810X_ENABLE
	//��������:5810ƽ̨�����ص���ʱ����ʹ�����Զ�����
	uint a = 0;
	NDK_SysStopWatch(&a);
#endif

	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

#if !K21_ENABLE
	while(1)
	{
		sleepflag=cls_show_msg("��ѡ����������\n1.������\n2.ǳ����(��ʱ���ڻ���)\n3.ǳ����90��������");//ע��ѡ�������ģʽҪ��������һ��
		
		if((sleepflag=='1')||(sleepflag=='2')||(sleepflag=='3'))
			break;		
	}
#else
	while(1)
	{
		sleepflag=cls_show_msg("��ѡ����������\n2.ǳ����\n3.ǳ����90��������");//ע��ѡ�������ģʽҪ��������һ��
		
		if((sleepflag=='2')||(sleepflag=='3'))
			break;		
	}
#endif
	
	while(i)
	{
		if(NetUp()!=SUCC)
		{
			cls_show_msg("line %d:NetUpʧ��", __LINE__);
			return;
		}
		if(i==2)
		{
			cls_show_msg("������ѽ���!�������������,һ��ʱ����밴������.");
#if K21_ENABLE//Ϊ����Ͷ�ƽ̨��Ʒ�����߶�ʱ�ػ���Ĭ�����ã��������߲��ػ�
			NDK_SysSetShutdownDelay(0);
			sleep(1);
#endif
#if ANDIROD_ENABLE
			if((nRet=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
			if((nRet=NDK_SysGoSuspend()) != NDK_OK)
#endif
			{
				cls_show_msg("line %d:����ʧ��(%d)", __LINE__, nRet);
				NetDown();
				return;
			}
		}
		else
			cls_show_msg("������ѽ���!�ȴ�pos�Զ��������ߣ����ߺ��밴������");
        if(cls_show_msg("���߻��Ѻ��Ƿ����̳��ָý��棬�ǰ�[Enter],��[����]") != ENTER)//�����˻����ӳ٣��ײ㷢��ATָ���ӳ����������󣬵��½���ˢ���ӳ٣����������жϣ�20170621 linying added
		{
			cls_show_msg("line %d:���߻��Ѻ󣬽���ˢ��ʧ��", __LINE__);
			NetDown();
			return;
		}
		sleep(1);//������ͻ��Ѻ���Ҫһ��ʱ������ģ��ſ��Է�����ȷ��״̬��Ӧ��Ҳ��һ�β���ʱ��
		if((nRet=NDK_PppCheck(&pppstatus, &errcode))!=NDK_OK)
		{
			cls_show_msg("line %d:��ȡPPP״̬ʧ��(%d)", __LINE__, nRet);
			NetDown();
			return;
		}
		
		if(sleepflag=='1')//������ppp�ᱻ�Ͽ�����Ҫ�������ӣ�ǳ����ppp������ ����ֱ�ӽ���tcp
		{
			if(pppstatus!=PPP_STATUS_DISCONNECT)//�ж�״̬�Ƿ�Ϊδ����״̬PPP_STATUS_DISCONNECT
			{
				cls_show_msg("line %d:ppp״̬����ȷ,����ʧ��(%d)", __LINE__,pppstatus);
				NetDown();
				return;
			}
			cls_printf("���˳�����!���½���PPP������...");
			//�����߻��Ѻ�,Ԥ������ģ�龭���µ����ϵ�Ĺ���,��PPPӦ�ѶϿ�
			set_linkstatus_down();
			set_netstatus_down();
			//���½���PPP����/�Ͽ�Ӧ����
			if(NetUp()!=SUCC)
			{
				cls_show_msg("line %d:NetUpʧ��", __LINE__);
				return;
			}
		}
		else if(sleepflag=='2')//ǳ����ģʽPPP״̬Ӧ����Ȼ��������PPP_STATUS_CONNECTED״̬
		{
			if(pppstatus!=PPP_STATUS_CONNECTED)
			{
				cls_show_msg("line %d:ppp״̬����ȷ,����ʧ��(%d)", __LINE__,pppstatus);
				NetDown();
				return;
			}
		}
		else if(sleepflag=='3')//��������90�������ϣ�״̬���ܶϵ�Ҳ����δ�ϵ���
		{
			if((pppstatus!=PPP_STATUS_CONNECTED)&&(pppstatus!=PPP_STATUS_DISCONNECT)&&(pppstatus!=PPP_STATUS_DISCONNECTING))
			{
				cls_show_msg("line %d:ppp״̬����ȷ,����ʧ��(%d)", __LINE__,pppstatus);
				NetDown();
				return;
			}
			if(pppstatus==PPP_STATUS_DISCONNECTING)//����ײ����ڹҶϣ��������ȡ״ֱ̬����ȡ���Ѿ��Ҷϵ�״̬
			{
				count=0;
				while(1)
				{
					sleep(1);
					if((nRet=NDK_PppCheck(&pppstatus, &errcode))!=NDK_OK)
					{
						cls_show_msg("line %d:��ȡPPP״̬ʧ��(%d)", __LINE__, nRet);
						NetDown();
						return;
					}
					if(pppstatus==PPP_STATUS_DISCONNECT)
						break;	
					count++;
					if(count>=30)//30���ȡ״̬ʱ��
					{
						cls_show_msg("line %d:��ʱ��δ��ȡ���Ҷ�״̬(%d)", __LINE__, count);
						NetDown();
						return;
					}
				}
			}
			if(pppstatus==PPP_STATUS_DISCONNECT)//PPP����Ͽ� ����Ҫ����
			{
				//�����߻��Ѻ�,Ԥ������ģ�龭���µ����ϵ�Ĺ���,��PPPӦ�ѶϿ�
				set_linkstatus_down();
				set_netstatus_down();
				//���½���PPP����/�Ͽ�Ӧ����
				if(NetUp()!=SUCC)
				{
					cls_show_msg("line %d:NetUpʧ��", __LINE__);
					return;
				}
			}
			else//δ�Ͽ�����������ֱ������TCP
				cls_show_msg("pppstatus=%d", pppstatus);
		}
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg("line %d:TransUp(%d)", __LINE__, h_tcp);
			NetDown();
			return;
		}
		
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg("line %d:���ݷ���ʧ�ܣ�Ԥ��%d,ʵ��%d", __LINE__, sendpacket.len,slen);
			NetDown();
			return;
		}
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg("line %d:���ݽ���ʧ�ܣ�Ԥ��%d,ʵ��%d", __LINE__, sendpacket.len,rlen);
			NetDown();
			return;
		}
		if(MemCmp(sendpacket.header, rbuf, rlen))
		{
			TransDown(h_tcp);
			cls_show_msg("line %d:����ʧ��", __LINE__);
			NetDown();
			return;
		}

		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			NetDown();
			return;
		}
		if(NetDown()!=SUCC)
		{
			cls_show_msg("line %d:NetDownʧ��", __LINE__);
			return;
		}
#if !K21_ENABLE
		i--;
#else
		i=i-2;//�Ͷ˲�Ʒ�����꺯������ֱ���˳�,��֧���Զ����߹���
#endif
	}
	cls_show_msg("�������߲��Գɹ�");
	return;
}

#if K21_ENABLE
int set_ppp_autoconnect(void)
{	
	int ret = -1;
	ST_PPP_CFG stPPPCfg;
	
	if(cls_show_msg("�Ƿ�����ppp�Զ�����,��[Enter],��[����]")==ENTER)
	{
		stPPPCfg.nPPPFlag = LCP_PPP_AUTOCONNECT ; //�Զ�����
		sprintf(stPPPCfg.szApn, WLMAPN);
		if((ret=NDK_PppSetCfg(&stPPPCfg, 0))!=NDK_OK)
		{
			cls_show_msg1(3,"line %d:PPP�Զ���������ʧ��",__LINE__);
			return FAIL;
		}
	}
	else
	{
		stPPPCfg.nPPPFlag = LCP_PPP_UNAUTOCONNECT; //���Զ�����
		sprintf(stPPPCfg.szApn, WLMAPN);
		if((ret=NDK_PppSetCfg(&stPPPCfg, 0))!=NDK_OK)
		{
			cls_show_msg1(3,"line %d:PPP���Զ���������ʧ��",__LINE__);
			return FAIL;
		}
	}

	return SUCC;
}
#endif

static void wireless_abnormity(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while (1)
	{
		keyin = cls_show_msg("1.��λ����\n"
			              	"2.PPP->AT\n"
							"3.���߲���\n"
							"4.�����쳣\n"
							"5.��վ�л�����");
		switch (keyin)
		{
		case '1':
			wireless_reset();
			break;
		case '2':
			wireless_PPP2AT();//Ŀǰֻ��BGS2�ܹ�֧�ֶ�·����
			break;			
		case '3':
			wireless_sleep();
			break;
		case '4':
			tcp_abnormal();
			cls_show_msg("������δ������̨�ķ�������ַ,Ӧ������ͨ!");
			break;
		case '5':
#if K21_ENABLE
			if(set_ppp_autoconnect()!=SUCC)
				return;
#endif
			wireless_comm_press(0);
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

static void keeplinetest(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while (1)
	{
		keyin = cls_show_msg("1.ά�ֳ�����\n"
			              		"2.��ά�ֳ�����\n"
							"3.������\n"
							);
		switch (keyin)
		{
		case '1':
			ppp_inkeeptest();
			break;
		case '2':
			ppp_outkeeptest();
			break;			
		case '3':
			ppp_upkeeptest();
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

void systest4(void)
{
	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	
#if CPU5810X_ENABLE
	int flag_4g=0,ret=-1;
	Network_t type = fourth_G;
	char *p=NULL;
	char szOutbuf[1024] = {0};
	ST_ATCMD_PACK STatcmdpack = {0};
	EM_WLM_STATUS emStatus = 0;
	//�ж��Ƿ���EC20ģ��
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CGMR;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((ret = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:��ȡģ��汾ʧ��%d)", __LINE__, ret);
		RETURN;
	}
	if((p=strstr(szOutbuf,"EC20"))!=NULL)
	    	flag_4g=1;
	else
		flag_4g=0;
#endif

	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
#if CPU5810X_ENABLE
		if(flag_4g==1)
		{
			NDK_NetAddRouterTable(COMM_TYPE_PPP,"0.0.0.0");//��ֹ֮ǰ������Ա����wifi���������ߣ�û���л�·�ɵ�������ģʽ�л�ʧ�� 20180808 liny added
			type=select_nettype();
			if(conf_conn_setseg(type)!=SUCC)
			{
			    cls_show_msg1(2, "����ģʽ�л�ʧ��!");
			    return;
			}
		}
#endif		
		if(conf_conn_WLM(TRUE)!=SUCC)
			return;
		g_CycleTime = 100;

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��ѹ������
		wireless_press();

		//ִ�����ܲ���
		//wireless_ability();//�ĳɵ����շ����ܺ���Ҫ������̨,�޷��Զ���ȡ����ֵ
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	else
	{
	    while (1)
	    {	
		    keyin = cls_show_msg("1.����ѹ��\n"
							"2.��������\n"
							"3.�쳣 0.����\n"
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
							"4.����������"
#endif							
							);
		    switch(keyin)
		    {
		    case '1':
			    wireless_press();
			    break;
		    case '2':
			    WLM_ability();
			    break;
		    case '3':
			    wireless_abnormity();
			    break;	
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
		    case '4':
			    keeplinetest();
			    break;
#endif
		    case '0':
#if CPU5810X_ENABLE
			    if(flag_4g==1)
			    {
				    NDK_NetAddRouterTable(COMM_TYPE_PPP,"0.0.0.0");//��ֹ֮ǰ������Ա����wifi���������ߣ�û���л�·�ɵ�������ģʽ�л�ʧ�� 20180808 liny added
			        type=select_nettype();
			        if(conf_conn_setseg(type)!=SUCC)
			        {
			           cls_show_msg1(2, "����ģʽ�л�ʧ��!");
				       break;
			        }
		        }
#endif
			    switch(conf_conn_WLM(TRUE))
			    {
			    case SUCC:
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
		    case ESC:
			    return;
			    break;
		    default:
			    continue;
			    break;
		    }
	    }
    }
	
	return;
}

