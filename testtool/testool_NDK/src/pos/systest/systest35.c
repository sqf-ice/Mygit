/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ۺ�ģ��
* file name		: 27.c
* Author 			: 
* version			: V1.0
* DATE			: 20110809
* directory 		: .\SRC\general\
* description		: PP60����
* related document	: PP60����Աʹ���ֲ�.doc������ĵ�
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
#define	TESTITEM	"PP60"

//������ʱ����һЩ�ⲻ��������,������Ϻ�ȥ��!
#define	TEST	ENABLE	//�������޸����������,���˺��,��һ����ر�
//#define	TEST	DISABLE

#define	DEFAULT_CNT_STR	"15000"	//"3000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(1000)

#define	DEBUG_CMD_ALL		(-1)		//for PP60
//��������CMD�Ͷ���DEBUG_CMDΪ����(����������뷵������);����ʹ��ʱ,�رմ˺�
//#define	DEBUG_CMD			'H'	//CMD_H
//#define	DEBUG_CMD			DEBUG_CMD_ALL	//ȫ���������

#define ONEHZSIZE (64)

/*------------global variables definition-----------------------*/
static int g_PP60type = DEVICE_PP60RF/*DEVICE_PP60PT*/;
static int g_adjust = 0;
static int g_algtype = PP60_TDESFLAG;//֧�ֹ���SM4�㷨����Ŀ���20160303
static int g_LCDSize = 0; //����������� ���� ֧�ִ�������Ϊ1,����SP10_T����;����Ϊ0

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct data_result {
	int len;
	uchar *data;
} DATA_RESULT;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
int conf_NL829_bps(void)
{
	/*private & local definition*/
	int nKeyin = 0, loop = 0;
	char *bps[] = {"9600", "19200", "38400", "57600", "115200"};
	static int first = TRUE;//�״ο����ϵ�ı�־
	
	/*process body*/
	// 1.������NL829���ӵ�POS���ڲ���
	cls_show_msg1(2, "�������������贮�ڵĲ�������");
	g_devicecom = select_serial();

	// 2.����NL829���ڲ���
	cls_show_msg1(2, "������NL829���ڵĲ���(bps)����");
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
		case MAXMENUITEM://'5':
			//if(initaux(g_devicecom, (first==TRUE)?BPS57600:g_devicecombps, DB8|STOP1|NP)!=SUCC)//first֮��ĳ�ʼ�����Ǳ����,��������һ��
			if(NDK_PortOpen(g_devicecom, (first==TRUE)?"57600":g_devicecombps)!=NDK_OK)//first֮��ĳ�ʼ�����Ǳ����,��������һ��
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
					NDK_PortOpen(g_devicecom, bps[loop]);//initaux(g_devicecom, bps[loop], DB8|STOP1|NP);
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
	if(NDK_PortOpen(g_devicecom, bps[nKeyin-'1'])!=NDK_OK)//if(initaux(g_devicecom, bps[nKeyin-'1'], DB8|STOP1|NP)!=SUCC)
	{
		cls_show_msg1(2, "line %d:��ʼ��POS����ʧ��", __LINE__);
		return FAIL;
	}
	strcpy(g_devicecombps, bps[nKeyin-'1']);//g_devicecombps = bps[nKeyin-'1'];//g_bps�������ý��е�������
	first = FALSE;
	return SUCC;
}

static int PP60_code(uchar *in, int inlen, uchar *out)
{
	/*private & local definition*/
	int id = 0;
	
	/*process body*/
	for(; id<inlen; id++)
	{
		out[id*2] = PP60_CODE_H(in[id]);
		out[id*2+1] = PP60_CODE_L(in[id]);
	}
	return inlen*2;
}

static int PP60_decode(uchar *in, int inlen, uchar *out)
{
	/*private & local definition*/
	int id = 0;
	
	/*process body*/
	if(inlen%2)
		return FAIL;
	for(; id<inlen; id+=2)
		out[id/2] = PP60_DECODE(*(in+id), *(in+id+1));
	return inlen/2;
}

//data��NL829����ӿڵ�data��ͬ,ָ������(1byte)�������������.�ʿ���ΪNULL,����Ҳ����Ϊ0
static int PP60_pack_data(uchar cmd, uchar *data, int datalen, uchar *frame)
{
	/*private & local definition*/
	int TailOffset = PP60_OFFSET_DATA+datalen;
	int framelen = TailOffset+PP60_LEN_TAIL;
	
	/*process body*/
#if 0
	if(datalen<0||datalen>PP60_MAXLEN_DATA)
		return FAIL;
#endif
	frame[PP60_OFFSET_HEAD] = ESC;
	frame[PP60_OFFSET_CMD] = cmd;
	if(data!=NULL&&datalen!=0)
		memcpy(frame+PP60_OFFSET_DATA, data, datalen);
	memcpy(frame+TailOffset, PP60_TAIL, PP60_LEN_TAIL);
	return framelen;
}

static ushort PP60_send_frame(uchar *cmdstr, int cmdlen)
{
	/*private & local definition*/
	
	/*process body*/
	if(g_PP60type==DEVICE_PP60PT)
	{
		serial_clrportbuf_all(g_devicecom);
		//���������֡����һ�㲻�ᳬ��������󻺳�,��portwriteһ�㶼��ɹ�,�ɲ��л���жϷ���
		return (NDK_PortWrite(g_devicecom, (uint)cmdlen, (char *)cmdstr)==NDK_OK)?NL829_PP60_SUCC:NL829_PP60_FAIL_SEND;
	}
	else if(g_PP60type==DEVICE_PP60RF)
		return NL829_pack_PP60cmd(cmdstr, cmdlen);
	else
		return NL829_PP60_NOTSUPPORT;
}

//PP60���ص������м������:0xaa, 0x55, 0x56, 0x02+����+�ַ���, ����������(��,�汾��Ϣ,sn.��Ȼ,�������,��ʽҲ�ǲ�����)
//����Щ����еĵ�һ������һ���ǲ�ͬ��
//��ʽ�ϲ�ͳһ�Ҷ�����������ݽ�Ϊ��,�ʲ��ṩͳһ�Ľ������,�����ṩ���պ���
static ushort PP60PT_recv_frame(uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort len = 0;
	uchar frame[PP60_MAXLEN_FRAME] = {0};
	int rlen = 0, ret = 0;
	
	/*process body*/
	//if(portread(g_devicecom, 1, (char *)frame, PP60_TIMEOUT_BASE+g_adjust)==1)
	if(NDK_PortRead(g_devicecom, 1, (char *)frame, (PP60_TIMEOUT_BASE+g_adjust)*1000, &rlen)==NDK_OK&&rlen==1)
	{
		if(frame[0]==0xaa||frame[0]==0x55||frame[0]==0x56)
		{
			*outlen = 1;
			*out = frame[0];
		}
		else if(frame[0]==PP60_CIPHERRSP_HEAD)
		{
			//if(portread(g_devicecom, 1, (char *)frame+1, /*0*/PP60_TIMEOUT_BASE+g_adjust)==1)
			if(NDK_PortRead(g_devicecom, 1, (char *)frame+1, /*0*/(PP60_TIMEOUT_BASE+g_adjust)*1000, &rlen)==NDK_OK&&rlen==1)
			{
				//if(portread(g_devicecom, frame[1], (char *)frame+2, /*0*/PP60_TIMEOUT_BASE+g_adjust)==frame[1])
				if(NDK_PortRead(g_devicecom, frame[1], (char *)frame+2, /*0*/(PP60_TIMEOUT_BASE+g_adjust)*1000, &rlen)==NDK_OK&&rlen==frame[1])
				{
					*outlen = frame[1]+2;
					memcpy(out, frame, *outlen);
				}
				else
					return PP60_ERR_FRAME1;
			}
			else
				return PP60_ERR_FRAME2;
		}
		else//��ȡ������������ʽ���ݵ����
		{
			//if((len=portread(g_devicecom, PP60_MAXLEN_FRAME-1, (char *)frame+1, PP60_TIMEOUT_BASE+g_adjust))>0)//���ڲ����������,�������������,���ظ��ϲ�ȥ�ж�
			if((ret=NDK_PortRead(g_devicecom, PP60_MAXLEN_FRAME-1, (char *)frame+1, (PP60_TIMEOUT_BASE+g_adjust)*1000, &rlen))==NDK_OK&&rlen>0)
			{
				len = rlen;
				*outlen = len+1;
				memcpy(out, frame, *outlen);
			}
			else if(ret==NDK_ERR_TIMEOUT)//else if(len==NDK_ERR_TIMEOUT)
			{
				*outlen = 1;
				*out = frame[0];
			}
			else
				return PP60_ERR_FRAME3;
		}
	}
	else	
		return NL829_PP60_FAIL_COMM;

	return NL829_PP60_SUCC;
}

//inlenΪϣ�����ص����ݳ���,������ʵ�ֵ�ڵ��õ�NL829_get_PP60rspʱ�����Ч��
//ע��:�������ɰ�ֻ˵����·���յĳɰ�,���巵�����ݴ���ĳɰ����廹��Ҫ��һ����֤out��outlen(�����������)!
static ushort PP60_recv_frame(ushort inlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	
	/*process body*/
	if(g_PP60type==DEVICE_PP60PT)
		return PP60PT_recv_frame(out, outlen);
	else if(g_PP60type==DEVICE_PP60RF)
		return NL829_get_PP60rsp(/*10*/NL829_TIMEOUT_BASE+g_adjust, inlen, out, outlen);
	else
		return NL829_PP60_NOTSUPPORT;
}

//����out��outlen�Ƿ����Ԥ��,����һ���ж�.�����������سɹ�ʱ,������֤out��outlen������һ���Ԥ��.������Ƶ���NL829_unpack_dataҲ����append���н���У��
static ushort PP60_frame_factory(uchar cmd, uchar *data, int datalen, ushort inlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	int len = 0;
	ushort ret1 = NL829_FAIL;
	uchar frame_cmd[PP60_MAXLEN_FRAME] = {0};
	
	/*process body*/
	//���
	len = PP60_pack_data(cmd, data, datalen, frame_cmd);
#if defined DEBUG_CMD//���������
	if(cmd==DEBUG_CMD||DEBUG_CMD==DEBUG_CMD_ALL)
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<len; i++)
			NDK_ScrPrintf(" %02X", *(frame_cmd+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif
	//����
	if((ret1=PP60_send_frame(frame_cmd, len))!=NL829_PP60_SUCC)
		return ret1;//return PP60_ERR_FRAME_SND;
	//�հ�
	if((ret1=PP60_recv_frame(inlen, out, outlen))!=NL829_PP60_SUCC)
		return ret1;//����NL829_ERR_FRAME_XXX(����Ľ��մ���)
       
#if defined DEBUG_CMD//���������
	if(cmd==DEBUG_CMD||DEBUG_CMD==DEBUG_CMD_ALL)
	{
		int i = 0;
		cls_printf("line %d:", __LINE__);//__FILE__̫����,Ӱ��������ݵ���ʾ,�ʴ˴�����ʾ��
		for(; i<*outlen; i++)
			NDK_ScrPrintf(" %02X", *(out+i));
		NDK_ScrRefresh();
		lib_getch();
	}
#endif
	//���(outδ����ȷ��������,�����.����out�������Ϊ����,����һ�������ȷ���)
	return NL829_PP60_SUCC;
}

//Fʵ����û��,���������д
static ushort PP60_cmd_A(uchar M, uchar N, uchar F)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	int datalen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = M;
	data[1] = N;
	data[2] = F;
	datalen = (F==PP60_NODESFLAG)?2:3;
	if((ret=PP60_frame_factory('A', data, datalen, PP60_CMDA_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDA_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort __PP60_cmd_Bb(uchar cmd, uchar bps)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = bps;
	if((ret=PP60_frame_factory(cmd, data, 1, PP60_CMDB_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDB_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;	
}

//ESC+B��ESC+b��PP60RF�ϲ�֧��,������ظ�0x0001
static ushort PP60_cmd_B(uchar bps){return __PP60_cmd_Bb('B' ,bps);}

//static ushort PP60_cmd_b(uchar bps){return __PP60_cmd_Bb('b' ,bps);}

extern /*static*/ ushort PP60_cmd_C(void)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	if((ret=PP60_frame_factory('C', NULL, 0, PP60_CMDC_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDC_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if((out[0]==PP60_FAIL) || (out[0]==PP60_ERR)) //����ɨ�趯��ָ��ʱ���Ҳ���ʹ���ָ��᷵��0xFF��������PP60_ERR״̬  20170828 sull
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//id������ֵ����,���ش����ַ�
extern /*static*/ ushort PP60_cmd_D(uchar id, uchar *str, int len)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	int datalen = 1+len;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	memcpy(data+1, str, len);
	if((ret=PP60_frame_factory('D', data, datalen, PP60_CMDD_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDD_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if((out[0]==PP60_FAIL) || (out[0]==PP60_ERR)) //����ɨ�趯��ָ��ʱ���Ҳ���ʹ���ָ��᷵��0xFF��������PP60_ERR״̬  20170828 sull
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_E(uchar id)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	if((ret=PP60_frame_factory('E', data, 1, PP60_CMDE_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDE_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_F(uchar id, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	if((ret=PP60_frame_factory('F', data, 1, PP60_CMDF_RCVLEN, out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1% ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) == 2)
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]% ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) != 0)
			return PP60_ERR_CMD_BASE+1+out1[0];
		else//�ɹ������,ֻ�ͳ��������ݼ���,ͷ�볤�Ȳ�����(��Ϊ��ȷ�ϵ�,��֪��)
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

//����ȥ��02ͷ�볤������������ݴ���out,out�ĳ��ȴ���outlen
static ushort PP60_cmd_G(uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar out1[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	if((ret=PP60_frame_factory('G', NULL, 0, PP60_CMDG_RCVLEN, out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1%2==0)
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]%2==1)//out1[1]% ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) !=0)
			return PP60_ERR_CMD_BASE+1+out1[0];
		else//�ɹ������,ֻ�ͳ��������ݼ���,ͷ�볤�Ȳ�����(��Ϊ��ȷ�ϵ�,��֪��)
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
//str��ԭʼ���ݲ�Ҫ�����ַ���,��ֻҪ�����ݴ�����
static ushort PP60_cmd_H(uchar id, uchar *str, uchar inlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0/*, expectlen = (id==0)?PP60_CMDH0_RCVLEN:PP60_MAXLEN_DATA PP60_CMDH1_RCVLEN*/, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	data[1] = inlen;
	memcpy(data+2, str, inlen);
	if((ret=PP60_frame_factory('H', data, 2+inlen, (id==0)?PP60_CMDH0_RCVLEN:PP60_MAXLEN_DATA, out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
	if(id==1)
	{
		if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
			return NL829_PP60_FAIL;
		else if(outlen1% ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) == 2)
		{
			if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]% ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) != 0)
				return PP60_ERR_CMD_BASE+1+out1[0];
			else
			{
				*outlen = out1[1];
				memcpy(out, out1+2, out1[1]);
				return NL829_PP60_SUCC;
			}
		}
		else
			return PP60_ERR_CMD_BASE;
	}
	else //if(id==0)//id��0��1�������ֻ�ж�out[0]
	{
		if(outlen1==PP60_CMDH0_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		{
			if(out1[0]==PP60_SUCC)
				return NL829_PP60_SUCC;
			else if(out1[0]==PP60_FAIL)
				return NL829_PP60_FAIL;
			else
				return PP60_ERR_CMD_BASE+1+out1[0];
		}
		else
			return PP60_ERR_CMD_BASE;
	}
}

static ushort PP60_cmd_I(uchar id, uchar *cardno, int cardnolen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};	
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	memcpy(data+1, cardno, cardnolen);
	if((ret=PP60_frame_factory('I', data, 1+cardnolen, ((g_algtype==PP60_TDESFLAG)?PP60_CMDI_RCVLEN:PP60_CMDI_SM4_RCVLEN), out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1== ((g_algtype==PP60_TDESFLAG)?PP60_CMDI_RCVLEN:PP60_CMDI_SM4_RCVLEN) )
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]!= ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) )
			return PP60_ERR_CMD_BASE+1+out1[0];
		else
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_X(uchar id, uchar *cardno, int cardnolen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};	
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	memcpy(data+1, cardno, cardnolen);
	if((ret=PP60_frame_factory('X', data, 1+cardnolen, ((g_algtype==PP60_TDESFLAG)?PP60_CMDX_RCVLEN:PP60_CMDX_SM4_RCVLEN), out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{
		out[0] = out1[0];
		*outlen = outlen1;
		if(out1[0]==0x56)
			return NL829_PP60_SUCC;
		else// if(out1[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
	}
	else if(outlen1== ((g_algtype==PP60_TDESFLAG)?PP60_CMDX_RCVLEN:PP60_CMDX_SM4_RCVLEN) )
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]!= ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) )
			return PP60_ERR_CMD_BASE+1+out1[0];
		else
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_J(uchar id, uchar *cardno, int cardnolen, uchar *STR, int STRlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};	
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	memcpy(data+1, cardno, cardnolen);
	memcpy(data+1+cardnolen, STR, STRlen);
	if((ret=PP60_frame_factory('J', data, 1+cardnolen+STRlen, ((g_algtype==PP60_TDESFLAG)?PP60_CMDJ_RCVLEN:PP60_CMDJ_SM4_RCVLEN), out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1== ((g_algtype==PP60_TDESFLAG)?PP60_CMDJ_RCVLEN:PP60_CMDJ_SM4_RCVLEN) )
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]!= ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) )
			return PP60_ERR_CMD_BASE+1+out1[0];
		else
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
static ushort PP60_cmd_K(uchar M, uchar *STR, int STRlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};	
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = M;
	memcpy(data+1, STR, STRlen);
	if((ret=PP60_frame_factory('K', data, 1+STRlen, ((g_algtype==PP60_TDESFLAG)?PP60_CMDK_RCVLEN:PP60_CMDK_SM4_RCVLEN), out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1== ((g_algtype==PP60_TDESFLAG)?PP60_CMDK_RCVLEN:PP60_CMDK_SM4_RCVLEN) )
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]!= ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) )
			return PP60_ERR_CMD_BASE+1+out1[0];
		else
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

extern /*static*/ ushort PP60_cmd_L(uchar maxPINlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = maxPINlen;
	if((ret=PP60_frame_factory('L', data, 1, PP60_CMDL_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDL_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȷֱ��OK��NK����һ��PP60_code
static ushort PP60_cmd_M(uchar P, uchar *OK, int OKlen, uchar *NK, int NKlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = P;
	memcpy(data+1, OK, OKlen);
	memcpy(data+1+OKlen, NK, NKlen);
	if((ret=PP60_frame_factory('M', data, 1+OKlen+NKlen, PP60_CMDM_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDM_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

extern /*static*/ ushort PP60_cmd_N(uchar minPINlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = minPINlen;
	if((ret=PP60_frame_factory('N', data, 1, PP60_CMDN_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDN_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_O(uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar out1[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	if((ret=PP60_frame_factory('O', NULL, 0, PP60_MAXLEN_DATA, out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1==out1[1]+2)
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD)
			return PP60_ERR_CMD_BASE+1+out1[0];
		else
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_P(uchar *STR, int STRlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	memcpy(data, STR, STRlen);
	if((ret=PP60_frame_factory('P', data, STRlen, PP60_CMDP_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDP_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//������Ŀǰ��֧��,ʵ�����������쳣����(��ʶ������Ĳ���)
static ushort PP60_cmd_Q(void)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
#if 0
	return PP60_frame_factory('Q', NULL, 0, PP60_MAXLEN_DATA, out, &outlen);
#else
	if((ret=PP60_frame_factory('Q', NULL, 0, PP60_CMDQ_RCVLEN/*PP60_MAXLEN_DATA*/, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDQ_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
#endif
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
static ushort PP60_cmd_R(uchar *XX, int XXlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	memcpy(data, XX, XXlen);
	if((ret=PP60_frame_factory('R', data, XXlen, PP60_CMDR_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDR_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
static ushort PP60_cmd_S(uchar M, uchar N, uchar *STR, int STRlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = M;
	data[1] = N;
	data[2] = STRlen;
	memcpy(data+3, STR, STRlen);
	if((ret=PP60_frame_factory('S', data, 3+STRlen, PP60_CMDS_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDS_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
static ushort PP60_cmd_T(uchar XX, uchar *STR, int STRlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = XX;
	memcpy(data+1, STR, STRlen);
	if((ret=PP60_frame_factory('T', data, 1+STRlen, ((g_algtype==PP60_TDESFLAG)?PP60_CMDT_RCVLEN:PP60_CMDT_SM4_RCVLEN), out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		return NL829_PP60_FAIL;
	else if(outlen1== ((g_algtype==PP60_TDESFLAG)?PP60_CMDT_RCVLEN:PP60_CMDT_SM4_RCVLEN) )
	{
		if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]!= ((g_algtype==PP60_TDESFLAG)?PP60_CIPHERRSP_LEN:PP60_SM4_CIPHERRSP_LEN) )
			return PP60_ERR_CMD_BASE+1+out1[0];
		else
		{
			*outlen = out1[1];
			memcpy(out, out1+2, out1[1]);
			return NL829_PP60_SUCC;
		}
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
static ushort PP60_cmd_U(uchar M, uchar N, uchar *MK, int MKlen, uchar *UK, int UKlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = M;
	data[1] = N;
	memcpy(data+2, MK, MKlen);
	memcpy(data+2+MKlen, UK, UKlen);
	if((ret=PP60_frame_factory('U', data, 2+MKlen+UKlen, PP60_CMDU_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDU_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_V(uchar *out, ushort *outlen){return PP60_frame_factory('V', NULL, 0, PP60_MAXLEN_DATA, out, outlen);}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
//��DD�Ǳ���������.code���ɱ�����ִ����Ϊ�˿��Բ��Ե�DDlenΪ���������
static ushort PP60_cmd_W(uchar XX, uchar YY, uchar *DD, int DDlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = XX;
	data[1] = YY;
	memcpy(data+2, DD, DDlen);
	if((ret=PP60_frame_factory('W', data, 2+DDlen, PP60_CMDW_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDW_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

static ushort PP60_cmd_Z(void)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	if((ret=PP60_frame_factory('Z', NULL, 0, PP60_CMDZ_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDZ_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//ע��:��Ҫ��ȷʹ�ñ�����,��ҪҪ֮ǰ�ȵ���һ��PP60_code
//����˵��:id==1ʱ,�����ĺ���������;id!=1ʱ,����������Ҫ��Ӧ��Ԥ����.Ϊ2ʱ,ǰ��������������
static ushort PP60_cmd_x(uchar id, uchar *in, int inlen, uchar *out, ushort *outlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen1 = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0};
	int outlen2 = 0;
	
	/*process body*/
	if(id!=1&&(out==NULL||outlen==NULL))
		return PP60_ERR_CMD_BASE;
	
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = id+'0';
	memcpy(data+1, in, inlen);
	if((ret=PP60_frame_factory('x', data, 1+inlen, (id==1)?PP60_CMDx1_RCVLEN:PP60_MAXLEN_DATA, out1, &outlen1))!=NL829_PP60_SUCC)
		return ret;
	if(id==1)
	{
		if(outlen1==PP60_CMDx1_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
		{	
			//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
			if(out1[0]==PP60_SUCC)
				return NL829_PP60_SUCC;
			else if(out1[0]==PP60_FAIL)
				return NL829_PP60_FAIL;
			else
				return PP60_ERR_CMD_BASE+1+out1[0];//��1������һ��out[0]Ϊ0x00�����
		}
		else
			return PP60_ERR_CMD_BASE;
	}
	else //if(id==2)//idΪ��1��2���쳣����
	{
		if(outlen1==1&&out1[0]==PP60_FAIL)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
			return NL829_PP60_FAIL;
		else if(outlen1==out1[1]+2)
		{
			if(out1[0]!=PP60_CIPHERRSP_HEAD||out1[1]%2!=0)
				return PP60_ERR_CMD_BASE+1+out1[0];
			else
			{
				if((outlen2=PP60_decode(out1+2, outlen1-2, out))==FAIL)
					return PP60_ERR_DECODE;
				*outlen = outlen2;
				return NL829_PP60_SUCC;
			}
		}
		else
			return PP60_ERR_CMD_BASE;
	}
}

static ushort PP60_cmd_h(uchar yy,uchar *in, int inlen)
{
	/*private & local definition*/
	ushort ret = 0, outlen = 0;
	uchar data[PP60_MAXLEN_DATA] = {0}, out[PP60_MAXLEN_DATA] = {0};
	
	/*process body*/
	//data��ָESC+cmd��0D 0A֮�������
	data[0] = yy;
	memcpy(data+1, in, inlen);
	if((ret=PP60_frame_factory('h', data, 1+inlen, PP60_CMDh_RCVLEN, out, &outlen))!=NL829_PP60_SUCC)
		return ret;
	if(outlen==PP60_CMDh_RCVLEN)//Ԥ�ڳ����Ƿ���ʵ�ʳ���һ��
	{	
		//����PP60����ֵ����1byte��,��829��2bytes��,Ϊ��ͳһ����,��PP60���ؽ���ת��
		if(out[0]==PP60_SUCC)
			return NL829_PP60_SUCC;
		else if(out[0]==PP60_FAIL)
			return NL829_PP60_FAIL;
		else
			return PP60_ERR_CMD_BASE+1+out[0];//��1������һ��out[0]Ϊ0x00�����
	}
	else
		return PP60_ERR_CMD_BASE;
}

//idֻ������1,2
static ushort __test_PP60_cmd_D(uchar id)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL, ret = NL829_PP60_FAIL;
	
	/*process body*/
	PP60_cmd_C();//����ǰ��
	if((staval=PP60_cmd_D(id, NULL, 0))!=NL829_PP60_SUCC||cls_show_msg("��%d�������ַ���ʾ,��[ENTER];����[����]", id)==ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ	
	if((staval=PP60_cmd_D(id, (uchar *)"\xf0\xf1\xf2", strlen("\xf0\xf1\xf2")))!=NL829_PP60_FAIL||cls_show_msg("��%d�������ַ���ʾ,��[ENTER];����[����]", id)==ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ	
	if((staval=PP60_cmd_D(id, (uchar *)"\x31\xf0\xf1\xf2", strlen("\x31\xf0\xf1\xf2")))!=NL829_PP60_FAIL||cls_show_msg("��%d�������ַ���ʾ,��[ENTER];����[����]", id)==ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_D(id, (uchar *)"\x80\x81\x82\x83\x84\x31\x32\x33\x34", strlen("\x80\x81\x82\x83\x84\x31\x32\x33\x34")))!=NL829_PP60_SUCC||cls_show_msg("��%d������ʾ\"����������1234\",��[ENTER];����[����]", id)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_D(id, (uchar *)"\x80\x81\x82\xBB\xBC\x31\x32\x33\x34", strlen("\x80\x81\x82\xBB\xBC\x31\x32\x33\x34")))!=NL829_PP60_SUCC||cls_show_msg("��%d������ʾ\"������ԤԼ1234\",��[ENTER];����[����]", id)!=ENTER)//�ֿ���������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_D(id, (uchar *)"\x80\x81\x82\x83\x84\x80\x81\x82\x83\x84", strlen("\x80\x81\x82\x83\x84\x80\x81\x82\x83\x84")))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	
	ret = NL829_PP60_SUCC;
ERR:
	PP60_cmd_C();//���Ժ���
	return ret;
}

static ushort test_PP60_cmd_D(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL;
	int i = 1, n = 8;
	
	/*process body*/
	//cls_printf("����D������,�����ĵȴ�...");
	n=(g_LCDSize==1)?8:3;
	for(i=1;i<n;i++)
	{
		if((staval=__test_PP60_cmd_D(i))!=NL829_PP60_SUCC)
			return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_D(n, (uchar *)"\x80\x81\x82\x83\x84\x31\x32\x33\x34", strlen("\x80\x81\x82\x83\x84\x31\x32\x33\x34")))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_O(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0};
	ushort staval = NL829_PP60_FAIL, outlen = 0;
	
	/*process body*/
	cls_printf("����PED�ϵ�2������%s��[ENTER],�����ĵȴ�...", "1234");//������cls_show_msg������������,ʹ��cls_printf����
	memset(out, 0, sizeof(out));//�Ӳ��ø���outlen���ض�
	if((staval=PP60_cmd_O(out, &outlen))!=NL829_PP60_SUCC||memcmp(out, "1234", outlen))
	{
		//DEBUG_HEX(out, outlen, NULL);//����02 04 31 32 33 34
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	cls_printf("����PED���Ȱ�[ENTER]����,�ٰ�[ESC]...");
	
	if(g_LCDSize==1)
		memset(out, WHITE, sizeof(out));//�����豸0Ϊ��ɫ,���԰���Ҫ�޸ĳ�WHITE
	else
		memset(out, 0, sizeof(out));//�Ӳ��ø���outlen���ض�
	if((staval=PP60_cmd_O(out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_P(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL;
	
	/*process body*/
	//�쳣
	if((staval=PP60_cmd_P((uchar *)"\x90\x91\x8c\x8d\xa8\xa9\xaa\xac\xb5", strlen("\x90\x91\x8c\x8d\xa8\xa9\xaa\xac\xb5")))!=NL829_PP60_FAIL)//��Ϊ"��������С��ͨ����"
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	if((staval=PP60_cmd_P((uchar *)"\x90\xf1\xfc\xfd", strlen("\x90\xf1\xfc\xfd")))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}

	cls_printf("����PED�ϰ�[ENTER]...");
	if((staval=PP60_cmd_P(NULL, 0))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	cls_printf("����PED�ϰ�[ESC]...");
	if((staval=PP60_cmd_P(NULL, 0))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}

	//����
	cls_printf("PED��2����ʾ\"��������\",����PED�ϰ�[ENTER]...");
	if((staval=PP60_cmd_P((uchar *)"\x90\x91\x8c\x8d", strlen("\x90\x91\x8c\x8d")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	cls_printf("PED��2����ʾ\"��������\",����PED�ϰ�[ESC]...");
	if((staval=PP60_cmd_P((uchar *)"\x90\x91\x8c\x8d", strlen("\x90\x91\x8c\x8d")))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}

	cls_printf("PED��2����ʾ\"��������С��ͨ\",����PED�ϰ�[ENTER]...");
	if((staval=PP60_cmd_P((uchar *)"\x90\x91\x8c\x8d\xa8\xa9\xaa", strlen("\x90\x91\x8c\x8d\xa8\xa9\xaa")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);//????
		return NL829_PP60_FAIL;
	}
	cls_printf("PED��2����ʾ\"��������С��ͨ\",����PED�ϰ�[ESC]...");
	if((staval=PP60_cmd_P((uchar *)"\x90\x91\x8c\x8d\xa8\xa9\xaa", strlen("\x90\x91\x8c\x8d\xa8\xa9\xaa")))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	

	return NL829_PP60_SUCC;
}

//����˼·:�ȵ���W����������LOGO,�ٵ���Z��������ʾЧ������֤W����
//����W��ʵ��,��һ�����سɹ�,��Ļ�ͻ����LOGO��ʾ,�ʷ��Z���cls_show_msg����ȷ�ϵĲ���
static ushort test_PP60_cmd_W(void)
{
	/*private & local definition*/
	//�й�����//244bytesԭʼ����
	uchar logo[] = {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0X08,0X08,0X08,0X08,0XFF,0X08,
				0X08,0X08,0X08,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,
				0X40,0X48,0X49,0X49,0X49,0X4F,0X49,0X49,0X49,0X48,0X40,0X7F,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X0C,0X34,0XE7,
				0X24,0X24,0X00,0X7F,0X49,0X49,0X49,0X49,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X40,0X40,0X7F,0X48,0X48,0X7F,0X40,0X08,0X88,0X68,0X0F,0X08,0X28,0XC8,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0X40,0X40,0X40,
				0X40,0XFF,0X40,0X40,0X40,0X40,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0XFF,0X02,0X12,0X12,0X12,0X12,0XF2,0X12,0X52,0X32,0X12,0X02,0XFF,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,
				0X80,0X80,0XFE,0X84,0X88,0X80,0XFF,0X02,0XC4,0X30,0X18,0X24,0X46,0X04,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X06,0XFC,0X88,0X88,0XFF,0X09,0X82,0X8C,0XB0,0XC0,
				0XB0,0X8C,0X83,0X82,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0XFF};//���һ��0XFF�Ǵ����,����ʹ��ʱҪ����ȥ��
	uchar out[PP60_MAXLEN_DATA] = {0}, tmp[244] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, ret = NL829_PP60_FAIL;
	int outlen = 0;
	
	/*process body*/
	//����ǰ��
  	/*if(g_LCDSize==1)//20170401�����������Ѿ��޸�SP10��SP10-T�İ�ɫͳһΪ0
		memset(tmp, WHITE, sizeof(tmp));
	else*/
		memset(tmp, 0, sizeof(tmp));
	outlen = PP60_code(tmp, sizeof(tmp), out);
#if TEST
	if((staval=PP60_cmd_W(0, 0, out, outlen))!=NL829_PP60_SUCC||(staval1=PP60_cmd_W(0, 1, out, outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//cls_show_msg("�������PP60����");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("1.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
#endif
	
	outlen = PP60_code(logo, sizeof(logo), out);
#if 0	//��λ������ʧ�ܵ�BUG��
	if((staval=PP60_cmd_W(0, 0, out, (outlen-2)/2))!=NL829_PP60_SUCC)//ֻ���ϰ�������,���豸�Ի�ƽ���ֳ����°�������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//cls_show_msg("�����������...");
	if((staval=PP60_cmd_W(0, 1, out, (outlen-2)/2))!=NL829_PP60_SUCC)//ֻ���ϰ�������,���豸�Ի�ƽ���ֳ����°�������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg("����������͵�1������\n");
	show_msg("ret:%04x", PP60_cmd_W(0, 0, out, (outlen-2)/2));
	cls_show_msg("����������͵�2������\n");
	show_msg("ret:%04x", PP60_cmd_W(0, 1, out, (outlen-2)/2));
	while(1);
#endif
	if((staval=PP60_cmd_W(122, 0, out, outlen-2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//cls_show_msg("�������PP60����");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("2.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_W(0, 2, out, outlen-2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//cls_show_msg("�������PP60����");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("3.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
#if TEST
	if((staval=PP60_cmd_W(0, 0, NULL, 0))!=NL829_PP60_FAIL||(staval1=PP60_cmd_W(0, 1, NULL, 0))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
#endif
	if((staval=PP60_cmd_W(0, 0, out, outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_W(0, 1, out, outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//cls_show_msg("�������PP60����");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("4.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_W(0, 0, out, (outlen-2)/2+1))!=NL829_PP60_FAIL||(staval1=PP60_cmd_W(0, 1, out, (outlen-2)/2+1))!=NL829_PP60_FAIL)//������Ϊ����,�޷��ֳ����°��оͱ���
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//cls_show_msg("�������PP60����");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("5.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

#if TEST	
	//���������е�Ȩ�޲���
	if((staval=PP60_cmd_W(0, 1, out, (outlen-2)/2))!=NL829_PP60_FAIL||(staval1=PP60_cmd_W(0, 0, out, (outlen-2)/2))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	if((staval=PP60_cmd_W(0, 0, out, (outlen-2)/2))!=NL829_PP60_SUCC||(staval1=PP60_cmd_W(0, 1, out, (outlen-2)/2))!=NL829_PP60_SUCC)//ֻ���ϰ�������,���豸�Ի�ƽ���ֳ����°�������
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	if((staval1=PP60_cmd_W(0, 1, out, (outlen-2)/2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval1);
		goto ERR;
	}
	//cls_show_msg("�������PP60����");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("6.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//end of ���������е�Ȩ�޲���
	//cls_show_msg("����PC����...");//���ڵ���ʱ�Ӵ��ڻ����������
	if((staval=PP60_cmd_W(60, 0, out, outlen-2))!=NL829_PP60_SUCC||(staval1=PP60_cmd_W(60, 1, out, outlen-2))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//cls_show_msg("�������PP60��ʾ�Ұ���");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("��PP60��ʾ�Ұ�����[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
#endif
	if((staval=PP60_cmd_W(0, 0, out, outlen-2))!=NL829_PP60_SUCC||(staval1=PP60_cmd_W(0, 1, out, outlen-2))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//cls_show_msg("�������PP60�����о���ʾ\"�й�����\"");
	if(/*(staval=PP60_cmd_Z())!=NL829_PP60_SUCC||*/cls_show_msg("��PP60�����о���ʾ\"�й�����\"��[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	ret = NL829_PP60_SUCC;
ERR:
	//���Ժ���
	outlen = PP60_code(tmp, sizeof(tmp), out);	
	PP60_cmd_W(0, 0, out, outlen);
	PP60_cmd_Z();
	return ret;
}

static ushort test_PP60_cmd_x(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0;
	
	/*process body*/
	if((staval=PP60_cmd_x(0, NULL, 0, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
#if TEST
	if((staval=PP60_cmd_x(2, (uchar *)"\x41\x42", strlen("\x41\x42"), out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
#endif

	memset(out, 0, sizeof(out));
	if((staval=PP60_cmd_x(2, NULL, 0, out, &outlen))==NL829_PP60_FAIL)//δ���ع�sn
	{
		if((staval=PP60_cmd_x(1, NULL, 0, NULL, NULL))!=NL829_PP60_FAIL)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return NL829_PP60_FAIL;
		}
		if((staval=PP60_cmd_x(1, (uchar *)"\x41", strlen("\x41"), NULL, NULL))!=NL829_PP60_FAIL||(staval1=PP60_cmd_x(1, (uchar *)"\x41\x42\x43", strlen("\x41\x42\x43"), NULL, NULL))!=NL829_PP60_FAIL)
		{
			cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
			return NL829_PP60_FAIL;
		}
		outlen = PP60_code((uchar *)"abcdefghijklmnopqrstuvwxy", strlen("abcdefghijklmnopqrstuvwxy"), out);
		if((staval=PP60_cmd_x(1, out, outlen, NULL, NULL))!=NL829_PP60_FAIL)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return NL829_PP60_FAIL;
		}
		cls_show_msg1(2, "δ���ع�SN,��������SN��...");
		outlen = PP60_code((uchar *)PP60_SN, strlen(PP60_SN), out);
		if((staval=PP60_cmd_x(1, out, outlen, NULL, NULL))!=NL829_PP60_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return NL829_PP60_FAIL;
		}
		if((staval=PP60_cmd_x(1, out, outlen, NULL, NULL))!=NL829_PP60_FAIL)//���³ɹ���,����һ��Ӧʧ��
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			return NL829_PP60_FAIL;
		}
		memset(out, 0, sizeof(out));
		if((staval=PP60_cmd_x(2, NULL, 0, out, &outlen))!=NL829_PP60_SUCC||outlen!=strlen(PP60_SN))//�ٻ�ȡ����У��
		{
			cls_show_msg("line %d:����ʧ��(%04x,%d)", __LINE__, staval, outlen);
			return NL829_PP60_FAIL;
		}
		else
		{
			if(memcmp(out, PP60_SN, outlen))
			{
				cls_show_msg("line %d:����ʧ��", __LINE__);
				return NL829_PP60_FAIL;
			}
		}
	}
	else if(outlen==0)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		return NL829_PP60_FAIL;	
	}

	//�����ع�sn
	if(cls_show_msg("��ȡ��SN:%s,��ȷ��[ENTER],����[����]", out)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}

	if((staval=PP60_cmd_x(2, (uchar *)"\x41\x42", strlen("\x41\x42"), out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	
	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_h(void)
{
	/*private & local definition*/
 	uchar HZtest[] = {
 						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00,
						0x08, 0xc8, 0x38, 0xcf, 0x08, 0x28, 0x90, 0xa8, 
						0x08, 0x19, 0x09, 0xff, 0x05, 0x04, 0xff, 0x4a, 
						0xa4, 0xa3, 0x24, 0x28, 0x10, 0xb0, 0x10, 0x00, 
						0x8a, 0x7f, 0x00, 0x5f, 0x80, 0x7f, 0x00, 0x00
					 };
	uchar out[PP60_MAXLEN_DATA] = {0}, tmp[244] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, ret = NL829_PP60_FAIL;
	int i = 0, outlen = 480, cnt1 = 0, cnt2 = 0, n = 8;

	/*process body*/
	//����ǰ��
	n=(g_LCDSize==1)?8:2;
	for (i = 0; i<n; i++)
	{
		if((staval=PP60_cmd_h(i, NULL, 0))!=NL829_PP60_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%d,%04x)", __LINE__, i, staval);
			goto ERR;
		}
	}
	if(cls_show_msg("7.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//�������
	memset(out, 0x31, sizeof(out));
	if((staval=PP60_cmd_h(0, out, outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}	
	PP60_code(HZtest, sizeof(HZtest), out);
	if((staval=PP60_cmd_h(n, out, outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_h(0, out, 1+outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}	
	if((staval=PP60_cmd_h(0, out, 2+outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	for (i = 0; i<n; i++)
	{
		if((staval=PP60_cmd_h(i, out, cnt1=((rand()%outlen)|1)))!=NL829_PP60_FAIL)
		{
			cls_show_msg("line %d:����ʧ��(%d,%04x)", __LINE__, i, staval);
			goto ERR;
		}	
	}
	if(cls_show_msg("8.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//����ʹ��
	for (i = 0; i<n; i++)
	{
		if((staval=PP60_cmd_h(i, out, outlen))!=NL829_PP60_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%d,%04x)", __LINE__, i, staval);
			goto ERR;
		}	
	}
	if(cls_show_msg("��PP60д��%d������[ENTER],����[����]",(g_LCDSize==1)?(n-1):n)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	for (i = 0; i<n; i++)
	{
		if((staval=PP60_cmd_h(i, NULL, 0))!=NL829_PP60_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%d,%04x)", __LINE__, i, staval);
			goto ERR;
		}	
	}
	if(cls_show_msg("9.��PP60������[ENTER],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	memset(tmp, 0x00, sizeof(tmp));
	for (i = 0; i<n; i++)
	{
		cnt2 = (rand()%6+1)*ONEHZSIZE;
		PP60_code(HZtest, sizeof(HZtest), out);
		if((staval=PP60_cmd_h(i, out, cnt2))!=NL829_PP60_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%04x)%d,%04x", __LINE__, staval, i, cnt2);
			goto ERR;
		}
		if(cls_show_msg("��PP60(��%d��)��%d������,��[ENTER],����[����]", (i==0)?2:i, cnt2/ONEHZSIZE)!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
			goto ERR;
		}
		PP60_code(tmp, sizeof(tmp), out);
		if((staval=PP60_cmd_h(i, out, outlen))!=NL829_PP60_SUCC)
		{
			cls_show_msg("line %d:����ʧ��(%d)%04x", __LINE__, i, staval);
			goto ERR;
		}
	}
	ret = NL829_PP60_SUCC;
ERR:
	//���Ժ���
	PP60_cmd_Z();
	return ret;
}

static ushort test_PP60_cmd_R(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0};	
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0;
	
	/*process body*/
	cls_printf("��λ��Կ����,�����ĵȴ�...");
	if((staval=PP60_cmd_R((uchar *)"\x41", strlen("\x41")))!=NL829_PP60_FAIL||(staval1=PP60_cmd_R((uchar *)"\x41\x41\x41", strlen("\x41\x41\x41")))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	outlen = PP60_code((uchar *)"\xa8", 1, out);//168
	if((staval=PP60_cmd_R(out, outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	//���õ�0,167����,����Կ
	if((staval=PP60_cmd_R((uchar *)"\x41\x41", strlen("\x41\x41")))!=NL829_PP60_SUCC
	||(staval1=PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	
	return NL829_PP60_SUCC;
}

//DES��SM4�¶�8�ֽ�/16�ֽ�����������������/���ܼ���
static int AlgCompute(const int Alg_Type, uchar *data_in, int data_len, uchar *data_out, uchar *pkey, int key_len, int nMode)
{
	int j=0, num=0, ret=0;
	if( (Alg_Type!=PP60_TDESFLAG&&Alg_Type!=PP60_SM4FLAG) || data_in==NULL || pkey==NULL)
		return NDK_ERR;
	if(Alg_Type == PP60_TDESFLAG)
	{	
		num=data_len/8;
		for(;j<num;j++)
		{
			if(NDK_AlgTDes(data_in+8*j, data_out+8*j, pkey, key_len, nMode) != NDK_OK)
			{
				cls_show_msg("line %d:����ʧ��", __LINE__);
				return NDK_ERR;
			}
		}
	}
#if ALG_SM_ENABLE
	else
	{
		num=data_len/16;
		for(;j<num;j++)
		{			
			if((ret=NDK_AlgSM4Compute(pkey, NULL, data_len, data_in+16*j, data_out+16*j, nMode))!= NDK_OK)
			{
				cls_show_msg("line %d:����ʧ�� %d ",  __LINE__,ret);		
				return NDK_ERR;
			}
		}
	}
#endif	
	return NDK_OK;
}

//�������ɹ�ִ�е�Ч��:00��mkey��Ϊȫ0x42, ukey(00)��Ϊȫ0x38, ukey(01)��Ϊȫ0x40;168�鲻���޸�(ʹ��Ĭ��,mkeyΪȫ0x38,ukey(x)Ϊȫ0x00)
static ushort test_PP60_cmd_MSU(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0}, out2[PP60_MAXLEN_DATA] = {0}, OK[16] = {0}, NK[16] = {0}, tmp[16] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0, outlen1 = 0, outlen2 = 0;
	
	/*process body*/
	cls_printf("������Կ��,�����ĵȴ�...");//��ʱ�ϳ��Ĳ���(��,����829ת��,ʱ��ͻ�ӳ�)������ʾ,�ܿ���ɵļ���Ҫ�ֹ�ȷ�ϵĲ����ɲ���ʾ
	
	//test M
	//ǰ��
	memset(OK, 0x38, sizeof(OK));
	memset(NK, 0x42/*0x39��0x38��Ϊ��һ����key*/, sizeof(NK));
	memset(tmp, 0x40, sizeof(tmp));
	outlen = PP60_code(OK, sizeof(OK), out);
	outlen1= PP60_code(NK, sizeof(NK), out1);
	outlen2= PP60_code(tmp, sizeof(tmp), out2);
	
	if((staval=PP60_cmd_M(0xa8, out, outlen, out1, outlen1))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_M(0x00, NULL, 0, NULL, 0))!=NL829_PP60_FAIL||(staval1=PP60_cmd_M(0x00, OK, sizeof(OK), NK, sizeof(NK)))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_M(0x00, out1, outlen1, out, outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_M(0x00, out, outlen, out1, outlen1-2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	if((staval=PP60_cmd_M(0x00, out, outlen, out1, outlen1))!=NL829_PP60_SUCC)//00��mkey��Ϊȫ0x42
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}

	//test U
#if TEST
	if((staval=PP60_cmd_U(0xa8, 0x00, out1, outlen1, out, outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_U(0x00, 0x02, out1, outlen1, out, outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#endif
	if((staval=PP60_cmd_U(0x00, 0x00, NULL, 0, NULL, 0))!=NL829_PP60_FAIL||(staval1=PP60_cmd_U(0x00, 0x00, NK, sizeof(NK), OK, sizeof(OK)))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_U(0x00, 0x00, out, outlen, out1, outlen1))!=NL829_PP60_FAIL||(staval1=PP60_cmd_U(0x00, 0x01, out, outlen, out2, outlen2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_U(0x00, 0x00, out1, outlen1, out, outlen-2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_U(0x00, 0x00, out1, outlen1, out, outlen))!=NL829_PP60_SUCC)//00��ukey(00)��Ϊȫ0x38
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
#if 0	//��ʱ����for test S,����Ч����test S��ͬ
	if((staval=PP60_cmd_U(0x00, 0x01, out1, outlen1, out2, outlen2))!=NL829_PP60_SUCC)//00��ukey(01)��Ϊȫ0x40
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	return NL829_PP60_SUCC;
#endif
	
	//test S
	//ǰ��
	//if(mdes3(ENCRYPT, tmp, OK, NK)!=SUCC||mdes3(ENCRYPT, tmp+8, OK+8, NK)!=SUCC)//OK��������,�������м仺��
	//if(NDK_AlgTDes(tmp, OK, NK, sizeof(NK), ALG_TDS_MODE_ENC)!=NDK_OK||NDK_AlgTDes(tmp+8, OK+8, NK, sizeof(NK), ALG_TDS_MODE_ENC)!=NDK_OK)//OK��������,�������м仺��
	if(AlgCompute(g_algtype, tmp, sizeof(tmp), OK, NK, sizeof(NK), ((g_algtype==PP60_TDESFLAG)?ALG_TDS_MODE_ENC:ALG_SM4_ENCRYPT_ECB) )!=NDK_OK)//OK��������,�������м仺��
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		return NL829_PP60_FAIL;
	}
	outlen2 = PP60_code(OK, sizeof(OK), out2);

	if((staval=PP60_cmd_A(0x00, 0x01, g_algtype))!=NL829_PP60_SUCC)//��S�������֮ǰ���輤�����Ӧ���㷨����
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}//����Ѹü�����ŵ�Mָ��ǰ,��Sָ���ʧ��,ԭ����:����ϼ������SM4��Կ,�ھ���Mָ���޸ĺ󲻻ᱻʹ��,��Ҫ���¼���Żᱻʹ�� 20160309 zhenglw����
#if TEST
	if((staval=PP60_cmd_S(0xa8, 0x00, out2, outlen2))!=NL829_PP60_FAIL||(staval1=PP60_cmd_S(0x00, 0x02, out2, outlen2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_S(0x00, 0x01, NULL, 0))!=NL829_PP60_FAIL||(staval1=PP60_cmd_S(0x00, 0x01, OK, sizeof(OK)))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#endif
	if((staval=PP60_cmd_S(0x00, 0x01, out2, outlen2-2))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	if((staval=PP60_cmd_S(0x00, 0x01, out2, outlen2))!=NL829_PP60_SUCC)//00��ukey(01)��Ϊȫ0x40
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	
	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_K(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, str[PP60_CMDK_SM4_STRLEN] = {0}/*str��ʵֻҪ8�ֽڼ���,����������Щ*/, STR[PP60_CMDK_SM4_STRLEN*2] = {0}, key[16] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0;
	int STRlen = 0, data_len = 0;
	
	/*process body*/
	//ǰ��
	data_len=(g_algtype==PP60_TDESFLAG)?PP60_CMDK_STRLEN:PP60_CMDK_SM4_STRLEN;
	memset(key, 0x38, sizeof(key));
	memset(str, 0x31, data_len);
	STRlen = PP60_code(str, data_len, STR);

	if((staval=PP60_cmd_K(0xa8, STR, data_len, out, &outlen) )!=NL829_PP60_FAIL||(staval1=PP60_cmd_K(0xa7, STR, STRlen, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#if TEST
	if((staval=PP60_cmd_K(0xa7, NULL, 0, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_K(0xa7, str, data_len, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#endif
	if((staval=PP60_cmd_K(0xa7, STR, data_len-1, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_K(0xa7, STR, data_len+1, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_K(0xa7, STR, data_len-2, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_K(0xa7, STR, data_len+2, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_K(0xa7, STR, data_len, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		//DEBUG_HEX(out, outlen, NULL);
		if((STRlen=PP60_decode(out, outlen, STR))!=data_len/2)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, STRlen);
			return NL829_PP60_FAIL;			
		}
		if(AlgCompute(g_algtype, str, data_len/2, out, key, sizeof(key), ((g_algtype==PP60_TDESFLAG)?ALG_TDS_MODE_DEC:ALG_SM4_DECRYPT_ECB) ) != NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return NL829_PP60_FAIL;
		}
		if(memcmp(out, STR, STRlen))
		{
			//DEBUG_HEX(out, STRlen, NULL);
			//DEBUG_HEX(STR, STRlen, NULL);
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return NL829_PP60_FAIL;
		}
	}

	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_T(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0}, D[700] = {0}, data[350] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0;
	int Dlen = 0, i=-1;
	//���½��������PC������Ԥ�������
	DATA_RESULT result[ ]={{8,(uchar *)"\xFB\x04\xBE\xE7\x8F\x3A\x38\x53"}, //0 ʹ�÷���1  ����Ϊ21���ֽڵ�0x31 ��ԿΪ8���ֽڵ�0x00
						   {8,(uchar *)"\x62\x2E\x2C\xD5\x2C\x3F\xDC\x83"}, //1 ʹ�÷���2  ����Ϊ21���ֽڵ�0x31 ��ԿΪ8���ֽڵ�0x00
						   {8,(uchar *)"\x8C\xA6\x4D\xE9\xC1\xB1\x23\xA7"}, //2 ʹ�÷���1  ����Ϊ8���ֽڵ�0x00 ��ԿΪ8���ֽڵ�0x00
						   {8,(uchar *)"\x8C\xA6\x4D\xE9\xC1\xB1\x23\xA7"}, //3 ʹ�÷���2  ����Ϊ8���ֽڵ�0x00 ��ԿΪ8���ֽڵ�0x00
						   {8,(uchar *)"\xDC\x64\xBE\x0D\x5B\x0E\x71\x43"}, //4 ʹ�÷���1  ����Ϊ250���ֽڵ�0x31 ��ԿΪ8���ֽڵ�0x00
						   {8,(uchar *)"\x14\x40\x0F\x74\xED\xAD\x49\x59"}, //5 ʹ�÷���2  ����Ϊ250���ֽڵ�0x31 ��ԿΪ8���ֽڵ�0x00
						   {16,(uchar *)"\xC7\xCE\x1F\x68\xD5\xC8\x4A\xF3\xB4\xBD\x6D\xDD\x07\x0C\x97\x37"}, //0+6 ʹ�÷���1  ����Ϊ21���ֽڵ�0x31 ��ԿΪ16���ֽڵ�0x00
						   {16,(uchar *)"\x4C\xC0\xBB\xAE\x84\x0D\xE2\x16\x97\x84\x15\x9A\x31\xB8\x0F\x6F"}, //1+6 ʹ�÷���2  ����Ϊ21���ֽڵ�0x31 ��ԿΪ16���ֽڵ�0x00
						   {16,(uchar *)"\x9F\x1F\x7B\xFF\x6F\x55\x11\x38\x4D\x94\x30\x53\x1E\x53\x8F\xD3"}, //2+6 ʹ�÷���1  ����Ϊ16���ֽڵ�0x00 ��ԿΪ16���ֽڵ�0x00
						   {16,(uchar *)"\x9F\x1F\x7B\xFF\x6F\x55\x11\x38\x4D\x94\x30\x53\x1E\x53\x8F\xD3"}, //3+6 ʹ�÷���2  ����Ϊ16���ֽڵ�0x00 ��ԿΪ16���ֽڵ�0x00
						   {16,(uchar *)"\x4C\xC1\xAE\x75\xA2\xCD\x61\x95\x63\x71\x37\x04\xC2\x05\xE8\x32"}, //4+6 ʹ�÷���1  ����Ϊ250���ֽڵ�0x31 ��ԿΪ16���ֽڵ�0x00
						   {16,(uchar *)"\xAD\x80\x65\xFC\xCF\x8B\x26\x40\x41\x9D\xD6\x1C\x88\x22\x03\x6F"}  //5+6 ʹ�÷���2  ����Ϊ250���ֽڵ�0x31 ��ԿΪ16���ֽڵ�0x00
						  };
	
	/*process body*/
	cls_printf("����T������,�����ĵȴ�...");
	//ǰ��
	i=(g_algtype==PP60_TDESFLAG)?0:1;//�����㷨ѡ����֤����
	memset(data, 0x31, sizeof(data));
	Dlen = PP60_code(data, sizeof(data), D);

#if TEST
	if((staval=PP60_cmd_T(0x04, D, 8, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	if(g_PP60type!=DEVICE_PP60RF&&(staval=PP60_cmd_T(0x00, D, Dlen, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	if((staval=PP60_cmd_T(0x00, data, sizeof(data), out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_T(0x01, data, sizeof(data), out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	//mac�㷨����û�����ݵ�(��Ϊ�г�ʼ����)
	if((staval=PP60_cmd_T(0x00, NULL, 0, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[2+i*6].len ||memcmp(out1, result[2+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}
#endif
	if((staval=PP60_cmd_T(0x01, NULL, 0, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[3+i*6].len ||memcmp(out1, result[3+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}
	//����һ��,�����Ӧ�ı�
#if TEST
	if((staval=PP60_cmd_T(0x00, NULL, 0, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[2+i*6].len ||memcmp(out1, result[2+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}
#endif
	if((staval=PP60_cmd_T(0x01, NULL, 0, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}	
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[3+i*6].len ||memcmp(out1, result[3+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}
	
	if((staval=PP60_cmd_T(0x00, D, 21*2-1, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_T(0x01, D, 21*2+1, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	if((staval=PP60_cmd_T(0x00, D, 21*2, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[i*6].len ||memcmp(out1, result[i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}
	if((staval=PP60_cmd_T(0x01, D, 21*2, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[1+i*6].len ||memcmp(out1, result[1+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}

	//��Կ����ȡ250���˴�Ϊ����֤��Կ������150-300֮�䱨�������
       if((staval=PP60_cmd_T(0x00, D, 250*2, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[4+i*6].len ||memcmp(out1, result[4+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	}
	if((staval=PP60_cmd_T(0x01, D, 250*2, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		if((Dlen=PP60_decode(out, outlen, out1))!=result[5+i*6].len ||memcmp(out1, result[5+i*6].data, Dlen))
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, Dlen);
			return NL829_PP60_FAIL;
		}
	} 

	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_H(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, str[PP60_SM4_CMDH_LS] = {0}/*str��ʵֻҪ8�ֽڼ���,����������Щ*/, STR[PP60_SM4_CMDH_LS*2] = {0}, key[16] = {0}, tmp[16] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0;
	int STRlen = 0, tmplen = 0, data_len = 0;
	
	/*process body*/
	cls_printf("����H������,�����ĵȴ�...");
	//ǰ��
	data_len=(g_algtype==PP60_TDESFLAG)?PP60_CMDH_LS:PP60_SM4_CMDH_LS;
	memset(key, 0, sizeof(key));
	memset(str, 0x31, data_len);
	STRlen = PP60_code(str, data_len, STR);

	if((staval=PP60_cmd_H(3, STR, data_len, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
#if TEST
	if((staval=PP60_cmd_H(0, NULL, 0, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_H(1, str, data_len, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#endif
	if((staval=PP60_cmd_H(0, STR, data_len-1, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_H(1, STR, data_len+1, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#if TEST
	if((staval=PP60_cmd_H(0, STR, data_len-2, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_H(1, STR, data_len+2, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
#endif
	if((staval=PP60_cmd_H(0, STR, data_len, out, &outlen))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		//DEBUG_HEX(out, outlen, NULL);
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len/2)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			return NL829_PP60_FAIL;
		}
		//if(NDK_AlgTDes(str, out, key, sizeof(key), ALG_TDS_MODE_ENC)!=NDK_OK)//if(mdes3(ENCRYPT, str, out, key)!=SUCC)
		if(AlgCompute(g_algtype, str, data_len/2 , out, key, sizeof(key), ((g_algtype==PP60_TDESFLAG)?ALG_TDS_MODE_ENC:ALG_SM4_ENCRYPT_ECB) ) != NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return NL829_PP60_FAIL;
		}		
		if(memcmp(out, tmp, tmplen))
		{
			//DEBUG_HEX(out, tmplen, NULL);
			//DEBUG_HEX(tmp, tmplen, NULL);
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return NL829_PP60_FAIL;
		}
	}
	if((staval=PP60_cmd_H(1, STR, data_len, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return NL829_PP60_FAIL;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len/2)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			return NL829_PP60_FAIL;
		}
		if(AlgCompute(g_algtype, str, data_len/2, out, key, sizeof(key), ((g_algtype==PP60_TDESFLAG)?ALG_TDS_MODE_ENC:ALG_SM4_ENCRYPT_ECB) ) != NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return NL829_PP60_FAIL;
		}		
		if(memcmp(out, tmp, tmplen))
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return NL829_PP60_FAIL;
		}
	}

	return NL829_PP60_SUCC;
}

static ushort test_PP60_cmd_LN(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL;
	
	/*process body*/
	//��������L��Nʱ������L<N��ʵ�����,�ŵ��쳣��������
	//ע��:������볤��Ҫ����0,��С���볤�������Ϊ0
#if TEST
	if((staval=PP60_cmd_N(0x1e))!=NL829_PP60_SUCC||(staval1=PP60_cmd_L(0x01))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	if((staval1=PP60_cmd_N(0x00))!=NL829_PP60_SUCC||(staval=PP60_cmd_L(0x00))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
#endif
	if((staval=PP60_cmd_N(0x1e))!=NL829_PP60_SUCC||(staval1=PP60_cmd_L(0x1e))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	if((staval=PP60_cmd_N(0x1f))!=NL829_PP60_FAIL||(staval1=PP60_cmd_L(0x1f))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}

	return NL829_PP60_SUCC;
ERR://�ָ�Ĭ������
	PP60_cmd_N(0x04);
	PP60_cmd_L(0x10);
	return NL829_PP60_FAIL;
}

static ushort test_PP60_cmd_F(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, tmp[32] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0, ret = NL829_PP60_FAIL;
	int tmplen = 0, i=0;
	DATA_RESULT result[ ]={{16,(uchar *)"\x04\x16\x19\xB0\x79\x4C\xF5\x60\x5C\x60\xEE\xEF\x2D\x98\xE8\x41"}, // 0 ����Ϊ13���ֽڵ�0x31+3���ֽڵ�0x00 keyΪ8�ֽڵ�0X38 
						   {8 ,(uchar *)"\x1C\x6D\xDA\x6E\x97\xDC\xC7\xD3"}, // 1 ����Ϊ6���ֽڵ�0x31+10���ֽڵ�0x00 keyΪ8�ֽڵ�0X38
						   {24,(uchar *)"\x04\x16\x19\xB0\x79\x4C\xF5\x60\x04\x16\x19\xB0\x79\x4C\xF5\x60\xA9\x94\x82\x48\xB4\x19\x48\xEB"}, // 2 ����Ϊ23���ֽڵ�0x31+9���ֽڵ�0x00 keyΪ16�ֽڵ�0X38 
						   {16,(uchar *)"\x14\xFD\x55\xAD\x41\xEB\x3D\x89\x79\x5E\x6A\xCD\x0B\x68\x91\x03"}, // 0+3 ����Ϊ13���ֽڵ�0x31+3���ֽڵ�0x00 keyΪ16�ֽڵ�0X38 
						   {16,(uchar *)"\xAE\xD8\x5B\xE2\x38\x73\x19\xE6\x14\x9E\x10\xF0\xD0\x31\x54\x08"}, // 1+3 ����Ϊ6���ֽڵ�0x31+10���ֽڵ�0x00 keyΪ16�ֽڵ�0X38 
						   {32,(uchar *)"\xAF\x93\xE6\x2F\x52\xCB\xE9\x73\x4C\x1B\x70\x35\x00\x45\x34\x07\x23\x89\xD6\x6A\x4A\x7D\x8A\x1D\x0A\x7B\x55\x9A\xD0\x8D\x7E\xCC"}  // 2+3 ����Ϊ23���ֽڵ�0x31+9���ֽڵ�0x00 keyΪ16�ֽڵ�0X38 
						  };

	/*process body*/
	//ǰ��
	PP60_cmd_C();
	i=(g_algtype==PP60_TDESFLAG)?0:1;
	
	//�쳣
	if((staval=PP60_cmd_F(6, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//����
	//ǰ��
	if((staval=PP60_cmd_L(0x0d))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x0d))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	cls_printf("����PP60������13��1+[ENTER],��ʾ\"����\"+*+����");//�豸��13��1���м���ʱ,������8�������ȵĻᲹ0x00
	if((staval=PP60_cmd_F(0, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[3*i].len||memcmp(result[3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	//ǰ��
	if((staval=PP60_cmd_L(0x17))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x17))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	cls_printf("����PP60������23��1+[ENTER],��ʾ\"������\"+*+����");
	if((staval=PP60_cmd_F(1, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[2+3*i].len||memcmp(result[2+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	//ǰ��
	if((staval=PP60_cmd_L(0x06))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x06))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}	
	cls_printf("����PP60������6��2�ٸ�Ϊ6��1+[ENTER],��ʾ\"����\"+����+����");
	if((staval=PP60_cmd_F(2, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[1+3*i].len||memcmp(result[1+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+����+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	cls_printf("����PP60������6��1+[ENTER],��ʾ\"������\"+����+����");
	if((staval=PP60_cmd_F(3, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[1+3*i].len||memcmp(result[1+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+����+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������6��1+[ENTER],��ʾ\"����\"+*+������");
	if((staval=PP60_cmd_F(4, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[1+3*i].len||memcmp(result[1+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+������,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	cls_printf("����PP60������6��1+[ENTER],��ʾ\"����\"+����+������");
	if((staval=PP60_cmd_F(5, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[1+3*i].len||memcmp(result[1+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+����+������,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	ret = NL829_PP60_SUCC;
ERR:
	PP60_cmd_C();
	return ret;
}

static ushort test_PP60_cmd_E(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, tmp[16] = {0}, tmp1[32] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0, ret = NL829_PP60_FAIL;
	int tmplen = 0, i = 0;
	DATA_RESULT result[ ]={{8 ,(uchar *)"\x90\xC4\x75\xFC\xA1\xBA\x89\x83"}, // 0 ����Ϊ31 32 33 34 35 36 37 00 keyΪ8�ֽڵ�0X38  
						   {8 ,(uchar *)"\x2D\x07\x18\x5E\xE5\x3A\x44\x48"}, // 1 ����Ϊ31 32 33 34 35 00 00 00 keyΪ8�ֽڵ�0X38 
						   {16,(uchar *)"\x2A\xAB\xAD\xB8\x86\x4A\xDD\x74\x7E\xE6\xEA\x2E\xB7\x7D\x84\x59"}, // 0+2 ����Ϊ31 32 33 34 35 36 37 + 11���ֽڵ�0x00 keyΪ8�ֽڵ�0X38  
						   {16,(uchar *)"\x30\xE9\x98\x4A\x43\x68\x73\xE3\x52\x3D\x4E\x28\x26\x69\xFD\xDD"}  // 1+2 ����Ϊ31 32 33 34 35 + 11���ֽڵ�0x00 keyΪ8�ֽڵ�0X38 
						  };
	
	/*process body*/
	//ǰ��
	PP60_cmd_C();
	i=(g_algtype==PP60_TDESFLAG)?0:1;
	
	//�쳣
	if((staval=PP60_cmd_E(8))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//����
	//ǰ��//�������������뷶ΧΪ[4,8],�����ܽ��Ϊ8bytes.����λ������8�������F���Ѳ���,��������
	//�޸�PIN�Ĳ���Ҳ��F������,��������
	if((staval=PP60_cmd_L(0x08))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x04))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	
	cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");//�豸��13��1���м���ʱ,������8�������ȵĻᲹ0
	if((staval=PP60_cmd_E(0))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[2*i].len||memcmp(result[2*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			//DEBUG_HEX(out, outlen, NULL);
			//DEBUG_HEX(tmp, 8, NULL);
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������1234567+[ENTER],��ʾ\"������\"+*+����");
	if((staval=PP60_cmd_E(1))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[2*i].len||memcmp(result[2*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	cls_printf("����PP60������12345+[ENTER],��ʾ\"����\"+����+����");
	if((staval=PP60_cmd_E(2))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[1+2*i].len||memcmp(result[1+2*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+����+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������12345+[ENTER],��ʾ\"������\"+����+����");
	if((staval=PP60_cmd_E(3))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=result[1+2*i].len||memcmp(result[1+2*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+����+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	cls_printf("����PP60������12345+[ENTER],��ʾ\"����\"+*+����");
	if((staval=PP60_cmd_E(4))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=strlen("12345")||memcmp("12345", tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������12345+[ENTER],��ʾ\"������\"+*+����");
	if((staval=PP60_cmd_E(5))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=strlen("12345")||memcmp("12345", tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	//ע��:��������������ֻ֧�ִ�д��ĸ,��֧��Сд
	cls_printf("����PP60������1A2Y3+[ENTER],��ʾ\"����\"+*+����");
	if((staval=PP60_cmd_E(6))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=strlen("1A2Y3")||memcmp("1A2Y3", tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������1A2Y3+[ENTER],��ʾ\"������\"+*+����");
	if((staval=PP60_cmd_E(7))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=strlen("1A2Y3")||memcmp("1A2Y3", tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	//��֤LNλ�����õ���Ч��
	if((staval=PP60_cmd_N(0x01))!=NL829_PP60_SUCC||(staval1=PP60_cmd_L(0x01))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}	
	cls_printf("����PP60������1+[ENTER],��ʾ\"����\"+*+����");
	if((staval=PP60_cmd_E(4))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!=strlen("1")||memcmp("1", tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	if((staval=PP60_cmd_N(0x1e))!=NL829_PP60_SUCC||(staval1=PP60_cmd_L(0x1e))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}	
	cls_printf("����PP60������30��1+[ENTER],��ʾ\"����\"+*+����");
	if((staval=PP60_cmd_E(4))!=NL829_PP60_SUCC||(staval1=PP60_cmd_G(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	else//��һ��У��
	{
		memset(tmp1, 0x31, sizeof(tmp1));
		tmp1[30] = 0x00;
		if((tmplen=PP60_decode(out, outlen, tmp))!=strlen((char *)tmp1)||memcmp(tmp1, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	ret = NL829_PP60_SUCC;
ERR:
	PP60_cmd_C();
	return ret;	
}

static ushort __test_PP60_cmd_IX(ushort (*func)(uchar , uchar *, int, uchar *, ushort *))
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, tmp[16] = {0};
	uchar cardno[PP60_CARDNOLEN*2] = {0}, cardno1[PP60_CARDNOLEN*2] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0, ret = NL829_PP60_FAIL;
	int tmplen = 0, i = 0, data_len = 0;
	DATA_RESULT result[ ]={{8 ,(uchar *)"\xFA\x0B\x71\x38\x95\x29\xE5\xCE"}, // 0 ����ΪPIN(05 12 34 5F FF FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ8�ֽڵ�0X40 
						   {8 ,(uchar *)"\x71\xCB\xE5\x0C\xB5\xE0\xF0\xC6"}, // 1 ����ΪPIN(07 12 34 56 7F FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ8�ֽڵ�0X40
						   {8 ,(uchar *)"\xB3\x2C\xAD\x1B\xDF\xCE\xFC\xED"}, // 2 ����ΪPIN(0B 11 11 11 11 11 1F FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ8�ֽڵ�0X40
						   {16,(uchar *)"\xF9\x93\x0B\x77\x81\xE1\xAD\x2B\x44\x19\xF0\x19\x99\x79\x21\x78"}, // 0+3 ����ΪPIN(05 12 34 5F FF FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ16�ֽڵ�0X40
						   {16,(uchar *)"\x37\x08\x7D\x7A\xD5\x12\x5B\x09\x10\x3A\x5F\xD5\x25\x5C\x03\xEA"}, // 1+3 ����ΪPIN(07 12 34 56 7F FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ16�ֽڵ�0X40
						   {16,(uchar *)"\x71\x64\xD7\x79\x8E\x9A\x52\x26\x27\x8E\xF4\xEA\x01\x0C\x00\x9E"}  // 2+3 ����ΪPIN(0B 11 11 11 11 11 1F FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ16�ֽڵ�0X40
						  };

	/*process body*/
	//ǰ��
	PP60_cmd_C();
	data_len=(g_algtype==PP60_TDESFLAG)?PP60_LEN_PINBLOCK:PP60_SM4_LEN_PINBLOCK;
	i=(g_algtype==PP60_TDESFLAG)?0:1;

	//ǰ��
	if((staval=PP60_cmd_L(0x0f))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x00))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	
	//�쳣
	//cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");
#if TEST
	//�쳣1
	//cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");	
	memset(cardno, 0x01, sizeof(cardno));
	strcpy((char *)cardno1, "11111111111a");
	if((staval=func(0, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_FAIL||(staval1=func(0, cardno1, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
#endif
	//�쳣2
	//cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");
	memset(cardno, 0x31, sizeof(cardno));
	if((staval=func(0, cardno, 0, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=func(0, cardno, PP60_CARDNOLEN-1, out, &outlen))!=NL829_PP60_FAIL||(staval1=func(0, cardno, PP60_CARDNOLEN+1, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}	
	if((staval=func(0, cardno, PP60_CARDNOLEN-2, out, &outlen))!=NL829_PP60_FAIL||(staval1=func(0, cardno, PP60_CARDNOLEN+2, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	//�쳣3
	//memset(cardno, 0x31, sizeof(cardno));
	if((staval=func(3, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//�쳣4,���볬����PIN
	cls_printf("����PP60������15��1+[ENTER],��ʾ\"����\"+*+����");
	//memset(cardno, 0x31, sizeof(cardno));
	if((staval=func(0, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//����
	strcpy((char *)cardno, "750123456789");
	cls_printf("����PP60������12345+[ENTER],��ʾ\"����\"+*+����");
	if((staval=func(0, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len ||memcmp(result[3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			//DEBUG_HEX(out, outlen, NULL);
			//DEBUG_HEX(tmp, tmplen, NULL);
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������1234567+[ENTER],��ʾ\"������\"+*+����");
	if((staval=func(1, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len ||memcmp(result[1+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	cls_printf("����PP60������11��1+[ENTER],��ʾ\"����\"+*+������");
	if((staval=func(2, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len ||memcmp(result[2+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+������,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	//׷��ֱ�Ӱ�ȷ�Ϸ��ص�����
	if(func==PP60_cmd_X)
	{
	cls_printf("����PP60������[ENTER],��ʾ\"����\"+*+����");
	if((staval=func(0, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if(outlen!=1||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, outlen);
			goto ERR;
		}
	}

	cls_printf("����PP60������[ENTER],��ʾ\"������\"+*+����");
	if((staval=func(1, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if(outlen!=1||cls_show_msg("�����������,����ʾ\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, outlen);
			goto ERR;
		}
	}

	cls_printf("����PP60������[ENTER],��ʾ\"����\"+*+������");
	if((staval=func(2, cardno, PP60_CARDNOLEN, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if(outlen!=1||cls_show_msg("�����������,����ʾ\"����\"+*+������,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, outlen);
			goto ERR;
		}
	}		
	}

	ret = NL829_PP60_SUCC;
ERR:
	PP60_cmd_C();
	return ret;
}

static ushort test_PP60_cmd_I(void){return __test_PP60_cmd_IX(PP60_cmd_I);}

static ushort test_PP60_cmd_X(void){return __test_PP60_cmd_IX(PP60_cmd_X);}

static ushort test_PP60_cmd_J(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, tmp[16] = {0};
	uchar cardno[PP60_CARDNOLEN*2] = {0}, cardno1[PP60_CARDNOLEN*2] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0, ret = NL829_PP60_FAIL;
	int tmplen = 0 , i = 0, data_len = 0;
	DATA_RESULT result[ ]={{8 ,(uchar *)"\xFA\x0B\x71\x38\x95\x29\xE5\xCE"}, // 0 ����ΪPIN(05 12 34 5F FF FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ8�ֽڵ�0X40 
						   {8 ,(uchar *)"\x71\xCB\xE5\x0C\xB5\xE0\xF0\xC6"}, // 1 ����ΪPIN(07 12 34 56 7F FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ8�ֽڵ�0X40
						   {8 ,(uchar *)"\xB3\x2C\xAD\x1B\xDF\xCE\xFC\xED"}, // 2 ����ΪPIN(0B 11 11 11 11 11 1F FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ8�ֽڵ�0X40
						   {16,(uchar *)"\xF9\x93\x0B\x77\x81\xE1\xAD\x2B\x44\x19\xF0\x19\x99\x79\x21\x78"}, // 0+3 ����ΪPIN(05 12 34 5F FF FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ16�ֽڵ�0X40
						   {16,(uchar *)"\x37\x08\x7D\x7A\xD5\x12\x5B\x09\x10\x3A\x5F\xD5\x25\x5C\x03\xEA"}, // 1+3 ����ΪPIN(07 12 34 56 7F FF FF FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ16�ֽڵ�0X40
						   {16,(uchar *)"\x71\x64\xD7\x79\x8E\x9A\x52\x26\x27\x8E\xF4\xEA\x01\x0C\x00\x9E"}  // 2+3 ����ΪPIN(0B 11 11 11 11 11 1F FF) ����� PAN(00 00 75 01 23 45 67 89)   keyΪ16�ֽڵ�0X40
						  };
	
	/*process body*/
	//ǰ��
	PP60_cmd_C();
	data_len=(g_algtype==PP60_TDESFLAG)?PP60_LEN_PINBLOCK:PP60_SM4_LEN_PINBLOCK;
	i=(g_algtype==PP60_TDESFLAG)?0:1;

	//ǰ��
	if((staval=PP60_cmd_L(0x0f))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x00))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	
	//�쳣
	//cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");
#if TEST
	//�쳣1
	//cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");	
	memset(cardno, 0x01, sizeof(cardno));
	strcpy((char *)cardno1, "11111111111a");
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN, NULL, 0, out, &outlen))!=NL829_PP60_FAIL||(staval1=PP60_cmd_J(0, cardno1, PP60_CARDNOLEN, NULL, 0, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
#endif
	//�쳣2
	//cls_printf("����PP60������1234567+[ENTER],��ʾ\"����\"+*+����");
	memset(cardno, 0x31, sizeof(cardno));
	if((staval=PP60_cmd_J(0, cardno, 0, NULL, 0, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN-1, NULL, 0, out, &outlen))!=NL829_PP60_FAIL/*���1λ���Żᵱ��STR����||(staval1=PP60_cmd_J(0, cardno, PP60_CARDNOLEN+1, NULL, 0, out, &outlen))!=NL829_PP60_FAIL*/)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}	
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN-2, NULL, 0, out, &outlen))!=NL829_PP60_FAIL/*���2λ���Żᵱ��STR����||(staval1=PP60_cmd_J(0, cardno, PP60_CARDNOLEN+2, NULL, 0, out, &outlen))!=NL829_PP60_FAIL*/)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//�쳣3
	//memset(cardno, 0x31, sizeof(cardno));
	if((staval=PP60_cmd_J(3, cardno, PP60_CARDNOLEN, NULL, 0, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//�쳣4,���볬����PIN
	cls_printf("����PP60������15��1+[ENTER],��ʾ\"����\"+*+����");
	//memset(cardno, 0x31, sizeof(cardno));
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN, NULL, 0, out, &outlen))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ
	//�쳣5,STRlen����,STR��������ʾ�ַ�(������ʾASC���Զ���ĺ���ת������)
	//cls_printf("����PP60������15��1+[ENTER],��ʾ\"����\"+*+����");
	//memset(cardno, 0x31, sizeof(cardno));
	if((staval=PP60_cmd_C())!=NL829_PP60_SUCC)//������,Ҫ��֤ǰ��������ȷ
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN, (uchar *)"\x80\x81\x82\x83\x84\x80\x81\x82", strlen("\x80\x81\x82\x83\x84\x80\x81\x82"), out, &outlen))!=NL829_PP60_FAIL||cls_show_msg("��PP60���κ���ʾ������,��[ENTER],����[����]")==ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ
	//cls_printf("����PP60������1234+[ENTER],��ʾ\"7Ԫ\"+\"����\"+*+����");
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN, (uchar *)"\x37\xff\xff\x87", strlen("\x37\xff\xff\x87"), out, &outlen))!=NL829_PP60_FAIL||cls_show_msg("��PP60���κ���ʾ������,��[ENTER],����[����]")==ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	//����
	strcpy((char *)cardno, "750123456789");
	cls_printf("����PP60������12345+[ENTER],��ʾ\"��ӭ\"+\"����\"+*+����");
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN, (uchar *)"\x92\x93", strlen("\x92\x93"), out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len ||memcmp(result[3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"��ӭ\"+\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}
	
	cls_printf("����PP60������1234567+[ENTER],��ʾ\"��ӭ\"+\"������\"+*+����");
	if((staval=PP60_cmd_J(1, cardno, PP60_CARDNOLEN, (uchar *)"\x92\x93", strlen("\x92\x93"), out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len ||memcmp(result[1+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"��ӭ\"+\"������\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	cls_printf("����PP60������11��1+[ENTER],��ʾ\"��ӭ\"+\"����\"+*+������");
	if((staval=PP60_cmd_J(2, cardno, PP60_CARDNOLEN, (uchar *)"\x92\x93", strlen("\x92\x93"), out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp)) != data_len ||memcmp(result[2+3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"��ӭ\"+\"����\"+*+������,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	//���IX����,׷��STRΪ�յĲ��Լ���
	cls_printf("����PP60������12345+[ENTER],��ʾ\"����\"+*+����");
	if((staval=PP60_cmd_J(0, cardno, PP60_CARDNOLEN, NULL, 0, out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	else//��һ��У��
	{
		if((tmplen=PP60_decode(out, outlen, tmp))!= data_len ||memcmp(result[3*i].data, tmp, tmplen)||cls_show_msg("�����������,����ʾ\"����\"+*+����,��[ENTER],����[����]")!=ENTER)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, tmplen);
			goto ERR;
		}
	}

	ret = NL829_PP60_SUCC;
ERR:
	PP60_cmd_C();
	return ret;
}

static void test_scr_function(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL;

	/*process body*/
	//����ǰ��
	//case1:��Ļ��
	cls_show_msg1(2, "��Ļ��ָ�����...");//����ʾ��ˢ�����Բ˵�
	// �����������
	// cls_show_msg("���õ��㷨����Ϊ: 0x%x", g_algtype);
	if((staval=PP60_cmd_C())!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_D()!=NL829_PP60_SUCC)
		goto ERR;
	PP60_cmd_C();//���Ժ���
	if(test_PP60_cmd_O()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_P()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_W()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_h()!=NL829_PP60_SUCC)
		goto ERR;
	
	cls_show_msg("��Ļ��ָ�����ͨ��");
	
ERR://������,�ָ�Ĭ��
	PP60_cmd_C();
	PP60_cmd_Z();
	PP60_cmd_R((uchar *)"\x41\x41", strlen("\x41\x41"));
	PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48"));
	return;

}
static void test_key_function(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, staval2 = NL829_PP60_FAIL;
	
	//case2:��Կ������
	//ע�����²���˳��İ�����ǰ��Ϊ���ߵ�ǰ�õ�(�����໥������),����仯����˳���������Բ���
	//G�����������,���������������໥��֤����
	cls_show_msg1(2, "��Կ������ָ�����...");//����ʾ��ˢ�����Բ˵�
	if((staval=PP60_cmd_Z())!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��//ע�Ȿ������"������λ"����������"������λ"ʱ����Ĭ�ϼ����0���0��ukey
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_R()!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��//ִ�гɹ��Ļ�,�����õ�0,167����,����Կ
		goto ERR;
	if(test_PP60_cmd_MSU()!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��
		goto ERR;
	//����MAC�������ӽ�������(K,T,H)
	//if(test_PP60_cmd_K()!=NL829_PP60_SUCC)//K��������Կ����,�ʲ��ؼ���
	//	goto ERR;
#if TEST
	if((staval=PP60_cmd_A(0xa8, 0x00, g_algtype))!=NL829_PP60_FAIL||(staval1=PP60_cmd_A(0x00, 0x02, g_algtype))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
#endif
	if((staval=PP60_cmd_A(0xa7, 0x00, PP60_NODESFLAG))!=NL829_PP60_SUCC||(staval1=PP60_cmd_A(0xa7, 0x00, PP60_DESFLAG))!=NL829_PP60_SUCC||(staval2=PP60_cmd_A(0xa7, 0x00, PP60_TDESFLAG))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x,%04x)", __LINE__, staval, staval1, staval2);
		goto ERR;
	}	
	if((staval=PP60_cmd_A(0xa7, 0x00, g_algtype))!=NL829_PP60_SUCC)//��TDES��ʽ�����168���00�Ź�����Կ,��һ��������ԿҲ��ȫ0x00
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_K()!=NL829_PP60_SUCC)//��������SM4�����㷨,��A�����ȼ���󷽿�ʹ�� 20160303��������㷨֧������
		goto ERR;	
	if(test_PP60_cmd_T()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_H()!=NL829_PP60_SUCC)
		goto ERR;
	//����PIN����(L,N,E,F)
	if((staval=PP60_cmd_A(0x00, 0x00, g_algtype))!=NL829_PP60_SUCC)//ukey(00)��Ϊȫ0x38//ǰ������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_LN()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_F()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_E()!=NL829_PP60_SUCC)
		goto ERR;
	//����PINBLOCK�ļ���(I,J,X)
	if((staval=PP60_cmd_A(0x00, 0x01, g_algtype))!=NL829_PP60_SUCC)//ukey(01)��Ϊȫ0x40//ǰ������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_I()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_X()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_J()!=NL829_PP60_SUCC)
		goto ERR;
	
	cls_show_msg("��Կ������ָ�����ͨ��");
ERR://������,�ָ�Ĭ��
	PP60_cmd_C();
	PP60_cmd_Z();
	PP60_cmd_R((uchar *)"\x41\x41", strlen("\x41\x41"));
	PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48"));
	return;

}

static void test_other_function(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0};
	ushort staval = NL829_PP60_FAIL, outlen = 0;

	//case3:����
	cls_show_msg1(2, "������ָ�����...");//����ʾ��ˢ�����Բ˵�
	memset(out, 0, sizeof(out));//�Ӳ��ø���outlen���ض�
	if((staval=PP60_cmd_V(out, &outlen))!=NL829_PP60_SUCC||cls_show_msg("ȡ���汾:%s,��ȷ��[ENTER];����[����]", out)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_x()!=NL829_PP60_SUCC)
		goto ERR;

	cls_show_msg("������ָ�����ͨ��");
ERR://������,�ָ�Ĭ��
	PP60_cmd_C();
	PP60_cmd_Z();
	PP60_cmd_R((uchar *)"\x41\x41", strlen("\x41\x41"));
	PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48"));
	return;
}
static void test_function(void)
{
	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	while(1)
       {
		keyin = cls_show_msg("1.��Ļ��ָ��\n"
							 "2.��Կ������ָ��\n"
							 "3.����ָ��\n");
		switch(keyin) 
		{
		case '1':
			test_scr_function();
			break;
		case '2':
			test_key_function();
			break;
		case '3':
			test_other_function();
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
#if 0 //��ԭ����һϵ����Բ��
static void test_function(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, staval2 = NL829_PP60_FAIL, outlen = 0;

	/*process body*/
	//����ǰ��
	//case1:��Ļ��
	cls_show_msg1(2, "��Ļ��ָ�����...");//����ʾ��ˢ�����Բ˵�
	// �����������
	// cls_show_msg("���õ��㷨����Ϊ: 0x%x", g_algtype);
	if((staval=PP60_cmd_C())!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_D()!=NL829_PP60_SUCC)
		goto ERR;
	PP60_cmd_C();//���Ժ���
	if(test_PP60_cmd_O()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_P()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_W()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_h()!=NL829_PP60_SUCC)
		goto ERR;

	//case2:��Կ������
	//ע�����²���˳��İ�����ǰ��Ϊ���ߵ�ǰ�õ�(�����໥������),����仯����˳���������Բ���
	//G�����������,���������������໥��֤����
	cls_show_msg1(2, "��Կ������ָ�����...");//����ʾ��ˢ�����Բ˵�
	if((staval=PP60_cmd_Z())!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��//ע�Ȿ������"������λ"����������"������λ"ʱ����Ĭ�ϼ����0���0��ukey
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_R()!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��//ִ�гɹ��Ļ�,�����õ�0,167����,����Կ
		goto ERR;
	if(test_PP60_cmd_MSU()!=NL829_PP60_SUCC)//˳��䵱��case�Ĳ���ǰ��
		goto ERR;
	//����MAC�������ӽ�������(K,T,H)
	//if(test_PP60_cmd_K()!=NL829_PP60_SUCC)//K��������Կ����,�ʲ��ؼ���
	//	goto ERR;
#if TEST
	if((staval=PP60_cmd_A(0xa8, 0x00, g_algtype))!=NL829_PP60_FAIL||(staval1=PP60_cmd_A(0x00, 0x02, g_algtype))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
#endif
	if((staval=PP60_cmd_A(0xa7, 0x00, PP60_NODESFLAG))!=NL829_PP60_SUCC||(staval1=PP60_cmd_A(0xa7, 0x00, PP60_DESFLAG))!=NL829_PP60_SUCC||(staval2=PP60_cmd_A(0xa7, 0x00, PP60_TDESFLAG))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x,%04x)", __LINE__, staval, staval1, staval2);
		goto ERR;
	}	
	if((staval=PP60_cmd_A(0xa7, 0x00, g_algtype))!=NL829_PP60_SUCC)//��TDES��ʽ�����168���00�Ź�����Կ,��һ��������ԿҲ��ȫ0x00
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_K()!=NL829_PP60_SUCC)//��������SM4�����㷨,��A�����ȼ���󷽿�ʹ�� 20160303��������㷨֧������
		goto ERR;	
	if(test_PP60_cmd_T()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_H()!=NL829_PP60_SUCC)
		goto ERR;
	//����PIN����(L,N,E,F)
	if((staval=PP60_cmd_A(0x00, 0x00, g_algtype))!=NL829_PP60_SUCC)//ukey(00)��Ϊȫ0x38//ǰ������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_LN()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_F()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_E()!=NL829_PP60_SUCC)
		goto ERR;
	//����PINBLOCK�ļ���(I,J,X)
	if((staval=PP60_cmd_A(0x00, 0x01, g_algtype))!=NL829_PP60_SUCC)//ukey(01)��Ϊȫ0x40//ǰ������
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_I()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_X()!=NL829_PP60_SUCC)
		goto ERR;
	if(test_PP60_cmd_J()!=NL829_PP60_SUCC)
		goto ERR;
	
	//case3:����
	cls_show_msg1(2, "������ָ�����...");//����ʾ��ˢ�����Բ˵�
	memset(out, 0, sizeof(out));//�Ӳ��ø���outlen���ض�
	if((staval=PP60_cmd_V(out, &outlen))!=NL829_PP60_SUCC||cls_show_msg("ȡ���汾:%s,��ȷ��[ENTER];����[����]", out)!=ENTER)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if(test_PP60_cmd_x()!=NL829_PP60_SUCC)
		goto ERR;

	cls_show_msg("ָ�����ͨ��");
	//return;
ERR://������,�ָ�Ĭ��
	PP60_cmd_C();
	PP60_cmd_Z();
	PP60_cmd_R((uchar *)"\x41\x41", strlen("\x41\x41"));
	PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48"));
	return;
}
#endif

//���˼·:ѡ��һ�������в���Ҫ�ֹ������,ͬʱ��PP60ÿ���յ��������Ǳ䳤��仯����ѹ������
static void test_press_DCWAT(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0}, D[64] = {0}, data[32] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL;
	int succ = 0, cnt = DEFAULT_CNT_VLE, bak = 0, outlen = 0, outlen1 = 0, Dlen = 0, ret = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint len = 0, i=0;
	uchar tmp0[32], tmp1[32];
	DATA_RESULT result[ ]={{8, (uchar *)"\xfb\x04\xbe\xe7\x8f\x3a\x38\x53"}, //0 ʹ�÷���1  ����Ϊ21���ֽڵ�0x31 ��ԿΪ8���ֽڵ�0x00 
						   {8, (uchar *)"\x62\x2e\x2c\xd5\x2c\x3f\xdc\x83"}, //1 ʹ�÷���2  ����Ϊ21���ֽڵ�0x31 ��ԿΪ8���ֽڵ�0x00 
						   {16,(uchar *)"\xC7\xCE\x1F\x68\xD5\xC8\x4A\xF3\xB4\xBD\x6D\xDD\x07\x0C\x97\x37"}, //0+2 ʹ�÷���1  ����Ϊ21���ֽڵ�0x31 ��ԿΪ16���ֽڵ�0x00 
						   {16,(uchar *)"\x4C\xC0\xBB\xAE\x84\x0D\xE2\x16\x97\x84\x15\x9A\x31\xB8\x0F\x6F"}, //1+2 ʹ�÷���2  ����Ϊ21���ֽڵ�0x31 ��ԿΪ16���ֽڵ�0x00 
						  };
	//�й�����//244bytesԭʼ����
	uchar logo[] = {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0X08,0X08,0X08,0X08,0XFF,0X08,
				0X08,0X08,0X08,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,
				0X40,0X48,0X49,0X49,0X49,0X4F,0X49,0X49,0X49,0X48,0X40,0X7F,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X0C,0X34,0XE7,
				0X24,0X24,0X00,0X7F,0X49,0X49,0X49,0X49,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X40,0X40,0X7F,0X48,0X48,0X7F,0X40,0X08,0X88,0X68,0X0F,0X08,0X28,0XC8,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0X40,0X40,0X40,
				0X40,0XFF,0X40,0X40,0X40,0X40,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0XFF,0X02,0X12,0X12,0X12,0X12,0XF2,0X12,0X52,0X32,0X12,0X02,0XFF,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,
				0X80,0X80,0XFE,0X84,0X88,0X80,0XFF,0X02,0XC4,0X30,0X18,0X24,0X46,0X04,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X06,0XFC,0X88,0X88,0XFF,0X09,0X82,0X8C,0XB0,0XC0,
				0XB0,0X8C,0X83,0X82,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
				0X00,0X00,0X00,0X00,0X00,0X00,0XFF};//���һ��0XFF�Ǵ����,����ʹ��ʱҪ����ȥ��

	/*process body*/
	//����ǰ��
	outlen = PP60_code(logo, sizeof(logo), out);
	i=(g_algtype==PP60_TDESFLAG)?0:1;
	PP60_code(result[2*i].data, result[2*i].len, tmp0);
	PP60_code(result[1+2*i].data, result[1+2*i].len, tmp1);
	
	memset(data, 0x31, sizeof(data));
	Dlen = PP60_code(data, sizeof(data), D);
	if((staval=PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48")))!=NL829_PP60_SUCC)//���õ�167����,����Կ
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}

	//��ʽ����
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
	if ((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	while(cnt)
	{
		cls_printf("ѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", cnt, succ);
		cnt--;

		if((staval=PP60_cmd_D(rand()%2+1, (uchar *)"\x80\x81\x82\x83\x84\x31\x32\x33\x34", strlen("\x80\x81\x82\x83\x84\x31\x32\x33\x34")))!=NL829_PP60_SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, staval);
			continue;
		}
		if((staval=PP60_cmd_C())!=NL829_PP60_SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, staval);
			continue;
		}
#if TEST
		if((staval=PP60_cmd_W(0, 0, out, outlen-2))!=NL829_PP60_SUCC||(staval1=PP60_cmd_W(0, 1, out, outlen-2))!=NL829_PP60_SUCC)//�������������ݵ�W����
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x,%04x)", __LINE__, bak-cnt, staval, staval1);
			continue;
		}
#endif
		if((staval=PP60_cmd_A(/*rand()%0xa8*/0xa7, rand()%2, g_algtype))!=NL829_PP60_SUCC)//�������Կ�Ų�������,Ŀǰд����0xa7,��Ϊ�������������Ҫ�Դ�Ϊǰ������
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, staval);
			continue;
		}
#if TEST
		if((staval=PP60_cmd_T(0x00, D, 21*2, out1, (ushort *)&outlen1))!=NL829_PP60_SUCC||outlen1!=(result[2*i].len*2)||memcmp(out1, tmp0, outlen1))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x,%d)", __LINE__, bak-cnt, staval, outlen1);
			continue;
		}
		if((staval=PP60_cmd_T(0x01, D, 21*2, out1, (ushort *)&outlen1))!=NL829_PP60_SUCC||outlen1!=(result[1+2*i].len*2)||memcmp(out1, tmp1, outlen1))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x,%d)", __LINE__, bak-cnt, staval, outlen1);
			continue;
		}
#endif
		
		succ++;
	}

	cls_show_msg("DCWAT����ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void test_press_A(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL;
	int succ = 0, cnt = DEFAULT_CNT_VLE, bak = 0, ret = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint len = 0;

	/*process body*/
	//��ʽ����
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
	if ((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	while(cnt)
	{
		cls_printf("ѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", cnt, succ);
		cnt--;

		if((staval=PP60_cmd_A(rand()%0xa8, rand()%2, g_algtype))!=NL829_PP60_SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%04x)", __LINE__, bak-cnt, staval);
			continue;
		}
		succ++;
	}

	cls_show_msg("A����ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void test_press(void)
{
	/*private & local definition*/
	int keyin = 0;
	PF func[] = {test_press_DCWAT, test_press_A}; 

	/*process body*/
	while(1)
       {
		keyin = cls_show_msg("1.���ѹ��\n"
							"2.����ѹ��");//A�����ڵ�BPS�¸����׿��ٲ������;���ѹ������нϴ��������Ķ���������,�ڲ�ͬ��������Ҳ�����(�����һЩ�ų���).��ԭ���Ϸ���,ÿ�����������,ֻ�Ƕ��������Ч�ʸ��߶���(������������Ͳⲻ�������)

		switch(keyin)
		{
		case '1':
		case '2':	
			func[keyin-'1']();
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

//�ɸ��ݲ�������,������չ
static void test_ability(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, str[PP60_CMDK_SM4_STRLEN] = {0}/*str��ʵֻҪ8�ֽڼ���,����������Щ*/, STR[PP60_CMDK_SM4_STRLEN*2] = {0}, key[16] = {0};
	ushort staval = NL829_PP60_FAIL, outlen = 0;
	int STRlen = 0, data_len = 0;
	float time = 0;
	
	/*process body*/
	cls_printf("���ܲ���...");
	data_len=(g_algtype==PP60_TDESFLAG)?PP60_CMDK_STRLEN:PP60_CMDK_SM4_STRLEN;

	// 1.��������
	if((staval=PP60_cmd_R((uchar *)"\x4b\x48", strlen("\x4b\x48")))!=NL829_PP60_SUCC)//���õ�167����,����Կ
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}

	memset(key, 0x38, sizeof(key));
	memset(str, 0x31, data_len);
	STRlen = PP60_code(str, data_len, STR);

	lib_StartStopwatch();
	if((staval=PP60_cmd_A(0xa7, 0x00, g_algtype))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval);
		return;
	}
	if((staval=PP60_cmd_K(0xa7, STR, data_len, out, &outlen))!=NL829_PP60_SUCC)//K��������Կ����,�ʲ��ؼ���
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	else//��һ��У��
	{
		time = lib_ReadStopwatch();
		//DEBUG_HEX(out, outlen, NULL);
		if((STRlen=PP60_decode(out, outlen, STR))!= data_len/2)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, STRlen);
			return;		
		}
		//if(NDK_AlgTDes(str, out, key, sizeof(key), ALG_TDS_MODE_DEC)!=NDK_OK)//if(mdes3(DISCRYPT, str, out, key)!=SUCC)
		if(AlgCompute(g_algtype, str, data_len/2 , out, key, sizeof(key), ((g_algtype==PP60_TDESFLAG)?ALG_TDS_MODE_DEC:ALG_SM4_DECRYPT_ECB) )!=NDK_OK)//OK��������,�������м仺��
		{
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return;
		}
		if(memcmp(out, STR, STRlen))
		{
			//DEBUG_HEX(out, STRlen, NULL);
			//DEBUG_HEX(STR, STRlen, NULL);
			cls_show_msg("line %d:����ʧ��", __LINE__);
			return;
		}
	}
	if(g_algtype==PP60_TDESFLAG)
		cls_show_msg("����DES�����(��)�������ʱ:%fs", time);
	else
		cls_show_msg("����SM4�����(��)�������ʱ:%fs", time);
	// 2.ȡ�汾
	memset(out, 0, sizeof(out));//�Ӳ��ø���outlen���ض�
	lib_StartStopwatch();
	if((staval=PP60_cmd_V(out, &outlen))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		return;
	}
	time = lib_ReadStopwatch();
	cls_show_msg("ȡ���汾:%s(��ʱ:%fs)", out, time);
}

//�����Ժ������������쳣����,����һЩ�����ڹ��ܲ����еķǳ������(�����ʱ�϶�Ĳ���)Ҳ���ڴ˴�
static void test_abnormity(void)
{
	/*private & local definition*/
	uchar out[PP60_MAXLEN_DATA] = {0}, out1[PP60_MAXLEN_DATA] = {0}, frame_cmd[NL829_MAXLEN_FRAME] = {0}, OK[16] = {0}, NK[16] = {0};
	ushort staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL, outlen = 0, outlen1 = 0;
	int len = 0, subcase = 0;
	
	/*process body*/
	cls_show_msg1(2, "�쳣�������...");

	// 1.��֧�ֵ�����,��Q����
	if((staval=PP60_cmd_Q())!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ

	// 2.��ʶ�������
	if((staval=PP60_send_frame((uchar *)"1234567", strlen("1234567")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_recv_frame(1, out, &outlen))==NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ

	// 3.���������ͷǰ,�в�,β���и�������,��C����Ϊ��
	//�м����
	if((staval=PP60_send_frame((uchar *)"\x1b\x43\x0a\x0d\x0a", strlen("\x1b\x43\x0a\x0d\x0a")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_recv_frame(PP60_CMDC_RCVLEN, out, &outlen))!=NL829_PP60_SUCC||out[0]!=PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, out[0]);
		goto ERR;
	}
	//ͷ������(��1b��ͷ)
	if((staval=PP60_send_frame((uchar *)"\x0a\x1b\x43\x0d\x0a", strlen("\x0a\x1b\x43\x0d\x0a")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_recv_frame(PP60_CMDC_RCVLEN, out, &outlen))!=NL829_PP60_SUCC||out[0]!=PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, out[0]);
		goto ERR;
	}
	//β������
	if((staval=PP60_send_frame((uchar *)"\x1b\x43\x0d\x0a\x0a", strlen("\x1b\x43\x0d\x0a\x0a")))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_recv_frame(PP60_CMDC_RCVLEN, out, &outlen))!=NL829_PP60_SUCC||out[0]!=PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x, %02x)", __LINE__, staval, out[0]);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ

	//���²��Ե�ǰ������
	g_adjust = PP60_TIMEOUT_MAX;

	// 4.���е�keyȫ������
	cls_printf("����PP60����key��,�����ĵȴ�...");
	if((staval=PP60_cmd_R(NULL, 0))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_printf("���óɹ�,��֤��...");
	//��֤mkey��Ϊ��ȫ0x38,ukey��Ϊ��ȫ0x30
	memset(OK, 0x38, sizeof(OK));
	memset(NK, 0x55/*�ײ���Ϊ0x39��0x38��һ����key*/, sizeof(NK));
	outlen = PP60_code(OK, sizeof(OK), out);
	outlen1= PP60_code(NK, sizeof(NK), out1);
	if((staval=PP60_cmd_M(0x03, out, outlen, out1, outlen1))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	if((staval=PP60_cmd_U(0x03, 0x00, out1, outlen1, out1, outlen1))!=NL829_PP60_SUCC||(staval1=PP60_cmd_U(0x03, 0x01, out1, outlen1, out1, outlen1))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ

	// 5.���벻��λ��PIN+[ENTER],Ӧ��ʱ����0x55
	if((staval=PP60_cmd_L(0x1e))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x10))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	cls_printf("����PP60������1234+[ENTER]���ٰ���,�����ĵȴ���Ӧ...");//���ٰ�������PP60���¼��㳬ʱ
	if((staval=PP60_cmd_E(0x02))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ

	// 6.��󳤶�С����С����ʱ,�޷�����ȷ��,Ӧ��ʱ����0x55
	if((staval=PP60_cmd_L(0x01))!=NL829_PP60_SUCC||(staval1=PP60_cmd_N(0x1e))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x,%04x)", __LINE__, staval, staval1);
		goto ERR;
	}
	cls_printf("����PP60������1+[ENTER]���ٰ���,�����ĵȴ���Ӧ...");
	if((staval=PP60_cmd_E(0x02))!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	cls_show_msg1(2, "%d#����������ͨ��!", ++subcase);//��Ļ�б仯,��������֮�����Ļ��ʾ
	
	// 7.��󳤶�С����С����ʱ,�޷�����ȷ��,����һ����������PP60�˳��ȴ���ʱ̬,�������������ϵ�,PP60����Ӧ��
	//cls_printf("����PP60������1+[ENTER]...");
	//���
	len = PP60_pack_data('E', (uchar *)"2", 1, frame_cmd);
	//����
	if((staval=PP60_send_frame(frame_cmd, len))!=NL829_PP60_SUCC)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}
	//�����������ķ���,������һ���������ж��豸�Ľ���
	if((staval=PP60_cmd_C())!=NL829_PP60_FAIL)
	{
		cls_show_msg("line %d:����ʧ��(%04x)", __LINE__, staval);
		goto ERR;
	}

	cls_show_msg("�쳣����ͨ��");
ERR://������,�ָ�Ĭ��
	g_adjust = 0;
	PP60_cmd_Z();
	PP60_cmd_R((uchar *)"\x41\x44", strlen("\x41\x44"));
	return;
}

static int select_PP60(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	int type[] = {DEVICE_PP60PT, DEVICE_PP60RF, DEVICE_PP60RF};

	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("_ѡ��PP60����_\n"
							"1.��ͨPP60\n"
							"2.PP60RF\n"
							"3.����PP60RF\n");
		switch(nKeyin)
		{
		case '1':
		case '2':
			g_LCDSize= 0;
			return type[nKeyin-'1'];
			break;
		case '3':
			g_LCDSize = 1;//����������̿���
			return type[nKeyin-'1'];
			break;
		case ESC:
			g_LCDSize = 0;
			cls_show_msg1(2, "������Ϊ��ͨ��(Ĭ��)...");
			return DEVICE_PP60PT;
			break;
		default:
			continue;
			break;
		}
	}
}

static int select_PP60_ALGORITHM(void)
{
	/*private & local definition */
	int nKeyin = 0;
	int alg_select[] = {PP60_TDESFLAG, PP60_SM4FLAG};
	/* process body */
	while(1)
	{
		nKeyin = cls_show_msg("ѡ��PP60�����㷨����\n"
							"1.DES�㷨\n"
							"2.SM4�㷨\n");
		switch(nKeyin)
		{
			case '1':
			case '2':
				return alg_select[nKeyin-'1'];
				break;
			case ESC:
				cls_show_msg1(2, "�����㷨���ͽ�����ΪDES...");
				return PP60_TDESFLAG;
				break;
			default:
				continue;
				break;
		}
	}
}
			
static int conf_PP60PT_bps(void)
{
	/*private & local definition*/
	int nKeyin = 0, loop = 0;
	//int bps[] = {BPS1200, BPS2400, BPS4800, BPS9600, /*BPS14400*/BPS9600, BPS19200, BPS38400, BPS57600};//ע��POS�ϲ����ṩBPS14400,��PED��Ҫ����Ϊ���BPS
	char *bps[] = {"1200",  "2400", "4800", "9600", "9600", "19200", "38400", "57600"};
	int bpsid=0;
	
	/*process body*/
	// 1.������NL829���ӵ�POS���ڲ���
	cls_show_msg1(2, "�������������贮�ڵĲ�������");
	g_devicecom = select_serial();
	if(cls_show_msg("�뽫POS����bps����Ϊ��PP60һ��,[ENTER]����[����]�˳�")==ENTER)
	{
		strcpy(g_devicecombps, conf_serial(&bpsid));//g_devicecombps = conf_serial();
		if(NDK_PortOpen(g_devicecom, g_devicecombps)!=NDK_OK)//if(initaux(g_devicecom, g_devicecombps, DB8|STOP1|NP)!=SUCC)
		{
			cls_show_msg1(2, "line %d:��ʼ��POS����ʧ��", __LINE__);
			return FAIL;
		}
		return SUCC;
	}
	else
		return FAIL;

	// 2.����NL829���ڲ���
	if(cls_show_msg("��������PP60PT BPS��?\n"
					"[ȷ��]��[����]��\n")!=ENTER)
		return SUCC;
	
	do
	{
		nKeyin = cls_show_msg("1.1200   2.2400\n3.4800   4.9600\n5.14400  6.19200\n7.38400  8.57600");
		switch(nKeyin)
		{
		case ESC:
		case '5':
			cls_show_msg1(2, "��֧��BPS14400,������ΪBPS9600(Ĭ��)...");
			nKeyin = '4';//ת��
		case '1':
		case '2':
		case '3':
		case '4':	
		case '6':
		case '7':
		case '8':
			cls_printf("��������BPS��...");
			if(PP60_cmd_B(nKeyin-'0')!=NL829_PP60_SUCC)//ʹ�ô�B,�Ա�֤�µ���ֵ����Ч
			{
				cls_printf("����BPS�Զ�������...�����ĵȴ�");
				//�����ɹ�,�����п��ܵ�BPS���г���
				for(loop=0; loop<SZ_ARRAY(bps); loop++)
				{
					NDK_PortOpen(g_devicecom, bps[loop]);//initaux(g_devicecom, bps[loop], DB8|STOP1|NP);
					if(PP60_cmd_B(nKeyin-'1')==NL829_PP60_SUCC)
						break;
				}
				if(loop==SZ_ARRAY(bps))
				{
					cls_show_msg1(2, "line %d:��������PP60PT BPSʧ��,���������ٳ���", __LINE__);
					return FAIL;
				}
			}
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin<'1'||nKeyin>'8');

	// 3.����NL829��ǰBPS����POS���ڵ�BPS
	if(NDK_PortOpen(g_devicecom, bps[nKeyin-'1'])!=NDK_OK)//if(initaux(g_devicecom, bps[nKeyin-'1'], DB8|STOP1|NP)!=SUCC)
	{
		cls_show_msg1(2, "line %d:��ʼ��POS����ʧ��", __LINE__);
		return FAIL;
	}
	strcpy(g_devicecombps, bps[nKeyin-'1']);//g_devicecombps = bps[nKeyin-'1'];//g_bps�������ý��е�������
	return SUCC;
}

static void conf_PP60(void)
{
	/*private & local definition*/
	ushort staval = NL829_PP60_FAIL;

	/*process body*/
	// 1.����PP60ʵ�ʵ�����(PT or RF)
	g_PP60type = select_PP60();

	// 2.����PP60�����㷨(SM4��3DES or DES)
	g_algtype = select_PP60_ALGORITHM();

	// 3.����ʵ�����ͽ�����Ӧ����
	cls_show_msg("��ȷ��POS������������!���������...");
	if((g_PP60type==DEVICE_PP60PT)?conf_PP60PT_bps():conf_NL829_bps()!=SUCC)
		cls_show_msg("line %d:����ʧ��!������POS���������������.", __LINE__);
	else
	{
		if(g_PP60type==DEVICE_PP60PT)
			cls_show_msg1(2, "��·���óɹ�!");
		else//������PP60RF,��һ������ͨ����
		{
			sleep(2);//20120221add
			staval = NL829_cnnt_PP60();
			cls_show_msg1(2, "��·����%s(%04x)!", staval==NL829_PP60_SUCC?"�ɹ�":"ʧ��", staval);
		}
	}
}

/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		  jennywone163 	20110819		created
*****************************************************************/
void systest35(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg("%s������...���Ƚ�������,���������ʼ\n", TESTITEM);
	g_PP60type = DEVICE_PP60PT;

	while(1)
       {
		keyin = cls_show_msg("1.���� 2.ѹ��\n"
							"3.���� 4.�쳣\n"
							"0.����");

		switch(keyin)
		{
		case '1':
			test_function();
			break;
		case '2'://ѹ������һ�����ǲ��Թ��ܵ�ѹ��,��һ����Ҳ�Ǵ���ͨѶ��ѹ��
			if(ENTER!=cls_show_msg("ѹ������(������Բ�����ESC�жϲ���),ENTER������..."))
				break;			
			test_press();
			break;
		case '3':
			test_ability();
			break;
		case '4':
			test_abnormity();
			break;
		case '9'://����,����ɾ��SN
			{
				uchar out[PP60_MAXLEN_DATA] = {0};
				ushort /*staval = NL829_PP60_FAIL, staval1 = NL829_PP60_FAIL,*/ outlen = 0;
				PP60_cmd_x(3, NULL, 0, out, &outlen);
			}
			break;
		case '0'://��ͬ��BPS���ò�������(��,ѹ������)��һ��,���Գ�Ĭ�Ϲ���BPS��,�����͵�BPSҲҪ����һ��
			conf_PP60();
			break;
		case ESC:
			cls_show_msg("����������BPS���²���!���������...");
			if(g_PP60type==DEVICE_PP60RF)
				cls_show_msg("�����н�����Ƶ���ֵĲ���!");	
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

