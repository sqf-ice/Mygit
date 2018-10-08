/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys16.c
* Author 			: linw
* version			: 
* DATE				: 20130109
* directory 		: 
* description		: ��������NDK_SysGetPosInf��ȡposӲ����Ϣ�ӿڹ����Ƿ���ȷ
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetPosInfo"	
#define MAXWAITTIME 5
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/*��������:��ȡ����ģ������
	����cin
	0xFF :û������ģ��
	0x02 :MC39 
	0x03 :sim300 
	0x06 :M72 
	��λΪ0x80��ʾCDMAģ��
	0x81 :dtgs800 
	0x82 :dtm228c 
	����:�����������
*/
static char * WireType(char cin)
{
	switch(cin)
	{
	case 0xff:
		return ("��");
	case 0x02:
		return ("GPRS+MC39");
	case 0x03:
		return ("GPRS+sim300");
	case 0x06:
		return ("GPRS+M72");
	case 0x81:
		return ("CDMA+dtgs800");
	case 0x82:
		return ("CDMA+dtm228c");
   	case 0x85://GP720 ���� 20131128
		return ("CDMA+ce910");
	case 0x07://SP60 ���� 20140117
		return ("GPRS+BGS2");
	case 0x08://SP60 ���� 20140117
		return ("GPRS+G610");
	case 0x83://AC35ƽ̨���� 20180627 
		return ("GPRS+Ac35");
	default:
		return ("δ֪");
	}
}

/*��������:��ȡ��ͷģ������
	����cin
	0x01 :giga
	0x02 :mesh 
	0x03 :magtek 
	0x04 :Idtech-dualmag 
	0x10 :ADC�����
	
	����:�����ͷ����
*/
static char * MagHeadType(char cin)
{
	switch(cin)
	{
		case 0xff:
			return ("��");
			break;
		case 0x01:
			return ("giga");
			break;
		case 0x02:
			return ("mesh");
			break;
		case 0x03:
			return ("magtek");
			break;
		case 0x04:
			return ("Idtech-dualmag");
			break;
		case 0x10:
			return ("ADC�����");
			break;
		case 0x11:
			return ("GPIO");
			break;
		default:
			return ("δ֪");
			break;
	}
}

/*��������:��ȡ����ģ������
	����cin
	0xFF:û������ģ��
	0x01 :BM77
	0x02 :AT24 
	0x03 :AP6210B 
	0x04 :ALLTEC20706 
	0x05:YC1021
	0x06:DA14681
	
	����:�����������
*/
static char *BtType(char cin)
{
	switch(cin)
	{
		case 0xff:
			return ("��");
			break;
#if K21_ENABLE
		case 0x01:
			return ("BM77");
			break;
		case 0x02:
			return ("AT24");
			break;
		case 0x03:
			return ("AP6210B");
			break;
		case 0x04:
			return ("ALLTEC20706");
			break;
		case 0x05:
			return ("YC1021");
			break;
		case 0x06:
			//return ("DA14681");
			return ("TC35661");
			break;
#else
		case 0x01:
				return ("SD8787");
				break;
			case 0x02:
				return ("AP6210");
				break;
			case 0x03:
				return ("AP6210B");
				break;
			case 0x04:
				return ("AP6212");
				break;
			case 0x05:
				return ("AP6236");
				break;
			case 0x06:
				return ("AP6255");
				break;
#endif
		default:
			return ("δ֪");
			break;
	}
}

/*��������:��ȡɨ��ģ������
	����cin
	0xFF:û������ģ��
	0x01 :EM1300
	0x02 :EM3000 
	0x03 :SE955 
	0x04 :UE966 
	0x05 :EM3095
	0x06 :EM1365
	0x06 :EM1365
	0x07 :E20390
	0x08 :GC0312
	����:�����������
*/
static char *ScanType(char cin)
{
	switch(cin)
	{
		case 0xff:
			return ("��");
			break;
		case 0x01:
			return ("EM1300");
			break;
		case 0x02:
			return ("EM3000");
			break;
		case 0x03:
			return ("SE955");
			break;
		case 0x04:
#if K21_ENABLE 
			return ("0390");
#else
			return ("UE966");
#endif
			break;
		case 0x05:
#if defined ME50NX||defined ME68
			return ("AC35");
#else
			return ("EM3095");
#endif
			break;
		case 0x06:
			return ("EM1365");
			break;
		case 0x07:
			return ("E20390");
			break;
		case 0x08:
			return ("GC0312");
			break;
		default:
			return ("δ֪");
			break;
	}
}
/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	        linw         20130109	created
*****************************************************************/
void sys16(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int ret = -1;
	unsigned int unLen=0;
	char sBuf[128]={0};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��

	//case1: �쳣���ԣ����������psBufΪ��ָ��
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_KEY_COUNT, &unLen, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2: �쳣���ԣ����������emFlag����SYS_HWINFO_GET_KEY_COUNT
	if((ret=NDK_SysGetPosInfo(20, &unLen, sBuf)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
#if 1	//�ĵ�������,�Դ����˵��Ϊ����(����)
	//case3: �쳣���ԣ����������punLenΪ��ָ��
	//ME31��֧�ֻ�ȡPOS���������������û�ȡ��������������������CASE
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TYPE/*SYS_HWINFO_GET_KEY_COUNT*/, NULL, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	
	//case4: ������ȡpos��������
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TYPE, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("��������:%s,�������������ļ����бȽ�",sBuf);


	//case5: ������ȡposӲ����Ϣ
	memset(sBuf,0xff,sizeof(sBuf)-1);
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case5.1:����ģ��
	send_result("����ģ��:%s,�������������ļ����бȽ�",WireType(sBuf[0]));
	
	//case5.2��Ƶģ��
	send_result("��Ƶģ��:%s,�������������ļ����бȽ�",\
		sBuf[1]==0xff?"��":(sBuf[1]==0x01?"RC531":(sBuf[1]==0x02?"PN512":(sBuf[1]==0x03?"AS3911":(sBuf[1]==0x04?"RC663":(sBuf[1]==0x05?"MH1608":"δ֪"))))));
	
	//case5.3�ſ�ģ��
	send_result("�ſ�ģ��:%s,�������������ļ����бȽ�",MagHeadType(sBuf[2]));
	
	//case5.4ɨ��ͷģ��
	send_result("ɨ��ͷģ��:%s,�������������ļ����бȽ�",ScanType(sBuf[3]));
	
	//case5.5����������
	send_result("����������:%s,�������������ļ����бȽ�",sBuf[4]==0xff?"��֧��":(sBuf[4]==0x01?"֧��":"δ֪"));

	//case5.6���ڸ���
	send_result("���ڸ���:%s,�������������ļ����бȽ�",sBuf[5]==0xff?"��":(sBuf[5]==0x01?"1":(sBuf[5]==0x02?"2":(sBuf[5]==0x03?"3":"δ֪"))));
	
	//case5.7�Ƿ�֧��USB
	send_result("֧��USB:%s,�������������ļ����бȽ�",sBuf[6]==0xff?"��":(sBuf[6]==0x01?"��":"δ֪"));
	
	//case5.8�Ƿ�֧��MODEMģ��
	send_result("֧��MODEM:%s,�������������ļ����бȽ�",sBuf[7]==0xff?"��":(sBuf[7]==0x01?"��":"δ֪"));
	
	//case5.9�Ƿ�֧��wifiģ��
	send_result("֧��wifi:%s,�������������ļ����бȽ�",sBuf[8]==0xff?"��":(sBuf[8]==0x01?"��USI WM-G-MR-09":(sBuf[8]==0x02?"��ESP8266":(sBuf[8]==0x03?"��99":"δ֪"))));
	
	//case5.10�Ƿ�֧����̫�� pinfo[9]:�Ƿ�֧����̫�� 0xFF :�� 0x01 :dm9000 0x02 :bcm589xcore //����4.4.3�汾NDK�ĵ��޸� linwl20131029
	send_result("֧����̫��:%s,�������������ļ����бȽ�",sBuf[9]==0xff?"��":(sBuf[9]==0x01?"dm9000":(sBuf[9]==0x02?"bcm589xcore":"δ֪")));
	
	//case5.11��ӡģ��
	send_result("��ӡģ��:%s,�������������ļ����бȽ�",sBuf[10]==0xff?"��":((sBuf[10]>0 && sBuf[10]<0x80)?"����":((sBuf[10]>0x80&&sBuf[10]<0xfe)?"���":"δ֪")));
	
	//case5.12�Ƿ�֧�ִ��� 0xFF :�� 0x01:ts_2046 0x02:589x_ts //����4.4.3�汾NDK�ĵ��޸� linwl20131029
	send_result("֧�ִ���:%s,�������������ļ����бȽ�",sBuf[11]==0xff?"��":(sBuf[11]==0x01?"ts_2046":(sBuf[11]==0x02?"589x_ts":"δ֪")));
	
	//case5.13�Ƿ�����ƵLED��
	send_result("��ƵLED��:%s,�������������ļ����бȽ�",sBuf[12]==0xff?"��":(sBuf[12]==0x01?"��":"δ֪"));
	
	//case5.14�Ƿ�������
	//send_result("�Ƿ�������:%s,�������������ļ����бȽ�",sBuf[13]==0xff?"��":(sBuf[13]==0x01?"BM77":(sBuf[13]==0x02?"AT24":(sBuf[13]==0x03?"AP6210B":(sBuf[13]==0x04?"ALLTEC20706":(sBuf[13]==0x05?"YC1021":"δ֪"))))));	
	send_result("����ģ��:%s,�������������ļ����бȽ�",BtType(sBuf[13]));
	
	//case5.15�Ƿ���NFC
	send_result("�Ƿ���NFC:%s,�������������ļ����бȽ�",sBuf[14]==0xff?"��":(sBuf[14]==0x01?"��":"δ֪"));

	//case5.16�Ƿ��й���оƬ
	send_result("�Ƿ��й���оƬ:%s,�������������ļ����бȽ�",sBuf[15]==0xff?"��":(sBuf[15]==0x01?"THK88":(sBuf[13]==0x02?"CCM":"δ֪")));

	//case6:ȡbios�汾��Ϣ 
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BIOS_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("bios�汾��Ϣ:%s,���ж��Ƿ���ȷ",sBuf);
	
	
#if	1//  //����˵K21û�д�ͳ���������к�ֻ�л����� ���°汾��Ҫ��
	//case7:ȡ�������к�
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("�������к�:%s,���ж��Ƿ���ȷ",sBuf);	
#endif

	//case8:ȡ����������
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_PSN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("����������:%s,���ж��Ƿ���ȷ",sBuf);

	//case9:ȡ�����
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOARD_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("�����:%s,���ж��Ƿ���ȷ",sBuf);

	//case10:ȡposˢ������
	memset(sBuf,0,sizeof(sBuf));
#if K21_ENABLE//K21ƽ̨��֧��ȡ�ſ�ˢ������
	ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_CREDITCARD_COUNT, &unLen, sBuf);
	if((ret!=NDK_ERR_PARA)&&(ret!=NDK_ERR_NOT_SUPPORT))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
#else
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_CREDITCARD_COUNT, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("ˢ������:%s,���ж��Ƿ���ȷ",sBuf);
#endif

#if PRN_ENABLE
	//case11:ȡpos��ӡ�ܳ���
	memset(sBuf,0,sizeof(sBuf));
#if K21_ENABLE//K21ƽ̨��֧��ȡpos��ӡ�ܳ���
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_PRN_LEN, &unLen, sBuf)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#else
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_PRN_LEN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("��ӡ�ܳ���:%smm,���ж��Ƿ���ȷ",sBuf);
#endif
#endif

	//case12:ȡpos����������ʱ��
	memset(sBuf,0,sizeof(sBuf));
#if K21_ENABLE//K21ƽ̨��֧��ȡ��������ʱ��
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_RUNTIME, &unLen, sBuf)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
#else
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_RUNTIME, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("��������ʱ��:%s,���ж��Ƿ���ȷ",sBuf);
#endif

	//case13:ȡpos���������� 
	memset(sBuf,0,sizeof(sBuf));
#if K21_ENABLE//K21ƽ̨��֧��ȡ�������� 
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_KEY_COUNT, &unLen, sBuf)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
#else
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_KEY_COUNT, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("��������:%s,���ж��Ƿ���ȷ",sBuf);
#endif

	//case14:ȡCPU ���� 5892оƬ����ZA9LоƬ  add by chensj 2013/06/24
	memset(sBuf,0,sizeof(sBuf));
#if K21_ENABLE//K21ƽ̨��֧��ȡȡCPU ����
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_CPU_TYPE, &unLen, sBuf)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#else
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_CPU_TYPE, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("CPU ����Ϊ:%s,���ж��Ƿ���ȷ",sBuf);
#endif

#if !(defined ME15CHX||defined ME30MH||defined ME15B||defined ME66)
	//case15:ȡPOS����BOOT�汾��
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOOT_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("POS����BOOT�汾��Ϊ:%s,���ж��Ƿ���ȷ",sBuf);
#endif	

#if !K21_ENABLE
	//case16:ȡPOS����BIOS�Ĳ����汾��
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_PATCH_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("POS����BIOS������Ϊ:%s,���ж��Ƿ���ȷ",sBuf);

	//case16:ȡ��Կ�汾��Ϣ
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_PUBKEY_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("��Կ�汾��ϢΪ:%s,���ж��Ƿ���ȷ",sBuf);
#endif

#if defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME62//ME50NXû��PCI�̼��汾��
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_PCIFW_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("PCI�̼��汾��Ϊ:%s,���ж��Ƿ���ȷ",sBuf);
#endif

#if OVERSEAS_ENABLE    //����汾�ǻ�ȡTID
	//case17:��ȡ�ն� ���0��9��������ɹ�10λ
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TID, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("�ն� ���Ϊ:%s,���ж��Ƿ���ȷ",sBuf);
#else //�еͶ�POS��ȡTUSN   
	//case17:��ȡTUSN��Ϣ
#if !defined ME66 //ME66��֧�ֻ�ȡTUSN
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TUSN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("TUSNΪ:%s,���ж��Ƿ���ȷ",sBuf);
#endif
#endif

	//�Ͷ�������ȡwifi�̼��汾�� 20180503 sull add
#if NEW_WIFI_ENABLE
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_WIFIFW_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("WIFI�̼��汾��Ϊ:%s,���ж��Ƿ���ȷ",sBuf);
#endif

	//��׿��Ʒ������ȡ�ǽ�оƬ�ͺ�  20180503 sull add
#if ANDIROD_ENABLE
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_RFID_CHIP_TYPE, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("�ǽ�оƬ�ͺ�Ϊ:%s,���ж��Ƿ���ȷ",sBuf);
#endif

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

