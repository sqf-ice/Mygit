/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ۺ�ģ��
* file name		: 28.c
* Author 			: 
* version			: V1.0
* DATE			: 20110620
* directory 		: .\SRC\general\
* description		: NL829����
* related document	: NL829����Աʹ���ֲ�.doc������ĵ�
*				  
************************************************************************
* log			: 
* Revision 1.0  20110620 10:10 jennywone163
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"NL829"

#define	MAXWAITTIME 30
//#define	DATA16		"0123456789ABCDEF"

#define	DEFAULT_CNT_STR	"15000"	//"3000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(1000)

/*------------global variables definition-----------------------*/
static uchar gen28_bak[/*MAXLEN_DATA*/NL829_LEN_BLKDATA+8] = DATA16;
static int g_NL829type = DEVICE_NL829LCD;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern ushort PP60_cmd_L(uchar);//����ʱ��Ҫͬʱ����systest35������ú����޷��ҵ�ԭ�ͱ�������
extern ushort PP60_cmd_N(uchar);
extern ushort PP60_cmd_C(void);
extern ushort PP60_cmd_D(uchar, uchar, int);
/*---------------functions definition---------------------------*/
static int select_port(void)
{
	/*private & local definition*/
	int nKeyin = 0, port = 0;

	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("��ѡ����������Ĵ���:\n�ٴ���1\n�ڴ���2\n��USB����\n");
		switch (nKeyin)
		{
		case '1':
			port = PORT_NUM_COM1;
			break;
		case '2':
			port = PORT_NUM_COM2;
			break;
		case '3':
			port = PORT_NUM_USB_HOST;
			break;
		default:
			continue;
			break;
		}
		return port;
	}
}

static int conf_test_bps(void)
{
	/*private & local definition*/
	int nKeyin = 0, loop = 0, ret = -1;
	char *bps[] = {"9600", "19200", "38400", "57600", "115200"};
	static int first = TRUE;//�״ο����ϵ�ı�־
	
	/*process body*/
	// 1.������NL829���ӵ�POS���ڲ���
	cls_show_msg1(2, "��ѡ������豸���ӵ������");
	g_devicecom = select_port();

	if(g_devicecom!=PORT_NUM_USB_HOST)
	{
		// 2.����NL829���ڲ���
		cls_show_msg1(2, "����������豸ͨѶ������bps������");
		do
		{
			nKeyin = cls_show_msg("1.9600   2.19200\n3.38400  4.57600\n5.115200");
#define MAXMENUITEM ('5')

			switch(nKeyin)
			{
			case ESC:
				cls_show_msg1(2, "������ΪBPS57600(Ĭ��)...");
				nKeyin = '4';//ת��
			case '1':
			case '2':
			case '3':
			case '4':	
			case MAXMENUITEM:
				if(NDK_PortOpen(g_devicecom, (first==TRUE)?"57600":g_devicecombps)!=NDK_OK)
				{
					cls_show_msg1(2, "line %d:��ʼ��POS����ʧ��", __LINE__);
					return FAIL;
				}
				cls_printf("��������BPS��...");
				if(NL829_SetBPS(nKeyin-'1')!=NL829_SUCC)
				{
					cls_printf("����BPS�Զ�������...�����ĵȴ�");
					//�����ɹ�,�����п��ܵ�BPS���г���
					for(; loop<SZ_ARRAY(bps); loop++)
					{
						NDK_PortOpen(g_devicecom, bps[loop]);
						if(NL829_SetBPS(nKeyin-'1')==NL829_SUCC)
							break;
					}
					if(loop==SZ_ARRAY(bps))
					{
						cls_show_msg1(2, "line %d:����NL829��BPSʧ��,���������ٳ���", __LINE__);
						return FAIL;
					}
				}
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;			
			}
		}while(nKeyin<'1'||nKeyin>MAXMENUITEM);
#undef MAXMENUITEM	
		
		// 3.����NL829��ǰBPS����POS���ڵ�BPS
		if((ret=NDK_PortOpen(g_devicecom, bps[nKeyin-'1']))!=NDK_OK)//if(initaux(g_devicecom, bps[nKeyin-'1'], DB8|STOP1|NP)!=SUCC)
		{
			cls_show_msg("line %d:��ʼ��POS����ʧ��ret=%d", __LINE__, ret);
			return FAIL;
		}
		strcpy(g_devicecombps, bps[nKeyin-'1']);//g_devicecombps = bps[nKeyin-'1'];//g_bps�������ý��е�������
		first = FALSE;
	}
	else 
	{
		if((ret=NDK_PortOpen(g_devicecom, "115200,8,N,1,20"))!=NDK_OK)//if(initaux(g_devicecom, bps[nKeyin-'1'], DB8|STOP1|NP)!=SUCC)
		{
			cls_show_msg("line %d:��ʼ��POS����ʧ��ret=%d", __LINE__, ret);
			return FAIL;
		}
	}
	
	return SUCC;
}

static int select_NL829(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	int type[] = {DEVICE_NL829LCD, DEVICE_NL829MR_PP60RF, DEVICE_YH100};

	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("_ѡ��PP60����_\n"
							"1.�ɵĴ�LCD��\n"
							"2.829MR/PP60RF\n"//����LCD�ľɻ���Ҳѡ2
							"3.YH100\n");
		switch(nKeyin)
		{
		case '1':
		case '2':
		case '3':
			return type[nKeyin-'1'];
			break;
		case ESC:
			cls_show_msg1(2, "������Ϊ829MR��PP60RF(Ĭ��)...");
			return DEVICE_NL829MR_PP60RF;
			break;
		default:
			continue;
			break;
		}
	}
}

static uchar NL829_select_ICSAM(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	uchar ICSAM[] = {NL829_CARDNO_SAM1, NL829_CARDNO_SAM2, NL829_CARDNO_SAM3, NL829_CARDNO_SAM4, NL829_CARDNO_IC1};//��Ϊ829�Զ����,��Ҫ������POS��SAM�Ķ���

	/*process body*/
	while (1)
	{	
		nKeyin = cls_show_msg("_ѡ������_\n"
							"1.SAM1 2.SAM2\n"
							"3.SAM3 4.SAM4\n"
							"5.IC1\n");
		switch(nKeyin)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':	
			return ICSAM[nKeyin-'1'];
			break;
		case ESC:
			cls_show_msg1(2, "��ʹ��Ĭ�ϵ�SAM1...");
			return NL829_CARDNO_SAM1;
			break;
		default:
			continue;
			break;
		}
	}	
}

static void little2big_uint(uint src, uint *des)
{
	/*private & local definition*/
	uchar tmp[8] = {0}, *addr = (uchar *)&src;
	int loop = sizeof(uint)-1;
	
	/*process body*/
	for(; loop>=0; loop--)
		tmp[loop] = *addr++;
	memcpy((void *)des, tmp, sizeof(uint));
}

//ver���bios�汾�ַ���
static ushort NL829_GetBiosVer(uchar *ver)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_GETBIOSVER, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_GETBIOSVER_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen==0)
			return NL829_ERR_CMD_GETBIOSVER;
		memcpy(ver, append, appendlen);
		ver[appendlen] = 0x00;
	}
	return staval;
}

static ushort NL829_beep(ushort msec, uchar cnt)
{
	/*private & local definition*/
	ushort ret = 0, msec1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_BEEP, NL829_LEN_CMD);
	little2big_ushort(msec, &msec1);
	memcpy(data+NL829_OFFSET_ARGS, &msec1, sizeof(msec1));
	data[NL829_OFFSET_ARGS+sizeof(msec1)] = cnt;
#if 1	//��������
	if((ret=NL829_frame_factory(data, NL829_BEEP_DATALEN_SND, status, append, &appendlen, msec*cnt/1000))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_BEEP_APPLEN_RCV)
		return NL829_ERR_CMD_BEEP;
	return NL829_GETSTAVAL(status);
#else	//���ַ��������Ч��(����ʱ������)
	//ע��:  ��״̬�벢����������,�ڷ��������н����󷵻�,Ӧ��Ҳ���Բ���᷵��ֵ,�ڷ�����һ������ǰ����ջ��弴�ɡ�
	//�ʲ������NL829_frame_factory�ķ���ֵ,�˹��ж�ʵ��Ч������,��������Զ���سɹ�
	//�����з���,��״̬�򼰸����������Ԥ��һ��
	if((ret=NL829_frame_factory(data, NL829_BEEP_DATALEN_SND, status, append, &appendlen, 0))==NL829_SUCC)//˵�������շ�
	{
		if(appendlen!=NL829_BEEP_APPLEN_RCV)
			return NL829_ERR_CMD_BEEP;
		if(NL829_GETSTAVAL(status)==NL829_SUCC)
			return NL829_SUCC;
	}
	else if(ret==NL829_ERR_FRAME_SND||ret==NL829_ERR_FRAME_UNPACK)
		return ret;
	else if(ret==NL829_ERR_FRAME_STX)//�����δ���յ�Ӧ��Ĵ���
		return NL829_SUCC;
	else
		return ret;//�����Ľ��մ���
#endif
}

static ushort NL829_LEDCtl(uchar ctl)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_LEDCTL, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = ctl;
	if((ret=NL829_frame_factory(data, NL829_LEDCTL_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_LEDCTL_APPLEN_RCV)
		return NL829_ERR_CMD_LEDCTL;
	return NL829_GETSTAVAL(status);
}

//������LED�ǳ�������˸����\x31\x16   linwl 20120220
static ushort NL829_LEDCtlT(uchar ctl)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_LEDCTLT, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = ctl;
	if((ret=NL829_frame_factory(data, NL829_LEDCTLT_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_LEDCTLT_APPLEN_RCV)
		return NL829_ERR_CMD_LEDCTLT;
	return NL829_GETSTAVAL(status);
}

//purchase��balance����ӦΪBCD��ʽ
static ushort NL829_LCDCtl(uchar flag, uint purchase, uint balance)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	uint purchase1 = 0, balance1 = 0;
	
	/*process body*/
	memcpy(data, NL829_LCDCTL, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = flag;
	little2big_uint(purchase, &purchase1);
	little2big_uint(balance, &balance1);
	memcpy(data+NL829_OFFSET_ARGS+1, &purchase1, sizeof(purchase1));
	memcpy(data+NL829_OFFSET_ARGS+1+sizeof(purchase1), &balance1, sizeof(balance1));
	if((ret=NL829_frame_factory(data, NL829_LCDCTL_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_LCDCTL_APPLEN_RCV)
		return NL829_ERR_CMD_LCDCTL;
	return NL829_GETSTAVAL(status);
}

//ע��: ���������������ʾ��LCD�ϣ�������������ʾ�����иı�����������ݡ�
static ushort NL829_LCDCtl_new(uchar mode, uchar *title)
{
	/*private & local definition*/
	ushort ret = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	int appendlen = 0, datalen = NL829_LEN_CMD+1+sizeof(mode)+strlen((char *)title);
	
	/*process body*/
	if(datalen>NL829_MAXLEN_DATA)
		return NL829_ERR_FRAME_DATALEN2;
	memcpy(data, NL829_LCDCTL, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = 0x07;//Э��涨����
	data[NL829_OFFSET_ARGS+1] = mode;
	memcpy(data+NL829_OFFSET_ARGS+2, title, strlen((char *)title));
	if((ret=NL829_frame_factory(data, datalen, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_LCDCTL_APPLEN_RCV)
		return NL829_ERR_CMD_LCDCTL;
	return NL829_GETSTAVAL(status);
}

//����(12�ֽ�)���̻���(16�ֽ�)�ַ���
static ushort NL829_SetHWinfo(uchar flag, uchar *info)
{
	/*private & local definition*/
	ushort ret = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	int appendlen = 0, datalen = NL829_OFFSET_ARGS+strlen(NL829_SETHWINFO_CHECK)+1+strlen((char *)info);
	
	/*process body*/
	memcpy(data, NL829_SETHWINFO, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, (char *)NL829_SETHWINFO_CHECK);
	data[NL829_OFFSET_ARGS+strlen(NL829_SETHWINFO_CHECK)] = flag;
	strcpy((char *)data+NL829_OFFSET_ARGS+strlen(NL829_SETHWINFO_CHECK)+1, (char *)info);
	if((ret=NL829_frame_factory(data, datalen, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_SETHWINFO_APPLEN_RCV)
		return NL829_ERR_CMD_SETHWINFO;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_GetHWinfo(uchar flag, uchar *info)
{
	/*private & local definition*/
	ushort ret = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	int appendlen = 0;
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_GETHWINFO, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, NL829_GETHWINFO_CHECK);
	data[NL829_OFFSET_ARGS+strlen(NL829_GETHWINFO_CHECK)] = flag;
	if((ret=NL829_frame_factory(data, NL829_GETHWINFO_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	staval = NL829_GETSTAVAL(status);
	if(staval==NL829_SUCC)
	{
		if(appendlen==0)
			return NL829_ERR_CMD_GETHWINFO;
		else
		{
			memcpy(info, append, appendlen);
			info[appendlen] = 0x00;
		}
	}
	if((staval==NL829_ARGERR||staval==NL829_HWINFO_ERR)&&appendlen!=0)
		return NL829_ERR_CMD_GETHWINFO1;
	return staval;
}

//��ȡ�Ļ���(12�ֽ�)���̻���(16�ֽ�)����info��(�������ᴦ����ַ���),�ϲ�ӿڿɸ����ĵ���Ӧ���ʽ�����жϴ���
static ushort NL829LCD_HWinfo(uchar flag, uchar *info)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829LCD_HWINFO, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, (char *)NL829LCD_HWINFO_CHECK);
	data[NL829_OFFSET_ARGS+strlen(NL829LCD_HWINFO_CHECK)] = flag;
	if((ret=NL829_frame_factory(data, NL829LCD_HWINFO_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	staval = NL829_GETSTAVAL(status);
	if(flag==NL829LCD_FLAG_SETSN&&appendlen!=0)
		return NL829LCD_ERR_CMD_HWINFO;
	if(flag==NL829LCD_FLAG_GETMACHINE||flag==NL829LCD_FLAG_GETMERCHANT)
	{
		if(staval==NL829_SUCC)
		{
			if(appendlen==0)
				return NL829LCD_ERR_CMD_HWINFO;
			else
			{
				memcpy(info, append, appendlen);
				info[appendlen] = 0x00;
				//return NL829_SUCC;
			}		
		}
		if(staval==0x0002&&appendlen!=0)
			return NL829LCD_ERR_CMD_HWINFO1;
	}
	return staval;
}

//rtcΪ�����8�ֽ�BCDʱ��
static ushort NL829LCD_GetRTC(uchar *rtc)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829LCD_GETRTC, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829LCD_GETRTC_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen!=NL829LCD_GETRTC_APPLEN_RCV)
			return NL829LCD_ERR_CMD_GETRTC;
		memcpy(rtc, append, appendlen);
		//ver[appendlen] = 0x00;
	}
	return staval;
}

//rtcΪ�����BCDʱ��,inlen����ֵΪ8
static ushort NL829LCD_SetRTC(uchar *rtc, uchar inlen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829LCD_SETRTC, NL829_LEN_CMD);
	memcpy(data+NL829_LEN_CMD, rtc, inlen);
	if((ret=NL829_frame_factory(data, NL829_LEN_CMD+inlen, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829LCD_SETRTC_APPLEN_RCV)
		return NL829LCD_ERR_CMD_SETRTC;
	return NL829_GETSTAVAL(status);
}

//���ɹ�,�򷵻�һ�ֽ�״̬�����sta��
static ushort NL829_ICSAM_GetSta(uchar *sta)
{
	/*private & local definition*/
	ushort ret = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	int appendlen = 0;
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_ICSAM_GETSTA, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_ICSAM_GETSTA_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	staval = NL829_GETSTAVAL(status);
	if(staval==NL829_SUCC)
	{
		if(appendlen!=NL829_ICSAM_GETSTA_APPLEN_RCV)
			return NL829_ERR_ICSAM_GETSTA;
		else
			*sta = append[0];
	}
	if(staval==NL829_ARGERR&&appendlen!=0)
		return NL829_ERR_ICSAM_GETSTA1;
	return staval;
}

//����sam��/psam�����ϵ�ָ��
static ushort NL829_ICSAM_poweron(uchar cardnum, uchar *cardtype, uchar *ATR, int *ATRlen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_ICSAM_POWERON, NL829_LEN_CMD);
	memcpy(data+NL829_OFFSET_ARGS, NL829_ICSAM_POWERON_RESERVED, NL829_ICSAM_POWERON_RESERVED_LEN);
	data[NL829_OFFSET_ARGS+NL829_ICSAM_POWERON_RESERVED_LEN] = cardnum;
	if((ret=NL829_frame_factory(data, NL829_ICSAM_POWERON_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	staval = NL829_GETSTAVAL(status);
	if(staval==NL829_SUCC)
	{
		if(appendlen==0)
			return NL829_ERR_ICSAM_POWERON;
		else
		{
			*cardtype = append[0];
			*ATRlen = appendlen-1;
			memcpy(ATR, append+1, *ATRlen);
			ATR[*ATRlen] = 0x00;
		}
	}
	if(staval==NL829_ARGERR&&appendlen!=0)
		return NL829_ERR_ICSAM_POWERON1;
	if(staval==NL829_SAM_FAIL_POWERON&&appendlen!=1)
		return NL829_ERR_ICSAM_POWERON2;
	return staval;
}

//����sam��/psam�����µ�ָ��
static ushort NL829_ICSAM_powerdown(uchar cardnum)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_ICSAM_POWERDOWN, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = cardnum;
	if((ret=NL829_frame_factory(data, NL829_ICSAM_POWERDOWN_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_ICSAM_POWERDOWN_DATALEN_RCV)
		return NL829_ERR_ICSAM_POWERDOWN;
	return NL829_GETSTAVAL(status);
}

//�����Ӧ�������(���ִ�)
static ushort NL829_CPU_active(ushort sec, uchar *type, uchar *UID, int *UIDlen, uchar *ATR, int *ATRlen)
{
	/*private & local definition*/
	ushort ret = 0, sec1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_CPU_ACTIVE, NL829_LEN_CMD);
	little2big_ushort(sec, &sec1);
	memcpy(data+NL829_OFFSET_ARGS, &sec1, sizeof(sec1));
	if((ret=NL829_frame_factory(data, NL829_CPU_ACTIVE_DATALEN_SND, status, append, &appendlen, sec))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen!=0)
		{
			//���ж�,�޴��˲Ÿ����������
			if(append[1]+append[append[1]+2]+3!=appendlen)
				return NL829_ERR_CPU_ACTIVE;
			*type = append[0];
			*UIDlen = append[1];
			memcpy(UID, &append[2], *UIDlen);
			*ATRlen = append[append[1]+2];
			memcpy(ATR, &append[append[1]+3], *ATRlen);
			return NL829_SUCC;
		}
		else
			return NL829_ERR_CPU_ACTIVE1;
	}
	else
		return staval;
}

static ushort NL829_CPU_discnnt(ushort sec)
{
	/*private & local definition*/
	ushort ret = 0, sec1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_CPU_DISCNNT, NL829_LEN_CMD);
	little2big_ushort(sec, &sec1);
	memcpy(data+NL829_OFFSET_ARGS, &sec1, sizeof(sec1));
	if((ret=NL829_frame_factory(data, NL829_CPU_DISCNNT_DATALEN_SND, status, append, &appendlen, sec))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_CPU_DISCNNT_APPLEN_RCV)
		return NL829_ERR_CPU_DISCNNT;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_APDUrw(uchar cardno, uchar *apdu, int apdulen, uchar *r_apdu, int *r_apdulen)
{
	/*private & local definition*/
	ushort ret = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	int appendlen = 0, datalen = NL829_LEN_CMD+1+apdulen;
	ushort staval = NL829_FAIL;
	
	/*process body*/
	*r_apdulen = 0;
	memcpy(data, NL829_APDURW, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = cardno;
	memcpy(data+NL829_OFFSET_ARGS+1, apdu, apdulen);
	if((ret=NL829_frame_factory(data, datalen, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		*r_apdulen = appendlen;
		if(appendlen!=0)
			memcpy(r_apdu, append, appendlen);	
	}
	else
	{
		if(appendlen!=0)
			return NL829_ERR_APDURW;
	}
	return staval;
}

//Ӧ�ó����ڶԷǽӴ�����CPU������Mifare-1�������������󣬻�������Ѱ����ʱ��Ӧ�ùر���Ƶ���ߡ�
static ushort NL829_AntennaCtl(ushort msec)
{
	/*private & local definition*/
	ushort ret = 0, msec1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_ANTENNACTL, NL829_LEN_CMD);
	little2big_ushort(msec, &msec1);
	memcpy(data+NL829_OFFSET_ARGS, &msec1, sizeof(msec1));
	if((ret=NL829_frame_factory(data, NL829_ANTENNACTL_DATALEN_SND, status, append, &appendlen, (msec==0xffff)?0:(msec/1000)))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_ANTENNACTL_APPLEN_RCV)
		return NL829_ERR_ANTENNACTL;
	return NL829_GETSTAVAL(status);
}

//�����Ӧ�������(���ִ�)
static ushort NL829_M1_active(uchar mode, ushort sec, uchar *ATQ, int *ATQlen, uchar *SNR, int *SNRlen, uchar *SAK, int *SAKlen)
{
	/*private & local definition*/
	ushort ret = 0, sec1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_M1_ACTIVE, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = mode;
	little2big_ushort(sec, &sec1);
	memcpy(data+NL829_OFFSET_ARGS+1, &sec1, sizeof(sec1));
	if((ret=NL829_frame_factory(data, NL829_M1_ACTIVE_DATALEN_SND, status, append, &appendlen, sec))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen!=0)
		{
			//���ж�,�޴��˲Ÿ����������
			if(append[0]+append[append[0]+1]+append[append[0]+append[append[0]+1]+2]+3!=appendlen)
				return NL829_ERR_M1_ACTIVE;
			*ATQlen = append[0];
			memcpy(ATQ, &append[1], *ATQlen);
			*SNRlen = append[append[0]+1];
			memcpy(SNR, &append[append[0]+2], *SNRlen);
			*SAKlen = append[append[0]+append[append[0]+1]+2];
			memcpy(SAK, &append[append[0]+append[append[0]+1]+3], *SAKlen);		
			return NL829_SUCC;
		}
		else
			return NL829_ERR_M1_ACTIVE1;
	}
	else
		return staval;
}

//���ر����ߣ�ʹ������HALT״̬��������WUPAѰ���������»��ѡ�
static ushort NL829_M1_halt(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_M1_HALT, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_M1_HALT_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_HALT_APPLEN_RCV)
		return NL829_ERR_M1_HALT;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_M1_storekey(uchar mode, uchar sector, uchar *key)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_M1_STOREKEY, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = mode;
	data[NL829_OFFSET_ARGS+1] = sector;
	memcpy(data+NL829_OFFSET_ARGS+2, key, NL829_LEN_KEY);
	if((ret=NL829_frame_factory(data, NL829_M1_STOREKEY_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_STOREKEY_APPLEN_RCV)
		return NL829_ERR_M1_STOREKEY;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_M1_loadkey(uchar mode, uchar sector)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_M1_LOADKEY, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = mode;
	data[NL829_OFFSET_ARGS+1] = sector;
	if((ret=NL829_frame_factory(data, NL829_M1_LOADKEY_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_LOADKEY_APPLEN_RCV)
		return NL829_ERR_M1_LOADKEY;
	return NL829_GETSTAVAL(status);
}

//SNR��ʵ����UID��,����"����ͻ"�׶η���
static ushort NL829_M1_auth(uchar mode, uchar *SNR, int SNRlen, uchar block)//20161019����SNRlen����,SNR���ܰ���0x00������strlen����
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_M1_AUTH, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = mode;
	memcpy(data+NL829_OFFSET_ARGS+1, SNR, SNRlen/*strlen((char *)SNR)*//*NL829_LEN_SNR*/);//�ĺ�ȡ��M����uid�Ż���ȷ
	data[NL829_OFFSET_ARGS+1+SNRlen/*strlen((char *)SNR)*//*NL829_LEN_SNR*/] = block;
	if((ret=NL829_frame_factory(data, NL829_M1_AUTH_DATALEN_SND-NL829_LEN_SNR+SNRlen/*strlen((char *)SNR)*/, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_AUTH_APPLEN_RCV)
		return NL829_ERR_M1_AUTH;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_M1_auth_ex(uchar mode, uchar *SNR, int SNRlen, uchar block, uchar *key)//20161019����SNRlen����,SNR���ܰ���0x00������strlen����
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_M1_AUTH_EX, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = mode;
	memcpy(data+NL829_OFFSET_ARGS+1, SNR, SNRlen/*strlen((char *)SNR)*//*NL829_LEN_SNR*/);//�ĺ�ȡ��M����uid�Ż���ȷ
	data[NL829_OFFSET_ARGS+1+SNRlen/*strlen((char *)SNR)*//*NL829_LEN_SNR*/] = block;
	memcpy(data+NL829_OFFSET_ARGS+SNRlen/*strlen((char *)SNR)*//*NL829_LEN_SNR*/+2, key, NL829_LEN_KEY);
	if((ret=NL829_frame_factory(data, NL829_M1_AUTH_EX_DATALEN_SND-NL829_LEN_SNR+SNRlen/*strlen((char *)SNR)*/, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_AUTH_EX_APPLEN_RCV)
		return NL829_ERR_M1_AUTH_EX;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_M1_BLKRead(uchar block, uchar *out)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_M1_BLKREAD, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = block;
	if((ret=NL829_frame_factory(data, NL829_M1_BLKREAD_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen!=NL829_M1_BLKREAD_APPLEN_RCV)
			return NL829_ERR_M1_BLKREAD;
		memcpy(out, append, appendlen);
	}
	else
	{
		if(appendlen!=0)
			return NL829_ERR_M1_BLKREAD1;
	}
	return staval;
}

static ushort NL829_M1_BLKWrite(uchar block, uchar *wdata)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_M1_BLKWRITE, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = block;
	memcpy(data+NL829_OFFSET_ARGS+1, wdata, NL829_LEN_BLKDATA);
	if((ret=NL829_frame_factory(data, NL829_M1_BLKWRITE_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_BLKWRITE_APPLEN_RCV)
		return NL829_ERR_M1_BLKWRITE;
	return NL829_GETSTAVAL(status);
}

//wdata 4�ֽ�,�ǲ���С�����(��POS����ֲ����,��Ҳ��С����,����NL829��඼���ô�����ͬ)
//��ֱ��ʹ��unit*,ֱ��ʹ�ò�����ת��
static ushort __NL829_M1_BLKIncDec(uchar cmd_L, uchar block, uint *wdata)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	data[0] = NL829_M1_BLKINCDEC_H;
	data[1] = cmd_L;
	data[2] = block;
	memcpy(data+NL829_OFFSET_ARGS+1, wdata, NL829_LEN_BLKINCDEC);
	if((ret=NL829_frame_factory(data, NL829_M1_BLKINCDEC_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_BLKINCDEC_APPLEN_RCV)
		return NL829_ERR_M1_BLKINCDEC;
	return NL829_GETSTAVAL(status);
}

static ushort NL829_M1_BLKInc(uchar block, uint *wdata){return __NL829_M1_BLKIncDec(NL829_M1_BLKINC_L, block, wdata);}

static ushort NL829_M1_BLKDec(uchar block, uint *wdata){return __NL829_M1_BLKIncDec(NL829_M1_BLKDEC_L, block, wdata);}

//ratsָ���װ�����͸����̲����ܼ�����Ӧ����ʽ��ǰ���NL829_M1_BLKRead��һģһ�� add by zhengjd 20141029
int NL829_RfidTypeARats(uchar cid,int *pnDatalen, uchar *psDatabuf)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_RFIDTYPEARATS, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS] = cid;
	if((ret=NL829_frame_factory(data, NL829_RFIDTYPEARATS_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		*pnDatalen = appendlen;
		if(appendlen!=0)
			memcpy(psDatabuf, append, appendlen);	
	}
	else
	{
		if(appendlen!=0)
			return NL829_ERR_APDURW;
	}
	return NL829_GETSTAVAL(status);

}

int NL829CpuActiveKeyInput(ushort sec, uchar *type, uchar *UID, int *UIDlen, uchar *ATR, int *ATRlen,uchar *keyin)
{
	/*private & local definition*/
	ushort ret = 0, sec1 = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_CPU_ACTIVE_KEY_INPUT, NL829_LEN_CMD);
	little2big_ushort(sec, &sec1);
	memcpy(data+NL829_OFFSET_ARGS, &sec1, sizeof(sec1));
	if((ret=NL829_frame_factory(data, NL829_CPU_ACTIVE_KEY_INPUT_DATALEN, status, append, &appendlen, sec))!=NL829_SUCC)
		return ret;
	staval=NL829_GETSTAVAL(status);
	if(staval==NL829_SUCC)
	{
		if(appendlen!=0)
		{
			//���ж�,�޴��˲Ÿ����������
			if(append[1]+append[append[1]+2]+3!=appendlen)
				return NL829_ERR_CPU_ACTIVE;
			*type = append[0];
			*UIDlen = append[1];
			memcpy(UID, &append[2], *UIDlen);
			*ATRlen = append[append[1]+2];
			memcpy(ATR, &append[append[1]+3], *ATRlen);
			return NL829_SUCC;
		}
		else
			return NL829_ERR_CPU_ACTIVE1;
	}
	else if(staval==NL829_CPU_ACTIVE_INPUT_SUCC)
	{
		memcpy(keyin,append,appendlen);
		return NL829_SUCC;
	}   
	else
		return staval;
}

//����SP10ɨ��ͷ��ʼɨ������
static ushort NL829_SCAN_Startscan(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_SCAN_STARTSCAN, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_SCANSTARTSCAN_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_SCANSTARTSCAN_APPLEN_RCV)
		return NL829_ERR_SCAN_START;
	return NL829_GETSTAVAL(status);
}

//����SP10ɨ��ͷ��ȡɨ��ͷ��������ָ��
static ushort NL829_SCAN_GetData(uchar *out, int *outlen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_SCAN_GETDATA, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_SCANGETDATA_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		*outlen = appendlen;
		memcpy(out, append, appendlen);//��ɨ�赽����������
	}
	else
	{
		if(appendlen!=0)
			return NL829_ERR_SCAN_GETDATA;
	}

	return staval;
}

//����SP10ɨ��ͷ�ر�ɨ������
static ushort NL829_SCAN_Stopscan(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_SCAN_STOPSCAN, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_SCANSTOPSCAN_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_SCANSTOPSCAN_APPLEN_RCV)
		return NL829_ERR_SCAN_STOP;
	return NL829_GETSTAVAL(status);
}

//����SP10ɨ��ͷ����ʽɨ������
static ushort NL829_SCAN_Doscan(int timeout, uchar *out, int *outlen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_SCAN_DOSCAN, NL829_LEN_CMD);
	data[NL829_LEN_CMD]=timeout;
	if((ret=NL829_frame_factory(data, NL829_SCANDOSCAN_DATALEN_SND, status, append, &appendlen, timeout))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen==0)
			return NL829_ERR_SCAN_DOSCAN;
		else
			*outlen = appendlen;
			memcpy(out, append, appendlen);//��ɨ�赽����������
	}

	return staval;
}
#if 0  //���ݿ����ṩ��NL829MR_PP60RF����Աʹ���ֲ�v1.7  (ɾ��ɨ��ͷ�Ĵ���дָ��) ɨ��ͷ���ùر�
//����SP10ɨ��ͷ���üĴ���ָ��
static ushort NL829_SCAN_SetScan(uchar *setdata, int datalen, uchar *out, int *outlen)
{
	/*private & local definition*/
	ushort ret = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	int appendlen = 0;
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_SCAN_SETSCAN, NL829_LEN_CMD);
	memcpy(data+NL829_LEN_CMD, setdata, datalen);
	if((ret=NL829_frame_factory(data, NL829_SCANSETSCAN_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		if(appendlen!=NL829_SCANSETSCAN_APPLEN_RCV)
			return NL829_ERR_SCAN_SETSCAN;
		else
			*outlen = appendlen;
			memcpy(out, append, appendlen);//��ɨ�赽����������
	}
	
	return staval;
}
#endif
//����SP10T��ʾͼƬ
static ushort NL829_Show_Image(uchar *x,uchar *y,uchar *w,uchar *h,uchar *cnum,uchar *snum,uchar *sdata, int dlen)
{
	
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_SHOW_IMAGE, NL829_LEN_CMD);
	memcpy(data+NL829_OFFSET_ARGS,x,2);
	memcpy(data+NL829_OFFSET_ARGS+2,y,2);
	memcpy(data+NL829_OFFSET_ARGS+4,w,2);
	memcpy(data+NL829_OFFSET_ARGS+6,h,2);
	memcpy(data+NL829_OFFSET_ARGS+8,cnum,2);
	memcpy(data+NL829_OFFSET_ARGS+10,snum,2);
	memcpy(data+NL829_OFFSET_ARGS+12,sdata,dlen);
	if((ret=NL829_frame_factory(data, NL829_SHOWIMAGE_DATALEN_SND+dlen, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_SHOWIMAGE_APPLEN_RCV)
		return NL829_ERR_SHOW_IMAGE;
	return NL829_GETSTAVAL(status);
}

//����������ȡ��ص�����ָ��20170804 by jiangym
static ushort NL829_Start_Recv_Key(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_STR_REC_KEY, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_STR_REC_KEY_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_STR_REC_KEY_APPLEN_RCV)
		return NL829_ERR_STR_REC_KEY;
	return NL829_GETSTAVAL(status);
}
static ushort NL829_Get_Key_Value(uchar *outdata, int *datalen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	ushort staval = NL829_FAIL;
	
	/*process body*/
	memcpy(data, NL829_GET_KEY_VALUE, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_GET_KEY_VALUE_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;	
	if((staval=NL829_GETSTAVAL(status))==NL829_SUCC)
	{
		*datalen = appendlen;
		memcpy(outdata, append, appendlen);//����ȡ�ļ�ֵ����
	}
	else
	{
		if(appendlen!=0)
			return NL829_ERR_GET_KEY_VALUE;
	}
	return staval;
}
static ushort NL829_End_Recv_Key(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_END_REC_KEY, NL829_LEN_CMD);
	if((ret=NL829_frame_factory(data, NL829_END_REC_KEY_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_END_REC_KEY_APPLEN_RCV)
		return NL829_ERR_END_REC_KEY;
	return NL829_GETSTAVAL(status);
}
static ushort NL829_Scan_Animation(uchar ctrl,uchar icon_num,uchar speed,uchar *d_time,uchar *d_icons,int n)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, NL829_SCAN_ANIMATION, NL829_LEN_CMD);
	data[NL829_OFFSET_ARGS]=ctrl;
	data[NL829_OFFSET_ARGS+1]=icon_num;
	data[NL829_OFFSET_ARGS+2]=speed;
	memcpy(data+NL829_OFFSET_ARGS+3,d_time,2);
	memcpy(data+NL829_OFFSET_ARGS+5,d_icons,n);
	if((ret=NL829_frame_factory(data, NL829_SCAN_ANIMATION_DATALEN_SND+n, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_SCAN_ANIMATION_APPLEN_RCV)
		return NL829_ERR_SCAN_ANIMATION;
	return NL829_GETSTAVAL(status);
}


#if 0	//ʵ��������
static ushort __NL829_M1_BLKTransRest(uchar cmd_L, uchar block)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};
	
	/*process body*/
	data[0] = NL829_M1_BLKTRANSREST_H;
	data[1] = cmd_L;
	data[2] = block;
	if((ret=NL829_frame_factory(data, NL829_M1_BLKTRANSREST_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC)
		return ret;
	if(appendlen!=NL829_M1_BLKTRANSREST_APPLEN_RCV)
		return NL829_ERR_M1_BLKTRANSREST;
	return NL829_GETSTAVAL(status);
}

//�ر�˵��:
//Ŀǰ��Ȼָ���ĵ����Ǹ�������������ָ��,�����������������Ѿ������˴��Ͳ�������Ҫ��ִ�д��ͣ������еĿ��᷵�ز���Ԥ֪�Ĵ���
//ֻ�ǳ����ĵ�������,��������ʵ��һ�¶��ѡ���Ҫʹ��(��Ȼ��������������ָ��Ӧֱ�ӷ��سɹ�)��
static ushort NL829_M1_BLKTrans(uchar block){return __NL829_M1_BLKTransRest(NL829_M1_BLKTRANS_L, block);}

static ushort NL829_M1_BLKRest(uchar block){return __NL829_M1_BLKTransRest(NL829_M1_BLKREST_L, block);}
#endif

//L4 testcmdҪ������,�쳣(�����ô����BPS)��������д,������ʹ�õ������Ͻ������伴��,�����ӿ��Խ�ʡ�ظ�����
static void test_control_cmd(void)
{
	/*private & local definition*/
	uchar buf[NL829_MAXLEN_DATA] = {0}, old_time[16] = {0}, new_time[16] = {0};
	ushort staval = NL829_FAIL;
	int loop = 0, test_rtc = FALSE;
	char *led[] = {"��", "��", "��", "��"};
	
	/*process body*/
	cls_show_msg1(2, "�ǿ�������������...");//����ʾ��ˢ�����Բ˵�
	//����ǰ��
	NL829_LEDCtl(0x0f);
	NL829_LEDCtlT(0x0f);
	if(g_NL829type==DEVICE_NL829LCD)
		NL829_LCDCtl(0x06, 0x00000000, 0x00000000);

	if(g_NL829type!=DEVICE_YH100)//YH100 ʹ��USB���ڲ������ò�����
	{
		//��������BPS(���쳣���,�������ͨ��"����"�ı�����BPS��������ָ���ȫ����Լ���)
		if((staval=NL829_SetBPS(NL829_BPS115200+1))!=NL829_BPS_NOTSUPPORT)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return;
		}
		if((staval=NL829_SetBPS(0xff))!=NL829_BPS_NOTSUPPORT)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return;
		}
	}

	//����ȡ�汾
	if((staval=NL829_GetBiosVer(buf))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	else
	{
		if(cls_show_msg("ȡ���汾:%s,[ENTER]ͨ��[����]��ͨ��", buf)!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return;
		}
	}

	if(g_NL829type!=DEVICE_YH100)//��֧�ַ�������LED��
	{
		//���Է�����
		if((staval=NL829_beep(0, 100))!=NL829_SUCC||cls_show_msg("NL829������beep��,��[ENTER];����[����]")==ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return;
		}
		cls_show_msg1(2, "����������ͨ��!");

		if((staval=NL829_beep(10000, 0))!=NL829_SUCC||cls_show_msg("NL829������beep��,��[ENTER];����[����]")==ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return;
		}
		cls_show_msg1(2, "����������ͨ��!");

		cls_show_msg("NL829������3��beep(ÿ������2s),���������...");
		if((staval=NL829_beep(2000, 3))!=NL829_SUCC||cls_show_msg("NL829������3��beep,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return;
		}
	
		//LED������ʾ
		cls_show_msg1(2, "NL829 LED����һ����...");
		strcpy((char *)buf, "\x8f\x4f\x2f\x1f");//Ԥ���������(����ǰ��)
		for(loop=0; loop<12; loop++)
		{
			if((staval=NL829_LEDCtl(buf[loop%4]))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			cls_printf("%s����...", led[loop%4]);
			sleep(2);//msdelay(500);//ʱ�䳤Щ,���ò��������㹻ʱ��ȷ��
		}
		NL829_LEDCtl(0x0f);
		cls_show_msg1(2, "NL829 LED��ȫ��ȫ��...");
		for(loop=0; loop<8; loop++)
		{
			if((staval=NL829_LEDCtl((loop%2)?0x0f:0xff))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			sleep(2);//msdelay(500);
		}
		//LED ��˸���� by linwl 20120220
		cls_show_msg1(2, "NL829 LED����һ��˸...");
		for(loop=0; loop<12; loop++)
		{
			if((staval=NL829_LEDCtlT(buf[loop%4]))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			cls_printf("%s����˸...", led[loop%4]);
			sleep(5);//msdelay(500);//ʱ�䳤Щ,���ò��������㹻ʱ��ȷ��
		}
		NL829_LEDCtlT(0x0f);
		cls_show_msg1(2, "NL829 LED��ȫ��ȫ��...");
		for(loop=0; loop<8; loop++)
		{
			if((staval=NL829_LEDCtlT((loop%2)?0x0f:0xff))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			sleep(5);//msdelay(500);
		}
		cls_show_msg1(2, "NL829 LED��ȫ����˸...");
		NL829_LEDCtlT(0xff);
		NL829_LEDCtl(0x0f);
		NL829_LEDCtlT(0xff);
		sleep(10);
		if(cls_show_msg("NL829 LED������˸,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			goto ERR;
		}
		NL829_LEDCtlT(0x0f);
		if(cls_show_msg("NL829 LEDЧ��OK,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			goto ERR;
		}
	}
	else
	{
		//LED������ʾ
		cls_show_msg1(2, "NL829 LED���ƽ�����...");
		strcpy((char *)buf, "\x8f\x4f\x2f\x1f");//Ԥ���������(����ǰ��)
		if((staval=NL829_LEDCtl(0x1f))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		cls_printf("������...");
		sleep(2);//msdelay(500);//ʱ�䳤Щ,���ò��������㹻ʱ��ȷ��
		NL829_LEDCtl(0x0f);
		cls_show_msg1(2, "NL829 LED���ƽ�����...");
		for(loop=0; loop<8; loop++)
		{
			if((staval=NL829_LEDCtl((loop%2)?0x0f:0xff))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			sleep(2);//msdelay(500);
		}
		//LED ��˸���� by linwl 20120220
		cls_show_msg1(2, "NL829 ��LED����˸...");
		if((staval=NL829_LEDCtlT(0x1f))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		cls_printf("������˸...");
		sleep(5);//msdelay(500);//ʱ�䳤Щ,���ò��������㹻ʱ��ȷ��
		NL829_LEDCtlT(0x0f);
		cls_show_msg1(2, "NL829 LED������...");
		for(loop=0; loop<8; loop++)
		{
			if((staval=NL829_LEDCtlT((loop%2)?0x0f:0xff))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			sleep(5);//msdelay(500);
		}
		cls_show_msg1(2, "NL829 ��LED����˸...");
		NL829_LEDCtlT(0xff);
		NL829_LEDCtl(0x0f);
		NL829_LEDCtlT(0xff);
		sleep(10);
		if(cls_show_msg("NL829 LED������˸,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			goto ERR;
		}
		NL829_LEDCtlT(0x0f);
		if(cls_show_msg("NL829 LEDЧ��OK,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			goto ERR;
		}
	}

	//LCD����
	if(g_NL829type==DEVICE_NL829LCD)
	{
		//�쳣
		if((staval=NL829_LCDCtl(0x00, 0xaaaaaaaa, 0xffffffff))!=NL829LCD_LCD_FMTERR||cls_show_msg("NL829����ʾ�κν��,��[ENTER];����[����]")==ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;			
		}
		if((staval=NL829_LCDCtl(0x08, 0x00000000, 0x00000000))==NL829_SUCC||cls_show_msg("NL829����ʾ�κν��,��[ENTER];����[����]")==ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;			
		}
		//NL829_LCDCtl_new���쳣�����ൽ����������ȥ��

		//����
		if((staval=NL829_LCDCtl_new(NORMAL_TEXT, (uchar *)"����LCD���Կ�ʼ"))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl(0x06, 0x00000000, 0x00000000))!=NL829_SUCC||cls_show_msg("NL829����ʾ��ӭʹ��,��[ENTER];����[����]")!=ENTER||cls_show_msg("��NL829�ı�����\"����LCD���Կ�ʼ\",��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}		
		//���²����л��������ʾʱ�Ḳ�Ǳ���,�ʸı���Բ���,ֻҪ�������һ�¼���cls_show_msg("���²�����,NL829�ı�������Ϊ\"LCD���Կ�ʼ\",���������...");
		if((staval=NL829_LCDCtl(0x00, 0x00000000, 0x00000000))!=NL829_SUCC||cls_show_msg("NL829����ʾ���0.00Ԫ,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl_new(NORMAL_TEXT, (uchar *)"�㿴�����о��Ǵ������!"))!=NL829LCD_LCD_FMTERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;			
		}
		if((staval=NL829_LCDCtl(0x01, 0x00000001, 0x00000001))!=NL829_SUCC||cls_show_msg("NL829����ʾ���׳ɹ������0.01Ԫ,��[ENTER];����[����]")!=ENTER/*||cls_show_msg("��NL829�ı�����\"����LCD���Կ�ʼ\",��[ENTER];����[����]")!=ENTER*/)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl_new(0x02, (uchar *)"����������!"))!=NL829LCD_LCD_FMTERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;			
		}
		if((staval=NL829_LCDCtl(0x02, 0x00000010, 0x00000010))!=NL829_SUCC||cls_show_msg("NL829����ʾ����ʧ��,��[ENTER];����[����]")!=ENTER/*||cls_show_msg("��NL829�ı�����\"����LCD���Կ�ʼ\",��[ENTER];����[����]")!=ENTER*/)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl(0x03, 0x00000100, 0x00000100))!=NL829_SUCC||cls_show_msg("NL829����ʾ���㼰���1.00Ԫ,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl(0x04, 0x99999999, 0x99999999))!=NL829_SUCC||cls_show_msg("NL829����ʾ�����쳣��999999.99Ԫ,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl(0x05, 0x00000010, 0x00000010))!=NL829_SUCC||cls_show_msg("NL829����ʾ��ˢ��������0.10Ԫ,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl_new(REVERSE_TEXT, (uchar *)"����LCD���Խ���"))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_LCDCtl(0x06, 0x00000000, 0x00000000))!=NL829_SUCC||cls_show_msg("NL829����ʾ��ӭʹ��,��[ENTER];����[����]")!=ENTER||cls_show_msg("��NL829�ı�����\"����LCD���Խ���\",��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}

	//Ӳ����Ϣ�������ȡ
	if(g_NL829type==DEVICE_NL829MR_PP60RF||g_NL829type==DEVICE_YH100)
	{
		//�쳣
		if((staval=NL829_GetHWinfo(NL829_FLAG_MACHINE+1, buf))!=NL829_ARGERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_SetHWinfo(NL829_FLAG_MACHINE+1, (uchar *)NL829_HWINFO))!=NL829_ARGERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		//����
		//NL829_FLAG_SN
		if((staval=NL829_GetHWinfo(NL829_FLAG_SN, buf))==NL829_HWINFO_ERR)//��δ����,������
		{
			if((staval=NL829_SetHWinfo(NL829_FLAG_SN, (uchar *)NL829_HWINFO))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			else
			{
				if((staval=NL829_GetHWinfo(NL829_FLAG_SN, buf))!=NL829_SUCC||strcmp((char *)buf, NL829_HWINFO))
				{
					cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
					goto ERR;
				}
			}
		}
		else if(staval!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		//NL829_FLAG_MACHINE
		if((staval=NL829_GetHWinfo(NL829_FLAG_MACHINE, buf))==NL829_HWINFO_ERR)//��δ����,������
		{
			if((staval=NL829_SetHWinfo(NL829_FLAG_MACHINE, (uchar *)NL829_HWINFO))!=NL829_SUCC)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
			else
			{
				if((staval=NL829_GetHWinfo(NL829_FLAG_MACHINE, buf))!=NL829_SUCC||strcmp((char *)buf, NL829_HWINFO))
				{
					cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
					goto ERR;
				}
			}
		}
		else if(staval!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}
	else//DEVICE_NL829LCD//Ŀǰ����ʹ��PC��������߽�������������SN����,����ֻ����һ�¶�ȡ����,�̻���
	{
		//�쳣
		if((staval=NL829LCD_HWinfo(NL829LCD_FLAG_GETMERCHANT+1, buf))!=NL829_ARGERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		//����
		//NL829LCD_FLAG_SETSN
		cls_show_msg("��ʹ��SN���ع��߲���SN����,�����������������...");
		//NL829LCD_FLAG_GETMACHINE
		if((staval=NL829LCD_HWinfo(NL829LCD_FLAG_GETMACHINE, buf))==NL829_SUCC)
		{
			if(cls_show_msg("ȡ������:%s,��ȷ��[ENTER];����[����]", buf)!=ENTER)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
		}
		else if(staval==NL829_HWINFO_ERR)
		{
			if(cls_show_msg("δ��ȡ������,��ȷ��.��ȷ��[ENTER];����[����]")!=ENTER)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
		}
		else
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		//NL829LCD_FLAG_GETMERCHANT
		if((staval=NL829LCD_HWinfo(NL829LCD_FLAG_GETMERCHANT, buf))==NL829_SUCC)
		{
			if(cls_show_msg("ȡ���̻���:%s,��ȷ��[ENTER];����[����]", buf)!=ENTER)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
		}
		else if(staval==NL829_HWINFO_ERR)
		{
			if(cls_show_msg("δ��ȡ���̻���,��ȷ��.��ȷ��[ENTER];����[����]")!=ENTER)
			{
				cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
				goto ERR;
			}
		}
		else
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}

	//RTC���������ȡ(�����������ֵش�rtc1������ֲ����)
	if(g_NL829type==DEVICE_NL829LCD)
	{
		if((staval=NL829LCD_GetRTC(old_time))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		test_rtc = TRUE;

		//���ò�����ʱ��(BCD��,�����ֵ��Χ����)
		memcpy(new_time, "\x19\x81\x11\x03\x07\x17\x37\x03", 8);
		if((staval=NL829LCD_SetRTC(new_time, 8))!=NL829_ARGERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		//���ò�����ʱ��(��BCD��)
		memcpy(new_time, "\x20\x0a\x11\x03\x07\x17\x37\x03", 8);
		if((staval=NL829LCD_SetRTC(new_time, 8))!=NL829_ARGERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		//���ò�����ʱ��(����������)
		memcpy(new_time, "\x20\x00", 2);
		if((staval=NL829LCD_SetRTC(new_time, 2))!=NL829_ARGERR)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}

		//���ú���ʱ��(BCD��)//֧�ֵ����:2000-2099
		memcpy(new_time, "\x20\x30\x11\x03\x07\x17\x37\x03", 8);
		if((staval=NL829LCD_SetRTC(new_time, 8))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		memset(new_time, 0, sizeof(new_time));
		if((staval=NL829LCD_GetRTC(new_time))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		else //У������
		{
			if(memcmp(new_time, "\x20\x30\x11\x03", 4)||memcmp(new_time+5, "\x17\x37", 2))//������ʱ��Ӧ��У���,�������벻����
			{
				cls_show_msg("line %d:����ʧ��(%.2x%.2x-%.2x-%.2x\n%.2x:%.2x)", __LINE__, new_time[0], new_time[1], new_time[2], new_time[3], new_time[5], new_time[6]);
				goto ERR;
			}
		}

		if((staval=NL829LCD_SetRTC(old_time, 8))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}

	cls_show_msg("�ǿ�������������ͨ��");
	return;
ERR:
	NL829_LEDCtl(0x0f);
	NL829_LEDCtlT(0x0f);
	if(g_NL829type==DEVICE_NL829LCD)
	{
		NL829_LCDCtl(0x06, 0x00000000, 0x00000000);
		if(test_rtc)
			NL829LCD_SetRTC(old_time, 8);
	}
	return;
}

static int test_RF_M1_KeyMode(int akeymode,int bkeymode)
{
	/*private & local definition*/
	int ATQlen = 0, SNRlen = 0, SAKlen = 0;
	uchar buf[NL829_MAXLEN_DATA] = {0}, ATQ[NL829_LEN_ATQ] = {0}, SNR[NL829_LEN_SNR] = {0}, SAK[NL829_LEN_SAK] = {0};
	ushort staval = NL829_FAIL;
	time_t oldtime = 0, diff = 0;
	uint inc = 0x01;
	
	/*process body*/
	//cls_show_msg1(2, "M1�������...");//����ʾ��ˢ�����Բ˵�
	//����ǰ��
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	//ԤװA,B Key��һ��,����ֻʹ��key-A, key-Bֻ�治װ��//�����Ԥװ������"	key-A"��"������1"֮���Ƶ���ǰ��Ϊǰ��,������Զ࿨��洢�᷵��30 e8(�����޷����������,RC531ʹȻ)
	if((staval=NL829_M1_storekey(akeymode, 0x00, NL829_AUTHKEY))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_storekey(bkeymode, 0x00, NL829_AUTHKEY))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//���ǵ��������̵�������,������"δload��ֱ����֤����д"������(�ŵ�key-B�����в���),��Ϊ�޷���ȡ�Ƿ�load��,Ԥ�ڽ������Ԥ��
	if((staval=NL829_M1_loadkey(akeymode, 0x00))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//�쳣(�ɵ������Ե��쳣������ڴ˲���,��Ҫǰ�������Ĳ��Կ��Էŵ�������������չ)
	if((staval=NL829_M1_active(0xff, 1, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;		
	}
	if((staval=NL829_M1_storekey(akeymode, 0x10, NL829_AUTHKEY))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_loadkey(akeymode, 0x10))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_auth(0xff, SNR, 0, 0))!=NL829_ERR_FRAME_STX/*NL829_ARGERR*/)//û����Ѱ�������������SNR��Ϊ0�����´����ָ��Ȳ��ԣ�ָ������ݳ��Ȳ��Ե�����²����أ�����Ӧ�ó�����ղ�����ͷ  modify by zhengjd
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_auth_ex(0xff, SNR, 0, 0, NL829_AUTHKEY))!=NL829_ERR_FRAME_STX/*NL829_ARGERR*/)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	cls_show_msg("��ȷ��NL829��Ӧ����M-1��,�����������...��ȴ�%ds", MAXWAITTIME);
	oldtime = time(NULL);
	while((diff=time(NULL)-oldtime)<MAXWAITTIME)
	{
		cls_printf("��ȷ��NL829��Ӧ����M-1��,��ȴ�%ds", MAXWAITTIME-diff);
		if((staval=NL829_M1_active(NL829_M1_WUPA, 0/*1*/, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_TIMEOUT)//0x0000��ʾѰ��1�ξ���������
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}
	cls_show_msg("��ȷ��NL829��Ӧ����2��M-1��,�����������...");
	if((staval=NL829_M1_active(NL829_M1_WUPA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//ע��:M1��CPU��ͬ,CPU����ʱ��һЩ��ȫҪ��ߵĳ�����Ҫ�µ�����Ѱ��,��M1�޴�Ҫ��.�����µĲ��Բ��Ծ���rf5��ͬ,�⽫���򻯲��Դ���(����ʱ�������¿�ʼ).	
	//����ʵ����֤���ֲ��Կ�����Ҫ����������Ѱ����!��Ҳ��rf5��������ʵ��
	//�����������̵Ĳ���
	//key-A
	//������1
	cls_show_msg("��ȷ��NL829��Ӧ����1��M-1��,�����������...");
	//Ѱ��(���� "Ѱ��-����ͻ-ѡ��")
	if((staval=NL829_M1_active(NL829_M1_REQA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��֤
	if((staval=NL829_M1_auth(akeymode, SNR, SNRlen, 0x01))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//���д��������
	if((staval=NL829_M1_BLKWrite(0x02, NL829_BLK02DATA_FF))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKRead(0x02, buf))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else
	{
		if(memcmp(NL829_BLK02DATA_FF, buf, NL829_LEN_BLKDATA))
		{
			cls_show_msg("line %d:���дʧ��", __LINE__);
			goto ERR;
		}
	}
	//����������֮ǰ,���ϵ����ݱ����ȷ���Ǯ����ʽ,���������ʧ��
	if((staval=NL829_M1_BLKInc(0x02, &inc))==NL829_SUCC)//!=MI_INCRERR)//ʵ�ʷ���
	{
		cls_show_msg("line %d:����������Ӧʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��Ҫ�����µ�Ѱ��һ��
	if((staval=NL829_AntennaCtl(10))!=NL829_SUCC)//10ms���ٴ�����
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//������2
	//Ѱ��(���� "Ѱ��-����ͻ-ѡ��")
	if((staval=NL829_M1_active(NL829_M1_REQA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��֤
	if((staval=NL829_M1_auth(akeymode, SNR, SNRlen, 0x01))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}	
	if((staval=NL829_M1_BLKDec(0x02, &inc))==NL829_SUCC)//!=MI_INCRERR)//ʵ�ʷ���
	{
		cls_show_msg("line %d:����������Ӧʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��Ҫ�����µ�Ѱ��һ��
	if((staval=NL829_AntennaCtl(10))!=NL829_SUCC)//10ms���ٴ�����
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//������3
	//Ѱ��(���� "Ѱ��-����ͻ-ѡ��")
	if((staval=NL829_M1_active(NL829_M1_REQA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��֤
	if((staval=NL829_M1_auth(akeymode, SNR, SNRlen, 0x01))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}	
	if((staval=NL829_M1_BLKWrite(0x02, NL829_BLK02DATA_ORI))!=NL829_SUCC)	
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKInc(0x02, &inc))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����������ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKInc(0x02, &inc))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����������ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKDec(0x02, &inc))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����������ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}		
	//�����������õ�����ʵ��Ч��Ϊ+1
	if((staval=NL829_M1_BLKRead(0x02, buf))!=NL829_SUCC)
	{
		cls_show_msg("line %d:���ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else
	{
		if(memcmp(BLK02DATA_INC1, buf, LEN_BLKDATA))
		{
			cls_show_msg("line %d:���дʧ��", __LINE__);
			goto ERR;
		}			
	}
	//halt
	if((staval=NL829_M1_halt())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//key-B,ʹ���ⲿ��֤(ʹ���ڲ���֤������load,Ԥ��ʧ��)
	//������4
	//cls_show_msg("��ȷ��NL829��Ӧ����һ��M-1��,�����������...");
	//Ѱ��(���� "Ѱ��-����ͻ-ѡ��")
	if((staval=NL829_M1_active(NL829_M1_REQA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_halt())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_active(NL829_M1_WUPA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)//����ģʽ������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��֤
#if 0	//���ڲ���ʹ�õ�A,B key��ֵ��һ����,��load�������Ƿ��ڹ��õ���Կ�����,����Ȼδload��B,���˴��Ի�ɹ�.����,auth��mode������������ʹ�ò�ͬ���㷨,��load��mode�����ò�ͬ!
	//ʹ���ڲ���֤������load,Ԥ��ʧ��,��ʹ���ⲿ��֤
	if((staval=NL829_M1_auth(NL829_AUTHKEY_TYPE_B, SNR, SNRlen, 0x01))!=NL829_M1_NOTAUTHERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
#endif
	if((staval=NL829_M1_auth_ex(bkeymode, SNR, SNRlen, 0x01, NL829_AUTHKEY))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//���д��������(��key-A�в��Թ������ݲ����ز�����)
	if((staval=NL829_M1_BLKWrite(0x02, NL829_BLK02DATA_ORI))!=NL829_SUCC)	
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKInc(0x02, &inc))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����������ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKInc(0x02, &inc))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����������ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_M1_BLKDec(0x02, &inc))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����������ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}		
	//�����������õ�����ʵ��Ч��Ϊ+1
	if((staval=NL829_M1_BLKRead(0x02, buf))!=NL829_SUCC)
	{
		cls_show_msg("line %d:���ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else
	{
		if(memcmp(BLK02DATA_INC1, buf, LEN_BLKDATA))
		{
			cls_show_msg("line %d:���дʧ��", __LINE__);
			goto ERR;
		}			
	}
	//halt
	if((staval=NL829_M1_halt())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//������5
	cls_show_msg("����NL829��Ӧ������1��4k M1��,���������...");
	//Ѱ��(���� "Ѱ��-����ͻ-ѡ��")
	if((staval=NL829_M1_active(NL829_M1_WUPA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��֤/����,�˴�ʹ���ⲿ��֤(�ڲ���֤����,����Կ��Ĵ洢��������Ӱ��)
	//���ڲ�֪��4k����B��Կ�Ƕ���,ʹ��A��Կ(������A��ԿΪȫ0xff)
	if((staval=NL829_M1_auth_ex(akeymode, SNR/*UID*/, SNRlen, 0xfe, NL829_AUTHKEY))!=NL829_SUCC)//��֤�Ŀ��ڵ�4K�ڵ�����
	{
		cls_show_msg("line %d:�ⲿ��֤ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//��д,���ǵ����Կ�����,����������
	if((staval=NL829_M1_BLKRead(0xfd, buf))!=NL829_SUCC)//0xfe���������Ŀ�Ӧ���ܶ�
	{
		cls_show_msg("line %d:���ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//���Ա�,��ȫ�رճ�
	if((staval=NL829_AntennaCtl(0xffff))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//cls_show_msg("M1�������ͨ��");
	//return;
ERR:
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	return staval;
}

static void test_RF_M1(void)
{
	/*private & local definition*/
	ushort staval = NL829_FAIL;
    
	/*process body*/
    cls_show_msg1(2, "M1�������...");//����ʾ��ˢ�����Բ˵�

    if((staval=test_RF_M1_KeyMode(NL829_AUTHKEY_TYPE_A,NL829_AUTHKEY_TYPE_B))!=NL829_SUCC)
    {
        cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
        goto ERR;
    }
    if((staval=test_RF_M1_KeyMode(NL829_AUTHKEY_TYPE_A1,NL829_AUTHKEY_TYPE_B1))!=NL829_SUCC)
    {
        cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
        goto ERR;
    }

    cls_show_msg("M1�������ͨ��");
        return;

ERR:
    NL829_AntennaCtl(0xffff);//��ȫ�رճ�
    return;
}

void NL829_ICSAM_powerdown_all(void)
{
	/*private & local definition*/

	/*process body*/
	NL829_ICSAM_powerdown(NL829_CARDNO_IC1);
	NL829_ICSAM_powerdown(NL829_CARDNO_SAM1);
	NL829_ICSAM_powerdown(NL829_CARDNO_SAM2);
	NL829_ICSAM_powerdown(NL829_CARDNO_SAM3);
	NL829_ICSAM_powerdown(NL829_CARDNO_SAM4);	
}

//��SAM1,SAM2��һ����
static void test_ICSAM(void)
{
	/*private & local definition*/
	int len = 0;
	uchar buf[NL829_MAXLEN_DATA] = {0}, sta = 0x00;	
	ushort staval = NL829_FAIL, ret = 0;
	
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	//���������Ŀ��Ĳ�ͬ,�еĿ���֧�����������,������޹�,��ʹ�õĲ��Կ��й�
	//char sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 0};	
	
	/*process body*/
	if(g_NL829type==DEVICE_NL829LCD)
	{
		if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x00)
		{
			cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
			goto ERR;
		}
		cls_show_msg("�����IC1,���������...");
		if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x01)//Ӧֻ�ܼ�⵽����IC1
		{
			cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
			goto ERR;
		}		
	}
	
	cls_show_msg1(2, "IC/SAM�������...");//����ʾ��ˢ�����Բ˵�
	//����ǰ��
	NL829_ICSAM_powerdown_all();
	
	//�쳣
	if((staval=NL829_ICSAM_poweron(0xf0, buf, buf+1, &len))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_APDUrw(0xf0, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_APDUrw(NL829_CARDNO_SAM1, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SAM_NOTPOWERON
	||(ret=NL829_APDUrw(NL829_CARDNO_SAM2, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SAM_NOTPOWERON)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;
	}
	if((staval=NL829_ICSAM_powerdown(0xf0))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	
	//����
	//SAM1
	//�ϵ�
	if((staval=NL829_ICSAM_poweron(NL829_CARDNO_SAM1, buf, buf+1, &len))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(buf[0]==NL829_CARD_T0||buf[0]==NL829_CARD_T1)
		cls_show_msg1(2, "SAM1(T%c��)�ϵ�ɹ�", (buf[0]==NL829_CARD_T0)?'0':'1');
	else //δ֪������
	{
		cls_show_msg("line %d:����ʧ��(%02x)", __LINE__, buf[0]);
		goto ERR;
	}
	if(g_NL829type==DEVICE_NL829LCD)
	{	
	//ȡ״̬
	if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x11)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
		goto ERR;
	}
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_SAM1, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;
	}
	//�µ�
	if((staval=NL829_ICSAM_powerdown(NL829_CARDNO_SAM1))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//SAM2
	//�ϵ�
	if((staval=NL829_ICSAM_poweron(NL829_CARDNO_SAM2, buf, buf+1, &len))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(buf[0]==NL829_CARD_T0||buf[0]==NL829_CARD_T1)
		cls_show_msg1(2, "SAM2(T%c��)�ϵ�ɹ�", (buf[0]==NL829_CARD_T0)?'0':'1');
	else //δ֪������
	{
		cls_show_msg("line %d:����ʧ��(%02x)", __LINE__, buf[0]);
		goto ERR;
	}
	if(g_NL829type==DEVICE_NL829LCD)
	{	
	//ȡ״̬
	if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x21)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
		goto ERR;
	}
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_SAM2, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;	
	}
	//�µ�
	if((staval=NL829_ICSAM_powerdown(NL829_CARDNO_SAM2))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(g_NL829type==DEVICE_NL829LCD)
	{
	//SAM3
	//�ϵ�
	if((staval=NL829_ICSAM_poweron(NL829_CARDNO_SAM3, buf, buf+1, &len))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(buf[0]==NL829_CARD_T0||buf[0]==NL829_CARD_T1)
		cls_show_msg1(2, "SAM3(T%c��)�ϵ�ɹ�", (buf[0]==NL829_CARD_T0)?'0':'1');
	else //δ֪������
	{
		cls_show_msg("line %d:����ʧ��(%02x)", __LINE__, buf[0]);
		goto ERR;
	}
	//ȡ״̬
	if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x41)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
		goto ERR;
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_SAM3, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;	
	}
	//�µ�
	if((staval=NL829_ICSAM_powerdown(NL829_CARDNO_SAM3))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//SAM4
	//�ϵ�
	if((staval=NL829_ICSAM_poweron(NL829_CARDNO_SAM4, buf, buf+1, &len))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(buf[0]==NL829_CARD_T0||buf[0]==NL829_CARD_T1)
		cls_show_msg1(2, "SAM4(T%c��)�ϵ�ɹ�", (buf[0]==NL829_CARD_T0)?'0':'1');
	else //δ֪������
	{
		cls_show_msg("line %d:����ʧ��(%02x)", __LINE__, buf[0]);
		goto ERR;
	}
	//ȡ״̬
	if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x81)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
		goto ERR;
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_SAM4, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;	
	}
	//�µ�
	if((staval=NL829_ICSAM_powerdown(NL829_CARDNO_SAM4))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//IC1
	//�ϵ�
	if((staval=NL829_ICSAM_poweron(NL829_CARDNO_IC1, buf, buf+1, &len))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(buf[0]==NL829_CARD_T0||buf[0]==NL829_CARD_T1)
		cls_show_msg1(2, "IC1(T%c��)�ϵ�ɹ�", (buf[0]==NL829_CARD_T0)?'0':'1');
	else //δ֪������
	{
		cls_show_msg("line %d:����ʧ��(%02x)", __LINE__, buf[0]);
		goto ERR;
	}
	//ȡ״̬
	if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x03)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
		goto ERR;
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_IC1, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;	
	}
	//�µ�
	if((staval=NL829_ICSAM_powerdown(NL829_CARDNO_IC1))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	}

	//׷�Ӽ��ſ����ϵ�ʱ,getsta�Ĳ���
#if 0	//�ݲ�֧��
	if(g_NL829type==DEVICE_NL829LCD)
	{
		if((staval=NL829_ICSAM_poweron(NL829_CARDNO_SAM1, buf, buf+1, &len))!=NL829_SUCC||(ret=NL829_ICSAM_poweron(NL829_CARDNO_SAM2, buf, buf+1, &len))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, ret);
			goto ERR;
		}
		if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0x31)
		{
			cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
			goto ERR;
		}
		if((staval=NL829_ICSAM_poweron(NL829_CARDNO_SAM3, buf, buf+1, &len))!=NL829_SUCC||(ret=NL829_ICSAM_poweron(NL829_CARDNO_SAM4, buf, buf+1, &len))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, ret);
			goto ERR;
		}
		if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0xf1)
		{
			cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
			goto ERR;
		}		
		if((staval=NL829_ICSAM_poweron(NL829_CARDNO_IC1, buf, buf+1, &len))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		if((staval=NL829_ICSAM_GetSta(&sta))!=NL829_SUCC||NL829_ICSAM_GETSTA_MASK(sta)!=0xf3)
		{
			cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, sta);
			goto ERR;
		}
		NL829_ICSAM_powerdown_all();
	}
#endif

	cls_show_msg("IC/SAM�������ͨ��");
	return;
ERR://������Ҳ��һ�ֲ��Ժ���
	NL829_ICSAM_powerdown_all();
	return;
}

static int test_RF_CPU_KEY_INPUT(void)
{
	int len = 0, UIDlen = 0, ATRlen = 0;
	uchar buf[NL829_MAXLEN_DATA] = {0}, UID[NL829_LEN_UID] = {0}, ATR[256] = {0}, keybuf[NL829_LEN_KEY_INPUT]={0},tmp[21]={0};
	ushort staval = NL829_FAIL, staval1=NL829_FAIL,ret = 0;
	time_t oldtime = 0, diff = 0;

	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������

	cls_show_msg1(2, "��Ƶ��Ѱ���Ͱ���������ѯ����...");
	cls_show_msg("��ȷ��NL829��Ӧ����CPU��,�����������...��ȴ�%ds", MAXWAITTIME);
	oldtime = time(NULL);
	while((diff=time(NULL)-oldtime)<MAXWAITTIME)
	{
		cls_printf("��ȷ��NL829��Ӧ����CPU��,��ȴ�%ds", MAXWAITTIME-diff);
		if((staval=NL829CpuActiveKeyInput(0, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_TIMEOUT)//0x0000��ʾѰ��1�ξ���������
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}
	//case1  ����Ѱ���࿨
	cls_show_msg("��ȷ��NL829��Ӧ����2��A��,�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	
	cls_show_msg("��ȷ��NL829��Ӧ����2��B��,�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	cls_show_msg("��ȷ��NL829��Ӧ����A,B����1��,�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//case2  ����Ѱ����APDU
	//A��������Ѱ����APDU
	cls_show_msg("���ڸ�Ӧ������һ�ű�׼A�������������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||((ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC&&(ret=NL829_GETSTAVAL(buf+len-2))!=0x6d00))
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;
	}
    //B�������ļ��APDU
	cls_show_msg("��ȷ��NL829��Ӧ����1��B��,�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC||buf[0]!=NL829_TYPE_B)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, buf[0]);
		goto ERR;
	}
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||((ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC&&(ret=NL829_GETSTAVAL(buf+len-2))!=0x6d00))
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;
	}
	//case 3  ���Լ����������
	cls_show_msg("�뽫��Ӧ������Ƶ���ƿ������������...");
	//�����������ǰ�ã���������ַ���Сֵ����Ϊ0�����ֵ����Ϊ21
	if((staval=PP60_cmd_N(0x00))!=NL829_PP60_SUCC||(staval1=PP60_cmd_L(0x18))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//case3.1  �����0���ַ�
	cls_show_msg("���������������[ENTER]�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC||strlen((char *)keybuf)!=0)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2,"#3.1����ͨ��");
	//case3.2  ����6λ�ַ�
	cls_show_msg("���������������%s��[ENTER]�����������...","123456");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC||memcmp(keybuf, "123456", strlen("123456")))
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2,"#3.2����ͨ��");
	//case3.3  ����21���ַ�
	memset(tmp,0x31,sizeof(tmp));
	cls_show_msg("���������������21��1��[ENTER]�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC||memcmp(keybuf, tmp, sizeof(tmp)))
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	} 
	cls_show_msg1(2,"#3.3����ͨ��");
	//case3.4  ������ַ�����21��
	cls_show_msg("���������������24��1��[ENTER]�����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC||memcmp(keybuf, tmp, sizeof(tmp)))
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	} 
	cls_show_msg1(2,"#3.4����ͨ��");
         
	//case4  ��ʱ�������
	//case4.1  �������룬������û���������ֵ�ʱ�򣬷���Ѱ����ʱ
	cls_show_msg("�뽫��Ӧ���е���Ƶ�����ߣ����������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_TIMEOUT)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg("�뽫��Ӧ���е���Ƶ�����ߣ����������������%s��ȡ���������������ֺ����ĵȴ������������...","123456");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_TIMEOUT)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2,"#4.1����ͨ��");
	//case4.2  �������룬���������������ֵ�ʱ�򣬷������볬ʱ
	cls_show_msg("�뽫��Ӧ���е���Ƶ�����ߣ����������������%s�����ĵȴ������������...","123456");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2,"#4.2����ͨ��");
	//case4.3  ���������ʱ�����˸������������֣���Ӧ��������Ƶ������Ƶ�����apdu����Ӧ������
	cls_show_msg("�����������������%s��ȡ����������������,Ȼ���ڸ�Ӧ������һ�ű�׼A�������������...","123456");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_SUCC||buf[0]!=NL829_TYPE_A)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, buf[0]);
		goto ERR;
	}
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;
	}
	cls_show_msg1(2,"#4.3����ͨ��");
	//case4.4  �������밴��ȡ���˳�
	cls_show_msg1(2,"�����ϰ�ȡ���˳������������...");
	if((staval=NL829CpuActiveKeyInput(5, buf, UID, &UIDlen, ATR, &ATRlen,keybuf))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	cls_show_msg1(2, "��Ƶ��Ѱ���Ͱ���������ѯ����ͨ��...");
    return NL829_SUCC;
 ERR:
	PP60_cmd_N(0x04);
	PP60_cmd_L(0x10);
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	return NL829_FAIL;
}

static void test_RF_CPU(void)
{
	/*private & local definition*/
	int len = 0, UIDlen = 0, ATRlen = 0, pnDatalen=0, ATQlen = 0, SNRlen = 0, SAKlen = 0;
	uchar buf[NL829_MAXLEN_DATA] = {0}, UID[NL829_LEN_UID] = {0}, ATR[256] = {0}, psDatabuf[256]={0}, ATQ[NL829_LEN_ATQ] = {0}, SNR[NL829_LEN_SNR] = {0}, SAK[NL829_LEN_SAK] = {0};
	ushort staval = NL829_FAIL, ret = 0;
	time_t oldtime = 0, diff = 0;
	
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	//���������Ŀ��Ĳ�ͬ,�еĿ���֧�����������,������޹�,��ʹ�õĲ��Կ��й�
	//char sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 0};	
	
	/*process body*/
	cls_show_msg1(2, "�ǽ�CPU���������...");//����ʾ��ˢ�����Բ˵�
	//����ǰ��
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	
	//�쳣(�ɵ������Ե��쳣������ڴ˲���,��Ҫǰ�������Ĳ��Կ��Էŵ�������������չ)
	if((staval=NL829_APDUrw(0xf0, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_NOTACTIVE)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//case1:����Ѱ������ͻѡ��ǰ�ĵ���,Ӧ�÷���ʧ��
	if((staval=NL829_RfidTypeARats(0, &pnDatalen, psDatabuf))!=NL829_M1_NOTAGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
    
	cls_show_msg("��ȷ��NL829��Ӧ����CPU��,�����������...��ȴ�%ds", MAXWAITTIME);
	oldtime = time(NULL);
	while((diff=time(NULL)-oldtime)<MAXWAITTIME)
	{
		cls_printf("��ȷ��NL829��Ӧ����CPU��,��ȴ�%ds", MAXWAITTIME-diff);
		if((staval=NL829_CPU_active(0/*1*/, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_TIMEOUT)//0x0000��ʾѰ��1�ξ���������
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
	}

	cls_show_msg("��ȷ��NL829��Ӧ����2��A��,�����������...");
	if((staval=NL829_CPU_active(5, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	
	cls_show_msg("��ȷ��NL829��Ӧ����2��B��,�����������...");
	if((staval=NL829_CPU_active(5, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	cls_show_msg("��ȷ��NL829��Ӧ����A,B����1��,�����������...");
	if((staval=NL829_CPU_active(5, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_MULCARD)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	
	//�����������̵Ĳ���
	//type-A
	cls_show_msg("��ȷ��NL829��Ӧ����1��A��,�����������...");
	//Ѱ��������
	if((staval=NL829_CPU_active(5, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_SUCC||buf[0]!=NL829_TYPE_A)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, buf[0]);
		goto ERR;
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||(ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		return;
	}
	//�Ͽ�����
	if((staval=NL829_CPU_discnnt(5))!=NL829_TIMEOUT)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	cls_show_msg("�뽫NL829��Ӧ����A���ƿ�,�����������...");
	if((staval=NL829_CPU_discnnt(5))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	//�رճ�
	if((staval=NL829_AntennaCtl(10))!=NL829_SUCC)//10ms���ٴ�����
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	//type-B
	cls_show_msg("��ȷ��NL829��Ӧ����1��B��,�����������...");
	//Ѱ��������
	if((staval=NL829_CPU_active(5, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_SUCC||buf[0]!=NL829_TYPE_B)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, buf[0]);
		goto ERR;
	}
	//APDU��д
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||((ret=NL829_GETSTAVAL(buf+len-2))!=NL829_CPU_SUCC&&(ret=NL829_GETSTAVAL(buf+len-2))!=0x6d00))
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		return;
	}
	//�Ͽ�����
	if((staval=NL829_CPU_discnnt(5))!=NL829_TIMEOUT)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	cls_show_msg("�뽫NL829��Ӧ����B���ƿ�,�����������...");
	if((staval=NL829_CPU_discnnt(5))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	//�رճ�
	if((staval=NL829_AntennaCtl(10))!=NL829_SUCC)//10ms���ٴ�����
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	//���Է�װ��Ratsָ���ܷ���M1��Ѱ������ͻ��ѡ���Ļ����ϼ���A�����ж�д����add by zhengjd 20141029
	//����M1������
	cls_show_msg("���ڸ�Ӧ������һ�ű�׼A��,���������...");
	if((staval=NL829_M1_active(NL829_M1_REQA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//����pssakbuf ֵ�ж���A������M������A��ִ�л�ȡA����ATS
	if((SAK[0]&0x20)!=0x20)//bit5Ϊ1ʱ���ж�ΪA��,Ȼ��ִ��A������ apdu
	{
		cls_show_msg("line %d:����ʧ��(0x%02x)", __LINE__, SAK[0]);
		goto ERR;
	}

	//case2:��������
	if((ret=NL829_RfidTypeARats(0, &pnDatalen, psDatabuf))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if((staval=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), buf, &len))!=NL829_SUCC||((ret=NL829_GETSTAVAL(buf+len-2))!=0x9000&&(ret=NL829_GETSTAVAL(buf+len-2))!=0x6d00))
	{
		cls_show_msg("line %d:����ʧ��(%04x, %04x)", __LINE__, staval, ret);
		goto ERR;
	}
	if(g_NL829type==DEVICE_NL829MR_PP60RF)//YH100 û�м���,��֧�ָù���
	{
		if((ret=test_RF_CPU_KEY_INPUT())!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__,ret);
			return;
		}
	}

	cls_show_msg("�ǽ�CPU���������ͨ��");
	//return;
ERR:
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	return;
}

//������ķǷ����,829Ӧ��Ӧ��
static void test_abnormity(void)
{
	/*private & local definition*/
	int datalen = 0, LRCOffset = 0, appendlen = 0, subcase = 0, ATQlen = 0, SNRlen = 0, SAKlen = 0;
	ushort ret1 = NL829_FAIL;
	uchar frame_cmd[NL829_MAXLEN_FRAME*2] = {0}, *data = NULL;
	uchar resp[NL829_MAXLEN_DATA] = {0}, status[2] = {0}, append[NL829_MAXLEN_DATA] = {0};//frame_resp�е�data��
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	uchar ATQ[NL829_LEN_ATQ] = {0}, SNR[NL829_LEN_SNR] = {0}, SAK[NL829_LEN_SAK] = {0};
	
	/*process body*/
	cls_show_msg1(2, "�쳣�������...");//����ʾ��ˢ�����Բ˵�
	//����ǰ��
	serial_clrportbuf_all(g_devicecom);
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	NL829_LEDCtl(0x0f);	
	
	// 1.�����֡��ʽ,�豸���Բ�Ӧ��
	//���
	memset(frame_cmd, 0xaa, sizeof(frame_cmd));
	//����
	if((ret1=NL829_send_frame(frame_cmd, sizeof(frame_cmd)))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	//�հ�
	if((ret1=NL829_recv_frame(resp, &datalen, 40/(strlen(g_devicecombps)-2)))!=NL829_ERR_FRAME_STX)//40�ǰ�1K��������BPS300������������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ	

	// 2.Data_len>512,�豸���Բ�Ӧ��
	//���
	datalen = 768;
	frame_cmd[NL829_OFFSET_STX] = STX;
	frame_cmd[NL829_OFFSET_DATALEN] = datalen/256;
	frame_cmd[NL829_OFFSET_DATALEN+1] = datalen%256;
	data = frame_cmd+NL829_OFFSET_DATA;
	memcpy(data, NL829_SETHWINFO, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, NL829_SETHWINFO_CHECK);
	data[NL829_OFFSET_ARGS+strlen(NL829_SETHWINFO_CHECK)] = 0x00;//SN
	LRCOffset = NL829_LEN_FRAMEHEAD+datalen;
	frame_cmd[LRCOffset] = MakeLRC(data, datalen);
	frame_cmd[LRCOffset+NL829_LEN_LRC] = ETX;
	//����
	if((ret1=NL829_send_frame(frame_cmd, LRCOffset+NL829_LEN_LRC+1))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	//�հ�
	if((ret1=NL829_recv_frame(resp, &datalen, 40/(strlen(g_devicecombps)-2)))!=NL829_ERR_FRAME_STX)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ	

	// 3.��ʶ�������,�豸���Բ�Ӧ��
	//���
	datalen = NL829_pack_data((uchar *)"\xff\xff", strlen("\xff\xff"), frame_cmd);
	//����
	if((ret1=NL829_send_frame(frame_cmd, datalen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	//�հ�
	if((ret1=NL829_recv_frame(resp, &datalen, 40/(strlen(g_devicecombps)-2)))!=NL829_ERR_FRAME_STX)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ	

	// 4.ĳЩ����(��Ҫ����ָӲ����Ϣ�������ȡ����)���쳣��֮ǰû�в��Ե���,�ڴ˴�׷�Ӳ���,�豸����Ԥ�ڴ���(��,��������)
	//�������SN���������ȡΪ��,�Ի���ʵ����һ����,������
	if(g_NL829type==DEVICE_NL829LCD)
	{
		if((ret1=NL829_send_frame((uchar *)"\x02\x00\x08\x31\x20\x31\x32\x33\x34\x35\x20\x03", 12))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
			goto ERR;
		}
		if((ret1=NL829_recv_frame(resp, &datalen, 40/(strlen(g_devicecombps)-2)))!=NL829_ERR_FRAME_STX)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
			goto ERR;
		}
	}
	else
	{
	data = resp;//����ָ��,ָ���ַ���Ը���ʵ����Ҫ����,����Ķ���С�����,��!
	memcpy(data, NL829_SETHWINFO, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, "1234567");
	data[NL829_OFFSET_ARGS+strlen("1234567")] = NL829_FLAG_SN;
	strcpy((char *)data+NL829_OFFSET_ARGS+strlen("1234567")+1, NL829_SETHWINFO_CHECK);
	datalen = NL829_OFFSET_ARGS+strlen("1234567")+1+strlen(NL829_SETHWINFO_CHECK);
	if((ret1=NL829_frame_factory(data, datalen, status, append, &appendlen, 0))!=NL829_SUCC||appendlen!=NL829_SETHWINFO_APPLEN_RCV)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %d)", __LINE__, ret1, appendlen);
		goto ERR;
	}
	if((ret1=NL829_GETSTAVAL(status))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}

	data = resp;
	memcpy(data, NL829_GETHWINFO, NL829_LEN_CMD);
	strcpy((char *)data+NL829_OFFSET_ARGS, "1234567");
	data[NL829_OFFSET_ARGS+strlen("1234567")] = NL829_FLAG_SN;
	datalen = NL829_OFFSET_ARGS+strlen("1234567")+1;
	if((ret1=NL829_frame_factory(data, NL829_GETHWINFO_DATALEN_SND, status, append, &appendlen, 0))!=NL829_SUCC||appendlen!=0)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %d)", __LINE__, ret1, appendlen);
		goto ERR;
	}
	if((ret1=NL829_GETSTAVAL(status))!=NL829_ARGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ	

	// 5.���������ͷǰ,�в�,β���и�������,��beep����Ϊ��,20161028 �����YH100ֻ������,�޷�����,���޸��Է���������Ϊ����
	//�м����
	if((ret1=NL829_send_frame((uchar *)"\x02\x00\x04\x31\x14\x10\x10\x25\x03", 9))!=NL829_SUCC)
	//if((ret1=NL829_send_frame((uchar *)"\x02\x00\x06\x31\x13\x07\x07\xd0\x01\xf3\x03", 11))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	if((((ret1=NL829_recv_frame(resp, &datalen, 40/(strlen(g_devicecombps)-2)))!=NL829_ERR_FRAME_STX)&&((ret1!=NL829_SUCC)||((appendlen=NL829_unpack_data(resp, datalen, status, append))!=NL829_BEEP_APPLEN_RCV)||(NL829_GETSTAVAL(status)!=NL829_ARGERR)))
	||cls_show_msg("NL829��������,��[ENTER];����[����]")==ENTER)//����beep��
	{
		cls_show_msg("line %d:����ʧ��(%04x,%d,%04x)", __LINE__, ret1, appendlen, NL829_GETSTAVAL(status));
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ	
	//ͷ������(��02��ͷ)
	if((ret1=NL829_send_frame((uchar *)"\x03\x02\x00\x03\x31\x14\x10\x35\x03", 9))!=NL829_SUCC)
	//if((ret1=NL829_send_frame((uchar *)"\x03\x02\x00\x05\x31\x13\x07\xd0\x01\xf4\x03", 11))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	if((ret1=NL829_recv_frame(resp, &datalen, 0/*0x07d0*0x01/1000*/))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	if((appendlen=NL829_unpack_data(resp, datalen, status, append))!=NL829_LEDCTL_APPLEN_RCV/*NL829_BEEP_APPLEN_RCV*/||(ret1=NL829_GETSTAVAL(status))!=NL829_SUCC||cls_show_msg("NL829��������,��[ENTER];����[����]")!=ENTER)//����beep��
	{
		cls_show_msg("line %d:����ʧ��(%d,%04x)", __LINE__, appendlen, ret1);
		goto ERR;
	}
	NL829_LEDCtl(0x0f);	
	//β������
	if((ret1=NL829_send_frame((uchar *)"\x02\x00\x03\x31\x14\x10\x35\x03\x03", 9))!=NL829_SUCC)
	//if((ret1=NL829_send_frame((uchar *)"\x02\x00\x05\x31\x13\x07\xd0\x01\xf4\x03\x03", 11))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	if((ret1=NL829_recv_frame(resp, &datalen, 0/*0x07d0*0x01/1000*/))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	if((appendlen=NL829_unpack_data(resp, datalen, status, append))!=NL829_LEDCTL_APPLEN_RCV/*NL829_BEEP_APPLEN_RCV*/||(ret1=NL829_GETSTAVAL(status))!=NL829_SUCC||cls_show_msg("NL829��������,��[ENTER];����[����]")!=ENTER)//����beep��
	{
		cls_show_msg("line %d:����ʧ��(%d,%04x)", __LINE__, appendlen, ret1);
		goto ERR;
	}
	NL829_LEDCtl(0x0f);	
	// 6.����ָ��жϵ�����(���ܴ���"��ֹ״̬")
	if(g_NL829type==DEVICE_NL829MR_PP60RF)//YH100 �޷�����,�����Ը�ָ��
	{
		//NL829_beep
		datalen = NL829_pack_data((uchar *)"\x31\x13\xff\xff\xff", strlen("\x31\x13\xff\xff\xff"), frame_cmd);
		if((ret1=NL829_send_frame(frame_cmd, datalen))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
			goto ERR;
		}
		cls_show_msg("beep����,�������ֹͣ����,ͬʱ��LED��...");
		if((ret1=NL829_LEDCtl(0x1f))!=NL829_SUCC||cls_show_msg("NL829��ֹͣ����,ͬʱ��LED��,��[ENTER];����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
			goto ERR;
		}
	}

	if(g_NL829type==DEVICE_NL829MR_PP60RF||g_NL829type==DEVICE_YH100)//YH100 ��֧������,�ʽ��̸ĳ��� 0x8f�ĳ�0x1f 20161028
	{
	//NL829_CPU_active
	datalen = NL829_pack_data((uchar *)"\x32\x24\xff\xff", strlen("\x31\x13\xff\xff"), frame_cmd);
	if((ret1=NL829_send_frame(frame_cmd, datalen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("���������LED��...");
	if((ret1=NL829_LEDCtl(0x0f))!=NL829_SUCC||cls_show_msg("NL829����LED��,��[ENTER];����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("��ȷ��NL829��Ӧ����1��CPU��,�����������...");
	if((ret1=NL829_APDUrw(NL829_CARDNO_RFID, sendbuf, sizeof(sendbuf), resp, &datalen))!=NL829_NOTACTIVE)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("��ȷ��NL829��Ӧ����CPU��,�����������...");
	//NL829_M1_active
	cls_show_msg("��ȷ��NL829��Ӧ����1��M-1��,�����������...");
	if((ret1=NL829_M1_active(NL829_M1_WUPA, 5, ATQ, &ATQlen, SNR, &SNRlen, SAK, &SAKlen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("�뽫NL829��Ӧ����M-1���ƿ�,�����������...");//���˻��SNR
	datalen = NL829_pack_data((uchar *)"\x33\x21\x52\xff\xff", strlen("\x33\x21\x52\xff\xff"), frame_cmd);
	if((ret1=NL829_send_frame(frame_cmd, datalen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("���������LED��...");
	if((ret1=NL829_LEDCtl(0x1f))!=NL829_SUCC||cls_show_msg("NL829����LED��,��[ENTER];����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("�뽫ͬ1��M-1��������NL829��Ӧ��,�����������...");
	if((ret1=NL829_M1_auth_ex(NL829_AUTHKEY_TYPE_B, SNR, SNRlen, 0x01, NL829_AUTHKEY))!=NL829_M1_NOTAGERR)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}
	cls_show_msg("���������LED��...");
	if((ret1=NL829_LEDCtl(0x0f))!=NL829_SUCC||cls_show_msg("NL829����LED��,��[ENTER];����[����]")!=ENTER)//NL829_LEDCtl(0x0f);
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret1);
		goto ERR;
	}	
	//cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ
	}

	cls_show_msg("�쳣�������ͨ��");
ERR:
	//���Ժ���
	serial_clrportbuf_all(g_devicecom);
	NL829_AntennaCtl(0xffff);//��ȫ�رճ�
	NL829_LEDCtl(0x0f);
	return;
}

//NL829_RFID_xxx�������˵��:ͳһ���Ϊ��type��UID��������
//���ֻ����Ƶʹ��,֮����չΪ�����ڽӴ�ʽSAM����.UID�ڲ�����ƵʱΪUID,����SAMʱ��������SAM1����SAM2
static ushort NL829_RFID_seek(SMART_t type, uchar *UID)
{
	/*private & local definition*/
	int UIDlen = 0, ATRlen = 0, ATQlen = 0, SAKlen = 0;
	uchar buf[NL829_MAXLEN_DATA] = {0}, ATR[256] = {0}, ATQ[NL829_LEN_ATQ] = {0}, SAK[NL829_LEN_SAK] = {0};
	ushort  staval = NL829_FAIL;

	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
		if((staval=NL829_CPU_active(5, buf, UID, &UIDlen, ATR, &ATRlen))!=NL829_SUCC||buf[0]!=(type==CPU_A?NL829_TYPE_A:NL829_TYPE_B))
		{
			//cls_show_msg1(g_keeptime, "line %d:Ѱ��ʧ��(0x%04x, 0x%02x)", __LINE__, staval, buf[0]);
			return (staval!=NL829_SUCC)?staval:NL829_FAIL;			
		}
		return NL829_SUCC;
		break;
	case MIFARE_1:
		return NL829_M1_active(NL829_M1_WUPA, 5, ATQ, &ATQlen, UID, &UIDlen, SAK, &SAKlen);//ʹ���Ƽ�ֵ�����в���
		break;
	case SAM:
	case IC:	
		return NL829_ICSAM_poweron(UID[0], buf, ATR, &ATRlen);//�����buf��ʶ��T0 or T1
		break;
	default:
		return NL829_FAIL;
		break;
	}	
}

static ushort NL829_RFID_active(SMART_t type, uchar *UID)
{
	/*private & local definition*/

	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
		return NL829_SUCC;
		break;
	case MIFARE_1:
		//���ڲ�֪��4k����B��Կ�Ƕ���,ʹ��A��Կ(������A��ԿΪȫ0xff)
		return NL829_M1_auth_ex(NL829_AUTHKEY_TYPE_A, UID, 4, 0x01, NL829_AUTHKEY);//SNR���ܰ���0�ֽڣ����贫�ݹ̶�����4
		break;
	case SAM:
	case IC:	
		return NL829_SUCC;
		break;
	default:
		return NL829_FAIL;
		break;
	}	
}

static ushort NL829_RFID_iccrw(SMART_t type, uchar *UID)
{
	/*private & local definition*/
	int rcvlen = 0;
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	uchar Select_1pay[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
#else
	uchar Select_1pay[] = 
	{0x00, 0xA4, 0x04, 0x00, 0x0E,
	  '1',  'P',  'A',  'Y',  '.',  'S',  'Y',  'S',  '.',  'D',  'D',  'F',  '0',  '1',
	  0x00
	};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif
	uchar out[NL829_MAXLEN_DATA] = {0};
	ushort  staval = NL829_FAIL, ret = 0;

	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
		if((staval=NL829_APDUrw(NL829_CARDNO_RFID, Select_1pay, sizeof(Select_1pay), out, &rcvlen))!=NL829_SUCC||((ret=NL829_GETSTAVAL(out+rcvlen-2))!=NL829_CPU_SUCC&&(ret=SMART_GETSW(out+rcvlen-2))!=0x6d00))
		{
			cls_show_msg1(g_keeptime, "line %d:��дʧ��(%04x, 0x%04x)", __LINE__, staval, ret);
			return (staval!=NL829_SUCC)?staval:NL829_FAIL;
		}
		return NL829_SUCC;
		break;
	case MIFARE_1:
#if 0	//���ǵ����Կ�������ֻ��������,��һ�ֲ��Բ���
		if((ret=CMD_BLKWrite(0x02, BLK02DATA_FF))!=NL829_SUCC/*MI_OK*/)
		{
			cls_show_msg1(g_keeptime, "line %d:��дʧ��(ret=%d)", __LINE__, ret);
			return ret;
		}
#endif
		if((staval=NL829_M1_BLKRead(0x02, out))!=NL829_SUCC/*MI_OK*/)
		{
			cls_show_msg1(g_keeptime, "line %d:���ʧ��(%04x)", __LINE__, staval);
			return staval;
		}
		else
		{
#if 0	//���ǵ����Կ�������ֻ��������,��һ�ֲ��Բ���
			if(memcmp(BLK02DATA_FF, out, LEN_BLKDATA))
#else
			if(!memcmp(gen28_bak, DATA16, NL829_LEN_BLKDATA))//�״�
				memcpy(gen28_bak, out, NL829_LEN_BLKDATA);
			else if(memcmp(gen28_bak, out, NL829_LEN_BLKDATA))//������
#endif
			{
				cls_show_msg1(g_keeptime, "line %d:���дʧ��", __LINE__);
				return NL829_FAIL;
			}
		}
		return NL829_SUCC;
		break;
	case SAM:
	case IC:
		if((staval=NL829_APDUrw(UID[0], Select_1pay, sizeof(Select_1pay), out, &rcvlen))!=NL829_SUCC||(ret=NL829_GETSTAVAL(out+rcvlen-2))!=NL829_CPU_SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��дʧ��(%04x, 0x%04x)", __LINE__, staval, ret);
			return (staval!=NL829_SUCC)?staval:NL829_FAIL;
		}
		return NL829_SUCC;
		break;
	default:
		return NL829_FAIL;
		break;
	}
}

static ushort NL829_RFID_powerdown(SMART_t type, uchar *UID, ushort msec)
{
	/*private & local definition*/

	/*process body*/
	switch(type)
	{
	case CPU_A:
	case CPU_B:
	case MIFARE_1:
		return NL829_AntennaCtl(msec);
		break;
	case SAM:
	case IC:
		return NL829_ICSAM_powerdown(UID[0]);
		break;
	default:
		return NL829_FAIL;
		break;
	}	
}
#if 0  //���ݿ����ṩ��NL829MR_PP60RF����Աʹ���ֲ�v1.7  (ɾ��ɨ��ͷ�Ĵ���дָ��) ɨ��ͷ���ùر�
static void set_scan(void)
{
	/*private & local definition*/
	ushort staval = NL829_FAIL;
	int plen = 0;
	uchar pvalue[1024] = {0};
	uchar setcode39FULLenable[16] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x1C, 0x21, 0x32, 0x87};
	//uchar setcode39enable[16] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x1C, 0x11, 0x30, 0x97};
	uchar setcodeMSIenable[16] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x1F, 0x09, 0xF6, 0xFD};
	
	/*process body*/
    cls_show_msg1(1, "����ɨ��ͷ�Ĵ�����...");

	//case4:����ʶ��Code39��
	if((staval=NL829_SCAN_SetScan(setcodeMSIenable, 9, pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	//case5:����ʶ��MSI��
	if((staval=NL829_SCAN_SetScan(setcode39FULLenable, 9, pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}

    cls_show_msg("ɨ��ͷ�Ĵ����������");
	return;
}
#endif
static void test_scan(void)
{
	/*private & local definition*/
	ushort staval = NL829_FAIL;
	int plen = 0;
	uchar pvalue[1024] = {0};
#if 0  //��ָ��ȥ���� ��ʱ�ر�
	uchar setcode39disable[16] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x1C, 0x00, 0x32, 0x87};
	uchar setcode39enable[16] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x1C, 0x11, 0x30, 0x97};
#endif
	
	/*process body*/
    cls_show_msg1(1, "ɨ��ͷɨ��ָ�����...");//����ʾ��ˢ�����Բ˵�

	//case1:����ʽ����,��ʱδɨ����ȡɨ������Ӧ����0x0001
	if((staval=NL829_SCAN_Doscan(0,pvalue, &plen))!=NL829SCAN_TIMEOUT)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}

	//case2:������ʽɨ������£�δʵ��ɨ��ֱ�ӻ�ȡɨ������Ӧ�÷���0x0001����ɨ��
    if((staval=NL829_SCAN_Startscan())!=NL829_SUCC)
    {
        cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
    }
	if((staval=NL829_SCAN_GetData(pvalue, &plen))!=NL829SCAN_SCANING)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		NL829_SCAN_Stopscan();
		return;
	}
	if((staval=NL829_SCAN_Stopscan())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}

	//case3:������������ʽɨ��,ʵ��ɨ���Ӧ���ܹ����سɹ�,����ȡ��ɨ������,Ĭ�������֧��Code39���ɨ��
	cls_printf("����10����ɨ��Code39��");
	if((staval=NL829_SCAN_Doscan(10,pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		return;
	}
#if 0
	//case4:���ò�ʶ��Code39��,Ԥ��ɨ��Ӧ�ó�ʱ
	if((staval=NL829_SCAN_SetScan(setcode39disable, 9, pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	cls_printf("����5����ɨ��Code39��,Ԥ�ڳ�ʱ�޷�ʶ��");
	if((staval=NL829_SCAN_Doscan(5, pvalue, &plen))!=NL829SCAN_TIMEOUT)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	
	//case5:����ʶ��Code39��,Ԥ��ɨ��Ӧ�óɹ�
	if((staval=NL829_SCAN_SetScan(setcode39enable, 9, pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	cls_printf("����10��������ɨ��Code39��");
	if((staval=NL829_SCAN_Doscan(10,pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		return;
	}
#endif	
	//case6:�������÷�����ʽɨ�����,ɨ�赽������Ӧ�ú�Ԥ��һ��
	while(1)
	{
		if((staval=NL829_SCAN_Startscan())!=NL829_SUCC)
	    {
	        cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			NL829_SCAN_Stopscan();
			return;
	    }
		if(cls_show_msg1(20,"����20���ڽ���ɨ��,ɨ������������,ȡ�����˳�")==ESC)
			break;
		plen=0;
		memset(pvalue,0,sizeof(pvalue));
		if((staval=NL829_SCAN_GetData(pvalue, &plen))!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			NL829_SCAN_Stopscan();
			return;
		}
		if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			NL829_SCAN_Stopscan();
			return;
		}
		if((staval=NL829_SCAN_Stopscan())!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return;
		}
	}

	//case7:������ʽ,δ����start�����getdata��stop,Ԥ�ڲ������쳣
	if((staval=NL829_SCAN_GetData(pvalue, &plen))!=NL829SCAN_SCANING)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval=NL829_SCAN_Stopscan())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}

    cls_show_msg("ɨ��ͷɨ��ָ�����ͨ��");
	return;
}

//����SP10T ɨ�趯������ָ�� 20170828 sull add
static void test_showanimation(void)
{
	ushort staval = NL829_FAIL;
	int plen = 0 ;
	uchar pvalue[1024] = {0};

	//case1:�쳣��������:ctrl��Ϊ0x03������������Ҫ��ʾͼƬ��������(����������Ϊ1����ʾͼƬ4��)��Ҫ��ʾ��ͼƬ���в����ڵ�(��ʾͼƬ��Ϊ(0xff)
	if((staval = NL829_Scan_Animation(0x03, 0x04,0x00,(uchar *)"\x00\x00",(uchar *)"\x01\x02\x03\x04",strlen("\x01\x02\x03\x04"))) != NL829_FAIL)
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval = NL829_Scan_Animation(0x01, 0x01,0x00,(uchar *)"\x00\x00",(uchar *)"\x01\x02\x03\x04",strlen("\x01\x02\x03\x04"))) != NL829_ERR_ANIMATION)
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval = NL829_Scan_Animation(0x01, 0x01, 0x00, (uchar *)"\x00\x00", (uchar *)"\xFF",strlen("\xFF"))) != NL829_NOEXIT_ANIMATION)
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}

	//case2:��������
	//case2.1��������ʽɨ�裬��ɨ�����볬ʱʱ����Ϊ10s������������ɨ�������ǩ�����Ͻ���ʾ����
	if((staval = NL829_Scan_Animation(0x01,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval = NL829_Scan_Animation(0x02,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
    cls_show_msg("����10����ɨ������,ɨ�������ǩ�����Ͻ���ʾ���������������");
	if((staval=NL829_SCAN_Doscan(10,pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	if(cls_show_msg("ɨ�������ǩ�������Ƿ���ʾ����?�ǰ�[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}

	//case2.2:��������ʽɨ�裬��ɨ�����볬ʱʱ����Ϊ10s���رն�����ɨ�������ǩ�����Ͻ�����ʾ����
	if((staval = NL829_Scan_Animation(0x00,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_CLOSE)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval = NL829_Scan_Animation(0x02,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_CLOSE)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	cls_show_msg("����10����ɨ������,ɨ�������ǩ�����Ͻ�����ʾ���������������");	
	memset(pvalue,0,sizeof(pvalue));
	if((staval=NL829_SCAN_Doscan(10,pvalue, &plen))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if(cls_show_msg("ɨ�������ǩ�������Ƿ���ʾ����?�ǰ�[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	
	//case2.3:���÷�����ʽɨ�裬�����������Ƚ�ÿ��ͼƬ��ʾʱ����ΪĬ��(0.8s)������Ϊ0.4s(ǩ�����϶�����ʾ�ٶȱ��),����Ϊ1.2s(ǩ�����϶�����ʾ�ٶȱ���)
	if((staval = NL829_Scan_Animation(0x01,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}	
	cls_show_msg("ǩ��������ʾ�Ķ����ٶ�ΪĬ�Ϻ�����ٱ��������������");
	if((staval=NL829_SCAN_Startscan())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
	sleep(5);
	if((staval = NL829_Scan_Animation(0x01,0x04,0x04,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	sleep(5);
	if((staval = NL829_Scan_Animation(0x01,0x04,0x0f,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	sleep(5);
	if((staval=NL829_SCAN_Stopscan())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	if(cls_show_msg("ǩ��������ʾ�Ķ����ٶ��Ƿ�ΪĬ�Ϻ�����ٱ���?�ǰ�[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}

	//case2.4:���÷�����ʽɨ�裬������������������ʾʱ����Ϊ5s��ɨ��ʱ����Ϊ10s��ǩ��������ʾ����5s������ʾ����
	if((staval = NL829_Scan_Animation(0x01,0x04,0x00,(uchar *)"\x00\x32", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
	cls_show_msg("ǩ��������ʾ����5s�󽫲�����ʾ���������������");
	if((staval=NL829_SCAN_Startscan())!=NL829_SUCC)
    {
	    cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
    }
	sleep(10);
	if((staval=NL829_SCAN_Stopscan())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	if(cls_show_msg("ǩ��������ʾ����5s���Ƿ�����ʾ����?�ǰ�[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
	
	//case3�쳣����:PP60ָ����(��������ɨ������£�����PP60���Ӧִ�гɹ������÷�����ʽɨ��)
	cls_printf("���ָ�����...");
	if((staval = NL829_Scan_Animation(0x01,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_OPEN)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
	if((staval = NL829_SCAN_Startscan()) != NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
	sleep(1);//����˵����ɨ������ʱ���0.1s����ʾ�������ʼ���ʱ1s������ֱ��ִ����һ��ָ��᷵�سɹ�
	if((staval = PP60_cmd_C()) != NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	if((staval=PP60_cmd_D(2, NULL, 0))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	if((staval = NL829_SCAN_Stopscan()) != NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR2;
	}
	if((staval = NL829_Scan_Animation(0x00,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"))) != NL829_ANIMATION_CLOSE)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR1;
	}
	cls_show_msg("ɨ�趯������ָ�����ͨ��");
ERR1:
	NL829_Scan_Animation(0x00,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"));
	return;
ERR2:
	NL829_Scan_Animation(0x00,0x04,0x00,(uchar *)"\x00\x00", (uchar *)"\x01\x02\x03\x04", strlen("\x01\x02\x03\x04"));
	NL829_SCAN_Stopscan();
	return;
}


//����SP10T ������ʾͼƬ 20170712  jiangym add
static void test_showimage(void)
{
	ushort staval = NL829_FAIL;
	int  len = 0, i = 0, cnt = 0, num = 0, value = 0, x1 = 0, y1 = 0;
	uchar testbuf[256] = {0}, sdata[256] = {0};
	uchar x[2] = {0}, y[2] = {0}, w[2] = {0}, h[2] = {0}, cnum[2] = {0}, snum[2] = {0};
	uchar *err_para[] = {
					(uchar*) "\x00\x00", //0
					(uchar*) "\x00\x01", // 1
					(uchar*) "\x00\x08",// 2
					(uchar*) "\x01\x40",// 3
					(uchar*) "\x00\xF0",// 4
					(uchar*) "\x01\x41",// 5
					(uchar*) "\x00\xF1",// 6
					  };
	char sCUPBMP48x51[] =
	{
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X04,0X00,0X00,0X00,0X00,
	0X60,0X06,0X00,0X00,0X00,0X00,0XC7,0XE3,0X00,0X00,0X00,0X03,0X8F,0XF3,0X80,0X00,
	0X00,0X07,0X1F,0XF9,0XC0,0X00,0X00,0X0E,0X1F,0XF8,0XE0,0X00,0X00,0X1E,0X1E,0X00,
	0X70,0X00,0X00,0X3C,0X3C,0X00,0X78,0X00,0X00,0X38,0X3C,0X38,0X3C,0X00,0X00,0X78,
	0X3E,0X78,0X3E,0X00,0X00,0XF8,0X1F,0XF8,0X3E,0X00,0X00,0XF8,0X1F,0XF8,0X3F,0X00,
	0X01,0XFC,0X0F,0XF0,0X7F,0X80,0X03,0XFE,0X07,0XC0,0XFF,0X80,0X07,0XFF,0X00,0X01,
	0XFF,0XC0,0X07,0XFF,0X80,0X03,0XFF,0XE0,0X07,0XFF,0XC0,0X07,0XFF,0XE0,0X07,0XFF,
	0XE0,0X0F,0XFF,0XC0,0X03,0XFF,0XF0,0X1F,0XFF,0X80,0X01,0XFF,0XF8,0X3F,0XFF,0X00,
	0X00,0XFF,0XFC,0X7F,0XFE,0X00,0X00,0X7F,0XFE,0XFF,0XFC,0X00,0X00,0X3F,0XFE,0XFF,
	0XF8,0X00,0X00,0X1F,0XFC,0X7F,0XF0,0X00,0X00,0X0F,0XFC,0X7F,0XF0,0X00,0X00,0X0F,
	0XF8,0X3F,0XF0,0X00,0X00,0X0F,0XF8,0X1F,0XF0,0X00,0X00,0X0F,0XF8,0X1F,0XF0,0X00,
	0X00,0X1F,0XF0,0X0F,0XF8,0X00,0X00,0X3F,0XE0,0X07,0XFC,0X00,0X00,0XFF,0XC0,0X07,
	0XFE,0X00,0X00,0XFF,0X80,0X03,0XFF,0X00,0X00,0XFF,0X00,0X01,0XFF,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X07,0XFE,0X21,0X00,0X40,
	0X04,0X04,0X02,0X3F,0XE0,0XA0,0X7F,0XC5,0XFA,0X20,0X01,0X10,0X44,0X44,0X42,0X4F,
	0XC2,0X08,0X44,0X45,0XFA,0X40,0X0D,0XF6,0X44,0X44,0X42,0XCF,0XC0,0X00,0X7F,0XC4,
	0X52,0X40,0X00,0X00,0X04,0X04,0X4A,0X4F,0XC3,0XF8,0X04,0X07,0XFE,0X48,0X42,0X08,
	0X04,0X04,0X02,0X4F,0XC2,0X08,0X04,0X07,0XFE,0X48,0X43,0XF8,0X00,0X00,0X00,0X00,
	0X00,0X00, 
	};
	
	//case1:�쳣��������
	if((staval=NL829_Show_Image(err_para[1],err_para[0],err_para[3],err_para[4],err_para[0],err_para[1],testbuf,256)) !=NL829_ARGERR)	
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval=NL829_Show_Image(err_para[0],err_para[0],err_para[5],err_para[4],err_para[0],err_para[1],testbuf,256)) !=NL829_ARGERR)	
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval=NL829_Show_Image(err_para[0],err_para[1],err_para[3],err_para[4],err_para[0],err_para[1],testbuf,256)) !=NL829_ARGERR)	
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval=NL829_Show_Image(err_para[0],err_para[0],err_para[3],err_para[6],err_para[0],err_para[1],testbuf,256)) !=NL829_ARGERR)	
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if((staval=NL829_Show_Image(err_para[2],err_para[2],err_para[3],err_para[4],err_para[0],err_para[1],testbuf,256)) !=NL829_ARGERR)	
	{
		cls_show_msg("line %d:�쳣��������ʧ��(%04x)", __LINE__, staval);
		return;
	}

	//case2:��������
	cls_printf("ǩ������������ʾͼƬ");	
	len = sizeof(sCUPBMP48x51);
	cnt = len/256;
	value = len%256;
	if(value != 0)
		num=cnt;
	else
		num=cnt-1;	
	memcpy(w,"\x00\x30",2);//48
	memcpy(h,"\x00\x33",2);//51
	for(x1=0,y1=0;x1<=320-48, y1<=240-51;x1+=7,y1+=7)
	{
		x[0]=(x1&0xffff)>>8;
		x[1]=(x1)&0xff;
		y[0]=(y1&0xffff)>>8;
		y[1]=(y1)&0xff;
		for(i=0;i<cnt;i++)
		{
			cnum[0]=(i&0xffff)>>8;
			cnum[1]=(i)&0xff;
			snum[0]=((num-i)&0xffff)>>8;
			snum[1]=(num-i)&0xff;
			memcpy(sdata,sCUPBMP48x51+256*i,256);
			if((staval=NL829_Show_Image(x,y,w,h,cnum,snum,sdata,256)) !=NL829_SUCC) 
			{	
				cls_show_msg("line %d:ͼƬ����ʧ��(%04x)", __LINE__, staval);
				return;
			}
		}
		if(value != 0)
		{
			cnum[0]=(cnt&0xffff)>>8;
			cnum[1]=cnt&0xff;
			snum[0]=0x00;
			snum[1]=0x00;
			memcpy(sdata,sCUPBMP48x51+256*cnt,value);
			if((staval=NL829_Show_Image(x,y,w,h,cnum,snum,sdata,value)) !=NL829_SUCC) 
			{	
				cls_show_msg("line %d:ͼƬ����ʧ��(%04x)", __LINE__, staval);
				return;
			}
		}	
	}
	if(cls_show_msg("ǩ�������Ƿ�������ʾ��ȷ��ͼƬ,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:������ʾͼƬ����ʧ��", __LINE__);
		return;
	}
}
static void test_show(void)
{
	/*private & local definition*/
	int keyin = 0;
				
	/*process body*/
	while(1)
	{
		keyin = cls_show_msg("1.��ʾͼƬ\n"
								 "2.��ʾ����\n");
		switch(keyin) 
		{
			case '1':
				test_showimage();
				break;
			case '2':
				test_showanimation();	
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
	

//�������԰�����ȡ:��ʼ���հ���,��ȡ����ֵ,�������հ���
static void test_get_key(void)
{
	ushort staval = NL829_FAIL;
	uchar outbuf[2] = {0};
	int outlen = 0;
	
#include "kbtable.h"

	//case1:��ʼ���հ���,��ȡ����,��������	
	if((staval=NL829_Start_Recv_Key())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}	
	cls_show_msg("����SP10������̰���%s�������������",table[1].keyname);
	if((staval=NL829_Get_Key_Value(outbuf,&outlen ))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	if (outbuf[0] != table[1].keycode )
	{
		cls_show_msg("line %d: ��ȡ��������ʧ��(%s)", __LINE__,outbuf);
		return;
	}
	if((staval=NL829_End_Recv_Key())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	
	//case2:��ʼ���հ���,������������,����,��ȡ����,��������
	if((staval=NL829_Start_Recv_Key())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	//��������ָ��Ĳ��Ժ��ٽ��л�ȡ����
	test_showimage();
	//����
	cls_show_msg("����SP10������̰���%s�������������",table[10].keyname);
	if((staval=NL829_Get_Key_Value(outbuf,&outlen ))!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}	
	if (outbuf[0] != table[10].keycode )
	{
		cls_show_msg("line %d: ��ȡ��������ʧ��(%s)", __LINE__,outbuf);
		return;
	}
	if((staval=NL829_End_Recv_Key())!=NL829_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	cls_show_msg1(3,"��������ͨ��");
}
static void test_function(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while(1)
       {
		keyin = cls_show_msg("1.�ǿ��� 2.M-1\n"
							"3.�Ӵ�CPU 4.����\n" //2.4G:�˵���ʱ�ر�,ʹ�ð����滻,������Ҫ�ٿ���,20170806 jym
							"5.�ǽ�CPU 6.�쳣\n"
							"7.ɨ��ͷ 8.��ʾ\n");
		
		switch(keyin) 
		{
		case '1':
			test_control_cmd();
			break;
		case '2':
			test_RF_M1();
			break;
		case '3':
			if(g_NL829type!=DEVICE_YH100)
			{
				if(ENTER!=cls_show_msg("��ȷ����װ��������SAM(IC1����),[ENTER]����..."))
					break;
				test_ICSAM();
			}
			else
				cls_show_msg("YH100��֧�ֽӸò���");
			break;
		case '4':// 2.4G����RFSIM��һ��,Ŀǰ2.4G�ѻ���������
			//cls_show_msg1(2, "�ù����ݲ�֧��!");
			test_get_key();
			break;
		case '5':
			test_RF_CPU();
			break;
		case '6':	
			test_abnormity();
			break;
		case '7':	
			test_scan();
			break;
		case '8':	
			test_show();
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

//���˵��:����ע���ܡ�ѹ��(������һ��ǿ�����Ƿ�ɹ�)��,����ע����
//Ҳ����עability����,����ֵ�ں������й����и���
static void test_ability(SMART_t type)
{
	/*private & local definition*/
#define	nTestTime	(10)	//(5)

	//int nTestTime = 5;
	int count = 0;
	float fTimeElapsed = 0.0;
	
	//�˴�����Ӧ��RFID_iccrw��ʹ�õ�һ��
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	uchar Select_1pay[] ={'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
#else
	uchar Select_1pay[] = 
	{0x00, 0xA4, 0x04, 0x00, 0x0E,
	  '1',  'P',  'A',  'Y',  '.',  'S',  'Y',  'S',  '.',  'D',  'D',  'F',  '0',  '1',
	  0x00
	};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif

	uchar UID[NL829_LEN_UID] = {0};
	char *card[] = {"CPU_A", "CPU_B"/*, "HS_B"*/, "M-1"/*, "RFID-SIM"*/, "FELICA"};

	/*process body*/
	if(g_NL829type!=DEVICE_YH100)
	{
		//���ж��Ƿ�Ĭ��BPS,��Ĭ��BPS����������
		if(strcmp(g_devicecombps, "57600"))//if(g_devicecombps!=BPS57600)
		{
			cls_show_msg("���Ƚ�POS��BPS����Ϊ57600(Ĭ��),������˳�...");
			return;
		}
	}
	if(type!=SAM&&type!=IC)
		cls_show_msg("����NL829��Ӧ������%s��,���������...", card[type]);
	else
	{
		if(ENTER!=cls_show_msg("��ȷ����װ������IC/SAM,[ENTER]����..."))
			return;
		UID[0] = NL829_select_ICSAM();
	}
	
	//Ѱ��
#if 0
	cls_printf("���ڲ���Ѱ���ٶ�...\n");
	count = 0;
	lib_StartStopwatch();
	while(NL829_RFID_seek(type, UID)==NL829_SUCC)
	{
		count++;
		if((fTimeElapsed=ReadStopwatch())>nTestTime)
			break;
		if(NL829_RFID_powerdown(type, UID, 0xffff)!=NL829_SUCC)
			break;
	}
	if(count>0)//��Ϊ���ܿ����ڴ˲��Գ�����һ��100%�ɹ�,��ֻ���������ɹ���ƽ��ʱ�伴��//if(fTimeElapsed>nTestTime)
		cls_show_msg("Ѱ���ٶ�:%.0f��/��", fTimeElapsed/count);
	else
	{
		cls_show_msg("line %d:Ѱ���з���ʧ��(cnt=%d, err=%x)\n", __LINE__, count, GetErrorCode());
		return;
	}
#else	//������һ��Ѱ����ʱ��
	cls_printf("���ڲ���%s�ٶ�...\n", (type!=SAM&&type!=IC)?"Ѱ��":"�ϵ�");
	lib_StartStopwatch();
	if(NL829_RFID_seek(type, UID)!=NL829_SUCC)//����ע�ɹ���,��ѹ������ȥ��ע
	{
		cls_show_msg("line %d:%sʧ��", __LINE__, (type!=SAM&&type!=IC)?"Ѱ��":"�ϵ�");
		goto ERR;
	}
	cls_show_msg("%s�ٶ�:%.0f��/��", (type!=SAM&&type!=IC)?"Ѱ��":"�ϵ�", lib_ReadStopwatch());//ʱ���С,�����ⲻ����
#endif
	
	//����
	if (NL829_RFID_active(type, UID)!=NL829_SUCC)//����ע�ɹ���,��ѹ������ȥ��ע
	{	
		cls_show_msg("line %d:����ʧ��", __LINE__);
		goto ERR;
	}
	
	//����iccrw����
	cls_printf("���ڲ��Զ�д�ٶ�...\n");
	count = 0;
	lib_StartStopwatch();
	while(NL829_RFID_iccrw(type, UID)==NL829_SUCC)
	{
		count++;
		if((fTimeElapsed=lib_ReadStopwatch())>nTestTime)
			break;
	}
	if(fTimeElapsed>nTestTime)
		cls_show_msg("ÿ�����:%.0f�ֽ�", count*(type==MIFARE_1?NL829_LEN_BLKDATA:Select_1pay[4])/fTimeElapsed);
	else 
		cls_show_msg("line %d:��д�з���ʧ��(cnt=%d)\n", __LINE__, count);
	
	//�رճ�
	if (NL829_RFID_powerdown(type, UID, 0xffff)!=SUCC)//����ע�ɹ���,��ѹ������ȥ��ע
	{	
		cls_show_msg("line %d:%sʧ��", __LINE__, (type!=SAM&&type!=IC)?"�رճ�":"�µ�");
		return;
	}
	return;
ERR:
	NL829_RFID_powerdown(type, UID, 0xffff);//�˳�ǰ�رճ�
	return;
}

//��Ƶ��ѹ������
//���˵��:��ע��һ��ǿ�����Ƿ�����������
//���ڽӴ�/�ǽӵ�ѹ������,����Ҫ�ж�iccrw��Ľӿڷ��سɹ���,��Ҫ������״̬��(SW)��״̬����н�һ���ж�!!!
static void test_press(SMART_t type)
{
	/*private & local definition*/
	int succ = 0, cnt = DEFAULT_CNT_VLE, bak = 0, ret1 = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uchar UID[NL829_LEN_UID] = {0};
	ushort ret = 0;
	char *card[] = {"CPU_A", "CPU_B"/*, "HS_B"*/, "M-1"/*, "RFID-SIM"*/, "FELICA"};
	uint len = 0;

	/*process body*/
	if(type!=SAM&&type!=IC)
		cls_show_msg("����NL829��Ӧ������%s��,���������...", card[type]);
	else
	{
		if(ENTER!=cls_show_msg("��ȷ����װ������IC/SAM,[ENTER]����..."))
			return;
		UID[0] = NL829_select_ICSAM();
	}
	
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
	if ((ret1=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret1==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "ѹ��������...\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", cnt, succ))
			break;
		cnt--;

		//Ѱ��
		if((ret=NL829_RFID_seek(type, UID))!=NL829_SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:%sʧ��(%04x)", __LINE__, bak-cnt, (type!=SAM&&type!=IC)?"Ѱ��":"�ϵ�", ret);
			continue;//break;			
		}
		//����
		if ((ret=NL829_RFID_active(type, UID))!=NL829_SUCC)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		//��д
		if ((ret=NL829_RFID_iccrw(type, UID))!=NL829_SUCC)
		{	
			NL829_RFID_powerdown(type, UID, 10);
			cls_show_msg1(g_keeptime, "line %d:��%d��:��дʧ��(%04x)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		//�رճ�
		if ((ret=NL829_RFID_powerdown(type, UID, 0xffff))!=NL829_SUCC)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:%sʧ��(%04x)", __LINE__, bak-cnt, (type!=SAM&&type!=IC)?"�رճ�":"�µ�", ret);
			continue;//break;
		}
		succ++;
	}

	NL829_RFID_powerdown(type, UID, 0xffff);//�˳�ǰ�رճ�
	cls_show_msg("ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

//��Ƶ����ɨ��ͷ�Ľ������
//���˵��:��ע��һ��ǿ�����Ƿ�����������
//���ڽӴ�/�ǽӵ�ѹ������,����Ҫ�ж�iccrw��Ľӿڷ��سɹ���,��Ҫ������״̬��(SW)��״̬����н�һ���ж�!!!
static void test_rfcscan_cross(SMART_t type)
{
	/*private & local definition*/
	int succ = 0, cnt = 10, bak = 0, plen = 0, keyin = 0;
	char CntStr[8] = "10";
	uchar UID[NL829_LEN_UID] = {0}, pvalue[500] = {0};
	ushort ret = 0, ret1 = 0;
	char *card[] = {"CPU_A", "CPU_B"/*, "HS_B"*/, "M-1"/*, "RFID-SIM"*/, "FELICA"};
	uint len = 0;
	
	/*process body*/
	if(type!=SAM&&type!=IC)
		cls_show_msg("��׼����%s��,���������...", card[type]);
	else
	{
		if(ENTER!=cls_show_msg("��ȷ����װ������IC/SAM,[ENTER]����..."))
			return;
		UID[0] = NL829_select_ICSAM();
	}
	
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
	if ((keyin=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||keyin==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	
	while(cnt)
	{
		//��ɨ��ͷ
		if((ret=NL829_SCAN_Startscan())!=NL829_SUCC)
	    {
			cls_show_msg1(g_keeptime, "line %d:��%d��:��ɨ��ͷʧ��(%04x)", __LINE__, bak-cnt, ret);
			continue;
	    }
		
		cls_printf("����з��ÿ�Ƭ��ɨ�����\n��ʣ%d��(�ѳɹ�%d��)", cnt, succ);
		cnt--;

		ret = -1;
		ret1 = -1;
		ret1=NL829_RFID_seek(type, UID);
		ret=NL829_SCAN_GetData(pvalue, &plen);

		if(ret==NL829_SUCC)
		{
			plen=0;
			memset(pvalue,0,sizeof(pvalue));
			if((ret=NL829_SCAN_GetData(pvalue, &plen))==NL829_SUCC)
			{
				if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
				{
					cls_show_msg1(g_keeptime, "line %d:��%d��:ɨ��������ȷ", __LINE__, bak-cnt);
					return;
				}
				succ++;
			}
		}
		else if(ret1==NL829_SUCC)
		{
			//����
			if ((ret=NL829_RFID_active(type, UID))!=NL829_SUCC)
			{	
				cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, ret);
				continue;//break;
			}
			//��д
			if ((ret=NL829_RFID_iccrw(type, UID))!=NL829_SUCC)
			{	
				NL829_RFID_powerdown(type, UID, 10);
				cls_show_msg1(g_keeptime, "line %d:��%d��:��дʧ��(%04x)", __LINE__, bak-cnt, ret);
				continue;//break;
			}
			//�رճ�
			if ((ret=NL829_RFID_powerdown(type, UID, 0xffff))!=NL829_SUCC)
			{	
				cls_show_msg1(g_keeptime, "line %d:��%d��:%sʧ��(%04x)", __LINE__, bak-cnt, (type!=SAM&&type!=IC)?"�رճ�":"�µ�", ret);
				continue;//break;
			}
			cls_show_msg1(2,"��Ƭ�����ɹ�,�����߿�Ƭ");
			succ++;
		}
		else
			cnt++;//����������û�гɹ�����£������Ѳ��Դ���
		
		if((ret=NL829_SCAN_Stopscan())!=NL829_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, ret);
			return;
		}
	}

	NL829_SCAN_Stopscan();
	NL829_RFID_powerdown(type, UID, 0xffff);//�˳�ǰ�رճ�
	cls_show_msg("����������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void test_rw_press(SMART_t type)
{
	/*private & local definition*/
	int succ = 0, cnt = DEFAULT_CNT_VLE, bak = 0, ret1 = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uchar UID[NL829_LEN_UID] = {0};
	ushort ret = 0;
	char *card[] = {"CPU_A", "CPU_B"/*, "HS_B"*/, "M-1"/*, "RFID-SIM"*/, "FELICA"};
	uint len = 0;

	/*process body*/
	if(type!=SAM&&type!=IC)
		cls_show_msg("����NL829��Ӧ������%s��,���������...", card[type]);
	else
	{
		if(ENTER!=cls_show_msg("��ȷ����װ������IC/SAM,[ENTER]����..."))
			return;
		UID[0] = NL829_select_ICSAM();
	}
	
	//����ʹ�ô���������,����ʱ��(��Ϊ��дSAM�й�ʱ���ж�,��ʱ�нϴ�ƫ��)
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
	if ((ret1=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret1==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	while(cnt)
	{
		//Ѱ��
		if((ret=NL829_RFID_seek(type, UID))!=NL829_SUCC)
		{
			cnt--;
			if(ESC==cls_show_msg1(g_keeptime, "line %d:��%d��:%sʧ��(%04x)", __LINE__, bak-cnt, (type!=SAM&&type!=IC)?"Ѱ��":"�ϵ�", ret))
				break;
			else
				continue;//break;
		}
		//����
		if ((ret=NL829_RFID_active(type, UID))!=NL829_SUCC)
		{	
			cnt--;
			if(ESC==cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, ret))
				break;
			else
				continue;//break;
		}

		while(cnt)
		{
			cls_printf("��дѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", cnt, succ);
			cnt--;

			//��д
			if ((ret=NL829_RFID_iccrw(type, UID))!=NL829_SUCC)
			{	
				NL829_RFID_powerdown(type, UID, 10);
				cls_show_msg1(g_keeptime, "line %d:��%d��:��дʧ��(%04x)", __LINE__, bak-cnt, ret);
				break;					
			}
			succ++;
		}
	}

	//�رճ�
	if ((ret=NL829_RFID_powerdown(type, UID, 0xffff))!=NL829_SUCC)//�������,���������ùر�
	{	
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(%04x)", __LINE__, (type!=SAM&&type!=IC)?"�رճ�":"�µ�", ret);
		//��ʱ�µ�ʧ��,��������ѹ�����Գɹ���return;
	}

	cls_show_msg("��дѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void test_compatibility(SMART_t type)
{
	/*private & local definition*/
#if 0
	uchar UID[NL829_LEN_UID] = {0};
	uchar out[NL829_MAXLEN_DATA] = {0};
	ushort ret = 0;
#endif

	/*process body*/
	switch(type)
	{
	case MIFARE_1:// 4k M-1���Ĳ���
#if 0	//����test_RF_M1��
		cls_show_msg("����NL829��Ӧ������1��4k M1��,���������...");
		//Ѱ��
		if((ret=NL829_RFID_seek(type, UID))!=NL829_SUCC)
		{
			cls_show_msg("line %d:Ѱ��ʧ��(%04x)", __LINE__, ret);
			goto ERR;
		}
		//��֤/����,�˴�ʹ���ⲿ��֤(�ڲ���֤����,����Կ��Ĵ洢��������Ӱ��)
		//���ڲ�֪��4k����B��Կ�Ƕ���,ʹ��A��Կ(������A��ԿΪȫ0xff)
		if((ret=NL829_M1_auth_ex(NL829_AUTHKEY_TYPE_A, UID, 0xfe, NL829_AUTHKEY))!=NL829_SUCC)//��֤�Ŀ��ڵ�4K�ڵ�����
		{
			cls_show_msg("line %d:�ⲿ��֤ʧ��(%04x)", __LINE__, ret);
			goto ERR;
		}
		//��д,���ǵ����Կ�����,����������
		if((ret=NL829_M1_BLKRead(0xfd, out))!=NL829_SUCC)//0xfe���������Ŀ�Ӧ���ܶ�
		{
			cls_show_msg("line %d:���ʧ��(%04x)", __LINE__, ret);
			goto ERR;
		}
		//�رճ�
		if ((ret=NL829_RFID_powerdown(type, UID, 0xffff))!=NL829_SUCC)
		{	
			cls_show_msg("line %d:�رճ�ʧ��(%04x)", __LINE__, ret);
			goto ERR;
		}
		break;
#endif
	case CPU_A:
	case CPU_B:
	case SAM:
	case IC:
	default:
		cls_show_msg("���޼����Բ�����Ŀ!��ѡ������������");
		return;
		break;
	}

	cls_show_msg("�����Բ���ͨ��");
	return;
#if 0
ERR:
	NL829_RFID_powerdown(type, UID, 0xffff);//�˳�ǰ�رճ�
	return;
#endif
}

static void conf_NL829(SMART_t *type)
{
	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("1.����������\n"
							"2.��·����\n"
							"3.��������\n"
							);//"4.ɨ��Ĵ�������\n"

		switch(nKeyin) 
		{
		case '1':
			if(cls_show_msg("��ʾ:���ܲ��Բ��ؽ�������!��Ҫ���ð�[ENTER]...")!=ENTER)
				break;
			*type = select_smarttype();//������öԹ��ܲ��Բ�������
			break;
		case '2':
			cls_show_msg("��ȷ��POS������������!���������...");
			if(conf_test_bps()!=SUCC)
				cls_show_msg("line %d:����ʧ��!������POS���������������.", __LINE__);
			else
				cls_show_msg1(2, "��·���óɹ�!");
			break;
		case '3':
			g_NL829type = select_NL829();
			break;
	//	case '4':
	//		set_scan();
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

/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		  jennywone163 	20110620		created
*****************************************************************/
void systest36(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	SMART_t type = CPU_A;
	
	/*process body*/
	cls_show_msg("%s������...���Ƚ�������,���������ʼ\n", TESTITEM);
	g_NL829type = DEVICE_NL829LCD;

	while(1)
	{
		keyin = cls_show_msg("1.���� 2.����\n"
							"3.�ۺ�ѹ��\n"
							"4.��дѹ��\n"
							"5.������ 0.����\n"
							"6.ɨ����Ƶ����\n");

		memcpy(gen28_bak, DATA16, NL829_LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������		
		switch(keyin)
		{
		case '1':
			test_function();
			break;
		case '2':
			test_ability(type);
			cls_show_msg("���������������ͽ��в���!���������...");
			break;
		case '3':
			test_press(type);
			break;
		case '4':
			if(ENTER!=cls_show_msg("��дѹ������(������Բ�����ESC�жϲ���),ENTER������..."))
				break;
			test_rw_press(type);
			break;
		case '5':
			test_compatibility(type);
			break;
		case '6':
			test_rfcscan_cross(type);
			break;
		case '0':
			conf_NL829(&type);
			break;
		case ESC:
			cls_show_msg("����������BPS���²���!���������...");
			if(g_NL829type==DEVICE_NL829MR_PP60RF)
				cls_show_msg("�����н���PED���ֵĲ���!");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	NDK_PortClose(g_devicecom);
	return;
}

