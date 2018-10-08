/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest19.c
* Author 			: chenjs
* version			: 
* DATE			: 20130227
* directory 	: 
* description	:��ӡ/MDM�������
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM		"��ӡ/MDM����"
#define	MAXWAITTIME	(120)	//30->120 for ���

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
*			  		chenjs		   20130227	 		created
*****************************************************************/
static void print_modem(void)
{
	/*private & local definition*/
	int icount = 0, isucc = 0, ret = -1;
	int send_len = 0, rec_len = 0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	
   	/*process body*/		
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();

	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)//if (FAIL==clrprintbuf())
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ��ʹ������Чmodify 20131122 jiangym
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}
	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", icount+1, TESTITEM, isucc)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		icount++;
		
		//��ʼ��MODEM
		cls_printf("��ʼ��MODEM��(��%d��)...", icount);
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:��ʼ��MODEM(��%d��)ʧ��(%d)", __LINE__, icount, ret);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME , g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//����
		cls_printf("MODEM������(��%d��)...", icount);
		if(mdm_dial(MDMDialStr)!=SUCC)
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:MODEM����(��%d��)ʧ��", __LINE__, icount);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//����ͨѶ
		cls_printf("MODEM����ͨѶ��(��%d��)...", icount);
		//��������
		if((send_len = mdm_send(sendpacket.header, sendpacket.len))!=sendpacket.len)
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, send_len, sendpacket.len);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//��������
		if((rec_len = mdm_rev(rbuf, sendpacket.len, 20))!=sendpacket.len)
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��������ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rec_len, sendpacket.len);
			continue;
		}
		//�Ƚ�����
		if(MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:����У��ʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			mdm_hangup();
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//�Ҷ�
		cls_printf("MODEM�Ҷ���(��%d��)...", icount);
		if(mdm_hangup()!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:MODEM�Ҷ�(��%d��)ʧ��", __LINE__, icount);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}		
		
		isucc++;
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s������� ѭ����%d�� �ɹ���%d��", TESTITEM, icount, isucc);
	return;
}

void systest19(void)
{
	/*private & local definition*/
	int keyin = 0;	
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		//MDM����
		if(conf_conn_MDM()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,MDM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//��ӡ����
		conf_prn();

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		print_modem();
			
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		keyin = cls_show_msg("1.�������\n"
							"0.MDM����\n"
							"9.��ӡ����");
		switch(keyin)
		{
		case '1':
			print_modem();
			break;
		case '0':			
			switch(conf_conn_MDM())
			{
			case SUCC:
				cls_show_msg1(2, "����modem�ɹ�!");
				break;
			case FAIL:
				cls_show_msg("line %d:����modemʧ��!!!\n������˳�����...", __LINE__);	
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				break;
			}
			break;
		case '9':
			conf_prn();
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

