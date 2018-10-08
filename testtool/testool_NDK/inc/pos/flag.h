/*
 * FileName:       
 * Author:         zhangnw  Version: 1.6.3  Date: 2006-8-2
 * Description:   POS��Ʒ����ͷ�ļ�����ģ��Ĺ����ļ�������Ҳ�ɷ��ڴ˴���
 *			  ���ļ��ɷ�TCR��Ʒ��ͬ������,��ֻ��ĳ��Ʒ���еģ����ɶ�����INC\TCR�����Լ���Ŀ¼��
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version>  <desc>
 *	 				201706054         ��lib.h�з������
 */

#ifndef _FLAG_H_
#define _FLAG_H_

/*-----------------------includes-------------------------------*/
//#include "mode.h"

/*---------------constants/macro definition---------------------*/

//ƽ̨��־λ�����ڼ��ٲ��Դ����е��������������
#define    ANDIROD_ENABLE (defined N900AZ||defined N910AZ||defined IM81AZ||defined N700||defined N850||defined X5||defined N920||defined N510||defined N550||defined N910P) //��׿ƽ̨
#define	K21_ENABLE	(defined ME31||defined SP10||defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined ME15||defined ME15C||defined ME15CHX||defined ME15B||defined ME20||defined ME50NX||defined ME68||defined ME66||CPU5810X_ENABLE||ANDIROD_ENABLE)//�Ͷ�ƽ̨��Ʒ����K21��GD32��||defined IM81||defined N900ͳһΪIM81AZ /N900AZ
#define	CPU5892_ENABLE 	(defined SP60||defined SP50||defined SP80||defined ME31S||defined ME31SLS||defined SP33||defined SP600||defined GP720||defined ME50S||defined ME50NS)//���ڰ汾NDK��Ʒ
#define	CPU5830X_ENABLE  (defined SP930||defined SP930P||defined SP630)//CPU 5830Xƽ̨��Ʒ
#define	CPUMDM_ENABLE (defined ME31SMDM||defined SP930MDM) //CPU MDM9x07ƽ̨��Ʒ
#define    CPU5810X_ENABLE (defined ME50||defined ME50N||defined SP610||defined ME62||defined ME50C||defined ME50H)//CPU5810Xƽ̨��Ʒ���ڵͶ˲�Ʒ
#define	GP_ENABLE	(defined GP730||defined NL8510GP)//����GPƽ̨��Ʒ
#define 	OVERSEAS_ENABLE	(defined SP60G||defined SP50G||defined SP80G||defined SP33G||defined SP600G||defined SP930G||defined SP930PG||defined SP630PG||defined SP830PG||defined ME51||CPUMDM_ENABLE)//���ʰ汾NDK��Ʒ

//�޼��̱�־��û�м��̵��豸��Ҫ���������ñ�־
#define KB_DISABLE	(defined NL829STD||defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME65||defined ME66)//||defined IM81AZ	//||defined N900||defined N900AZ||defined N910AZ

//��Һ����־��û��Һ�����豸��Ҫ���������ñ�־
#define SCR_DISABLE	(defined NL829STD|defined SP10||defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME66)//||defined IM81|||defined N900||defined N900AZ||defined N910AZ||defined IM81AZ

//Ӳ����֧��1�ŵ��ı�־
#define TK1_DISABLE	(defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME30THM||defined ME32THM)

//��ر�־:�޷�ȡ����ص��豸��Ҫ���������ñ�־
#define BATTERY_GETOUT_ENABLE !(defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME66||CPU5810X_ENABLE||defined ME50NX||defined ME68||defined ME50S||defined ME50NS||defined ME31S||defined ME31SLS||defined SP33||defined SP600||defined SP33G||defined SP600G||defined ME51)

//��ȫģ������㷨֧�ֱ�־
#define SEC_SM4_ENABLE	(CPU5892_ENABLE||CPU5830X_ENABLE||defined ME30||(ANDIROD_ENABLE&&!defined N910P))//||defined ME30THM

//��ȫģ��AES�㷨֧�ֱ�־
#define SEC_AES_ENABLE	 ANDIROD_ENABLE

//ALGģ������㷨֧�ֱ�־
#define ALG_SM_ENABLE	(CPU5892_ENABLE||CPU5830X_ENABLE||defined ME30||CPU5810X_ENABLE||defined ME50NX||defined ME68||ANDIROD_ENABLE)//||defined ME30THM

//��wifi����֧�ֱ�־
#define NEW_WIFI_ENABLE (defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME62||defined ME50NX||defined ME68)

//TR31��Կ��װ��ʽ֧�ֵı�־
#define TR31_ENABLE !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��TR31��ʽ��Կ

//CBCģʽ��װ��ʽ֧�ֵı�־
#define CBC_LOAD_ENABLE (ANDIROD_ENABLE&&!defined N910P)

//DUKPT��Կ��ϵ֧�ֵı�־
#define DUKPT_ENABLE !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��DUKPT��ʽ��Կ��ϵ

//ALGģ��RSA�㷨����֧�ֵı�־,��ӦRSA���㷨
#define ALG_RSA_ENABLE !(CPU5810X_ENABLE||defined ME50NX||defined ME68||defined ME15||defined ME15C||defined ME15CHX||defined ME15B||defined ME30S||defined ME30MH||defined ME30GX||defined ME32||defined ME65||defined ME32GX)	//ME15C��֧��RSA�㷨CPU5810X_ENABLE||

//SECģ��RSA��Կ����֧�ֵı�־,��ӦRSAӲ�㷨
#define SEC_RSA_ENABLE (CPU5892_ENABLE||CPU5830X_ENABLE)	//������5892ƽ̨֧��RSA��Կ��װ

//SHA�㷨֧�ֵı�־
#define SHA_ENABLE !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��SHA�㷨

//PIN�㷨֧�ֵı�־
#define PINKEY_ENABLE !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��PIN�㷨

//������֧��JUSTWORKģʽ�ı�־
#define JUSTWORK_FLAG (defined ME15C||defined ME15CHX||defined ME15B||defined ME20)	//ME15C ME20 ��֧��JUSTWORKģʽ||defined ME30MH

//�Ͷ�������YC1021оƬ��Ҫ�ֶ������㲥��־λ
#define SET_BTDISCOVER_ENABLE (defined ME30MH ||defined ME30GX||defined ME32||defined ME32GX||defined ME32THM||defined ME65||defined ME30THM)

#define	ENABLE		1
#define	DISABLE		0

//�����Ʒ�̼�7.0�汾���Ͽ���
#if 0
#define OVERSEAS_OS70_ENABLE ENABLE
#else
#define OVERSEAS_OS70_ENABLE DISABLE
#endif

//4.2���������汾����
#if 0
#define BT42_ENABLE  ENABLE
#else
#define BT42_ENABLE  DISABLE
#endif
//PCI��֤�汾������־ 
#if 0
#define BTPCI_ENABLE  ENABLE
#else
#define BTPCI_ENABLE  DISABLE
#endif
//����֧��PINCODEģʽʹ��
#define PINCODE_ENABLE  !(BT42_ENABLE&& BTPCI_ENABLE)//4.2������������PCI�汾�޴����ģʽ
//����֧��JUSTWORK AND SCģʽʹ��   ���������ģʽ (���ڰ汾)
#define JUSTWORK_SC_ENABLE  (BT42_ENABLE && !BTPCI_ENABLE)//4.2������������BLEЭ�����������ģʽ
//����֧��just workģʽʹ��(�������Ķ���֧�ִ�ģʽ ֻ��PCI��֤�汾�޴�ģʽ)
#define JUSTWORK_ENABLE  !BTPCI_ENABLE // PCI��֤�汾��֧��

//����ͷ֧�ֱ�־
#define CAMERA_ENABLE 0//defined SP600

//ɨ��ͷ������֧�ֱ�־
#define SCANASYCN_ENABLE (defined ME50||defined ME50NX||defined ME68||defined ME66||defined ME50N||defined ME50C||defined ME50H)//||defined ME50N//||defined SP600

//ɨ��ͷ֧�ֱ�־(����)
#define SCAN_ENABLE (CPU5892_ENABLE||CPU5830X_ENABLE||defined SP600G||defined ME50||defined ME51||defined ME50N||defined ME50C||defined ME50H)

//�����ɨ��ͷ֧�ֱ�־
#define SCAN_SOFT_ENABLE defined SP630||defined SP630PG

//˫SIM��Ӳ��֧�ֱ�־
#define DOUBLESIM_ENABLE (defined SP600||defined ME51)

//����汾����MFI����֧�ֱ�־
#if 0
	#define	BTMFI_ENABLE	ENABLE
#else
	#define	BTMFI_ENABLE	DISABLE
#endif

//�Ͷ�WiFi�Ƿ�֧�����ȼ����ܱ�־λ(����Э��ջ��Wifiģ�鲻֧��,����Э��ջ֧��)
#if 0
	#define  WIFI_PRIORITY_ENABLE 	ENABLE   //����ģ��
#else
	#define  WIFI_PRIORITY_ENABLE	DISABLE  //���� �ž�����ģ��
#endif

//��ʽ�������֧�� ����
#define SPRINTF_UNSPORT ANDIROD_ENABLE

//Statusbar
#if !(defined GP730||defined NL8510GP||K21_ENABLE || defined SP600G || defined SP600)
	#define	STABAR_ENABLE	ENABLE
#else
	#define	STABAR_ENABLE	DISABLE
#endif

//״̬��ģ��LED��֧�ֱ�־
#if CPU5892_ENABLE|| CPU5830X_ENABLE||OVERSEAS_ENABLE
	#define  	STABAR_LED_ENABLE 	ENABLE
#else
	#define 	STABAR_LED_ENABLE 	DISABLE
#endif

//mag
#if !(defined N510||defined N550)
	#define	MAG_ENABLE	 ENABLE
#else
	#define	MAG_ENABLE	 DISABLE
#endif

#if 0
#define READCARD3_SUPPORT
#endif

//IC/SAM
#if !(defined ME50N||defined ME20||defined ME62||defined ME50NX||defined ME68||defined ME50C||defined ME50H||defined N510||defined N550)//defined ME50֧��IC��510��֧��IC��֧��sam1��
	#define	IC1_ENABLE	ENABLE
#else
	#define	IC1_ENABLE	DISABLE
#endif

#define IC2_ENABLE DISABLE

#if !(defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||CPU5810X_ENABLE||defined ME50NX||defined ME68||defined ME66||defined ME50S||defined ME50NS||defined SP600G||defined ME51||defined N550)  //20140218 chensj ,������ȷ��ME30��֧��С��
	#define SAM1_ENABLE ENABLE
#else
	#define SAM1_ENABLE DISABLE
#endif

#if defined IM81||defined IM81AZ||defined N850||defined N700||defined SP60||defined SP50||defined SP80||defined SP600||defined SP33G||defined SP600G||defined ME51||defined SP60G||defined SP50G||defined SP80G||defined NL8510GP||defined SP830PG
	#define SAM2_ENABLE ENABLE
#else
	#define SAM2_ENABLE DISABLE
#endif

#if defined GP730||defined GP720
	#define SAM3_ENABLE ENABLE
#else
	#define SAM3_ENABLE DISABLE
#endif

#if 0
	#define SAM4_ENABLE ENABLE
#else
	#define SAM4_ENABLE DISABLE
#endif

#if !(defined ME31||defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined SP10||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||CPU5810X_ENABLE||defined ME50NX||defined ME68||defined ME66)
	#define	SAM_POWERON_MEANWHILE	ENABLE//δ����Ӧ��֧��IC/SAM֮��ͬʱ�ϵ�,��Ӳ���Ͽ��Էֿ���������
#else
	#define	SAM_POWERON_MEANWHILE	DISABLE
#endif

#define ICSAM_PPS_ENABLE	DISABLE

//IC/SAM����
#define	ICNUM		(IC1_ENABLE+IC2_ENABLE)
#define	SAMNUM	(SAM1_ENABLE+SAM2_ENABLE+SAM3_ENABLE+SAM4_ENABLE)
#define	ICSAMNUM		(ICNUM+SAMNUM)

//RFID
#if !(defined ME15C||defined ME15CHX)
	#define	RFID_ENABLE	ENABLE
#else
	#define	RFID_ENABLE	DISABLE
#endif
//15693Э���Ƿ�֧��
#if 0// ANDIROD_ENABLE
	#define  ISO15693_ENABLE   ENABLE
#else
	#define  ISO15693_ENABLE   DISABLE
#endif
//M0���Ƿ�֧��
#if  0
	#define  M0_ENABLE   ENABLE
#else
	#define  M0_ENABLE   DISABLE
#endif

#if !defined RFCARD_TYPE_A
#define RFCARD_TYPE_A	0xcc
#define RFID_TYPE_A RFCARD_TYPE_A //�º���
#endif
#if !defined RFCARD_TYPE_B
#define RFCARD_TYPE_B	0xcb
#define RFID_TYPE_B RFCARD_TYPE_B
#endif
#if !defined RFCARD_TYPE_AB
#define RFCARD_TYPE_AB 0xcd
#define RFID_TYPE_AB RFCARD_TYPE_AB
#endif

//ȡsmart��״̬��(SW),��IC/SAM/RFID��ʹ��.��,0x9000��ʾ����ִ�� �ɹ�
#define	SMART_2BYTESTOUSHORT(addr0)	(((*(uchar *)(addr0)<<8)&0x0000ff00)|((*((uchar *)(addr0)+1))&0x000000ff))	//ע��һЩ��������uchar��char��һ����,������λʱ����������,������ת��ushort��,ֱ��&0x0000ffff����
#define	SMART_GETSW					SMART_2BYTESTOUSHORT

//NFC
#if 0
	#define	NFC_ENABLE	ENABLE//����������Ƶ�����豸��Ҫ֧��NFC���ܣ�ʵ�ʲ������������Ա����ʵ����������ÿ���
#else
	#define	NFC_ENABLE	DISABLE
#endif

//port
#define INVALID_COM 0xff  //zhangnw20061207 add �ݶ�0xff������������Դ��ͻ

#if defined GP720||defined NL8510GP||defined SP80||defined SP50||defined SP80G||defined SP50G||defined SP830PG||defined N850||defined X5
	#define COMNUM 2

#elif defined ME31||defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined ME15||(ANDIROD_ENABLE&&!defined N850&&!defined X5)||CPU5810X_ENABLE||defined ME50NX||defined ME68||defined ME66||defined ME50S||defined ME50NS||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG||defined ME15CHX||defined ME15B//(||defined ME11||defined ME15 ����USB����,��Ҫʹcomnum=1)//IM81AZ��Ȼ�д��ڵ���׿���޷�ʹ��,����Ҳ��Ϊû�д���
	#define COMNUM 0
#elif 0
	#define COMNUM 3
#else //Ĭ��һ������,��GP730
	#define COMNUM 1
#endif

//���ڣ�USB��ģʽ�Ļ���������16K���Ͷ�ƽ̨����2047
#if !K21_ENABLE
#define BUFSIZE_SERIAL (1024*16)//(1024*4)
#elif (defined ME30MH ||defined ME30GX||defined ME32||defined ME32GX||defined ME65)
#define BUFSIZE_SERIAL  (1024)
#elif CPU5810X_ENABLE
#define BUFSIZE_SERIAL  (1024*4)
#else
#define BUFSIZE_SERIAL (1024*2-1)
#endif

//������USB��ģʽ �������ж˴�С4K���Ͷ˴�С2047
#if !K21_ENABLE
#define BUFSIZE_BT (1024*4)
#elif defined ME15C||defined ME15CHX||defined ME15B||defined ME20||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME30THM//ME15C��ʱֻ֧�ֵ�1023���������ݰ�����ME20֧��1024
#define BUFSIZE_BT  (1024-1)
#else
#define BUFSIZE_BT  (1024*2-1)
#endif

#define BUFSIZE_USBHOST BUFSIZE_BT
//#define	MAXSNDLEN	BUFSIZE_SERIAL

//sec

//ALG

//disp
#if 0	//���º��ѹ�ʱ,ԭ����Ҫ�ṩ��dispģ�鼰main�е�setuserscreenʹ��,��NDK����Ӧ�Ľӿڻ�ȡ��Щ��Ϣ,ͨ���ӿڻ�ȡ����
#if defined DEMO||defined SP730
	#define	LINE_NUMS	(144-1)			/* ����ʽ��POS��Y�᷽�����ֵ */
	#define	COLU_NUMS	(128-1)			/* ����ʽ��POS��X�᷽�����ֵ */
#else
	#define	LINE_NUMS	(64-1)			/* ����ʽ��POS��Y�᷽�����ֵ */
	#define	COLU_NUMS	(128-1)			/* ����ʽ��POS��X�᷽�����ֵ */
#endif
#endif
#define	BLACK 0
#define	WHITE 0xFFFF
#define	RED	  0xFACB
#define	GREEN 0x3384

#if 0	
#define	GUI_SUPPORT
#endif

//appmgr

/*USB������ģʽ*/
#if !K21_ENABLE
	#define	USBHOST_ENABLE	ENABLE //K21��Ʒƽ̨��֧��
#else
	#define	USBHOST_ENABLE	DISABLE
#endif

/*U��*/
#if !K21_ENABLE
	#define	USB_ENABLE	ENABLE //K21��Ʒƽ̨��֧��
#else
	#define	USB_ENABLE	DISABLE
#endif

/*SD*/
#if !(K21_ENABLE||OVERSEAS_ENABLE)
	#define	SD_ENABLE	ENABLE //K21��Ʒƽ̨��֧��,���ʰ汾Ҳ��֧��SD��
#else
	#define	SD_ENABLE	DISABLE
#endif

/*TOUCH*/
#if !((K21_ENABLE&&!defined ME50&&!defined SP610)||defined NL8510GP) //K21��Ʒƽ̨��֧�� 8510GPҲ��֧�� ME50֧�ִ���
	#define	TOUCH_ENABLE	ENABLE
#else
	#define	TOUCH_ENABLE	DISABLE
#endif

//ETH
#if !K21_ENABLE ||defined ME50C//K21��Ʒƽ̨��֧��
	#define	ETH_ENABLE	ENABLE
#else
	#define	ETH_ENABLE	DISABLE
#endif

//BT
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE) ||defined ME51||defined SP630PG//�Ⱥ���ȷ����Щ��Ʒ��Ҫ֧���������ٵ����ú꿪�� 
	#define	BT_ENABLE	ENABLE
#else
	#define	BT_ENABLE	DISABLE
#endif

//BT ��ģʽ
#if !(K21_ENABLE||OVERSEAS_ENABLE) ||defined ME51||defined SP630PG//defined SP60//�Ⱥ���ȷ����Щ��Ʒ��Ҫ֧���������ٵ����ú꿪��
	#define	BT_MASTER_ENABLE	ENABLE
#else
	#define	BT_MASTER_ENABLE	DISABLE
#endif

//DHCP
#if !K21_ENABLE
	#define	DHCP_ENABLE	ENABLE //K21ƽ̨��֧��
#else
	#define	DHCP_ENABLE	DISABLE
#endif

//WIFI/WALN
#if !K21_ENABLE||(CPU5810X_ENABLE&&!defined SP610)||defined ME50NX||defined ME68
	#define	WLAN_ENABLE	ENABLE //K21ƽ̨��֧��
#else
	#define	WLAN_ENABLE	DISABLE
#endif

//WIFI AP
#if !(K21_ENABLE||OVERSEAS_ENABLE)//��������WIFI���豸ҲҪ֧��WIFI AP����,ʵ�ʲ������������Ա���ݲ�����������ÿ���
	#define	WIFIAP_ENABLE	ENABLE
#else
	#define	WIFIAP_ENABLE	DISABLE	
#endif

#define WLAN_SIGNAL_MIN	(50)
#define WLAN_APCNT_MIN	(1)
#define WLAN_APCNT_MAX	(100)

//dns
//�����б��ά���뱣��DNS1��DNS3����Ч��,DNS2�ǿ��õ�
#define	DNS1	"220.181.111.148"	//�õ�ַ��baidu��IP,Ҳ����Ч��DNS
#if 0	
#define	DNS2	"211.138.151.161"	//����,�ƶ�ʹ��
#else
//#define	DNS2	"218.85.157.99"	//��ͨʹ��
#define	DNS2	"8.8.8.8"	//ʹ��google��DNS
#endif
//#define	DNS2	"192.168.30.1"	//"61.54.28.13"
#define	DNS3	"192.168.30.2"	//�õ�ַ����Ч��

//prnt
#if !(defined NL829STD||defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined SP10||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME62||defined ME50NX||defined ME68||defined ME66||defined ME50S||defined ME50NS||defined ME51)
	#define	PRN_ENABLE		ENABLE
	
	#define	MAXPRNBUFSIZE		(1024*2)
	//�����������п�Ϊ����ģʽ�µ�,�Ŵ�ģʽ�µ�ֵΪ����һ��
	#define	MAXPRLINEWIDE_TP		384
#if defined GP710||defined GP720
	#define	MAXPRLINEWIDE_HIP		360	//����Ŵ�180,���������ɴ�360
#else
	#define	MAXPRLINEWIDE_HIP		280	//144
#endif
	#define	MAXPRLINEWIDE_FIP		424

#if defined GP_ENABLE||CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE||ANDIROD_ENABLE||defined ME31//||defined IM81||defined N900
	#define PRNGREY_ENABLE ENABLE

	#define	GREY_MINVALID		(0)
#if 0	//defined E90||defined E90G
	#define	GREY_MAXVALID		(7)
#else
	#define	GREY_MAXVALID		(5)
#endif
	#define	DEFAULT_GREY_VLE	(3)	//Ӧ�ò�Ĭ�ϻҶ�,��ײ㲻һ��һ��
#else
	#define PRNGREY_ENABLE DISABLE
#endif

#define PRNUNDERLINE_EN	ENABLE

#else
	#define	PRN_ENABLE		DISABLE
#endif

/***********************
 * ����ͼ�����Ű�ģʽ*
 ***********************/
typedef enum PRN_TYPESETTING
{
	TPSET_AUTO = 0,		//<�Զ���Ӧ�����ֻ���ͼ�񣬱�֤���غϴ�ӡ
	TPSET_TEXTUP,		//<�������ϣ��������غϣ��ļ���ֱ�Ӹ�д��ͼ����
	TPSET_PICUP,		//<ͼ�����ϣ��������غϣ�ͼ��ֱ�Ӹ�д���ļ���
	TPSET_MIX			//<����ͼ��Ƕ�ף����غϣ����ֺ�ͼ��Ƕ�״�ӡ
}PRN_TYPESETTING;

//sys
#define	SLEEP_ALLOW_INTERRUPT	//Ĭ��ϵͳ��sleep(���������sleep��)���ɱ��ж�
#if defined ME15CHX||defined ME30MH||defined ME15B||defined ME32||defined ME66//��֧��NDK_SysDelay
#define	sleep(a)		NDK_SysMsDelay(a*1000)
#else
#define	sleep(a)		NDK_SysDelay(a*10)
#endif
#if defined GP720||defined NL8510GP||defined SP80||defined SP50||defined SP80G||defined SP50G||defined SP830PG
	#define	SLEEP_ENABLE	DISABLE
#else
	#define	SLEEP_ENABLE	ENABLE
#endif


/*MODEM*/
//��MODEMӲ������˵,һ����ͬʱ֧��ͬ�����첽��.���������ʱδ������һ����֧��(����,����δ����,�ӿ�δ���ŵ�)
//sdlc
#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG) //K21��Ʒƽ̨��֧��
	#define	SDLC_ENABLE	ENABLE
#else
	#define	SDLC_ENABLE	DISABLE
#endif
#define	SLEEP_SDLC_HANGUP	(5)
#define	SDLCPCKTHEADER		"\x60\x80\x00\x80\x00"
#define	SDLCPCKTHEADERLEN		(5)
#define	SDLCPCKTMAXLEN		(1024)//(350)   //�����ؿ���֧�ֱȽϴ�������շ���Ϊ�����ӽ�Ӧ�����ݰ����Ƚ�350�ĳ�1024  20140504 linwl

//asyn
#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG) //K21��Ʒƽ̨��֧��
	#define	ASYN_ENABLE	ENABLE
#else
	#define	ASYN_ENABLE	DISABLE
#endif
#define	ASYNPCKTHEADERLEN	PCKT0203HEADERLEN
#define	ASYNRESPMAXLEN	(256)
#define	ASYNPCKTMAXLEN	(4*1024)	//PACKMAXLEN	//���ֵ����ASYNͨѶ����֧�ֵ�������,ֻ�Ƕ������޶���

// ����
#if !K21_ENABLE||CPU5810X_ENABLE||defined ME50NX||defined ME68
	#define	WLM_ENABLE	ENABLE //K21��Ʒƽ̨��֧��(��5810x��Ʒ)
#else
	#define	WLM_ENABLE	DISABLE
#endif
#define SQ_2G_MIN		(0)
#define SQ_2G_MAX		(31)
#define SQ_3G_MIN		(100)
#define SQ_3G_MAX		(199)

//ppp
#if !K21_ENABLE||CPU5810X_ENABLE||defined ME50NX||defined ME68
	#define	PPP_ENABLE	ENABLE //K21��Ʒƽ̨��֧��(��5810x��Ʒ)
#else
	#define	PPP_ENABLE	DISABLE
#endif

//TD
#if 0
	#define	TD_ENABLE	ENABLE
#else
	#define	TD_ENABLE	DISABLE
#endif

//sckt
#if !K21_ENABLE||CPU5810X_ENABLE||defined ME50NX||defined ME68
	#define	NDKSOCK_ENABLE	ENABLE //K21��Ʒƽ̨��֧��(��5810x��Ʒ)
#else
	#define	NDKSOCK_ENABLE	DISABLE
#endif

//ssl 
#if CPU5892_ENABLE||OVERSEAS_ENABLE
	#define	NDKSSL_ENABLE	0   //ENABLE   20140822 ssl ��5.0�汾����ʱȥ��
#else
	#define	NDKSSL_ENABLE	0	//DISABLE
#endif

#endif
