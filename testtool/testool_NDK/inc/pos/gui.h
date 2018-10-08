/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ͼ���û�����
* file name		: gui.h
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20060410
* directory 		: .\SRC\main\
* description		: 
* related document	: 
* 
************************************************************************
* log			: 
* Revision 1.2  20060530 10:34 
* ����POS2005����
* Revision 1.1  20060420 12:34 
* zhangnw modify ֧�ֶ����
* Revision 1.0  20060410 12:34 zhangnw created
************************************************************************/
#ifndef _GUI_H_ 
#define _GUI_H_

/*-----------------------includes-------------------------------*/
#include "flag.h"

/*---------------constants/macro definition---------------------*/
#if !(defined ME15CHX||defined ME15B||defined ME66)
#define MAXDISPBUFSIZE	(512)//(1024)//���⻺��̫�� ʹ�õͶ�ƽ̨��Ʒ��ջ��� linwl20140304
#else
#define MAXDISPBUFSIZE	(256)
#endif

#if 0	//����,ʹ��NDK�Ľӿڻ�ȡ������
#if defined DEMO||defined GP730
#define	g_cMachineModal	(730) 	
#define	LCDCOL				(16)
#define	LCDROW				(20-2)	//�����и�״̬��(�ɹ̼�����)
#elif defined GP720
#define	g_cMachineModal	(8200) 
#define	LCDCOL				(16)		//x����
#define	LCDROW				(8)		//y����
//�����������������ں���
#endif
#endif

/*----------global variables declaration("extern")--------------*/
#if PRN_ENABLE||!SCR_DISABLE
extern const char NL_Icon[32];
extern const char NL_BMP[1024];
extern const char CCB_BMP[1024];
extern const char CCB_BMP_144[1152];
#endif
/*---------------functions declaration--------------------------*/
uint GUI_GetLCDWidth(void);
uint GUI_GetLCDHeight(void);
int GUI_GetLCDSize(void);
uint GUI_GetFontWidth(void);
uint GUI_GetFontHeight(void);
int GUI_GetFontSize(void);

#if 0//ANDIROD_ENABLE   //JNI����NDK�ӿ� �ʲ���ʹ��adb�Ӵ������ ����ش��ڲ����ر� 20170726 jym
int setComConfig(int fd,int baud_rate, int data_bits, char parity, int stop_bits,char ir_en,char block_en);
int NDK_AZPortOpen(EM_PORT_NUM emPort, const char *pszAttr);
int NDK_AZPortRead(EM_PORT_NUM emPort, uint unLen, char *pszOutbuf,int nTimeoutMs, int *pnReadlen);
int NDK_AZPortWrite(EM_PORT_NUM emPort, uint unLen,const char *pszInbuf);
int NDK_AZPortClose(EM_PORT_NUM emPort);
#endif

#if !ANDIROD_ENABLE
int ShowMessageBox(char* pMsg, char cStyle, int iWaitTime);
int cls_printf(const char* msg, ...);
int cls_show_msg1(int time, const char* msg, ...);
int cls_show_msg(const char* msg, ...);
int cls_show_msg1_record(char* filename, char* funname,int time, const char* msg, ...);
int cls_show_msg_record(char* filename, char* funname,const char* msg, ...);
#endif
void GUI_DispStringEx(int x, int y, char* pMsg, EM_TEXT_ATTRIBUTE iTextAttr);
int show_msg1(int time, const char* msg, ...);
int show_msg(const char* msg, ...);
void show_stopwatch(int enable, time_t clk);

#endif

