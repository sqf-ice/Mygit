/*
* �´�½��˾ ��Ȩ����(c) 2011-2015
*
* ͳһƽ̨NDK API
* ��    �ߣ�	��Ʒ������
* ��    �ڣ�	2012-08-17
* ��	����	V1.00
* ����޸��ˣ�
* ����޸����ڣ�
*/
#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#ifndef __NDKAPI__H
#define __NDKAPI__H

/** @addtogroup �����붨��
* @{
*/
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned short ushort;
/**
 *@brief ���������뷵��
*/
typedef enum {
	NDK_OK,					/**<�����ɹ�*/
	NDK_ERR=-1,				/**<����ʧ��*/
	NDK_ERR_INIT_CONFIG = -2,	 /**<��ʼ������ʧ��*/
	NDK_ERR_CREAT_WIDGET = -3,	/**<�����������*/
	NDK_ERR_OPEN_DEV = -4,/**<���豸�ļ�����*/
	NDK_ERR_IOCTL = -5,	/**<�������ô���*/
	NDK_ERR_PARA = -6,	/**<�����Ƿ�*/
	NDK_ERR_PATH = -7,	/**<�ļ�·���Ƿ�*/
	NDK_ERR_DECODE_IMAGE = -8,	/**<ͼ�����ʧ��*/
	NDK_ERR_MACLLOC=-9,			/*�ڴ�ռ䲻��*/
	NDK_ERR_TIMEOUT = -10,		/**<��ʱ����*/
	NDK_ERR_QUIT = -11,		/**<��ȡ���˳�*/
	NDK_ERR_WRITE = -12, /**<д�ļ�ʧ��*/
	NDK_ERR_READ = -13, 	/**<���ļ�ʧ��*/
	NDK_ERR_OVERFLOW = -15,		/**<�������*/
	NDK_ERR_SHM = -16,	/**<�����ڴ����*/
	NDK_ERR_NOSWIPED = -50,	/**<�޴ſ�ˢ����¼*/
	NDK_ERR_NO_SIMCARD = -201,	/**<��SIM��*/
	NDK_ERR_PIN = -202, /**<SIM���������*/
	NDK_ERR_PIN_LOCKED = -203,	/**<SIM��������*/
	NDK_ERR_PIN_UNDEFINE = -204,	/**<SIM��δ�������*/
	NDK_ERR_EMPTY = -205,			/**<���ؿմ�*/
	NDK_ERR_PPP_PARAM = -301,	/**<PPP��������*/
	NDK_ERR_PPP_DEVICE = -302,/**<PPP��Ч�豸*/
	NDK_ERR_PPP_OPEN = -303, /**<PPP�Ѵ�*/
	NDK_ERR_TCP_ALLOC = -304,	/**<�޷�����*/
	NDK_ERR_TCP_PARAM = -305,	/**<��Ч����*/
	NDK_ERR_TCP_TIMEOUT = -306,	/**<���䳬ʱ*/
	NDK_ERR_TCP_INVADDR = -307,	/**<��Ч��ַ*/
	NDK_ERR_TCP_CONNECT = -308,	/**<û������*/
	NDK_ERR_TCP_PROTOCOL = -309,/**<Э�����*/
	NDK_ERR_TCP_NETWORK = -310,	/**<�������*/
	NDK_ERR_TCP_SEND = -311,	/**<���ʹ���*/
	NDK_ERR_TCP_RECV = -312,	/**<���մ���*/
	NDK_ERR_GET_NETADDR = -401,/**<��ȡ���ص�ַ����������ʧ��*/
	NDK_ERR_GET_GATEWAY = -402,	/**<��ȡ���ص�ַʧ��*/
	NDK_ERR_ADDR_ILLEGAL =-403,/**<��ȡ��ַ��ʽ����*/
	NDK_ERR_MODEM_COUNTRYFAIL=-501,/**<MODEM�򲹶�ʧ��*/
	NDK_ERR_MODEM_SDLCINITFAIL=-502,/**<MODEMͬ����ʼ��ʧ��*/
	NDK_ERR_MODEM_ASYNINITFAIL=-503,/**<MODEM�첽��ʼ��ʧ��*/
	NDK_ERR_MODEM_SDLCDIALFAIL=-504,/**<MODEMͬ������ʧ��*/
	NDK_ERR_MODEM_INIT_NOT=-505,/**<MODEMδ���г�ʼ��*/
	NDK_ERR_MODEM_SDLCWRITEFAIL=-506,/**<MODEMͬ��дʧ��*/
	NDK_ERR_MODEM_ASYNWRITEFAIL=-507,/**<MODEM�첽дʧ��*/
	NDK_ERR_MODEM_SDLCREADFAIL=-508,/**<MODEMͬ����ʧ��*/
	NDK_ERR_MODEM_ASYNREADFAIL=-509,/**<MODEM�첽��ʧ��*/
	NDK_ERR_MODEM_SDLCHANGUPFAIL=-510,/**<MODEMͬ���Ҷ�ʧ��*/
	NDK_ERR_MODEM_ASYNHANGUPFAIL=-511,/**<MODEM�첽�Ҷ�ʧ��*/
	NDK_ERR_MODEM_SDLCCLRBUFFAIL=-512,/**<MODEMͬ���建��ʧ��*/
	NDK_ERR_MODEM_ASYNCLRBUFFAIL=-513,/**<MODEM�첽�建��ʧ��*/

	NDK_ERR_ICC_WRITE_ERR =			-601,	/**< д����83c26����*/
	NDK_ERR_ICC_COPYERR=			-602,	/**<�ں����ݿ�������*/
	NDK_ERR_ICC_POWERON_ERR=		-603,	/**<�ϵ����*/
	NDK_ERR_ICC_COM_ERR=			-604,	/**<�������*/
	NDK_ERR_ICC_CARDPULL_ERR=		-605,	/**<���γ���*/
	NDK_ERR_ICC_CARDNOREADY_ERR=	-606,	/**<��δ׼����*/

	NDK_ERR_APP_BASE=(-800),							/**<Ӧ�ýӿڴ������*/
	NDK_ERR_APP_CFG_OPEN=(NDK_ERR_APP_BASE-1),			/**<�������ļ�����*/
	NDK_ERR_APP_CFG_GET=(NDK_ERR_APP_BASE-2),			/**<��ȡ�����ļ������*/
	NDK_ERR_APP_CFG_SET=(NDK_ERR_APP_BASE-3),			/**<д�����ļ������*/
	NDK_ERR_APP_NOT_EXIST=(NDK_ERR_APP_BASE-4),			/**<Ӧ�������*/
	NDK_ERR_APP_CFG_INIT=(NDK_ERR_APP_BASE-5),			/**<�������ļ�����*/
	NDK_ERR_APP_CFG_ALLOC=(NDK_ERR_APP_BASE-6),			/**<�����½ڵ����*/
	NDK_ERR_APP_CFG_DUMP=(NDK_ERR_APP_BASE-7),			/**<�������ļ�����*/
	NDK_ERR_APP_CFG_SECTION=(NDK_ERR_APP_BASE-8),		/**<*/
	NDK_ERR_APP_MAX_CHILD=(NDK_ERR_APP_BASE-9),			/**<��Ӧ���������������������Ŀ*/
	NDK_ERR_APP_CREAT_CHILD=(NDK_ERR_APP_BASE-10),		/**<�����ӽ��̴���*/
	NDK_ERR_APP_WAIT_CHILD=(NDK_ERR_APP_BASE-11),		/**<�ȴ��ӽ��̽�������*/
	NDK_ERR_APP_FILE_READ=(NDK_ERR_APP_BASE-12),		/**<���ļ�����*/
	NDK_ERR_APP_FILE_WRITE=(NDK_ERR_APP_BASE-13),		/**<д�ļ�����*/
	NDK_ERR_APP_FILE_STAT=(NDK_ERR_APP_BASE-14),		/**<��ȡ�ļ���Ϣ����*/
	NDK_ERR_APP_FILE_OPEN=(NDK_ERR_APP_BASE-15),		/**<�ļ��򿪴���*/
	NDK_ERR_APP_NLD_HEAD_LEN=(NDK_ERR_APP_BASE-16),		/**<NLD�ļ���ȡͷ��Ϣ���ȴ���*/
	NDK_ERR_APP_PUBKEY_EXPIRED=(NDK_ERR_APP_BASE-17),	/**<��Կ��Ч��*/
	NDK_ERR_APP_MMAP=(NDK_ERR_APP_BASE-18),				/**<�ڴ�ӳ�����*/
	NDK_ERR_APP_MALLOC=(NDK_ERR_APP_BASE-19),			/**<��̬�ڴ�������*/
	NDK_ERR_APP_SIGN_DECRYPT=(NDK_ERR_APP_BASE-20),		/**<ǩ�����ݽ�ǩ����*/
	NDK_ERR_APP_SIGN_CHECK=(NDK_ERR_APP_BASE-21),		/**<ǩ������У�����*/
	NDK_ERR_APP_MUNMAP=(NDK_ERR_APP_BASE-22),			/**<�ڴ�ӳ���ͷŴ���*/
	NDK_ERR_APP_TAR=(NDK_ERR_APP_BASE-23),				/**<tar����ִ��ʧ��*/

    NDK_ERR_SECP_BASE = (-1000),
    NDK_ERR_SECP_TIMEOUT = (NDK_ERR_SECP_BASE - 1),             /**<��ȡ��ֵ��ʱ*/
    NDK_ERR_SECP_PARAM = (NDK_ERR_SECP_BASE - 2),               /**<��������Ƿ�*/
    NDK_ERR_SECP_DBUS = (NDK_ERR_SECP_BASE - 3),                /**<DBUSͨѶ����*/
    NDK_ERR_SECP_MALLOC = (NDK_ERR_SECP_BASE - 4),              /**<��̬�ڴ�������*/
    NDK_ERR_SECP_OPEN_SEC = (NDK_ERR_SECP_BASE - 5),            /**<�򿪰�ȫ�豸����*/
    NDK_ERR_SECP_SEC_DRV = (NDK_ERR_SECP_BASE - 6),             /**<��ȫ�豸��������*/
    NDK_ERR_SECP_GET_RNG = (NDK_ERR_SECP_BASE - 7),             /**<��ȡ�����*/
    NDK_ERR_SECP_GET_KEY = (NDK_ERR_SECP_BASE - 8),             /**<��ȡ��Կֵ*/
    NDK_ERR_SECP_KCV_CHK = (NDK_ERR_SECP_BASE - 9),             /**<KCVУ�����*/
    NDK_ERR_SECP_GET_CALLER = (NDK_ERR_SECP_BASE - 10),         /**<��ȡ��������Ϣ����*/
    NDK_ERR_SECP_OVERRUM = (NDK_ERR_SECP_BASE - 11),            /**<���д�������*/

    NDK_ERR_SECVP_BASE = (-1100),                           /**<δ֪����*/
    NDK_ERR_SECVP_TIMEOUT = (NDK_ERR_SECVP_BASE - 1),       /**<��ȡ��ֵ��ʱ*/
    NDK_ERR_SECVP_PARAM = (NDK_ERR_SECVP_BASE - 2),         /**<��������Ƿ�*/
    NDK_ERR_SECVP_DBUS = (NDK_ERR_SECVP_BASE - 3),          /**<DBUSͨѶ����*/
    NDK_ERR_SECVP_OPEN_EVENT0 =	(NDK_ERR_SECVP_BASE - 4),   /**<��event0�豸����*/
    NDK_ERR_SECVP_SCAN_VAL = (NDK_ERR_SECVP_BASE - 5),      /**<ɨ��ֵ��������*/
    NDK_ERR_SECVP_OPEN_RNG = (NDK_ERR_SECVP_BASE - 6),      /**<��������豸����*/
    NDK_ERR_SECVP_GET_RNG = (NDK_ERR_SECVP_BASE - 7),       /**<��ȡ���������*/

    NDK_ERR_SECCR_BASE = (-1200),                           /**<δ֪����*/
    NDK_ERR_SECCR_TIMEOUT = (NDK_ERR_SECCR_BASE - 1),       /**<��ȡ��ֵ��ʱ*/
    NDK_ERR_SECCR_PARAM = (NDK_ERR_SECCR_BASE - 2),         /**<��������Ƿ�*/
    NDK_ERR_SECCR_DBUS = (NDK_ERR_SECCR_BASE - 3),          /**<DBUSͨѶ����*/
    NDK_ERR_SECCR_MALLOC = (NDK_ERR_SECCR_BASE - 4),        /**<��̬�ڴ�������*/
    NDK_ERR_SECCR_OPEN_RNG = (NDK_ERR_SECCR_BASE - 5),      /**<��������豸����*/
    NDK_ERR_SECCR_DRV = (NDK_ERR_SECCR_BASE - 6),           /**<�������ܴ���*/
    NDK_ERR_SECCR_KEY_TYPE = (NDK_ERR_SECCR_BASE - 7),      /**<��Կ���ʹ���*/
    NDK_ERR_SECCR_KEY_LEN = (NDK_ERR_SECCR_BASE - 8),       /**<��Կ���ȴ���*/
    NDK_ERR_SECCR_GET_KEY = (NDK_ERR_SECCR_BASE - 9),       /**<��ȡ��Կ����*/

    NDK_ERR_SECKM_BASE = (-1300),
    NDK_ERR_SECKM_TIMEOUT = (NDK_ERR_SECKM_BASE - 1),           /**<��ȡ��ֵ��ʱ*/
    NDK_ERR_SECKM_PARAM = (NDK_ERR_SECKM_BASE - 2),             /**<��������Ƿ�*/
    NDK_ERR_SECKM_DBUS = (NDK_ERR_SECKM_BASE - 3),              /**<DBUSͨѶ����*/
    NDK_ERR_SECKM_MALLOC = (NDK_ERR_SECKM_BASE - 4),            /**<��̬�ڴ�������*/
    NDK_ERR_SECKM_OPEN_DB = (NDK_ERR_SECKM_BASE - 5),           /**<���ݿ�򿪴���*/
    NDK_ERR_SECKM_DEL_DB = (NDK_ERR_SECKM_BASE - 6),            /**<ɾ�����ݿ����*/
    NDK_ERR_SECKM_DEL_REC = (NDK_ERR_SECKM_BASE - 7),           /**<ɾ����¼����*/
    NDK_ERR_SECKM_INSTALL_REC = (NDK_ERR_SECKM_BASE - 8),       /**<��װ��Կ��¼����*/
    NDK_ERR_SECKM_READ_REC = (NDK_ERR_SECKM_BASE - 9),          /**<����Կ��¼����*/
    NDK_ERR_SECKM_OPT_NOALLOW = (NDK_ERR_SECKM_BASE - 10),      /**<����������*/
    NDK_ERR_SECKM_KEY_MAC = (NDK_ERR_SECKM_BASE - 11),          /**<��ԿMACУ�����*/
    NDK_ERR_SECKM_KEY_TYPE = (NDK_ERR_SECKM_BASE - 12),         /**<��Կ���ʹ���*/
    NDK_ERR_SECKM_KEY_ARCH = (NDK_ERR_SECKM_BASE - 13),         /**<��Կ��ϵ����*/
    NDK_ERR_SECKM_KEY_LEN  = (NDK_ERR_SECKM_BASE - 14),         /**<��Կ���ȴ���*/

	NDK_ERR_RFID_INITSTA=			-2005,  /**<�ǽӴ���-��Ƶ�ӿ��������ϻ���δ����*/
	NDK_ERR_RFID_NOCARD=			-2008,  /**<�ǽӴ���-�޿�  0x0D*/
	NDK_ERR_RFID_MULTICARD=			-2009,  /**<�ǽӴ���-�࿨״̬*/
	NDK_ERR_RFID_SEEKING=			-2010,  /**<�ǽӴ���-Ѱ��/���������ʧ��*/
	NDK_ERR_RFID_PROTOCOL=			-2011,  /**<�ǽӴ���-��֧��ISO1444-4Э�飬��M1��  F*/

	NDK_ERR_RFID_NOPICCTYPE=		-2012,  /**<�ǽӴ���-δ���ÿ� 0x01*/
	NDK_ERR_RFID_NOTDETE=			-2013,  /**<�ǽӴ���-δѰ��   0x02*/
	NDK_ERR_RFID_AANTI=				-2014,  /**<�ǽӴ���-A����ͻ(���ſ�����)  0x03*/
	NDK_ERR_RFID_RATS=				-2015,  /**<�ǽӴ���-A��RATS���̳���   0x04*/
	NDK_ERR_RFID_BACTIV=			-2016,  /**<�ǽӴ���-B������ʧ��   0x07*/
	NDK_ERR_RFID_ASEEK=				-2017,  /**<�ǽӴ���-A��Ѱ��ʧ��(���ܶ��ſ�����)   0x0A*/
	NDK_ERR_RFID_BSEEK=				-2018,  /**<�ǽӴ���-B��Ѱ��ʧ��(���ܶ��ſ�����)   0x0B*/
	NDK_ERR_RFID_ABON=				-2019,  /**<�ǽӴ���-A��B��ͬʱ����   0x0C*/
	NDK_ERR_RFID_UPED=				-2020,  /**<�ǽӴ���-�Ѿ�����(�ϵ�)   0x0E*/
	NDK_ERR_RFID_NOTACTIV=			-2021,  /**<�ǽӴ���-δ����*/

	NDK_ERR_MI_NOTAGERR=			-2030,  /**<�ǽӴ���-�޿�,				0xff*/
	NDK_ERR_MI_CRCERR=				-2031,  /**<�ǽӴ���-CRC��,				0xfe*/
	NDK_ERR_MI_EMPTY=				-2032,  /**<�ǽӴ���-�ǿ�,				0xfd*/
	NDK_ERR_MI_AUTHERR=				-2033,  /**<�ǽӴ���-��֤��,			0xfc*/
	NDK_ERR_MI_PARITYERR=			-2034,  /**<�ǽӴ���-��ż��,			0xfb*/
	NDK_ERR_MI_CODEERR=				-2035,  /**<�ǽӴ���-���մ����			0xfa*/
	NDK_ERR_MI_SERNRERR=            -2036,  /**<�ǽӴ���-����ͻ����У���	0xf8*/
	NDK_ERR_MI_KEYERR=              -2037,  /**<�ǽӴ���-��֤KEY��			0xf7*/
	NDK_ERR_MI_NOTAUTHERR=          -2038,  /**<�ǽӴ���-δ��֤				0xf6*/
	NDK_ERR_MI_BITCOUNTERR=         -2039,  /**<�ǽӴ���-����BIT��			0xf5*/
	NDK_ERR_MI_BYTECOUNTERR=        -2040,  /**<�ǽӴ���-�����ֽڴ�			0xf4*/
	NDK_ERR_MI_WriteFifo=           -2041,  /**<�ǽӴ���-FIFOд����			0xf3*/
	NDK_ERR_MI_TRANSERR=            -2042,  /**<�ǽӴ���-���Ͳ�������		0xf2*/
	NDK_ERR_MI_WRITEERR=            -2043,  /**<�ǽӴ���-д��������			0xf1*/
	NDK_ERR_MI_INCRERR=				-2044,  /**<�ǽӴ���-������������		0xf0*/
	NDK_ERR_MI_DECRERR=             -2045,  /**<�ǽӴ���-������������		0xef*/
	NDK_ERR_MI_OVFLERR=             -2046,  /**<�ǽӴ���-�������			0xed*/
	NDK_ERR_MI_FRAMINGERR=          -2047,  /**<�ǽӴ���-֡��				0xeb*/
	NDK_ERR_MI_COLLERR=             -2048,  /**<�ǽӴ���-��ͻ				0xe8*/
	NDK_ERR_MI_INTERFACEERR=        -2049,  /**<�ǽӴ���-��λ�ӿڶ�д��		0xe6*/
	NDK_ERR_MI_ACCESSTIMEOUT=       -2050,  /**<�ǽӴ���-���ճ�ʱ			0xe5*/
	NDK_ERR_MI_PROTOCOLERR=			-2051,  /**<�ǽӴ���-Э���				0xe4*/
	NDK_ERR_MI_QUIT=                -2052,  /**<�ǽӴ���-�쳣��ֹ			0xe2*/
	NDK_ERR_MI_PPSErr=				-2053,  /**<�ǽӴ���-PPS������			0xe1*/
	NDK_ERR_MI_SpiRequest=			-2054,  /**<�ǽӴ���-����SPIʧ��		0xa0*/
	NDK_ERR_MI_NY_IMPLEMENTED=		-2055,  /**<�ǽӴ���-�޷�ȷ�ϵĴ���״̬	0x9c*/
	NDK_ERR_MI_CardTypeErr=			-2056,  /**<�ǽӴ���-�����ʹ�			0x83*/
	NDK_ERR_MI_ParaErrInIoctl=		-2057,  /**<�ǽӴ���-IOCTL������		0x82*/
	NDK_ERR_MI_Para=				-2059, /**<�ǽӴ���-�ڲ�������			0xa9*/
}EM_NDK_ERR;

/** @} */ // �������������

/** @addtogroup ��ʾ
* @{
*/


typedef unsigned int color_t;/**<RGBɫ����ֵ,0(��ɫ) - 0xFFFF(��ɫ)*/

/**
 *@brief ��ʾģʽ����
*/
typedef enum {
	TEXT_ATTRIBUTE_NORMAL = 1<<0,			/**<����������ʾ*/
	TEXT_ATTRIBUTE_REVERSE = 1<<1,		/**<���巴����ʾ*/
	TEXT_ATTRIBUTE_UNDERLINE = 1<<2,		/**<������»���*/
}EM_TEXT_ATTRIBUTE;

/**
 *@brief �������
*/
typedef enum {
	BACKLIGHT_OFF ,			/**<����Һ������*/
	BACKLIGHT_ON,			/**<�ر�Һ��*/
	BACKLIGHT_LOCKON,		/**<��ֹҺ�������Զ��ر�*/
}EM_BACKLIGHT;

/**
 *@brief ϵͳ����ѡ����
*/
typedef enum {
	DISPFONT_CUSTOM,			/**<ϵͳĬ�ϵ���ʾ����*/
	DISPFONT_EXTRA,			/**<����ߴ����ʾ����*/
	DISPFONT_USER,				/**<�û��Զ�������*/
}EM_DISPFONT;

/**
 *@brief ϵͳ������ɫ���ö���
*/
typedef enum {
	FONTCOLOR_NORMAL,				/**<����������ʾʱ������ɫ*/
	FONTCOLOR_REVERSE,				/**<���巴����ʾʱ������ɫ*/
	FONTCOLOR_BG_REVERSE,				/**<���巴����ʾʱ������ɫ*/
}EM_FONTCOLOR;

/**
 *@brief �������ģʽ����
*/
typedef	enum  {
    RECT_PATTERNS_NO_FILL,        /**<����䣬ֻ���Ʊ߿�*/
    RECT_PATTERNS_SOLID_FILL      /**<���ģʽ*/
}EM_RECT_PATTERNS;

/**
 *@brief	�û���������ʼ����
 *@details  �����ڳ���������ã��ú����ɹ����ú���ʾģ���API��������ʹ�á�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrInitGui(void);/**	*@example NDK_disp_example.c**/


/**
 *@brief	��ȡ��ʾģ��汾��
 *@retval	pszVer	����ģ��汾,�����pnVerӦ�ò�С��16�ֽڡ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetVer(char *pszVer);

/**
 *@brief	������ʾģʽ������ȡ֮ǰ����ʾģʽ��
 *@param	emNewattr 	Ҫ���õ�����ʾģʽ��
 *@retval	pemOldattr 	���֮ǰ����ʾģʽ��peOldattrΪNULLʱ�����ء�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetAttr(EM_TEXT_ATTRIBUTE emNewattr, EM_TEXT_ATTRIBUTE *pemOldattr);

/**
 *@brief	���浱ǰ��Ļ
 *@details	������ʾ���ݡ����λ�ü���ʾģʽ���ñ������ɵ���\ref NDK_ScrPop "NDK_ScrPop"���ٻָ���ʾ��
			NDK_ScrPush��\ref NDK_ScrPop "NDK_ScrPop"�ɶ�ʹ�ã�����Ƕ�ס�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrPush(void);

/**
 *@brief	���ٻָ�����\ref NDK_ScrPush "NDK_ScrPush"�������ʾ״̬��������ʾ���ݡ����λ�ü��ı���ʾ���ԡ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrPop(void);

/**
 *@brief	���ñ���ͼƬ��ͼƬ�ļ�֧�ָ�ʽ��鿴�����͵�ͼƬ��ʽ���ơ�
 *@param	pszfilepath 	ͼƬ�ļ�·��+�ļ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetbgPic(char *pszfilepath);

/**
 *@brief	ȡ������ͼƬ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrClrbgPic(void);

/**
 *@brief	�������ѹ���Ƶ���������(0,0)��ͬʱ����Ļ��ʾģʽ����ΪTEXT_ATTRIBUTE_NORMAL��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrClrs(void);

/**
 *@brief	���������(��λ������)���ѹ���Ƶ�(0,unStartY)����
		  	������������ӿڱ߽�ʱ�����ӿڱ߽�Ϊ׼��
 *@param	unStartY		��ʼ�кţ������꣬��λ�����أ�����0��ʼ����
 *@param	unEndY			�����кţ������꣬��λ�����أ�����0��ʼ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrClrLine(uint unStartY,uint unEndY);


/**
 *@brief	Һ����ʾ���λ���Ƶ���������(unX,unY)����
			�����������Ƿ������걣��λ�ò���,���ش�����Ϣ��
 *@param	unX 	�����꣨��λ�����أ�
 *@param	unY 	�����꣨��λ�����أ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGotoxy(uint unX, uint unY);


/**
 *@brief	��ȡ��ǰ���ع��λ�õĺ�����������ꡣ
 *@retval	punX ���ع��λ�õĺ����꣨��λ�����أ���
 *@retval	punY ���ع��λ�õ������꣨��λ�����أ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetxy(uint *punX, uint *punY);


/**
 *@brief	������ʾ����ߴ硣
 *@details  δ�����������ʾ����Ϊʵ����Ļ�ߴ磬ͨ���ýӿ�������ʾ���������API����ʾ����ֻ�ڸ���������Ч��\n
			������(10,10,100,100)ΪӦ����ʾ������Ӧ�ó�����ʹ�õ���������(0,0)ʵ����
			����Ļ��������(10,10),��������Ҳֻ�����������(10,10,100,100)��Χ�ڵ���ʾ���ݡ�
 *@param	unX		Ӧ�ó�����ʾ������ʼ����ĺ����꣨��λ�����أ���
 *@param	unY		Ӧ�ó�����ʾ������ʼ����ĺ����꣨��λ�����أ���
 *@param	unWidth	Ӧ�ó�����ʾ�����ȣ���λ�����أ���
 *@param	unHeight 	Ӧ�ó�����ʾ����߶ȣ���λ�����أ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetViewPort(uint unX,uint unY,uint unWidth, uint unHeight);

/**
 *@brief	��ȡ��ǰ��ʾ����ߴ硣
 *@retval	punX		��ʾ������ʼ����ĺ����꣨��λ�����أ���
 *@retval	punY		��ʾ������ʼ����������꣨��λ�����أ���
 *@retval	punWidth	��ʾ����߶ȣ���λ�����أ���
 *@retval	punHeight	��ʾ����߶ȣ���λ�����أ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetViewPort(uint *punX,uint *punY,uint *punWidth,uint *punHeight);


/**
 *@brief	����ʾ������ʾBitmapͼƬ��
 *@details  bitmap��ʽ��1byte��Ӧ8�����ص�,0��ʾ�׵㣬1��ʾ�ڵ㣬��ʾ���ݺ������У�����ͼ��ʾ:\n
-----------------D7~~D0--------------D7~~D0------------------\n
Byte 1: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte2	\n
Byte 3: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte4	\n
Byte 5: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte6	\n
Byte 7: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte8	\n
Byte 9: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte10	\n
Byte11: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte12	\n
Byte13: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte14	\n
Byte15: �� �� �� �� �� �� �� ��  ��  �� �� �� �� �� �� �� Byte16	\n
---------------------------------------------------------------\n
	�����ʾͼƬ��Χ������Ļ��Χ��ͨ��\ref NDK_ScrSetViewPort "NDK_ScrSetViewPort()"���õ��û�ʹ������ʱ����ú���������Ч�����ص���ʧ�ܡ�
 *@param	unX 		ͼƬ����ʾ��������ϽǺ����꣨��λ�����أ�
 *@param	unY 		ͼƬ����ʾ��������Ͻ������꣨��λ�����أ�
 *@param	unWidth 	ͼƬ��ȣ���λ�����أ�
 *@param	unHeight 	ͼƬ�߶ȣ���λ�����أ�
 *@param	psBuf 		BitmapͼƬ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrDrawBitmap(uint unX,uint unY,uint unWidth, uint unHeight, const char *psBuf);


/**
 *@brief	����ʾ��������������(unStartX,unStartY)��(unEndX,unEndY)��ֱ�ߣ�unColor��ʾ���ߵ�RGBɫ��ֵ��
 *@param	unStartX 	ֱ�ߵ��������꣨��λ�����أ�
 *@param	unStartY 	ֱ�ߵ���������꣨��λ�����أ�
 *@param	unEndX 		ֱ�ߵ��յ�����꣨��λ�����أ�
 *@param	unEndY 		ֱ�ߵ��յ������꣨��λ�����أ�
 *@param	unColor 		��ɫ��ֵ <0-0xFFFF>
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrLine(uint unStartX, uint unStartY, uint unEndX, uint unEndY, color_t unColor);


/**
 *@brief	����ʾ����һ�����Ρ�
 *@details	������α߽糬����Ļ��Χ��ͨ��\ref NDK_ScrSetViewPort "NDK_ScrSetViewPort()"���õ��û�ʹ������ʱ����ú���������Ч�����ص���ʧ�ܡ�
 *@param	unX 		���ε��������꣨��λ�����أ�
 *@param	unY 		���ε���������꣨��λ�����أ�
 *@param	unWidth 		���εĿ���λ�����أ�
 *@param	unHeight 		���εĸߣ���λ�����أ�
 *@param	emFill_pattern 	0Ϊ���ģʽ��1Ϊ���ģʽ
 *@param	unColor	��ɫ��ֵ <0-0xFFFF>
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrRectangle(uint unX, uint unY, uint unWidth, uint unHeight, EM_RECT_PATTERNS emFill_pattern, color_t unColor);


/**
 *@brief	����ʾ������ʾָ����ͼƬ��ͼƬ�ļ�֧�ָ�ʽ��鿴�����͵�ͼƬ��ʽ���ơ�
 *@details	�����ʾͼƬ��Χ������Ļ��Χ��ͨ��\ref NDK_ScrSetViewPort "NDK_ScrSetViewPort()"���õ��û�ʹ������ʱ����ú���������Ч�����ص���ʧ�ܡ�
 *@param	unX 		ͼƬ��ʾ�����ϽǺ����꣨��λ�����أ�
 *@param	unY 		ͼƬ��ʾ�����Ͻ������꣨��λ�����أ�
 *@param	unWidth 	ͼƬ�Ŀ���λ�����أ�
 *@param	unHeight 	ͼƬ��ʾ�ĸߣ���λ�����أ�
 *@param	pszPic 	ͼƬ�ļ����ڵ�·����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrDispPic(uint unX,uint unY,uint unWidth, uint unHeight, const char *pszPic);

/**
 *@brief	ȡ��ʾ������ָ��������������ɫ��ֵ��
 *@param	unX 		�����꣨��λ�����أ�
 *@param	unY 		�����꣨��λ�����أ�
 *@retval	punColor	���ص���ɫֵ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetPixel(uint unX, uint unY, color_t *punColor);


/**
 *@brief	����ʾ������ָ���������껭�㡣
 *@param	unX 		�����꣨��λ�����أ�
 *@param	unY		�����꣨��λ�����أ�
 *@param	unColor 	��ɫ��ֵ <0-0xFFFF>
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrPutPixel(uint unX, uint unY, color_t unColor);


/**
 *@brief	���Դ����û����õ���ʾ�����ڵ�����ˢ�µ�Һ��������ʾ��
 *@details	ϵͳȱʡΪ�Զ�ˢ�¡�Ϊ��������������ͨ��\ref NDK_ScrAutoUpdate "NDK_ScrAutoUpdate()"����Ϊ���Զ�ˢ�£���NDK_ScrRefresh
			���ú�ϵͳ�Ž��Դ��е�����ˢ�µ�Һ�����ϡ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrRefresh(void);

/**
 *@brief	���Դ���ȫ������ˢ�µ�LCDȫ������ʾ��
 *@details	�ýӿ���\ref NDK_ScrRefresh "NDK_ScrRefresh()"�������ڲ���������ʾ����Ĵ�С����ͨ��NDK_ScrSetViewPort���õ���ʾ����Ϊȫ��ʱ
			����\ref NDK_ScrRefresh "NDK_ScrRefresh()"��ýӿ�Ч��һ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrFullRefresh(void);

/**
 *@brief	�����Ƿ��Զ�ˢ�¡�
 *@param	nNewauto
					��0:�Զ�ˢ��
					0:���Զ�ˢ�£�ֻ�е���\ref NDK_ScrRefresh "NDK_ScrRefresh()"����ʾ�Դ��е����ݡ�
 *@retval	pnOldauto	��������֮ǰ���Զ�ˢ��״̬��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrAutoUpdate(int nNewauto, int *pnOldauto);


/**
 *@brief	��ȡҺ�����ߴ硣
 *@retval		punWidth	����LCD��ȣ���λ�����أ���
 *@retval		punHeight	����LCD�߶ȣ���λ�����أ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetLcdSize(uint *punWidth,uint *punHeight);

/**
 *@brief	��ȡҺ����ɫ�
 *@details	�������ж�Һ�����ǵ�ɫ�������
 *@retval		puncd	����Һ����ɫ�1----�ڰ���ɫ,
										16----16λɫ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetColorDepth(uint *puncd);

/**
 *@brief	���ر��������
 *@param	emBL	BACKLIGHT_OFF �C �ر�Һ������
 					BACKLIGHT_ON �C��Һ������
 					BACKLIGHT_LOCKON �CҺ�����ⳣ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrBackLight(EM_BACKLIGHT emBL);

/**
 *@brief		������Ļ�Աȶȡ�
 *@param		unContrast	�Աȶȼ���Ϊ0~63��0���63���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetContrast(uint unContrast);


/**
 *@brief	����ʹ�õ���ʾ���塣
 *@details  ϵͳĬ��֧��2�ֳߴ�����С������ɹ��л�,��ͬ�����趨��ϵͳĬ������ߴ粻һ����ͬ\n\n
			����֧��:����16x16 ASCII:8x16 (DISPFONT_CUSTOM)\n
					 ����24x24 ASCII:12x24 (DISPFONT_EXTRA)\n
			�ڰ���֧��:����12x12 ASCII:6x12 (DISPFONT_CUSTOM)\n
						����16x16 ASCII:8x16 (DISPFONT_EXTRA)\n\n
			������ʾ����Ժ�����ʾ���������ã���֮ǰ��ˢ����ʾ��������Ч
 *@param	emType	ѡ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetFontType(EM_DISPFONT emType);


/**
 *@brief	��ȡ��ǰϵͳʹ�õĺ�����ʾ�����͸ߡ�
 *@details  ϵͳʹ�õ�ASCII�ַ���������ȹ̶�Ϊ���ֵ�һ��
 *@retval	punWidth	���ص�ǰϵͳ��ǰ��ʾ����ĺ��ֵ����
 *@retval	punHeight	���ص�ǰϵͳ��ǰ��ʾ����ĺ��ֵ���ߡ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrGetFontSize(uint *punWidth,uint *punHeight);


/**
 *@brief	�����м����ּ�ࡣ
 *@details  wspace���ڵ�ǰʹ�õ�Ӣ���������ؿ�ȵ�2��ʱ���ּ����ΪӢ���������ؿ�ȵ�2��
			hspace���ڵ�ǰʹ�õ�Ӣ���������ظ߶�ʱ���м����ΪӢ���������ظ߶�
 *@param	unWspace	�ּ�ࣨ��λ�����أ�
 *@param	unHpace	�м�ࣨ��λ�����أ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetSpace(uint unWspace,uint unHpace);


/**
 *@brief	����������ɫ���������ԡ����ԡ����Ա���ɫ��
 *@param	unColor	��ɫ��ֵ
 *@param	emType	ѡ�����ö���

 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrSetFontColor(color_t unColor, EM_FONTCOLOR emType);



/**
 *@brief	��ʾ�ַ�����
 *@param	unX	��ʾ�ַ���λ�õĺ�����
 *@param	unY	��ʾ�ַ���λ�õ�������
 *@param	pszS	Ҫ��ʾ���ַ���ָ��
 *@param	unMode	������ʾASCII�ַ�ʱ�ߴ�
					1��ʹ��С��Ӣ�����壬�������ڰ������ֵ��ַ���
						�ڰ�����8x8�ߴ�Ӣ������
						������8x16�ߴ�Ӣ������
					0��ϵͳ��ǰʹ�õ���ʾ������ASCII����ߴ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrDispString(uint unX,uint unY,const char *pszS,uint unMode);

/**
 *@brief	��Ļ��ʾ��ʽ�������ʹ�÷���ͬprintf
 *@param	format	��������ĸ�ʽ
  *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ScrPrintf(const char *format, ...);
/** @} */ // ��ʾģ�����

/** @addtogroup ����
* @{
*/

/**
 *@brief �������
*/
typedef enum {
	INPUT_CONTRL_NOLIMIT, 		/**<����������ASCII���ַ���������ֱ�ӷ���*/
	INPUT_CONTRL_LIMIT,		/**<ֻ��������������С���㣬������ֱ�ӷ���*/
	INPUT_CONTRL_NOLIMIT_ERETURN,		/**<����������ASCII���ַ���������ȴ�ȷ�ϼ�*/
	INPUT_CONTRL_LIMIT_ERETURN,			/**<ֻ��������������С���㣬������ȴ�ȷ�ϼ�����*/
}EM_INPUT_CONTRL;

/**
 *@brief �����ַ���ʾ����
*/
typedef enum {
	INPUTDISP_NORMAL, 		/**<��������������ʾ�ַ�*/
	INPUTDISP_PASSWD,			/**<����������ʾΪ'*' */
	INPUTDISP_OTHER,			/**<֧�ִ�Ԥ��ֵ�Ľ������뻺��*/
}EM_INPUTDISP;

/**
 *@brief ���뷨����
*/
typedef enum {
    IME_NUMPY,	/**<��ƴ*/
    IME_ENGLISH, 	/**<Ӣ��*/
	IME_NUM,	/**<����*/
	IME_BIHUA,	/**<�ʻ�*/
    IME_QUWEI,	/**< ��λ*/
    IME_MAXNUM=IME_QUWEI,	/**<�����ڴ����ж�*/
}EM_IME;

/**
 *@brief	������̻�������
 *@return
 *@li		NDK_OK			   �����ɹ�
 *@li		����EM_NDK_ERR    ����ʧ��
*/
int NDK_KbFlush(void);

/**
 *@brief	���س���������ϼ����ܡ�
 *@param	nSelect 	0   �ر� 1   ����
 *@param	nMode 		0   ������  1   ��ϼ�
 *@param	pnState 	��ȡ����������ϼ�ԭ����״̬��0--�ر� 1---������
 *@return
 *@li		NDK_OK				  �����ɹ�
 *@li		����EM_NDK_ERR    ����ʧ��
*/
int NDK_KbSwitch(int nSelect, int nMode,int *pnState);

/**
 *@brief 	��ȡ����������ϼ��Ŀ���״̬��
 *@param	nMode 	0   ������  1   ��ϼ�
 *@param	pnState 	��ȡ����������ϼ�״̬��0--�ر� 1---������
 *@return
 *@li		NDK_OK 				   �����ɹ�
 *@li		����EM_NDK_ERR	   ����ʧ��
*/
int NDK_KbGetSwitch(int nMode,int *pnState);

/**
 *@brief 	��ʱʱ���ڶ�ȡ���̰���ֵ
 *@details	�ڹ涨��ʱ�����������������������:����һ�������ȴ��ſ������ؼ��롣
 *@param	unTime	С�ڵ���0 :�޳�ʱ��һֱ�ȴ�������
							����:Ϊ�ȴ�ʱ��(����Ϊ��λ)
 *@param	pnCode	��ȡ������룬���ڹ涨��ʱ����û�а������£�pnCode��ֵΪ0
 *@li       NDK_OK 				   �����ɹ�
 *@li   	����EM_NDK_ERR	   ����ʧ��
*/
int NDK_KbGetCode(uint unTime, int *pnCode);

/**
 *@brief	��ȡ�������е��׸����̼�ֵ����������
 *@details	��鰴���������Ƿ��а��������ж��������ؼ���,��û�м�������������0��
   			һ���API����һ������ѭ����ʹ�ã�����ʹ��֮ǰӦ��\ref NDK_KbFlush "NDK_KbFlush"�ѻ����������
 			��\ref NDK_KbGetCode "NDK_KbGetCode"�����ڱ����������еȴ��������������ء�
 *@param	pnCode	��ȡ������룬�ް�������ʱpnCode��ֵΪ0
 *@return
 *@li        	NDK_OK 				   �����ɹ�
 *@li   		����EM_NDK_ERR	   ����ʧ��
*/
int NDK_KbHit(int *pnCode);

/**
*@brief		�����ַ���
*@details	�Ӽ��̶���һ���Ի��з�Ϊ�ս�����ַ�����������뻺����pszBuf�С�
			ESC�����ز���ʧ��,�س�������ɷ���,�������ܼ���Ч��
*@param		pszBuf	�����ַ�������
*@param		unMin	��С���봮��
*@param		unMaxlen	������봮��
*@param		punLen	��ȡʵ�����봮�ĳ���(>0)
*@param		emMode	��ʾ���ͣ�
					ȡֵINPUTDISP_NORMALʱ��ʾ�ַ���
					ȡֵINPUTDISP_PASSWDʱ��ʾ'*'��
					ȡֵΪINPUTDISP_OTHERʱ��pcBuf�������ݣ���\0Ϊ��β���ַ������൱���Ѿ��Ӽ��������������,������������ʾ������
*@param		unWaittime	�ȴ������ʱ�䣬����0һֱ�ȴ�������Ϊ�ȴ�������������ʱû�а��»س������Զ����أ�����TimeOut��
*@param		emControl	INPUT_CONTRL_NOLIMIT������ASCII���ַ���������ֱ�ӷ���
						INPUT_CONTRL_LIMIT��ֻ��������С���㣬������ֱ�ӷ���
						INPUT_CONTRL_NOLIMIT_ERETURN������ASCII���ַ���������ȴ�ȷ�ϼ�����
						INPUT_CONTRL_LIMIT_ERETURN��ֻ��������С���㣬������ȴ�ȷ�ϼ�����
 *@return
 *@li        	NDK_OK 			   �����ɹ�
 *@li   		����EM_NDK_ERR	   ����ʧ��

*/
int NDK_KbGetInput(char *pszBuf,uint unMin,uint unMaxlen,uint *punLen,EM_INPUTDISP emMode,uint unWaittime, EM_INPUT_CONTRL emControl);

/**
 *@brief	�������뷨
 *@details 	ͨ��������ĸ����ѡ�����뷨��
			���벽�裺
			a. ��ƴ��������ֱ������ƴ���硰xin�����롰946������ѡ�񡣰����˸�������룬����������������ƶ�ƴ��ѡ��
			b. ��ȷ�ϡ����뱸ѡ��������ѡ����Ҫ�ĺ��֣�������������������ơ���������˸񡱼����˻ء�a�����衣
    					����ȷ�ϡ���ѡ����Ҫ�ĺ��֡�
			c. �����뷨״̬�£���ѡ�ֵ�ʱ�򣬿���ͨ������������л���
	 		���������룺
				�����ӿ������뷨������£������ּ���0�����ֱ����ţ�����������������ƶ�ѡ����ţ���ȷ�ϼ���ѡ�����ţ�

 *@param	pszS 		���������ַ�����pcS�������ݣ���\0Ϊ��β���ַ������൱���Ѿ��Ӽ�������������ݡ�
 *@param	unMaxlen 	���������ַ�������󳤶ȡ�
 *@param	emMethod		���뷨ѡ��,����emMethodȡEM_IME֮�������ֵ����ú���Ĭ�ϼ�����ƴ���뷨��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_KbHZInput(char *pszS,uint unMaxlen,EM_IME emMethod);

/** @} */ // ����ģ�����


/** @addtogroup ��ӡ
* @{
*/

/**
 *@brief ���嶨��ֵ
*/
typedef enum {
	PRN_HZ_FONT_24x24 = 1,
	PRN_HZ_FONT_16x32 ,
	PRN_HZ_FONT_32x32 ,
	PRN_HZ_FONT_32x16 ,
	PRN_HZ_FONT_24x32 ,
	PRN_HZ_FONT_16x16 ,
	PRN_HZ_FONT_12x16 ,
	PRN_HZ_FONT_16x8 ,
	PRN_HZ_FONT_24x24A ,			/**<��������24x24����*/
	PRN_HZ_FONT_24x24B ,			/**<����24x24����*/
	PRN_HZ_FONT_24x24C ,			/**<����24x24����*/
	PRN_HZ_FONT_24x24USER ,
	PRN_HZ_FONT_12x12A ,			/**<����12x12����*/
	PRN_HZ_FONT_48x24A ,
	PRN_HZ_FONT_48x24B ,
	PRN_HZ_FONT_48x24C ,
	PRN_HZ_FONT_24x48A ,
	PRN_HZ_FONT_24x48B ,
	PRN_HZ_FONT_24x48C ,
	PRN_HZ_FONT_48x48A ,
	PRN_HZ_FONT_48x48B ,
	PRN_HZ_FONT_48x48C
}EM_PRN_HZ_FONT;

typedef enum {
	PRN_ZM_FONT_8x16 = 1,
	PRN_ZM_FONT_16x16 ,
	PRN_ZM_FONT_16x32 ,
	PRN_ZM_FONT_24x32 ,
	PRN_ZM_FONT_6x8 ,
	PRN_ZM_FONT_8x8 ,
	PRN_ZM_FONT_5x7 ,
	PRN_ZM_FONT_5x16 ,
	PRN_ZM_FONT_10x16 ,
	PRN_ZM_FONT_10x8 ,
	PRN_ZM_FONT_12x16A ,       /**<MSGothic����12x16����*/
	PRN_ZM_FONT_12x24A ,				/**<Gulimche����12x24����*/
	PRN_ZM_FONT_16x32A ,				/**<MSGothic����16x32����*/
	PRN_ZM_FONT_12x16B ,				/**<MSGothic����12x16����*/
	PRN_ZM_FONT_12x24B ,				/**<MSGothic����12x24����*/
	PRN_ZM_FONT_16x32B ,				/**<MSGothic����16x32����*/
	PRN_ZM_FONT_12x16C ,				/**<�������12x16����*/
	PRN_ZM_FONT_12x24C ,				/**<�������12x24����*/
	PRN_ZM_FONT_16x32C ,				/**<�������16x32����*/
	PRN_ZM_FONT_24x24A ,
	PRN_ZM_FONT_32x32A ,
	PRN_ZM_FONT_24x24B ,
	PRN_ZM_FONT_32x32B ,
	PRN_ZM_FONT_24x24C ,
	PRN_ZM_FONT_32x32C ,
	PRN_ZM_FONT_12x12
}EM_PRN_ZM_FONT;

/**
 *@brief  ��ӡ��״̬�Լ������壬ȡ��ӡ��״̬����ֵ���������������ϵĹ�ϵ
*/

typedef enum{
	PRN_STATUS_OK = 0,			/**<��ӡ������*/
	PRN_STATUS_BUSY = 8,			/**<��ӡ�����ڴ�ӡ*/
	PRN_STATUS_NOPAPER = 2,       /**<��ӡ��ȱֽ*/
	PRN_STATUS_OVERHEAT = 4,      /**<��ӡ������*/
	PRN_STATUS_VOLERR = 112       /**<��ӡ����ѹ�쳣*/
}EM_PRN_STATUS;

/**
 *@brief ��ӡ������ö������ֵ
*/

typedef enum {
    PRN_TYPE_TP = 0,             /**<������ӡ��*/
    PRN_TYPE_HIP,              /**<�������*/
    PRN_TYPE_FIP,              /**<Ħ�����*/
    PRN_TYPE_END               /**<��*/
}EM_PRN_TYPE;

/**
 *@brief  ��ӡ����ӡģʽ
*/
typedef enum{
	PRN_MODE_ALL_DOUBLE = 0,			/**<����Ŵ�����Ŵ�*/
	PRN_MODE_WIDTH_DOUBLE ,			/**<����Ŵ���������*/
	PRN_MODE_HEIGHT_DOUBOE,       /**<��������������Ŵ�*/
	PRN_MODE_NORMAL               /**<������������������*/
}EM_PRN_MODE;


/**
 *@brief �»��߹��ܿ��ص�ö������ֵ
*/
typedef enum{
	PRN_UNDERLINE_STATUS_OPEN = 0,			/**<�»��߹��ܿ�*/
	PRN_UNDERLINE_STATUS_CLOSE			   /**<�»��߹��ܹ�*/
}EM_PRN_UNDERLINE_STATUS;

/**
 *@brief �ֿ�ע����Ϣ
*/
typedef struct
{
	int nOffset;						/**<��Ҫ��ȡ�������ֿ��е�������*/
	int nFontByte;					   /**<��Ҫ��ȡ�������ֿ���ռ�ֽ���*/
}ST_PRN_RECMSG;


/**
 *@brief �»��߹��ܿ��ص�ö������ֵ
*/
typedef struct
{
	char *pszName;	    			/**<�ֿ������ֿ�����·��*/
	int  nNum;				    	/**<ע���,֧��0~7ע���*/
	int  nDirection;   				/**<0:�������壻 1:��������*/
	int  w;							/**<������ʾ�Ŀ��*/
	int  h;							/**<������ʾ�ĸ߶�*/
	int  (*func)(char *, ST_PRN_RECMSG *);	/**<��ȡ���Ƶ�ַ��ռ���ֽ����ĺ���*/
	int  nIsHZ;                     /**<ע����ֿ��Ǻ����ֿ⻹��ASCII�ֿ⣬1:HZ  0:ZM*/
}ST_PRN_FONTMSG;


/**
 *@brief 		  ��ӡ����ʼ��
 *@details  	�����建����,���ô�ӡ����(�������塢�߾��ģʽ��)��
 *@param      unPrnDirSwitch  �Ƿ������ͱߴ��ǵ���NDK_PrnStart��ʼ��ӡ���ء�
              0--�رձ��ͱߴ���(Ĭ��)
	              �ڸ�ģʽ�£����е�NDK_PrnStr,NDK_PrnImage����ɵ���ת�����������������ݴ浽���ݻ�������
	              �ڵ���NDK_PrnStart֮��ſ�ʼ���кʹ�ӡ��صĹ�����������ֽ�ʹ�ӡ��
              1--�������ͱߴ���
	              �ڸ�ģʽ�£�ֻҪ��һ�����ݣ��ͻ���������ӡ������NDK_PrnStart�����κ�Ч����ֱ�ӷ��ء�
	              ����NDK_PrnFeedByPixel����������ֽ���ء����ڹر�ģʽ�¸ò�������NDK_PrnStart֮����С�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnInit(uint unPrnDirSwitch);
/**
 *@brief 		��ӡ�ַ���
 *@details 		�ú�������ת�����д�ӡ���ַ��������󻺳�������ӡ�����ڵ���Start֮��ʼ������ӡ���ú���Ϊ�����������
 *@param		pszBuf Ϊ��\0Ϊ��β�Ĵ�,�������ݿ�ΪASC�룬���� ����\n��\r(��ʾ�������У����ڿ�����ֱ�ӽ�ֽ)��
 				��pszBuf�����к��ֺ�ASCII�Ļ�ϴ�ʱ,��ĸ�ͺ���ֻ�����һ�������йء�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnStr(const char *pszBuf);
/**
 *@brief 		��ʼ������ӡ.
 *@details 	NDK_PrnStr��NDK_PrnImage�����������ת���ɵ���洢���������й��������øú�����ʼ������ӡ��
			 			����NDK_PrnStart��ӡ������Ҫ�жϷ���ֵ�Ƿ�Ϊ0���������-1��˵�����ӡ����ʧ�ܣ����������ش�ӡ��״ֵ̬�������м�������������
			 			NDK_PrnStart��ӡ����֮��������ȴ����ش�ӡ��״̬��ֵ��Ӧ�ÿɸ���NDK_PrnStart���ص�ֵ���жϴ�ӡ��״̬�Ƿ�������
			 			(������صķǴ�ӡ��״ֵ̬����NDK_OK��������ϵͳ����ʱ��ҪӦ��ȥȡ��ӡ��״̬���ÿ����ԱȽ�С)
 *@return
 *@li	NDK_OK				��ӡ������ȡ��ӡ��״̬����
 *@li	EM_NDK_ERR		����ϵͳ����(��NDK_ERR_OPEN_DEV����NDK_ERR_MACLLOC)
*/
int NDK_PrnStart(void);
/**
 *@brief 		��ӡͼ��(�ú���Ҳ��ת����ӡ���󵽵��󻺳���������NDK_PrnStart��ʼ��ӡ)
 *@details  	�����������384���㡣���xsize��xpos���֮�ʹ�������������ƻ᷵��ʧ�ܣ�����Ǻ���Ŵ�ģʽ�Ļ����ܳ���384/2��
 *@param 		unXsize ͼ�εĿ�ȣ����أ�
 *@param 		unYsize ͼ�εĸ߶ȣ����أ�
 *@param 		unXpos  ͼ�ε����Ͻǵ���λ�ã��ұ�������xpos+xsize<=ndk_PR_MAXLINEWIDE������ģʽΪ384������Ŵ�ʱΪ384/2��
 *@param 		psImgBuf ͼ���������,Ϊ�������У���һ���ֽڵ�һ�е�ǰ8���㣬D7Ϊ��һ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnImage(uint unXsize,uint unYsize,uint unXpos,const char *psImgBuf);
/**
 *@brief 		���ڻ�ȡ����ӡ������
 *@retval   pemType ���ڷ��ش�ӡ�����͵�ֵ�������£�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnGetType(EM_PRN_TYPE *pemType);
/**
 *@brief 		ȡ��ӡ�����İ汾��Ϣ.
 *@retval   pszVer ���ڴ洢���ذ汾�ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnGetVersion(char *pszVer);
/**
 *@brief 		���ô�ӡ����
 *@details  ����ASCII��ӡ����ͺ������塣Ӧ�ò�ɲο��ײ��Ӧ�ò�Ľӿ��ļ��е���ض��塣
 *@param 	emHZFont	���ú��������ʽ
 *@param    emZMFont	����ASCII�����ʽ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetFont(EM_PRN_HZ_FONT emHZFont,EM_PRN_ZM_FONT emZMFont);
/**
 *@brief		��ȡ��ӡ��״ֵ̬.
 *@details		�ڴ�ӡ֮ǰ��ʹ�øú����жϴ�ӡ���Ƿ�ȱֽ��
 *@retval	    pemStatus ���ڷ��ش�ӡ��״ֵ̬(����������������߶��ֵ���Ĺ�ϵ)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnGetStatus(EM_PRN_STATUS *pemStatus);
/**
 *@brief�� 	���ô�ӡģʽ.
 *@param 	emMode ��ӡģʽ(Ĭ����ʹ������ģʽ)
 *@param     unSigOrDou ��ӡ��˫��ѡ��0--���� 1--˫��(ֻ�������Ч����������)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetMode(EM_PRN_MODE emMode,uint unSigOrDou);
/**
 *@brief  	������ͼ�ε��Ű��ӡ.(��δʵ��)
 *@param    unXsize ͼ����(����)�����ܴ���384��������8��������
 *@param    unYsize ͼ��߶�(����)�����ܴ���384��������8��������
 *@param    unXpos  ͼ��ĺ���ƫ��λ�ã�(xpos+xsize)���ܴ���384
 *@param    unYpos  ͼ�������ƫ��λ��
 *@param    psImgBuf ͼ����󻺳壬����Ϊ�������еĵ�������
 *@param    pszTextBuf] �����ַ����׵�ַ���ַ������Ȳ��ܳ���1K
 *@param    unMode ��ӡ��ģʽ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnIntegrate(uint unXsize,uint unYsize,uint unXpos,uint unYpos,const char *psImgBuf,const char *pszTextBuf, uint unMode);
/**
 *@brief		���ô�ӡ�Ҷ�
 *@details		���ô�ӡ�Ҷ�(����ʱ��)���Ա���ڲ�ͬ�Ĵ�ӡֽ���д�ӡЧ��΢��.
 *@param    unGrey �Ҷ�ֵ����Χ0~5��0Ϊ���Ч����5Ϊ��Ũ�Ĵ�ӡЧ������ӡ����Ĭ�ϵĻҶȼ���Ϊ3��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetGreyScale(uint unGrey);
/**
 *@brief  	���ô�ӡ��߽硢�ּ�ࡢ�м�ࡣ�ڶԴ�ӡ����Ч���ú�һֱ��Ч��ֱ���´�
 *@param  unBorder ��߾� ֵ��Ϊ��0-288(Ĭ��Ϊ0)
 *@param    unColumn �ּ�� ֵ��Ϊ��0-255(Ĭ��Ϊ0)
 *@param    unRow �м�� ֵ��Ϊ��0-255(Ĭ��Ϊ0)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetForm(uint unBorder,uint unColumn, uint unRow);
/**
 *@brief 	  ��������ֽ
 *@details	�ô�ӡ����ֽ������Ϊ���ص�,���øú�������û��������ֽ�����Ǵ��ڻ������У��ȵ���start֮��ʹ�ӡ����һ��ִ��
 *@param    unPixel ��ֽ���ص� ֵ��> 0
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnFeedByPixel(uint unPixel);

/**
 *@brief	��ӡ�Ƿ����»��߹���.
 *@param  emStatus 0�����»��ߣ�1�����»���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetUnderLine(EM_PRN_UNDERLINE_STATUS emStatus);

/**
 *@brief	���ö��뷽ʽ.(��δʵ��)
 *@param  unType 0:�����; 1���ж���; 2�Ҷ���;3�رն��뷽ʽ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetAlignment(uint unType);
/**
 *@brief	��ȡ��ӡ����.
 *@retval punLen �����Ѿ���ӡ�ĳ���(��ǰ��������ʼ��ӡ�����ڴ�ӡ����Ϊֹ����ӡ����ӡ���ܹ�����)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnGetPrintedLen(uint *punLen);
/**
 *@brief		�Զ�������ע�ᡣ
 *@param		pstMsg ST_PRN_FONTMSG����ָ�룬ʹ���Զ���ע��Ҫ�����Ӧ��Ϣ����д
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnFontRegister(ST_PRN_FONTMSG *pstMsg);
/**
 *@brief	����ע�����ѡ���ӡ����.
 *@param  unFontId ע�������id(�����ú�Ḳ��NDK_PrnSetFont���趨������)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnSetUsrFont(uint unFontId);
/**
 *@brief	��øôδ�ӡ�ĵ�������.
 *@retval  punLine ��������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnGetDotLine(uint *punLine);
/**
 *@brief	��ӡbmp��png�ȸ�ʽ��ͼƬ
 *@details  �ú������洢��pos�ϵ�ͼƬ���н����洢�����󻺳�����  ����ͼƬ���������һ����ʱ�䣬��Ҫ��ʱ����Ҫ��һ���ĵȴ�ʱ��
 *@param  pszPath ͼƬ���ڵ�·��
 *@param  unXpos  ͼ�ε����Ͻǵ���λ�ã��ұ�������xpos+xsize(�����ͼƬ�Ŀ��ֵ)<=ndk_PR_MAXLINEWIDE������ģʽΪ384������Ŵ�ʱΪ384/2��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PrnPicture(uint unXpos,const char *pszPath);

/** @} */ // ��ӡģ�����


/** @addtogroup �ļ�ϵͳ
* @{
*/

/**
 *@brief 		���ļ�.
 *@details
 *@param    pszName �ļ���
 *@param    pszMode ��ģʽ "r"(��ֻ����ʽ�򿪣������������ʧ��) or "w"(��д�ķ�ʽ�򿪣�����ļ��������򴴽�)��
 *@return
 *@li	 fd				�����ɹ������ļ�������
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsOpen(const char *pszName,const char *pszMode);


/**
 *@brief 		�ر��ļ�.
 *@details
 *@param    nHandle �ļ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsClose(int nHandle);

/**
 *@brief 		�Ӵ򿪵�nHandle�ļ���ǰָ���unLength���ַ���������psBuffer.
 *@details
 *@param    nHandle �ļ����
 *@param    unLength����Ҫ��ȡ���ַ��ĳ���
 *@retval    psBuffer����Ҫ����Ļ�����ע��Ҫ�㹻����length�ֽ�
 *@return
 *@li	length				ʵ�ʶ�ȡ���ļ��ĳ���
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsRead(int nHandle, char *psBuffer, uint unLength );

/**
 *@brief 		��򿪵�nHandle�ļ�д��unLength���ֽ�.
 *@details
 *@param    nHandle �ļ����
 *@param    psBuffer����Ҫд���ļ����ݵĻ�����
 *@param    unLength����Ҫд��ĳ���
 *@return
 *@li	length				����ʵ��д�뵽�ļ��ĳ���
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsWrite(int nHandle, const char *psBuffer, uint unLength );

/**
 *@brief 		�ƶ��ļ�ָ�뵽��unPosition���ulDistance��λ��
 *@details
 *@param    nHandle �ļ����
 *@param    ulDistance�����ݲ���unPosition���ƶ���дλ�õ�λ������
 *@param    unPosition����Ҫ��ȡ���ַ��ĳ���
 						SEEK_SET ����offset��Ϊ�µĶ�дλ�á�
						SEEK_CUR ��Ŀǰ�Ķ�дλ����������offset��λ������
						SEEK_END ����дλ��ָ���ļ�β��������offset��λ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsSeek(int nHandle, ulong ulDistance, uint unPosition );

/**
 *@brief 		ɾ��ָ���ļ�
 *@details
 *@param    pszName Ҫɾ�����ļ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsDel(const char *pszName);

/**
 *@brief 		�ļ�����
 *@details
 *@param    pszName �ļ���
 *@retval   punSize �ļ���С����ֵ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsFileSize(const char *pszName,uint *punSize);

/**
 *@brief 		�ļ�������
 *@details
 *@param    pszsSrcname ԭ�ļ���
 *@param    pszDstname Ŀ���ļ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsRename(const char *pszsSrcname, const char *pszDstname );

/**
 *@brief 		�����ļ��Ƿ����
 *@details
 *@param    pszName �ļ���
 *@return
 *@li	NDK_OK				�����ɹ�(�ļ�����)
 *@li	����EM_NDK_ERR		����ʧ��
*/

int NDK_FsExist(const char *pszName);

/**
 *@brief 		�ļ��ض�
 *@details   NDK_FsTruncate()�Ὣ����pszPath ָ�����ļ���С��Ϊ����unLen ָ���Ĵ�С�����ԭ�����ļ���С�Ȳ���unLen���򳬹��Ĳ��ֻᱻɾȥ��
 		   			���ԭ���ļ��Ĵ�С��unLenС�Ļ�������Ĳ��ֽ�����0xff
 *@param    pszPath �ļ�·��
 *@param    unLen ��Ҫ�ض̳���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsTruncate(const char *pszPath ,uint unLen );

/**
 *@brief 	  	��ȡ�ļ���λ��
 *@details   ����ȡ���ļ���Ŀǰ�Ķ�дλ��
 *@param    nHandle �ļ����
 *@retval    pulRet �ļ���λ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsTell(int nHandle,ulong *pulRet);

/**
 *@brief 	  	ȡ�ļ�ϵͳ���̿ռ��ʹ�����
 *@details
 *@param    	unWhich :0--�Ѿ�ʹ�õĴ��̿ռ�1--ʣ��Ĵ��̿ռ�
 *@retval     pulSpace ���ش��̿ռ�ʹ��������ʣ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsGetDiskSpace(uint unWhich,ulong *pulSpace);
/**
 *@brief 		����Ŀ¼.
 *@details
 *@param    pszName Ŀ¼����
 *@return
 *@li	 NDK_OK				�����ɹ�����
 *@li	����EM_NDK_ERR		����ʧ��
*/

int NDK_FsCreateDirectory(const char *pszName);


/**
 *@brief 		ɾ��Ŀ¼.
 *@details
 *@param    pszName Ŀ¼����
 *@return
 *@li	 NDK_OK				�����ɹ�����
 *@li	����EM_NDK_ERR		����ʧ��
*/

int NDK_FsRemoveDirectory(const char *pszName);
/**
 *@brief 		�ļ�ϵͳ��ʽ��
 *@details	�ù��ܽ����ڴ�ͳpos��gpƽ̨posֱ�ӷ���-1
 *@return
 *@li	 NDK_OK				�����ɹ�����
 *@li	����EM_NDK_ERR		����ʧ��
*/

int NDK_FsFormat(void);


/**
 *@brief 		�г��ƶ�Ŀ¼�µ������ļ�
 *@details	�����psBuf�ĵ�sizeһ��Ҫ���󣬷�������������pbuf ÿ20���ֽڴ洢һ���ļ���
 						ǰ19 Ϊ�ļ����������Զ��ض̡���20�ֽ����Ϊ1���ʾ���ļ�Ϊ�ļ��У�0Ϊ��ͨ�ļ�
 *@param       pPath ָ��Ҫ��ȡ��Ŀ¼
 *@retval        psBuf ���ļ����洢��pbuf�ܷ���
 *@retval        punNum ���ظ��ļ���Ŀ¼���ļ�����
 *@return
 *@li	 NDK_OK				�����ɹ�����
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FsDir(const char *pPath,char *psBuf,uint *punNum);

/** @} */ // �ļ�ϵͳģ�����


/** @addtogroup Ӧ�ù���
* @{
*/

#define NDK_APP_NAME_MAX_LEN		32	/**< Ӧ��������󳤶�*/
#define NDK_APP_VER_MAX_LEN			16	/**< �汾��󳤶�*/
#define NDK_APP_BUILD_TIME_LEN		32	/**< ����ʱ����󳤶�*/

/**
 *@brief Ӧ����Ϣ,�ýṹ��NLD�ṹ��ʹ�ã��漰�������⡣�ṹ���岻�����޸ģ����������ع��ߴ������Ϣ��һ��
*/
typedef struct APPINFO{
    uchar 	szAppName[NDK_APP_NAME_MAX_LEN + 1];		/**<�û���������*/
    uchar	szVerBuf[NDK_APP_VER_MAX_LEN + 1];			/**<�û�����汾��Ϣ*/
    int		nSeriNo;									/**<���,V2�汣��,�����岻ͬ	*/
    uchar	szBuildTime[NDK_APP_BUILD_TIME_LEN + 1];	/**<�ļ�����ʱ��*/
    int		nIsMaster;									/**<���ر�־ */
    uint	sunReverse[3];
}ST_APPINFO;

typedef int (*CallbackMock)(int, void *,int,void **,int *);

/**
 *@brief	���ûص�����ָ��
 *@param	NDK_EventMain	������ַ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AppSetEventCallBack(CallbackMock NDK_EventMain);

/**
 *@brief	����Ӧ�ó���
 *@param	pszAppName	Ӧ�����ơ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AppRun(const uchar *pszAppName);

/**
 *@brief	ִ���¼�����
 *@param	pszFilename		Ӧ������
 *@param	psInEventMsg	�����¼���Ϣ
 *@param	nInlen			�����¼�����
 *@param	nMaxOutLen		���������¼��ĳ���
 *@retval	psOutEventMsg	��ȡ����Ӧ����Ϣ
 *@retval	pnOutLen		ʵ����������ݳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AppDoEvent(const uchar *pszFilename,int nModuleID,const void *psInEventMsg, int nInlen, void *psOutEventMsg, int nMaxOutLen, int *pnOutLen);

/**
 *@brief	��ȡ��װ��Ӧ�ó����Ƿ���Ҫ�����ı�־λ
 *@param	*nRebootFlag		�������1��ʾ��װ��Ӧ�ó�����Ҫ����������Ч��0����Ҫ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����NDK_ERRCODE		����ʧ��
*/
int NDK_AppGetReboot(int *nRebootFlag);
/**
 *@brief	װ��Ӧ��
 *@param	pszFilename		Ӧ������
 *@param	nRebootFlag		������־������ͳPOS�ն��⣬�����ն˸ò���������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AppLoad(const uchar *pszFilename, int nRebootFlag);

/**
 *@brief	��ȡӦ����Ϣ
 *@param	pszAppName	Ӧ������, ��������ΪNULL��
 *@param	nPos		ƫ�ƣ���ƫ��λ��Ϊ��ϵͳ����Ϣ��ƫ�ƣ�������˳����ء�
 *@param	nSizeofInfo	���Ӧ����Ϣ����󳤶ȡ�
 *@retval	pstAppInfo	��ȡ����Ӧ����Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AppGetInfo(const uchar *pszAppName, int nPos, ST_APPINFO *pstAppInfo, int nSizeofInfo);

/**
 *@brief	ɾ��Ӧ�ó���
 *@param	pszAppName	Ӧ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AppDel(const uchar *pszAppName);


/** @} */ // ��Ӧ��ģ�����

/** @addtogroup �㷨
* @{
*/
#define MAX_RSA_MODULUS_LEN		512				/**< ���ģ���� */
#define MAX_RSA_PRIME_LEN		256				/**< ���ģ�������� */

/**
 *@brief �Գ���Կ�㷨
*/
typedef enum{
	ALG_TDS_MODE_ENC = 0,		/**< DES���� */
	ALG_TDS_MODE_DEC = 1,		/**< DES���� */
}EM_ALG_TDS_MODE;

/**
 *@brief RSA�㷨��Կ����
*/
typedef enum{
	RSA_KEY_LEN_512  = 512,
	RSA_KEY_LEN_1024 = 1024,
	RSA_KEY_LEN_2048 = 2048,
}EM_RSA_KEY_LEN;

/**
 *@brief RSA�㷨ָ��
*/
typedef enum{
	RSA_EXP_3 = 0x03,
	RSA_EXP_10001 = 0x10001,
}EM_RSA_EXP;

/**
 *@brief RSA��Կ
*/
typedef struct {
    ushort bits;       							/**< ģλ�� */
    uchar modulus[MAX_RSA_MODULUS_LEN+1];      	/**< ģ */
	uchar publicExponent[MAX_RSA_MODULUS_LEN+1]; 	/**< ��Կָ�� */
    uchar exponent[MAX_RSA_MODULUS_LEN+1];     	/**< ˽Կָ�� */
    uchar prime[2][MAX_RSA_PRIME_LEN+1];      	/**< pq���� */
    uchar primeExponent[2][MAX_RSA_PRIME_LEN+1]; 	/**< ������ָ������ֵ */
    uchar coefficient[MAX_RSA_PRIME_LEN+1];  	 	/**< ��������������ֵ */
}ST_RSA_PRIVATE_KEY;

/**
 *@brief RSA˽Կ
*/
typedef struct {
    ushort bits;                    			/**< ģλ�� */
    uchar modulus[MAX_RSA_MODULUS_LEN+1];  		/**< ģ */
    uchar exponent[MAX_RSA_MODULUS_LEN+1];		/**< ָ�� */
}ST_RSA_PUBLIC_KEY;

/**
 *@brief	����des
 *@param	psDataIn	�������ݻ���
 *@param	psKey		��Կ����,����8,16,24
 *@param    nKeyLen     ��Կ���ȣ�ֵֻ��Ϊ8,16,24
 *@param	nMode		����ģʽ �μ�\ref EM_ALG_TDS_MODE "EM_ALG_TDS_MODE"
 *@retval	psDataOut	�������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgTDes(uchar *psDataIn, uchar *psDataOut, uchar *psKey, int nKeyLen, int nMode);

/**
 *@brief	����sha1
 *@param	psDataIn	��������
 *@param	nInlen		���ݳ���
 *@retval	psDataOut	������ݣ�sha1����������Ϊ20�ֽڣ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgSHA1(uchar *psDataIn, int nInlen, uchar *psDataOut);

/**
 *@brief	����sha256
 *@param	psDataIn	��������
 *@param	nInlen		���ݳ���
 *@retval	psDataOut	������ݣ�sha256����������Ϊ  �ֽڣ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgSHA256(uchar *psDataIn, int nInlen, uchar *psDataOut);

/**
 *@brief	����sha512
 *@param	psDataIn	��������
 *@param	unInlen		����ģʽ
 *@retval	psDataOut	������ݣ�sha512����������Ϊ �ֽڣ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgSHA512(uchar *psDataIn, int nInlen, uchar *psDataOut);

/**
 *@brief	RSA��Կ������
 *@param	nProtoKeyBit		��Կλ������ǰ֧��512��1024��2048λ �ο�\ref EM_RSA_KEY_LEN "EM_RSA_KEY_LEN"
 *@param	nPubEType			ָ�����ͣ��ο�\ref EM_RSA_EXP "EM_RSA_EXP"
 *@retval	pstPublicKeyOut		��Կ
 *@retval	pstPrivateKeyOut	˽Կ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgRSAKeyPairGen( int nProtoKeyBit, int nPubEType, ST_RSA_PUBLIC_KEY *pstPublicKeyOut, ST_RSA_PRIVATE_KEY *pstPrivateKeyOut);

/**
 *@brief	RSA��Կ�Լӽ���
 *@details	�ú�������RSA���ܻ��������,���ܻ����ͨ��ѡ�ò�ͬ����Կʵ�֡���(Modul,Exp)ѡ��˽����Կ,����м���;��ѡ�ù�����Կ,����н��ܡ�
 			psDataIn�ĵ�һ���ֽڱ���С��psModule�ĵ�һ���ֽڡ� �ú�����ʵ�ֳ��Ȳ�����2048 bits ��RSA���㡣
 			��������ݿ��ٵĻ�������ģ����+1��
 *@param	psModule		ģ����,�ַ�������ʽ����,��"31323334"
 *@param	nModuleLen	ģ�ĳ��� ֻ������ѡ��512/8,1024/8,2048/8
 *@param	psExp			���RSA�����ָ��������ָ�롣����e.����λ��ǰ,��λ�ں��˳��洢,��"10001"
 *@param	psDataIn		���ݻ���,�������Ĵ�С���ģ�ĳ��ȴ�1
 *@retval	psDataOut		�������,��������ݳ��ȵ���ģ�ĳ��ȡ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgRSARecover(uchar *psModule, int nModuleLen, uchar *psExp, uchar *psDataIn, uchar *psDataOut);

/**
 *@brief	RSA��Կ��У��
 *@param	pstPublicKey		��Կ
 *@param	pstPrivateKey		˽Կ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AlgRSAKeyPairVerify(ST_RSA_PUBLIC_KEY *pstPublicKey, ST_RSA_PRIVATE_KEY *pstPrivateKey);


/** @} */ // �㷨ģ�����


/** @addtogroup ��ȫ
* @{
*/

#define MAX_RSA_MODULUS_LEN		512		/**<RSA���ģ����*/

/**
 *@brief �ն���Կ����
*/
typedef enum{
	SEC_KEY_TYPE_TLK=0,	/**<�ն�װ����Կ*/
	SEC_KEY_TYPE_TMK,	/**<�ն�MAC��Կ*/
	SEC_KEY_TYPE_TPK,	/**<�ն�PIN��Կ*/
	SEC_KEY_TYPE_TAK,	/**<�ն�MAC��Կ*/
	SEC_KEY_TYPE_TDK,	/**<�ն����ݼӽ�����Կ*/
}EM_SEC_KEY_TYPE;

/**
 *@brief ��ԿУ��ģʽ
*/
typedef enum{
	SEC_KCV_NONE=0,		/**<����֤*/
	SEC_KCV_ZERO,		/**<��8���ֽڵ�0x00����DES/TDES����,�õ������ĵ�ǰ4���ֽڼ�ΪKCV*/
	SEC_KCV_VAL,		/**<���ȶ���Կ���Ľ�����У��,�ٶԡ�\x12\x34x56\x78\x90\x12\x34\x56������DES/TDES��������,�õ����ĵ�ǰ4���ֽڼ�ΪKCV*/
	SEC_KCV_DATA,		/**<����һ������KcvData,ʹ��Դ��Կ��[aucDstKeyValue(����) + KcvData]����ָ��ģʽ��MAC����,�õ�8���ֽڵ�MAC��ΪKCV */
}EM_SEC_KCV;

/**
 *@brief MAC�㷨
*/
typedef enum{
	SEC_MAC_X99=0,      /**< X99�㷨�����ݷ�Ϊ8�ֽ�block�����㲹0��ÿ��block���ܺ�����һ��block������Կ���ȼ���*/
	SEC_MAC_X919,       /**< X99�㷨�����ݷ�Ϊ8�ֽ�block�����㲹0��ÿ��block���ܺ�����һ��block������ԿDES���ܣ�
                            ���֡�����Կ����Ϊ16�ֽ���3DES�����Ϊ8�ֽڰ�DES*/
	SEC_MAC_ECB,        /**< ȫ���������󣬽�����������DES����б任���ο������淶�й���ECB�㷨˵��*/
	SEC_MAC_9606,       /**< ȫ����������������������des����*/
}EM_SEC_MAC;
/**
 * ����ʵ��PIN������̵ĳ�ʱ���Ƶı���
 */
typedef enum {
	SEC_PIN_ISO9564_0=3,    /**<ʹ�����˺ż��ܣ����벻��λ����'F'*/
	SEC_PIN_ISO9564_1=4,    /**<��ʹ�����˺ż��ܣ����벻��λ���������*/
	SEC_PIN_ISO9564_2=5,    /**<��ʹ�����˺ż��ܣ����벻��λ����'F'*/
	SEC_PIN_ISO9564_3=6     /**<ʹ�����˺ż��ܣ����벻��λ���������*/
}EM_SEC_PIN;

/**
 *@brief DES�������ͣ����ڲ�ͬλ���Խ��л����㣬
        ���磺SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODE
            ��ʾ����8�ֽ���Կ���ȣ�ʹ��ECBģʽ���м�������
*/
typedef enum{
	SEC_DES_ENCRYPT=0,                  /**<DES����*/
	SEC_DES_DECRYPT=1,                  /**<DES����*/
	SEC_DES_KEYLEN_DEFAULT=(0<<1),      /**<ʹ�ð�װ���ȵ���Կ���м���*/
    SEC_DES_KEYLEN_8=(1<<1),            /**<ʹ��8�ֽ���Կ���м���*/
    SEC_DES_KEYLEN_16=(2<<1),           /**<ʹ��16�ֽ���Կ���м���*/
    SEC_DES_KEYLEN_32=(3<<1),           /**<ʹ��32�ֽ���Կ���м���*/
	SEC_DES_ECBMODE=(1<<3),             /**<ʹ��ECBģʽ���м���*/
	SEC_DES_CBCMODE=(1<<4),             /**<ʹ��CBCģʽ���м���*/
}EM_SEC_DES;

/**
 *@brief ��Կ��Ϣ
*/
typedef struct{
    uchar 	ucScrKeyType; 		/**< ��ɢ����Կ��Դ��Կ����Կ���ͣ��ο� KEY_TYPE, ���õ���ucDstKeyType���ڵ���Կ����*/
    uchar 	ucDstKeyType; 		/**< Ŀ����Կ����Կ���ͣ��ο�KEY_TYPE */
    uchar 	ucScrKeyIdx;		/**< ��ɢ����Կ��Դ��Կ����,����һ���1��ʼ,����ñ���Ϊ0,���ʾ�����Կ��д����������ʽ */
    uchar 	ucDstKeyIdx;		/**< Ŀ����Կ���� */
    int 	nDstKeyLen;			/**< Ŀ����Կ����,8,16,24 */
    uchar 	sDstKeyValue[24];	/**< д����Կ������ */
}ST_SEC_KEY_INFO;

/**
 *@brief У����Ϣ
*/
typedef struct{
    int 	nCheckMode; 		/**< У��ģʽ */
    int 	nLen;				/**< У������������ */
    uchar 	sCheckBuf[128];		/**< У�����ݻ����� */
}ST_SEC_KCV_INFO;

/**
 *@brief �ѻ�����PIN��Կ
*/
typedef struct
{
	uint	unModlen;					/**< ���ܹ�Կģ����  */
	uchar	sMod[MAX_RSA_MODULUS_LEN];  /**< ���ܹ�Կģ��,���ֽ���ǰ,���ֽ��ں�,����λǰ��0 */
	uchar	sExp[4];       				/**< ���ܹ�Կָ��,���ֽ���ǰ,���ֽ��ں�,����λǰ��0 */
	uchar	ucIccRandomLen;   			/**< �ӿ�Ƭȡ�õ��������  */
	uchar	sIccRandom[8];   			/**< �ӿ�Ƭȡ�õ������  */
}ST_SEC_RSA_PINKEY;

/**
 *@brief RSA��Կ��Ϣ
*/
typedef struct {
    ushort usBits;                    		/**< RSA��Կλ�� */
    uchar sModulus[MAX_RSA_MODULUS_LEN];  	/**< ģ */
    uchar sExponent[MAX_RSA_MODULUS_LEN]; 	/**< ָ�� */
}ST_SEC_RSA_KEY;

/**
 *@brief	��ȡ��ȫ�ӿڰ汾
 *@retval	pszVerInfoOut	�汾��Ϣ��С��16�ֽڣ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetVer(uchar * pszVerInfoOut);

/**
 *@brief	��ȡ�����
 *@param	nRandLen		��Ҫ��ȡ�ĳ���
 *@retval	pvRandom		���������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����\ref EM_NDK_ERR "EM_NDK_ERR"		����ʧ��
*/
int NDK_SecGetRandom(int nRandLen , void *pvRandom);

/**
 *@brief	���ð�ȫ����
 *@details	1���û�һ��ͨ���˺��������˰�ȫ������Ϣ��������������ݴ����õ�������Ϣ���п��ơ�
 			���û�е��ô˺������ã�����������ᰴ��Ĭ�ϵ���Ͱ�ȫ���ý��С�
 			2��ͨ����ȫ������Ϣֻ�������ߣ��������ͣ�������Ϣ������������һλ��1����0������Ϊ��ȫ�Խ��ͣ���
 *@param	unCfgInfo		������Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecSetCfg(uint unCfgInfo);

/**
 *@brief	��ȡ��ȫ����
 *@retval	punCfgInfo		������Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetCfg(uint *punCfgInfo);

/**
 *@brief	��ȡ��Կkcvֵ
 *@details	��ȡ��Կ��KCVֵ,�Թ��Ի�˫��������Կ��֤,��ָ������Կ���㷨��һ�����ݽ��м���,�����ز����������ġ�
 *@param	ucKeyType		��Կ����
 *@param	ucKeyIdx		��Կ���
 *@param	pstKcvInfoOut	KCV����ģʽ
 *@retval	pstKcvInfoOut	KCVֵ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetKcv(uchar ucKeyType, uchar ucKeyIdx, ST_SEC_KCV_INFO *pstKcvInfoOut);

/**
 *@brief	����������Կ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecKeyErase(void);

/**
 *@brief	д��һ����Կ,����TLK,TMK��TWK��д�롢��ɢ,������ѡ��ʹ��KCV��֤��Կ��ȷ�ԡ�
 *@details
 	PED����������Կ��ϵ,���ϵ��µ�˳������Ϊ��
	TLK��Terminal Key Loading Key
    	�յ��л�POS��Ӫ�̵�˽����Կ,���յ��л���POS��Ӫ���ڰ�ȫ������ֱ��д�롣
    	����Կÿ��PED�ն�ֻ��һ��,����������1��1

	TMK��Terminal Master Key��Acquirer Master Key
		�ն�����Կ,���߳�Ϊ�յ�������Կ��������Կ����100��,��������1��100
		TMK�����ڰ�ȫ������ֱ��д��,ֱ��д��TMK,��ͨ��TMK��ɢTWK�ķ�ʽ��MK/SK����Կ��ϵһ�¡�
	TWK��Transaction working key = Transaction Pin Key + Transaction MAC Key + Terminal DES Enc Key + Terminal DES DEC/ENC Key
		�ն˹�����Կ,����PIN���ġ�MAC���������Կ��������Կ����100��,��������1��100��
		TPK:����Ӧ������PIN��,����PIN Block��
		TAK:����Ӧ�ñ���ͨѶ��,����MAC��
		TEK:���ڶ�Ӧ�����������ݽ���DES/TDES���ܴ����洢��
		TDK:���ڶ�Ӧ�����������ݽ���DES/TDES�ӽ�������
	TWK�����ڰ�ȫ������ֱ��д��,ֱ��д��TWK��Fixed Key��Կ��ϵһ�¡�ÿ����Կ����������,����,��;�ͱ�ǩ��
	������Կ�ı�ǩ����д����Կǰͨ��API�趨��,����Ȩ����Կ��ʹ��Ȩ�޲���֤��Կ���ᱻ���á�

	DUKPT��Կ���ƣ�
	DUKPT��Derived Unique Key Per Transaction����Կ��ϵ��һ�ν���һ��Կ����Կ��ϵ,��ÿ�ʽ��׵Ĺ�����Կ��PIN��MAC���ǲ�ͬ�ġ�
	��������KSN��Key Serial Number���ĸ���,KSN����ʵ��һ��һ�ܵĹؼ����ӡ� ÿ��KSN��Ӧ����Կ��������Կ��;����������ͬ����Կ��
 	������Կ����10�顣��д��TIK��ʱ��,��Ҫѡ�����������,��ʹ��DUKPT��Կʱѡ���Ӧ����������
 *@param	pstKeyInfoIn		��Կ��Ϣ
 *@param	pstKcvInfoIn		��ԿУ����Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecLoadKey(ST_SEC_KEY_INFO * pstKeyInfoIn, ST_SEC_KCV_INFO * pstKcvInfoIn);

/**
 *@brief	�������μ���PINBlock���߼���MAC֮����С���ʱ��
 *@details 	PINBLOCK���ʱ��ļ��㷽ʽ��
 			Ĭ��Ϊ120����ֻ�ܵ���4��,��TPKIntervalTimeMsĬ��ֵΪ30��,���øú����������ú�,����Ϊ4*TPKIntervalTimeMsʱ����ֻ�ܵ���4�Ρ�
 			���紫���TPKIntervalTimeMsΪ20000(ms),��80����ֻ�ܵ���4��
 *@param	unTPKIntervalTimeMs	PIN��Կ������ʱ�䣬0-����Ĭ��ֵ��0xFFFFFFFF�����ı�
 *@param	unTAKIntervalTimeMs	MAC��Կ������ʱ�䣬0-����Ĭ��ֵ��0xFFFFFFFF�����ı�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecSetIntervaltime(uint unTPKIntervalTimeMs, uint unTAKIntervalTimeMs);

/**
 *@brief	���ù��ܼ�����
 *@details 	��������������У����ܼ���;���ж���
 *@param	ucType	������;���Ͷ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecSetFunctionKey(uchar ucType);

/**
 *@brief	����MAC
 *@param	ucKeyIdx		��Կ���
 *@param	psDataIn		��������
 *@param	nDataInLen		�������ݳ���
 *@param	ucMod			MAC����ģʽ �ο�/ref EM_SEC_MAC "EM_SEC_MAC"
 *@retval	psMacOut		MACֵ������8�ֽ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetMac(uchar ucKeyIdx, uchar *psDataIn, int nDataInLen, uchar *psMacOut, uchar ucMod);

/**
 *@brief	��ȡPIN Block
 *@param	ucKeyIdx		��Կ���
 *@param	pszExpPinLenIn	���볤�ȣ���ʹ��,���зָ���磺0,4,6
 *@param	pszDataIn		��ISO9564Ҫ�������PIN BLOCK
 *@param	ucMode			����ģʽ �ο�/ref EM_SEC_PIN "EM_SEC_PIN"
 *@param	nTimeOutMs		��ʱʱ�䣨���������120�룩��λ:ms
 *@retval	psPinBlockOut	    PIN Block���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetPin(uchar ucKeyIdx, uchar *pszExpPinLenIn,const uchar * pszDataIn, uchar *psPinBlockOut, uchar ucMode, uint nTimeOutMs);

/**
 *@brief	����DES
 *@details 	ʹ��ָ����Կ����des���㣬ע�⣺1~100��Ž��мӽ���
 *@param	ucKeyType		DES��Կ����
 *@param	ucKeyIdx		DES��Կ���
 *@param	psDataIn		������Ϣ
 *@param	nDataInLen		���ݳ���
 *@param	ucMode			����ģʽ �ο�/ref EM_SEC_DES "EM_SEC_DES"
 *@retval	psDataOut		���������Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecCalcDes(uchar ucKeyType, uchar ucKeyIdx, uchar * psDataIn, int nDataInLen, uchar *psDataOut, uchar ucMode);

/**
 *@brief	У���ѻ�����PIN
 *@details 	��ȡ����PIN,Ȼ����Ӧ���ṩ�Ŀ�Ƭ�����뿨Ƭͨ����,������PIN BLOCKֱ�ӷ��͸���Ƭ(PIN BLOCK��ʽ���÷���������)��
 *@param	ucIccSlot		IC����
 *@param	pszExpPinLenIn	���볤�ȣ���ʹ��,���зָ���磺0,4,6
 *@param	ucMode			IC������ģʽ(ֻ֧��EMV)
 *@param	unTimeoutMs		��ʱʱ��
 *@retval	psIccRespOut	��ƬӦ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecVerifyPlainPin(uchar ucIccSlot, uchar *pszExpPinLenIn, uchar *psIccRespOut, uchar ucMode,  uint unTimeoutMs);

/**
 *@brief	У���ѻ�����PIN
 *@details 	�Ȼ�ȡ����PIN,����Ӧ���ṩ��RsaPinKey������PIN����EMV�淶���м���,Ȼ����Ӧ���ṩ�Ŀ�Ƭ�����뿨Ƭͨ����,������PINֱ�ӷ��͸���Ƭ
 *@param	ucIccSlot		IC����
 *@param	pszExpPinLenIn	���볤�ȣ���ʹ��,���зָ���磺0,4,6
 *@param	pstRsaPinKeyIn	RSA��Կ����
 *@param	ucMode			IC������ģʽ(ֻ֧��EMV)
 *@param	unTimeoutMs		��ʱʱ��
 *@retval	psIccRespOut	��ƬӦ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecVerifyCipherPin(uchar ucIccSlot, uchar *pszExpPinLenIn, ST_SEC_RSA_KEY *pstRsaPinKeyIn, uchar *psIccRespOut, uchar ucMode, uint unTimeoutMs);

/**
 *@brief	��װDUKPT��Կ
 *@param	ucGroupIdx		��Կ��ID
 *@param	ucSrcKeyIdx		ԭ��ԿID���������ܳ�ʼ��Կֵ����ԿID��
 *@param	ucKeyLen		��Կ����
 *@param	psKeyValueIn	��ʼ��Կֵ
 *@param	psKsnIn		    KSNֵ
 *@param	pstKcvInfoIn	Kcv��Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecLoadTIK(uchar ucGroupIdx, uchar ucSrcKeyIdx, uchar ucKeyLen, uchar * psKeyValueIn, uchar * psKsnIn, ST_SEC_KEY_INFO * pstKcvInfoIn);

/**
 *@brief	��ȡDUKPTֵ
 *@param	ucGroupIdx		DUKPT��Կ��ID
 *@retval	psKsnOut		��ǰKSN��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetDukptKsn(uchar ucGroupIdx, uchar * psKsnOut);

/**
 *@brief	KSN������
 *@param	ucGroupIdx		DUKPT��Կ��ID
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecIncreaseDukptKsn(uchar ucGroupIdx);

/**
 *@brief	��ȡDUKPT��Կ��PIN Block
 *@param	ucGroupIdx		��Կ���
 *@param	pszExpPinLenIn	���볤�ȣ���ʹ��,���зָ���磺0,4,6
 *@param	psDataIn		��ISO9564Ҫ�������PIN BLOCK
 *@param	ucMode			����ģʽ
 *@param	unTimeoutMs		��ʱʱ��
 *@retval	psKsnOut		��ǰKSN��
 *@retval	psPinBlockOut	PIN Block���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetPinDukpt(uchar ucGroupIdx, uchar *pszExpPinLenIn, uchar * psDataIn, uchar* psKsnOut, uchar *psPinBlockOut, uchar ucMode, uint unTimeoutMs);

/**
 *@brief	����DUKPT��ԿMAC
 *@param	ucGroupIdx		��Կ���
 *@param	psDataIn		��������
 *@param	nDataInLen		�������ݳ���
 *@param	ucMode			MAC����ģʽ
 *@retval	psMacOut		MACֵ������8�ֽ�
 *@retval	psKsnOut		��ǰKSN��
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecGetMacDukpt(uchar ucGroupIdx, uchar *psDataIn, int nDataInLen, uchar *psMacOut, uchar *psKsnOut, uchar ucMode);

/**
 *@brief	����DES
 *@details 	ʹ��ָ����Կ����des����
 *@param	ucGroupIdx		DUKPT��Կ���
 *@param	ucKeyVarType	��Կ����
 *@param	psIV			��ʼ����
 *@param	psDataIn		������Ϣ
 *@param	usDataInLen		���ݳ���
 *@param	ucMode			����ģʽ
 *@retval	psDataOut		���������Ϣ
 *@retval	psKsnOut		��ǰKSN��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecCalcDesDukpt(uchar ucGroupIdx, uchar ucKeyVarType, uchar *psIV, ushort usDataInLen, uchar *psDataIn,uchar *psDataOut,uchar *psKsnOut ,uchar ucMode);

/**
 *@brief	��װRSA��Կ
 *@param	ucRSAKeyIndex	��Կ���
 *@param 	pstRsakeyIn		RSA��Կ��Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecLoadRsaKey(uchar ucRSAKeyIndex, ST_SEC_KEY_INFO *pstRsakeyIn);

/**
 *@brief	��װRSA��Կ
 *@param	ucRSAKeyIndex	��Կ���
 *@param 	psDataIn		���������ݣ����ܳ�����Կģ��
 *@param	nDataInLen		���ݳ���
 *@retval	psDataOut		�������,��ģ�ȳ�
 *@retval	psKeyInfoOut	��Կ��Ϣ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SecRecover(uchar ucRSAKeyIndex, const uchar *psDataIn, int nDataInLen, uchar *psDataOut, uchar *psKeyInfoOut);

/** @} */ // ��ȫģ�����



/** @addtogroup ����ͨѶ
* @{
*/


/**
 *@brief  ����ѡ��
*/
typedef enum {
    PORT_NUM_COM1 = 0,	/**<����1*/
    PORT_NUM_COM2 = 1,	/**<����2*/
    PORT_NUM_WIRELESS = 2,	/**<����ģ��*/
    PORT_NUM_MUX1 = 3,	/**<��·����1*/
    PORT_NUM_MUX2 = 4,	/**<��·����2*/
    PORT_NUM_MUX3 = 5,	/**<��·����3*/
    PORT_NUM_MODEM = 6,	/**<����ģ��*/
    PORT_NUM_WIFI = 7,	/**<Wifiģ��*/
    PORT_NUM_USB = 8	/**<USBģ��*/
} EM_PORT_NUM;




/**
 *@brief	��ʼ�����ڣ��Դ��ڲ����ʣ�����λ����żλ��ֹͣλ�Ƚ������á�����ÿ��ʹ�ô���֮ǰ�ȵ��øó�ʼ������
 *@param	emPort	ָ���Ĵ���
 *@param	pszAttr	ͨѶ�ʺ͸�ʽ��,��"115200,8,N,1",���ֻд��������ȱʡΪ"8,N,1"
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PortOpen(EM_PORT_NUM emPort, const char *pszAttr);

/**
 *@brief	�رմ���
 *@param	emPort	ָ���Ĵ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_PortClose(EM_PORT_NUM emPort);

/**
 *@brief	���趨��ʱʱ�����ָ���Ĵ��ڣ���ȡָ�����ȵ����ݣ������pszOutbuf
 *@param	emPort	ָ���Ĵ���
 *@param	unLen	��ʾҪ�������ݳ��ȣ�>0
 *@param	nTimeoutMs	�ȴ�ʱ�䣬��λΪ����
 *@retval	pszOutbuf	�������ݻ�������ͷָ��
 *@retval	pnReadlen	���ض���ʵ�ʳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_PortRead(EM_PORT_NUM emPort, uint unLen, char *pszOutbuf,int nTimeoutMs, int *pnReadlen);

/**
 *@brief	��ָ���Ĵ�����ָ�����ȵ����ݣ������pszInbuf
 *@param	emPort	ָ���Ĵ���
 *@param	unLen	��ʾҪд�����ݳ���
 *@param	pszInbuf	���ݷ��͵Ļ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_PortWrite(EM_PORT_NUM emPort, uint unLen,const char *pszInbuf);

/**
 *@brief	�ж�ָ�����ڷ��ͻ������Ƿ�Ϊ��
 *@param	emPort	ָ���Ĵ���
 *@return
 *@li	NDK_OK	������������
 *@li	NDK_ERR	������������
*/
int NDK_PortTxSendOver(EM_PORT_NUM emPort);

/**
 *@brief	���ָ�����ڵĽ��ջ�����
 *@param	emPort	ָ���Ĵ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PortClrBuf(EM_PORT_NUM emPort);

/**
 *@brief	ȡ���������ж����ֽ�Ҫ����ȡ
 *@param	emPort	ָ���Ĵ���
 *@retval	pnReadlen	���ػ���������ȡ�ĳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PortReadLen(EM_PORT_NUM emPort,int *pnReadlen);

/** @} */ // ����ͨѶģ�����

/** @addtogroup modemģ��
* @{
*/

/**
*@brief  modem����״̬����
*/
typedef enum MDM_STATUS
{
	MDMSTATUS_NORETURN_AFTERPREDIAL=0,
	MDMSTATUS_OK_AFTERPREDIAL=1,
	MDMSTATUS_CONNECT_AFTERPREDIAL=2,
	MDMSTATUS_MS_NODIALTONE = -2,
	MDMSTATUS_MS_NOCARRIER =	-3,
	MDMSTATUS_MS_BUSY = -4,
	MDMSTATUS_MS_ERROR = -5,
	MDMSTATUS_LINE_VOL_TOO_LOW =-8,
	MDMSTATUS_NOPREDIAL = -11,
}EM_MDMSTATUS;
/**
*@brief  modem��ʼ�������Ĳ����������Ͷ���
*/
typedef enum MDM_Patchtype
{
	MDM_PatchType5=5,//Ĭ�ϵĲ�������ʽ
	MDM_PatchType4=4,
	MDM_PatchType3=3,//�������Ӳ�����
	MDM_PatchType2=2,
	MDM_PatchType1=1,
}EM_MDM_PatchType;

/**
 *@brief	ͬ�����ų�ʼ��������
 *@param	nType		��������������Ӧ��ͬ����·����������ʹ�á�
 *@return	��
  *@li	NDK_OK				�����ɹ�
 *@li	����NDK_ERR_MODEM_COUNTRYFAIL	�򲹶�ʧ��
                 NDK_ERR_MODEM_SDLCINITFAIL    ��ʼ��ʧ��
*/
int NDK_MdmSdlcInit(EM_MDM_PatchType nType);

/**
 *@brief	�첽modem��ʼ����
 *@param	nType 	��������������Ӧ��ͬ����·����������ʹ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	NDK_ERR_MODEM_ASYNINITFAIL	��ʼ��ʧ��
*/
int NDK_MdmAsynInit(EM_MDM_PatchType nType);

/**
 *@brief	modem���ź�����
 *@param	pszDailNum 	���ź���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����NDK_ERR_MODEM_SDLCDIALFAIL		ͬ������ʧ��
                 NDK_ERR_MODEM_INIT_NOT         ����ǰδ����ʼ��
*/
int NDK_MdmDial(const char * pszDailNum);

/**
 *@brief	���modem״̬��
 *@param	*pemStatus 	modem״̬��ʵ�ʷ���ֵ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MdmCheck(EM_MDMSTATUS  *pemStatus);

/**
 *@brief	modem���ݷ��͡�
 *@param	pszData 	���͵�����
 *@param	unDatalen 	���͵����ݳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����NDK_ERR_MODEM_SDLCWRITEFAIL		ͬ������ʧ��
				 NDK_ERR_MODEM_ASYNWRITEFAIL      �첽����ʧ��
				 NDK_ERR_MODEM_INIT_NOT                 δ��ʼ��
*/
int NDK_MdmWrite(const char *pszData,uint unDatalen);

/**
 *@brief	modem���ݽ��ա�
 *@param	*pszData 	���յ�����
 *@param	*punDatalen 	���յ����ݳ���
  *@param	unSenconds	��ʱʱ�䣬��SΪ��λ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����	NDK_ERR_MODEM_SDLCREADFAIL	ͬ�����ղ���ʧ��
					NDK_ERR_MODEM_ASYNREADFAIL   �첽����ʧ��
					NDK_ERR_MODEM_INIT_NOT                 δ��ʼ��
*/
int NDK_MdmRead(char  *pszData,uint *punDatalen,uint unSenconds);

/**
 *@brief	modem�ҶϺ�����
 *@param	��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MdmHangup(void);

/**
 *@brief	���modem��������
 *@param	��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MdmClrbuf(void);

/**
 *@brief	��ȡmodem���ȡ�
 *@param	*punReadlen 	���صĳ���ֵ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MdmGetreadlen(uint *punReadlen);

/**
 *@brief	modem��λ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MdmReset(void);

/**
 *@brief	AT�����������
 *@param	pucCmdstr 	��������
 *@param	pszRespData 	���ص���Ӧ����
 *@param	punLen 	���ص����ݳ���
 *@param	unTimeout 	��ʱʱ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MdmExCommand(uchar *pucCmdstr,uchar *pszRespData,uint *punLen,uint unTimeout);

/** @} */ // modemģ�����

/** @addtogroup ����ģ��
* @{
*/

/**
 *@brief  ����ģ�鳣�������
*/
typedef enum {
    WLM_CMD_E0=0,					/**<�رջ���*/
    WLM_CMD_CSQ=1,				/**<ȡ�ź�ֵ*/
    WLM_CMD_CREG=2,				/**<����ע��״̬*/
    WLM_CMD_CPIN=3,				/**<��ѯ��PIN*/
    WLM_CMD_CPIN0=4,			/**<����PIN��*/
    WLM_CMD_CGATT0=5,			/**<ע��GPRS����*/
    WLM_CMD_CGATT1=6,			/**<ע��GPRS����*/
    WLM_CMD_DIAL=7,				/**<����*/
    WLM_CMD_D2=8,					/**<����Ӳ���ҶϹ���*/
    WLM_CMD_COPS=9,				/**<ע������*/
    WLM_CMD_CGMR=10,			/**<ȡģ��汾*/
    WLM_CMD_CGSN,         /**<<��ȡGSN��(imei��meid)*/
    WLM_CMD_END,          /**<�����������������������֮ǰλ��*/
    WLM_CMD_UNDEFINE=1024,		/**<�б���δ����*/
    WLM_CMD_PUBLIC=255
} EM_WLM_CMD;

/**
 *@brief  ATָ�����ݰ�
*/
typedef struct NDK_WL_ATCMD{
		EM_WLM_CMD AtCmdNo;	/**<�����*/
		char *pcAtCmd;			/**<�����ִ�*/
		char *pcAddParam;		/**<���Ӳ���*/
}ST_ATCMD_PACK;

/**
 *@brief  ����ģ�鷵��״̬����
*/
typedef enum{
    WLM_STATUS_UNTYPED=4,	/**<δ���巵��*/
    WLM_STATUS_NO_CARRIER=3,/**<���ز�*/
    WLM_STATUS_RING=2,		/**<RING��*/
    WLM_STATUS_CONNECT=1,	/**<���Ӵ�*/
    WLM_STATUS_OK=0,				/**<�ɹ�*/
    WLM_STATUS_ERROR=-1,			/**<����*/
    WLM_STATUS_RET_ERROR=-114,	/**<���س���*/
}EM_WLM_STATUS;


/**
 *@brief	����MODEM��Ӳ����λ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlModemReset(void);

/**
 *@brief	�ر�����MODEMģ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlModemClose(void);

/**
 *@brief	����MODEM��ʼ�����л����ڵ����߲��ж�ģ��ATָ���ܷ�������Ӧ�����SIM��
 *@param	nTimeout	��ʱʱ�䣬��λMS
 *@param	pszPINPassWord	PIN��
 *@retval	pemStatus	ִ�гɹ���������״̬��ʧ�ܷ��� NDK_FAIL	ʧ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlInit(int nTimeout,const char *pszPINPassWord,EM_WLM_STATUS *pemStatus);


/**
 *@brief	��ȡ����MODEM�ź�ǿ��
 *@retval	pnSq	ȡ�����ź�ǿ�ȣ�ȡ����ֵ	0-31 Ϊ�ɹ���99	Ϊδ֪,-1 Ϊʧ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlModemGetSQ(int *pnSq);


/**
 *@brief	������ر�ϵͳ��ȡ����ģ���ź�ǿ��
 *@param	nx	��ʾ�����ź���ʼ������
 *@param	ny	��ʾ�����ź���ʼ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlShowSignalQuality(int nx, int ny);


/**
 *@brief	������ģ�鷢�ͺͽ���ATָ���pATCmdPackΪ��NULLʱ����ֱ�ӵȴ�����,������ATָ��;���pcOutputΪ��NULL�����������ATָ��
 *@param	pstATCmdPack	ATָ�����ݰ�
 *@param	unMaxlen	������󳤶ȣ�pszOutput���������ȣ���=0ʱʹ��ȱʡ����1024��
 *@param	unTimeout	���ʱʱ�䣬��λ��MS
 *@retval	pszOutput	�����
 *@retval	pemStatus	ִ�гɹ���������״̬��ʧ�ܷ��� NDK_FAIL	ʧ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlSendATCmd(const ST_ATCMD_PACK *pstATCmdPack,char *pszOutput,uint unMaxlen,uint unTimeout,EM_WLM_STATUS *pemStatus);

/**
 *@brief	�ر���Ƶ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlCloseRF(void);

/**
 *@brief	ѡ��SIM��
 *@param	ucSimNo	����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_WlSelSIM(uchar ucSimNo);


/** @} */ // ����ģ�����


/** @addtogroup SocketͨѶ
* @{
*/


/**
 *@brief	��TCPͨѶͨ��
 *@retval	pnFd	����TCPͨ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpOpen(int *pnFd);

/**
 *@brief	�ر�TCPͨѶͨ��
 *@param	unFd	Ҫ�رյ�TCPͨ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpClose(uint unFd);

/**
 *@brief	�ȴ�TCP�رճɹ���һ���رռ�ʱ�˳�������NDK_TcpClose()�󣬱���������øú���ȷ��TCP��·��ȫ�ر�
 *@param	unFd	TCPͨ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpWait(uint unFd);

/**
 *@brief	�󶨱��˵�IP��ַ�Ͷ˿ں�
 *@param	unFd		TCPͨ�����
 *@param	pszMyIp	Դ��ַ
 *@param	usMyPort	Դ�˿�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpBind(uint unFd, const char *pszMyIp, ushort usMyPort);

/**
 *@brief	���ӷ�����
 *@param	unFd	TCPͨ�����
 *@param	pszRemoteIp	Զ�̵�ַ
 *@param	usRemotePort	Զ�̶˿�
 *@param	unTimeout	Զ�����ӳ�ʱʱ�䣬��λΪ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpConnect(uint unFd, const char *pszRemoteIp, ushort usRemotePort, uint unTimeout);

/**
 *@brief	�������������
 *@param	unFd	TCPͨ�����
 *@param	nBacklog	�ȴ����Ӷ��е���󳤶�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpListen(uint unFd, int nBacklog);

/**
 *@brief	������������
 *@param	unFd	TCPͨ�����
 *@param	pszPeerIp	��������ʵ��ĵ�ַ
 *@param	usPeerPort	��������ʵ��Ķ˿�
 *@retval	punNewFd	����TCPͨ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpAccept(uint unFd, const char *pszPeerIp, ushort usPeerPort, uint *punNewFd);

/**
 *@brief	��������
 *@param	unFd	TCPͨ�����
 *@param	pInbuf	���ͻ������ĵ�ַ
 *@param	unLen	�������ݵĳ���
 *@param	unTimeout	��ʱʱ�䣬��λΪ��
 *@retval	punWriteLen	����ʵ�ʷ��ͳ���,���ΪNULL�򲻽���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpWrite(uint unFd, const void *pInbuf, uint unLen, uint unTimeout, uint *punWriteLen);

/**
 *@brief	��������
 *@param	unFd	TCPͨ�����
 *@param	unLen	�������ݵĳ���
 *@param	unTimeout	��ʱʱ�䣬��λΪ��
 *@retval	pOutbuf	���ջ������ĵ�ַ
 *@retval	punReadlen	����ʵ�ʷ��ͳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_TcpRead(uint unFd, void *pOutbuf, uint unLen, uint unTimeout, uint *punReadlen);

/**
 *@brief	��UDPͨѶͨ��
 *@retval	pnFd	����UDPͨ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_UdpOpen(int *pnFd);


/** @} */ // SocketͨѶģ�����


/** @addtogroup PPPͨѶ
* @{
*/

/**
 *@brief  PPP����״̬����
*/
typedef enum {
    PPP_STATUS_DISCONNECT=0,		/**<PPPδ����*/
    PPP_STATUS_CONNECTING=2,		/**<PPP��������*/
    PPP_STATUS_CONNECTED=5,			/**<PPP������*/
    PPP_STATUS_DISCONNECTING=6		/**<���ڹҶ�*/
} EM_PPP_STATUS;

/**
 *@brief  PPP���ò���
*/
typedef struct {
    int nDevType;			/**<���ͣ�����������MODEM��������MODEM��0��ʾ���ߣ�1��ʾ����*/
    unsigned int nPPPFlag;	/**<�Ƿ�֧��ά�ֳ����ӱ�ʶ��������Ҫ�������Ӧ�ĺ궨��*/
    char szApn[64];			/**<APN����*/
    char szDailNum[32];		/**<���ź���*/
    int (*ModemDial)(void);/**<���ź���*/
    unsigned int PPPIntervalTimeOut; /**<ά�ֳ����ӵ����ݰ����͵�ʱ����,<30S��������г����ӵ�ά��*/
    unsigned char nMinSQVal;	/**<��ʼ��ʱ���������С���ź�ֵ*/
    char szPin[31];				/**<SIM��PIN��*/
    char nPPPHostIP[20];		/**<ά�ֳ�������ҪPIN������IP*/
} ST_PPP_CFG;

/**
 *@brief	����PPP����
 *@param	pstPPPCfg	ppp�����ṹ(�豸��������ȱʡʱΪ����)
 *@param	nuValidLen	������Ч����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_PppSetCfg(ST_PPP_CFG *pstPPPCfg, uint nuValidLen);

/**
 *@brief	PPP����
 *@param	pszUsername	�û���
 *@param	pszPassword	����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_PppDial(const char *pszUsername,const char *pszPassword);

/**
 *@brief	PPP�Ҷ�
 *@param	nHangupType	�Ҷ����� 0 �������Ҷ� 1 �����Ҷ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PppHangup(int nHangupType);

/**
 *@brief	��ȡPPP״̬
 *@retval	pemStatus	����PPP״̬,ΪNULL��ִ�иò���
 *@retval	pnErrCode	����PPP���Ӵ���,ΪNULL��ִ�иò���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PppCheck(EM_PPP_STATUS *pemStatus, int *pnErrCode);

/**
 *@brief	��ȡ���ص�ַ��������ַ
 *@retval	punLocalAddr	���ر��ص�ַ,ΪNULL�򲻽���
 *@retval	punHostAddr	����������ַ,ΪNULL�򲻽���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PppGetAddr(uint *punLocalAddr, uint *punHostAddr);

/**
 *@brief	��һ�����ʮ���Ƶ�IPת����һ������������
 *@param	pszIp	IP��ַ�ַ���
 *@retval	pulIpAddr	����ת����ĳ�������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PppAddrChange(register const char *pszIp, ulong *pulIpAddr);

/** @} */ // PPPͨѶģ�����

/** @addtogroup ��̫��ͨѶ
* @{
*/


/**
 *@brief	���������ַ,������ΪNULL���ز�������(���ú����ԭ�Ѷ�̬����ĵ�ַ,Ҳ�ᱻ�޸�)
 *@param	pszIp	����IP ��ַ�ַ�������ָ��,ΪNULL����IP��ַ.��֧��IPV4Э��
 *@param	pszMask	�������������ַ�������ָ��,ΪNULL����Mask��ַ.��֧��IPV4Э��
 *@param	pszGateway	�������ص�ַ�ַ�������ָ��,ΪNULL����Gateway��ַ.��֧��IPV4Э��
 *@param 	pszDns	����DNS������IP��ַ,ΪNULL����DNS��ַ,��������3��DNS,֮���Էֺŷ�';'����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_EthSetAddress(const char *pszIp, const char *pszMask, const char *pszGateway, const char *pszDns);


/**
 *@brief	��ȡ����MAC��ַ
 *@retval	pszMacAddr	����MAC��ַ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_EthGetMacAddr(char *pszMacAddr);


/**
 *@brief	��ȡ�����ַ,������ΪNULL���ز�������
 *@retval	pszIp	����IP��ַ,ΪNULL��ȡIP��ַ
 *@retval	pszGateway	�������ص�ַ,ΪNULL��ȡGateway��ַ
 *@retval	pszMask	������������,ΪNULL��ȡMask��ַ
 *@retval	pszDns	DNS������IP��ַ,ΪNULL��ȡDNS��ַ,һ����ȡ������DNS,֮���Էֺŷ�';'����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_EthGetNetAddr(char *pszIp, char *pszMask, char *pszGateway, char *pszDns);


/**
 *@brief	������̫������ģʽ
 *@param	nMode	����ģʽ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_EthSetWorkMode(int nMode);


/**
 *@brief	ʹ��dhcp��ȡ�����ַ
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_NetDHCP(void);


/**
 *@brief	����PING����
 *@param	pszIp	����IP��ַ�ַ�������ָ��,����Ϊ��ָ��.��֧��IPV4Э��
 *@param	nTimeoutMs	��ʱʱ��,��λΪ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_NetPing(char *pszIp, uint nTimeoutMs);


/** @} */ // ��̫��ͨѶģ�����


/** @addtogroup �ſ�
* @{
*/

/**
 *@brief	�򿪴ſ��豸
 *@param	��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_MagOpen(void);

/**
 *@brief	�رմſ��豸
 *@param	��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_MagClose(void);


/**
 *@brief	��λ��ͷ
 *@details	 ��λ��ͷ������ſ�����������
 *@param	��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_MagReset(void);


/**
 *@brief	�ж��Ƿ�ˢ����
 *@retval	pcSwiped	1----��ˢ��    0-----δˢ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_MagSwiped(uchar * pcSwiped);


/**
 *@brief	��ȡ�ſ���������1��2��3�ŵ�������
 *@details	��MagSwiped�������ʹ�á��������Ҫĳ�ŵ�����,���Խ��ôŵ���Ӧ��ָ����ΪNULL,��ʱ����������ôŵ�������
 *@retval	pszTk1	�ŵ�1
 *@retval	pszTk2	�ŵ�2
 *@retval	pszTk3	�ŵ�3
 *@retval	pnErrorCode	�ſ��������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_MagReadNormal(char *pszTk1, char *pszTk2, char *pszTk3, int *pnErrorCode);


/**
 *@brief	��ȡ�ſ���������1��2��3�ŵ���ԭʼ����
 *@details	��MagSwiped�������ʹ��,�������Ҫĳ�ŵ�����,���Խ��ôŵ���Ӧ��ָ����ΪNULL,��ʱ����������ôŵ�������
 *@retval	pszTk1	�ŵ�1
 *@retval	punTk1Len	�ŵ�1���ݳ���
 *@retval	pszTk2	�ŵ�2
 *@retval	punTk2Len	�ŵ�2���ݳ���
 *@retval	pszTk3	�ŵ�3
 *@retval	punTk3Len	�ŵ�3���ݳ���

 *@return:
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_MagReadRaw(uchar *pszTk1, ushort* punTk1Len, uchar *pszTk2, ushort* punTk2Len,uchar *pszTk3, ushort* punTk3Len );

/** @} */ // �ſ�ģ�����

/** @addtogroup IC��
* @{
*/

typedef enum{
  ICTYPE_IC,  /**<�Ӵ�ʽIC��*/
  ICTYPE_SAM1, /**<SAM1��*/
  ICTYPE_SAM2, /**<SAM2��*/
  ICTYPE_SAM3, /**<SAM3��*/
  ICTYPE_SAM4, /**<SAM4��*/
  ICTYPE_M_1, /**<at24c01 at24c02 at24c04 at24c08 at24c16 at24c32 at24c64 */
  ICTYPE_M_2, /**<sle44x2*/
  ICTYPE_M_3, /**<sle44x8*/
  ICTYPE_M_4, /**<at88sc102*/
  ICTYPE_M_5, /**<at88sc1604*/
  ICTYPE_M_6, /**<at88sc1608*/
}EM_ICTYPE;

/**
 *@brief	��ȡ��������汾��
 *@retval 	pszVersion   ������������汾��,Ҫ�󻺳��С������16�ֽ�
 *��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int  NDK_IccGetVersion(char *pszVersion);

/**
 *@brief	���ÿ�����
 *@param	emIctype��  �����͡�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_IccSetType(EM_ICTYPE emIctype);

/**
 *@brief	��ȡ������
 *@retval 	pemIctype �����͡�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_IccGetType(EM_ICTYPE *pemIctype);

/**
 *@brief	��ȡ��Ƭ״̬
 *@retval 	pnSta   bit0�����IC��1�Ѳ忨��Ϊ��1��������Ϊ��0��
 *					bit1�����IC��1���ϵ磬Ϊ��1��������Ϊ��0��
 *					bit2�����SAM��1���ϵ磬Ϊ��1��������Ϊ��0��
 *					bit3�����SAM��2���ϵ磬Ϊ��1��������Ϊ��0��
 *					bit4�����SAM��3���ϵ磬Ϊ��1��������Ϊ��0��
 *					bit5�����SAM��4���ϵ磬Ϊ��1��������Ϊ��0��
 *					bit6������������ֵ��0��
 *					bit7������������ֵ��0��
 *��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_IccDetect(int *pnSta);


/**
 *@brief	�ϵ�
 *@param	emIctype�� ICC_IC �Ӵ�ʽIC������
 *                    ICC_SAM1���ӣ��ͣ�����
 *					  ICC_SAM2���ӣ��ͣ�����
 *					  ICC_SAM3���ӣ��ͣ�����
 *					  ICC_SAM4���ӣ��ͣ�����
 *@retval ����psAtrbuf  ��ʾATR����
 *@retval     pnAtrlen  ��ʾATR���ݵĳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_IccPowerUp (EM_ICTYPE emIctype, uchar *psAtrbuf,int *pnAtrlen);

/**
 *@brief	�µ�
 *@param	emIctype�� ICC_IC �Ӵ�ʽIC������
 *                    ICC_SAM1���ӣ��ͣ�����
 *					  ICC_SAM2���ӣ��ͣ�����
 *					  ICC_SAM3���ӣ��ͣ�����
 *					  ICC_SAM4���ӣ��ͣ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_IccPowerDown(EM_ICTYPE emIctype);

/**
 *@brief	IC������
 *@param	emIctype�� ICC_IC �Ӵ�ʽIC������
 *                    ICC_SAM1���ӣ��ͣ�����
 *					  ICC_SAM2���ӣ��ͣ�����
 *					  ICC_SAM3���ӣ��ͣ�����
 *					  ICC_SAM4���ӣ��ͣ�����
 *@param �� nSendlen  �� �������ݵĳ���
 *@param	psSendbuf�������͵�����
 *@retval 	pnRecvlen    �������ݳ���
 *@retval 	psRecebuf    ���յ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR			����ʧ��
*/
int NDK_Iccrw(EM_ICTYPE emIctype, int nSendlen,  uchar *psSendbuf, int *pnRecvlen,  uchar *psRecebuf);

/** @} */ // IC��ģ�����

/** @addtogroup ��Ƶ��
* @{
*/
typedef enum {
	RFID_Autoscan=		0x00,
	RFID_RC531=			0x01,
	RFID_PN512=			0x02,
	RFID_No=			0xFF,
}EM_RFID;

/**
 *@brief	��ȡ��Ƶ�����汾��
 *@param	pszVersion	���ص������汾���ַ���
 *@return
 *@li	NDK_OK			�����ɹ�
 *@li	EM_NDK_ERR		����ʧ��
*/
int  NDK_RfidVersion(uchar *pszVersion);


/**
 *@brief	��Ƶ�ӿ�������ʼ�����������ж������Ƿ�װ���ɹ���
 *@retval 	psStatus  ���䱸��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidInit(uchar *psStatus);


/**
 *@brief	����Ƶ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidOpenRf(void);


/**
 *@brief	�ر���Ƶ������ɽ��͹��Ĳ�������Ƶ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidCloseRf(void);


/**
 *@brief	��ȡ��Ƭ״̬(�Ƿ���Ѱ���������ж��Ƿ������)
 *@return
 *@li	NDK_OK		��Ѱ���ɹ�(æ״̬)
 *@li	NDK_ERR_RFID_NOTACTIV		δѰ���ɹ�(��æ״̬)
*/
int NDK_RfidPiccState(void);


/**
 *@brief	�豸ǿ������
 *@return
 *@li	NDK_OK		ִ�гɹ�
 *@li	NDK_ERR		ִ��ʧ��
*/
int NDK_RfidSuspend(void);


/**
 *@brief	�豸����
 *@return
 *@li	NDK_OK		ִ�гɹ�
 *@li	NDK_ERR		ִ��ʧ��
*/
int NDK_RfidResume(void);


/**
 *@brief	����Ѱ������(Ѱ������ǰ����һ�μ��ɣ�M1������ʱ��Ҫ���ó�TYPE-A��ģʽ)
 *@param	ucPicctype = 0xcc����ʾѰ��ʱֻ���TYPE-A�Ŀ�.
 *			   0xcb����ʾѰ��ʱֻ���TYPE-B�Ŀ�.
 *			   0xcd����ʾѰ��ʱ���TYPE-A��TYPE-B�Ŀ�.
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidPiccType(uchar ucPicctype);


/**
 *@brief	��Ƶ��Ѱ��(̽��ˢ�������Ƿ��п�)
 *@retval 	psPicctype	����Ŀ����� 0xcc=TYPE-A����0xcb=TYPE-B��
 *@return
 *@li	NDK_OK				Ѱ���ɹ�����������picctype
 *@li	����EM_NDK_ERR		Ѱ��ʧ��
*/
int NDK_RfidPiccDetect(uchar *psPicctype);


/**
 *@brief	��Ƶ������(�п��������),�൱��powerup ��
 *@retval 	psPicctype	����Ŀ����� 0xcc=TYPE-A����0xcb=TYPE-B��
 *@retval	pnDatalen	 ���ݳ���
 *@retval	psDatabuf	���ݻ�����(A��ΪUID��B��databuf[1]~[4]ΪUID��������Ӧ�ü�Э����Ϣ)
 *@return
 *@li	NDK_OK				����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidPiccActivate(uchar *psPicctype, int *pnDatalen,  uchar *psDatabuf);


/**
 *@brief	�ر���Ƶʹ��ʧЧ����д����������Ӧ��ִ�иò������൱��powerdown ��
 *@param	ucDelayms	��0��һֱ�ر�RF;����0��ر�ucDelayms������ٴ�RF��
 *                      �ر�6��10ms��ʹ��ʧЧ���������û�������Ķ���������Ӧ����0�Թر�RF ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidPiccDeactivate(uchar ucDelayms);


/**
 *@brief	��Ƶ��APDU����,����д������(�Ѽ���������))
 *@param	nSendlen		���͵������
 *@param	psSendbuf		�����������
 *@retval 	pnRecvlen	�������ݳ���
 *@retval	psRecebuf	�������ݻ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RfidPiccApdu(int nSendlen, uchar *psSendbuf, int *pnRecvlen,  uchar *psRecebuf);


/**
 *@brief	M1Ѱ��(Ѱ�����ͱ����Ѿ�����ΪTYPE-A)
 *@param	ucReqcode		0=����REQA, ��0=����WUPA, һ���������Ҫʹ��WUPA
 *@retval 	pnDatalen	 �������ݳ���(2�ֽ�)
 *@retval	psDatabuf		�������ݻ�����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Request(uchar ucReqcode, int *pnDatalen, uchar *psDatabuf);


/**
 *@brief	M1������ͻ(NDK_RfidPiccDetect̽���п��������)
 *@retval 	pnDatalen	�������ݳ���(UID����)
 *@retval	psDatabuf	�������ݻ�����(UID)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Anti(int *pnDatalen, uchar *psDatabuf);


/**
*@brief		M1��ѡ��(NDK_M1Anti�ɹ��������)
*@param		nUidlen			uid����
*@param		pnUidbuf	     uid���ݻ�����
*@retval 	psSakbuf	    ѡ����������(1�ֽ�SAK)
*@return
*@li	NDK_OK				�����ɹ�
*@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Select(int nUidlen, uchar *pnUidbuf, uchar *psSakbuf);


/**
 *@brief	M1����֤��Կ�洢(ͬһ����Կ�洢һ�μ���)
 *@param	ucKeytype		��֤��Կ���� A=0x00 ��B=0x01
 *@param	ucKeynum	 ��Կ���к�(0~15)
 *@param	psKeydata		��Կ,6�ֽ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1KeyStore(uchar ucKeytype,  uchar ucKeynum, uchar *psKeydata);


/**
 *@brief	M1��װ���Ѵ洢����Կ(ͬһ����Կ����һ�μ���)
 *@param	ucKeytype	��֤��Կ���� A=0x00 ��B=0x01
 *@param    ucKeynum	 ��Կ���к�(0~15��A/B����16����Կ)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1KeyLoad(uchar ucKeytype,  uchar ucKeynum);


/**
 *@brief	M1�����Ѽ��ص���Կ��֤
 *@param	nUidlen	uid����
 *@param	psUidbuf	uid����(NDK_M1Anti��ȡ��)
 *@param	ucKeytype	��֤��Կ���� A=0x00 ��B=0x01
 *@param    ucBlocknum	Ҫ��֤�Ŀ��(ע��:����������!)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1InternalAuthen(int nUidlen, uchar *psUidbuf, uchar ucKeytype, uchar ucBlocknum);


/**
 *@brief	M1��ֱ�����KEY��֤
 *@param	nUidlen	uid����
 *@param	psUidbuf	uid����(NDK_M1Anti��ȡ��)
 *@param	ucKeytype	��֤��Կ���� A=0x00 ��B=0x01
 *@param	psKeydata	key(6�ֽ�)
 *@param    ucBlocknum	Ҫ��֤�Ŀ��(ע��:����������!)

 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1ExternalAuthen(int nUidlen, uchar *psUidbuf, uchar ucKeytype, uchar *psKeydata, uchar ucBlocknum);


/**
 *@brief	M1��'��'��ȡ����
 *@param	ucBlocknum	Ҫ���Ŀ��
 *@retval	pnDatalen	��ȡ�Ŀ����ݳ���
 *@retval	psBlockdata	������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Read(uchar ucBlocknum, int *pnDatalen, uchar *psBlockdata);


/**
 *@brief	M1��'��'д����
 *@param	ucBlocknum	Ҫд�Ŀ��
 *@param	pnDatalen	��ȡ�Ŀ����ݳ���
 *@param	psBlockdata	������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Write(uchar ucBlocknum, int *pnDatalen, uchar *psBlockdata);


/**
 *@brief	M1��'��'��������
 *@param	ucBlocknum	ִ�����������Ŀ�š�ע�⣺��������ֻ��ԼĴ�����δ����д��������������⣬���Ŀ����ݱ���������/������ʽ��
 *@param	nDatalen	�������ݳ���(4�ֽ�)
 *@param	psDatabuf	��������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Increment(uchar ucBlocknum, int nDatalen, uchar *psDatabuf);


/**
 *@brief	M1��'��'��������
 *@param	ucBlocknum	ִ�м��������Ŀ�š�ע�⣺��������ֻ��ԼĴ�����δ����д��������������⣬���Ŀ����ݱ���������/������ʽ��
 *@param	nDanalen	�������ݳ���(4�ֽ�)
 *@param	psDatabuf	��������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Decrement(uchar ucBlocknum, int nDanalen, uchar *psDatabuf);


/**
 *@brief	M1����/����������Ĵ��Ͳ���(�Ĵ���ֵ����д�뿨�Ŀ�������)
 *@param	ucBlocknum	ִ�м��������Ŀ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Transfer(uchar ucBlocknum);


/**
 *@brief	M1���Ĵ���ֵ�ָ�����(�ָ��Ĵ�����ʼֵ��ʹ֮ǰ����/����������Ч)
 *@param	ucBlocknum:   ִ�м��������Ŀ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_M1Restore(uchar ucBlocknum);


/**
*@brief	���׿���Ѱ��(���ڲ��ԵȲ����мӿ��ٶ�)
*@param	nModecode	  =0����Ѱ������0����Ѱ��
*@return
*@li	NDK_OK				�����ɹ�
*@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_PiccQuickRequest(int nModecode);


/**
 *@brief	���ζ�ISO1443-4Э��֧�ֵ��ж�
 *@param	nModecode	��0��ִ������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SetIgnoreProtocol(int nModecode);


/**
*@brief	��ȡ����ISO1443-4Э��֧�ֵ�����
*@retval	pnModecode	��0��ִ������
*@return
*@li	NDK_OK				�����ɹ�
*@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_GetIgnoreProtocol(int *pnModecode);


/**
 *@brief	��ȡ��Ƶ�ӿ�оƬ����
 *@retval	pnRfidtype	��/ref EM_RFID "EM_RFID"
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int  NDK_GetRfidType(int *pnRfidtype);

/** @} */ // ��Ƶ��ģ�����

/** @addtogroup ϵͳ�ӿ�
* @{
*/

/**
 *@brief  ȡPOS�汾��Ӳ����Ϣ��������
 *@details SYS_HWINFO_GET_POS_TYPE ���صĻ����ַ�����ʽΪ"NLGP-XXX"����"NLGP-730"��"NLGP-8510"\n
		   SYS_HWINFO_GET_HARDWARE_INFO ����Ӳ����Ϣ�������£�
				 ��ȡPOSӲ����Ϣÿһ���ֽڴ���ͬ��ģ��\n
				 ͬһ�ֽ��ڲ�ͬ��ֵ����ͬһģ��Ĳ�ͬоƬ����\n
				 ����֧��63���ⲿ�豸��Ӧ�ô���������鲻С��64 \n\n
				 �ο�\ref EM_SYS_HWTYPE "EM_SYS_HWTYPE"
				 
   pinfo[0]:����ģ��\n
    0xFF     :û������ģ��\n
    0x01     :SIM300	\n
    0x02    :M72	\n	
    0x03    :DTGS800	\n
    0x04    :SM700	\n
    0x05    :MC39I	\n
    0x06    :DTM228C	\n
    0x07    :MC8331	\n
    0x08    :EM200	\n\n
   pinfo[1]:��Ƶģ��	\n
    0xFF     :û����Ƶģ��	\n
    0x01     :RC531	\n
    0x02    :PN512	\n\n
   pinfo[2]:�ſ�ģ��	\n
    0xFF     :û�дſ�ģ��	\n
    0x01     :giga	\n
    0x02     :mesh	\n\n
   pinfo[3]:ɨ��ͷģ��	\n
    0xFF     :û�дſ�ģ��	\n
    0x01     :EM1300	\n
    0x02     :EM3000	\n
    0x03     :SE955	\n\n
   pinfo[4]:�Ƿ�֧������������	\n
    0xFF     :��֧��	\n
    0x01     :֧��	\n\n
   pinfo[5]:���ڸ���	\n
    0xFF     :��	\n
    0x01     :1��	\n
    0x02     :2��	\n\n
   pinfo[6]:�Ƿ�֧��USB	\n
    0xFF     :��	\n
    0x01     :��	\n\n
   pinfo[7]:MODEMģ��	\n
    0xFF     :��	\n
    0x01     :��	\n\n
   pinfo[8]:wifiģ��	\n
    0xFF     :��wifiģ��	\n
    0x01     :��"USI WM-G-MR-09"ģ��	\n\n
   pinfo[9]:�Ƿ�֧����̫��	\n
    0xFF     :��	\n
    0x01     :��	\n\n
   pinfo[10]:��ӡģ��	\n
    0xFF     :�޴�ӡģ��	\n
    0x01~0x7F     :����	\n
    0x82~0x7E     :���	\n\n
   pinfo[11]:�Ƿ�֧�ִ���	\n
    0xFF     :��	\n
    0x01     :��	\n\n
   pinfo[12]:�Ƿ�����ƵLED��	\n
    0xFF     :��	\n
    0x01     :��	\n
*/
typedef enum {
	SYS_HWINFO_GET_POS_TYPE=0,      		/**<ȡpos��������   			*/
	SYS_HWINFO_GET_HARDWARE_INFO,       /**<��ȡPOS������֧��Ӳ�����ͣ���ϸ����ֵ��������*/
	SYS_HWINFO_GET_BIOS_VER,        		/**<ȡbios�汾��Ϣ 			 */
	SYS_HWINFO_GET_POS_USN,        		/**<ȡ�������к�    		*/
	SYS_HWINFO_GET_POS_PSN,        		/**<ȡ����������    		*/
	SYS_HWINFO_GET_BOARD_VER,       		/**<ȡ�����        			*/
	SYS_HWINFO_GET_CREDITCARD_COUNT,		/**<ȡposˢ������					*/
	SYS_HWINFO_GET_PRN_LEN,				/**<ȡpos��ӡ�ܳ���    		*/
	SYS_HWINFO_GET_POS_RUNTIME,          /**<ȡpos����������ʱ��  */
	SYS_HWINFO_GET_KEY_COUNT,            /**<ȡpos����������  */
	SYS_HWINFO_GET_CPU_TYPE,
} EM_SYS_HWINFO;

/**
 *@brief  ��ȡϵͳ������Ϣ��������
*/
typedef enum {
	SYS_CONFIG_SLEEP_ENABLE,	    /**<����ʹ�� 0:��ֹ 1:���� */
	SYS_CONFIG_SLEEP_TIME,      	/**<��������ʱ��ǰ����ʱ��*/
	SYS_CONFIG_SLEEP_MODE,      	/**<����ģʽ 1:ǳ���� 2:������*/
	SYS_CONFIG_LANGUAGE,			/**<��ȡϵͳ���� 0:���� 1:english */
	SYS_CONFIG_APP_AUTORUN,      	/**<�����Զ��������س��� 0:���� 1:����*/	
} EM_SYS_CONFIG;

/**
 *@brief  Ӳ���豸��Ϣ����
*/
typedef enum {
	SYS_HWTYPE_WIRELESS_MODEM=0,	/**<����modem */
	SYS_HWTYPE_RFID,				/**< ��Ƶ��*/
	SYS_HWTYPE_MAG_CARD,			/**< �ſ�*/
	SYS_HWTYPE_SCANNER,				/**< ɨ��ͷ*/
	SYS_HWTYPE_PINPAD,				/**< �������*/
	SYS_HWTYPE_AUX,					/**< ����*/
	SYS_HWTYPE_USB,					/**< USB*/
	SYS_HWTYPE_MODEM,				/**< modem*/
	SYS_HWTYPE_WIFI,				/**< wifi*/
	SYS_HWTYPE_ETHERNET,			/**< ��̫��*/
	SYS_HWTYPE_PRINTER,				/**< ��ӡ��*/
	SYS_HWTYPE_TOUCHSCREEN,			/**< ������*/
	SYS_HWTYPE_RFIDLED,				/**< ��ƵLED��*/
	SYS_HWTYPE_MAX					/**< ���ֵ��ֻ����������*/
} EM_SYS_HWTYPE;

/**
 *@brief	��ȡNDK��汾��
 *@retval   pszVer	�汾���ַ���,�����С������8�ֽ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_Getlibver(char *pszVer);

/**
 *@brief 		ϵͳ��ʼ��(��δʹ��)
 *@details
 *@return
 *@li	 fd				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��

*/
int NDK_SysInit(void);
/**
 *@brief 		POS����
 *@details
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysReboot(void);
/**
 *@brief 		POS�ػ�
 *@details
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysShutDown(void);
/**
 *@brief 		Beepֻ��һ�������Ҫ������������������м����ʱ
 *@details
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysBeep(void);
/**
 *@brief 		����beep������
 *@details
 *@param    unVolumn    ��Ҫ���õ������Ĳ�����������ΧΪ0~5�������õײ�Ĭ��Ϊ5
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysSetBeepVol(uint unVolumn);
/**
 *@brief 		ȡbeep������
 *@details
 *@retval    punVolumn    ��Ҫ���õ������Ĳ���
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysGetBeepVol(uint *punVolumn);
/**
 *@brief 		��һ����Ƶ����һ����ʱ��
 *@details
 *@param    unFrequency������Ƶ�ʣ���λ:Hz    ����ΧΪ0 < unFrequency <=4000
 *@param    unSeconds����������ʱ�䣬��λ:ms   ����ΧΪunSeconds > 0
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysTimeBeep(uint unFrequency,uint unSeconds);
/**
 *@brief 		�����Ƿ������Զ���������
 *@param    unFlag  0:�������Զ��������ߣ�1:�����Զ��������ߣ�����ֵ�������Ϸ�
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysSetSuspend(uint unFlag);
/**
 *@brief 		�����Ƿ�������������
 *@details  �����Ƿ��Զ��������߿��ضԴ˺�����Ӱ�졣ֻҪ���û���������������
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysGoSuspend(void);
/**
 *@brief 		ȡ��Դ����
 *@retval   punVol  ����в��Դ��Ϊ0�����򷵻ص�ص���
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysGetPowerVol(uint *punVol);
/**
 *@brief 		��λ��ʱ( ��λʱ��Ϊ0.1s)
 *@details
 *@param    unDelayTime ��ʱʱ�䣬��ΧunDelayTime > 0
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysDelay(uint unDelayTime);
/**
 *@brief 		��λ��ʱ (��λʱ��Ϊ0.1s)
 *@details
 *@param    unDelayTime ��ʱʱ�䣬��ΧunDelayTime > 0
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysMsDelay(uint unDelayTime);
/**
 *@brief 		ȡPOS��ǰʱ��
 *@details
 *@param     pstTime  ����tm�ṹ�����͵�ָ�룬���ص�ǰposʱ��
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysGetPosTime(struct tm *pstTime);

/**
 *@brief 		����POS��ǰʱ��
 *@details
 *@param     stTime  ����tm�ṹ�����͵ı���������posʱ��Ϊ����time��ʱ��
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysSetPosTime(struct tm stTime);

/**
 *@brief 		  ��ȡָ���ֿ������(�ӿ��ݶ���δʵ��)
 *@details    ����ƫ��ȡָ���ֿ��n���ֽڵ�����
 *@param      pPath �ֿ����ڵ�·��
 *@param      unOffset �ֿ�ƫ����
 *@param      unLen Ҫȡ�ֿ��ֽ���
 *@retval     psBuf ���ڴ洢ȡ�������ֿ�������Ϣ
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysReadFont(const char * pPath,uint unOffset,char *psBuf,uint unLen);
/**
 *@brief 		����pos��Ƶ4�ƵƵ�����״̬
 *@details
 *@param      nLed    nLed �ĵ�4λ��ʾ4�Ƶơ����ҵ���ֱ���1��2��3��4������(nLed&(1 << 0)) !=0  �Ļ���ʾ������һ�Ƶ�
 *@param      nStatus  nStatus �ĵ�8λ��ʾ4�ƵƵ�״̬��ÿ��λΪ��ʾһ�ƵƵ���������ҵ���ֱ���1��2��3��4��
 		     ����(nStatus&3)Ϊ��һ�Ƶ�Ҫ���Ƶ�״̬��((nStatus>>2)&3)Ϊ�ڶ��Ƶơ�
 		     0��ʾ״̬���䣬Ϊ1��ʾ״̬Ϊ����2Ϊ��3Ϊ����
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_LedStatus(int nLed,int nStatus);
/**
 *@brief 		�����ܱ���ʼ��ʱ
 *@details  ��NDK_SysStartWatch()��NDK_SysStopWatch()���ʹ�á�������1��������
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysStartWatch(void);
/**
 *@brief 		ֹͣ�ܱ��������ֵ
 *@details
 *@retval   punTime �ܱ����ʱ�ļ���ֵ
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysStopWatch(uint *punTime);
/**
 *@brief 	��ȡposӲ����Ϣ�ӿ�
 *@details	��������ȡӲ����Ϣ������emFlag���ڷ�Χ�ڣ��򷵻ز����������ûȡ���汾��Ϣ����NDK_ERR
 			����Ĳ����������С���ݶ�Ϊ100�ֽ�,��С������16�ֽڡ�apiֻ����ǰ100���ֽڵ���Ϣ
 *@param    emFlag ��Ҫ��ȡ�豸��Ϣ��������
 *@retval   punLen ���ش��ص�buf��Ϣ�ĳ���
 *@retval   psBuf���ڴ洢���ص���Ϣ
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SysGetPosInfo(EM_SYS_HWINFO emFlag,uint *punLen,char *psBuf);

/**
 *@brief    ��ȡϵͳ������Ϣ
 *@param    emConfig ��Ҫ��ȡ������Ϣ��������
 *@retval   pnvalue ���ص�����ֵ
 *@return
 *@li	 NDK_OK				�����ɹ�
 *@li	����NDK_ERRCODE		����ʧ��
*/
int NDK_SysGetConfigInfo(EM_SYS_CONFIG emConfig,int *pnvalue);

/** @} */ // ϵͳ�ӿڽ���

/** @addtogroup ����
* @{
*/

/**
 *@brief	2����󲻳���12λ���޷������ִ��ӷ�
 *@details	2�����ִ������λ��ӣ���ӽ�����ܳ���12λ
 *@param	pszDigStr1		���ִ�1
 *@param	pszDigStr2		���ִ�2
 *@param	pnResultLen		���������pszResult�Ĵ�С
 *@retval	pszResult		��Ӻ͵����ִ�
 *@retval	pnResultLen		��Ӻ͵�λ��
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AddDigitStr(const uchar *pszDigStr1, const uchar *pszDigStr2, uchar* pszResult, int *pnResultLen );

/**
 *@brief	��6λ���ִ�pcStrNum����1��Ż�ԭֵ
 *@param	pszStrNum		��Ҫ�����ӵ����ִ�,��������������Ϊ7
 *@retval	pszStrNum		���Ӻ�Ľ����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IncNum (uchar * pszStrNum );

/**
 *@brief	�Ѵ�С����Ľ���ַ���תΪ����С����Ľ���ַ���
 *@param	pszSource		��ת���Ľ���ַ���
 *@param	pnTargetLen		Ŀ�껺�����Ĵ�С
 *@retval	pszTarget		ת������ַ���
 *@retval	pnTargetLen		ת������ַ�������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_FmtAmtStr (const uchar* pszSource, uchar* pszTarget, int* pnTargetLen );

/**
 *@brief	��AscII����ַ���ת����ѹ����HEX��ʽ
 *@details	��ż�����ȵ��ַ������ݶ��뷽ʽ����ȡ��0���Ҳ�F��ʽ
 *@param	pszAsciiBuf		��ת����ASCII�ַ���
 *@param	nLen			�������ݳ���(ASCII�ַ����ĳ���)
 *@param	ucType			���뷽ʽ  0�������  1���Ҷ���
 *@retval	psBcdBuf		ת�������HEX����
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AscToHex (const uchar* pszAsciiBuf, int nLen, uchar ucType, uchar* psBcdBuf);

/**
 *@brief	��HEX������ת����AscII���ַ���
 *@param	psBcdBuf		��ת����Hex����
 *@param	nLen			ת�����ݳ���(ASCII�ַ����ĳ���)
 *@param	ucType			���뷽ʽ  1�������  0���Ҷ���
 *@retval	pszAsciiBuf		ת�������AscII������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_HexToAsc (const uchar* psBcdBuf, int nLen, uchar ucType, uchar* pszAsciiBuf);

/**
 *@brief	����ת��Ϊ4�ֽ��ַ����飨��λ��ǰ��
 *@param	unNum		��Ҫת����������
 *@retval	psBuf		ת��������ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IntToC4 (uchar* psBuf, uint unNum );

/**
 *@brief	����ת��Ϊ2�ֽ��ַ����飨��λ��ǰ��
 *@param	unNum		��Ҫת����������
 *@retval	psBuf		ת��������ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IntToC2(uchar* psBuf, uint unNum );

/**
 *@brief	4�ֽ��ַ�����ת��Ϊ���ͣ���λ��ǰ��
 *@param	psBuf		��Ҫת�����ַ���
 *@retval	unNum		ת�������������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_C4ToInt(uint* unNum, uchar* psBuf );

/**
 *@brief	2�ֽ��ַ�����ת��Ϊ���ͣ���λ��ǰ��
 *@details	psBuf����Ҫ>=2
 *@param	psBuf		��Ҫת�����ַ���
 *@retval	unNum		ת�������������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_C2ToInt(uint *unNum, uchar *psBuf);

/**
 *@brief	����(0-99)ת��Ϊһ�ֽ�BCD
 *@param	nNum		��Ҫת����������(0-99)
 *@retval	ch			ת�������һ��BCD�ַ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_ByteToBcd(int nNum, uchar *ch);

/**
 *@brief	һ�ֽ�BCDת��Ϊ����(0-99)
 *@param	ch		��Ҫת����BCD�ַ�
 *@retval	pnNum	ת�����������ֵ(0-99)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_BcdToByte(uchar ch, int *pnNum);

/**
 *@brief	����(0-9999)��Ҫת����������(0-9999)
 *@param	nNum		���ִ�1
 *@param	pnBcdLen	����������Ĵ�С
 *@retval	pnBcdLen	ת�����BCD���ȣ�����ɹ���ֵ���̶�����ֵΪ2
 *@retval	psBcd		ת����������ֽ�BCD
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IntToBcd(uchar *psBcd, int *pnBcdLen, int nNum);

/**
 *@brief	���ֽ�BCDת��Ϊ����(0-9999)
 *@details	psBcd����Ӧ����2
 *@param	psBcd		��Ҫת�������ֽ�BCD
 *@retval	nNum		ת���������(0-9999)
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_BcdToInt(const uchar * psBcd, int *nNum);

/**
 *@brief	����LRC
 *@details	psbuf����ĳ���>nLen
 *@param	psBuf		��Ҫ����LRC���ַ���
 *@param	nLen		��Ҫ����LRC���ַ����ĳ���
 *@retval	ucLRC		����ó���LRC
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_CalcLRC(const uchar *psBuf, int nLen, uchar *ucLRC);

/**
 *@brief	�ַ���ȥ��ո�
 *@param	pszBuf		����ַ����Ļ�����
 *@retval	pszBuf		ȥ����ո����ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_LeftTrim(uchar *pszBuf);

/**
 *@brief	�ַ���ȥ�ҿո�
 *@param	pszBuf		����ַ����Ļ�����
 *@retval	pszBuf		ȥ���ҿո����ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_RightTrim(uchar *pszBuf);

/**
 *@brief	�ַ���ȥ���ҿո�
 *@param	pszBuf			����ַ����Ļ�����
 *@retval	pszBuf			ȥ�����ҿո����ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AllTrim(uchar *pszBuf);

/**
 *@brief	��һ�ַ��������ĳһ�ַ�ʹ֮����ΪnLen
 *@details	pszString����ĳ���Ӧ>nlen, �ַ����ĳ���ҪС��nlen
 *@param	pszString		����ַ����Ļ�����
 *@param    nLen			�ַ�������
 *@param	ch				��Ҫ������ַ�
 *@param	nOption			��������
                          	0    ���ַ���ǰ����ַ�
                          	1    ���ַ���������ַ�
                          	2    ���ַ���ǰ����ַ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_AddSymbolToStr(uchar *pszString, int nLen, uchar ch, int nOption);

/**
 *@brief	��ȡ�Ӵ�
 *@details	�Ӵ����'\0'������
 *@param	pszSouStr		��Ҫ���н�ȡ���ַ���
 *@param	nStartPos		Ҫ��ȡ�Ӵ�����ʼλ�� �ַ�����λ����1��ʼ����
 *@param	nNum			Ҫ��ȡ���ַ���
 *@retval	pszObjStr		���Ŀ�괮�Ļ�����
 *@retval	pnObjStrLen		�Ӵ��ĳ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_SubStr(const uchar *pszSouStr, int nStartPos, int nNum, uchar *pszObjStr, int *pnObjStrLen);

/**
 *@brief	�жϸ���һ�ַ��ǲ��������ַ�
 *@param	ch		��Ҫ�жϵ��ַ�
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IsDigitChar(uchar ch);

/**
 *@brief	����һ�ִ��Ƿ�Ϊ�����ִ�
 *@param	pszString		��Ҫ�жϵ��ַ���
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IsDigitStr(const uchar *pszString);

/**
 *@brief	�ж�ĳ���Ƿ�����
 *@param	nYear		���
 *@return
 *@li	NDK_OK				�����ɹ�(����)
 *@li	����EM_NDK_ERR		����ʧ��(������)
*/
int NDK_IsLeapYear(int nYear);

/**
 *@brief	�ҳ�ĳ��ĳ�µ��������
 *@param	nYear		���
 *@param	nMon		�·�
 *@retval	pnDays		����ݸ��¶�Ӧ���������
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_MonthDays(int nYear, int nMon, int *pnDays);

/**
 *@brief	�ж��ṩ���ַ����ǲ��ǺϷ������ڸ�ʽ��
 *@param	pszDate		���ڸ�ʽ�ַ���  ��ʽΪ YYYYMMDD
 *@return
 *@li	NDK_OK				�����ɹ�
 *@li	����EM_NDK_ERR		����ʧ��
*/
int NDK_IsValidDate(const uchar *pszDate);

/** @} */ // ����ģ�����

typedef struct{
	unsigned short  timeout;				//��ʱʱ��5~200��
	unsigned short  size;					//����ṹ�Ĵ�С��������չʹ��
	int  indexKey;					//��Կ���
	int  type;		//����
	char * pPAN;					//���˺�
	unsigned char *  pPINBlock;			//pinblock
	void *  pAD;					//�������ݣ�����dukpt��ksn��
	int  sizeAD;					//�������ݳ���
//	int  keySimulated ;
	int  nPINmismatch ;
	unsigned int  Exponent; 		//RSA��Կָ��
	unsigned char  SWBytes [2] ;			//У������,���硰9000��
	int  PINBlockFormat; 	//pin block��ʽ
	unsigned char *  pSCRC;				//У�����
}sec_vpp_data;

int SDK_SecVPPEvent(int handle, int flag);
int SDK_SecVPPInit(int handle, sec_vpp_data stSecVppData);
#endif
/* End of this file */

