
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ���Գ��򹫹�������
* file name		: lib.c
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20060628
* directory 		: .\SRC\main\
* description		: 
* related document	: TCR690 API���Գ��� ��ϸ���˵����.doc
* 
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>
#if ANDIROD_ENABLE
#include <dlfcn.h>
#include <android/log.h>
#endif

/*---------------constants/macro definition---------------------*/

/*------------global variables definition-----------------------*/
int g_UCID = 0;
char NowSystestName[64]={0};
int g_SequencePressFlag = 0; //�ۺϲ��� ����ѹ���Ŀ���  0:����������ѹ������  1:��������ѹ������
int auto_flag = 1;//���Է�ʽ���� 0:��ʾ���Զ���ϲ��� 1:��ʾ���Զ������� 2.��ʾ���ֶ�����
int packetnum = 0;
int g_wifistopflag = 1;//��wifi������������1:��ʾ����ʵ�ʹҶ�  0:��ʾ�����йҶ�
#if ANDIROD_ENABLE
int (*cls_show_msg)(const char* msg, ...) = NULL;
int (*cls_show_msg1)(int time, const char* msg, ...) = NULL;
int (*ShowMessageBox)(char* pMsg, char cStyle, int iWaitTime) = NULL;
int (*cls_printf)(const char* msg, ...) = NULL;
int (*lib_kbgetinput)(char * pszBuf,uint unMin,uint unMaxLen,uint * punLen,EM_INPUTDISP emMode,uint unWaittime, EM_INPUT_CONTRL emControl) = NULL;
int (*lib_getkeycode)(uint sec) = NULL;
void (*send_result)(char *fmt, ...) = NULL;
int (*NDK_ScrPrintf)(const char *psFormat, ...) = NULL;
int (*NDK_ScrRefresh)(void) = NULL;
int (*NDK_ScrClrs)(void) = NULL;
int (*cls_show_msg_record)(char* filename, char* funname,const char* msg, ...) = NULL;
int (*cls_show_msg1_record)(char* filename, char* funname,int time, const char* msg, ...) = NULL;
#endif

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: send_result
* functional description 	: 
* input parameter		: 
* output parameter	 	: 
* return value			: 
* history 		 		: author		date			remarks
*			 		 zhangnw	 20060320     	created
*					 zhangnw	 20060602   	   delete �����int port����,ֱ��ʹ��g_com
*****************************************************************/
#if !ANDIROD_ENABLE
void send_result(char *fmt, ...)
{
	/*private & local definition*/
	char tmp[512] = {0}, result[400] = {0};//zhangnw20060704 modify 256->512
	va_list argptr;
	char * p_result = result;

	/*process body*/
	va_start(argptr, fmt);
#if SPRINTF_UNSPORT
	if(t_print(&p_result, fmt, argptr)>sizeof(result)-1) 
	{
		cls_show_msg("%s, line %d:��Ϣ̫��,������˳�...", __FILE__, __LINE__);
		NDK_SysReboot();//nlexit(-1);
	}
#else
	if(vsprintf(p_result, fmt, argptr)>sizeof(result)-1) 
	{
		cls_show_msg("%s, line %d:��Ϣ̫��,������˳�...", __FILE__, __LINE__);
		NDK_SysReboot();//nlexit(-1);
	}
	va_end(argptr);
#endif 
	
	if (send_flag)
	{
		sprintf(tmp, "%12d%45c%20s\r\n", g_UCID, ' ', result);//��ʽ�ɵ���
#if 0//ANDIROD_ENABLE
		NDK_AZPortWrite(g_com, strlen(tmp), tmp);
#else
		NDK_PortWrite(g_com, strlen(tmp), tmp);//�ڶ���para���ܳ���1000
#endif
	}
	else//�����ͣ������ɸ�Ϊ������ʽ��ɣ����ӡ�������
		;
	
	cls_show_msg1(2, "%d������:%s", g_UCID, result);
}
#endif
/****************************************************************
* function name 	 	: 
* functional description 	: ��һ�����ո�ָ��������ַ���ת��Ϊ����
* input parameter	 	: str: �������ֵĴ�;
*					  cnt: array��������ĸ���
* output parameter		:
* return value		 	: ʵ��ת���ĸ���
* history 		 		: author		date		  		remarks
*				  	zhangnw 	 20060308     	 created
*				 	zhangnw 	 20060309      ��get_line()������Ĵ������ٳ���\n�����Դ˴�����Ӧ�޸�
*					zhangnw 	 20060606   	���Ӵ���,ʹ��array�е����ݲ��ظ�
*****************************************************************/
static int digitstr2array(char* str, unsigned char* array, int cnt)
{
	/*private & local definition*/
	unsigned int i = 0, j = 0;
	int found = FALSE;

	/*process body*/
	memset(array, 0, cnt);//memset(array,0,cnt*sizeof(int));
	while (!ISDIGIT(*str))
	{
		//�������ж�\0֮��str++;
		if (*str=='\0'||str==NULL)
			return i;
		
		str++;
	}

	for (i = 0; i<cnt; i++)
	{
		for (j = 0 ; j<i ; j++)
		{
			if (array[j]==atoi(str))
			{
				found = TRUE;
				break;
			}
		}
		if (found==FALSE)
			array[i] = atoi(str);
		else
		{
			i--;
			found = FALSE;
		}

		while (ISDIGIT(*++str));
		
		if (*str=='\0')
			return ++i;
		
		while (!ISDIGIT(*str))
		{
			//�������ж�\0֮��str++;
			if (*str=='\0')
				return ++i;
			
			str++;
		}
	}
	return i;
}

/****************************************************************
* function name 	 	: 
* functional description 	: ����ѡ�����
* input parameter	 	: ȫ�����Ա�ʾ���Խ���ȫ��Χ��ѡ��(�˹����ݲ�ʵ��);
*					  ����Ϊֻ��sel_modģ�鼰funcpointer�ĺ���������ѡ��
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20060704 	 	created
*****************************************************************/
int select_test(unsigned char select_id[], int cnt)
{
	/*private & local definition*/
	int ret = 0;
	uint len = 0;
	char buf[512] = {0};

	/*process body*/
	memset(buf, 0, sizeof(buf));
#if 0//KB_DISABLE	//�Ӽ��̻�ȡ����
	cls_printf("������������(��.�ָ�):\n");
	NDK_PortClrBuf(g_com);//clrportbuf(g_com, 1);//����ջ���
	NDK_PortRead(g_com, 100, buf, 6*1000, &len);//portread(g_com, 100, buf, 10);//10s->6s���100���ַ�������
	NDK_PortWrite(g_com, len, buf);//��ѡ����������Ե����ڣ��ܹ�����������ʲô����
#endif
//#else	ʹ�÷�װ����lib_kbgetinput
	cls_printf("������������(��.�ָ�):\n");
	//if ((ret=getnumstr(buf, sizeof(buf)-1, NORMAL, 0))==QUIT||ret==TIMEOUT)//quit�Ļ�,��ôselect_id��û�ı�,?//zhangnw20110622ע:getnumstr(0)����ΪΪ����:1.��ͳ����Զ�ȴ�;2.������PCI��ȫ���һ�µĴﵽ���ʱʱ����
	if ((ret=lib_kbgetinput(buf, 0, sizeof(buf)-1, &len, INPUTDISP_NORMAL, 0, /*INPUT_CONTRL_LIMIT*/INPUT_CONTRL_NOLIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		return 0;//return SUCC;/*FAIL*///zhangnw20060704 QUIT Ҳ����SUCC

	return digitstr2array(buf, select_id, cnt);//����ýӿڷ���0��ôselect_id��û�ı�,ֱ�ӷ���
	//return SUCC;
}

/****************************************************************
* function name 	 	: process
* functional description 	: ������,���Ը����������ص�
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20060704 	 	created
*****************************************************************/
//ע��:���ڹ����ѽ�idX���ظ������ݹ���,����������,���ӿڲ����ٿ������������
//ʹ��(u)char����Ϊid������,��Ҫ���ںܶ�ط������ٴ������С��(strlen�ɵ�),�����Ҳ��Լ��ϵͳģ����������಻�ɳ���255
int process(PF* list, unsigned char id[])
{
	/*private & local definition*/
	int nKeyin = 0, i = 0, j = 0, cnt = 0;
	PF tmp = NULL;
	unsigned char select_id[MAXID] = {0};//��ʵ,�ĵ���˵"������Ĭ��char��Ϊ��unsigned char",�������Ƕ����unsigned
	int found = FALSE, sel_flag = FALSE;
	char msg[32] = {0};

	/*process body*/
	//ע��:��˾��Ʒƽ̨����ʹ���й��ڴ����Ŀ⺯��(malloc/calloc�Ȳ���ʹ��),Ҫ���ȷ���
#if 0
	memset(select_id, 0, sizeof(select_id));
	memcpy(select_id, id, sizeof(id));
	cnt = sizeof(id)/sizeof(int);
#else
	cnt = strlen((char *)id);
	strcpy((char *)select_id, (char *)id);
#endif
restart:
	for (i = 0; select_id[i]!=0; i++)
	{
		for (j = 0; j<cnt; j++)
		{
			if (select_id[i]==id[j])//j��Ӧ�Ǹ����������,����������?
			{
				found = TRUE;
				nKeyin = lib_getkeycode(2);//nKeyin = getkeycode(2);
				switch(nKeyin)
				{
				case ESC:
					//�˳�����ģ��Ĳ���
					return FAIL;
					break;
				case '9':
					//����������ӿ���ѡ�����
					while (1)
					{
						nKeyin = ShowMessageBox("������ʾ����\n���п�ѡ������?", BTN_OK|BTN_CANCEL, -1);
						if (nKeyin==BTN_OK)
						{
							sel_flag = TRUE;
							break;
						}
						else //if (nKeyin==BTN_CANCEL)
						{
							sel_flag = FALSE;
							break;
						}
					}

					if (sel_flag==TRUE)
					{
						if (select_test(select_id, MAXID)>=SUCC)
						{
							found = FALSE;//Ϊ��һ��Ѱ�Ҹ�λ
							goto restart;
						}
						else
							return FAIL;
					}
					//����ִ��tmp = list[j];//break;
				default: //����,timeout����0
					g_UCID = id[j];
					tmp = list[j];
					break;
				}

				tmp();
				//�����Ӳ��Ὣ�ظ��������˵�//break;
			}
		}

		//û�ҵ�,������
		if (found==FALSE)
		{
			sprintf(msg, "������ʾ����\nδ�ҵ�%d������", select_id[i]);
			while (ShowMessageBox(msg, BTN_OK, -1)!=BTN_OK);
			//�ݲ�ֱ�ӷ���,�������Լ���֮����ڵ�id����//return FAIL;
		}

		found = FALSE;//ƥ���־��λ
	}
	return SUCC;
}

/****************************************************************
* function name 	 	: 
* functional description 	: ���ٷ�ҳ����
* input parameter	 	: 
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20060704 	 	created
*****************************************************************/
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
int jump2page(int maxpage, int* curpage)
{
	/*private & local definition*/
	char page[4] = {0};
	int oldpage = *curpage, newpage = 0, cnt = 0, flag = TRUE, ret = 0;
	uint len = 0;

	/*process body*/
	while (1)
	{
		do
		{
			memset(page, 0, sizeof(page));
			if (!flag)
			{
				show_msg1(10, "\n���벻����,������!\n");
				if (++cnt==3)
					return FAIL;
			}

#if 0//KB_DISABLE	//�Ӽ��̻�ȡ����
			cls_printf("��ǰ���ڵ�%dҳ\n"
				"������˵�ҳ(1---%d):\n", oldpage, maxpage);
			NDK_PortClrBuf(g_com);//����ջ���
			NDK_PortRead(g_com, 1, page, MAX_SIGNINT, &len);//NL829STDû��KBӲ��,����������	
#endif			
//#else  ʹ�÷�װ����lib_kbgetinput
			cls_printf("��ǰ���ڵ�%dҳ\n"
				"������˵�ҳ(1---%d):\n", oldpage, maxpage);
			if ((ret=lib_kbgetinput(page, 0, sizeof(page)-1, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
				return  FAIL;
		}while (!(flag=is_digit_str(page)));

		newpage = atoi(page);
		if (newpage<1 || newpage>maxpage)
		{
			show_msg1(10, "\n����ҳ��Խ��!\n");
			flag = FALSE;
			continue;
		}
		else
		{
			*curpage = newpage;
			return SUCC;
		}
	}
}
#endif
/****************************************************************
* function name 	 	: 
* functional description 	: �Զ�����
* input parameter	 	: 
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20060704 	 	created(��nlmain����ȡ)
*****************************************************************/
void auto_test(int* auto_flag, int* nKeyin, int* SelPage, int maxpage, const char* tcrmenu[], const signed char *skip_modid)
{
	/*private & local definition*/
	int loop = 0, modid = 0;

	/*process body*/
	//ASSERT(*auto_flag==FALSE || *auto_flag==TRUE);
	
LOOP:
	while (1)
	{
		if (*auto_flag==FALSE||*auto_flag==2)//������Ժ��ֶ�������Ҫ��������
		{
			//��ʾ�˵�
			*nKeyin = cls_show_msg(tcrmenu[*SelPage-1]);//g_menu������menu1.h ��
			return;
		}
		else if (*auto_flag==TRUE)//�Զ�����ʱnKeyin��ֵ ���ɼ����ṩ
		{
			//step1:��ʾ���ڽ��еĲ�����Ŀ���ȴ�һЩʱ�䣬���������˳��Զ����ԣ�
			if (ESC==cls_show_msg1(2, "����ȫ�Զ�������...\nESC�ж�ȫ�Զ�����"))
			{
				*auto_flag = FALSE;
				continue;
			}

			//step2 :�Զ�����*nKeyin��*SelPage��
			++*nKeyin;
			if (*nKeyin=='9')
			{
				*nKeyin = '1';
				if (++*SelPage>maxpage)//��Ϊ�Զ����Ե���ֹ����
				{
					//��ʾ�Զ��������
					while (1)
					{
						if (ShowMessageBox("������ʾ����\n�Զ��������", BTN_OK, 5)==BTN_OK)//��ʱʱ��-1 �޸ĳ�5��
						{
							if(*auto_flag==1)//�Զ�����������
								NDK_SysReboot();
							
							*auto_flag = FALSE;//�Զ����Խ���,��־��λ
							break;
						}
					}

					*SelPage = 1;//��λ
					continue;
				}
			}

			//zhangnw20061116 ʹ��skip_modid�����˲������Զ����ԵĲ�����
			modid = (*SelPage-1)*MAXCOUNT+*nKeyin-'1';
			for (loop=0; skip_modid[loop]!=-1; loop++)//-1��skip_modID�Ľ�����־
			{
				if (skip_modid[loop]==modid)
					goto LOOP;
			}

			return;
		}
	}
}

/*
 * Function:	   send_reporthead
 * Description:    ��ʼ���ɹ�����PC�˷��Ͳ��Ա���ͷ
 * Calls:   	   
 * Called By:      init()
 * Table Accessed: 
 * Table Updated:  
 * Input:   	   
 * Output:  	   
 * Return:  	   
 * Others:  	   zhangnw20060831 ��init()�з������
 */
void send_reporthead(void)
{
	/*private & local definition*/
	time_t now = 0;
	struct tm *t;
	char tmp[384] = {0}, BoardVer[128] = {0}, BIOSVer[32] = {0}, machine[32] = {0};
	uint len = 0;
#if 0	//defined E90
	int adjust = 1940;
#else
	int adjust = 1900;
#endif

	/*process body*/
	if (send_flag)
	{
		time(&now);
		t = localtime(&now);

		if(NDK_OK!=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TYPE, &len, machine))		
			strcpy(machine, "UNKNOWN");

		if(NDK_OK!=NDK_SysGetPosInfo(SYS_HWINFO_GET_BIOS_VER, &len, BIOSVer))		
			strcpy(BIOSVer, "UNKNOWN");
		
		if(NDK_OK!=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOARD_VER, &len, BoardVer))		
			strcpy(BoardVer, "UNKNOWN");
		
#if 0	//zhangnw201101����
		sprintf(tmp, REPORTHEAD, ctime(&now), SlaveCPUVer, BIOSVer, HandwareVer);
#else
		sprintf(tmp, REPORTHEAD, 
				t->tm_wday, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, t->tm_year+adjust, 
				machine, BIOSVer, BoardVer);
#endif
		while(NDK_PortTxSendOver(g_com)!=NDK_OK);//���з����ʱ��//clrportbuf(g_com, 0);
#if 0//ANDIROD_ENABLE
		NDK_AZPortWrite(g_com, strlen(tmp), tmp);
#else
		NDK_PortWrite(g_com, strlen(tmp), tmp);
#endif
	}
	else
		;
}

#if !defined ME66 //ME66�ռ�С������
#if PRN_ENABLE
int print_Ex(char *buf, ...)
{
	/*private & local definition*/
	int cnt = 0;
	va_list args;
	char prnbuf[MAXPRNBUFSIZE] = {0};
	char * p_prnbuf = prnbuf;
	/*process body*/
	memset(prnbuf, 0, sizeof(prnbuf));

	va_start(args, buf);
#if SPRINTF_UNSPORT
	cnt = t_print(&p_prnbuf, buf, args);
#else
	cnt = vsprintf(p_prnbuf, buf, args);
	va_end(args);
#endif 

	if (cnt>sizeof(prnbuf)-1)
	{
		cls_show_msg("%s, line %d: ����ӡ���ݳ���,������˳�...", __FILE__, __LINE__);
		NDK_SysReboot();//nlexit(-1);
	}
	return NDK_PrnStr(prnbuf);//print(prnbuf);
}

int lib_getprinterlen(void)
{
	/*private & local definition*/
	uint unLen=0;
	char szBuf[128]={0};

	/*process body*/
	if(NDK_SysGetPosInfo(SYS_HWINFO_GET_PRN_LEN ,&unLen,szBuf)!=NDK_OK)
		return FAIL;
	return atoi(szBuf);
}

int lib_getprintertype(void)
{
	/*private & local definition*/
	uint unLen=0;
	char szBuf[128]={0};
	
	/*process body*/
	if(NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO,&unLen,szBuf)!=NDK_OK||szBuf[10]==0xFF)
		return FAIL;
	
	return (szBuf[10]>=0x81/*0x82*/)?PRN_TYPE_HIP:PRN_TYPE_TP;//��������ִ��׻���Ħ��,ͳһ���ش���
}
#endif

//MAG
#if MAG_ENABLE
char __check_stripe_result_tk23(char *track, char *name)
{
	/*private & local definition*/

	/*process body*/
	if(track[0]==0x7f||track[0]==0x7e)
		return track[0];
	else
	{
		cls_show_msg1(2, "%s�ŵ�����(%d��):%s\n", name, strlen(track), track);//�Ͷ�ƽ̨��֧��NDK_ScrDispString(0, 0, track, 1)�еĲ���1�������޸Ĵſ�������ʾ��ʽ 20141023 linwl
		//cls_show_msg1(2, "%s�ŵ�����(%d��):", name, strlen(track));
//#if SCR_DISABLE
//		NDK_PortWrite(g_com, strlen(track), track);//portwrite(g_com, strlen(tips), tips);
//		NDK_PortWrite(g_com, strlen("\r\n"), "\r\n");//portwrite(g_com, strlen("\r\n"), "\r\n");
//#endif
		//NDK_ScrDispString(0, 0, track, 1);
		//NDK_ScrRefresh();
		//wait_anykey(30);
	}

	return 0x00;
}

char check_stripe_result_tk2(char *track2)
{
	/*private & local definition*/

	/*process body*/
	return __check_stripe_result_tk23(track2, "��");
}

char check_stripe_result_tk3(char *track3)
{
	/*private & local definition*/

	/*process body*/
	return __check_stripe_result_tk23(track3, "��");
}

char check_stripe_result_tk1(char *track1)
{
	/*private & local definition*/

	/*process body*/
	NDK_ScrInitGui();
	if(track1[0]==0x7f)
		return track1[0];
	else if(track1[0]==0x7e)
	{
		if(ENTER==cls_show_msg("��һ�ŵ�������.���������ݻ�Ӳ����֧��1�ŵ�,�밴ȷ��,����������"))
			;
		else
			return track1[0];
	}
	else
	{
		cls_show_msg1(2, "һ�ŵ�����(%d��):%s\n", strlen(track1), track1);
//		cls_show_msg1(2, "һ�ŵ�����(%d��):", strlen(track1));
//#if SCR_DISABLE
//		NDK_PortWrite(g_com, strlen(track1), track1);//portwrite(g_com, strlen(tips), tips);
//		NDK_PortWrite(g_com, strlen("\r\n"), "\r\n");//portwrite(g_com, strlen("\r\n"), "\r\n");
//#endif
//		NDK_ScrDispString(0, 0, track1, 1);
//		NDK_ScrRefresh();
//		wait_anykey(30);
	}

	return 0x00;
}

#if !defined READCARD3_SUPPORT
//һ�����׵�readcard3����
//ʹ��nl_mag������ʵ��һ����readcard3�����൱�Ľӿ�,��֧��ˢ�ſ�
//���ڷ���ֵ�����:�����API�п��ܷ�����ͬ�Ĵ��󷵻�ֵ���޷������Ǻδ�����ʱ,�������ӳ�����ʾ�򽫳�����__LINE__��Ϊ����ֵ
int readcard3(/*int type, */int tracks, /*int keylen, */int waittime, char *track2, char *track3 , char *track1, int *err)
{
	/*private & local definition*/
	int nRet = FAIL, ret1 = FAIL/*, ret2 = FAIL, ret3 = FAIL*/;
	time_t oldtime = 0, diff = 0;
	uchar Swiped = 0;

	/*process body*/
	//type,keylen���������ж�
#if defined ME15CHX ||defined ME15B|| ANDIROD_ENABLE
	NDK_MagClose();//���Ӹ��رն���,�����ظ��򿪷��ش���
#endif
	if((ret1=NDK_MagOpen())!=NDK_OK)
		return ret1;//FAIL;
	
#if !(defined ME15CHX||defined ME30MH||defined ME15B||defined ME32||defined N920)
	oldtime = time(NULL);
	NDK_KbFlush();
	do
	{
		if(lib_kbhit()==ESC)//if(kbhit()==ESC)
		{
			nRet = NDK_ERR_QUIT;//QUIT;
			break;
		}
			
		if(NDK_MagSwiped(&Swiped)==NDK_OK&&Swiped==TRUE)//��ѯֱ���дŵ�����(��������)
		{
			if((ret1=NDK_MagReadNormal(tracks&TK1?track1:NULL, tracks&TK2?track2:NULL, tracks&TK3?track3:NULL, err))!=NDK_OK)	
				nRet = ret1;//FAIL;
			else
				nRet = STRIPE;
			
			NDK_MagReset();//����Ҷ��ͳ�ṩ�������ڶ���֮��Ҫ��λ��ͷ������ſ����������� 20140304 linwl
			break;
		}
	}while(waittime==0||(diff=time(NULL)-oldtime)<waittime);
	
	if(waittime!=0 && diff>=waittime)//��ʱ
		nRet = NDK_ERR_TIMEOUT;//TIMEOUT;
#else
	while(1)
	{
		NDK_MagSwiped(&Swiped);
		if(Swiped==TRUE)//��ѯֱ���дŵ�����(��������)
			break;
	}
	if((ret1=NDK_MagReadNormal(tracks&TK1?track1:NULL, tracks&TK2?track2:NULL, tracks&TK3?track3:NULL, err))!=NDK_OK)
		nRet = ret1;//FAIL;
	else
		nRet = STRIPE;
#endif
	
	if((ret1=NDK_MagClose())!=NDK_OK)//��accept�ɶ�ʹ��
		return ret1;//FAIL;

	return nRet;
}
#endif
#endif
#endif

void lib_StartStopwatch(void)
{

#if K21_ENABLE
	uint a = 0;
	NDK_SysStopWatch(&a);
#endif	

	NDK_SysStartWatch();
	return;
}

float lib_ReadStopwatch(void)
{
	/*private & local definition*/
	uint time = 0;
	
	/*process body*/
#if !K21_ENABLE
	NDK_SysStopWatch(&time);
#else
	NDK_SysReadWatch(&time);
#endif
	return time/1000.0;
}
#if defined ME30GX||defined ME32GX
int INT_ReadStopwatch(void)
{
	/*private & local definition*/
	uint time = 0;
	
	/*process body*/
	NDK_SysReadWatch(&time);
	return time/1000;
}
#endif

//�޼��̲�Ʒʹ�ü������������ʹ�ô������͵�����̨��
//�����м��̵Ļ���ʹ��ԭ�ȵļ��̽ӿں���  20141031 jiangym add  ���ô˺������ԭ�ȵ�NDK_KbGetInput
#if !ANDIROD_ENABLE
int lib_kbgetinput(char * pszBuf,uint unMin,uint unMaxLen,uint * punLen,EM_INPUTDISP emMode,uint unWaittime, EM_INPUT_CONTRL emControl)
{
	int ret = -1;
#if KB_DISABLE
	int len = 0, i = 0; //��������
	char tempbuf[100] = {0};
	strcpy(tempbuf,pszBuf);
	cls_printf("%s",pszBuf);
	memset(pszBuf,0,strlen(pszBuf));
	NDK_PortClrBuf(g_com);//����ջ���
	
	//while(NDK_PortRead(g_com, unMaxLen, pszBuf, 3*1000, &len)==-10)//�˴�NDK.SO��װ������unMaxLen ��ʱδ����unMaxLen���ȵ����ݷ���-10�����Ǻ��еͶ�һ������ʵ�ʶ����ĳ���,�Ѿ��޸�,20150819linwl
	//	;
#if 0//ANDIROD_ENABLE
	NDK_AZPortRead(g_com, unMaxLen, pszBuf, 5*1000, &len);
#else
	NDK_PortRead(g_com, unMaxLen, pszBuf, 5*1000, &len);
#endif
	if(len==0)
	{
		strcpy(pszBuf,tempbuf);//���û������ʹ��Ĭ�ϵ�,��Ӧ��ʱ�����
		ret = NDK_ERR;
	}
	else if(len==1)
	{
		if(pszBuf[0]==0x0d||pszBuf[0]==0x1b)//���﹦�ܼ�ֻ֧��ESC��ENTER���������ܼ���������(�����óɿ�)
		{
			strcpy(pszBuf,tempbuf);//���ֻ��һ���ֽ���Ϊ���ܼ�����ʹ��Ĭ��ֵ
			ret = NDK_ERR;
		}
		ret = NDK_OK;
	}
	else
		ret = NDK_OK;

	for(i = 1;i<len;i++)//δ����PC���̵Ĺ��ܼ�������󵱳���Ч�ַ�������ȥ���ܼ�����,����������������111�ƺ��˻س���,�������Ƶ�BtName�ͱ�� 111\r ����%s��ʾʱ�ͻ�����쳣
	{
		if(pszBuf[i]==0x0d||pszBuf[i]==0x0a||pszBuf[i]==0x1b)
			pszBuf[i] = 0;
	}
	//cls_printf("0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x",pszBuf[0],pszBuf[1],pszBuf[2],pszBuf[3],pszBuf[4],pszBuf[5],pszBuf[6]);
	
	cls_printf("%s",pszBuf);
	*punLen = len;
#else
	ret =NDK_KbGetInput(pszBuf,unMin,unMaxLen,punLen,emMode,unWaittime,emControl);
#endif
	return ret;
}
#endif

void ftos(float f, char *str)
{ 
	sprintf(str,"%d.%06d", (int)f, (int)((f-(int)f)*1000000));
	return;
}

#if !defined ME66 //ME66��֧������ �ռ�С������
#if BT_ENABLE
int lib_bt_pairing(uint unaccept)
{
	int flag = 0, ret = -1, link_status = 1;
	uint ulen = 0;
	char pkey[7] = {0};
	time_t diff = 0, oldtime = time(NULL);
	char *pairmodestr[]={"Just Work","PIN Code","SSP PIN","PassKey","Jusk Work SC"};

	//cls_show_msg1(1,"����Ϊ%sģʽ",pairmodestr[g_pair_mode]);
	/*process body*/
	if(g_pair_mode == 2 || g_pair_mode == 3)//	2,3����ģʽ����ȴ�������ӣ�һ����Գɹ���Ĭ�����ӳɹ���
	{
		memset(pkey,0,sizeof(pkey));
		while(1)
		{
			if(cls_show_msg1(1,"%sģʽ,�ȴ��ֻ��������߷������������,ȡ�����˳�", pairmodestr[g_pair_mode])==ESC)
				return NDK_ERR_QUIT;
			if((ret=NDK_BTStatus(&link_status))!=NDK_OK)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ȡ����״̬ʧ��(%d)", __LINE__,ret);
				return ret;
			}
			if(!link_status)
			{
				cls_printf("�����ӳɹ�");//��������ʱ�������ȶ��������������:���������շ�,�Ҷ�,��λ��
				sleep(3);
				NDK_BTGetPairingStatus(pkey,&flag);//����Ա�־
				return NDK_OK;
			}
			if((ret=NDK_BTGetPairingStatus(pkey,&flag))!=NDK_OK)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ȡ���״̬ʧ��(%d)", __LINE__,ret);
				return ret;
			}
			switch(flag)
			{
				case 0:
					if((diff=time(NULL)-oldtime)>60)
					{
						cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ʱ��δ��⵽�������(%d)", __LINE__, flag);
						return NDK_ERR_TIMEOUT;
					}
					continue;
					break;
				case 1:
					if(pkey[0] == '\0')
					{
						cls_printf("�����ֻ�����ʾ�������:\n");
						lib_kbgetinput(pkey, 0, 6, &ulen, INPUTDISP_PASSWD, 60, INPUT_CONTRL_LIMIT_ERETURN);
					}
					NDK_BTConfirmPairing(pkey, unaccept);
					break;
				case 2:
					cls_printf("���ӳɹ�");
					sleep(2);//��������ʱ�������ȶ��������������:���������շ�,�Ҷ�,��λ��
					return NDK_OK;
					break;
				case 3:
					cls_show_msg1(2,"���ʧ��,���������");
					return NDK_ERR;
					break;
				default:
					cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��⵽δ֪�����״̬(%d)", __LINE__,flag);
					return NDK_ERR;
					break;
			}
		}
	}
	else//	0,1����ģʽ������Ծͽ���ȴ�����4just work scģʽҲ�ߴ˷�֧
	{
		while(1)
		{
			if(cls_show_msg1(1,"%sģʽ,�ȴ��ֻ��������߷������������,ȡ�����˳�", pairmodestr[g_pair_mode])==ESC)
				return NDK_ERR_QUIT;
			if((ret=NDK_BTStatus(&link_status))!=NDK_OK)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ȡ����״̬ʧ��(%d)", __LINE__,ret);
				return ret;
			}
			if(!link_status)
			{
				cls_printf("���ӳɹ�");//��������ʱ�������ȶ��������������:���������շ�,�Ҷ�,��λ��
				sleep(3);
				return NDK_OK;
			}
			if((diff=time(NULL)-oldtime)>60)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ʱ��δ��⵽���ӳɹ�״̬(%d)", __LINE__, link_status);
				return NDK_ERR_TIMEOUT;
			}
		}
	}
	
	return ret;
}

//�޸�BT����ʽǰ�������ֽڱ�ʾ���ȸ��ֽ���ǰ,����ΪBCD��ʽ�Ұ������ȱ����Լ�У����,���һ����sumcheckУ���롣
void update_BT_packet(char *buf, int len)
{
	/*private & local definition*/
	int i = 0;
	
	/*process body*/
	NDK_ByteToBcd(len/100,(uchar*)buf);
	NDK_ByteToBcd(len%100,(uchar*)buf+1);
	buf[len-1]=0;
	
	for(i=0;i<len-1;i++)
		buf[len-1]+=buf[i];
	
	return;
}

//��Ӧ���ĸ�ʽ(POS�˷��͵��ֻ�APK�˵����ݰ���ʽ)
//STX(1���ֽڹ̶�0x02)+ָ�����ݳ���(2���ֽ�BCD��ʽ:��ָ��ſ�ʼ��ETXǰ���ܳ���)+ָ���(2���ֽڹ̶�0xff 0x0c)+ָʾλ(1���ֽ�)+���к�(1���ֽ�)+��Ӧ(2���ֽ�)+���ò���(1���ֽ�)+data����(2���ֽ�)+����data(datalen���ֽ�)+����λETX(1���ֽ�)+LRCУ��λ(1���ֽ�)
//���ò��� 0x01��Ӧ֪ͨ�ֻ�����ɨ��ʱ����t1ֵ����λs��BCD�루���ã�ID2������ʱ���� T1��ռ 2�ֽڣ�
//���ò��� 0x02֪ͨ�ֻ�����ɨ���豸�������豸��ʱ���� t2 ֵ����λ s��BCD�루���ã�ID2������ʱ���� T2��ռ 2�ֽڣ�
//���ò��� 0x03֪ͨ�ֻ��Ͽ���ǰ���ӣ�������
//���ò��� 0x04֪ͨ�ֻ�ֹͣ���ԣ�������
//���ò��� 0x05���ݻش�����ش����ݣ�
//���ò��� 0x06��Ӧȷ�ϱ��ģ�֪ͨ�ֻ���Ӧ�յ����յ��ش���ȷ���յ���
//���ò��� 0x07�յ��ֻ����ݣ��ش����ݣ�ID 2 Ϊ�ش����ݣ�
int bt_cmd_packet(int cmdid, char *data, int datalen, char* cmd)
{
#if 0
	int i = 0;
#endif
	if(cmdid==1||cmdid==2)//������ʱʱ��ʱ��������ֻ��Ϊ2���ֽڵ�ʱ����ʱ
	{
		if(data==NULL||datalen!=2)
			return BTCMD_SETTIME_ERR;
	}
	if(cmd==NULL)
		return BTCMD_PARA_ERR;
	if(cmdid<1||cmdid>7)
		return BTCMD_ID_ERR;
	if(datalen<0)
		return BTCMD_DATALEN_ERR;

	memset(cmd,0,sizeof(cmd));
	//STX
	cmd[0] = 0x02;
	//�������ݳ���
	NDK_ByteToBcd((datalen+9)/100,(uchar*)cmd+1);
	NDK_ByteToBcd((datalen+9)%100,(uchar*)cmd+2);
	//ָ���
	cmd[3] = 0xff;
	cmd[4] = 0x0c;
	//ָʾλ
	if(cmdid==6||cmdid==7)
		cmd[5] = 0x2f;
	else
		cmd[5] = 0x5f;
	//���к�
	if(packetnum==255)
		packetnum = 1;
	else
		packetnum = packetnum+1;
	cmd[6] = packetnum;//���к�
	//��Ӧ��
	cmd[7] = 0x30;
	cmd[8] = 0x30;
	//���ò��������
	cmd[9] = cmdid;
	//�����������ݳ���
	NDK_ByteToBcd((datalen)/100,(uchar*)cmd+10);
	NDK_ByteToBcd((datalen)%100,(uchar*)cmd+11);
	//��������
	memcpy(cmd+12,data,datalen);
	//������ETX
	cmd[datalen+12] = 0x03;
	//LRC
	NDK_CalcLRC((uchar*)cmd+1,datalen+12,(uchar*)cmd+datalen+13);

#if BTMFI_ENABLE//���ʹ�þɵ��������ߣ���Ҫ�ٶ���������������汾������Ҫ�õ�
	update_BT_packet(cmd, datalen+14);
#endif

#if 0
	fprintf(stderr,"\n0x%2x����:",cmdid);
	for(i=0;i<datalen+MIN_BTCMD_LEN;i++)
		fprintf(stderr,"%02X ",cmd[i]);
	fprintf(stderr,"\n");
#endif	
	return BTCMD_OK;
}

int bt_cmd_uppacket(char* cmd, int datalen, char *data)
{
#if 0
	int i = 0;
	fprintf(stderr,"���յ�����:");
	for(i=0;i<datalen+MIN_BTCMD_LEN;i++)
		fprintf(stderr,"%02X ",cmd[i]);
	fprintf(stderr,"\n");
#endif	

	if(data==NULL||cmd==NULL)
		return BTCMD_PARA_ERR;
	//if(cmd[6]!=packetnum+1)
	//	return BTCMD_SN_ERR;

	packetnum = cmd[6];//�������к�
	memset(data,0,sizeof(data));
	memcpy(data,cmd+12,datalen);//��ȡʵ�ʲ�������
	
	return BTCMD_OK;
}

//packlen:���ݳ��� randflag:�������־
int bt_comm_once(int packlen, int randflag)
{
	int ret = 0, cmdlen=0, recvLen=0, j = 0;
	char sendbuf[BUFSIZE_BT-MIN_BTCMD_LEN]={0}, recbuf[BUFSIZE_BT-MIN_BTCMD_LEN]={0};
	char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};

	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)//����ջ���
	{
		cls_show_msg1(3,"line %d:�建����ʧ��(%d)", __LINE__, ret);
		return NDK_ERR;
	}

	if(randflag == 1)
	{
		for (j=0; j<sizeof(sendbuf); j++)
			sendbuf[j]=rand()%256;
	}
	else
		memset(sendbuf, 0x38, sizeof(sendbuf));
	
	//�������ݻش�����
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������Ӻ��ʼ��������pos�����������");//����MFI�ػ����Թ�����Ҫ�ֶ��������Բ�����ʾ�в���
#else
	cls_show_msg1(3,"3���������ݰ����ͽ��պͱȽ�");
#endif
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendbuf, packlen, cmdsendBuf);
	cmdlen=packlen+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1(3,"line %d:����ʧ��(%d)", __LINE__, ret);
		return NDK_ERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, 10*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		cls_show_msg1(3,"line %d:���մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, ret, recvLen, cmdlen);
		return NDK_ERR;
	}
	bt_cmd_uppacket(cmdrecvBuf, packlen, recbuf);
	if(memcmp(sendbuf, recbuf, packlen))
	{
		cls_show_msg1(3,"line %d:����У��ʧ��", __LINE__);
		return NDK_ERR;
	}
	//������Ӧȷ�ϱ���
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg1(3,"line %d:����ʧ��(%d)", __LINE__, ret);
		return NDK_ERR;
	}
	sleep(1);//������ʱȷ���ֻ��ܹ��յ�06ָ��,������������,����Ҷ�

	return NDK_OK;
}
#else
int lib_bt_pairing(uint unaccept) {return NDK_ERR;}
void update_BT_packet(char *buf, int len){return;}
int bt_cmd_packet(int cmdid, char *data, int datalen, char* cmd) {return NDK_ERR;}
int bt_cmd_uppacket(char* cmd, int datalen, char *data) {return NDK_ERR;}
int bt_comm_once(int packlen, int randflag) {return NDK_ERR;}
#endif

#if !(defined ME15CHX||defined ME15B)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
#if BT_MASTER_ENABLE
//������ģʽɨ���ѡ��Ҫ���ӻ���Ե��豸,ͨ���������ز����豸��MAC��ַ
//�������dectypeΪ�����豸�ķ�ʽĬ��Ϊ0 Ĭ�Ϸ�ʽ����Ӧ��ɨ��ʹ��˫ģ����ɨ�裬1λ��ģSPP��ʽ 2Ϊ��ģBLE��ʽ
int lib_btmaster_scan(char *pszMac, int dectype)
{
	int ret = 0,snum=0,i=0, sacntype=BT_DEVICE_TYPE_DUMO;
	ST_BT_DEV sresult[50];

	switch(dectype) 
	{
		case 0:
			sacntype = BT_DEVICE_TYPE_DUMO; /**<˫ģ*/
			break;
		case 1:	
			sacntype = BT_DEVICE_TYPE_BREDR; /**<��ģ:SPP*/
			break;	
		case 2:
			sacntype = BT_DEVICE_TYPE_BLE; /**<��ģ:BLE*/
			break;
		default:
			sacntype = BT_DEVICE_TYPE_DUMO; /**<˫ģ*/
			break;
	}
	
	//if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	if((ret=NDK_BTMasterScan(sacntype))!=NDK_OK)
	{
		cls_show_msg1(5, "%s,line %d:���豸ɨ��ʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;	
	}
	
	memset(sresult,0,sizeof(sresult));
	while(1)
	{
		if(cls_show_msg1(1,"����ɨ��������ģʽ�豸��ESC���˳�")==ESC)
			break;
		if((ret=NDK_BTMasterGetScanResults(NULL,sresult,15,&snum))!=NDK_OK)
		{
			cls_show_msg("%s,line %d:���豸��ȡɨ����ʧ��(%d)", __FILE__, __LINE__, ret);
			NDK_BTMasterStopScan();
			return ret;
		}
		if (snum > 0)
		{
			for (i=0; i<snum; i++)
			{
				if(cls_show_msg("�Ƿ�ѡ��BT[%d]:%s MACΪ:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x��Ϊ���Դ��豸,�ǰ�[ȷ��],��������", i, sresult[i].sBtName,sresult[i].sBtMac[0],sresult[i].sBtMac[1],sresult[i].sBtMac[2],sresult[i].sBtMac[3],sresult[i].sBtMac[4],sresult[i].sBtMac[5])==ENTER)
				{
					memcpy(pszMac,sresult[i].sBtMac,6);
					NDK_BTMasterStopScan();
					return NDK_OK;
				}
			}
		}
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ֹͣɨ��ʧ��(%d)", __LINE__, ret);
		NDK_BTMasterStopScan();
		return ret;
	}
	cls_show_msg1(2, "line %d:δɨ�赽�����õ��������豸", __LINE__);
	return NDK_ERR_QUIT;
}

//������ģʽ������Ժ���:����ȡ�����Ӿͷ�����δ������������,��Գɹ������ӳɹ�����
int connect_pair_process(char *mac,int accept)
{
	int ret = -1, status = -1, constatus = -1;
	char skey[7]={0},pincode[7]={0};
	EM_PAIRING_MODE mode=0;
	time_t oldtime=0;
	uint unLen = 0;
	
	oldtime=time(NULL);
	while(1)
	{
		//��ȡ����״̬,�ɹ�ֱ�ӷ���
		if((ret=NDK_BTMasterGetConStatus(mac,&constatus))!=NDK_OK)
		{
			cls_show_msg1(2,"%s,line %d:��ȡ����״̬ʧ��%d", __FILE__, __LINE__, ret);
			return ret;
		}
		if(constatus==0)
		{
			cls_show_msg1(2,"���豸����豸�ѳɹ�����");
			return NDK_OK;
		}
		//״̬��δ����������������
		if((ret=NDK_BTMasterGetBondStatus(&mode,skey,&status))!=NDK_OK)
		{
			cls_show_msg1(2,"%s,line %d:��ȡ���״̬ʧ��%d", __FILE__, __LINE__, ret);
			return ret;
		}
		switch(status)
		{
			case 0:
				if(time(NULL)-oldtime>30)//30�����
				{
					cls_show_msg1(2,"%s,line %d:���״̬��ʱ��Ϊ0,�쳣�˳�", __FILE__, __LINE__);
					return NDK_ERR_TIMEOUT;
				}
				continue;
				break;
			case 1:
				if(mode==1)
				{
					cls_printf("PinCodeģʽ:��������豸��PIN��:");
					if ((ret=lib_kbgetinput(pincode,0, sizeof(pincode)-1, &unLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
						accept = 0;
					if((ret=NDK_BTMasterBondConfirm(pincode,accept))!=NDK_OK)
					{	
						cls_show_msg1(2,"%s,line %d:ȷ����Ժ���ʧ��(%d)", __FILE__, __LINE__, ret);
						return ret;;
					}
				}
				else if(mode==2)
				{
					if(cls_show_msg("SPPģʽ:[Enter]ȷ�����,[����]ȡ�����")!=ENTER)
						accept = 0;
					if((ret=NDK_BTMasterBondConfirm(skey,accept))!=NDK_OK)
					{	
						cls_show_msg1(2,"%s,line %d:ȷ����Ժ���ʧ��(%d)", __FILE__, __LINE__, ret);
						return ret;;
					}
				}
				else if(mode==3)
				{
					cls_show_msg("PASSKEYģʽ:���ڴ��豸������key:%s�����������",skey);
				}
				continue;
				break;
			case 2:
				cls_show_msg1(2,"��Գɹ�");//��Գɹ���ײ��������Ӳ�����break�������ȡ����״̬
				break;
			case 3:
				cls_show_msg1(2,"���ʧ��");
				return NDK_ERR;
				break; 
			default:
				cls_show_msg1(2,"%s,line %d:��ȡ��δ֪״̬(%d)", __FILE__, __LINE__, status);
				return NDK_ERR;
				break;
		}
		if(time(NULL)-oldtime>60)//��Ժ�30������
		{
			cls_show_msg1(2,"%s,line %d:���ӳ�ʱ,�쳣�˳�", __FILE__, __LINE__);
			return NDK_ERR_TIMEOUT;
		}
		NDK_SysMsDelay(100);//��ʱ100ms
	}
	return NDK_OK;
}

//������ģʽ������Ժ���:����accept=1��ʾȷ����ԣ�accept=0��ʾȡ�����
//����ֵΪ���״̬��������״̬
int lib_btmaster_pairing(int accept)
{
	int ret = -1, status = 0;
	char skey[7]={0},pincode[7]={0};
	EM_PAIRING_MODE mode=0;
	time_t oldtime=0;
	uint unLen = 0;
	
	oldtime=time(NULL);
	while(1)
	{
		if(cls_show_msg1(1,"��ȡ���״̬��...")==ESC)
			break;
		if((ret=NDK_BTMasterGetBondStatus(&mode,skey,&status))!=NDK_OK)
		{
			return ret;
		}
		switch(status)
		{
			case 0:
				cls_printf("���״̬Ϊ0");
				if(time(NULL)-oldtime>60)
				{
					cls_show_msg1(2,"��ʱ��δ�����������");
					return NDK_ERR_TIMEOUT;
				}
				continue;
				break;
			case 1:
				if(mode==1)
				{
					cls_printf("��������豸��PIN��:");
					if ((ret=lib_kbgetinput(pincode,0, sizeof(pincode)-1, &unLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
						accept = 0;
					if((ret=NDK_BTMasterBondConfirm(pincode,accept))!=NDK_OK)
					{	
						return ret;
					}
				}
				else if(mode==2)
				{
					if(cls_show_msg("[Enter]ȷ�����,[����]ȡ�����")!=ENTER)
						accept = 0;
					if((ret=NDK_BTMasterBondConfirm(skey,accept))!=NDK_OK)
					{	
						return ret;
					}
				}
				else if(mode==3)
				{
					cls_show_msg("���ڴ��豸������key:%s�����������",skey);
				}
				continue;
				break;
			case 2:
				cls_show_msg1(2,"��Գɹ�");
				return status;
			case 3:
				cls_show_msg1(2,"���ʧ��");
				return status;
			default:
				cls_show_msg1(2,"��ȡ��δ֪״̬(%d)", status);
				return status;
				break;
		}
	}
	return NDK_OK;
}

//���������ڻ�ȡ��ģʽ����״̬��
//pszMac��ʾҪ��ȡ����·��MAC��
//pnStatus��ʾҪ��ȡ��״̬1Ϊ��ȡδ����״̬��0Ϊ��ȡ����״̬
//����ֵΪ�������ý��,��ʱ����ȡ��
int lib_btmaster_getconstatus(char *pszMac, int pnStatus)
{
	int ret = 0, c_status = 1;
	time_t oldtime = 0;
	oldtime=time(NULL);
	while(1)
	{
		if(cls_show_msg1(1,"��ȡ����״̬��(%d),ESC���˳�",c_status)==ESC)
			return NDK_ERR_QUIT;
		if((ret=NDK_BTMasterGetConStatus(pszMac,&c_status))!=NDK_OK)
		{
			cls_show_msg1(10,"%s,line %d:��ȡ����״̬ʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;
		}
		if(c_status==pnStatus)
			return ret;
		if(time(NULL)-oldtime>30)
		{
			cls_show_msg1(10,"%s,line %d:��ʱ��δ���ӳɹ�", __FILE__, __LINE__); 
			return NDK_ERR_TIMEOUT;
		}
	}
}
#else
int lib_btmaster_scan(char *pszMac, int dectype){return NDK_ERR;}
int connect_pair_process(char *mac,int accept){return NDK_ERR;}
int lib_btmaster_pairing(int accept){return NDK_ERR;}
int lib_btmaster_getconstatus(char *pszMac, int pnStatus){return NDK_ERR;}
#endif
#endif

#if ANDIROD_ENABLE && !defined IM81AZ//defined N900AZ||defined N910AZ||defined N700 //��Ҫ�Ӵ������ϻ�ȡPIN������豸��Ҫ�ú�����ʼ����������  //defined IM81||defined IM81AZ||defined N900||
int touchscrean_getnum(void)//��ȡ�ڴ������������pinֵ
{
	int ret=0, i=0, j=0;
	uchar num[80]={0}, fun[36]={0}, numserial[11]={0};
#if 1
	int x0 = 0, x1 = 135, x2 = 270, x3 = 405, x4 = 540, y0 = 462, y1 = 568, y2 = 674, y3 = 780, y4 = 886;
#elif defined N910AZ||defined N700
	int x0 = 0, x1 = 180, x2 = 240, x3 = 360,x4 = 480,x5 = 540, x6 = 720, y0 = 0, y1 = 320, y2 = 640, y3 = 960, y4 = 1280;
#else//������Ҫ��������������ڴ˴�����
	;
#endif
//N900�ֱ��ʱ��ֲ��䲻���ļ�ȥ��ȡ����ֱ��д����  N910����Ҫȥ��ȡ�ļ���ȡ�ֱ�������Ǹ߷ֱ��ʾ͸ĳɲ�һ��������
#if 1//defined N910AZ||defined N700 //ȫ�����ļ����ȡ
	int fp = -1, readlen = 0;
	char touch_hr[] = {"1280x720"};//�߷ֱ�����1280x720
	char touch_hr_850[] = {"1024x600"};//850�ķֱ����� 1024x600 
	char readbuf[20] = {0};
	if((fp=open("/sys/class/touchscreen/resolution", O_RDONLY))<0)
	{
		cls_show_msg1(2,"line %d: ���ļ�ʧ��(%d)", __LINE__, fp);
		return FAIL;
	}
	memset(readbuf, 0, sizeof(readbuf));
	read(fp,readbuf, sizeof(readbuf));
	//��ȡ�ֱ��ʻ�ȡ���߷ֱ�����ת����������ǵͷֱ��ʲ����ı�
	if(!memcmp(touch_hr,readbuf,4))//ʹ��ǰ�ĸ����Ƚϲ���  //910��700һ��
	{
#if defined N920
		x0 = 0; x1 = 180; x2 = 360; x3 = 540;x4 = 720;
		y0 =614; y1 = 755; y2 = 896; y3 = 1037; y4 = 1178;
#else
		x0 = 0; x1 = 180; x2 = 360; x3 = 540;x4 = 720;
		y0 = 665; y1 = 818; y2 = 972; y3 = 1126; y4 = 1280;
#endif
	}
	//����N850�ķֱ���
	else if(!memcmp(touch_hr_850,readbuf,4))  //850
	{
		x0 = 0; x1 = 150; x2 = 300; x3 = 450;x4 = 600;
		y0 = 495; y1 = 609; y2 = 723; y3 = 837; y4 = 952;
	}
	else   //900
	{
		x0 = 0; x1 = 135; x2 = 270; x3 = 405;x4 = 540;
		y0 = 462;y1 = 568; y2 = 674; y3 = 780; y4 = 886;
	}
	close(fp);
#endif	
	int numInt[40] = { 
			x0, y0, x1, y1, x1, y0, x2, y1, x2, y0, x3, y1,
	
			x0, y1, x1, y2, x1, y1, x2, y2, x2, y1, x3, y2,
	
			x0, y2, x1, y3, x1, y2, x2, y3, x2, y2, x3, y3,
	
			x1, y3, x2, y4};
	int funInt[18] = { 
			0x1B, 0, x3, y0, x4, y1,
	
			0x0A, 0, x3, y1, x4, y2,
	
			0x0D, 0, x3, y2, x4, y4};
			
	for (i = 0, j = 0; i < 40; i++, j++) 
	{
		num[j] = (uchar) (numInt[i] & 0xff);
		j++;
		num[j] = (uchar) ((numInt[i] >> 8) & 0xff);
	}
	for (i = 0, j = 0; i < 18; i++, j++) 
	{
		fun[j] = (uchar) (funInt[i] & 0xff);
		j++;
		fun[j] = (uchar) ((funInt[i] >> 8) & 0xff);
	}
	memset(numserial, 0, sizeof(numserial));
	ret=NDK_SecVppTpInit(num, fun, numserial);
	if(ret==NDK_OK)
	{
		cls_printf("��ʱ��������������ּ��̵�˳��Ϊ\n%c,%c,%c \n%c,%c,%c \n%c,%c,%c \n  %c\n", numserial[0], numserial[1], numserial[2], numserial[3], numserial[4], numserial[5], numserial[6], numserial[7], numserial[8], numserial[9]);
	}
	return ret;
}
#else
int touchscrean_getnum(void){return NDK_OK;}
#endif
#endif

//��֧�ֱ�׼c���ʽ�����������ƽ̨����ʹ������Ӧ�ò�ʵ�ֵ���ʵ���������
#if SPRINTF_UNSPORT
void t_printchar(char **str, int c)
{
	//putchar(c);
	
	if (str) {
		if(c == '\n') {
			**str = '\r';
			++(*str);
			**str = c;
			++(*str);
		}
		else { 
			**str = c;
			++(*str);
		}
		
	}
	else
		putchar(c);//(void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

int t_prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			t_printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		t_printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		t_printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

int t_printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return t_prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			t_printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + t_prints (out, s, width, pad);
}

int t_print( char **out, const char *format, va_list args )
{
	register int width, pad;
	register int pc = 0;
	char scr[2];
	for (; *format != 0; ++format) {		
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += t_prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += t_printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += t_printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}	
			if( *format == 'X' ) {
				pc += t_printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += t_printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += t_prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			t_printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	va_end( args );
	return pc;
}

int t_printf(const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return t_print( 0, format, args );
}

int t_sprintf(char *out, const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return t_print( &out, format, args );
}

int t_snprintf( char *buf, unsigned int count, const char *format, ... )
{
        va_list args;
        
        ( void ) count;
        
        va_start( args, format );
        return t_print( &buf, format, args );
}
#endif

#if !defined ME66 //ME66 �ռ�С������
#if NEW_WIFI_ENABLE
//��ȡAp������״̬����
int lib_getwifistatus(ST_WIFI_STATUS *pstStatus)
{
	/*private & local definition*/
	int ret;
	time_t oldtime;
	ST_WIFI_STATUS status;

	/*process body*/
	oldtime=time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>60)
		{	
			cls_show_msg1(10,"%s,line %d:δ��ȡ������״̬,��ʱ�˳�(%d)",__FILE__, __LINE__,status.status);//WIFI_WLAN_STATUS_INIT,WIFI_WLAN_STATUS_LINKINGΪ�м�״̬,�����ʱ��δ��ȡ���ȶ��Ľ���״̬Ҳ��Ϊһ���쳣
			return NDK_ERR;
		}
		memset(status.ssid, 0, sizeof(status.ssid));
		if((ret = NDK_WifiStatus(&status)) != NDK_OK)
		{
			cls_show_msg1(10,"%s,line %d:wifi��ȡ״̬ʧ��(%d)",__FILE__, __LINE__,ret);
			return NDK_ERR;
		}
		if(status.status == WIFI_WLAN_STATUS_LINKING)
			cls_show_msg1(1,"��������SSID:%s��",status.ssid);
		if(status.status == WIFI_WLAN_STATUS_SUCC || status.status == WIFI_WLAN_STATUS_FAIL || status.status == WIFI_WLAN_STATUS_EMPTY|| status.status == WIFI_WLAN_STATUS_AUTHENTICATION_REJECTED)
			break;
		else
			NDK_SysMsDelay(10);
	}
	memcpy(pstStatus->ssid,status.ssid,sizeof(status.ssid));
	pstStatus->status=status.status;
	return NDK_OK;
}
#endif

int GetWifiStopflag(void)
{
	/*private & local definition*/
	
	/*process body*/
	return g_wifistopflag;
}

int SetWifiStopflag(int stopflag)
{
	/*private & local definition*/
	
	/*process body*/
	g_wifistopflag = stopflag;
	return g_wifistopflag;
}

#if NEW_WIFI_ENABLE||defined SP610
int lib_tcp_checkstatus(uint unFd, int maxtime)
{
	int ret=0;
	int pnStatus;
	time_t oldtime;
	oldtime=time(NULL);
	while(1)
	{
		if( (ret=NDK_TcpCheck(unFd , &pnStatus, NULL)) !=NDK_OK )
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ȡTCP״̬ʧ��(%d)", __LINE__,ret);
			return NDK_ERR;
		}
		switch(pnStatus)
		{
		case SOCKET_STATUS_CONNECTERR:
			cls_show_msg1(1,"line %d:��ȡ�����ӳ���SOCKET_STATUS_CONNECTERR", __LINE__);
			//return SOCKET_STATUS_CONNECTERR;
			break;
		case SOCKET_STATUS_CONNECTING:
			cls_show_msg1(1,"�������ӷ�������...");
			break;	
		case SOCKET_STATUS_DISCONNECT:
			return SOCKET_STATUS_DISCONNECT;
			break;
		case SOCKET_STATUS_CONNECT:
			//cls_printf("���ӳɹ�");
			return SOCKET_STATUS_CONNECT;
			break;
		case SOCKET_STATUS_READY_TO_SEND:
			cls_show_msg1(1,"���ڷ���������...");
			break;
		case SOCKET_STATUS_SEND_OVER:
			return SOCKET_STATUS_SEND_OVER;
			break;
		default:
			//cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��⵽δ֪��״̬(%d)", __LINE__, pnStatus);
			return pnStatus;
			break;
		}
		if(time(NULL)-oldtime>maxtime)
		{	
			//cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ʱ��δ��⵽���ӳɹ�״̬(%d)", __LINE__, pnStatus);
			return NDK_ERR_TCP_TIMEOUT;
		}
	}
}
#endif
#endif

#if CAMERA_ENABLE||SCAN_ENABLE||SCANASYCN_ENABLE
//asycnΪ1ʱ,��ʾ�첽ɨ��;asycnΪ0ʱ,��ʾͬ��ɨ��
int lib_doscan(int asycn, int maxwaittime)
{
	int ret=0;
	uint plen=0;
	uchar pvalue[500]={0};
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
	uint poutlen=0;
	uchar poutvalue[500]={0};
#endif
#if defined ME66
	uint a = 0, a1 = 0;
#endif

	if(asycn!=0 && asycn!=1)
	{
		cls_show_msg1(10,"%s,line %d:д���������(%d)", __FILE__, __LINE__, asycn);
		return NDK_ERR;
	}
	
	plen=0;
	memset(pvalue,0,sizeof(pvalue));
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
	memset(poutvalue,0,sizeof(poutvalue));
#endif
	cls_show_msg1(2, "��׼���ô�ɨ���ͼ��,2���ʼɨ��.");
#if SCANASYCN_ENABLE
	if(asycn==1)
	{
		if((ret=NDK_ScanStart()) != NDK_OK )
		{
			NDK_ScanStop();
			cls_show_msg1(10,"%s,line %d:����ͷ��ʼɨ��ʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;
		}
#if defined ME66
		NDK_SysReadWatch(&a1);
#else
		lib_StartStopwatch();
#endif
		while(1)
		{	
#if defined ME66
			NDK_SysReadWatch(&a);
			if(((a-a1)/(1000*1.0))>maxwaittime)
#else			
			if(lib_ReadStopwatch()>maxwaittime)
#endif
			{
				NDK_ScanStop();
				cls_show_msg1(10,"%s,line %d:ɨ�賬ʱ", __FILE__, __LINE__);
				return NDK_ERR_TIMEOUT;
			}
			switch(ret=NDK_ScanState(pvalue, &plen))
			{
				case NDK_OK:
					NDK_ScanStop();
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
					if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen))!=NDK_OK)
					{
						cls_show_msg1(10,"%s,line %d:UTF8תGBKʧ��", __FILE__, __LINE__);
						return NDK_ERR;
					}
					if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", poutlen, poutvalue)!=ENTER)
#else
					if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
#endif
					{
						cls_show_msg1(10,"%s,line %d:ɨ��������Ԥ�ڲ���", __FILE__, __LINE__);
						return NDK_ERR;
					}
					return NDK_OK;
					break;
#if defined ME68||defined ME50NX||defined ME66
				case NDK_ERR:
#elif CPU5810X_ENABLE
				case NDK_ERR_READ:
#else
				case NDK_ERR_CAMERA_SCANNING:
#endif				
					continue;
					break;
				default:
					NDK_ScanStop();
					cls_show_msg1(10,"%s,line %d:ɨ��ʧ��:��ȡ���쳣״̬(%d)", __FILE__, __LINE__, ret);
					return ret;
					break;
			}
		}
	}
#endif	
	if(asycn==0)
	{
		if((ret=NDK_ScanDoScan(maxwaittime, pvalue, &plen))!=NDK_OK)
		{
			cls_show_msg1(10,"%s,line %d:����ʽɨ��ʧ��(%d)", __FILE__, __LINE__, ret);
			return ret;
		}
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
		if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen))!=NDK_OK)
		{
			cls_show_msg1(10,"%s,line %d:UTF8תGBKʧ��", __FILE__, __LINE__);
			return NDK_ERR;
		}
		if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", poutlen, poutvalue)!=ENTER)
#else
		if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
#endif
		{
			cls_show_msg1(10,"%s,line %d:ɨ��������Ԥ�ڲ���", __FILE__, __LINE__);
			return NDK_ERR;
		}
		return NDK_OK;
	}
}
#else
int lib_doscan(int asycn, int maxwaittime){return NDK_ERR_NOT_SUPPORT;}
#endif

#if  ANDIROD_ENABLE
//��װdlsym,ʹ���ڳ���ʱ�ܹ��г�����ʾ��Ϣ 20171027
#define  DLSYM(funcp, libhd, func) {  \
	char *dlError;\
	funcp = dlsym( libhd , #func); \
	dlError = (char *)dlerror(); \
	if(dlError || NULL == funcp){\
		__android_log_print(ANDROID_LOG_INFO, "libmain.a", "lib.c:dload_library,dlsym fail,%s,return %d !", dlError, (int)funcp);\
	}\
}
int  dload_library(void)
{
	void *handle;
	//�ĳ�ֱ�Ӵ򿪿��ļ�������ʹ�þ���·��,n9003G���ͺ�4G���;���·����ͬ 20171010 modify
	handle = dlopen ("libjnindk.so", RTLD_LAZY); 
	if (!handle) {
		__android_log_print(ANDROID_LOG_INFO, "libmain.a", "dlopen fail,return %d", (int)handle);
		return -2;
	}
//�ĳ�ʹ�÷�װ����,ʹ�����Ҳ�����Ӧ����ʱ�ܹ��г�����ʾ20171027	
	DLSYM(cls_show_msg, handle, cls_show_msg_android);
	DLSYM(cls_show_msg1, handle, cls_show_msg1_android);
	DLSYM(ShowMessageBox, handle, ShowMessageBox_android);
	DLSYM(cls_printf, handle, cls_printf_android);
	DLSYM(lib_kbgetinput, handle, lib_kbgetinput_android);
	DLSYM(lib_getkeycode, handle,lib_getkeycode_android);
	DLSYM(send_result, handle,send_result_android);
	DLSYM(NDK_ScrPrintf, handle,NDK_ScrPrintf_android);
	DLSYM(NDK_ScrRefresh, handle,NDK_ScrRefresh_android);
	DLSYM(NDK_ScrClrs, handle,NDK_ScrClrs_android);
	DLSYM(cls_show_msg_record, handle,cls_show_msg_record_android);
	DLSYM(cls_show_msg1_record, handle,cls_show_msg1_record_android);
	dlerror();
	dlclose(handle);
	return 0;
}
#endif

