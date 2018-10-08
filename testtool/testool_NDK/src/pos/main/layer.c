
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ���Գ����κ���
* file name		: layer.c
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20101217
* directory 		: .\SRC\main\
* description		: 
* related document	: 
* 
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	MAXTIMEOUT		(60)		//for UP/DOWN�ĳ�ʱʱ��
#define	TIME4KEEPSTATE	(2)		//ȷ��״̬���ֵĴ���,����Ҫ���ȷ��ʱ,����ֵ��Ϊ1����
#define	MAXTIMEOUT_PING	(5)

#define SCERTF  "cacert.pem"

#define WM_PPP_MSGSTRLEN		16
#define PPP_HANDLE_16300	1
#define WM_PPP_STATE_SUM_16300	9
#define WM_PPP_ERROR_SUM	5

enum{
	WM_PPP_NULL_16300,			//��ʼ̬
#define WM_PPP_ERROR_STARTSTATE	WM_PPP_NULL_16300	
	WM_PPP_CONNECT,
	WM_PPP_PROTOCOL,
#define WM_PPP_ERROR_STOPSTATE	WM_PPP_PROTOCOL		//����״̬		
	WM_PPP_WELCOME,
	WM_PPP_USERNAME,
	WM_PPP_PASSWORD,
	WM_PPP_MODE,
	WM_PPP_ENTHERNET,
	WM_PPP_SYSTEM,
	WM_PPP_IPMTU,				//9������״̬
	WM_PPP_ERROR_NO_DIALTONE,
	WM_PPP_ERROR_NO_CARRIER,
	WM_PPP_ERROR_BUSY,
	WM_PPP_ERROR_UNUMBER,
	WM_PPP_ERROR_PRONONE		//5�ֳ���״̬
};

typedef struct {
	unsigned int state;
	unsigned char statestr[WM_PPP_MSGSTRLEN];
}pppdial_state;

typedef struct {
	unsigned int state_sum;
	unsigned int state;
	unsigned int error_start;
	unsigned int error_stop;
	unsigned int error_sum;
	pppdial_state *pstate_str;
	pppdial_state *perror_str;
}pppdialstatetype;

/*------------global variables definition-----------------------*/
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
static uint g_socket = 0;//���ڹر�ʱʹ��
sock_t g_socktype = SOCK_DEFAULT;//���ڹر�ʱʹ��
#endif
//��ȫ�ֱ���ģ��ʵ����Դ����
#if 0	//SDLC_ENABLE||ASYN_ENABLE
static ModemStatus	mdmstatus	= INITIAL;
#endif
//����������ʵ����Ҳ��4���ڲ�"����"�����Ƿ���ù��ı�־
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
/*static*/ LinkStatus linkstatus 	= LINKDOWN;
/*static*/ NetStatus netstatus	= NETDOWN;
static TransStatus	transstatus	= TRANSDOWN;
NetworkSetting netsetting;
int TCPRESETFLAG = FALSE;//tcp ǿ�ƹҶϱ�־λ��Ĭ��Э��Ҷ� 20140422 linwl
#endif
LinkSetting linksetting;

/*---------------functions declaration--------------------------*/
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
static int __LinkDown(void);
#endif

/*---------------functions definition---------------------------*/
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
int serial_clrportbuf_all(EM_PORT_NUM port)
{
	/*private & local definition*/
	//int ret = 0;

	/*process body*/
	while(NDK_PortTxSendOver(port)!=NDK_OK);//���з����ʱ��//ret = clrportbuf(port, 0);
	//ret1 = clrportbuf(port, 1);
	return NDK_PortClrBuf(port);//return (ret==SUCC && ret1==SUCC)?SUCC:FAIL;
}
#endif

void init_linksetting(void)
{
	/*private & local definition*/
	
	/*process body*/
	memset(&linksetting, 0, sizeof(linksetting));

	//��һ��ʼ����ͨ�÷���
#if defined ME11||defined ME15||defined ME20||defined ME15CHX||defined ME15B||defined ME66
	if(COMNUM==0)
		g_com = PORT_NUM_USB;//ME11���ʼ����USB���ڣ������0xff�Ĵ��ڻ�����USB���ڹ����쳣 20140520 
	else
		g_com = PORT_NUM_COM1;//����USB����,ʹ�� ������
#elif ANDIROD_ENABLE||defined ME15C //||defined N900
		g_com = PORT_NUM_COM2;//N900ֻ��COM2
//#elif defined IM81AZ
//		g_com = PORT_NUM_COM1;//IM81AZ��COM1
#else
	g_com = INVALID_COM;//zhangnw20060410 add
#endif
	send_flag = TRUE;//zhangnw20060831 add for �Ƿ����͵ı�־
	setlinktype(NONE);//linktype = NONE;
	
	strcpy(BtPin, DEFAULT_BT_PIN);
	strcpy(BtName, DEFAULT_BT_NAME);
#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ���
	strcpy(g_bps, DEFAULT_BPS);//g_bps = DEFAULT_BPS;
	MDMPatchType = MDM_PatchType5;
	strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);
	strcpy(WLMAPN, DEFAULT_WLM_APN);
	strcpy(PPPUSR, DEFAULT_PPP_USR);
	strcpy(PPPPWD, DEFAULT_PPP_PWD);
#if DOUBLESIM_ENABLE
	SIMSLOT = SIM_SLOT1;
#endif
	g_devicecom = PORT_NUM_COM1;
	strcpy(g_devicecombps, DEFAULT_BPS);//g_devicecombps = DEFAULT_BPS;
	WlanEncMode = 0;//NOENCRYPT;
	WlanKeyType = 0;//NOKEY;
	strcpy(WlanPwd, DEFAULT_WLAN_PWD);
	strcpy(WlanEssid, DEFAULT_WLAN_ESSID);
	Wlan8021XEapMode = 0;
	strcpy(Wlan8021XIdentity, DEFAULT_WLAN_8021XIDENTITY);	

	//wifi mac�����ñ�־λ��ʼ��
	memset(WifiMac, 0, sizeof(WifiMac));//��ʼ���ɲ�ʹ��MAC
	WifiMacFlag = 0;

	//wifi ap ������ʼ��ȫ�ֲ���
	WifiApShareDev = 0;//WIFIAP_SHARE_DEV_ETH;
	WifiApSecMode = 4;//WIFI_NET_SEC_WPA2;
	strcpy(WifiApSsid, DEFAULT_WLANAP_SSID);
	WifiApHidden = 0;
	strcpy(WifiApKey, DEFAULT_WLANAP_KEY);
#endif	
	return;
}

//linktype���ݵĲ�������
void setlinktype(LinkType type){linktype = type;}
LinkType getlinktype(void){return linktype;}

#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ���
void init_netsetting(void)
{
	/*private & local definition*/
	
	/*process body*/
	memset(&netsetting, 0, sizeof(netsetting));

	//��һ��ʼ����ͨ�÷���
	//zhangnw201105modify ����INET_ADDR,���⿼���ֽ��������
	strcpy(netsetting.SvrIP.Str, DEFAULT_SVR_IP_STR);
	netsetting.SvrIP.value.data = INET_ADDR(DEFAULT_SVR_IP_STR);//DEFAULT_SVR_IP_VLE;
	netsetting.SvrTcpPort = DEFAULT_SVR_TCP_PORT;
	netsetting.SvrUdpPort = DEFAULT_SVR_UDP_PORT;
	strcpy(netsetting.LocalIP.Str, DEFAULT_LOCAL_IP_STR);
	netsetting.LocalIP.value.data = INET_ADDR(DEFAULT_LOCAL_IP_STR);//DEFAULT_LOCAL_IP_VLE;
	netsetting.LocalPort = 0;
	strcpy(netsetting.LocalMask.Str, DEFAULT_LOCAL_MASK_STR);
	netsetting.LocalMask.value.data = INET_ADDR(DEFAULT_LOCAL_MASK_STR);//DEFAULT_LOCAL_MASK_VLE;
	strcpy(netsetting.LocalGateway.Str, DEFAULT_LOCAL_GATEWAY_STR);
	netsetting.LocalGateway.value.data = INET_ADDR(DEFAULT_LOCAL_GATEWAY_STR);//DEFAULT_LOCAL_GATEWAY_VLE;
	netsetting.socktype = SOCK_NDKTCP;
#if 0
	netsetting.SSLHsOpt = HANDSHAKE_SSLv2;
	netsetting.SSLAuthOpt = SSL_AUTH_NONE;
	netsetting.SSLFileFmt = FILE_PEM;
#endif
	//ά��һ�����²�һ�µ�DNS��
	strcpy(netsetting.DNSHost[0], DNS1);
	strcpy(netsetting.DNSHost[1], DNS2);
	strcpy(netsetting.DNSHost[2], DNS3);
	netsetting.DHCPenable = DISABLE;

	//wifi ap ����ip
	strcpy(netsetting.WifiApIp.Str, DEFAULT_WIFIAP_IP_STR);
	netsetting.WifiApIp.value.data = INET_ADDR(DEFAULT_WIFIAP_IP_STR);
	return;
}

//����2��������Ҫʱ���԰�״̬��ΪDOWN,һ������²���Ҫʹ��
void set_linkstatus_down(void){linkstatus = LINKDOWN;}
void set_netstatus_down(void){netstatus = NETDOWN;}

ENDIAN_t GetCurrentEndianType(void)
{
#if 0
	/*private & local definition*/
	Value value;

	/*process body*/
	value.data = 0x01;
	return (ENDIAN_t)((value.asc[0]==0x01)?LITTLEENDIAN:BIGENDIAN);
#else
	return ENDIAN;
#endif
}

#if 0
ulong INET_ADDR(char *a)
{
	/*private & local definition*/
	char *p = a;
	uchar data[4] = {0};
	int cnt = 0;

	/*process body*/
	for(; cnt<sizeof(data); cnt++)
	{
		data[cnt] = atoi(p);
		if((p=strchr(p, '.'))==NULL&&cnt!=3)
			return 0;
		else
			p++;
	}
	
	if(LITTLEENDIAN==GetCurrentEndianType())
		return ((data[0]<<24)+(data[1]<<16)+(data[2]<<8)+data[3]);
	else
		return ((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0]);
}
#else //ʹ��NDK�ӿ�ʵ��
#if !K21_ENABLE
ulong INET_ADDR(char *a)
{
	/*private & local definition*/
	ulong ret = 0;

	/*process body*/
	return (NDK_PppAddrChange(a, &ret)==NDK_OK)?ret:0;
}
#else
ulong INET_ADDR(char *a){return 0;}
#endif
#endif

void INET_NTOA(ulong n, char *a)
{
	/*private & local definition*/

	/*process body*/
	if(LITTLEENDIAN==GetCurrentEndianType())
		sprintf(a, "%u.%u.%u.%u", *((uchar *)&n+3), *((uchar *)&n+2), *((uchar *)&n+1), *((uchar *)&n+0));
	else
		sprintf(a, "%u.%u.%u.%u", *((uchar *)&n+0), *((uchar *)&n+1), *((uchar *)&n+2), *((uchar *)&n+3));
}

unsigned short HTONS(unsigned short data)
{
	/*private & local definition*/
	
	/*process body*/
#if 0	//defined GP710
	if(netsetting.socktype==SOCK_STTCP||netsetting.socktype==SOCK_STUDP)
		return htons(data);
	else
		return data;
#else
	return data;
#endif
}

unsigned long HTONL(unsigned long data)
{
	/*private & local definition*/
	
	/*process body*/
#if 0
	return htonl(data);
#else
	return data;
#endif
}

void create_0203pack(char *buf, int tlen)
{
	/*private & local definition*/

	/*process body*/
	buf[0] = STX;//0x02;
	buf[1] = Data2BCD(tlen/100);
	buf[2] = Data2BCD(tlen%100);
	buf[tlen+3] = ETX;//0x03;
	buf[tlen+4] = MakeLRC((uchar *)buf+1, tlen+3);//��len��0x03��ʼ��LRC
}

#if SDLC_ENABLE||ASYN_ENABLE
//���int�ͷ���ֵ,�ɰ���2������
int mdm_detectCONNECT(void)
{
	/*private & local definition*/
	EM_MDMSTATUS sta = 0; 
	int times = 0, ret = 0;
	time_t diff = 0, oldtime = 0;

	/*process body*/
	oldtime = time(NULL);
	sleep(3);//��MDM״̬�ȶ�����ȡҲ����,������GP710�κ�ֱ��ȡ״̬���ܰβ���

	cls_printf("���MODEM״̬��,���Ժ�...");
	while(1)//do
	{
		if ((ret=NDK_MdmCheck(&sta))!=NDK_OK||sta<MDMSTATUS_NORETURN_AFTERPREDIAL||(diff=time(NULL)-oldtime)>MAXTIMEOUT)//NORETURN_AFTERPREDIAL:0
		{
			//����ret��ζ��Ҷ�һ��if(ret>=NORETURN_AFTERPREDIAL)
			//NDK_MdmHangup();//�ⲿ�������Լ���
			break;//return ret;
		}
		show_stopwatch(ENABLE, diff);
		if(sta==MDMSTATUS_CONNECT_AFTERPREDIAL)//CONNECT_AFTERPREDIAL:2
		{
			if(++times==TIME4KEEPSTATE)
				break;
			else
				NDK_SysDelay(10);//�ȶ�һ��ʱ���ٿ���
		}
	}//while(ret!=CONNECT_AFTERPREDIAL);//CONNECT_AFTERPREDIAL:2
	
	show_stopwatch(DISABLE, 0);
	return (ret==NDK_OK)?sta:ret;//Ԥ�ڷ���CONNECT_AFTERPREDIAL //�˴����˷���SUCC��,��NORETURN_AFTERPREDIAL��ֵ
}

static int mdm_dial_nocheck(char *phonenum)
{
	/*private & local definition*/
	int ret = 0;
	LinkType type = getlinktype();

	/*process body*/
	if(type==SYNC||type==ASYN)
		cls_printf("%s��è��%s��...\n", (type==SYNC)?"ͬ":"��", phonenum);//��װ����������ʾ��Ϣ��������ԭʼAPI
	else
		return FAIL;
	
	if((ret=NDK_MdmDial(phonenum))!=NDK_OK)//�ڲ�Ŀǰֻʹ��TONE��ʽ,����ʹ��PLUS��ʽ��
	{
		cls_show_msg1(10, "%s, line %d:NDK_MdmDialʧ��!(%d)", __FILE__, __LINE__, ret);
		//NDK_MdmHangup();//�ⲿ�������Լ���
		return ret;//FAIL;
	}
	
	return SUCC;
}

int mdm_dial(char *phonenum)
{
	/*private & local definition*/
	int ret = 0;
	LinkType type = getlinktype();

	/*process body*/
	if(type==SYNC||type==ASYN)
		cls_printf("%s��è��%s��...\n", (type==SYNC)?"ͬ":"��", phonenum);//��װ����������ʾ��Ϣ��������ԭʼAPI
	else
		return FAIL;
	
	if((ret=NDK_MdmDial(phonenum))!=NDK_OK)//�ڲ�Ŀǰֻʹ��TONE��ʽ,����ʹ��PLUS��ʽ��
	{
		cls_show_msg1(10, "%s, line %d:NDK_MdmDialʧ��!(%d)", __FILE__, __LINE__, ret);
		//NDK_MdmHangup();//�ⲿ�������Լ���
		return FAIL;
	}

	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		cls_show_msg1(10, "%s, line %d:��⵽�쳣״̬(%d)", __FILE__, __LINE__, ret);
		return FAIL;
	}
	
	return SUCC;
}

int mdm_hangup(void)
{
	/*private & local definition*/
	int ret = 0;
	LinkType type = getlinktype();

	/*process body*/
	if(type==SYNC||type==ASYN)
		cls_printf("%s��è�һ���...\n", (type==SYNC)?"ͬ":"��");//��װ����������ʾ��Ϣ��������ԭʼAPI
	else
		return FAIL;
	
	ret = NDK_MdmHangup();
	if(type==SYNC)
		sleep(SLEEP_SDLC_HANGUP);//�������ʱ������ΪPOS�������Ҫ,���ǲ����豸(ͬ��������)�Ҷ���,�Ǹ�����ʱ��

	return ret;//FAIL;
}
#else
int mdm_detectCONNECT(void){return MDMSTATUS_NOPREDIAL;}
//static int mdm_dial_nocheck(char *phonenum){return FAIL;}
int mdm_dial(char *phonenum){return FAIL;}
int mdm_hangup(void){return FAIL;}
#endif

#if SDLC_ENABLE
//�޸�SDLC����ǰ5�ֽڵİ�ͷ
void update_SDLC_packet(char *buf, int len)//����������,������չ,��ʽ��Ҳupdate_ASYN_packetͳһ
{
	/*private & local definition*/
	
	/*process body*/
	if(len<SDLCPCKTHEADERLEN)
	{
		cls_show_msg1(10, "%s, line %d:��������С��%d!", __FILE__, __LINE__, SDLCPCKTHEADERLEN);
		return;
	}
	
	memcpy(buf, SDLCPCKTHEADER, SDLCPCKTHEADERLEN);//TPDUͷ
	return;
}

int __LinkUp_SDLC(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	cls_printf("����ͬ���κ�������...\n");

	//��ʼ��
#if defined SP50||defined SP80
	if(sdlc_init_flag)
	{
		if((ret=NDK_MdmSdlcSingleForceInit(MDMPatchType, g_sdlc_bps))!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:ͬ��MODEM��ʼ��ʧ��!", __FILE__, __LINE__);
			return ret;
		}
	}
	else
#endif
	{
		if((ret=NDK_MdmSdlcInit(MDMPatchType))!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:ͬ��MODEM��ʼ��ʧ��!", __FILE__, __LINE__);
			return ret;
		}
	}
	NDK_MdmClrbuf();
	
	//����
	if((ret=mdm_dial(MDMDialStr))!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:����ͬ���κ�(ATDT%s)����ʧ��!", __FILE__, __LINE__, MDMDialStr);
		mdm_hangup();
		return ret;//FAIL;
	}
	NDK_MdmClrbuf();//sdlc_clrportbuf_all();

	linkstatus = LINKUP;
	return SUCC;
}

int __LinkDown_SDLC(void)
{
	/*private & local definition*/

	/*process body*/
	linkstatus = LINKDOWN;//���۳ɹ����,��·�ص�����DOWN̬
	cls_printf("�Ͽ�ͬ���κ�������...\n");
	return mdm_hangup();
}

static int __NetUp_SDLC(void)
{
	/*private & local definition*/
	
	/*process body*/
	//cls_printf("����ͬ��������...\n");
	cls_show_msg1(10, "%s, line %d:����ͬ������ʧ��!", __FILE__, __LINE__);
	return FAIL;	
}

static int __NetDown_SDLC(void)
{
	/*private & local definition*/
	
	/*process body*/
	//cls_printf("�Ͽ�ͬ��������...\n");
	cls_show_msg1(10, "%s, line %d:�Ͽ�ͬ������ʧ��!", __FILE__, __LINE__);
	return FAIL;
}
#else
void update_SDLC_packet(char *buf, int len){}
int __LinkUp_SDLC(void){return FAIL;}
int __LinkDown_SDLC(void){return FAIL;}
static int __NetUp_SDLC(void){return FAIL;}
static int __NetDown_SDLC(void){return FAIL;}
#endif

#if ASYN_ENABLE
#if 1//!( /*defined NL8510GP||*/defined GP730)730��8510GPͬ��
//ƴ��̽����ջ���,ֱ�����ٶ���������(�������ȶ�)
//�쳣ʱ�ŷ���FAIL,���������,����>=0
int getmodemreadlenN(void)
{
	/*private & local definition*/
	int ret = 0, oldlen = 0;
	uint rdlen = 0;
	time_t oldtime = 0, interval = 2;//interval = (mdmstatus==PREDIALED?40:2);//���ν��ռ������ʱ��;

	/*process body*/
	oldtime = time(NULL);
	while (1)
	{
		if((ret=NDK_MdmGetreadlen(&rdlen))!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:���ջ������쳣(%d)!", __FILE__, __LINE__, ret);
			return ret;//FAIL;
		}
		
		if (oldlen<rdlen)//�ж���������,����oldlen,�����¼�ʱ
		{
			oldlen = rdlen;
			oldtime = time(NULL);
		}
		else if(oldlen==rdlen)
		{
			//�����ͺ�POS��Ҫ�Ĳ���ʱ��E80:35 nl8200:30 nl8510:30
			if (time(NULL)-oldtime>interval)//XXs�ڶ�����һ��������,��Ϊ�ȶ���,����read��
				return rdlen;
		}
		else//oldlen>ret,??���ջ�����������ˣ�
		{
			cls_show_msg1(10, "%s, line %d:���ջ������쳣!", __FILE__, __LINE__);
			return FAIL;
		}
		
		if (lib_kbhit()==ESC)//����̽����̱��ж�
			return FAIL;
	}
}
#else
int getmodemreadlenN(void){return ASYNPCKTMAXLEN;}
#endif

//��ʽ(���̨���): 0x02(header, 1byte)+len(2bytes, BCD)+TPDU header(5bytes)+data(������)+0x03(tail, 1byte)+LRC(1byte)
//��̨��%x��ʽ��ʾ
void update_ASYN_packet(char *buf, int len)
{
	/*private & local definition*/
	
	/*process body*/
	if(len<ASYNPCKTHEADERLEN)
	{
		cls_show_msg1(10, "%s, line %d:��������С��%d!", __FILE__, __LINE__, ASYNPCKTHEADERLEN);
		return;
	}
	
	create_0203pack(buf, len-ASYNPCKTHEADERLEN);
	return;
}

int ASYNDial4PPP(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	cls_printf("�����첽�κ�������...\n");

	//��ʼ��
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:�첽MODEM��ʼ��ʧ��!", __FILE__, __LINE__);
		return ret;//FAIL;
	}
	NDK_MdmClrbuf();

	//����
	//ע��˲�������ͨ��·�Ĳ��ŷ�PPP����,PPP���ź���Ե���������ͨ��·�Ĳ��Ų���
	if((ret=mdm_dial_nocheck(MDMDialStr))!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:�����첽�κ�(ATDT%s)����ʧ��!", __FILE__, __LINE__, MDMDialStr);
		mdm_hangup();
		return ret;//FAIL;
	}
	//NDK_MdmClrbuf();//Asyn_clrportbuf_all();

	return SUCC;
}

int __LinkUp_ASYN(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
#if 1
	cls_printf("�����첽�κ�������...\n");

	//��ʼ��
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:�첽MODEM��ʼ��ʧ��!(ret=%d)", __FILE__, __LINE__, ret);
		return ret;//FAIL;
	}
	NDK_MdmClrbuf();

	//����
	//ע��˲�������ͨ��·�Ĳ��ŷ�PPP����,PPP���ź���Ե���������ͨ��·�Ĳ��Ų���
	if((ret=mdm_dial(MDMDialStr))!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:�����첽�κ�(ATDT%s)����ʧ��!", __FILE__, __LINE__, MDMDialStr);
		mdm_hangup();
		return ret;//FAIL;
	}
	NDK_MdmClrbuf();//Asyn_clrportbuf_all();
#else
	if((ret=ASYNDial())!=SUCC)
		return ret;
#endif

	linkstatus = LINKUP;
	return SUCC;
}

int __LinkDown_ASYN(void)
{
	/*private & local definition*/

	/*process body*/
	linkstatus = LINKDOWN;//���۳ɹ����,��·�ص�����DOWN̬
	cls_printf("�Ͽ��첽�κ�������...\n");
	return mdm_hangup();
}

/////////////////////////////˵��:���¹����첽PPP�ĺ�����ֲ��NL8510GP���Լ�(����yanm)///////////////////////////////
//�ַ���ɨ�躯��
//���ش�����λ��
static int findstr(const unsigned char * pstr, unsigned char * pbuf, int *buflen)
{
	unsigned int i,tmplen;
	char *pfirst3str,*pstris;
	char tmpstr[4];
	
	//����ǰ�����ֽ�
	memcpy(tmpstr,pstr,3);
	tmpstr[3]=0;
	pfirst3str=strstr((char *)pbuf,tmpstr);
	//�˶�������
	if(pfirst3str!=NULL)
	{
		pstris=strstr((char *)pbuf,(char *)pstr);
		if(pstris!=NULL)
		{
			//�ҵ���,���ش�����ʾλ��
//			return pfirst3str-pbuf;
			if((unsigned int)pstris==(unsigned int)pbuf)
				return TRUE;
			tmplen=strlen(pstris);
			for(i=0;i<tmplen;i++)
			{
				pbuf[i]=pstris[i];
			}
			pbuf[tmplen]=0;
			*buflen=tmplen;
			return TRUE;
		}
		else
		{
			//�е�һ���ֽ���Ի�����е���????
			if(strlen((char *)pfirst3str)<strlen((char *)pstr))
				return FALSE;
			tmplen=strlen(pfirst3str);
			for(i=0;i<tmplen;i++)
			{
				pbuf[i]=pfirst3str[i];
			}
			pbuf[tmplen]=0;
			*buflen=tmplen;
//			return pfirst3str-pbuf;
		}
	}
	return FALSE;
}

#define STEPLEN	1
int WM_PPP_16300_DIAL(void)
{
	pppdial_state WM_PPP_STATESTR_16300[WM_PPP_STATE_SUM_16300+1]={
	{WM_PPP_CONNECT,"CONNECT"},
//	{WM_PPP_PROTOCOL,"PROTOCOL"},
	{WM_PPP_WELCOME,"welcome!!"},
	{WM_PPP_USERNAME,"username:"},
	{WM_PPP_PASSWORD,"password:"},
	{WM_PPP_MODE,"PPP mode.\r\n"},
	{WM_PPP_ENTHERNET,"(Ethernet0)\r\n"},
	{WM_PPP_SYSTEM,"system.\r\n"},
	{WM_PPP_IPMTU,"bytes\r\n"}
	};

	pppdial_state WM_PPP_ERRSTR_16300[WM_PPP_STATE_SUM_16300+1]={
	{WM_PPP_ERROR_NO_DIALTONE,"NO DIALTONE"},
	{WM_PPP_ERROR_NO_CARRIER,"NO CARRIER"},
	{WM_PPP_ERROR_BUSY,"BUSY"},
	{WM_PPP_ERROR_UNUMBER,"NUMBER"},
	{WM_PPP_ERROR_PRONONE,"PROTOCOL:NONE"}
	};

	unsigned char databuffer[1024];
	unsigned int datalen,olddatalen;
	int ret;
	uint len;
	unsigned int flag=0,j,errcnt,normalcnt;
	pppdialstatetype WM_PPP_dialstate;

	//��ʼ��
	WM_PPP_dialstate.state_sum=WM_PPP_STATE_SUM_16300;
	WM_PPP_dialstate.state=WM_PPP_NULL_16300;
	WM_PPP_dialstate.error_start=WM_PPP_ERROR_STARTSTATE;
	WM_PPP_dialstate.error_stop=WM_PPP_ERROR_STOPSTATE;
	WM_PPP_dialstate.error_sum=WM_PPP_ERROR_SUM;
	WM_PPP_dialstate.pstate_str=WM_PPP_STATESTR_16300;
	WM_PPP_dialstate.perror_str=WM_PPP_ERRSTR_16300;
	//ѭ������
	datalen=olddatalen=0;
	normalcnt=errcnt=0;
	while(!flag)
	{
	//��ȡ����������
		if(datalen<(sizeof(databuffer)-STEPLEN-1))
		{
			olddatalen=datalen;

			len = STEPLEN;
			if((ret=NDK_MdmRead((char *)(databuffer+datalen),&len,(WM_PPP_CONNECT==(WM_PPP_dialstate.pstate_str)[normalcnt].state)?30:3))!=NDK_OK)
			{
				cls_show_msg1(10, "%s, line %d:��ȡ����ʧ��(%d)!", __FILE__, __LINE__, ret);
				return FAIL;
			}
			if(len!=STEPLEN)
			{
				cls_printf("���ݶ�ȡ�Ѿ�����");
				flag=1;	//������ʣ�����ݺ�Ӧ���ж�ѭ��
			}
			datalen+=len;
		}
		else
		{
			cls_show_msg1(10, "%s, line %d:��ʱ����������!", __FILE__, __LINE__);
			return FAIL;			
		}
		//ɨ������������ַ�ASCII�������򱨴���ֻɨ��ǰһ�ε���Ч����
		for(j=olddatalen;j<datalen;j++)
		{
			if(databuffer[j]&0x80)
			{
				cls_show_msg1(10, "%s, line %d:���ַ�ASCII��!", __FILE__, __LINE__);
				flag=1;
				break;
			}
		}
		datalen=j;
		databuffer[datalen]=0;
		//�������ݣ��ж�״̬
		//�������ݷ���
		ret=findstr((WM_PPP_dialstate.pstate_str)[normalcnt].statestr,databuffer,(int *)&datalen);
		if(TRUE==ret)
		{
			flag=0;
#if 0
			cls_printf("match(%d):[%d]\n[%s]\n[%s]\n",normalcnt
				,(WM_PPP_dialstate.pstate_str)[normalcnt].state
				,(WM_PPP_dialstate.pstate_str)[normalcnt].statestr,databuffer);
#endif
			switch((WM_PPP_dialstate.pstate_str)[normalcnt].state)
			{
			case WM_PPP_USERNAME:
				NDK_MdmWrite("16300\r", strlen("16300\r"));//modemportwrite("16300\r",6);
//				modemportwrite("yp0039@vpdn.zp\r",15);
				break;
			case WM_PPP_PASSWORD:
				NDK_MdmWrite("16300\r", strlen("16300\r"));//modemportwrite("16300\r",6);
//				modemportwrite("5gf34g\r",7);
				break;
			case WM_PPP_IPMTU:
				flag=1;
				break;
			default:
				break;
			}
			normalcnt++;
		}
		//�������ݷ���
		if((normalcnt>=WM_PPP_dialstate.error_start)&&(normalcnt<=WM_PPP_dialstate.error_stop))
		{
			for(errcnt=0;errcnt<WM_PPP_dialstate.error_sum;errcnt++)
			{
				ret=findstr((WM_PPP_dialstate.perror_str)[errcnt].statestr,databuffer,(int *)&datalen);
				if(TRUE==ret)
				{
					flag=1;
#if 0
					cls_printf("ERROR match:\n[%s]\n",(WM_PPP_dialstate.perror_str)[errcnt].statestr);//,databuffer
#endif
					if(WM_PPP_ERROR_PRONONE==WM_PPP_dialstate.perror_str[errcnt].state)
					{
#if 0
						cls_printf("Э�̴����½���\n���Ժ�...");
						msdelay(8000);
#else
						cls_show_msg1(10, "%s, line %d:Э�̴���(%d)!", __FILE__, __LINE__, WM_PPP_dialstate.perror_str[errcnt].state);
#endif
					}
					return -WM_PPP_dialstate.perror_str[errcnt].state;
				}
			}
		}

	//�����˳�
	}

	return	WM_PPP_dialstate.state;
}
#else
int getmodemreadlenN(void){return FAIL;}
void update_ASYN_packet(char *buf, int len){}
int ASYNDial4PPP(void){return FAIL;}
int __LinkUp_ASYN(void){return FAIL;}
int __LinkDown_ASYN(void){return FAIL;}
//static int findstr(const unsigned char * pstr, unsigned char * pbuf, int *buflen){return FAIL;}
int WM_PPP_16300_DIAL(void){return FAIL;}
#endif

#if WLM_ENABLE && PPP_ENABLE
int __LinkUp_WLM(void)
{
	/*private & local definition*/
	int ret = 0;
	EM_WLM_STATUS emStatus = -1;
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:	
		//����GPRS,CDMA����
		cls_printf("����%s������...\n", (type==GPRS)?"GPRS":((type==CDMA)?"CDMA":"TD"));
		if ((ret = NDK_WlInit(2000,NULL,&emStatus))!=NDK_OK||emStatus!=WLM_STATUS_OK)
		{
			cls_show_msg1(10,"%s, line %d:���߳�ʼ��ʧ��(%d,%d)", __FILE__, __LINE__, ret, emStatus);
			return FAIL;
		}
		break;
	default:
		return FAIL;
		break;
	}

	linkstatus = LINKUP;
	return SUCC;
}

int __LinkDown_WLM(void)
{
	/*private & local definition*/
	int err = 0, ret = 0;
	LinkType type = getlinktype();

	/*process body*/
	linkstatus = LINKDOWN;//���۳ɹ����,��·�ص�����DOWN̬
	
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:	
		cls_printf("�Ͽ�%s������...\n", (type==GPRS)?"GPRS":((type==CDMA)?"CDMA":"TD"));

#if 0
		printf("��ȴ�40��...");
		sleep(40);//����30��,���ƶ���̨�Զ��ͷ���·//�����Դ�,����
#else
		if((ret=WaitPPPState(PPP_STATUS_DISCONNECT, &err))!=SUCC)
		{
			cls_show_msg1(10, "%s, line %d:�Ͽ�%sʧ��!(%d)", __FILE__, __LINE__, (type==GPRS)?"GPRS":((type==CDMA)?"CDMA":"TD"), err);
#if  !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
			NDK_PppHangup(1);//NetPPPHangup(1);
#else
			NDK_PppHangup(0); //�������Ҷ�
#endif
			//linkstatus = LINKDOWN;
			return ret;//FAIL;
		}
#endif

		break;
	default:
		return FAIL;
		break;
	}

	//linkstatus = LINKDOWN;
	return SUCC;
}
#else
int __LinkUp_WLM(void){return FAIL;}
int __LinkDown_WLM(void){return FAIL;}
#endif

#if ETH_ENABLE
int __LinkUp_ETH(void)
{
	/*private & local definition*/

	/*process body*/
	cls_printf("������̫��������...\n");

	linkstatus = LINKUP;
	return SUCC;
}

int __LinkDown_ETH(void)
{
	/*private & local definition*/

	/*process body*/
	cls_printf("�Ͽ���̫��������...\n");
	NDK_EthDisConnect();//20151015 yanggf���������ӵ�
	
	linkstatus = LINKDOWN;//��̫�������رղ���LINKDOWN״̬
	return SUCC;
}

int __NetUp_ETH(void)
{
	/*private & local definition*/
	int ret = 0;
	char LocalIp[16] = {0}, LocalMask[16] = {0}, LocalGateway[16] = {0}, DNS[16*3+2/*16*/] = {0}; //�ĵ�˵��һ����ȡ������DNS,֮���Էֺŷ�';'��������Ҫ���󻺳��� 20180322

	/*process body*/
	cls_printf("������̫��������...\n");

	if(netsetting.DHCPenable==DISABLE)
	{
		if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1]))!=NDK_OK)
		{                                                                                            
			cls_show_msg1(10, "%s, line %d:���þ�̬��ַʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;//FAIL;
		}
	}

	if(netsetting.DHCPenable==ENABLE)
	{
		if((ret=NDK_NetDHCP())!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:����DHCPʧ��(ret=%d)", __FILE__, __LINE__, ret);
			return ret;//FAIL;	
		}
		
		//��һ�����붯̬IP(����һ��ȡ����,��ȡ����)    
		if((ret=lib_GetLocalAddr(LocalIp, LocalMask, LocalGateway, DNS))!=SUCC)//if((LocalIp=GetLocalAddr())==0)
		{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
			cls_show_msg1(10, "%s, line %d:��ȡ��̬IPʧ��", __FILE__, __LINE__);
			return ret;//FAIL;
		}

		//����ȡ����ַ��Ϣ��תΪul��ֵͬ����ȫ�ֱ�����(layer��ص�ȫ�ֱ���һ��Ӧ������ʱ������ͬ��,��xxxUp����ֻ��Ϊ��ֵ;��̬��ȡ�Ǹ�����,���ֱ�����Ҫ��Ϊ��ֵ���и���)
		strcpy(netsetting.LocalIP.Str, LocalIp);
		netsetting.LocalIP.value.data = INET_ADDR(LocalIp);
		strcpy(netsetting.LocalMask.Str, LocalMask);
		netsetting.LocalMask.value.data = INET_ADDR(LocalMask);
		strcpy(netsetting.LocalGateway.Str, LocalGateway);
		netsetting.LocalGateway.value.data = INET_ADDR(LocalGateway);
		//DNS��?
	}

	netstatus = NETUP;
	return SUCC;
}

int __NetDown_ETH(void)
{
	/*private & local definition*/
	int ret = 0;
	char LocalIp[16] = "1.0.0.1", LocalMask[16] = "255.0.0.0", LocalGateway[16] = "1.0.0.254", DNS[16] = "0.0.0.0";

	/*process body*/
	cls_printf("�Ͽ���̫��������...\n");

	//�ñ���IPΪ��Чֵ,ͬʱ��EthClose�ж�������������
	if ((ret=NDK_EthSetAddress(LocalIp, LocalMask, LocalGateway, DNS))!=NDK_OK)
	{                                                                                            
		cls_show_msg1(10, "%s, line %d:���þ�̬��ַʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;//FAIL;
	}

#if 0 //�Ͽ�ֻ��IPΪ��Чֵ,����ͬ����ȫ��������,��������һ��NetUp�����ô������������
	strcpy(netsetting.LocalIP.Str, LocalIp);
	netsetting.LocalIP.value.data = INET_ADDR(LocalIp);
	strcpy(netsetting.LocalMask.Str, LocalMask);
	netsetting.LocalMask.value.data = INET_ADDR(LocalMask);
	strcpy(netsetting.LocalGateway.Str, LocalGateway);
	netsetting.LocalGateway.value.data = INET_ADDR(LocalGateway);
#endif

	if((ret=__LinkDown_ETH())!=SUCC)//if(__LinkDown(type)!=SUCC)
	{
		netstatus = NETDOWN;
		return ret;//FAIL;
	}

	netstatus = NETDOWN;
	return SUCC;
}
#else
int __LinkUp_ETH(void){return FAIL;}
int __LinkDown_ETH(void){return FAIL;}
int __NetUp_ETH(void){return FAIL;}
int __NetDown_ETH(void){return FAIL;}
#endif

#if WLAN_ENABLE
int __LinkUp_WLAN(void)
{
	/*private & local definition*/
	int ret = 0;
#if NEW_WIFI_ENABLE
	ST_WIFI_STATUS status;
#else
	ST_WIFI_8021x_PARAM init2; //= {0}ȥ������
	ST_WIFI_PARAM init = {0};
	EM_WPA_CONSTATE state = 0;
	time_t diff = 0, oldtime = 0;
#endif

	/*process body*/
#if NEW_WIFI_ENABLE
	/*if((ret=NDK_WifiStatus(&status))!=NDK_OK)
	{
		cls_show_msg1(5, "%s, line %d:��ȡ%s״̬ʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
		return FAIL;
	}*/
	NDK_WifiStatus(&status);
	if(status.status == WIFI_WLAN_STATUS_SUCC)
	{
		linkstatus = LINKUP;
		return SUCC;
	}
	cls_printf("����AP(%s)��...\n", WlanEssid);
	//����start����
	if((ret=NDK_WifiStart())!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:��������%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
		NDK_WifiStop();
		return FAIL;
	}
	//�ж����ӳɹ�״̬
	if( ((ret=lib_getwifistatus(&status))!=NDK_OK) || (status.status!=WIFI_WLAN_STATUS_SUCC) || strcmp(status.ssid, WlanEssid) )
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����%s��ʱʧ��!(ret=%d,status=%d,ssid=%s)", __FILE__, __LINE__, WlanEssid, ret, status.status, status.ssid);
		NDK_WifiStop();
		return FAIL;
	}
#else
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:WIFI�豸��ʼ��ʧ��(%d)", __FILE__, __LINE__, ret);
		return FAIL;
	}
	
#if 0	
	memset(&init, 0, sizeof(init));
	strcpy(init.psEthIp, netsetting.LocalIP.Str);
	strcpy(init.psEthNetmask, netsetting.LocalMask.Str);
	strcpy(init.psEthGateway, netsetting.LocalGateway.Str);
	strcpy(init.psEthDnsPrimary, netsetting.DNSHost[0]);
	strcpy(init.psEthDnsSecondary, netsetting.DNSHost[1]);
	init.emSecMode = linksetting.WLANEncMode;
	init.ucIfDHCP = netsetting.DHCPenable;
	init.emKeyType =  linksetting.WLANKeyType;
#endif 

	init.ucIfDHCP = netsetting.DHCPenable;
	init.psEthIp = netsetting.LocalIP.Str;
	init.psEthNetmask = netsetting.LocalMask.Str;
	init.psEthGateway = netsetting.LocalGateway.Str;
	init.psEthDnsPrimary = netsetting.DNSHost[0];
	init.psEthDnsSecondary = netsetting.DNSHost[1];
	init.pszKey = linksetting.WLANPwd;
	init.emKeyType = linksetting.WLANKeyType;
	init.emSecMode = linksetting.WLANEncMode;

	cls_printf("����AP(%s)��...\n", WlanEssid);
	if(init.emSecMode==WIFI_NET_SEC_WPA_EAP)
	{
		init2.comm_param = init;
		init2.pszIdentity = linksetting.WLAN8021XIdentity;
		init2.emEap = linksetting.WLAN8021XEapMode;
		init2.pszca_cert = NULL;
	
		if((ret=NDK_WiFiConnect_8021x(WlanEssid, &init2))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
			NDK_WiFiDisconnect();
			NDK_WiFiShutdown();
			return FAIL;
		}
	}
	else
	{
		if((ret=NDK_WiFiConnect(WlanEssid, &init))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
			NDK_WiFiDisconnect();
			NDK_WiFiShutdown();
			return FAIL;
		}

	}
	
    oldtime = time(NULL);
	while(1)//do
	{
		if ((ret=NDK_WiFiConnectState(&state))!=NDK_OK||(diff=time(NULL)-oldtime)>30)//NORETURN_AFTERPREDIAL:0
		{
            cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����%sʧ��!(%d,%d)", __FILE__, __LINE__, WlanEssid, ret, state);
			NDK_WiFiDisconnect();
            NDK_WiFiShutdown();
		    return FAIL;
        }
		show_stopwatch(1, diff);
		if(state==WIFI_WPA_CONSTATE_CONTEXT)
			break;
		else
			NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
	}

	if((ret=NDK_WiFiIsConnected())!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
		NDK_WiFiDisconnect();
		NDK_WiFiShutdown();
		return FAIL;
	}
#endif

	linkstatus = LINKUP;
	return SUCC;
}

int __LinkDown_WLAN(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
#if NEW_WIFI_ENABLE
	//�Ҷϲ���
	if(g_wifistopflag == 1)
	{
		if((ret=NDK_WifiStop())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:�Ͽ�%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
			return FAIL;
		}
	}
	else
		;//�ر�
#else
	cls_printf("�Ͽ�AP(%s)������...\n", WlanEssid);	
	if((ret=NDK_WiFiDisconnect())!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:�Ͽ�%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
		NDK_WiFiShutdown();
		return FAIL;
	}
	if((ret=NDK_WiFiShutdown())!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:�Ͽ�%sʧ��!(%d)", __FILE__, __LINE__, WlanEssid, ret);
		NDK_WiFiShutdown();
		return FAIL;
	}
#endif
	linkstatus = LINKDOWN;//��̫�������رղ���LINKDOWN״̬
	return SUCC;
}

int __NetUp_WLAN(void)
{
	/*private & local definition*/
	int ret;
	char LocalIp[16],LocalMask[16], LocalGateway[16];
	
	/*process body*/
	cls_printf("����WLAN������...\n");
	
	if(netsetting.DHCPenable==ENABLE)
	{
		//��һ�����붯̬IP(����һ��ȡ����,��ȡ����)
		if((ret=lib_GetLocalAddr(LocalIp, LocalMask, LocalGateway,NULL))!=SUCC)//if((LocalIp=GetLocalAddr())==0)
		{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:��ȡ��̬IPʧ��(%d)", __FILE__, __LINE__, ret);
			return FAIL;
		}
		//����ȡ����ַ��Ϣ��תΪul��ֵͬ����ȫ�ֱ�����(layer��ص�ȫ�ֱ���һ��Ӧ������ʱ������ͬ��,��xxxUp����ֻ��Ϊ��ֵ;��̬��ȡ�Ǹ�����,���ֱ�����Ҫ��Ϊ��ֵ���и���)
		strcpy(netsetting.LocalIP.Str, LocalIp);
		netsetting.LocalIP.value.data = INET_ADDR(LocalIp);
		strcpy(netsetting.LocalMask.Str, LocalMask);
		netsetting.LocalMask.value.data = INET_ADDR(LocalMask);
		strcpy(netsetting.LocalGateway.Str, LocalGateway);
		netsetting.LocalGateway.value.data = INET_ADDR(LocalGateway);
	}
	
	//���Ǿ�̬���õ�,��IP��������wlan_configʱ�Ѵ���ײ�
	
	netstatus = NETUP;
	return SUCC;
}

int __NetDown_WLAN(void)
{
	/*private & local definition*/
	int ret = SUCC;

	/*process body*/
	cls_printf("�Ͽ�WLAN������...\n");
	if(__LinkDown_WLAN()!=SUCC)//if(__LinkDown(type)!=SUCC)
		ret = FAIL;

	netstatus = NETDOWN;
	return ret;
}
#else
int __LinkUp_WLAN(void){return FAIL;}
int __LinkDown_WLAN(void){return FAIL;}
int __NetUp_WLAN(void){return FAIL;}
int __NetDown_WLAN(void){return FAIL;}
#endif

#if PPP_ENABLE||ETH_ENABLE||WLAN_ENABLE
//ע��:��������Ҫ���ڶ�̬��ȡ����һϵ�е�ַ(��,(���߻�����)��̫��DHCP;����)
int lib_GetLocalAddr(char *ipAddr, char *subnet, char *gateway, char *dns)//ulong GetLocalAddr(void);
{
	/*private & local definition*/
	int ret = 0;
	ulong LocalIp = 0/*, HostIp = 0*/;
	time_t diff = 0, oldtime = time(NULL);

	/*process body*/
#if 1
	LinkType type = getlinktype();
	switch(type)
	{
		case ETH:
			//return NDK_EthGetNetAddr(ipAddr, subnet, gateway, dns);
			return NDK_NetGetAddr(COMM_TYPE_ETH, ipAddr, subnet, gateway, dns);
			break;
		case WLAN:
			return NDK_NetGetAddr(COMM_TYPE_WIFI, ipAddr, subnet, gateway, dns);
			break;
		case GPRS:
		case CDMA:
		case TD:
		case ASYN:
			{
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
				while((ret=NDK_PppGetAddr(&LocalIp, NULL/*&HostIp*/))!=NDK_OK||LocalIp==0)//δȡ�����ص�ַ(��δ����ɹ�)  
				{
					if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
						return FAIL;
					show_stopwatch(ENABLE, diff);
				}
				show_stopwatch(DISABLE, 0);
				INET_NTOA(HTONL(LocalIp), ipAddr);
#elif defined ME50NX||defined ME68
				;		//ME50NX��֧��NDK_PppGetNetAddr,ʵ�ʵײ�᷵��-18,�ʲ�����	20171227	
#else
				while((ret=NDK_PppGetNetAddr(ipAddr))!=NDK_OK)
				{
					if((diff=time(NULL)-oldtime)>MAXTIMEOUT)
						return FAIL;
					show_stopwatch(ENABLE, diff);
				}
#endif
				
				if(subnet!=NULL)
					strcpy(subnet, "0.0.0.0");
				if(gateway!=NULL)
					strcpy(gateway, "0.0.0.0");
				if(dns!=NULL)
					strcpy(dns, "0.0.0.0");
				
				return SUCC;
			}
			break;
		//case BT:
			//return NDK_NetGetAddr(COMM_TYPE_BT, ipAddr, subnet, gateway, dns);
			//break;
		default:
			cls_show_msg1(10, "%s, line %d:����δ֪����(%d)������ʧ��!", __FILE__, __LINE__, type);
			return FAIL;
			break;
	}
#else	
	if((getlinktype()==ETH)||(getlinktype()==WLAN))
		return NDK_EthGetNetAddr(ipAddr, subnet, gateway, dns);//FAIL;
	else
	{
		while((ret=NDK_PppGetAddr(&LocalIp, NULL/*&HostIp*/))!=NDK_OK||LocalIp==0)//δȡ�����ص�ַ(��δ����ɹ�)  
		{
			if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
				return FAIL;
			show_stopwatch(ENABLE, diff);
		}
		show_stopwatch(DISABLE, 0);
		INET_NTOA(HTONL(LocalIp), ipAddr);
		if(subnet!=NULL)
			strcpy(subnet, "0.0.0.0");
		if(gateway!=NULL)
			strcpy(gateway, "0.0.0.0");
		if(dns!=NULL)
			strcpy(dns, "0.0.0.0");
		
		return SUCC;
	}
#endif

}
#else
int lib_GetLocalAddr(char *ipAddr, char *subnet, char *gateway, char *dns){return FAIL;}
#endif

#if PPP_ENABLE
int WaitPPPState(EM_PPP_STATUS state, int *err)
{
	/*private & local definition*/
	int ret = 0, times = 0;
	EM_PPP_STATUS actual = PPP_STATUS_DISCONNECT;
	time_t diff = 0, oldtime = time(NULL);//��ʹ�ò������뺯��StartStopwatch()
	
	/*process body*/
	while(1)//((ret=GetPPPphase(ph))!=state)
	{
		if((ret=NDK_PppCheck(&actual, err))!=NDK_OK)
			return ret;//FAIL;
		if(actual==state)
		{
			if(++times==TIME4KEEPSTATE)
				break;
		}
		if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
			break;
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);//20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}

	show_stopwatch(DISABLE, 0);
	return (actual==state?SUCC:-3/*FAIL*/);//-3����������ĵ���ʧ�ܷ���-1
}

int __NetUp_PPP(void)
{
	/*private & local definition*/
	int ret = 0, err = 0;
	char LocalIp[16] = {0}, LocalMask[16] = {0}, LocalGateway[16] = {0}, DNS[16] = {0};
	//ST_PPP_CFG tmp_stPPPCfg;
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case ASYN:
		if((ret=WM_PPP_16300_DIAL())<0)
		{
			cls_show_msg1(10, "%s, line %d:�첽�δ�16300ʧ��!(%d)", __FILE__, __LINE__,  ret);
			__LinkDown();//NET�д�����,��PPP�Ļ�,Ӧֱ���ж�LINK��(��Ϊֻ�Ͽ�NET���ǲ�����)
			return ret;//FAIL;//�������__LinkDown�ɹ���,���ﷵ��FAIL����
		}
	case GPRS:
	case CDMA:
	case TD:
		//������ΪPPP�����׼������,����PPP����������
#if 0	//zhnagnw20130201�Ƶ�conf.c��
		memset((void *)&tmp_stPPPCfg, 0, sizeof(tmp_stPPPCfg));
		if(type==ASYN)
		{	
			strcpy(tmp_stPPPCfg.szDailNum, MDMDialStr);
			tmp_stPPPCfg.nDevType = 1;//0��ʾ ����,1Ϊ����
			//tmp_stPPPCfg.nPPPFlag = 1;
			//strcpy(tmp_stPPPCfg.szApn, "CMNET");
			tmp_stPPPCfg.ModemDial = ASYNDial;//�ײ�Ĭ�ϲ���æ������PPP���ź������ܣ������ҪӦ�ò���Ա�Լ����벦�ź�����
			tmp_stPPPCfg.PPPIntervalTimeOut = 600;

			//Ŀǰ��conf_MDM_lnk��conf_PPP�о��޶�����2���첽PPP����������,���ڴ˴�д��
			strcpy(PPPUSR, DEFAULT_ASYN_USR);
			strcpy(PPPPWD, DEFAULT_ASYN_PWD);
		}
		else
		{
			strcpy(tmp_stPPPCfg.szDailNum, "*99***1#");//���ɵײ�ָ��
			tmp_stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
			tmp_stPPPCfg.nPPPFlag = LCP_PPP_UNKEEPED;//������
			sprintf(tmp_stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(tmp_stPPPCfg.szApn, "\"%s\"", WLMAPN);
			tmp_stPPPCfg.ModemDial = NULL;
			//tmp_stPPPCfg.PPPIntervalTimeOut = 600;
		}
		
		if((ret=NDK_PppSetCfg(&tmp_stPPPCfg, sizeof(tmp_stPPPCfg)))!=NDK_OK)//if((ret=NetPPPCfg(&tmp_stPPPCfg, 0))<0)
		{
			cls_show_msg1(10, "%s, line %d:��·���������ʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;//FAIL;
		}
#endif
		
		if((ret=NDK_PppDial(PPPUSR, PPPPWD))!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:��PPPͨ��ʧ��!(%d)", __FILE__, __LINE__,  ret);
			__LinkDown();//NET�д�����,��PPP�Ļ�,Ӧֱ���ж�LINK��(��Ϊֻ�Ͽ�NET���ǲ�����)
			return ret;//FAIL;//�������__LinkDown�ɹ���,���ﷵ��FAIL����
		}

		cls_printf("����PPP������...\n");
		if((ret=WaitPPPState(PPP_STATUS_CONNECTED, &err))!=SUCC)
		{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
			cls_show_msg1(10, "%s, line %d:��ȡPHASE_NETWORKʧ��!(ret=%d,err=%d)", __FILE__, __LINE__, ret, err);
			NDK_PppHangup(1);//NetPPPHangup(1);
			__LinkDown();//NET�д�����,��PPP�Ļ�,ֱ���ж�LINK��(ֻ�Ͽ�NET���ǲ�����)
			return ret;//FAIL;
		}		

		//��һ�����붯̬IP(����һ��ȡ����,��ȡ����)    
		if((ret=lib_GetLocalAddr(LocalIp, LocalMask, LocalGateway, DNS))!=SUCC)//if((LocalIp=GetLocalAddr())==0)
		{//�Ȼ�ȡ����ʱ�Ĵ�����,�ٹر�
			cls_show_msg1(10, "%s, line %d:��ȡ��̬IPʧ��(%d)!", __FILE__, __LINE__, ret);
			NDK_PppHangup(1);//NetPPPHangup(1);
			__LinkDown();//NET�д�����,��PPP�Ļ�,ֱ���ж�LINK��(ֻ�Ͽ�NET���ǲ�����)
			return ret;//FAIL;
		}

		//����ȡ����ַ��Ϣ��תΪul��ֵͬ����ȫ�ֱ�����(layer��ص�ȫ�ֱ���һ��Ӧ������ʱ������ͬ��,��xxxUp����ֻ��Ϊ��ֵ;��̬��ȡ�Ǹ�����,���ֱ�����Ҫ��Ϊ��ֵ���и���)
		strcpy(netsetting.LocalIP.Str, LocalIp);
		netsetting.LocalIP.value.data = INET_ADDR(LocalIp);
		strcpy(netsetting.LocalMask.Str, LocalMask);
		netsetting.LocalMask.value.data = INET_ADDR(LocalMask);
		strcpy(netsetting.LocalGateway.Str, LocalGateway);
		netsetting.LocalGateway.value.data = INET_ADDR(LocalGateway);
		//DNS��?
		break;
	default:
		return FAIL;
		break;
	}

	netstatus = NETUP;
	return SUCC;	
}

int __NetDown_PPP(void)
{
	/*private & local definition*/
	int ret = 0;
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:
	case ASYN:
		cls_printf("�Ͽ�PPP������...\n");
#if  !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
		if((ret=NDK_PppHangup(1))!=NDK_OK)//if(NetPPPHangup(1)!=SUCC)//ʹ������ʽ�Ҷ�
#else
		if((ret=NDK_PppHangup(0))!=NDK_OK)
#endif			
		{
			cls_show_msg1(10, "%s, line %d:�Ͽ�PPP����ʧ��!", __FILE__, __LINE__);
			//��Ȼû�йسɹ�,�Ͳ�����״̬ΪDOWN��
			return ret;//FAIL;
		}
		
		//ע��:����ֱ�ӵ�LinkDown!(����ʵ������ȡ�������ǿ��Ե�)
		//ԭ��:1�����γ�A->B,B->A...ѭ������;2��NET���LINK��Ҳ������
		//PPPCLOSE��,�����Ѵ���LINKDOWN״̬,��Լ40s,�Զ˲Ż���ȫ�Ҷ�
		if((ret=__LinkDown())!=SUCC)
		{
			netstatus = NETDOWN;
			return ret;//FAIL;
		}
		break;
	default:
		return FAIL;
		break;
	}

	netstatus = NETDOWN;
	return SUCC;
}
#else
int WaitPPPState(EM_PPP_STATUS state, int *err){return FAIL;}
int __NetUp_PPP(void){return FAIL;}
int __NetDown_PPP(void){return FAIL;}
#endif

int LinkUp4AsynPPP(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	if(linkstatus==LINKUP)
		return SUCC;

	transstatus = TRANSDOWN;
	netstatus = NETDOWN;
	
	if((ret=ASYNDial4PPP())!=SUCC)
		return ret;	

	linkstatus = LINKUP;
	return SUCC;
}

//�ڲ�����,��LinkUp��������������ִ��UP
//����GetErrorCode��ʹ��:���ڷ�ֱ�ӵ���API����,��Ҫʹ��������λ����.���鿪����Աʹ��ret���������ִ���(����Խ�errcode��ret���ؼ���).
static int __LinkUp(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:
		return __LinkUp_WLM();
		break;
	case ASYN:
		return __LinkUp_ASYN();
		break;
	case SYNC:
		return __LinkUp_SDLC();
		break;
	case ETH://�������Լ�����װ��̫��
		return __LinkUp_ETH();
		break;
	case WLAN:
		return __LinkUp_WLAN();
		break;
	default:	//��ʶ��֧�ֵ���������,���ı�״̬����
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����δ֪����(%d)������ʧ��!", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}

	//�ŵ����Ӻ����и���linkstatus = LINKUP;
	cls_printf("������·��ɹ�!");
	return SUCC;
}

//ԭ���ϱ�����ֻ��֤��·���Զ�����,����֤�Զ�!
//����ϱ��������ڵ�һ��(PL)
//ͬ�ڵ�һ��Ļ���wireless_selfadapt,modemATCommond,WLM_get_res,InitAsynModem,AsynDial��
//���ý���:�ȶ�linktype��ֵ��,��LinkUp(linktype).Ҫ��LinkDown�ɶ�ʹ��
//ע��:Ҫʹ���첽èPPP,���������ȵ���LinkUp�ٵ���NetUp,��Ӧֱ�ӵ���NetUp(�����LinkUp4AsynPPP)!!!
int LinkUp(void)
{
	/*private & local definition*/

	/*process body*/
	if(linkstatus==LINKUP)
		return SUCC;

	transstatus = TRANSDOWN;
	netstatus = NETDOWN;
	return __LinkUp();
}

static int __LinkDown(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:
		return __LinkDown_WLM();
		break;
	case ASYN:
		return __LinkDown_ASYN();
		break;
	case SYNC:
		return __LinkDown_SDLC();
		break;
	case ETH:
		return __LinkDown_ETH();
		break;
	case WLAN:
		return __LinkDown_WLAN();
		break;
	default://��ʶ��֧�ֵ���������
		cls_show_msg1(10, "%s, line %d:�Ͽ�δ֪����(%d)������ʧ��!", __FILE__, __LINE__, type);
		return FAIL;//���ı�״̬����,��ǰreturn
		break;
	}

	//�ŵ����Ӻ����и���linkstatus = LINKDOWN;
	cls_printf("�Ͽ���·��ɹ�!");
	return SUCC;
}

//�ӱ�������ʵ�ֿ�,��������˵,��ʹ�����ñ�����,һ��ʱ���Ҳ��DOWN�ķ���(����ʱlinkstatus��������ָʾΪUP)
int LinkDown(void)
{
	/*private & local definition*/
	
	/*process body*/
	//���϶������ر�(����ж�)
	NetDown();//�ȹر�link���ϲ�(��net��),��������ȫ�ر�
	
	if(linkstatus==LINKDOWN)
		return SUCC;

	return __LinkDown();
}

static int __NetUp(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:
	case ASYN:
		return __NetUp_PPP();
		break;
	case SYNC://��֧��
		return __NetUp_SDLC();
		break;
	case ETH:
		return __NetUp_ETH();
		break;
	case WLAN:
		return __NetUp_WLAN();
		break;
	default:
		cls_show_msg1(10, "%s, line %d:����δ֪����(%d)������ʧ��!", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}

	//�ŵ����Ӻ����и���netstatus = NETUP;
	cls_printf("���������ɹ�!");
	return SUCC;
}

//����ϱ��������ڵ�2,3��(DL,NL)
//ͬһ��Ļ���IsNetConnected,conf_network��
//���ý���:�ȶ�linktype��ֵ��,�ٵ���.��������ȷ�������ú�������
//���� �ϲ��UP,�����н��²���UP����(�Լ򻯵���).�ر�ʱ,ִ���෴�Ķ���
//�ر�˵��:NetUp��LinkUp,NetDownҲ������LinkDown(����Ӧ����LinkDownҲ������NetDown).���������ǱȽ��ر��.
int NetUp(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	//���¶����������(����ж�)
	if((getlinktype()!=ASYN&&(ret=LinkUp())!=SUCC)
	||(getlinktype()==ASYN&&(ret=LinkUp4AsynPPP())!=SUCC))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:LinkUpʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;//FAIL;
	}
	
	if(netstatus==NETUP)
		return SUCC;

	transstatus = TRANSDOWN;
	return __NetUp();
}

static int __NetDown(void)
{
	/*private & local definition*/
	LinkType type = getlinktype();

	/*process body*/
	switch(type)
	{
	case GPRS:
	case CDMA:
	case TD:
	case ASYN:
		return __NetDown_PPP();
		break;
	case SYNC:
		return __NetDown_SDLC();
		break;
	case ETH:
		return __NetDown_ETH();
		break;
	case WLAN:
		return __NetDown_WLAN();
		break;
	default:
		cls_show_msg1(10, "%s, line %d:�Ͽ�δ֪����(%d)������ʧ��!", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}

	//�ŵ����Ӻ����и���netstatus = NETDOWN;
	cls_printf("�Ͽ������ɹ�!");
	return SUCC;
}

//����ֵûʲô����,�ɲ�ʹ��.��Ϊ�ڷ���FAIL,�û����ܵ�֪��
int NetDown(void)
{
	/*private & local definition*/

	/*process body*/
	TransDown(g_socket);//TransDown(g_socktype, g_socket);//����
	transstatus = TRANSDOWN;//��������Ĳ����ɹ���,������ǿ����һ��
	
	if(netstatus==NETDOWN)
		return SUCC;
	
	return __NetDown();
}

#if NDKSOCK_ENABLE
static int NDKTransUp(uint *s)
{
	/*private & local definition*/
	uint timeout = SO_TIMEO, sckt = 0;
	int ret = 0;
	sock_t socktype = netsetting.socktype;
	int (*socket[])(uint *) = {NDK_TcpOpen/*,  NDK_UdpOpen*/};
	
	/*process body*/
	if(socktype!=SOCK_NDKTCP && socktype!=SOCK_NDKUDP)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:�׽������ʹ���!", __FILE__, __LINE__);
		return FAIL;
	}
	
	cls_printf("NDK�����(%s)������...\n", (socktype==SOCK_NDKTCP)?"TCP":"UDP");
	
	//����socket
	if ((ret=socket[socktype](&sckt))!=NDK_OK)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:�����׽���ʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;
	}
	
	//����socket
	
	//�󶨱���IP
	//netsetting.LocalPort = rand()%10000+5000;//�ں�̨���������ֵ���ܻ��(�����岻��)
	netsetting.LocalPort = rand()%(65535-1024)+1024;  //�����ض˿ںŷ�Χ��5000~15000 ���� 1024~65535
	//netsetting.LocalPort++;//���ݱ�׼Э��ͻ�����Ҫ4����ȥ�ͷű��ض˿�,���������δ�ͷŵĶ˿������ʧ���޸Ķ˿��õ�����ʽ��
	if ((ret=NDK_TcpBind(sckt, netsetting.LocalIP.Str, HTONS(netsetting.LocalPort)))!=NDK_OK)//==SOCKET_ERROR)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:��ʧ��(%d)", __FILE__, __LINE__, ret);
		NDK_TcpClose(sckt);
		NDK_TcpWait(sckt);
		return ret;//FAIL;
	}
	
	//����socket
	//if(socktype==SOCK_NDKTCP)
	//{
		if((ret=NDK_TcpConnect(sckt, netsetting.SvrIP.Str, HTONS(netsetting.SvrTcpPort), timeout))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����ʧ��(%d)", __FILE__, __LINE__, ret);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			//sleep(10);//����ȴ����˷��������ĵ�4������ʱ,�Ŵӱ���������(������·̫���(NetDown),�Է��ղ���ACK)
			return ret;//FAIL;
		}
#if NEW_WIFI_ENABLE||defined SP610
		if((ret=lib_tcp_checkstatus(sckt, timeout))!=SOCKET_STATUS_CONNECT)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:TCP��ȡ����ʧ��(%d)", __FILE__, __LINE__, ret);
			NDK_TcpClose(sckt);
			NDK_TcpWait(sckt);
			return FAIL;//ret;
		}
#endif		
	//}

	*s = sckt;
	g_socktype = socktype;
	return SUCC;
}

//�������ô�³�ķ�ʽdown,����һ�����׵ı��յķ�ʽ������
static int NDKTransDown(uint s)
{
	/*private & local definition*/
	int ret = 0;
	
	/*process body*/
	cls_printf("NDK�����(%s)�Ͽ���...\n", (g_socktype==SOCK_NDKTCP)?"TCP":"UDP");
	if(TCPRESETFLAG==FALSE)
	{
		if ((ret=NDK_TcpClose(s))!=NDK_OK)//�ͷű�����Դ���ų�FIN
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:tcpCloseʧ��(%d)", __FILE__, __LINE__, ret);
			NDK_TcpWait(s);//close��ʧ����(����FIN��û�����Ͳ���wait��),tcpWait(s);
			return ret;//FAIL;
		}
	}
	else
	{
#if defined ME50C  //ME50Cֻ֧��ǿ�ƹҶ�,��û��Reset�ӿ�,��Ҫʹ��close�Ҷ� 20180620 modify
		if ((ret=NDK_TcpClose(s))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:tcpCloseʧ��(%d)", __FILE__, __LINE__, ret);
			NDK_TcpWait(s);
			return ret;
		}
#else
		if ((ret=NDK_TcpReset(s))!=NDK_OK)//ǿ�ƹҶ� 20140422 linwl 4.4.7����
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:tcpCloseʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;
		}
#endif
	}
#if 0
	if((ret=NDK_TcpWait(s))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:tcpWaitʧ��(%d)", __FILE__, __LINE__, ret);
		return FAIL;
	}
#else
	//msdelay(100);
	NDK_TcpWait(s);//���ж�,����������Ͽ��ȹ��϶��Ȳ����Զ˵�FIN�źŶ�������ʵ�����ͷ�����Դ��ɵ�״̬����
#endif
	//��sleep��Ϊ�˷�ֹtcpWaitû�еȴ��Է���FINҲ��û�����FIN��ACK(��tcpWaitֻ�Ǹ��պ���),��ֿ����������ӳٵ�����(��ʱ���������ͻȻ���õ�)
	//�ڲ�������ʱ,��������6s�������õ����(��ʱ�Է�6sʱ���յ�POS��FIN,�˺�ͻ�����Է����ϻ�ACK+FIN,��POS����ACK�����)
	//sleep(10);//����ȴ����˷��������ĵ�4������ʱ,�Ŵӱ���������(������·̫���(NetDown),�Է��ղ���ACK)
	
	return ret;
}

//��s����len�������ݷ��ڻ�ַΪrbuf�Ļ�������,rbuf��С�������len(���ϲ������б�֤)
//����ֵ��2������:>=0Ϊʵ�ʽ��ճ���;<0ΪNDK_TcpRead��(����)����ֵ.����Ʒ���ֵΪint��,�Դﵽһ�ַ���ֵͬʱ��ӳ2������Ϣ��Ŀ��
int NDKTransReceiveN(uint s, char *buf, uint len, uint timeout)
{
	/*private & local definition*/
	int ret = 0/*, rlen = 0, rcount = -1*/;
	uint rest = 0, total = 0, tmplen = 0;
	time_t diff = 0, oldtime = time(NULL);

	/*process body*/
	while(total<len)//ÿ��ֻ�ܽ���1488���ȣ�����3076Ҫ�ְ����գ�����ְ���
	{
		if((diff=time(NULL)-oldtime)>timeout)
			return total;
		else//���µȴ�ʱ��
			rest = timeout-diff;
		
		if((ret=NDK_TcpRead(s, buf+total, len-total, rest, &tmplen))!=NDK_OK)//��������,֮��ض���ʧ�ܵ���
		{
			//cls_show_msg1(10, "%s, line %d:����ʧ��(%d)", __FILE__, __LINE__, ret);
			return (total?total:ret);//return ret;//FAIL;
		}
		
		if (tmplen==0)
		{
#if 0
			if (++rcount==1)//����1������
			{
				//cls_show_msg1(10, "%s, line %d:�������ճ�ʱ(Ԥ��:%d ʵ��:%d)",  __FILE__, __LINE__, len, rlen);
				return total;//ret;//FAIL;
			}
			continue;
#else	//��������
			return total;
#endif
		}
		else
		{
			//rcount = 0;
			total+=tmplen;//�޸Ľ��յ��������ܳ���
		}
	}

	return total;
}

int NDKTransSendN(uint s, char *buf, uint len, uint timeout)
{
	/*private & local definition*/
	uint tmplen = 0;
	int ret = 0;

	/*process body*/
	return ((ret=NDK_TcpWrite(s, buf, len, timeout, &tmplen))==NDK_OK)?tmplen:ret;
}
#else
static int NDKTransUp(uint *s){return FAIL;}
static int NDKTransDown(uint s){return FAIL;}
int NDKTransReceiveN(uint s, char *buf, uint len, uint timeout){return FAIL;}
int NDKTransSendN(uint s, char *buf, uint len, uint timeout){return FAIL;}
#endif

#if NDKSSL_ENABLE
int WaitNDKSSLState(NDK_HANDLE sh, int state, int timeout)
{
	/*private & local definition*/
	int ret = 0;
	int times = 0;
	int actual = -1;
	time_t diff = 0, oldtime = time(NULL);//��ʹ�ò������뺯��StartStopwatch()
	
	/*process body*/
	while(1)
	{
		if((ret=NDK_GetSSLConnectStatus(sh, &actual))==NDK_ERR_TIMEOUT)
			return ret;//��ʱ�Ļ�,����ȷ���س�ʱ,������FAIL
		else if(ret!=NDK_OK)
			return FAIL;
		if(actual==state)
		{
			if(++times==TIME4KEEPSTATE)
				break;
		}
		if((diff=time(NULL)-oldtime)>timeout)//MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
			break;
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}

	show_stopwatch(DISABLE, 0);
	return (actual==state?SUCC:-3/*FAIL*/);
}

static int NDKSSLUp(NDK_HANDLE *s)
{
#define CERTF   "clientcert.pem"
#define KEY     "clientkey.pem"
#define PRIKEY_PWD	"123456"
	/*private & local definition*/
	int timeout = SO_TIMEO*1000, state = 0;//NDK_SSLConnect��ʱ������λ�Ǻ��� ����Ҫ*1000
	NDK_HANDLE sckt = NULL;
	int ret = NDK_OK;
	//ST_SOCKET_ADDR myadd;
	ST_SOCKET_ADDR svradd;
	int ciphers[] = {SSL3_CIPHER_RSA_NULL_MD5, SSL3_CIPHER_RSA_NULL_SHA, SSL3_CIPHER_RSA_RC4_40_MD5, SSL3_CIPHER_RSA_RC4_128_MD5, SSL3_CIPHER_RSA_RC4_128_SHA,
					SSL3_CIPHER_RSA_RC2_40_MD5, SSL3_CIPHER_RSA_IDEA_128_SHA, SSL3_CIPHER_RSA_DES_40_CBC_SHA, SSL3_CIPHER_RSA_DES_64_CBC_SHA,
		             		SSL3_CIPHER_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_DSS_DES_40_CBC_SHA, SSL3_CIPHER_DH_DSS_DES_64_CBC_SHA,
					SSL3_CIPHER_DH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_40_CBC_SHA, SSL3_CIPHER_DH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_40_CBC_SHA,
					SSL3_CIPHER_EDH_DSS_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_EDH_RSA_DES_40_CBC_SHA,
					SSL3_CIPHER_EDH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_ADH_RC4_40_MD5, SSL3_CIPHER_ADH_RC4_128_MD5,
					SSL3_CIPHER_ADH_DES_40_CBC_SHA, SSL3_CIPHER_FZA_DMS_NULL_SHA, SSL3_CIPHER_CK_FZA_DMS_FZA_SHA, SSL3_CIPHER_CK_FZA_DMS_RC4_SHA,
					SSL3_CIPHER_CK_ADH_DES_64_CBC_SHA, SSL3_CIPHER_CK_ADH_DES_192_CBC_SHA, 0x0};	
	
	/*process body*/
	cls_printf("SPSSL�㽨����...\n");
	//����SSLsocket
	if ((sckt=NDK_OpenSSLSocket(netsetting.SSLHsOpt, netsetting.SSLAuthOpt, ciphers))==NULL)
	{
		cls_show_msg1(10, "%s, line %d:�����׽���ʧ��", __FILE__, __LINE__);
		return FAIL;
	}
	//���ر���֤��
	if((ret=NDK_LoadClientCertificate(sckt,CERTF,SSL_FILE_PEM))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:���ر���֤��(%d)", __FILE__, __LINE__, ret);
		return ret;
	}
	//����˽Կ
	if((ret=NDK_LoadClientPrivateKey(sckt, KEY, SSL_FILE_PEM, PRIKEY_PWD))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:����˽Կ(%d)", __FILE__, __LINE__, ret);
		return ret;
	}
	//����socket
	if((ret=NDK_SetSSLBlockingMode(sckt, NDK_SUSPEND))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:���� ������ģʽʧ��(%d)", __FILE__, __LINE__, ret);
		NDK_CloseSSLSocket(sckt);
		return ret;
	}
	
#if 0	//no need
	//�󶨱���IP
	memset((void *) &myadd, 0, sizeof(ST_SOCKET_ADDR));
	myadd.addr_type = ADDR_IPV4;
	myadd.port = netsetting.LocalPort;
	myadd.addr = netsetting.LocalIP.Str;
	if((ret=NDK_SSLBind(sckt, &myadd))!=orvOK)
	{
		cls_show_msg1(10, "%s, line %d:��ʧ��(%d)", __FILE__, __LINE__, ret);
		NDK_CloseSSLSocket(sckt);
		return ret;
	}
#endif

	if(netsetting.SSLAuthOpt==SSL_AUTH_CLIENT)
	{
		if((ret=NDK_LoadServerCertificate(sckt, SCERTF, SSL_FILE_PEM))!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:װ�ط�����֤��ʧ��(%d)", __FILE__, __LINE__, ret);
			NDK_CloseSSLSocket(sckt);
			return ret;
		}
	}
	
	//����socket
	memset((void *)&svradd, 0, sizeof(ST_SOCKET_ADDR));
	svradd.unAddrType = SSL_ADDR_IPV4;
	svradd.usPort = netsetting.SvrTcpPort;
	svradd.psAddr = netsetting.SvrIP.Str;
	if((ret=NDK_SSLConnect(sckt, &svradd, timeout))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:����ʧ��(%d)", __FILE__, __LINE__, ret);
		NDK_CloseSSLSocket(sckt);
		return ret;
	}
	if((ret=NDK_GetSSLConnectStatus(sckt, &state))!=NDK_OK||state!=SSL_IS_CONNECTED)
	{
		cls_show_msg1(10, "%s, line %d:δ������(%d,%d)", __FILE__, __LINE__, ret, state);
		NDK_CloseSSLSocket(sckt);
		return ret;
	}

	*s = sckt;
	g_socktype = SOCK_NDKSSL;
	return SUCC;
}

static int NDKSSLDown(NDK_HANDLE s)
{
	/*private & local definition*/
	int ret = NDK_OK;
	int state = 0;
	
	/*process body*/
	cls_printf("SPSSL�Ͽ���...\n");
	if((ret=NDK_SSLDisconnect(s))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:�Ͽ�ʧ��(%d)", __FILE__, __LINE__, ret);
		//sleep(getlinktype()==ETH?3:6);//����ȴ����˷��������ĵ�4������ʱ,�Ŵӱ���������(������·̫���(NetDown),�Է��ղ���ACK)
		NDK_CloseSSLSocket(s);
		return ret;
	}
	if((ret=NDK_GetSSLConnectStatus(s, &state))!=NDK_OK||state!=SSL_IS_DISCONNECTED)
	{
		cls_show_msg1(10, "%s, line %d:δ�Ͽ�(%d,%d)", __FILE__, __LINE__, ret, state);
		NDK_CloseSSLSocket(s);
		return ret;
	}
	if(TCPRESETFLAG==FALSE)
	{
		if((ret=NDK_CloseSSLSocket(s))!=NDK_OK)
		{
			cls_show_msg1(10, "%s, line %d:�ͷ�SSLʧ��(%d)", __FILE__, __LINE__, ret);
			//��sleep��Ϊ�˷�ֹtcpWaitû�еȴ��Է���FINҲ��û�����FIN��ACK(��tcpWaitֻ�Ǹ��պ���),��ֿ����������ӳٵ�����(��ʱ���������ͻȻ���õ�)
			//�ڲ�������ʱ,��������6s�������õ����(��ʱ�Է�6sʱ���յ�POS��FIN,�˺�ͻ�����Է����ϻ�ACK+FIN,��POS����ACK�����)		
			//sleep(10);//����ȴ����˷��������ĵ�4������ʱ,�Ŵӱ���������(������·̫���(NetDown),�Է��ղ���ACK)
			return ret;
		}
	}
	else
	{
		if((ret=NDK_CloseSSLSocket(s))!=NDK_OK)//ǿ�ƹҶ�  20140422 linwl 4.4.7����
		{
			cls_show_msg1(10, "%s, line %d:�ͷ�SSLʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;
		}
	}
	
	return ret;
}

int NDKSSLReceiveN(NDK_HANDLE handle, void *pBuffer, /*const */size_t SizeOfBuffer/*, size_t *recv_data*/)
{
	/*private & local definition*/
	size_t tmplen = 0;
	int ret = 0;	

	/*process body*/
	NDK_SSLDataAvailable(handle, 2000);
	if(g_socktype==SOCK_NDKSSL)
		return ((ret=NDK_SSLReceive(handle, pBuffer, SizeOfBuffer, &tmplen))==NDK_OK)?tmplen:ret;//return NDK_SSLReceive(handle, pBuffer, SizeOfBuffer, recv_data);
	else
		return FAIL;
}

int NDKSSLSendN(NDK_HANDLE handle, /*const */char *pBuffer, size_t SizeOfBuffer/*, size_t *recv_data*/)
{
	/*private & local definition*/
	size_t tmplen = 0;
	int ret = 0;

	/*process body*/
	if(g_socktype==SOCK_NDKSSL)
		return ((ret=NDK_SSLSend(handle, pBuffer, SizeOfBuffer, &tmplen))==NDK_OK)?tmplen:ret;//return NDK_SSLSend(handle, pBuffer, SizeOfBuffer, recv_data);
	else
		return FAIL;
}
#else
int WaitNDKSSLState(NDK_HANDLE sh, int state, int timeout){return FAIL;}
static int NDKSSLUp(NDK_HANDLE *s){return FAIL;}
static int NDKSSLDown(NDK_HANDLE s){return FAIL;}
int NDKSSLReceiveN(NDK_HANDLE handle, void *pBuffer, /*const */size_t SizeOfBuffer/*, size_t *recv_data*/){return FAIL;}
int NDKSSLSendN(NDK_HANDLE handle, /*const */char *pBuffer, size_t SizeOfBuffer/*, size_t *sent_data*/){return FAIL;}
#endif

int __TransUp(uint *s)
{
	/*private & local definition*/
	int ret = 0;
	sock_t socktype = netsetting.socktype;
	
	/*process body*/
	switch(socktype)
	{
	case SOCK_NDKTCP:
	case SOCK_NDKUDP:
		ret = NDKTransUp(s);
		break;
	case SOCK_NDKSSL:
		ret = NDKSSLUp((NDK_HANDLE *)s);
		break;
	default:
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:����δ֪����(%d)�����ʧ��!", __FILE__, __LINE__, socktype);
		return FAIL;
		break;
	}

	if(ret==SUCC)
	{
		g_socket = *s;//����Ϊlinkdownʱʹ��
		//g_socktype = socktype;//�˾�������Ե�TransUp�гɹ��󱣴�,��ʹ�ø��Ե�TransDown�п�ʹ��g_socktype
		transstatus = TRANSUP;
		cls_printf("���������ɹ�!");
	}
	
	return ret;
}

//ע��:Ŀǰ������Ǵ����ֻ֧�ֵ�ͨ������
//���ñ�����ǰ,�����е���NetUp
//���ڴ�����װ�����ڲ���ȫ�ֱ�������(Ҳ��Ϊ��,TransUp��ʡȥ��socktype�����Ĳ���),��ʹ��ǰһ��Ҫ����һ�²��ܱ�֤��ȷʹ��
int TransUp(uint *s)
{
	/*private & local definition*/
	
	/*process body*/
	//���¶����������(����ж�)
#if 0 //���ǵ�������DOWN�Ե�,����UP���ɱ���������
	if(NetUp(type)!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:NetUpʧ��", __FILE__, __LINE__);
		return FAIL;
	}
#endif
	
	if(transstatus==TRANSUP)
		return SUCC;

	return __TransUp(s);
}

int __TransDown(uint s)
{
	/*private & local definition*/
	int ret = 0;
	
	/*process body*/
	//������Ӧ��ʵ����ȥ��ʾcls_printf("�����Ͽ���...\n");
	//ֻҪ�����ɹ�UP,��g_socktype������δ֪����,��default��Ҫ�д�����ʾ,���ĳЩ�����ط����в��Ѻ���Ϣ
	//�������û�гɹ�UP,��transstatus��DOWN״̬��,����������������
	switch(g_socktype)//switch(socktype)
	{
	case SOCK_NDKTCP:
	case SOCK_NDKUDP:
		ret = NDKTransDown(s);
		break;
	case SOCK_NDKSSL:
		ret = NDKSSLDown((NDK_HANDLE)s);
		break;
	default:
		transstatus = TRANSDOWN;//20160808 linwl����δ����δ֪����socket����û����ʵ�ʹҶϺ����Ӷ���
		//cls_show_msg1(10, "%s, line %d:�Ͽ�δ֪����(%d)�����ʧ��!", __FILE__, __LINE__, socktype);
		return FAIL;
		break;
	}
	if(ret==SUCC)
	{
		g_socket = 0;//-1;
		g_socktype = SOCK_DEFAULT;
		//transstatus = TRANSDOWN;
		cls_printf("�Ͽ������ɹ�!");
	}
	else
		cls_show_msg1_record(FILENAME,FUNCNAME,10, "%s, line %d:�����Ͽ�ʧ��!", __FILE__, __LINE__);

	transstatus = TRANSDOWN;//zhangnw20091223 ���۳ɹ���,һ����λ,��ʹ����socketδ��ȫ�ر�(��closeʧ��ʱ)Ҳ�Ȳ�����
	return ret;
}

int TransDown(uint s)
{
	/*private & local definition*/
	
	/*process body*/
	if(transstatus==TRANSDOWN)
		return SUCC;
	
	return __TransDown(s);
}

#if 0
//���:5s�ڲ��ϳ���ping,��һֱ���ɹ�,����Ϊʧ��.���Ի�����Ҫ�����̫�����״ο���ʱ,δ�Զ���������,����װ�����ַ���سɹ������
static int TestNetConnection(void)
{
	/*private & local definition*/
	time_t old = time(NULL), diff = 0;

	/*process body*/
	while(NDK_NetPing(netsetting.SvrIP.Str, 1)!=NDK_OK)//NDK_NetPing�����һ������Ϊs!�ĵ�д��Ϊms
	{
		if((diff=time(NULL)-old)>MAXTIMEOUT_PING)
			return FALSE;			

		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}
	show_stopwatch(DISABLE, 0);
	return TRUE;
}
#else
#if !K21_ENABLE
static int TestNetConnection(void){return (NDK_NetPing(netsetting.SvrIP.Str, MAXTIMEOUT_PING)!=NDK_OK)?FALSE:TRUE;}//NDK_NetPing�����һ������Ϊs!�ĵ�д��Ϊms
#else
#define TestNetConnection() FAIL
#endif
#endif

//�����ж�"�����"���������,��������MODEM��д����(��LINK��)ǰ����ͨ�Բ���
//����TRUE or FALSE.����,��������ı�lnk,net��״̬,�����ı䴫���״̬
int IsNetConnected(void)
{
	/*private & local definition*/
	int ret = FALSE;

	/*process body*/
	if (NetUp()!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:����UPʧ��!!!", __FILE__, __LINE__);
		return FALSE;
	}
	ret = TestNetConnection();//Ĭ��ʹ�ñ�׼socket��ʽ,�����ʹ��netsetting.socktype��������ͨ��
	NetDown();
	
	return ret;
}
#endif

