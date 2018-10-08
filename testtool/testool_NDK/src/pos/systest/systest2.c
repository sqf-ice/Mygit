/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: MODEM( �첽ͬ��)/�����ۺϲ���
* file name		: 2.c
* Author 			: 
* version			: V1.0
* DATE			: 20060911
* directory 		: .\SRC\systest\
* description		: MODEM�ۺϲ���
* related document	: NL720����Ա�ο��ֲ�.doc
*				  
************************************************************************
* log			: 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/

#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	UCID					2
#define	TESTITEM				"MODEM����,ѹ��"

#define	MAX_PHONENUM_CNT		150
#define	MAX_PHONENUM_SIZE	16
#define	SIZEOFSEPERATOR		strlen("\r\n")
#define	phonenumfile			"phonenum.txt"
#define	LOG_NACTEST			"system2.result"


/*------------global variables definition-----------------------*/
	
/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		  panxl 		   20060911 		created
*					zhangnw		  20100601		modify
*****************************************************************/
/*
������ƻ�ʹ���޷����к��и�λ������1��ѹ������,�������Ǵ����(ʵ���п�ͨ��ESC�ж����޴μ���)
maxcnt:
=0ʱ,��ʾѹ����������maxcnt����(����ESC����);
=-1ʱ,��ʾѹ����������maxcnt����(����ESC����),������и�λ����;
>0ʱ,maxcnt��ʾ���ѹ������;
<-1ʱ,abs(maxcnt)��ʾ���ѹ������,������и�λ����;
��������:���е���ѹ������
*/
static int __mdm_dial_press(char *dialstr, const int maxcnt, int *isucc, float *call_time, float *hangdowntime)
{
	/*private & local definition*/
	int icount = 0, ret = -1;
	*isucc = 0;
	*call_time = *hangdowntime = 0.0;
//	float tmp_time = 0.0;
	float init_time = 0.0;
//	LinkType type = getlinktype();
//	int initsucc = 0;//��¼��ʼ������ added by linwl 20140606

   	/*process body*/
	while (1)
	{
		if(cls_show_msg1(5, "��%d�β�%s��(�ѳɹ�%d��),��ESC�˳�", icount+1, dialstr, *isucc)==ESC)
			break;	
		if(maxcnt==0 || maxcnt==-1)
			;
		else if(icount==abs(maxcnt))
			break;
		icount++;

		if(maxcnt<0)
		{
			//��λMODEM
			cls_printf("��λMODEM��(��%d��)...", icount);
			mdm_reset();
		}

		if(maxcnt>=0&&icount==1)//��һ��ѹ����ʼʱ����һ�θ�λ����,�������Ա�֤��ʼ�������ʼ��,����һ�������ܹ���ȡ����ʼ����ʱ��
		{
			//��λMODEM
			//cls_printf("��λMODEM��(��%d��)...", icount);
			mdm_reset();
		}
		
		//��ʼ��MODEM
		cls_printf("��ʼ��MODEM��(��%d��)...", icount);
		//if(icount==1)//ֻ�е�һ�γ�ʼ��ʱ��ϳ�����ʼ��һ��֮���ٳ�ʼ���ײ�ʵ������ֱ�ӷ��ص�,���Ժ�����ʼ��ʱ��ԼΪ0
		lib_StartStopwatch();
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��ʼ��MODEM(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}
		init_time+=lib_ReadStopwatch();//��λ����� ��ʼ��ʱ���ۼ�ͳ��			
//		if(icount==1)
//			tmp_time=lib_ReadStopwatch();//����λ�����ֻͳ�Ƶ�һ��
//		initsucc++;//��¼��ʼ������ added by linwl 20140606
		
		//����
		cls_printf("MODEM��%s��(��%d��)...", dialstr, icount);
		lib_StartStopwatch();
		if(mdm_dial(dialstr)!=SUCC)
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:MODEM��%s(��%d��)ʧ��", __LINE__, dialstr, icount);
			continue;
		}
		*call_time += lib_ReadStopwatch();
		++*isucc;
		
		//�Ҷ�
		cls_printf("MODEM�Ҷ���(��%d��)...", icount);
		lib_StartStopwatch();
		if(mdm_hangup()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:MODEM�Ҷ�(��%d��)ʧ��", __LINE__, icount);
			continue;
		}
		*hangdowntime += (lib_ReadStopwatch()-(getlinktype()==SYNC?SLEEP_SDLC_HANGUP:0));
	}

//	if(maxcnt>=0)
//		cls_show_msg("hangdowntime=%f,tmp_time=%f, isucc = %d",*hangdowntime,tmp_time*(*isucc), *isucc);
//	if(maxcnt<0)
//		cls_show_msg("hangdowntime=%f,init_time=%f, isucc = %d",*hangdowntime,(init_time/initsucc)*(*isucc), *isucc);
	
//	if(type==SYNC)//ͬ������ʼ���Ĳ����ƶ����Ҷ��У����ԹҶ�ʱ����Ҫ����ͳ����ʵֵ
//	{//modified by linwl 20140606
//		if((maxcnt>=0)&&(*hangdowntime>tmp_time*(*isucc)))
//			*hangdowntime=*hangdowntime-tmp_time*(*isucc)/*icount*/;//����һ�θ�λ
//		if((maxcnt<0)&&(*hangdowntime>((init_time/initsucc)*(*isucc))))
//			*hangdowntime=*hangdowntime-(init_time/initsucc)*(*isucc);//�Ҷ϶�Ӧ�ĳ�ʼ��ʱ��=�ɹ�ʼ��ʱ��/�ɹ���ʼ������*�Ҷϳɹ��Ĵ���
//	}
	
	return icount;
}

#if 0
static int data_comm_press(void)
{
	/*private & local definition*/
	int i = 0, succ_cout = 0;
	int send_len = 0, rec_len = 0, len = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	float sendtime = 0.0,revtime = 0.0;

	/*process body*/
  	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();
	
	while (1)
	{
		if(cls_show_msg1(2, "���е�%d��ͨѶ��(�ѳɹ�%d��),��ESC�˳�", i+1, succ_cout)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
			
		//��������
		lib_StartStopwatch();
		if((len=mdm_send(sendpacket.header, sendpacket.len))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, len, sendpacket.len);
			mdm_clrportbuf_all();//�����ָ�һ��,�Բ�Ӱ����һ�β���
			continue;
		}
		sendtime+=lib_ReadStopwatch();
		send_len+=len;
		//��������
		lib_StartStopwatch();
		if((len=mdm_rev(rbuf, sendpacket.len, 20))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, len, sendpacket.len);
			mdm_clrportbuf_all();
			continue;
		}
		revtime+=lib_ReadStopwatch();
		rec_len+=len;
		//�Ƚ�����
		if(MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����У��ʧ��", __LINE__);
			continue;
		}
		
		succ_cout++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"Total:%d,succ:%d", i, succ_cout);
	if(sendtime!=0&&revtime!=0)//linwl ����������sendtime��revtimeΪ0ʱ�����ܷ��˳���20120607
		cls_show_msg_record(FILENAME,FUNCNAME,"����:%fB/s,����:%fB/s\n", send_len/sendtime, rec_len/revtime);
	return (i==succ_cout)?SUCC:FAIL;
}
#endif

static void comm_press(char *dialstr)
{
	/*private & local definition*/
	int ret = -1;
	EM_MDMSTATUS MdmStatus = 0;	//mdm״̬
	LinkType type = getlinktype();
	int i = 0, succ_cout = 0, last=-1;
	int send_len = 0, rec_len = 0, len = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	float sendtime = 0.0,revtime = 0.0;
	
   	/*process body*/
	strcpy(MDMDialStr, dialstr);
  	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();
	
PRE:
	if(LinkUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:������·ʧ��", __LINE__);
		return;
	}
	while (1)
	{
		if(cls_show_msg1(2, "���е�%d��ͨѶ��(�ѳɹ�%d��),��ESC�˳�", i+1, succ_cout)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
			
		//��������
		lib_StartStopwatch();
		if((len=mdm_send(sendpacket.header, sendpacket.len))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, len, sendpacket.len);
			mdm_clrportbuf_all();//�����ָ�һ��,�Բ�Ӱ����һ�β���
			if(IsContinuous(&i, &last))
			{
				LinkDown();
				goto PRE;
			}
			else
				continue;
		}
		sendtime+=lib_ReadStopwatch();
		send_len+=len;
		//��������
		lib_StartStopwatch();
		if((len=mdm_rev(rbuf, sendpacket.len, 20))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, len, sendpacket.len);
			mdm_clrportbuf_all();
			if(IsContinuous(&i, &last))
			{
				LinkDown();
				goto PRE;
			}
			else
				continue;
		}
		revtime+=lib_ReadStopwatch();
		rec_len+=len;
		//�Ƚ�����
		if(MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����У��ʧ��", __LINE__);
			mdm_clrportbuf_all();//�����ָ�һ��,�Բ�Ӱ����һ�β���
			if(IsContinuous(&i, &last))
			{
				LinkDown();
				goto PRE;
			}
			else
				continue;
		}
		
		succ_cout++;
	}
	
	if(type==SYNC)
	{
		//�Ҷ�״̬����
		
		if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_CONNECT_AFTERPREDIAL))
		{
			LinkDown();
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:SDLC_getmodemstatus��������%d", __LINE__,MdmStatus);
			return;
		}
	}

	if(LinkDown()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�Ͽ���·ʧ��", __LINE__);
		return;
	}
    if(type==SYNC)
	{
		
		if(((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)||(MdmStatus!=MDMSTATUS_NOPREDIAL))
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:SDLC_getmodemstatus��������", __LINE__);
			return;
		}
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"Total:%d,succ:%d", i, succ_cout);
	if(sendtime!=0&&revtime!=0)//linwl ����������sendtime��revtimeΪ0ʱ�����ܷ��˳���20120607
		cls_show_msg_record(FILENAME,FUNCNAME,"����:%fB/s,����:%fB/s", send_len/sendtime, rec_len/revtime);
	return;
}

static void call_comm_press(char *dialstr)
{
	/*private & local definition*/
	int icount = 0, isucc = 0;
	float call_time = 0.0, hangdowntime = 0.0, dial = 0.0, hang = 0.0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	
   	/*process body*/		
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();
	
	while (1)
	{
		if(cls_show_msg1(5, "��%d�β�%sͨѶ��(�ѳɹ�%d��),��ESC�˳�", icount+1, dialstr, isucc)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		icount++;

		if(mdm_dial_comm(dialstr, &sendpacket, FALSE, &dial, &hang)==SUCC)
		{
			isucc++;
			call_time+=dial;
			hangdowntime+=hang;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"ѭ����%d��,�ɹ���%d��", icount, isucc);
	if(isucc!=0)
		cls_show_msg_record(FILENAME,FUNCNAME,"��%sƽ��ʱ��%3.5f��/��,�Ҷ�ƽ��ʱ��(����ʼ��)%3.5f��/��", dialstr, call_time/isucc, hangdowntime/isucc);
	return;
}

static void call_press(char *dialstr)
{
	/*private & local definition*/
	int isucc = 0;
	float call_time = 0.0, hangdowntime = 0.0;
	
   	/*process body*/
	if(g_SequencePressFlag) 
		cls_show_msg_record(FILENAME,FUNCNAME,"ѭ����%d��,�ɹ���%d��", __mdm_dial_press(dialstr, GetCycleValue(), &isucc, &call_time, &hangdowntime), isucc);
	else
		cls_show_msg_record(FILENAME,FUNCNAME,"ѭ����%d��,�ɹ���%d��", __mdm_dial_press(dialstr, 0, &isucc, &call_time, &hangdowntime), isucc);
	if(isucc!=0)
		cls_show_msg_record(FILENAME,FUNCNAME,"��%sƽ��ʱ��%3.5f��/��,�Ҷ�ƽ��ʱ��%3.5f��/��", dialstr, call_time/isucc, hangdowntime/isucc);
	return;
}
static void send_result_NACtest(void)
{
	/*private & local definition*/
	int fp = 0, ret = 0, rdlen = 0;
	char result[1024] = {0};
	
	/*process body*/
	NDK_PortClose(PORT_NUM_COM1);
	cls_show_msg("�뽫POS�Ĵ���1����PC,����PC���չ���.�����������");
	if(	NDK_PortOpen(PORT_NUM_COM1, "115200") != NDK_OK )
	{
		cls_show_msg("line %d:�򿪴���2ʧ��", __LINE__);
		return;
	}
	if ((fp=NDK_FsOpen(LOG_NACTEST, "r"))< NDK_OK)
	{
		cls_show_msg("line %d:��%sʧ��(%d)", __LINE__, LOG_NACTEST);
		return;
	}
	while((rdlen=NDK_FsRead(fp, result,sizeof(result)))==sizeof(result))
	{
		if((ret=NDK_PortWrite(PORT_NUM_COM1, rdlen, result))!=NDK_OK)
		{
			cls_show_msg("line %d: ����ʧ��ret=%d", __LINE__, ret);
			NDK_FsClose(fp);
			return;
		}
	}
	if(rdlen>0)
	{
		if((ret=NDK_PortWrite(PORT_NUM_COM1,rdlen, result))!=NDK_OK)
			cls_show_msg("line %d: ����ʧ��ret=%d", __LINE__, ret);	
	}	
	NDK_FsClose(fp);
	NDK_PortClose(PORT_NUM_COM1);//�رմ���
	if (g_com!=INVALID_COM)//��Ĭ�ϵĴ���
	{
		if ((ret=NDK_PortOpen(g_com, "115200,8,N,1")) != NDK_OK)
		{
			//��ʼ��ʧ����,Ҳ����send_result("line %d: ��ʼ�����ڴ�", __LINE__);
			cls_show_msg("line %d: ��ʼ�����ڴ�", __LINE__);
			return;
		}
	}
	return;
}
static int  get_allphonenum(char *allphonenum, const int maxbuflen)
{
	/*private & local definition*/
	int fp = 0, readlen = 0;
	uint phonenumfilelen = 0;
	char *pos = NULL, *start = allphonenum;
	
	/*process body*/
	//step1:��ȡȫ���绰�����ļ�������
	if(NDK_FsExist(phonenumfile)!=NDK_OK)
	{
		cls_show_msg("line %d:�����ļ�������!", __LINE__);
		return FAIL;
	}
	if ((fp=NDK_FsOpen(phonenumfile, "r"))<0)
	{
		cls_show_msg("line %d:��%sʧ��!", __LINE__, phonenumfile);
		return FAIL;
	}
	if (NDK_FsFileSize(phonenumfile, &phonenumfilelen) != NDK_OK)
	{
		cls_show_msg("line %d:��ȡ%s�ļ�����ʧ��!", __LINE__, phonenumfile);
		return FAIL;
	}
	if(phonenumfilelen>maxbuflen-1)//��������������С���ļ�����
	{
		cls_show_msg("line %d:�޷����������ļ�����!", __LINE__);
		NDK_FsClose(fp);
		return FAIL;
	}
	memset(allphonenum, 0, maxbuflen);
	if ((readlen=NDK_FsRead(fp, allphonenum, maxbuflen-1)) < NDK_OK)//�ٶ�һ����ȷ��allphonenum�Ǵ�
	{
		cls_show_msg("line %d:���ļ�����(%d)", __LINE__, readlen);
		NDK_FsClose(fp);
		return FAIL;
	}
	NDK_FsClose(fp);

	//step2:��ʽ�����뻺��(\r\n->\x00\x00)
	while(strlen(start))
	{
		if((pos=strstr(start, "\x0D\x0A"))!=NULL)
		{
			memset(pos, 0x00, SIZEOFSEPERATOR);
			if(strlen(start)>=MAX_PHONENUM_SIZE)
			{
				cls_show_msg("line %d:��⵽��������(%s)!", __LINE__, start);
				return FAIL;	
			}
			start = pos+SIZEOFSEPERATOR;
		}
		else
			break;
	}
	
	return SUCC;
}

static int  get_phonenum(char *allphonenum, const int maxbuflen, int id, char *phonenum)
{
	/*private & local definition*/
	char *curr = allphonenum;
	
	/*process body*/
	//step1:�ҵ�ָ��id�ĺ��봮������phonenum
	while(id>0)
	{
		if(strlen(curr))//�ҵ�һ��"��Ч"����
		{
			if(--id==0)
			{
				strcpy(phonenum, curr);
				
#if 0	//�ɵ��������ж�,�Ƿ������޷Ǿ��ǰβ���
				//step2:phonenum�Ϸ����ж�
				if(is_digit_str(phonenum))
					return SUCC;
				else
				{
					//cls_show_msg("line %d:���ڷ����ֺ���!", __LINE__);
					return FAIL;
				}
#else
				return SUCC;
#endif
			}
		}
		curr+=strlen(curr)+1;
		if(curr+1>=allphonenum+maxbuflen)//Խ��
			return FAIL;//NOT FOUND
	}
	
	return FAIL;//NOT FOUND(�����id<=0)
}

static int nac_mdm_dial_press(char *dialstr, const int maxcnt, int *isucc, float *call_time, float *hangdowntime)
{
	/*private & local definition*/
	int icount = 0, ret = -1;
	*isucc = 0;
	*call_time = *hangdowntime = 0.0;
//	float tmp_time = 0.0;
   	/*process body*/
	while (1)
	{
		if(cls_show_msg1(5, "��%d�β�%s��(�ѳɹ�%d��),��ESC�˳�", icount+1, dialstr, *isucc)==ESC)
			break;	
		if(maxcnt==0 || maxcnt==-1)
			;
		else if(icount==abs(maxcnt))
			break;
		icount++;

		if(maxcnt<0)
		{
			//��λMODEM
			cls_printf("��λMODEM��(��%d��)...", icount);
			mdm_reset();
		}
		//��ʼ��MODEM
		cls_printf("��ʼ��MODEM��(��%d��)...", icount);
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��ʼ��MODEM(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}
		//����
		cls_printf("MODEM��%s��(��%d��)...", dialstr, icount);
		lib_StartStopwatch();
		if(mdm_dial(dialstr)!=SUCC)
		{
			mdm_hangup();
			cls_show_msg1(3, "line %d:MODEM��%s(��%d��)ʧ��", __LINE__, dialstr, icount);
			continue;
		}

		*call_time += lib_ReadStopwatch();
		++*isucc;
		//�Ҷ�
		cls_printf("MODEM�Ҷ���(��%d��)...", icount);
		lib_StartStopwatch();
		if(mdm_hangup()!=SUCC)
		{
			cls_show_msg1(3, "line %d:MODEM�Ҷ�(��%d��)ʧ��", __LINE__, icount);
			continue;
		}
		*hangdowntime += (lib_ReadStopwatch()-(getlinktype()==SYNC?SLEEP_SDLC_HANGUP:0));
	}
	
	return icount;
}


static void NAC_test(void)
{
	/*private & local definition*/
	char allphonenum[MAX_PHONENUM_CNT*MAX_PHONENUM_SIZE] = {0};//���ڴ��ȫ���绰����
	char phonenum[MAX_PHONENUM_SIZE] = {0};
	int icount = 0, isucc = 0;//���Ŵ����ͳɹ�����
	float call_time = 0.0, hangdowntime = 0.0;//����ʱ��͹Ҷ�ʱ��
	char resultbuf[128] = {0};//���ڴ�Ų��Խ��
	int fp = 0, index = 0, nKeyin = 0;
	
	/*process body*/
	//�����Ѵ��ڽ���ļ�,Ҫ��Ҫɾ��������������һ��
	if(NDK_FsExist(LOG_NACTEST)==NDK_OK)
	{
		nKeyin = cls_show_msg("��⵽��LOG,��ѡ��:\n"
						"1.���� 2.ɾ��\n");
		switch(nKeyin)
		{
		case '1':
			send_result_NACtest();
			return;
			break;
		case '2':
			NDK_FsDel(LOG_NACTEST);
			return;
			break;
		case ESC:
			return;
			break;
		default:
			break;
		}
	}

	//��ȡ�绰�����ļ�����ʽ������
	if(get_allphonenum(allphonenum, sizeof(allphonenum))!=SUCC)
		return;

	if((fp=NDK_FsOpen(LOG_NACTEST, "w"))<0)
	{
		cls_show_msg("line %d:����%sʧ��!", LOG_NACTEST);
		return;
	}
	//ÿ��ȡһ��������аκ�/�Ҷϵ�ѹ������,����ÿ�εĲ��Խ����¼���ļ�
	while(get_phonenum(allphonenum, sizeof(allphonenum), ++index, phonenum)!=FAIL)
	{
		icount = nac_mdm_dial_press(phonenum, 5/*10*/, &isucc, &call_time, &hangdowntime);//Ϊ�˱��⻰�����ģ���С��5�� 20140901 linwl
		if(isucc!=0)
			sprintf(resultbuf, "%s��%d�� �ɹ�%d��.����:%2.3fs/��,�Ҷ�:%2.3fs/��\r\n", phonenum, icount, isucc, call_time/isucc, hangdowntime/isucc);
		else
			sprintf(resultbuf, "%s��%d�� �ɹ�%d��.\r\n", phonenum, icount, isucc);

		if(cls_show_msg1(5, resultbuf)==ESC)
			break;
		if(NDK_FsWrite(fp, resultbuf, strlen(resultbuf))!=strlen(resultbuf))
		{
			cls_show_msg("line %d:д%sʧ��!", __LINE__, LOG_NACTEST);
			break;
		}
	}
	NDK_FsClose(fp);

	//���Ͳ��Խ��
	if(cls_show_msg("�������,�Ƿ����ͽ��?\n[ȷ��]��[����]��")==ENTER)
		send_result_NACtest();

	return;
}

static void reset_call_press(char *dialstr)
{
	/*private & local definition*/
	int isucc = 0;
	float call_time = 0.0, hangdowntime = 0.0;

   	/*process body*/
	if(g_SequencePressFlag) 
		cls_show_msg_record(FILENAME,FUNCNAME,"ѭ����%d��,�ɹ���%d��", __mdm_dial_press(dialstr, -(GetCycleValue()), &isucc, &call_time, &hangdowntime), isucc);
	else
		cls_show_msg_record(FILENAME,FUNCNAME,"ѭ����%d��,�ɹ���%d��", __mdm_dial_press(dialstr, -1, &isucc, &call_time, &hangdowntime), isucc);
	if(isucc!=0)
		cls_show_msg_record(FILENAME,FUNCNAME,"��%sƽ��ʱ��%3.5f��/��,�Ҷ�ƽ��ʱ��%3.5f��/��\n", dialstr, call_time/isucc, hangdowntime/isucc);
	return;
}

static void modem_press(void)
{
	/*private & local definition*/
	int keyin = '0';
	void (*pfunc[])(char *) = {comm_press, call_press, call_comm_press, reset_call_press};
	
	/*process body*/
	cls_show_msg1(g_keeptime, "��ȷ�����е绰����POS�绰��(����ժ��),���������...");
				
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			if( ++keyin == '5') //��Ӧ����Ĳ˵���+1���˵����Ӵ˴�ҲҪ��Ӧ������
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����ѹ�����Խ���\n",TESTITEM);
				return ;
			}
			if(cls_show_msg1(2, "������������ѹ������,ESC�˳�")==ESC)
				return;
		}
		else
		{
			keyin = cls_show_msg("1.ͨѶѹ��\n" 
							"2.����ѹ��\n"
							"3.ͨѶ����ѹ��\n"
							"4.��λ����ѹ��\n");
		}
		
		switch (keyin)
		{
		case '1':
		case '2':
		case '3':
		case '4':
			pfunc[keyin-'1'](MDMDialStr);
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

static void modem_ability(void)
{
	/*private & local definition*/
	int keyin = 0;
	void (*pfunc[])(char *) = {call_press, reset_call_press, call_press};
	
	/*process body*/
	while(1)
       {
              keyin = cls_show_msg("1.ͬ��MODEM����\n"
							"2.ͬ��MODEM�߼�\n"
							"3.�첽MODEM\n"
							"4.NAC\n");
		switch(keyin) 
		{
		case '1':
		case '2':
			if(getlinktype()!=SYNC)
			{
				cls_show_msg1(5, "MODEM�������ô���!\n���Ƚ�������...");
				return;
			}
			pfunc[keyin-'1'](MDMDialStr);
			break;
		case '3':
			if(getlinktype()!=ASYN)
			{
				cls_show_msg1(5, "MODEM�������ô���!\n���Ƚ�������...");
				return;
			}
			pfunc[keyin-'1'](MDMDialStr);
			break;
		case '4':
			NAC_test();
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

//modem�쳣����: �����Ƚ������ηǷ����벦��,�ٽ���һ����ȷ���벦�ſ����һ���Ƿ��ܲ�ͨ
static void modem_abnormal(void)
{
	/*private & local definition*/
	int icount = 0, ret = -1;
	char dialstr1[10]={"5840"},dialstr2[10]={"7540"}; /*modem�������ŷ�Χ#1~#8 */
	char dialstr[10]={0};
	
   	/*process body*/
	cls_printf("modem�쳣������");
	while (1)
	{
		if(++icount==4)
			break;

		//��ʼ��MODEM
		cls_printf("��ʼ��MODEM��(��%d��)...", icount);
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��ʼ��MODEM(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			return;
		}
		//����
		memset(dialstr,0,sizeof(dialstr));
		if(icount==1)
			strcpy(dialstr,dialstr1);
		else if(icount==2)
			strcpy(dialstr,dialstr2);
		else
			strcpy(dialstr,linksetting.WMDialStr);
		
		cls_printf("MODEM��%s��(��%d��)...", dialstr, icount);
		if(mdm_dial(dialstr)!=SUCC)
		{
			if(icount==3)
			{
				mdm_hangup();
				cls_show_msg1(g_keeptime, "line %d:MODEM�쳣����ʧ��", __LINE__);
				return;
			}
			mdm_hangup();
			continue;
		}

		//�Ҷ�
		cls_printf("MODEM�Ҷ���(��%d��)...", icount);
		mdm_hangup();
		break;	
	}
	cls_show_msg("MODEM�쳣����ͨ��,���������");
}

//��֤����MDMͬ��/�첽��ʼ���л���Ӧ���ܹ���������
static void modem_abnormal2(void)
{
	/*private & local definition*/
	char dialstr[10]={0};
	LinkType type = getlinktype();
	int i = 0, ret = -1;
	
   	/*process body*/
	cls_printf("modem�쳣������");
	if(type==ASYN)
		NDK_MdmSdlcInit(MDMPatchType);//�����첽����ǰ����ͬ����ʼ��
	else if(type==SYNC)
	{
		NDK_MdmSdlcInit(MDMPatchType);
		NDK_MdmAsynInit(MDMPatchType);//����ͬ������ǰ�����첽��ʼ��
	}
	for(i=0; i<2; i++)
	{
		cls_printf("��ʼ��MODEM��...");
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg("line %d:��ʼ��MODEMʧ��(%d)�쳣����ʧ��", __LINE__, ret);
			return;
		}
		strcpy(dialstr,linksetting.WMDialStr);
		if(mdm_dial(dialstr)!=SUCC)
		{
			cls_show_msg("line %d:MODEM��%sʧ���쳣����ʧ��", __LINE__, dialstr);
			mdm_hangup();
			return;
		}
		//�Ҷ�
		cls_printf("MODEM�Ҷ���...");
		if(mdm_hangup()!=SUCC)
		{
			cls_show_msg("line %d:MODEM�Ҷ�ʧ���쳣����ʧ��", __LINE__);
			return;
		}
	}
	cls_show_msg("MODEM�쳣����ͨ��,���������");
}

void  MDM_abnormal(void)
{
	int keyin = 0;
	
	while(1)
	{
         keyin = cls_show_msg("1.�Ƿ����벦��\n"
							"2.ͬ���첽�л�");
		switch(keyin)
		{
		case '1':
			modem_abnormal();
			break;
		case '2':
			modem_abnormal2();
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
void systest2(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_MDM()!=SUCC)
			return;
		g_CycleTime = 100;
		
		//�Զ�����������
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		
		//ִ��ѹ������
		modem_press();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while(1)
	{
		keyin = cls_show_msg("_MODEM�ۺϲ���_\n"
							"1.ѹ��  2.����\n"
							"3.MOEM�쳣����\n"
							"0.MODEM����");
		switch(keyin)
		{
		case '1':
			modem_press();
			break;
		case '2':
			modem_ability();
			break;
		case '3':
			MDM_abnormal();
			break;
		case '0':
			switch(conf_conn_MDM())
			{
			case SUCC:
				break;
			case FAIL:
			default:
				//return;
				break;
			}
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

