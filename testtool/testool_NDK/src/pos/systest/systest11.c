/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: wlan�ۺϲ���
* file name			: 11.c
* Author 			: chensj
* version			: 
* DATE				: 20130521
* directory 		: 
* description		: wlan�ۺϲ���
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"
#if defined SP930P||defined SP930PG||defined SP630PG||defined SP830PG||defined ME31SMDM||defined SP930MDM
#include "iconvplus.h" //phoenix+ƽ̨���õĿ��ļ�Ϊglibc iconv,��������Ʒ��ͬ��Ϊ����������Ʒ������ͷ�ļ�
#else
#include "iconv.h"
#endif

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"WLAN����,ѹ��"
#define	DEFAULT_CNT_STR	"100"	//"200"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
#define	PACKETLIFE			(30)	//20  1000
#define	SCAN_PRESS		2000
#define	MAXWAITTIME 	60	
#define    DATASUM    1024*2400//2M�����ݣ���λbyte

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

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
*			  		      chensj	        20130521	 	created
*****************************************************************/
//����ж�WiFi����utf-8תGBK��ʽ,ʹ�ô������ļ���ȡ��utf-8��ʽת�����ܹ���ɨ�������gbk��ʽ�ȶ���ȷsull add 20171228
#if !K21_ENABLE
//����ת��:��һ�ֱ���תΪ��һ�ֱ���
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;
	
	cd = iconv_open(to_charset,from_charset);
	if (cd==0) 
		return FAIL;
	memset(outbuf,0,outlen);
	//fprintf(stderr,"%s,%d\n",__FUNCTION__,__LINE__);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) 
		return FAIL;
	iconv_close(cd);
	return SUCC;
}
//UTF-8תΪGBK��
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
	return code_convert("UTF-8","GBK",inbuf,inlen,outbuf,outlen);
}
#endif

static void create_wlan_ability_packet(Packet *packet, char *buf)
{
	/*private & local definition*/
	
	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	 packet->len = packet->orig_len= MIN(PACKMAXLEN,4096);//1024;//����1.6�淶�����޸� 20131103 linwl
	packet->lifecycle = PACKETLIFE;//1000;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

#if 0  //�ĳɵ�����պͷ�������ֵ  20180706 linying modifiy
//���ܲ���:ͨѶ����1000��,�������,����1024
static void wlan_ability1(void)
{
	/*private & local definition*/
	int  i = 0, timeout = SO_TIMEO, tmpflag = 0;
	int slen = 0, rlen = 0, commsucc = 0, linksucc = 0, freesucc = 0, succ = 0;
	float commtimes = 0, preconnectTime = 0, freetimes=0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet snd_packet;
	uint h_tcp = -1;
#if K21_ENABLE
	char str[32] = {0};
#endif

	/*process body*/
	create_wlan_ability_packet(&snd_packet, buf);

	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=1)
		SetWifiStopflag(1);//ȷ�������ܲ���ʱwifiÿ�ζ�����stopȥ�Ҷ�
	
	while(1)
	{
		if(update_snd_packet(&snd_packet)!=SUCC)
			break;
		i++;

#if NEW_WIFI_ENABLE
		if(i==1)
			NDK_WifiStop();//��wifi�ڵ�һ�β���ǰҪ��һ�ιҶϲ���,���⿪���Զ�����wifi���������ֵ���
#endif
		lib_StartStopwatch();
		if(LinkUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��LinkUpʧ��", __LINE__, i);
			LinkDown();
			continue;//goto ERR;//�������,���ͳ�Ƴɹ���20171208 
		}
		linksucc++;
		preconnectTime+=lib_ReadStopwatch();
	
		if(NetUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, i);
			LinkDown();
			continue;//goto ERR;
		}
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			NetDown();
			LinkDown();
			continue;//goto ERR;
		}
		
		//�����շ���ʱ��	
		//memset(buf,0xff,sizeof(buf));
		lib_StartStopwatch();
		if((slen=sock_send(h_tcp, snd_packet.header, snd_packet.len, timeout))!=snd_packet.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, slen);
			NetDown();
			LinkDown();
			continue;//goto ERR;
		}	
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, snd_packet.len, timeout))!=snd_packet.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν���ʧ��(%d)", __LINE__, i, rlen);
			NetDown();
			LinkDown();
			continue;//goto ERR;
		}	
		commsucc++;
		commtimes+=lib_ReadStopwatch();
		
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransDownʧ��", __LINE__, i);
			NetDown();
			LinkDown();
			continue;//goto ERR;
		}

		lib_StartStopwatch();
		if(NetDown()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, i);
			LinkDown();
			continue;//goto ERR;
		}
		freesucc++;
		freetimes+=lib_ReadStopwatch();
		
		if(LinkDown()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��LinkDownʧ��", __LINE__, i);
			continue;//goto ERR;
		}
		succ++;
	}//ѭ������20�κ���㲦�ź͹Ҷϵ�ʱ��,ȡƽ��ֵ
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d:�ɹ�����%d", PACKETLIFE, succ);
	cls_show_msg_record(FILENAME,FUNCNAME, "Ԥ����%s��ʱ%fs\n", WlanEssid, preconnectTime/linksucc);
	cls_show_msg_record(FILENAME,FUNCNAME, "ͨѶ����%fKB/s\n", (snd_packet.len*2/1024)*commsucc/commtimes);
	cls_show_msg_record(FILENAME,FUNCNAME, "free��ʱ%fs\n", freetimes/freesucc);
#else
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d:�ɹ�����%d", PACKETLIFE, succ);
	memset(str,0,sizeof(str));
	ftos(preconnectTime/linksucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "Ԥ����%s��ʱ%ss\n", WlanEssid, str);
	
	memset(str,0,sizeof(str));
	ftos((snd_packet.len*2/1024)*commsucc/commtimes,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "ͨѶ����%sKB/s\n", str);
	
	memset(str,0,sizeof(str));
	ftos(freetimes/freesucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "free��ʱ%ss\n", str);
#endif

ERR:
	SetWifiStopflag(tmpflag);
	return;
}
#endif

//���ܲ���:ȡԤ����ʱ���freeʱ��
static void wlan_prelink_free_ability(void)
{
	/*private & local definition*/
	int  i = 0, tmpflag = 0;
	int linksucc = 0, freesucc = 0, succ = 0;
	float preconnectTime = 0, freetimes=0;
	uint h_tcp = -1;
#if K21_ENABLE
	char str[32] = {0};
#endif

	/*process body*/
	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=1)
		SetWifiStopflag(1);//ȷ�������ܲ���ʱwifiÿ�ζ�����stopȥ�Ҷ�
	
	while(i<PACKETLIFE)
	{
		i++;
#if NEW_WIFI_ENABLE
		if(i==1)
			NDK_WifiStop();//��wifi�ڵ�һ�β���ǰҪ��һ�ιҶϲ���,���⿪���Զ�����wifi���������ֵ���
#endif
		lib_StartStopwatch();
		if(LinkUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��LinkUpʧ��", __LINE__, i);
			LinkDown();
			continue;//goto ERR;//�������,���ͳ�Ƴɹ���20171208 
		}
		linksucc++;
		preconnectTime+=lib_ReadStopwatch();
		if(NetUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, i);
			LinkDown();
			continue;//goto ERR;
		}
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			NetDown();
			LinkDown();
			continue;//goto ERR;
		}
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransDownʧ��", __LINE__, i);
			NetDown();
			LinkDown();
			continue;//goto ERR;
		}

		lib_StartStopwatch();
		if(NetDown()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, i);
			LinkDown();
			continue;//goto ERR;
		}
		freesucc++;
		freetimes+=lib_ReadStopwatch();
		if(LinkDown()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��LinkDownʧ��", __LINE__, i);
			continue;//goto ERR;
		}
		succ++;
	}//ѭ������PACKETLIFE�κ���㲦�ź͹Ҷϵ�ʱ��,ȡƽ��ֵ
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"�����ܴ���%d:�ɹ�����%d", PACKETLIFE, succ);
	cls_show_msg_record(FILENAME,FUNCNAME, "Ԥ����%s��ʱ%fs\n", WlanEssid, preconnectTime/linksucc);
	cls_show_msg_record(FILENAME,FUNCNAME, "free��ʱ%fs\n", freetimes/freesucc);
#else
	cls_show_msg_record(FILENAME,FUNCNAME,"�����ܴ���%d:�ɹ�����%d", PACKETLIFE, succ);
	memset(str,0,sizeof(str));
	ftos(preconnectTime/linksucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "Ԥ����%s��ʱ%ss\n", WlanEssid, str);
	
	memset(str,0,sizeof(str));
	ftos(freetimes/freesucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "free��ʱ%ss\n", str);
#endif

//ERR:
	SetWifiStopflag(tmpflag);
	return;
}

//���������
static int  get_wlan_random_data(Packet *packet)
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
	return  SUCC;
}

//����������հ�
static void create_ability_packet(Packet *packet, char *buf)
{
	/*private & local definition*/

	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	//�еͶ�ͳһʹ��12k���ݲ���
	packet->len = packet->orig_len= PACKMAXLEN;
	packet->lifecycle =0;//PACKETLIFE;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

//����������ܲ���20180705 liny added
void WLAN_recv_only_ability(uint h_tcp)
{
	/*private & local definition*/
	int i = 0, timeout = SO_TIMEO,  slen = 0, rlen = 0,k=0;
	float recvtime = 0,size=0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
#if K21_ENABLE
	char str[32] = {0};
#endif
	/*process body*/
	create_ability_packet(&sendpacket, buf);
	update_snd_packet(&sendpacket);
	k=sendpacket.lifecycle =DATASUM/sendpacket.len;
	sendpacket.IsDataRnd = FALSE;//�������ѳ�ʼ����,͵͵�ı�Ϊ�����
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
	cls_show_msg_record(FILENAME, FUNCNAME, "�����������:%fKB/S",size*k/recvtime);
#else
	memset(str,0,sizeof(str));
	ftos(size*k/recvtime,str);
	cls_show_msg_record(FILENAME, FUNCNAME, "�����������:%sKB/S",str);
#endif
    return;
}

//���������ܲ���20180705 liny added
void WLAN_send_only_ability(uint h_tcp)
{
	/*private & local definition*/
	int i = 0, timeout = SO_TIMEO, slen = 0, k=0;
	float sendtime = 0,size=0;
	char buf[PACKMAXLEN] = {0};
	Packet sendpacket;
#if K21_ENABLE
	char str[32] = {0};
#endif
	/*process body*/
	create_ability_packet(&sendpacket, buf);
	k=DATASUM/sendpacket.len;
	get_wlan_random_data(&sendpacket);
	cls_printf("���ߵ��������ܲ�����...");
	while (i<k)
	{
		i++;
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
		NDK_SysMsDelay(10);//����̨һЩ����ʱ�� 10ms
    }
	size=sendpacket.len/1024;
	#if !K21_ENABLE
	cls_show_msg_record(FILENAME, FUNCNAME, "����������:%fKB/S",size*k/sendtime);
#else
	memset(str,0,sizeof(str));
	ftos(size*k/sendtime,str);
	cls_show_msg_record(FILENAME, FUNCNAME, "����������:%sKB/S",str);
#endif
    return;
}

//WiFi���ܲ���,����ͨѶȡ����ͨѶ��������20180705 liny added
void wlan_ability1(void)
{
	/*private & local definition*/
	int nKey = '0';
	void (*pf[2])(uint) = {WLAN_recv_only_ability, WLAN_send_only_ability};
	uint h_tcp = -1;
	
	cls_show_msg("ע��:���Ե����շ�����ʱ,��Ҫ�����ɵ����������̨,���������");
		
	nKey = cls_show_msg("1.Ԥ���Ӻ�freeʱ��\n"
		                  	"2.�����������\n"
					"3.����������\n");
	switch(nKey)
	{
		case '1':
			wlan_prelink_free_ability();
			break;
		case'2':
		case'3':
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

#if !NEW_WIFI_ENABLE
//���ܲ���:wlan �������ܲ��ԣ���Ҫ�ǿ��ǵ�����ʱposû�м��أ�Ӧ�ó����һ��Ԥ����ʱ�������ģ��
//ע��:free��������뿪���״����ӵ�����������/proc���Ƿ��������һ��AP����Ϣ
static void wlan_ability2(void)
{
	/*private & local definition*/
	ST_WIFI_PARAM init={0};
	int ret = 0;
	EM_WPA_CONSTATE state = 0;
	time_t diff = 0, oldtime = 0;
	float inittimes = 0.0;
#if K21_ENABLE
	char str[32] = {0};
	char str1[32] = {0};
#endif	

	/*process body*/
	//����ǰ��
	if(ENTER!=cls_show_msg("�Ƿ�Ϊ�����ϵ���״����б�����?\n[ȷ��]��[����]��"))
	{	
		cls_show_msg("�����¿�����ֱ�����б�����.");
		return;
	}
		
	//setlinktype(WLAN);
	//cls_show_msg("�밴AP(%s)�����ý��б�������,���������...",WlanEssid);

	// 2.��������
	//conf_WLAN_lnk1();
	//conf_local();

	memset(&init, 0, sizeof(init));
	init.psEthIp = netsetting.LocalIP.Str;
	init.psEthNetmask = netsetting.LocalMask.Str;
	init.psEthGateway = netsetting.LocalGateway.Str;
	init.psEthDnsPrimary = netsetting.DNSHost[0];
	init.psEthDnsSecondary = netsetting.DNSHost[1];
	init.emSecMode = linksetting.WLANEncMode;
	init.ucIfDHCP = netsetting.DHCPenable;
	init.emKeyType =  linksetting.WLANKeyType;
	init.pszKey = linksetting.WLANPwd;

	lib_StartStopwatch();//��ʼ��ʱ
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��ret=%d", __LINE__, ret);
		return;
	}
	inittimes=lib_ReadStopwatch();
	
	// 3.���Կ�ʼ
	cls_printf("����AP(%s)��...\n", WlanEssid);
	lib_StartStopwatch();//��ʼ��ʱ
	if((ret=NDK_WiFiConnect(WlanEssid, &init))!=NDK_OK)
	{
		cls_show_msg("line %d:����%sʧ��!(%d)", __LINE__, WlanEssid, ret);
		NDK_WiFiShutdown();
		return;
	}

	oldtime = time(NULL);
	while(1)//do
	{
		if ((ret=NDK_WiFiConnectState(&state))!=NDK_OK||(diff=time(NULL)-oldtime)>30)//NORETURN_AFTERPREDIAL:0
		{
    		cls_show_msg1(10, "%s, line %d:����%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
    		NDK_WiFiShutdown();
	    	return ;
    	}
		show_stopwatch(1, diff);
		if(state==WIFI_WPA_CONSTATE_CONTEXT)
			break;
		else
			NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
	}
	if((ret=NDK_WiFiIsConnected())!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:����%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
		NDK_WiFiShutdown();
		return ;
	}
#if !K21_ENABLE
	cls_show_msg("�����״�����%s��ʱ%fs\n��ʼ����ʱ%fs", WlanEssid, lib_ReadStopwatch(), inittimes);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch(),str);
	memset(str1,0,sizeof(str1));
	ftos(inittimes,str1);
	cls_show_msg("�����״�����%s��ʱ%ss\n��ʼ����ʱ%ss", WlanEssid, str, str1);
#endif
	NDK_WiFiShutdown();
	return;
}
#else
static void wlan_ability2(void){return;}
#endif

//Ӧ����Ҫ�������������ܲ����� sull add 20171219
static void wlan_ability3(void)
{
	/*private & local definition*/
	Packet snd_packet;
	int ret = 0, cnt = PACKETLIFE, tmpflag = 0, i = 0, slen = 0, rlen = 0, timeout = SO_TIMEO, count = 0; 
	int scansucc = 0, connsucc = 0, disconnsucc = 0, commsucc = 0, succ = 0;
	float scantimes = 0, conntimes = 0, disconntimes = 0, commtimes = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	uint h_tcp = -1;
	extern LinkStatus linkstatus;
	int sflag = 0;
#if NEW_WIFI_ENABLE
	ST_WIFI_STATUS status;
	ST_WIFI_AP_LIST list_scan;
	ST_WIFI_AP_INFO ap[32];
	//char ssidout[32][33];//uint ssidoutlen = 0;//char gbk[32] = {0};
	char str[32] = {0};		
#else
	ST_WIFI_8021x_PARAM init2 ;
	ST_WIFI_PARAM init = {0};
	EM_WPA_CONSTATE state = 0;
	time_t diff = 0, oldtime = 0;
	int numList = 0;
	int initsucc = 0, downsucc = 0;
	float inittimes = 0, downtimes = 0;
	char *ESSIDlist[64];
	char ESSIDlist1[64][128];
	ST_WIFI_APINFO ESSIDlist_info[64];
	char WlanEssid1[128] = {0};
#endif

	/*process body*/
	cls_printf("���������ܲ���...");
	create_wlan_ability_packet(&snd_packet, buf);
	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=1)
		SetWifiStopflag(1);//ȷ�������ܲ���ʱwifiÿ�ζ�����stopȥ�Ҷ�
	//�ж��Ȱ������ļ���ȡ��utf-8��ʽ��ssidת����gbk��ʽ
#if !K21_ENABLE	
	if((ret = u2g(WlanEssid, strlen(WlanEssid), WlanEssid1, sizeof(WlanEssid1))) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:utf8ת��gbkʧ��(%d)", __LINE__, ret);
		return;
	}
#endif
	while(cnt--)
	{
		sflag = 0;
		if(update_snd_packet(&snd_packet)!=SUCC)
			break;
		count++;
#if NEW_WIFI_ENABLE
		if(count==1)
			NDK_WifiStop();//��wifi�ڵ�һ�β���ǰҪ��һ�ιҶϲ���,���⿪���Զ�����wifi���������ֵ���
#endif
#if NEW_WIFI_ENABLE
		//����start����
		lib_StartStopwatch();
		if((ret=NDK_WifiStart())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d����������%sʧ��!(%d)",  __LINE__, count, WlanEssid, ret);
			NDK_WifiStop();
			continue;
		}
		//�ж����ӳɹ�״̬
		if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, WlanEssid) )
		{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������%s��ʱʧ��!(ret=%d,status=%d,ssid=%s)", __LINE__,count,  WlanEssid, ret, status.status, status.ssid);
				NDK_WifiStop();
				continue;
		}
		connsucc++;
		conntimes += lib_ReadStopwatch();
		
		cls_printf("ɨ��AP��,���Ժ�...");
		list_scan.num = 32;
		list_scan.list = ap;
		lib_StartStopwatch();
		if (((NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan)) != NDK_OK)||list_scan.num <= 0) 
		{

			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��WIFIɨ��ʧ��", __LINE__ , count);
			NDK_WifiStop();
			continue;
		}
		for (i=0; i<list_scan.num; i++)
		{
			#if 0 //ͳһ:���������ap��������ɨ��ʱ��  20180131
			if(!strcmp(ap[i].ssid,""))//���������ap  ֱ�Ӵ������buf
			{
				strcpy(ssidout[i],ap[i].ssid);
				if(!strcmp(ap[i].mac.octet,WifiMac))
				{
					scansucc++;
					scantimes + lib_ReadStopwatch();
					break;
				}
			}
			#endif
			if(WifiHiddenFlag)
				break;
			/*else
			{
				if((ret=NDK_Utf8ToGbk((uchar *)ap[i].ssid,strlen(ap[i].ssid),(uchar *)ssidout[i],&ssidoutlen))!=NDK_OK)
				{
					cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:utf8ת��gbkʧ��(%d)", __LINE__, ret);
					continue;
				}
			}*///ɨ���������utf-8��ʽ,�����ļ���ȡ��Ҳ��utf-8��ʽ,�ʲ���Ҫת��
			else
			{
				if(!strcmp(ap[i].ssid,WlanEssid))//ѡ��SSID��
				{
					scansucc++;
					scantimes += lib_ReadStopwatch();
					break;
				}
			}	
		}
		if(i>=list_scan.num)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:δɨ�赽������AP������ɨ��", __LINE__);
			//NDK_WifiStop(); ɨ�費�ɹ���Ӱ��֮��Ĳ��ԣ�Ϊ�˷�ֹ����·��������ɨ�����޷�����֮��Ĳ��ԣ�������ѭ��  20180316 sull modify
			//continue;
			sflag = 1;  //���ɨ�費�ɹ����ɹ�����-1 20180320 modify
		}	
#else
		//�ж�wifi��ʱ��
		lib_StartStopwatch();
		if ((ret=NDK_WiFiInit()) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��WIFI�豸��ʼ��ʧ��(%d)", __LINE__,count , ret);
			NDK_WiFiShutdown();
			continue;
		}
		initsucc++;
		inittimes += lib_ReadStopwatch();

		memset(ESSIDlist_info,0,sizeof(ESSIDlist_info));
		for (i=0; i<64; i++)
			ESSIDlist[i]=ESSIDlist1[i];

		//ɨ��AP
		cls_printf("ɨ��AP��,���Ժ�...");
		lib_StartStopwatch();
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList) != NDK_OK) || numList <= 0) 
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��WIFIɨ��ʧ��", __LINE__ , count);
			NDK_WiFiShutdown();
			continue;
		}
		for (i=0; i<numList; i++)
		{
			if(!strcmp(ESSIDlist[i],""))  //ͳһ:���������ap��������ɨ��ʱ��  20180131
				break;
			else
			{
				//ret=NDK_WiFiSignalCover(ESSIDlist1[i], &signal);//��Ҫ����ɨ��ʱ�䣬����Ҫ��ȡ�ź�ֵӰ��ʱ��  20180129
				if(!strcmp(ESSIDlist[i],WlanEssid1))
				{
					scansucc++;
					scantimes += lib_ReadStopwatch();
					break;
				}
			}
		}
		if(i>=numList)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��δɨ�赽������AP������ɨ��", __LINE__, count);
			//NDK_WiFiShutdown(); ɨ�費�ɹ���Ӱ��֮��Ĳ��ԣ�Ϊ�˷�ֹ����·��������ɨ�����޷�����֮��Ĳ��ԣ�������ѭ��  20180316 sull modify
			//continue;
			sflag = 1;  //���ɨ�費�ɹ����ɹ�����-1 20180320 modify
		}

		init.ucIfDHCP = netsetting.DHCPenable;
		init.psEthIp = netsetting.LocalIP.Str;
		init.psEthNetmask = netsetting.LocalMask.Str;
		init.psEthGateway = netsetting.LocalGateway.Str;
		init.psEthDnsPrimary = netsetting.DNSHost[0];
		init.psEthDnsSecondary = netsetting.DNSHost[1];
		init.pszKey = linksetting.WLANPwd;
		init.emKeyType = linksetting.WLANKeyType;
		init.emSecMode = linksetting.WLANEncMode;
		
		cls_printf("����AP(%s)��...\n", WlanEssid1);
		if(WlanEncMode==WIFI_NET_SEC_WPA_EAP)
		{
			init2.comm_param = init;
			init2.pszIdentity = linksetting.WLAN8021XIdentity;
			init2.emEap = linksetting.WLAN8021XEapMode;
			init2.pszca_cert = NULL;

			lib_StartStopwatch();
			if((ret=NDK_WiFiConnect_8021x(WlanEssid, &init2))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������%sʧ��!(%d)", __LINE__, count, WlanEssid, ret);
				NDK_WiFiDisconnect();
				NDK_WiFiShutdown();
				continue;
			}
		}
		else
		{
			lib_StartStopwatch();
			if((ret=NDK_WiFiConnect(WlanEssid, &init))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������%sʧ��!(%d)", __LINE__, count, WlanEssid, ret);
				NDK_WiFiDisconnect();
				NDK_WiFiShutdown();
				continue;
			}
		}
			
		oldtime = time(NULL);
		while(1)//do
		{
			if ((ret=NDK_WiFiConnectState(&state))!=NDK_OK||(diff=time(NULL)-oldtime)>30)//NORETURN_AFTERPREDIAL:0
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������%sʧ��!(%d,%d)", __LINE__, count, WlanEssid, ret, state);
				NDK_WiFiDisconnect();
				NDK_WiFiShutdown();
				break;
			}
			show_stopwatch(1, diff);
			if(state==WIFI_WPA_CONSTATE_CONTEXT)
				break;
			else
				NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
		}
		if((ret=NDK_WiFiIsConnected())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������%sʧ��!(%d)", __LINE__, count, WlanEssid, ret);
			NDK_WiFiDisconnect();
			NDK_WiFiShutdown();
			continue;
		}
		connsucc++;
		conntimes += lib_ReadStopwatch();

#endif
		linkstatus=LINKUP;
		if((ret = NetUp()) != SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��������ʧ��!(%d)",  __LINE__, count, ret);
			LinkDown();
			continue;
		}
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, count);
			NetDown();
			LinkDown();
			continue;
		}
		
		//�����շ���ʱ��	
		lib_StartStopwatch();
		if((slen=sock_send(h_tcp, snd_packet.header, snd_packet.len, timeout))!=snd_packet.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, count, slen);
			NetDown();
			LinkDown();
			continue;
		}	
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, snd_packet.len, timeout))!=snd_packet.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν���ʧ��(%d)", __LINE__, count, rlen);
			NetDown();
			LinkDown();
			continue;
		}	
		commsucc++;
		commtimes+=lib_ReadStopwatch();

		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransDownʧ��", __LINE__, count);
			NetDown();
			LinkDown();
			continue;
		}
#if NEW_WIFI_ENABLE
		//�Ҷϲ���
		if(g_wifistopflag == 1)
		{
			lib_StartStopwatch();
			if((ret=NDK_WifiStop())!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ�%sʧ��!(%d)", __LINE__, count,WlanEssid, ret);
				NDK_WifiStop();
				continue;
			}
			disconnsucc++;
			disconntimes += lib_ReadStopwatch();
		}
		else
			;//�ر�
#else
		cls_printf("�Ͽ�AP(%s)������...\n", WlanEssid1); 
		lib_StartStopwatch();
		if((ret=NDK_WiFiDisconnect())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ�%sʧ��!(%d)", __LINE__, count,WlanEssid, ret);
			NDK_WiFiShutdown();
			continue;
		}
		disconnsucc++;
		disconntimes += lib_ReadStopwatch();

		lib_StartStopwatch();
		if((ret=NDK_WiFiShutdown())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ�%sʧ��!(%d)", __LINE__, count,WlanEssid, ret);
			NDK_WiFiShutdown();
			continue;
		}
		downsucc++;
		downtimes += lib_ReadStopwatch();
#endif
		if(!sflag)
			succ++;
		linkstatus = LINKDOWN;
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d:�ɹ�����%d\n", PACKETLIFE, succ);
	cls_show_msg_record(FILENAME,FUNCNAME, "��%sʱ��%fs\n", WlanEssid1, inittimes/initsucc);
	cls_show_msg_record(FILENAME,FUNCNAME, "ɨ��%sʱ��%fs\n", WlanEssid1, scantimes/scansucc);
	cls_show_msg_record(FILENAME,FUNCNAME, "����%sʱ��%fs\n", WlanEssid1, conntimes/connsucc);
	cls_show_msg_record(FILENAME,FUNCNAME, "ͨѶ����%fKB/s\n", (snd_packet.len*2/1024)*commsucc/commtimes);
	cls_show_msg_record(FILENAME,FUNCNAME, "�Ͽ�%sʱ��%fs\n", WlanEssid1, disconntimes/disconnsucc);
	cls_show_msg_record(FILENAME,FUNCNAME, "�ر�%sʱ��%fs\n", WlanEssid1, downtimes/downsucc);
#else
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d:�ɹ�����%d", PACKETLIFE, succ);
	memset(str,0,sizeof(str));
	ftos(scantimes/scansucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "ɨ��%sʱ��%ss\n", WlanEssid, str);
	
	memset(str,0,sizeof(str));
	ftos(conntimes/connsucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "����%sʱ��%ss\n", WlanEssid, str);

	memset(str,0,sizeof(str));
	ftos((snd_packet.len*2/1024)*commsucc/commtimes,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "ͨѶ����%sKB/s\n", str);
	
	memset(str,0,sizeof(str));
	ftos(disconntimes/disconnsucc,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "�Ͽ�%sʱ��%ss\n", WlanEssid, str);
#endif
	return;
}

static void wlan_ability(void)
{
	/*private & local definition*/
	int key=0;
	
	/*process body*/
	while (1)
	{
		key = cls_show_msg("1.��ͨ����\n"
							"2.�״���������");
		
		switch(key)
		{
		case '1':
			wlan_ability1();
			break;
		case '2':
			wlan_ability2();
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

static void wlan_comm_press(void)
{
	/*private & local definition*/
	int nKey = '0';
	void (*pf[3])(uint) = {send_recv_press, recv_only_press, send_only_press};
	uint h_tcp = -1;
	
	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			if( ++nKey == '2' ) /*�˴�ֻ��˫���շ�����Ϊ˫��͵���ĺ�̨��һ��,����2��3��Ҫ�������*/
			{
				return ;
			}
			cls_show_msg1(3, "Ҫ���Ե����͡��������ѹ����ر�����ѹ�����ؽ���");
		}
		else
		{
			nKey = cls_show_msg("_WLAN�շ�ѹ��_\n"
								"1.˫���շ�ѹ��\n"
								"2.�������ѹ��\n"
								"3.������ѹ��\n");
		}
		
		switch(nKey)
		{
		case '1':
		case '2':
		case '3':
			if(NetUp()!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
				return;
			}
			
			if(TransUp(&h_tcp)!=SUCC)
			{
				NetDown();
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:TransUpʧ��", __LINE__);
				return;
			}
			pf[nKey-'1'](h_tcp);
			TransDown(h_tcp);
			NetDown();
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

static void wlan_cnnt_press(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0;
	uint h_tcp = -1;
	
	/*process body*/
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}
	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d������ѹ��(�ѳɹ�%d��),��ESC�˳�", i+1, succ_count)==ESC)
			break;

		if(g_SequencePressFlag&&(GetCycleValue()==i))  //�Զ�ѹ�����Դ����ﵽ���˳�
			break;
		
		i++;
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransDownʧ��", __LINE__);
			continue;
		}
		succ_count++;
	}
	
	NetDown();
	
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����:%d,�ɹ�����:%d", i, succ_count);
	return;
}

static void cnnt_comm_press(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0, j = 0, loop = 0, relaykey = 0;
	int slen = 0, rlen = 0, timeout = SO_TIMEO;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	uint h_tcp = -1;
#if NEW_WIFI_ENABLE	
	ST_WIFI_AP_INFO stApInfo;
#endif	
	/*process body*/
	relaykey = cls_show_msg1(5,"ѹ�����Ƿ�Ҫ�м��:[ENTER]�� [����]��")==ENTER?TRUE:FALSE;
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}
	
BEGIN:	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d�ν���/ͨѶ���ѹ��(�ѳɹ�%d��),���ν��շ�%d��", i+1, succ_count, (loop=(rand()%10+1)))==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;

		
		if(relaykey==TRUE) 
		{
			if(i%5==0)
			{	
				j = rand();
				if(j%3==1)
					cls_show_msg1(120,"�ȴ�120��");
				else if(j%3==2)
					cls_show_msg1(180,"�ȴ�180��");
				else //if(j%3==0)
					cls_show_msg1(240,"�ȴ�240��");
			}
		}
		
		//UP
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		
		for(j=0; j<loop; j++)
		{
		//��������
#if NEW_WIFI_ENABLE
		if(NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP, &stApInfo)!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ȡ�����ӵ�AP��Ϣʧ��", __LINE__)==ESC)
				break;
			else
				continue;
		}
#endif
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
			goto BEGIN;//continue;
		}
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
			goto BEGIN;//continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��ʧ��", __LINE__);
			goto BEGIN;//continue;
		}
		}
		
		//DOWN
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransDownʧ��", __LINE__);
			continue;
		}
		succ_count++;
	}
	
	NetDown();

	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����:%d,�ɹ�����:%d", i, succ_count);
	return;
}

static void wlan_updown_press(void)
{
	/*private & local definition*/
	int i = 0, succ = 0;
	uint h_tcp = -1;
	
	/*process body*/
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d�ν���ѹ��(�ѳɹ�%d��)", i+1, succ)==ESC)
			break;

		if(g_SequencePressFlag&&(GetCycleValue()==i))  //�Զ�ѹ�����Դ����ﵽ���˳�
			break;
		
		i++;
		
		if(NetUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, i);
			NetDown();
			continue;
		}
		if(TransUp(&h_tcp)!=SUCC)//��POS�������ʵ�ʷ���һ������,�Բ�����̫�����ض����ӵ�Ӱ��
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			NetDown();
			continue;
		}
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransDownʧ��", __LINE__, i);
			NetDown();
			continue;
		}		
		if(NetDown()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, i);
			continue;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"ѹ������:%d,�ɹ�����:%d", i, succ);
	return;
}

#if !CPU5810X_ENABLE
static void ping_press(void)
{
	/*private & local definition*/
	int ret = 0, succ = 0, i = 0, cnt = DEFAULT_CNT_VLE;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen=0;

	/*process body*/
	if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
	{
		cnt = GetCycleValue();
	}
	else
	{
		cls_printf("Ĭ��ѹ�����Դ���:");
		if ((ret= lib_kbgetinput(CntStr,2, sizeof(CntStr)-1, &unLen,2, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
			cnt = DEFAULT_CNT_VLE;
		else
			cnt = atoi(CntStr);
	}
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}

	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "pingѹ��������\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", cnt, succ))
			break;
		cnt--;
		i++;

		switch(ret=NDK_NetPing(netsetting.SvrIP.Str,30))
		{
		case 0:
			succ++;
			break;
		default:
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "���󷵻�%d", ret);
			break;
		}
		
	}
	NetDown();

	cls_show_msg_record(FILENAME,FUNCNAME,"pingѹ������%d��,�ɹ�%d��", i, succ);
	return;
} 
#else
static void ping_press(void)
{
	cls_show_msg_record(FILENAME,FUNCNAME,"��֧��pingѹ������");
	return;
}
#endif

static void scan_press(void)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = SCAN_PRESS, k = 0;
	char CntStr[8] = "2000";
	uint unLen = 0;
#if NEW_WIFI_ENABLE
	ST_WIFI_AP_LIST list_scan;
	ST_WIFI_AP_INFO ap[32];
	list_scan.list = ap;
#else
	int j = 0, i = 0, numList = 0;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];
#endif

	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:");
	if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
	{
		cnt = GetCycleValue();
	}
	else
	{
		if ((ret= lib_kbgetinput(CntStr,2, sizeof(CntStr)-1, &unLen,2, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
			cnt = SCAN_PRESS;
		else
			cnt = atoi(CntStr);
	}
#if NEW_WIFI_ENABLE
	//����ǰ��:
	if(NetUp()!=SUCC)
	{
		cls_show_msg("line %d:NetUpʧ��", __LINE__);
		return;
	}
	k=0;
	while(1)
	{
		if(cls_show_msg1(2, "Wifiɨ��ѹ��������\n�ܹ�%d��,��ʣ%d��,�ɹ�%d��,ESC�˳�",cnt,cnt-k,succ)==ESC)
			break;
		if(k++==cnt)
			break;
		memset(ap,0,sizeof(ap));
		list_scan.num = 10; 
		if ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan)) != NDK_OK) 
		{	
			if(cls_show_msg1_record(FILENAME,FUNCNAME,3,"line %d:%s��%d�β���ʧ��,ESC�˳�����,����������ɨ��", __LINE__, TESTITEM, k)==ESC)
				return;
		}	
		succ++;
	}
	if(NetDown()!=SUCC)
	{
		cls_show_msg("line %d:NetDownʧ��", __LINE__);
		return;
	}
#else
	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];
	//����ǰ��:WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��ret=%d", __LINE__, TESTITEM, ret);
		return;
	}

	k=0;
	while(1)
	{
		cls_printf("Wifiɨ��ѹ��������\n�ܹ�%d��,��ʣ%d��,�ɹ�%d��",cnt,cnt-k,succ);
		if(k++==cnt)
			break;
		for (j=0; j<3; j++)
		{
			if (NDK_WiFiGetNetList(ESSIDlist, &numList) == NDK_OK)
			{
#if 0
				if (numList > 0) 
				{
					for (i=0; i<numList; i++)
						cls_show_msg1(g_keeptime, "��ɨ�赽%d��AP,line:%d,AP[%d]=%s", numList,__LINE__, i, ESSIDlist1[i]);
					break;
				}
#else
				cls_show_msg1(1, "���ι�ɨ�赽%d��ap", numList);
#endif
				break;
			}
			sleep(1);
		}
		if (j == 3) /* ����3��ɨ��ʧ������Ϊ���쳣*/
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,3,"line %d:%s��%d�β���ʧ��,ESC�˳�����,����������ɨ��", __LINE__, TESTITEM,k)==ESC)
				return;
		}
		succ++;
	}
	NDK_WiFiShutdown();
#endif	
	cls_show_msg_record(FILENAME,FUNCNAME,"wifiɨ��ѹ������%d��,�ɹ�%d��", k-1, succ);
	return;
} 

//���˵��:���ǵ�֧��wifi�Ļ������ܲ�������̫����,��pingһ���Э��Ͳ���ͨ����̫���ۺϲ���������,���ڱ�������ҲӦ��ping�Ĳ���
static void wlan_press(void)
{
	/*private & local definition*/
	int nKey='0';
	
	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			if( ++nKey == '7' ) //��Ӧ����Ĳ˵���+1���˵����Ӵ˴�ҲҪ��Ӧ������
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����ѹ�����Խ���",TESTITEM);
				return ;
			}
			if(cls_show_msg1(3, "������������ѹ������,ESC�˳�")==ESC)
				return;
		}
		else
		{
			nKey = cls_show_msg("1.����2.����\n"
								"3.����+����\n"
								"4.����ѹ��\n"
								"5.pingѹ�� 6.ɨ��");
		}
		switch(nKey)
		{
		case '1':
			wlan_comm_press();
			break;
		case '2':
			wlan_cnnt_press();
			break;
		case '3':
			cnnt_comm_press();
			break;
		case '4':
			wlan_updown_press();
			break;
		case '5':
			ping_press();
			break;
		case '6':
			scan_press();
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

//���˵��:���ǵ���2��AP����û�п��õ�TCP/UDP��̨,��������Բ������������Ӷ���
void wlan_cross_AP(void)
{
#if NEW_WIFI_ENABLE
	/*private & local definition*/
	int ret = -1, retime = -1, i = 0, succ = 0;
	ST_WIFI_STATUS status;
	time_t oldtime;

	/*process body*/
	//��������ʱ��
	retime = 5000;//5000ms  
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)
	{
		cls_show_msg("line %d: ��������ʧ��(%d)", __LINE__, ret);
		return;
	}
	if(NetUp()!=SUCC)
	{
		cls_show_msg( "line %d:NetUpʧ��", __LINE__);
		NetDown();
		return;
	}
	i=0;
	while(1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d�ν���%s(�ѳɹ�%d��),ESC�˳�", i+1, WlanEssid, succ)==ESC)
			break;
		if(i++==10)
			break;
		cls_show_msg("��رղ���·���������������");
		/*if((ret = lib_getwifistatus(&status)) != NDK_OK)
		{
			cls_show_msg("line %d:��ȡwifi״̬ʧ��", __LINE__, ret);
			return;
		}*/
		//�����÷�װ������ԭ���ǹر�·������ײ㲢����������Ӧ,��ô�ڻ�ȡ״̬ʱ�����ȡ���ɹ���״̬��Ϊ�˱���Ӱ����Խ��
		oldtime=time(NULL);
		while(1)
		{
			if((time(NULL)-oldtime)>30)
			{
				cls_show_msg1(3,"δ��ȡ����ȷ��״̬,��ʱ�˳�");
				break;
			}
			if((ret = NDK_WifiStatus(&status)) != NDK_OK)
			{
				cls_show_msg("line %d:��ȡ wifi״̬ʧ��(%d)", __LINE__, ret);
				return;
			}
			if(status.status == WIFI_WLAN_STATUS_FAIL)
				break;
		}
		if(status.status != WIFI_WLAN_STATUS_FAIL)
		{
			cls_show_msg("line %d: ��ȡ״̬ʧ��(%d)", __LINE__, status.status);
			return;
		}
		cls_show_msg("��򿪲���·����10s�����������");
		/*if((ret = lib_getwifistatus(&status)) != NDK_OK)
		{
			cls_show_msg("line %d:��ȡwifi״̬ʧ��", __LINE__, ret);
			return;
		}*/
		//�����÷�װ������ԭ����Ϊ�˱��ⰴ����������ܻ��ȡ��ʧ��״̬ Ӱ����Խ��
		oldtime=time(NULL);
		while(1)
		{
			if((time(NULL)-oldtime)>30)
			{
				cls_show_msg1(3,"δ��ȡ����ȷ��״̬,��ʱ�˳�");
				break;
			}
			if((ret = NDK_WifiStatus(&status)) != NDK_OK)
			{
				cls_show_msg("line %d:��ȡ wifi״̬ʧ��(%d)", __LINE__, ret);
				return;
			}
			if(status.status == WIFI_WLAN_STATUS_SUCC)
				break;
		}
		if(status.status != WIFI_WLAN_STATUS_SUCC )
		{
			cls_show_msg("line %d: ��ȡ״̬ʧ��(%d)", __LINE__, status.status);
			return;
		}
		succ++;
	}
	if(NetDown()!=SUCC)
	{
		cls_show_msg("line %d:NetDownʧ��", __LINE__);
		return;
	}
	cls_show_msg("��AP����ͨ��...");
	//�ָ�Ĭ��ֵ
	retime = -1;  
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)
	{
		cls_show_msg("line %d: ��������ʧ��(%d)", __LINE__, ret);
		return;
	}
#else
	/*private & local definition*/
	LinkSetting linksetting1, linksetting2;
	NetworkSetting netsetting1, netsetting2;
	int total1 = 0, succ1 = 0, total2 = 0, succ2 = 0, *total = NULL, *succ = NULL;

	/*process body*/
	// 1.���ݵ�1��AP(��1��AP��Ӧ��֮ǰ�������������ù���)
	memcpy(&linksetting1, &linksetting, sizeof(LinkSetting));
	memcpy(&netsetting1, &netsetting, sizeof(NetworkSetting));
	
	// 2.���õ�2��AP,Ҳ���б���(Ҳ��ȫ�����õ������������ľֲ�����)
	cls_show_msg("������%sΪ��1��AP,����������2��AP������...", linksetting1.WLANEssid);
	//�ֹ�������·����
	g_conf_flag = 0;//Ϊ����֮ǰʹ�ù������ļ�,����Ҫ���������ó��ֶ�����
	if(conf_link(WLAN)!=SUCC)
	{
		cls_show_msg("line %d:������·����ʧ��", __LINE__);
		goto STEP4;
	}
	//�ֹ����ñ����������
	if(conf_local()!=SUCC)
	{
		cls_show_msg("line %d:���ñ����������ʧ��", __LINE__);
		goto STEP4;
	}
	memcpy(&linksetting2, &linksetting, sizeof(LinkSetting));
	memcpy(&netsetting2, &netsetting, sizeof(NetworkSetting));

	// 3.��AP����(����ǰ��Ҫ�����ݵ�AP����ȫ������)
	while (1)
	{
		if((total1+total2)%2)//if(rand()%2)
		{
			total = &total1;
			succ = &succ1;
			memcpy(&linksetting, &linksetting1, sizeof(LinkSetting));
			memcpy(&netsetting, &netsetting1, sizeof(NetworkSetting));
		}
		else
		{
			total = &total2;
			succ = &succ2;
			memcpy(&linksetting, &linksetting2, sizeof(LinkSetting));
			memcpy(&netsetting, &netsetting2, sizeof(NetworkSetting));
		}
		
		if(cls_show_msg1(2, "���ڽ��е�%d�ν���%s(�ѳɹ�%d��)", *total+1, WlanEssid, *succ)==ESC)
			break;
		++*total;
		
		if(NetUp()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, *total);
			NetDown();
			continue;
		}
		if(NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, *total);
			continue;
		}
		++*succ;
	}
	
	cls_show_msg("����AP1(%s)����:%d\n�ɹ�����:%d", linksetting1.WLANEssid, total1, succ1);
	cls_show_msg("����AP2(%s)����:%d\n�ɹ�����:%d", linksetting2.WLANEssid, total2, succ2);

STEP4:
	// 4.�ָ���1��APΪ���ۺϲ��Ե�����AP
	memcpy(&linksetting, &linksetting1, sizeof(LinkSetting));
	memcpy(&netsetting, &netsetting1, sizeof(NetworkSetting));
	
#endif	
	return;
}

//Ŀǰ���ڽ���(ǳ/��)����ʱ,�������wlan_free���ͷ���Դ
static void wlan_sleep(void)
{
	/*private & local definition*/
	extern NetStatus netstatus;
	extern LinkStatus linkstatus;
	int nRet = -1, i = 2;
	uint h_tcp = -1;
	
	/*process body*/
#if NEW_WIFI_ENABLE 
	ST_WIFI_STATUS status;
	int retime = 1;
	time_t oldtime;

#if CPU5810X_ENABLE
	//��������:5810ƽ̨�����ص���ʱ����ʹ�����Զ�����
	uint a = 0;
	NDK_SysStopWatch(&a);
#endif

	//��������ʱ��
	retime = 5000;//5000ms  
	if((nRet = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)
	{
		cls_show_msg("line %d: ��������ʧ��(%d)", __LINE__, nRet);
		return;
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
			cls_show_msg("������ѽ���!�������������,һ��ʱ��(���Ƕ�ʱ��ͳ�ʱ��)���밴������.");
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
		
		cls_printf("���˳�����!���½���wlan������...");
		//�������ߺ�,Ԥ��WLANģ���ѱ�free,������Ӧ�ѶϿ�
		netstatus = NETDOWN;
		linkstatus = LINKDOWN;
		//���½���wifi����/�Ͽ�Ӧ����
#if NEW_WIFI_ENABLE
		//��wifi�ܹ��Զ��������ɹ�
		//���ݻƾ����������ǳ�ʱ�����ߺ�����Ҫʱ���������,�ʸĳ�ѭ����ȡ����״̬ 20180718 modify
		oldtime=time(NULL);
		while(1)
		{
			if((nRet = NDK_WifiStatus(&status)) != NDK_OK)
			{
				cls_show_msg("line %d:��ȡwifi״̬ʧ��", __LINE__, nRet);
				return;
			}
			if(status.status == WIFI_WLAN_STATUS_SUCC)
				break;
			if((time(NULL)-oldtime)>MAXWAITTIME)//60s��ʱ
			{
				cls_show_msg("line %d: wifi����ʧ��,��ʱ�˳�(status=%d)", __LINE__, status.status);
				return;
			}
		}
#else
		if(NetUp()!=SUCC)
		{
			cls_show_msg("line %d:NetUpʧ��", __LINE__);
			return;
		}
#endif
//����ͨѶ 
		if(TransUp(&h_tcp)!=SUCC)
		{
			NetDown();
			cls_show_msg("line %d:TransUpʧ��", __LINE__);
			return;
		}
		//������10��ͨѶ���Լ���
		send_recv_press(h_tcp);
		TransDown(h_tcp);
		if(NetDown()!=SUCC)
		{
			cls_show_msg("line %d:NetDownʧ��", __LINE__);
			return;
		}
		i--;
	}

	cls_show_msg("WLAN���߲��Գɹ�");
	return;
}

//ѡ��λ��λ��
static int select_rst_flag(void)
{
	/*private & local definition*/
	int nKeyin = 0, flg[] = {RESET_PPPOPEN, RESET_PPPCLOSE, RESET_TCPOPEN, RESET_TCPCLOSE, RESET_NO};

	/*process body*/
	while (1)
	{	
		nKeyin = cls_show_msg("_ѡ��λ����_\n"
							"WLAN:1.��2.�ر�\n"
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

static void create_WLAN_reset_packet(Packet *packet, char *buf)
{
	/*private & local definition*/

	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len = PACKMAXLEN;//����
	packet->lifecycle = 0;
	packet->forever = TRUE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

//wifiͨѶ����
int wlan_dial_comm(Packet *sendpacket, int flag)
{
	/*private & local definition*/
	uint h_tcp = -1;
	char rbuf[PACKMAXLEN] = {0};
	int slen = 0, rlen = 0, ret = 0, timeout = SO_TIMEO;
	
	/*process body*/
	//����ǰ��
	if((ret=NetUp())!=SUCC)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:NetUpʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	if(flag==RESET_PPPOPEN)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "WLAN������(TCPδ��)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	if((ret=TransUp(&h_tcp))!=SUCC)
	{
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:TransUpʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	if(flag==RESET_TCPOPEN)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "TCP�Ѵ�(WLAN������)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	//��������
	if((slen=sock_send(h_tcp, sendpacket->header, sendpacket->len, timeout))!=sendpacket->len)
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __FILE__, __LINE__, slen, sendpacket->len);
		return FAIL;
	}
	//��������
	memset(rbuf, 0, sizeof(rbuf));
	if ((rlen=sock_recv(h_tcp, rbuf, sendpacket->len, timeout))!=sendpacket->len)
	{
		TransDown(h_tcp);//TransDown(netsetting.socktype, h_tcp);
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __FILE__, __LINE__, rlen, sendpacket->len);
		return FAIL;
	}
	if(MemCmp(sendpacket->header, rbuf, rlen))
	{
		TransDown(h_tcp);
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:У��ʧ��", __FILE__, __LINE__);
		return FAIL;
	}	
	if ((ret=TransDown(h_tcp))!=SUCC)
	{
		NetDown();
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "%s, line %d:TransDownʧ��", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	if(flag==RESET_TCPCLOSE)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "TCP�ѹر�(WLANδ�ر�)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	NetDown();
	if(flag==RESET_PPPCLOSE)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,2, "WLAN�ѹر�(TCP�ѹر�)...����������...");
		NDK_SysReboot();//nlexit(0);
	}
	return SUCC;
}

//wifi��λ����
static void wlan_reset(void)
{
	/*private & local definition*/
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	int rst_flag = RESET_NO;
	
	/*process body*/
	if((rst_flag=select_rst_flag())==RESET_NO)
		return;
	
	create_WLAN_reset_packet(&sendpacket, buf);
	update_snd_packet(&sendpacket);

	wlan_dial_comm(&sendpacket, rst_flag);
	cls_show_msg("WIFI��λ����ʧ��");//nener reach
	return;
}

//���˵��:���������ǰ����AP�ź�ǿ�Ⱥ;���ɷ��ȣ�����Խ���ź�Խǿ���������豸�������
void wlan_same_AP(void)
{
	/*private & local definition*/	
	char szIPaddr[16] = {0}, tmp_szIPaddr[16] = {0};
	time_t oldtime = 0;

	/*process body*/
	cls_show_msg("�뱣��״̬����������׼������ͬ����AP(��ͬ�ļ���ģʽ�����룬��ͬ��IP����),������AP����һ���ľ���(20��)�����������");
	//�ֹ�������·����
	if(conf_link(WLAN)!=SUCC)
	{
		cls_show_msg("line %d:������·����ʧ��", __LINE__);
		return;
	}
	//������Ϊ���ܹ��������ӵ���AP���������β��ö�̬���䷽ʽ��ȡIP
	netsetting.DHCPenable = ENABLE;

	// 3.��AP����(����ǰ��Ҫ�����ݵ�AP����ȫ������)
	cls_show_msg("�뿿������һ��AP���������ʼ����AP");
	if(NetUp()!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:NetUpʧ��", __LINE__);
		return;
	}
	memset(szIPaddr,0,sizeof(szIPaddr));	
	while (1)
	{		
		oldtime=time(NULL);
		while(1)
		{
			if((time(NULL)-oldtime)>MAXWAITTIME)
			{
				cls_show_msg1(g_keeptime,"��ʱ(60s)δȡ����ͬIP�˳�");
				break;	
			}
			strcpy(tmp_szIPaddr,szIPaddr);
			NDK_NetGetAddr(COMM_TYPE_WIFI, szIPaddr, NULL, NULL, NULL);
			if(strcmp(tmp_szIPaddr,szIPaddr))
				break;
			
		}
		if(cls_show_msg("�Ѿ��ɹ����ӵ�AP:%s,����IP��ַΪ:%s,��IP�Ƿ�Ϊ�Ͻ���AP��������:[��]��ȷ�ϣ�����������", WlanEssid, szIPaddr)!=ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:ͬ��AP����ʧ��", __LINE__);
			NetDown();//�����Ҷϴ�������
			NetUp();
			continue;
		}
		
		if(cls_show_msg("���ƶ�POS�뿪�����ӵ�AP���źŷ�Χ����״̬����AP�ź���ʧ�󿿽���һ��ͬ��AP,��״̬������AP�źź������,ȡ�����˳�����")==ESC)
			break;
	}
	
	if(NetDown()!=SUCC)
	{
		cls_show_msg1(g_keeptime, "line %d:NetDownʧ��", __LINE__);
		return;
	}
	
	cls_show_msg("ͬ��AP�������");
	return;
}

static void wlan_abnormity(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while (1)
	{
		keyin = cls_show_msg("1.��AP  2.��λ\n" 	//����AP
							"3.����\n"				// 6.ͬ��\n,��ʱ���θ�����,�ĳ�ʹ��4.����ѹ�����в���
							"4.�����쳣\n"
							"5.��ֹDHCP\n");
		switch (keyin)
		{
		case '1':
			wlan_cross_AP();
			break;
		case '2':
			wlan_reset();
			break;
		case '3':
			if(ENTER==cls_show_msg("��ֱ��ڹ�����������/ǳ����ģʽ����в���,[ENTER]����,[����]����"))
				wlan_sleep();//WLAN���ǳ����ģʽ�¶�Ӧ��Ҫ�����Զ����ߺͺ������ߵĹ�����֤ 20140615 linwl
			break;
		case '4':
			cls_show_msg("������δ������̨�ķ�������ַ,Ӧ������ͨ!");
			break;
	//	case '4':
			//cls_show_msg("�뽫һ��AP���ó�����SSID,Ӧɨ�費����SSID,��Ӧ���ܹ����Ӹ�AP");//��ֲ����Ԫ����������linwl20130614
	//		break;
		case '5':
			cls_show_msg("�뽫һ��AP��DHCP�����óɽ�ֹ�ģ�ʹ�þ�̬����Ӧ���ܹ��ɹ�!");
			break;
	//	case '6':
	//		wlan_same_AP();
	//		break;
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

void AP_Compatibility_test(void)
{
	int ApNUM = 0, i = 0, dhcpflag = 0, cnt = 0;
	char localip[16] = {0}, localmask[16] = {0}, localgateway[16] = {0};
	char pInfo[64] = {0}, tmp[64] = {0};

	setlinktype(WLAN);
	//��ȡ�����Ե�AP����,��Ӧ�����ļ��е�APNUM
	if(GetParaInfo("APNUM",pInfo)!=SUCC)
		return;
	if(trim_string2int(pInfo,&ApNUM)!=SUCC)
		return;
	//���Ӵ������ļ���ȡ���Դ��� 20171101 add by jym
	if(GetParaInfo("CycleTime",pInfo)!=SUCC)
		return;
	if(trim_string2int(pInfo,&cnt)!=SUCC)
		return;
	//�����Զ������Ա�־,�����Ʋ��Դ���Ϊ30 (�����������ļ���)
	g_CycleTime = cnt;//30;
	g_conf_flag = 1;
	g_SequencePressFlag = 1;
	g_keeptime = 3;
	g_PackLen = 4096; //���Ӱ��ĳ�ʼ����,�������ʱû������ֱ�ӽ���ʱʹ�õ����ݳ���Ϊ0, 20180612 modify

	//���÷�������ַ�Ͷ˿�
	if(conf_server()!=SUCC)
 		return;
	//����socketЭ��
	if(conf_trans()!=SUCC)
		return;
	//�Ͷ���Ҫ����������ת��ʹ�õ�ͨѶ�ӿ� 20171114
#if  CPU5810X_ENABLE||defined ME50NX ||defined ME68 
	NDK_NetAddRouterTable(COMM_TYPE_WIFI,"0.0.0.0");//netsetting.SvrIP.Str
#endif
	for(i=1;i<=ApNUM;i++)
	{
		//����AP���Ӳ���:SSID,PWD,ENCMODE��
		if(use_wifi_conf(i)!=SUCC)//��������ļ���ɾ����һ�������ڵ�·�ɺ���Լ�������ִ��20170927
			continue;
		
		//�������ļ���ȡDHCP��־
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp, "WifiDHCPenable%d", i);
		if(GetParaInfo(tmp,pInfo)!=SUCC)
			return;
		if(trim_string2int(pInfo,&dhcpflag)!=SUCC)
			return;
		if(dhcpflag)//Ϊ1 ʹ�� ʹ���Զ����䱾�ص�ַ
		{
			netsetting.DHCPenable = ENABLE;
		}
		else//Ϊ0��Ҫ�������ļ��ж�ȡ��̬���ص�ַ
		{
			//��ȡ��̬IP
			netsetting.DHCPenable = DISABLE;
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp, "WifiLocalIP%d", i);
			if(GetParaInfo(tmp,pInfo)!=SUCC)
				return;
			if(trim_string(pInfo,localip)!=SUCC)
				return;
			strcpy(netsetting.LocalIP.Str,localip);
			//��ȡ��������
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp, "WifiLocalMask%d", i);
			if(GetParaInfo(tmp,pInfo)!=SUCC)
				return;
			if(trim_string(pInfo,localmask)!=SUCC)
				return;
			strcpy(netsetting.LocalMask.Str,localmask);
			//��ȡ����
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp, "WifiLocalGatway%d", i);
			if(GetParaInfo(tmp,pInfo)!=SUCC)
				return;
			if(trim_string(pInfo,localgateway)!=SUCC)
				return;
			strcpy(netsetting.LocalGateway.Str,localgateway);
			netsetting.LocalIP.value.data = INET_ADDR(netsetting.LocalIP.Str);
			netsetting.LocalMask.value.data = INET_ADDR(netsetting.LocalMask.Str);
			netsetting.LocalGateway.value.data = INET_ADDR(netsetting.LocalGateway.Str);
			cls_show_msg1(1,"��������������óɹ�");
		}

		//������IP
		//socketЭ��
		
#if NEW_WIFI_ENABLE
		//����ȫ�ֲ�����AP�б��ļ���,ͬʱ���ú��б�
		if(set_addedapinfo_fromgloabvar()!= SUCC)
		{
			cls_show_msg1(10, "%s, line %d:AP�б�����ʧ��!(%d)", __FILE__, __LINE__);
			return;
		}
#endif
		if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"������AP%d:%s�ļ����Բ���,��������:", i, WlanEssid)==ESC)
			return;
		//wlan_comm_press();//ԭ���Ĳ�������˫���շ�ѹ��������+����,�ָĳ�������+�����ͽ���ѹ��
		cnnt_comm_press();//����+����
		wlan_updown_press();//����ѹ��
		wlan_ability3();//Ӧ��������ʹ�ø���ϸ�����ܲ������������ֵ�ɼ� 20171220
	}
	
	return;
}

//��������
void wlan_roam()
{
	/*private & local definition*/
	uint h_tcp = -1;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	int i = 0,  timeout = SO_TIMEO, cnt = 0, num = 0, ret = 0;
	uint slen = 0, rlen = 0;
	Packet sendpacket;
#if NEW_WIFI_ENABLE
	ST_WIFI_AP_INFO succ_ap;
#else
	char szMac[64] = {0};
	int j=0;
#endif

	/*process body*/
	/*���λ���:׼���ö�̨·����(��̨����)������Ϊ��ͬ��AP�����룬�������ĸ���ͬ�ĵط���ÿ��̨·����֮�����ԶһЩ�����2-3��*/
	if((cls_show_msg1(30, "��ȷ���Ѵ�����λ�������ú���������в��ԣ�����ESC�˳�")) == ESC)
		return;
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	cnt = sendpacket.lifecycle;
	num = cnt;

	/*process body*/
	if(NetUp()!=SUCC)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:NetUpʧ��", __LINE__);
		return;
	}
	if(TransUp(&h_tcp)!=SUCC)
	{
			NetDown();
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			return;
	}
	while(cnt--)
	{
		//�޸�Ϊ�ܹ�һֱ�鿴mac��ַ��������Ļˢ�µ�mac��ַ��Ϣ  20180302 sull modify
		NDK_ScrPrintf("ͨѶ�ɹ�%d�Σ��˳���ESC",i);
		NDK_KbHit(&ret);
		if(ret == ESC)
			break;
		//��������:Ҫ�ܹ��鿴mac��ַ��Ϣ,�ж���ʵʱ��ȡmac��ַ�Ľӿڣ��޷���������
#if NEW_WIFI_ENABLE
		memset(&succ_ap,0,sizeof(succ_ap));
		if( ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_CONNECT_AP,&succ_ap))!=NDK_OK) || strcmp(succ_ap.ssid,WlanEssid) )
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:�����ϴ����AP(ret = %d, ap = %s)", __LINE__, ret, succ_ap.ssid);
			continue;
		}
		cls_printf("��ǰmac��ַ:%02x:%02x:%02x:%02x:%02x:%02x", succ_ap.mac.octet[0], succ_ap.mac.octet[1], succ_ap.mac.octet[2], succ_ap.mac.octet[3], succ_ap.mac.octet[4], succ_ap.mac.octet[5]);
#endif
		//������ͨѶ�����ж�
		if(update_snd_packet(&sendpacket)!=SUCC)
			continue;
		 //�޸�Ϊ�ܹ�һֱ�鿴mac��ַ��������Ļˢ�µ�mac��ַ��Ϣ  20180302 sull modify
		//��������
		if(((ret=NDK_TcpWrite(h_tcp, sendpacket.header, sendpacket.len, timeout, &slen))!=NDK_OK) || (slen != sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η�������ʧ��(ret = %d, ʵ��%d, Ԥ��%d)", __LINE__, i, ret, slen, sendpacket.len);
			TransDown(h_tcp);
			TransUp(&h_tcp);
			continue;
		}
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if(((ret=NDK_TcpRead(h_tcp, rbuf, sendpacket.len, timeout, &rlen))!=NDK_OK) || (rlen != sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν�������ʧ��(ret = %d, ʵ��%d, Ԥ��%d)", __LINE__, i, ret, rlen, sendpacket.len);
			TransDown(h_tcp);
			TransUp(&h_tcp);
			continue;
		}	
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��У��ʧ��", __LINE__, i);
			TransDown(h_tcp);
			TransUp(&h_tcp);
			continue;
		}
		i++;
	}
	TransDown(h_tcp);
	NetDown();
	cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "wifi����ͨѶ�ܴ���%d��,�ɹ�%d��", num, i);
	return; 
}

void systest11(void)
{
	/*private & local definition*/
	int nKey = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_WLAN()!=SUCC)
			return;
		
		g_CycleTime = 100;
		//�Զ�����������
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��ѹ������
		wlan_press();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	else
	{
		while (1)
		{
			nKey = cls_show_msg("1.ѹ�� 2.����\n"
							    "3.�쳣 4.������\n"
							    "5.���� 0.����\n");
			switch(nKey)
			{
			case '1':
				wlan_press();
				break;
			case '2':
				wlan_ability();
				break;
			case '3':
				wlan_abnormity();
				break;
			case '4':
				AP_Compatibility_test();
				break;
			case '5':
				wlan_roam();
				break;
			case '0':
				switch(conf_conn_WLAN())
				{
				case SUCC:
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
				cls_show_msg("�뽫������AP��DHCP�ֱ����óɽ�ֹ�Ϳ������ֱ�ʹ�þ�̬�Ͷ�̬����Ӧ���ܹ��ɹ�!");
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

