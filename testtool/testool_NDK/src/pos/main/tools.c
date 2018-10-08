
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ���Գ��򹤾ߺ���
* file name		: layer.c
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20101217
* directory 		: .\SRC\main\
* description		: 
* related document	: 
* 
************************************************************************
* log			:
* Revision 1.0  20101217 22:34 zhangnw
* ��ԭ�в��Գ��򹫹���(lib.c)�з�����ȡ
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------functions definition---------------------------*/
#if defined ME15CHX||defined ME15B||defined ME66
int putchar(int c)
{
	NDK_PortWrite(PORT_NUM_USB, 1, (char*)&c);
	
	if (c == '\n') 
	{
		putchar('\r');
	}
	return c;
}
int NDK_ByteToBcd(int nNum, uchar *psCh)
{
	if (psCh==NULL) {
		return NDK_ERR_PARA;
	}

	if ((nNum < 0) || (nNum > 99)) {
		return NDK_ERR_PARA;
	}
	*psCh = ((nNum / 10) << 4) | (nNum % 10);
	return NDK_OK;
}
int NDK_CalcLRC(const uchar *psBuf, int nLen, uchar *ucLRC)
{
	int	i;

	if ((psBuf == NULL) || (ucLRC==NULL) ||
         (nLen <= 0) ) {
		return NDK_ERR_PARA;
	}

	*ucLRC = 0x00;
	for (i = 0; i < nLen; i++) {
		*ucLRC ^= psBuf[i];
	}
	return NDK_OK;
}
#endif

#if K21_ENABLE
#if defined ME20||defined ME15CHX||defined ME15B||defined ME66//defined ME11||defined ME15||defined ME15C  //��֧��NDK_SysGetPosTime�����Ĳ�Ʒ��򿪸ÿ���
time_t g_times = 0;
time_t time(time_t *calptr)
{
	sleep(1);
	g_times++;
	
	if(calptr!=NULL)
		*calptr = g_times;
	
	return g_times;
}
#else
time_t time(time_t *calptr)
{
	struct tm stOldPosTime;
	time_t tmp;
	
	if(NDK_SysGetPosTime(&stOldPosTime) != NDK_OK)
		return 0;

	tmp = mktime(&stOldPosTime);
	
	if(calptr!=NULL)
		*calptr = tmp;
	
	return tmp;
}
#endif
#endif

void Beep(int sec)
{
	/*private & local definition*/
	time_t oldtime = time(NULL);

	/*process body*/
	while((time(NULL)-oldtime)<=abs(sec))
		NDK_SysBeep();
}

#if KB_DISABLE
int lib_kbflush(void){return NDK_PortClrBuf(g_com);}

int lib_kbhit(void)
{
	/*private & local definition*/
	int nKeyin = 0, len = 0;

	/*process body*/
#if 0//ANDIROD_ENABLE
	NDK_AZPortRead(g_com, 1, (char *)&nKeyin, 0, &len);
#else
	NDK_PortRead(g_com, 1, (char *)&nKeyin, 0, &len);
#endif
	return nKeyin;
}

int lib_getkeycode(uint sec)
{
	/*private & local definition*/
	int nKeyin = 0, len = 0;

	/*process body*/
#if  0//ANDIROD_ENABLE
	NDK_AZPortRead(g_com, 1, (char *)&nKeyin, (sec<=0)?MAX_SIGNINT:(sec*1000), &len);
#else
	NDK_PortRead(g_com, 1, (char *)&nKeyin, (sec<=0)?MAX_SIGNINT:(sec*1000), &len);//NL829STDû��KBӲ��,����������
#endif
	return nKeyin;
}
#else
//����libǰ׺,��ʾ�����ھɵ�API
#if !K21_ENABLE
int lib_kbhit(void)
{
	/*private & local definition*/
	int nKeyin = 0;

	/*process body*/	
	NDK_KbHit(&nKeyin);
	return nKeyin;
}
#else
int lib_kbhit(void){return lib_getkeycode(1);}
#endif
#if !ANDIROD_ENABLE
int lib_getkeycode(uint sec)
{
	/*private & local definition*/
	int nKeyin = 0;

	/*process body*/
	NDK_KbGetCode(sec, &nKeyin);
	return nKeyin;
}
#endif
#endif

/****************************************************************
* function name 	 	: wait_anykey
* functional description 	: 
* input parameter		: ����Ϊ0ʱ��һֱ�ȴ��������˳�
* output parameter	 	: 
* return value			: ��ʱ����0;����������Ӧ�ļ���
* history 		 		: author		date			remarks
*			 		 zhangnw	 20060621     	created 
*****************************************************************/
//��������getkeycode�������Ǳ�������5s beep����
int wait_anykey(int sec)
{
	/*private & local definition*/
	int nKeyin = 0, beepflg = FALSE;
	time_t oldtime = 0, diff = 0 ,diffold = 1;

	/*process body*/
	if (sec<=0)
	{
		NDK_SysBeep();
		lib_kbflush();
		nKeyin = lib_getch();
	}
	else
	{
		lib_kbflush();
		oldtime = time(NULL);
		while((diff=time(NULL)-oldtime)<sec && (nKeyin=lib_kbhit())==0)//ע��,����ɨ�����1s��ִ�ж�ε�
		{	
			if(diffold!=diff)//ʱ�ӷ����仯��ˢ��һ�ε���ʱ,����һֱ����show_stopwatch��ϵͳռ����Դ
			{
				show_stopwatch(ENABLE, sec-diff);//����ʱ
				diffold = diff;
			}
			if(diff%5==0)//5sһ����
			{
				if(beepflg==FALSE)
				{
					NDK_SysBeep();
					beepflg = TRUE;
				}
			}
			else
				beepflg = FALSE;
		}
		show_stopwatch(DISABLE, 0);//�˳�ǰ�����ʱ
	}
	
	return nKeyin;
}

/****************************************************************
* function name 	 	: 
* functional description 	: �ж��ַ����Ƿ���ȫ�����ֹ���
* input parameter	 	: 
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20060704 	 	created
*****************************************************************/
int is_digit_str(char *str)
{
#if K21_ENABLE
	/*private & local definition*/
	int len = 0, i = 0;

	/*process body*/
	//zhangnw20060704 debug isdigit
	//���Է���ԭ����Ϊû�г�ʼ����׼C��,ʹ��isdigit������0
	if ((len = strlen(str))==0)
		return FALSE;
	for (; i<len; i++)
	{
		if (!ISDIGIT(str[i]))
			return FALSE;
	}
	return TRUE;
#else	
	//NDK���ṩ���ƹ��ܺ���ʱ,ʹ��NDK����
	return (NDK_IsDigitStr((uchar *)str)==NDK_OK)?TRUE:FALSE;
#endif
}

//������һ��ֻ�Ƽ�����ͨѶ��ص�ģ��(��,MDM,COM��)ʹ��,�ļ�ϵͳ��ģ�鲻Ҫʹ��,ֱ��ʹ��memcmp
//������������linktype
//��ȷ���SUCC,���ȷ���FAIL(ע��:����ֵ���������׼memcmp����ȫ��ͬ)
int MemCmp(char *sbuf, char *rbuf, int len)
{
	/*private & local definition*/
	
	/*process body*/
	//MDM���Ƚ�5btyes��TPDUͷ
	if(getlinktype()==ASYN)
		return (memcmp(sbuf, rbuf, 3)||memcmp(sbuf+8, rbuf+8, len-8))?FAIL:SUCC;
	else if(getlinktype()==SYNC)
		return memcmp(sbuf+SDLCPCKTHEADERLEN, rbuf+SDLCPCKTHEADERLEN, len-SDLCPCKTHEADERLEN)?FAIL:SUCC;
	else
		return memcmp(sbuf, rbuf, len)?FAIL:SUCC;
}
#if !(defined ME15CHX||defined ME66)
void debug_buf(char *bufname, char *bufaddr, int buflen)
{
	/*private & local definition*/
	int left = buflen;
	char *start = bufaddr;//backup bufaddr,��Ϊbufaddr������arg,��Ҫȥ������ֵ

	/*process body*/
	if(cls_show_msg("�Ƿ�Ӵ���1����%s(����:%dB)?\n[ENTER]����,[����]������", (bufname==NULL)?"":bufname, buflen)==ENTER)
	{
		NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1");
#if 0
		while(left>BUFSIZE_SERIAL)
		{
			NDK_PortWrite(PORT_NUM_COM1, BUFSIZE_SERIAL, start);//portwrite(AUX1, BUFSIZE_SERIAL, start);
			//���·��ͻ�����ַ��ʣ�෢�ͳ���
			start += BUFSIZE_SERIAL;
			left -= BUFSIZE_SERIAL;
			sleep(1);//msdelay(500);//���û��ͣ�ٵز��Ϸ���(msdelay(100)������,500ms��ɴ�sleep(1)����),ʵ�ⷢ�ֻᶪʧ����
		}
#endif
		NDK_PortWrite(PORT_NUM_COM1, left, start);
		sleep(1);
	}
}

uchar MakeLRC(uchar *buf, int len)
{
#if K21_ENABLE
	/*private & local definition*/
	int i = 0;
	uchar c = 0x00;

	/*process body*/
	for (; i<len; i++)
		c^=buf[i];
	
	return c;
#else	
	//NDK���ṩ���ƹ��ܺ���ʱ,ʹ��NDK����
	/*private & local definition*/
	uchar c = 0x00;

	/*process body*/
	NDK_CalcLRC(buf, len, &c);
	return c;
#endif
}

uchar Data2BCD(int data)
{
#if K21_ENABLE
	/*private & local definition*/

	/*process body*/
	ASSERT(data>=0);// && data<100);
	data%=100;//��������λ��ʮλ
	return (data/10*0x10+data%10);
#else	
	//NDK���ṩ���ƹ��ܺ���ʱ,ʹ��NDK����
	/*private & local definition*/
	uchar c = 0x00;

	/*process body*/
	data%=100;//��������λ��ʮλ
	NDK_ByteToBcd(data, &c);
	return c;
#endif
}

/****************************************************************
* function name 	 	: 
* functional description 	: �ú��������ж�����ͨѶѹ���Ƿ���������
* input parameter	 	: 
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		linwl 		   20141119 	 	       ��systest4.c�Ƶ�����
*****************************************************************/
int IsContinuous(int *cur, int *last)
{
	/*private & local definition*/
	int ret = FALSE;

	/*process body*/
	if(*cur==(*last+1))
		ret = TRUE;

	*last = *cur;
	return ret;
}

/******************************************************************************
*	function name 	 	:FGetC
*	functional description :���ļ��ж�ȡһ���ַ�	
*	input parameter	 	:int nFp �򿪵��ļ����
*	output parameter	 	: 
*	return value		 	:
*						�᷵�ض�ȡ�����ַ���
						������-1���ʾ�����ļ�β��
*     history 		 		: author			date			remarks
						jiangym 		    20150415      created
******************************************************************************/
int FGetC(int nFp)
{
	char ch = 0;
	if(NDK_FsRead(nFp, (char *)&ch, 1) != 1)
		return -1;
	else
		return ch;
}
/******************************************************************************
*	function name 	 	:FGets
*	functional description :���ļ��ж�ȡһ���ַ���
*	input parameter	 	:int nMaxLen ��ȡ����󳤶�
*						 int nFp �򿪵��ļ����
*	output parameter	 	: char *pszBuf �������ַ���
*	return value		 	:
*						���ɹ��򷵻�pszBufָ�룬
*						����NULL���ʾ�д�������
*     history 		 		: author			date			remarks
						jiangym 		    20150415      created
******************************************************************************/
char * FGets(char *pszBuf, int nMaxLen, int nFp)
{
	char *p;
	int nC = 0;

	if(pszBuf ==  NULL || nMaxLen <= 0)
		return NULL;
	p = pszBuf;
	while(--nMaxLen)
	{
		if((nC = FGetC(nFp)) == (-1))
			break;
		if ((*p++ = nC) == '\n')
			break;
	}
	if (p > pszBuf)
	{
		*p = 0;
		return pszBuf;
	}
	return NULL;
}
/******************************************************************************
*	function name 	 	:GetParaInfo
*	functional description :���ļ��л�ȡ��Ҫ����Ϣ
*	input parameter	 	:char *ConfName ��Ҫ���ҵ��ַ���
*	output parameter	 	:char *pInfo ���ҵ��ַ�����Ӧ����һ����Ϣ
*	return value		 	:
*						���ɹ��򷵻�SUCC
*						��ʧ�ܷ���FAIL
*     history 		 		: author			date			remarks
						jiangym 		    20150415      created
******************************************************************************/
int GetParaInfo(char *ConfName, char *pInfo)
{
	char szBuf[256] = {0};
	int fp = -1;

	if ((fp = NDK_FsOpen(PARACONF, "r"))<0)
	{	
		cls_show_msg("file:%s,line:%d�򿪲��������ļ�ʧ��(%d))",__FILE__ ,__LINE__,fp);
		return FAIL;
	}
	while(1)
	{
		memset(szBuf, 0, sizeof(szBuf));
		if(FGets(szBuf,sizeof(szBuf)-1,fp) == NULL)	//����ȡ��Ϣ
			break;
		
		szBuf[strlen(szBuf)-1] = '\0';
		if(strstr(szBuf, ConfName) != NULL)	//�����������Ϣ
		{		
			strcpy(pInfo, szBuf );		
			NDK_FsClose(fp);		
			return SUCC;
		}	
	}
	NDK_FsClose(fp);
	cls_show_msg1_record(FILENAME,FUNCNAME,10,"��ȡ%s����ʧ��", ConfName);//ΪWiFi�����Բ��Ըĳɻ�ȡ����ʧ��ֱ�ӱ����ļ������ִ�� 20170927
	return FAIL;
}
/******************************************************************************
*	function name 	 	:trim_string2int
*	functional description :��ȡ"="�ұߵ��ַ�����ת��Ϊ������
*	input parameter	 	:char *inbuf ��������ַ���
*	output parameter	 	:char *numbuf ���"="�ұߵĲ�תΪ������
*	return value		 	:
*
*     history 		 		: author			date			remarks
						jiangym 		    20150415      created
******************************************************************************/
int trim_string2int(char *inbuf, int *numbuf)
{
	char *tempnum;
	if((tempnum = strchr(inbuf, '=')) != NULL)	
	{
		tempnum++;
		if(strlen(tempnum) == 0)
		{
			cls_show_msg("file:%s,line:%d����������%s����Ϊ��,���������ļ��и�ֵ",__FILE__ ,__LINE__,inbuf);
			return FAIL;
		}
		else
			*numbuf = atoi(tempnum);
	}
	return SUCC;
}
/******************************************************************************
*	function name 	 	:trim_string
*	functional description :��ȡ"="�ұߵ��ַ���
*	input parameter	 	:char *inbuf ��������ַ���
*	output parameter	 	:char *numbuf ���"="�ұߵ��ַ���
*	return value		 	:
*
*     history 		 		: author			date			remarks
						jiangym 		    20150415      created
******************************************************************************/
int trim_string(char *inbuf, char *outbuf)
{
	char *tempout;
	if((tempout = strchr(inbuf, '='))!= NULL)	 
	{
		tempout++;		
		if(strlen(tempout) == 0)
		{
			cls_show_msg1(10,"file:%s,line:%d����������%s����Ϊ��,���������ļ��и�ֵ",__FILE__ ,__LINE__,inbuf);
			return FAIL;
		}
		else
			strcpy(outbuf, tempout);
	}
	return SUCC;
}
/******************************************************************************
*	function name 	 	:trim_left_string
*	functional description :��ȡ"="��ߵ��ַ���
*	input parameter	 	:char *inbuf ��������ַ���
*	output parameter	 	:char *outbuf ���"="��ߵ��ַ���
*	return value		 	:
*
*     history 		 		: author			date			remarks
						jiangym 		    20171101      created
******************************************************************************/
void trim_left_string(char *inbuf, char *outbuf)
{
	char *tempout;
	char tempbuf[80]= {0},  tempin[80] = {0};
	
	strcpy(tempin, inbuf);
	if((tempout = strchr(inbuf, '='))!= NULL)	 
	{	
		strcpy(tempbuf, tempout);
	}
	strncpy(outbuf,tempin, strlen(tempin)-strlen(tempbuf));
	return ;
}

/******************************************************************************
*	function name 	 	:trim_char2uchar
*	functional description :��ȡ"="�ұߵ��ַ�����ת����mac����
*	input parameter	 	:char *inbuf ��������ַ���
*	output parameter	 	:uchar *outbuf ���mac����
*	return value		 	:
*
*     history 		 		: author			date			remarks
						jiangym 		    20160308      created
******************************************************************************/
int trim_char2uchar(char *inbuf, uchar *outbuf)
{
	char tempout[32];
	int i = 0, j = 0, k =0;
	if(strlen(inbuf)<=0)
	{
		cls_show_msg("file:%s,line:%d�ַ���Ϊ��",__FILE__ ,__LINE__);
		return FAIL;
	}
	else 
	{
		for(i=0,j=0;i<strlen(inbuf);i++)
		{
			if(inbuf[i]!=':')
			{
				if((inbuf[i]>='0')&&(inbuf[i]<='9'))
					tempout[j] = inbuf[i]-0x30;
				else if((inbuf[i]>='A')&&(inbuf[i]<='F'))
					tempout[j] = inbuf[i]-'A'+10;
				else if((inbuf[i]>='a')&&(inbuf[i]<='f'))
					tempout[j] = inbuf[i]-'a'+10;
				else
					return FAIL;
				j++;
			}
		}
	}
	for(i=0;i<12/*strlen(tempout)*/;i++)//strlen(tempout)����0�����,���mac��00��Ӱ��Mac�������д����ѭ��12��
	{
		k=i/2;j=i%2;
		if(j==0)
			outbuf[k]=tempout[i]<<4;
		else
			outbuf[k]+=tempout[i];
	}
	return SUCC;
}
#endif

