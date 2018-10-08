#ifndef __LIBDEVMGR_H__
#define __LIBDEVMGR_H__

/* �豸�������� */
#define DEV_CLASS_CPLD 			"cpld"
#define DEV_CLASS_DISPLAY 		"disp"
#define DEV_CLASS_TOUCHSCREEN 		"ts"
#define DEV_CLASS_WIRELESS_MODEM 	"wlsmdm"
#define DEV_CLASS_WIFI 			"wifi"
#define DEV_CLASS_MODEM 		"mdm"
#define DEV_CLASS_ETHERNET 		"eth"
#define DEV_CLASS_MAG_CARD 		"mag"
#define DEV_CLASS_PRINTER 		"prn"
#define DEV_CLASS_RFID 			"rfid"
#define DEV_CLASS_SMART_CARD 	"sc"
#define DEV_CLASS_AUX 			"aux"
#define DEV_CLASS_SCANNER 		"scanner"
#define DEV_CLASS_MACHINE_TYPE 	"machine"
#define DEV_CLASS_PINPAD        "pinpad"
#define DEV_CLASS_USB           "usb"
#define DEV_CLASS_RFIDLED        "led"
#define DEV_CLASS_KDB			"keypad"

#define DEV_CLASS_SERIAL            "g_serial"
#define DEV_CLASS_STORAGE            "usb-storage"
#define DEV_CLASS_HCD            "za9l1_hcd"
#define DEV_CLASS_UDC            "za9l1_udc"

/*����������*/
#define CFG_NAME_EXIST 	"exist"
#define CFG_NAME_AUTO 	"auto"
#define CFG_NAME_TYPE 	"type"
#define CFG_NAME_DRIVER "driver"
#define CFG_NAME_PARAM 	"param"

/**��ѯ�豸��Ϣ
 * ������
 * 	dev_class : �豸��� �����������
 * 	exist : �豸�Ƿ���ڣ�0 - �����ڣ�1 - ���� �����������
 * 	type ������ �����������, ��"thermal"����"����"
 */
//extern void get_dev_info(void *dev_class, int *ret, int *exist, void *type);

/**�����豸����
 * ������
 * 	dev_class : �豸��� �����������
 * 	ret : 0 - �ɹ���1 - ʧ�� �����������
 */
extern int load_driver_by_class(const char *dev_class);

/**ж���豸����
 * ������
 * 	dev_class : �豸��� �����������
 * 	ret : 0 - �ɹ���1 - ʧ�� �����������
 */
extern int unload_driver_by_class(const char *dev_class);

/**�����豸����
 * ������
 * 	driver_name : �������� �����������
 * 	ret : 0 - �ɹ���1 - ʧ�� �����������
 */
extern int load_driver_by_name(const char *driver_name, const char *param);

/**ж���豸����
 * ������
 * 	driver_name : �������� �����������
 * 	ret : 0 - �ɹ���1 - ʧ�� �����������
 */
extern int unload_driver_by_name(const char *driver_name);

/**��ȡ�����ļ�����
 * ������
 * 	dev_class : �豸���� 
 *  	cfgname������������
 *  	cfgvalue��������ֵ (�������)
 *����ֵ: 0 - �ɹ���< 0 - ʧ�� 
 */
extern int get_dev_info(const char *dev_class, const char *cfgname, char *cfgvalue);

/**���������ļ�����
 * ������
 * 	driver_name : �������� �����������
 * 	cfgname:	���������� �����������
 *  cfgvalue:	������ֵ   �����������
 */
extern int set_dev_info(const char *dev_class, const char *cfgname, const char *cfgvalue);
#endif
