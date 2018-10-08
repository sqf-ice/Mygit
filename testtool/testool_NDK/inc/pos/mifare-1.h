/*
 * FileName:       
 * Author:         zhangnw  Version: 1.6.12  Date: 2009-12-27
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version>  <desc>
 */
#ifndef MIFARE_1_H
#define MIFARE_1_H

/*-----------------------includes-------------------------------*/

/*---------------constants/macro definition---------------------*/
#if 0
/*1���ǽӴ������� */
#define	MI_OK							(0)	//������óɹ�
#define	MI_NOTAGERR					(-1)  //0xff    ��Ӧ����û����Ƶ�� 
#define	MI_CRCERR						(-2)	//����CRC����
#define	MI_EMPTY						(-3)  //0xfd    ֵ��� 
#define	MI_AUTHERR						(-4)  //0xfc    ��֤����(M1) 
#define	MI_PARITYERR					(-5)  //0xfb    ��żУ����� 
#define	MI_CODEERR						(-6)	//��RF����ͨѶ����
#define	MI_SERNRERR					(-8)	//����ͻ�п����кŴ���
#define	MI_KEYERR						(-9)  //0xf7    ��֤��Կ����   
#define	MI_NOTAUTHERR					(-10)  //0xf6    δ��֤(M1) 
#define	MI_BITCOUNTERR				(-11)	//���ձ���������
#define	MI_BYTECOUNTERR				(-12)	//�����ֽ�������
#define	MI_WriteFifo					(-13)  //0xf3    дRC531�����쳣 
#define	MI_TRANSERR					(-14)	//����Transferִ�д���
#define	MI_WRITEERR					(-15)	//����Writeִ�д���
#define	MI_INCRERR						(-16)	//����Incrementִ�д���
#define	MI_DECRERR						(-17)  //0xef    ����Decrementִ�д��� 
#define	MI_READERR						(-18)	//����Readִ�д���
#define	MI_OVFLERR						(-19)	//ֵ�������
#define	MI_FRAMINGERR					(-21)	//֡����
#define	MI_ACCESSERR					(-22)  //0xea    RC531��д�쳣 
#define	MI_UNKNOWN_COMMAND			(-23)	//��Ч����
#define	MI_ErrCOMMAND					(MI_UNKNOWN_COMMAND)  //0xe9    ��Ч���� 
#define	MI_COLLERR						(-24)	//��ͻ����
#define	MI_RESETERR					(-25)	//��λ����
#define	MI_INITERR						(-25)	//��ʼ������
#define	MI_INTERFACEERR				(-26)	//�ӿڴ���
#define	MI_ACCESSTIMEOUT				(-27)	//���ʳ�ʱ
#define	MI_NOBITWISEANTICOLL			(-28)	//����ͻ�����е�TagType�����
#define	MI_PROTOCOLERR				(MI_NOBITWISEANTICOLL)  //0xe4    ����ͻ�����е�TagType����� 
#define	MI_QUIT						(-30)	//����ж��˳�
#define	MI_RECBUF_OVERFLOW			(-50)	//���ջ��������
#define	MI_SENDBUF_OVERFLOW			(-53)	//���ͻ��������
#define	MI_WRONG_PARAMETER_VALUE	(-60)	//��������
//#define	PARAERR						(-87)  //0xA9    C�ӿڵ��ã���������(mi_iccrw) 
#define	Err_Command					(-88)  //0xA8    C�ӿڵ��ã���������  
#define	Err_ioctl						(-89)  //0xA7    C�ӿڵ��ã�linux�ӿڣ���������
#define	MI_NY_IMPLEMENTED			(-100)	//ûװ��Կ����
#define	MI_CopyErr						(-124) //0x84    �ں����ݿ������� 

/*2���ǽӴ����ͽӴ����� API��ֲ�й�ͬʹ�õĴ�����룺 */
#define read_err      -1001   // ������83c26���� 
#define write_err      -1002  // д����83c26���� 
#define getbyte_err1    -1011  // �����ط����� 
#define poweroncheck_err  -1014  // �ϵ���(��ѹ)���� 
#define ATR_RST_err     -1015  // ATR �ڼ� RST�ź��쳣 
#define ATR_IO_err      -1016  // ATR �ڼ� IO�ź��쳣 
#define ATR_ColdStart_err  -1017  // �临λ�� IC���ظ� 
#define ATR_TA1_err     -1018  // ATR �� TA1���� 
#define ATR_TA2_err     -1019  // ATR �� TA2���� 
#define ATR_TC2_err     -1020  // ATR �� TC2���� 
#define ATR_TD2_err     -1021  // ATR �� TA3���� 
#define ATR_TA3_err     -1022  // ATR �� TA3���� 
#define ATR_TB3_err     -1023  // ATR �� TB3���� 
#define ATR_TC3_err     -1024  // ATR �� TC3���� 
#define ATR_WarmStart_err  -1025  // �ȸ�λ�� IC���ظ� 
#define T1_IFSD_err     -1026  // T=1 ʱ����IFSD���� 
#define T1_Abort_err    -1027  // T=1 ʱ IC������ 
#define CopyErr       -1028  // �ں����ݿ������� 

#define PowerOn_err     -1100  // �ϵ���� 
#define PowerOff_err    -1101  // �µ����  
#define Com_err       -1102  // ������� 
#define CardPull_err    -1103  // ���γ��� 
#define CardNoReady_err   -1104  // ��δ׼���� 
#define CaseProce_err    -1105  // CASE ������̳��� 
#define CaseOverflow_err  -1106  // ������� 

#define Open_err      -2000   // ���豸����(�Ӵ���) 
#define para_err      -2001  // �������� 
#define timeout_err     -2002  // readcard ��ʱ���� 
#define IcAlreadyUp_err   -2003  // IC���Ѿ��ϵ��� 
#define Open_err_rf     -2004   // ���豸����(�ǽӴ���) 
#define RC531_InitErr    -2005   // RC531δ���û���Ӳ���й��� 
#endif

//������zhangnw20091227 add for mifare-1
//mifare-1������֡��һЩ����
#define	SEQNR				(0)
//����̶����������ֽ����е�ƫ��λ��
#define	OFFSET_SEQNR		(0)
#define	OFFSET_CMD		(OFFSET_SEQNR+1)	//(1)
#define	OFFSET_STATUS		OFFSET_CMD
#define	OFFSET_DATALEN	(OFFSET_CMD+1)	//(2)
#define	OFFSET_DATA		(OFFSET_DATALEN+1)	//(3)
#define	LEN_FRAMEHEAD		OFFSET_DATA
#define	LEN_FRAMETAIL		(1)
#define	LEN_ATQA			(32)	//(2)
#define	LEN_SAK			(32)	//(1) //Ϊ��ֹ����UID���������,Ҳ������Ϊ�䳤,�������ֵ
#define	LEN_UID			(10)	//(4) //ע��:UID�ǿɱ䳤��,������4,7,10��ֵ,��,A��ΪUID;B��databuf[1]~[4]ΪUID,������Ӧ�ü�Э����Ϣ.�ʿ���Щ
#define	LEN_KEY			(6)
#define	LEN_BLKDATA		(16)
#define	LEN_BLKINCDEC		(4)
#define	MAXLEN_DATA		(0xff)	//(256)
#define	MAXLEN_FRAME		(MAXLEN_DATA+8)

//����Mifare-1������������ĵ��涨
#define	CMD_INIT							(0xff)
#define	CMD_HALT							(0x45)
#define	CMD_VOID_DATALEN_SND			(0)	//�޷���data
#define	CMD_VOID_DATALEN_RCV				(0)	//�޽���data
#define	CMD_REQUEST						(0x41)
#define	CMD_REQUEST_DATALEN_SND			(1)
#define	CMD_REQUEST_DATALEN_RCV			(LEN_ATQA)	//(2)
#define	CMD_ANTICOLL						(0x42)
#define	CMD_ANTICOLL_DATALEN_SND		(1)
#define	CMD_ANTICOLL_DATALEN_RCV		(LEN_UID)	//(4)
#define	CMD_SELECT							(0x43)
#define	CMD_SELECT_DATALEN_SND			(LEN_UID)	//(4)
#define	CMD_SELECT_DATALEN_RCV			(LEN_SAK)	//(1)
#define	CMD_STOREKEYE2					(0x36)
#define	CMD_STOREKEYE2_DATALEN_SND		(8)
#define	CMD_STOREKEYE2_DATALEN_RCV		(0)
#define	CMD_LOADKEYE2						(0x37)
#define	CMD_LOADKEYE2_DATALEN_SND		(3)
#define	CMD_LOADKEYE2_DATALEN_RCV		(0)
#define	CMD_AUTH							(0x44)
#define	CMD_AUTH_DATALEN_SND			(7)
#define	CMD_AUTH_DATALEN_RCV			(0)
#define	CMD_AUTH_EX						(0x54)	//�ɵ�������(0x4a)
#define	CMD_AUTH_EX_DATALEN_SND			(12)
#define	CMD_AUTH_EX_DATALEN_RCV			(0)
#define	CMD_BLKREAD						(0x46)
#define	CMD_BLKREAD_DATALEN_SND			(1)
#define	CMD_BLKREAD_DATALEN_RCV			(LEN_BLKDATA)		//(16)
#define	CMD_BLKWRITE						(0x47)
#define	CMD_BLKWRITE_DATALEN_SND		(LEN_BLKDATA+1)		//(17)
#define	CMD_BLKWRITE_DATALEN_RCV		(0)
#define	CMD_BLKINC							(0x48)
#define	CMD_BLKDEC							(0x49)
#define	CMD_BLKINCDEC_DATALEN_SND		(LEN_BLKINCDEC+1)	//(5)
#define	CMD_BLKINCDEC_DATALEN_RCV		(0)
#define	CMD_BLKTRANSFER					(0x38)
#define	CMD_BLKRESTORE					(0x39)
#define	CMD_BLKTRANSREST_DATALEN_SND	(1)
#define	CMD_BLKTRANSREST_DATALEN_RCV	(0)
#define	CMD_POWERDOWN					(0x4e)
#define	CMD_POWERDOWN_DATALEN_SND		(2)
#define	CMD_POWERDOWN_DATALEN_RCV		(0)
//����CPU�����Ϊ�˼��ݺ��ŵľɽӿڹ��ܶ�����for RFcard7
#define	HS_SETICTYPE						(0x80)
#define	HS_SETICTYPE_DATALEN_SND		(1)
#define	HS_SETICTYPE_DATALEN_RCV			(0)
#define	HS_REQUEST_TYPEB_DATALEN_RCV	(12)	
#define	HS_SELRESP_TYPEA					(0x68)
#define	HS_SELRESP_TYPEA_DATALEN_SND	(1)
#define	HS_SELRESP_TYPEA_DATALEN_RCV_MAX	(20)		//�������ݲ�������С��20
#define	HS_POWERUP_TYPEB					(0x82)
#define	HS_POWERUP_TYPEB_DATALEN_SND	(0)
#define	HS_POWERUP_TYPEB_DATALEN_RCV	(1)
#define	HS_APDURW							(0x69)
//#define	HS_APDURW_DATALEN_SND_MAX		(MAXLEN_DATA)
//#define	HS_APDURW_DATALEN_RCV_MAX		(MAXLEN_DATA)
#define	HS_POWERDOWN						(0x6a)
#define	HS_POWERDOWN_DATALEN_SND		(0)
#define	HS_POWERDOWN_DATALEN_RCV		(0)

//Ӧ�ò���չ�˷���״̬��,-2000��ʼΪӦ�ò㶨��Ĵ�����
#define	ERR_FRAME_BASE			(-5000)	//����ײ��Ѷ���Ĵ��󷵻����ͻ
#define	ERR_FRAME_SEQNR			(ERR_FRAME_BASE-1)
#define	ERR_FRAME_BCC				(ERR_FRAME_BASE-2)
#define	ERR_FRAME_LEN				(ERR_FRAME_BASE-3)

#define	ERR_CMD_BASE				(-6000)
#define	ERR_CMD_VOID				(ERR_CMD_BASE-1)	//�޲�����(init,halt��)��������
#define	ERR_CMD_REQUEST			(ERR_CMD_BASE-2)
#define	ERR_CMD_ANTICOLL			(ERR_CMD_BASE-3)
#define	ERR_CMD_SELECT			(ERR_CMD_BASE-4)
#define	ERR_CMD_STOREKEYE2		(ERR_CMD_BASE-5)
#define	ERR_CMD_LOADKEYE2		(ERR_CMD_BASE-6)
#define	ERR_CMD_AUTH				(ERR_CMD_BASE-7)
#define	ERR_CMD_AUTH_EX			(ERR_CMD_BASE-8)
#define	ERR_CMD_BLKREAD			(ERR_CMD_BASE-9)
#define	ERR_CMD_BLKWRITE			(ERR_CMD_BASE-10)
#define	ERR_CMD_BLKINCDEC			(ERR_CMD_BASE-11)
#define	ERR_CMD_BLKTRANSREST		(ERR_CMD_BASE-12)
#define	ERR_CMD_POWERDOWN		(ERR_CMD_BASE-13)
#define	ERR_CMD_INIT_SETTYPE		(ERR_CMD_BASE-14)
#define	ERR_CMD_INIT_SETTYPE1	(ERR_CMD_BASE-15)
//���������Ϊ�˼��ݺ��ŵľɽӿڹ��ܶ�����for RFcard7
#define	ERR_HS_SETICTYPE			(ERR_CMD_BASE-16)
#define	ERR_HS_SETICTYPE1			(ERR_CMD_BASE-17)
#define	ERR_HS_SELRESP_TYPEA		(ERR_CMD_BASE-18)
#define	ERR_HS_APDURW_LEN_SND	(ERR_CMD_BASE-19)
#define	ERR_HS_APDURW_LEN_RCV	(ERR_CMD_BASE-20)
#define	ERR_HS_POWERDOWN		(ERR_CMD_BASE-21)

#define	ERR_RW_BASE				(-10000)

#define	REQA				(0x00)
#define	WUPA				(0x26)	//�Ƽ�ֵ,һ���������Ҫʹ��WUPA
#define	AUTHKEY_TYPE_A	(0x60)
#define	AUTHKEY_TYPE_B	(0x61)
#define	AUTHKEY_TYPE_A1	(0x00)	
#define	AUTHKEY_TYPE_B1	(0x01)
#define	AUTHKEY			((uchar *)"\xff\xff\xff\xff\xff\xff")		//һ�㿨��������ȫFF,��Ҫ�޸�Ҫ�о������,������ʹ��������//"\x30\x31\x32\x33\x34\x35"
#define	BLK02DATA_FF		((uchar *)"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff")
#define	BLK02DATA_ORI		((uchar *)"\x67\x45\x23\x01\x98\xba\xdc\xfe\x67\x45\x23\x01\x02\xfd\x02\xfd")
#define	BLK02DATA_INC1	((uchar *)"\x68\x45\x23\x01\x97\xba\xdc\xfe\x68\x45\x23\x01\x02\xfd\x02\xfd")
//����M0���ĳ�ʼ��֤��Կ(����Ĭ����Կ)
#define	AUTHKEY_M0		((uchar *)"\x49\x45\x4D\x4B\x41\x45\x52\x42\x21\x4E\x41\x43\x55\x4F\x59\x46")

/*----------global variables declaration("extern")--------------*/
extern int (*rf_rw)(int, uchar *, uchar *);

/*---------------functions declaration--------------------------*/
extern int  RF_pack_cmd(uchar cmd, uchar *data, int datalen, uchar *cmdstr);
extern int __CMD_Void(uchar type);
//��װ������mifare-1��API�ӿ�
extern int CMD_Init(void);
extern int CMD_Halt(void);
extern int CMD_Request(uchar mode, uchar *out);
extern int CMD_Anticoll(uchar *out);
extern int CMD_Select(uchar *UID, uchar *out);
extern int CMD_StoreKeyE2(uchar mode, uchar sector, uchar *key);
extern int CMD_LoadKeyE2(uchar mode, uchar sector);
extern int CMD_Auth(uchar mode, uchar *UID, uchar sector, uchar block);
extern int CMD_Auth_Ex(uchar mode, uchar block, uchar *key, uchar *UID);
extern int CMD_BLKRead(uchar block, uchar *out);
extern int CMD_BLKWrite(uchar block, uchar *wdata);
extern int CMD_BLKInc(uchar block, /*uint*/uchar *wdata);
extern int CMD_BLKDec(uchar block, /*uint*/uchar *wdata);
extern int CMD_BLKTransfer(uchar block);
extern int CMD_BLKRestore(uchar block);
extern int CMD_Powerdown(ushort msec);
//���������Ϊ�˼��ݺ��ŵľɽӿڹ��ܶ�����for RFcard7
extern int HS_Init(uchar type);
//extern int HS_SetICType(uchar type);
#define	HS_Request_TypeA 	CMD_Request
#define	HS_Anticoll_TypeA	CMD_Anticoll
#define	HS_Select_TypeA	CMD_Select
extern int HS_SelResp_TypeA(uchar *out, int *outlen);
extern int HS_Request_TypeB(uchar mode, uchar *out);
extern int HS_Powerup_TypeB(void);
extern int HS_APDUrw(uchar *apdu, int apdulen, uchar *out, int *outlen);
extern int HS_Powerdown(void);

//extern int setpiccquickseek(unsigned char seekmodel);//��������posapi.h���ṩ
extern int mi_iccrw1(int sendlen, uchar *sendbuf, uchar *recebuf);//Ӧ�ò��ṩ
#endif

