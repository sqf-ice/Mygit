#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <time.h>

#define FALSE 0
#define TRUE (!FALSE)

#define FAIL (-1)
#define SUCC 0
#define FSFAIL	(-1)
#define QUIT (-2)
#define FUNCQUIT (-4)  //���ܼ��˳�
#define FSSUCC		0

#define FRONT 0 //�������ͷ�����ȼ���ߣ�
#define TAIL  1 //�������β�����ȼ���ͣ�


#define MIN(a, b)	((a)>(b)?(b):(a))

#define nlmain main(int argc, char *argv[]) \
	{ int ret; config_init(); ret=_nlmain(); config_exit(); return ret;} int _nlmain

typedef short SOCKET;

extern int ErrorCode;
extern unsigned int g_language; /* 1 english, 0 chinese */

//���ܼ�
#define  F1			0x01
#define  F2			0x02
#define  F3			0x03
#define  F4			0x04
#define  F5			0x05
#define  F6			0x06
#define	 F7			0x07
#define	 F8			0x08
#define	 F9			0x09
#define  LEFT		0x13
#define  RIGHT		0x14
#define  UP         0x11
#define  DOWN       0x12
#define  MENU       F3
//ר�ü�
#define  BASP		0x0a    //backspace
#define	 BACKSPACE	BASP
#define  CR			0x0D	//ȷ�ϼ�
#define  ENTER		CR
#define  ESC		0x1B
#define	 ZMK		0x1C	//��ĸ��
#define  DOT		0x2E	//.
//���ּ�
#define ZERO		0x30
#define ONE			0x31
#define TWO			0x32
#define THREE		0x33
#define FOUR		0x34
#define FIVE		0x35
#define SIX			0x36
#define SEVEN		0x37
#define EIGHT		0x38
#define NINE		0x39
//����ATM��
#define	 ATM1		((unsigned char)0xa1)
#define	 ATM2		((unsigned char)0xa2)
#define	 ATM3		((unsigned char)0xa3)
#define	 ATM4		((unsigned char)0xa4)
#define  ATM5		((unsigned char)0xa5)
#define	 ATM6		((unsigned char)0xa6)
#define  ATM7		((unsigned char)0xa7)
#define  ATM8		((unsigned char)0xa8)
//������ֽ��ֽ��
#define	 JZK		0xb1		//��ֽ��
#define	 TZK		0xb2		//��ֽ��

/********************** from posdef.h *********************/

/*         define keyboard value                   */
#define		KeyEsc          'c'		/* ESC �� */
#define		KeyEnter        'e'		/* �س��� */
#define		KeyLeft         F1		/* ���'l'   *///20010827ZW
#define		KeyRight        F5		/* �Ҽ�'r'   *///20010827
#define		KeyBackspace    'b'		/* �˸�� */
#define		Key0            '0'
#define		Key1            '1'
#define		Key2            '2'
#define		Key3            '3'
#define		Key4            '4'
#define		Key5            '5'
#define		Key6            '6'
#define		Key7            '7'
#define		Key8            '8'
#define		Key9            '9'
#define		KeyS            '*'
#define		KeyR            '.'		/* '#' */
/**************** end of from posdef.h *******************/

/* key value */
#define BASP_VALUE		BASP
#define ENTER_VALUE		ENTER
#define ESC_VALUE		ESC
#define ZMK_VALUE		ZMK
#define F1_VALUE		F1
#define F2_VALUE		F2
#define F3_VALUE		F3
#define F4_VALUE		F4
#define F5_VALUE		F5
#define F6_VALUE		F6
#define F7_VALUE		F7
#define F8_VALUE		F8
#define F9_VALUE		F9
#define ATM1_VALUE		ATM1
#define ATM2_VALUE		ATM2
#define ATM3_VALUE		ATM3
#define ATM4_VALUE		ATM4
#define ATM5_VALUE		ATM5
#define ATM6_VALUE		ATM6
#define ATM7_VALUE		ATM7
#define ATM8_VALUE		ATM8
#define FORWARD_VALUE		JZK
#define BACKWARD_VALUE		TZK

#define ISCHAR(kb)	(kb.mode & 1)
#define ALTCHAR(kb)	(kb.mode & 0x80)

struct event {
    unsigned char type;		/* EVENT_* */
    union {
        struct {
            unsigned char code;	/* key code for KBD event */
            unsigned char value;/* char value */
            unsigned char mode;	/*  bit0: 1, printable key; 0, other key
                                 *  bit7: 1, should replace the former char
                                 */
        } kb;
    } data;
    void * priv;
};


//Ϊ�˱������������߼���IME_BIHUA
#define  ENGLISH IME_ENGLISH	//Ӣ��
#define  PINYIN	IME_NUMPY// ȫƴ
#define  QUWEI	IME_QUWEI	// ��λ
#define  NUMPY	IME_NUMPY	//��ƴ
#define  BIHUA  IME_BIHUA
#define IME_LAST_ONE -1		//��ǰ��һ��

#define EVENT_KBD	1
#define EVENT_TIMER     2
#define EVENT_MAGCARD	3

/* from bioscfg.h in NL8200 */

#define LCD_MAX_COL 240 /* Available values of column coordinate: 0��239 */
#define LCD_MAX_ROW 320 /* Available values of row coordinate: 0��319 */
#define MAXCHARPERLINE 30

#define MAXDISPLINE		36	//NL-GP730��������8*8�ַ���ʾ���������,19,20��ϵͳ������Ϊ״̬����ʾ
#define NORMAL_TEXT		0
#define REVERSE_TEXT	1

#define FONT_DOT_24 1
#define FONT_DOT_16 0


enum COLORS {
    WHITE,
    BLACK
};

enum fill_patterns {/* Fill patterns for rectangle and circle */
    NO_FILL,        /* no fill */
    SOLID_FILL      /* fills area in solid fill color */
};

//������ת����
enum ROTATE {
    ROTATE_0,ROTATE_90,ROTATE_180,ROTATE_270
};

#define TYPE_PIC  0 //ͼ�͵������
#define TYPE_DOT  1//�ֿ�������

//������ת�������
typedef struct {
    char* buf;
    int xsize;
    int ysize;
} TYPEDOTMATRIX;

#define AUX1 0
#define AUX2 1
#define AUX3 2

#define AUX_WIRELESS 	AUX3		/*���߽ӿ�*/
//#define AUX_PINPAD 		AUX2			/*������̽ӿ�*/

#define RECVBUF	1
#define SENDBUF	0

#define		BPS300		0
#define		BPS1200		1
#define		BPS2400		2
#define		BPS4800		3
#define		BPS7200		4
#define		BPS9600		5
#define		BPS19200		6
#define		BPS38400		7
#define		BPS57600		8
//#define		BPS76800		9//���а汾BIOS��֧�ָò�����
#define		BPS115200		10

#define	DB8		0xc0		//8λ����λѡ��
#define	DB7		0x40		//7λ����λѡ��
#define	DB6		0x80		//6λ����λѡ��
#define	DB5		0x00		//5λ����λѡ��
#define	STOP2		0x20		//2λֹͣλ
#define	STOP15		0x10		//1.5λֹͣλ
#define	STOP1		0x08		//1λֹͣλ
#define	NP		0x04		//	��У��
#define	EP		0x02		//	żУ��
#define	OP		0x01		//	��У��

#define PARAERR	(0x1FF)

#define SOH     1
#define STX     2
#define ETX     3
#define EOT     4
#define ENQ     5
#define ACK     6
#define DLE     0x10
#define NAK     0x15
#define ETB     0x17

#define NORMAL_DENSITY	(0<<0)		//��ͨ
#define HIGH_DENSITY	(1<<0)		//����

#define  IC1    0				/*��һ��IC��*/
#define  IC2    1				/*�ڶ���IC��*/
#define  SIM1  0x80				/*��һ��SIM��*/
#define  SIM2  0x81				/*�ڶ���SIM��*/
#define  SIM3  0x82				/*������SIM��*/
#define  SIM4  0x83				/*���ĸ�SIM��*/
#define  SAM1  0x80				/*��һ��SIM��*/
#define  SAM2  0x81				/*�ڶ���SIM��*/
#define  SAM3  0x82				/*������SIM��*/
#define  SAM4  0x83				/*���ĸ�SIM��*/
#define  ESAM  0x83				/*���ĸ�SIM��*/
#define  MIFARE		0xa1		/*��Ƶ��*/

#define  IC_0	0
#define  IC_1	0x01		//IC��ѡ��
#define  TK2    0x02		/*ֻ������*/
#define  TK3    0x04		/*ֻ������*/
#define  TK2_3    	(TK2 | TK3)   	/* 0x06  ͬʱ��������*/
#define	 TK1		0x01	//ֻ��һ�ŵ�
#define	 TK1_2_3	( TK1 | TK2 | TK3 )
#define	 TK1_2		( TK1 | TK2 )
#define  TK1_3		( TK1 | TK3 )
#define  MAGCARD	 0x01		/*ˢ�ſ�*/
#define	 STRIPE		0x01		/*ˢ�ſ�*/
#define  INSERTIC  	0x02		/*����IC��*/
#define  KEYIN	0x04			/*���뿨��*/
#define	 JUSTRETURN 	0    	/*��־���������ȴ�ʱ��󷵻�*/
#define  STANDBY 		1		/*��־���������ȴ�ʱ�������*/
#define  TIMEOUT 		(-3)	/*��ʱ����*/

#define	EM3K_TRIG	(1<<2)	//ɨ��ͷTRIG�ܽ�,���ڿ�������ɨ��
#define	EM3K_WKUP	(1<<5)	//ɨ��ͷ���ѹܽ�


/*****************************************************************************
 * �����Ǵ�ӡģ����ص���Ϣ *******************************************************
 *****************************************************************************/
/********************
 * ��ӡ������Ϣ        *
 ********************/
//for printnew.c
#define DOT8x16  1
#define DOT16x16 2
#define DOT16x32 3
#define DOT24x32 4
#define DOT6x8   5
#define DOT8x8   6
#define DOT5x7      7
#define DOT5x16     8
#define DOT10x16    9
#define DOT10x8     10
/*����Ϊ������������--lingb. 20100809*/
#define DOT12x16A	11	/*MSGothic����12x16����*/
#define DOT12x24A	12	/*Gulimche����12x24����*/
#define DOT16x32A	13	/*MSGothic����16x32����*/
#define DOT12x16B	14	/*MSGothic����12x16����*/
#define DOT12x24B	15	/*MSGothic����12x24����*/
#define DOT16x32B	16	/*MSGothic����16x32����*/
#define DOT12x16C	17	/*�������12x16����*/
#define DOT12x24C	18	/*�������12x24����*/
#define DOT16x32C	19	/*�������16x32����*/

#define ASCFONTNUM  19   /**! !Attention: ���������ʱ����Ҫ�޸ĸò��� */

#define HZ24x24  1
#define HZ16x32  2
#define HZ32x32  3
#define HZ32x16  4
#define HZ24x32  5
#define HZ16x16  6
#define HZ12x16  7
#define HZ16x8   8
/*������������24x24�����ֿ�--lingb. 20100809*/
#define HZ24x24A  9		/*��������24x24����*/
#define HZ24x24B  10		/*����24x24����*/
#define HZ24x24C  11		/*����24x24����*/
#define HZ24x24USER  12		/*�û��Զ���24x24����*/


#define HZFONTNUM  12    /**! !Attention�����������ʱ����Ҫ�޸ĸò��� */


/**********************
 * ��ӡ�豸������Ϣ       *
 **********************/
#define PRNOPAPER       (1<<1)  //< ��ӡ��ȱֽ����ʾ��װֽ��
#define PRISHOT         (1<<2)  //< ��ӡ������
#define PRNOTREADY      (1<<3)  //< ��ӡ��æ��һ���ӡ�����ڴ�ӡʱ�᷵�ظ�ֵ
#define PRINTOVER       (1<<8)  //< ��ӡ�������������һ�δ�ӡ��Ϻ�δ����

#define TP_ERR_VOLT_LOW		(1<<4)	//�͵�ѹ����
#define TP_ERR_VOLT_WARN	(1<<5)	//�͵�ѹ��ʾ
#define TP_ERR_VOLT_TOP		(1<<6)	//�ߵ�ѹ����
#define PRNOPOWER			(TP_ERR_VOLT_LOW|TP_ERR_VOLT_WARN|TP_ERR_VOLT_TOP)
// print��printimage����������֮һʼ�շ��ظ�ֵ
#define PRUNKOWNSTATUS  (-16) //< ��ӡ��������δ��������������س���
#define PRHPInvalid     (1<<9) //< ��ӡ��ʧ�����������´�ӡ(�����Ǵ�ӡ������ֽм)
#define PRQUIT          (-2)
#define PROK            0     //< ��ӡ��״̬���ã����跳�ġ�

/**********************
 * ����ԭ�д���
 *********************/
#define TPNOPAPER       PRNOPAPER
#define TPISHOT         PRISHOT
#define TPNOTREADY      PRNOTREADY
#define TPPRINTOVER     PRINTOVER
#define TPNOPOWER       PRNOPOWER
#define TPUNKOWNSTATUS  PRUNKOWNSTATUS
#define TPHPInvalid     PRHPInvalid
#define TPQUIT          PRQUIT
#define TPOK            PROK

#define IPNOPAPER       PRNOPAPER
#define IPISHOT         PRISHOT
#define IPNOTREADY      PRNOTREADY
#define IPPRINTOVER     PRINTOVER
#define IPNOPOWER       PRNOPOWER
#define IPUNKOWNSTATUS  PRUNKOWNSTATUS
#define IPHPInvalid     PRHPInvalid
#define IPQUIT          PRQUIT
#define IPOK            PROK


/**********************
 * ��ӡ��������Ϣ         *
 **********************/
enum PRINTTYPE_DEF {
    PRINTTYPE_TP=0,             //< ������ӡ��
    PRINTTYPE_HIP,              //< �������
    PRINTTYPE_FIP,              //< Ħ�����
    PRINTTYPE_END               //< ��
};


/***********************
 * ��ӡģʽ��Ϣ            *
 ***********************/
#define IPMODE_NORMAL_DOUBLE    0       //< ��������˫��
#define IPMODE_NORMAL_SINGLE    1       //< ������������
#define IPMODE_COMPRESS_DOUBLE  2       //< ����ѹ��˫��
#define IPMODE_COMPRESS_SINGLE  3       //< ����ѹ������
#define IPMODE_NORMAL_DOUBLE_ROW    4   //< ��������˫�򣭺���ѹ��
#define IPMODE_NORMAL_SINGLE_ROW    5   //< �����������򣭺���ѹ��
#define IPMODE_COMPRESS_DOUBLE_ROW  6   //< ����ѹ��˫�򣭺���ѹ��
#define IPMODE_COMPRESS_SINGLE_ROW  7   //< ����ѹ�����򣭺���ѹ��
#define IPMODE_MAX_MODE     8           //< ?


/***********************
 * ����ͼ�����Ű�ģʽ*
 ***********************/
typedef enum PRN_TYPESETTING {
    TPSET_AUTO = 0,		//<�Զ���Ӧ�����ֻ���ͼ�񣬱�֤���غϴ�ӡ
    TPSET_TEXTUP,		//<�������ϣ��������غϣ��ļ���ֱ�Ӹ�д��ͼ����
    TPSET_PICUP,		//<ͼ�����ϣ��������غϣ�ͼ��ֱ�Ӹ�д���ļ���
    TPSET_MIX			//<����ͼ��Ƕ�ף����غϣ����ֺ�ͼ��Ƕ�״�ӡ
} PRN_TYPESETTING;

/*****************************************************************************
 * ��ӡģ����Ϣ���� *************************************************************
 *****************************************************************************/

//esimkey.c
#define ENCRYPT  0
#define DISCRYPT 1
#define MAINKEYTYPE 0x35
#define WORKKEYTYPE 0x33

#define NORMAL 1
#define PASSWD 0
#define	   ENCRYPT		0		/*����*/
#define    DECRYPT		1		/*����*/

#define CONNECT_AFTERPREDIAL	2	//������
#define OK_AFTERPREDIAL			1	//���ųɹ�
#define NORETURN_AFTERPREDIAL	0	//�����л򲦺��޷���
#define NOPREDIAL				(-11)//��δ����,��SDLC��������ֹ,�ڴ��ٴβ���
#define MS_NODIALTONE	(-2)		//û�м�⵽������
#define MS_NOCARRIER	(-3)  		//û�н��յ��ز����ز���ʧ
#define MS_BUSY			(-4)		//��⵽æ����绰��û��
#define MS_ERROR		(-5)		//�����г���
#define LINE_VOL_TOO_LOW	(-8)		//���ڲ��������зֻ���ʹ�õ绰��·����Ҫ���������ѹVB202M
//#define getch() getkeycode(0)

//ppp��״ֵ̬
typedef enum {
    PHASE_DEAD = 0,			//ppp�ر�
    PHASE_INITIALIZE,		//PPP���ڳ�ʼ��
    PHASE_ESTABLISH,		//PPP�����ȶ�������δ���IP��ַ
    PHASE_AUTHENTICATE,		//PPP���ڽ����û������������֤
    PHASE_CALLBACK,			//PPP�ظ�����
    PHASE_NETWORK,			//PPP����INTERNET�������IP��ַ���Խ���TCPIP����
    PHASE_TERMINATE			//ppp�ж���
} LinkPhase;

// �����ErrorCode��ϵͳ��������	//���ڴſ�ˢ���ɹ��ж�
enum SYS_ERRORCODE {
    MAGCARD_TK1_LRC_ERR=0xC01,										// һ��LRCУ��ʧ��
    MAGCARD_TK2_LRC_ERR=0xC02,										// ����LRCУ��ʧ��
    MAGCARD_TK3_LRC_ERR=0xC08,										// ����LRCУ��ʧ��
    MAGCARD_TK12_LRC_ERR=MAGCARD_TK1_LRC_ERR|MAGCARD_TK2_LRC_ERR,	// һ������LRCУ��ʧ��
    MAGCARD_TK23_LRC_ERR=MAGCARD_TK2_LRC_ERR|MAGCARD_TK3_LRC_ERR,	// ��������LRCУ��ʧ��
    MAGCARD_TK13_LRC_ERR=MAGCARD_TK1_LRC_ERR|MAGCARD_TK3_LRC_ERR,	// һ������LRCУ��ʧ��
    MAGCARD_TK123_LRC_ERR
    =MAGCARD_TK1_LRC_ERR|MAGCARD_TK2_LRC_ERR|MAGCARD_TK3_LRC_ERR	// һ����������LRCУ��ʧ��
};

enum PPP_DEVICE_TYPE {
    NET_DEV_NULL=0,
    PPP_DEV_WLM,	//�����豸,��GPRS CDMA ,���ǲ�������1
    PPP_DEV_WM	,		//����MODEM
    NET_DEV_ETH,		//��̫��
    NET_DEV_END   //�����ж����õ�ֵ�ķ�Χ
};

//����ģ������
enum WLMTYPE {
    WLM_NONE=0x00,
    WLM_CDMA_XXXX=0x80,
    WLM_GPRS_MC39I=0x02,
    WLM_GPRS_SM300=0x03,
    WLM_GPRS_SM700=0x05,
    WLM_CDMA_DTGS800=0x81,
    WLM_CDMA_DTM228C=0x82,
    WLM_CDMA_MC8331=0x83,
    WLM_CDMA_EM200=0x84,
	WLM_GPRS_M72=0x06
};

/* clock */
#define USE_HW_CLOCK	1	/* use hardware rtc */

enum CLOCK_STYLE {
    RTC_TIME_ONLY,
    RTC_DATE_TIME_ONE_LINE,
    RTC_DATE_TIME_TWO_LINES,
};

struct postime {
    char		yearh;      /*  ��ݵĸ��ֽ� BCD��ʾ*/
    char		yearl;		/*  ��ݵĵ��ֽ� BCD��ʾ*/
    char		month;		/*  �·�	 BCD��ʾ1--12*/
    char		day;		/*  ��		 BCD��ʾ1--31*/
    char		week;		/*	0--6��Ӧ������--������ */
    char		hour;		/*  Сʱ	 BCD��ʾ0--23*/
    char		minute;		/*	��	 BCD��ʾ0--59*/
    char		second;		/*	��	 BCD��ʾ0--59*/
};

#define ESIMtableAddress		0
/* end of bioscfg.h in NL8200*/

/*****************************************************************************
* PPP TCP������붨��
*****************************************************************************/
#define PPPERR_PARAM 	-1		/* Invalid parameter. */
#define PPPERR_OPEN 	-2		/* Unable to open PPP session. */
#define PPPERR_DEVICE 	-3		/* Invalid I/O device for PPP. */
#define PPPERR_ALLOC 	-4		/* Unable to allocate resources. */
#define PPPERR_USER 	-5		/* User interrupt. */
#define PPPERR_CONNECT 	-6		/* Connection lost. */
#define PPPERR_AUTHFAIL -7		/* Failed authentication challenge. */
#define PPPERR_PROTOCOL -8		/* Failed to meet protocol. */
#define PPPERR_TASKCREATE -9	/* Failed to create pppMain task, added by huly */

#define TCPERR_EOF 		-1		/* End of data. */
#define TCPERR_ALLOC 	-2		/* Unable to allocate a control block. */
#define TCPERR_PARAM 	-3		/* Invalid parameters. */
#define TCPERR_INVADDR 	-4		/* Invalid address. */
#define TCPERR_CONFIG 	-5		/* Invalid configuration. */
#define TCPERR_CONNECT 	-6		/* No connection. */
#define TCPERR_RESET 	-7		/* Connection reset received. */
#define TCPERR_TIMEOUT 	-8		/* Timeout on transmission. */
#define TCPERR_NETWORK 	-9		/* Network error - unreachable? */
#define TCPERR_PREC 	-10		/* IP Precedence error. */
#define TCPERR_PROTOCOL -11		/* Protocol error. */
#define TCPERR_PPPCLOSE -12		/* PPP�Ѿ��رգ�����TCP���� */

#ifdef USE_MULTIAPP_V1
/*************************** multi app **************************************/
enum USERPRG_INFO_METHOD {
    UPIM_DESP,
    UPIM_SERINO
};

//�û�������Ϣ
typedef struct tagUsrPrgInfo {
    char cSeriNo;				// �û�����ı��
    char cDespBuf[17];
    char cReserved[2];				//Ԥ��
    unsigned int nChksum;
    unsigned int nStartAddress;
    int  len;
    time_t itime;
    char cVerBuf[16];
    int Reserved[6];			//Ԥ��
} USRPRGINFO;
#endif

#define MAX_APP_NUM		9

/* user path */
#define USERBINFS_CHARDEV			"/dev/mtd5"
#define USERBINFS_BLKDEV			"/dev/mtdblock5"
#define USERBINFS_ROOT  			"/appfs"
#define USERDATFS_CHARDEV			USERBINFS_CHARDEV
#define USERDATFS_BLKDEV			USERBINFS_BLKDEV
#define USERDATFS_ROOT  			USERBINFS_ROOT
#define USER_BIN_PATH		USERBINFS_ROOT"/apps/"
#define USER_CONFIG_PATH	USERBINFS_ROOT"/etc/"
#define USER_DATA_PATH		USERDATFS_ROOT"/data/"
#define USER_LIB_PATH		USERBINFS_ROOT"/lib/"
#define TMP_PATH					"/tmp/"

#ifdef	CONFIG_NLGP730_L1
#define TMP_UPDATE_PATH					TMP_PATH"update/"
#endif
#define USER_PROGRAM_ENTRY		USER_BIN_PATH"userapp"
#define MIYAO_PROGRAM_ENTRY		USER_BIN_PATH"miyaoapp"
#define MULTI_USERAPP_FORMAT	"userapp%d"

#define MULTIAPP_CONFIG			USER_CONFIG_PATH"multiapp_v2.ini"

#ifndef PATH_MAX
#define PATH_MAX	4096
#endif

#define MAX_APPNAME_LEN	 	32
#define MAX_VERSION_LEN		16
#define MAX_TIME_LEN	 	32

#define USERPRG_EXIST 		-2
#define USERPRG_NOT_EXIST 	-3

/**
 * �����������Ͷ��壬�������PCI�ͼ췽���е�������֤��Կ��Ӧ�ù̼��ӽ�����Կʹ�á�
 * ͬʱ�����������еĴ�ͳ���ط�������ʹ��DFT_NORMAL_XXX����
 */
enum _DOWNLOAD_FILE_TYPE {
	DFT_NORMAL_APP = 0,		/*������ͨӦ����������*/
	DFT_NORMAL_FIRMWAR,		/*������ͨ�̼���������*/
	DFT_PCI_APP = 0x10,			/*PCI�汾��Ӧ����������*/
	DFT_PCI_FIRMWARE,		/*PCI�汾�Ĺ̼���������*/
};

/*��Ӧ�ö���*/
typedef struct tagV2USRPRGINFO {
    char 			cAppName[MAX_APPNAME_LEN + 1];
    char			cVerBuf[MAX_VERSION_LEN + 1];											/*�û�����汾��Ϣ					*/
    int				cSeriNo;													/*���,V2�汣��,�����岻ͬ	*/
//	char			sHeaderVer[4];										/*�ļ�ͷ�汾								*/
//	unsigned int	uiFileLen;										/* ���ض�λ��Ϣ�ļ�����			*/
//	time_t			tCreateTime;										/* �ļ�����ʱ��							*/
    char			tBuildTime[MAX_TIME_LEN + 1];											/* �ļ�����ʱ��							*/
    int				bIsMaster;
//	unsigned long    uiInfoBase;					/*�ļ���Ϣ����ַ		*/
    unsigned long	sReverse[3];				/*������*/
} V2USRPRGINFO;																/*Ӧ����Ϣ�ṹ�����Ÿ�Ӧ�ò�*/

/************************************************************************/
/* V2��Զ����������ṹ	                                                */
/************************************************************************/
typedef struct tagLocalCMDNODE {
    char			sAppName[17];					/*Ӧ�ó�������	16										*/
    char			sFileName[13];				/*Ӧ�ó����ļ���12										*/
    char			cType;								/*Ӧ������														*/
    unsigned char   ucCmd;					/*����																*/
    unsigned int	uiReverse[2];			/*����																*/
} UPDATECMD;												/*Զ��������Ӧ�ò㴫������ݽṹ			*/

/*���µĴ�������*/
enum {
    CMD_UPDATE=0,									/*����																	*/
    CMD_DELETE=1,									/*ɾ��																	*/
    CMD_INSERT=3,									/*���																	*/
    CMD_RESERVE=2									/*����																	*/
};															/*Ӧ�ó���������											*/


/************************************************************************/
/* Ӧ�ó�������					                                                */
/************************************************************************/
enum {
    APP_SLAVE=0XFF,								/*��Ӧ��																*/
    APP_MASTER=0XFA,							/*����																	*/
    APP_LIB=0XF6									/*Ӧ�ÿ�																*/
};

struct pos_info {
    char cpu_ver[16];
    unsigned int fclk, hclk, sdclk;			//��λ��MHZ
    int  sdram_size, nor_size, nand_size;	//��λ��sdram_size(KB) nor_size, nand_size(MB)
//	unsigned char hwconfig;
};

enum SLEEP_LEVEL {NO_SLEEP, MID_SLEEP, DEEP_SLEEP};

#define WLM_SQ_LIMIT_DEFAULT	8			//�ź�ֵȱʡ�ж�
#define WLM_SQ_APP_LIMIT		15			//Ӧ���жϵ�����ֵ

#define NET_EN_CFG
//208��Ϊ��֧��ȡ����Ӳ����Ϣ�����˵�������ʼֵ��1����2
#define NO_MCPU_VER		0x02	//��CPU����汾
#define NO_SCPU_VER		0x03	//��CPU�汾
#define NO_MODEM_VER	0x04	//MODEMӲ���汾
#define NO_FRAM_VER		0x05	//FRAM��������
#define NO_PRINTER_VER	0x06	//��ӡ��������
#define NO_CPLD_VER		0x07	//CPLD�汾
#ifdef NET_EN_CFG
#define NO_NET_VER		0x08	//�����豸���ͼ��汾
#endif
#define NO_LCD_VER		0x09	//Һ������
#define NO_POS_TYPE		0x0A	//POS���ͺţ���8200��8300
#define NO_POS_SN		0x0B	//POS�Ĳ�Ʒ���к�
#define NO_POS_OWNER	0x0C	//POS�Ĳ�Ȩ��Ϣ

#define NO_DISK_SPACE      0x0D    //������ʣ��ռ�
#define NO_USER_SPACE      0X0E    //Ӧ�ó���ʹ�ÿռ�
#define NO_SRAM_SPACE      0x0f    //�ڴ�ʣ��ռ�
#define NO_KEY_COUNT       0x10    //��������
#define NO_CARD_COUNT      0x11    //�ۼ�ˢ������ͳ��
#define NO_MODEMTIME_TOTAL 0X12   //�ۼ�����modemͨѶ����ʱ��
#define NO_PPPTIME_TOTAL   0X13   //�ۼ�PPPͨѶ����ʱ��
#define NO_TCPTIME_TOTAL   0x14   //�ۼ�TCPͨѶ����ʱ��
#define NO_PRINT_LEN       0X15    //�ۼƴ�ӡ������
#define NO_SYSTEM_ERROR    0X16    //�쳣��Ϣ
#define NO_POWERON_TIME    0x17    //����������ʱ��
#define NO_MAGCARD_TYPE    0x18    //��ȡ��ͷ�İ汾����
#define NO_SRAM_SIZE    0X20      //SRAM��С
#define NO_HDR_VER_END	0xff
#define NO_SIMPLEHDR_START	NO_MCPU_VER
#define NO_SIMPLEHDR_END	NO_SRAM_SIZE


#endif
