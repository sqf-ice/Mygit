/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ���Ͷ���
* file name		: typedef.h
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20060704
* directory 		: .\inc\
* description		: 
* related document	: TCR690 API���Գ��� ��ϸ���˵����.doc
* 
************************************************************************
* log			: 
* Revision 1.0  20060704 14:34 zhangnw
* created
************************************************************************/
#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
typedef void (*PF)(void);
typedef void* (*PTHREAD)(void*);

#if 0	//����Ӧ�ÿ��������ṩ
#if !defined UCHAR
	#define UCHAR	unsigned char
#endif
#if !defined SCHAR
	#define SCHAR	signed char
#endif
#if !defined USHORT
	#define USHORT	unsigned short
#endif
#if !defined UINT
	#define UINT		unsigned int
#endif
#if !defined ULONG
	#define ULONG	unsigned long
#endif

#if !defined uchar
	#define uchar	UCHAR
#endif
#if !defined schar
	#define schar	SCHAR
#endif
#if !defined ushort
	#define ushort	USHORT
#endif
#if !defined uint
	#define uint		UINT
#endif
#if !defined ulong
	#define ulong	ULONG
#endif
#else
typedef uchar UCHAR;
typedef ushort USHORT;
typedef uint UINT;
typedef ulong ULONG;
#endif

/**	������ö��*/
typedef enum __Bool{FALSE, TRUE}Bool;

/**�ֽ�����*/
typedef enum __ENDIAN_t{LITTLEENDIAN, BIGENDIAN}ENDIAN_t;

//�Զ��弸���κŵ�״̬��ֵ
/**	MDM״̬ö��*/
//typedef enum __ModemStatus{INITIAL, INITED, PREDIALED, DIALED}ModemStatus;
/**	��·״̬ö��*/
typedef enum __LinkStatus{LINKUP, LINKDOWN}LinkStatus;
/**	����״̬ö��*/
typedef enum _NetStatus{NETUP, NETDOWN}NetStatus;
/**	�����״̬ö��*/
typedef enum __TransStatus{TRANSUP, TRANSDOWN}TransStatus;

/**ͨѶ��ʽ*/
typedef enum __LinkType{NONE, GPRS, CDMA, TD, ASYN, SYNC, ETH, SERIAL, WLAN, BT}LinkType;

/*����ģʽ*/
typedef enum __Network_t{second_G,third_G,fourth_G}Network_t;

/**SOCK����,��-1��ʼ*/
typedef enum __sock_t{SOCK_DEFAULT=-1, SOCK_NDKTCP=0, SOCK_NDKUDP=1, SOCK_NDKSSL=2}sock_t;

typedef enum __SMART_t{CPU_A, CPU_B, /*HS_B, */MIFARE_1, FELICA, MIFARE_0,  ISO15693, /*RFID_SIM, */SAM, IC}SMART_t;

typedef enum __MEMORY_t{AT24C01,AT24C02,AT24C04,AT24C08,AT24C016,AT24C032,AT24C064,SLE4432_42,SLE4418_28,SLE5528,AT88SC102,AT88SC1604,AT88SC1608,AT88SC153}MEMORY_t;

typedef struct __LinkSetting
{
	int snd_com;
	int snd_flag;
	LinkType lnktype;
	char lnkbps[8];//int lnkbps;//һ�㴮��bps,����g_com������.g_com��BPS�Ƕ���Ϊ115200��
	int WMPatchType;//int WMCountry;//zhangnw20110427add
	char WMDialStr[16];
	char WLMDialStr[16];
	char WLMApn[64];
	char PPPUsr[8];
	char PPPPwd[8];
	int Simslot;//˫SIM�����۲���
	int devicecom;
	char devicecombps[8];//int devicecombps;//����lnkbps����,�������������ı���ֵ(��û�б���)
	int WLANEncMode;
	int WLANKeyType;
	//char WLANPwd[16];
	char WLANPwd[65];//·���������������64�ֽ����볤��,��Ӧ�޸Ĳ��԰���֧��linwl20150819
	char WLANEssid[128];//T_ApInfo WLANAp;
	int WLAN8021XEapMode;//802.1x��֤ģʽ
	char WLAN8021XIdentity[128];//802.1x��֤�û���
	char BTPin[7];//��������
	char BTName[16];//��������

	//wifi mac�����ñ�־
	uchar WIFIMac[6];
	int WIFIMacFlag;
	int WIFIHiddenFlag;
	
	int WIFIApShareDev;//WiFi�ȵ���������
	int WIFIApSecMode;//WiFi�ȵ㰲ȫ����ģʽ
	char WIFIApSsid[33];//WiFi�ȵ�ssid
	uchar WIFIApHidden;//�Ƿ�����ssid
	char WIFIApKey[65];//WiFi�ȵ�����

}LinkSetting;

typedef union __Value
{
	ulong	data;
	uchar	asc[4];
}Value;

typedef struct __IPAddr	//IPv4
{
	char Str[16];
	Value value;
}IPAddr;

//ssl  //��ȡ��flag.h��
/*#if CPU5892_ENABLE||OVERSEAS_ENABLE
	#define	NDKSSL_ENABLE	0   //ENABLE   20140822 ssl ��5.0�汾����ʱȥ��
#else
	#define	NDKSSL_ENABLE	0	//DISABLE
#endif*/

typedef struct __NetworkSetting
{
	IPAddr	SvrIP;
	ushort 	SvrTcpPort;
	ushort	SvrUdpPort;
	IPAddr	LocalIP;
	ushort	LocalPort;
	IPAddr	LocalMask;
	IPAddr	LocalGateway;
	sock_t	socktype;
#if NDKSSL_ENABLE
	EM_SSL_HANDSHAKE_OPT SSLHsOpt;
	EM_SSL_AUTH_OPT SSLAuthOpt;
	EM_SSL_FILE_FORMAT SSLFileFmt;
#endif
	char	DNSHost[3][16];
	int		DHCPenable;//zhangnw20111109add
	//char 	Reserved[8];

	IPAddr	WifiApIp;
}NetworkSetting;

typedef struct __Packet
{
	char	*header;
	int	orig_len;//��ʼ����
	int 	len;//ʵ�ʳ���
	int 	lifecycle;
	int 	forever;//����־ΪTRUEʱ,lifecycle��������(����������)
	int	IsLenRec;
	int	IsDataRnd;
}Packet;

#ifndef	C_class
	#define C_class struct 
#endif

typedef struct _KeyTable
{
	char* keyname;
	int keycode;
}KeyTable;

//����Ʒ���Ի�������

//end

/*------------global variables definition-----------------------*/

/*---------------structure definition---------------------------*/

#endif

