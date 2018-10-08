/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest20.c
* Author 			: chensl
* version			: 
* DATE			: 20130124
* directory 		: 
* description		: ��ӡ/LAN����
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
#define	TESTITEM		"��ӡ/LAN����"
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
*			  		chensl		   20130124	 		created
*****************************************************************/
static void print_LAN(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0;
	uint h_tcp = -1;
	int slen = 0, rlen = 0, timeout = SO_TIMEO;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*process body*/
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:NetUpʧ��", __LINE__);
		return;
	}

	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)//if (FAIL==clrprintbuf())
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ��ʹ������Чmodify 20131122 jiangym
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	print_bill();
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return; 						
	}
	while (1)
	{
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ_count)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
				
		//UP
		if(TransUp(&h_tcp)!=SUCC)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;						  
		}
		
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;						  
		}
		
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
			continue;
		}
		
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:У��ʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			TransDown(h_tcp);
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;						  
		}
		
		//DOWN
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:TransDownʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;						  
		}		
		succ_count++;	
	}
	
	NetDown();
	print_bill();
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return; 						
	}
	cls_show_msg_record(FILENAME ,FUNCNAME ,"ͨѶ����:%d �ɹ�����:%d", i, succ_count);
	return;
}

void systest20(void)
{
	/*private & local definition*/
	int keyin = 0, tmpflag = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	tmpflag = GetWifiStopflag();//��ȫ��wifi�Ͽ��Ŀ��ر��浽��ʱ
	if(tmpflag!=0)
		SetWifiStopflag(0);//ȷ����������ά��wifi���ӵ�����²���
		
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		//LAN����
		if(conf_conn_LAN()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,LAN����ʧ��,���������ļ�",TESTITEM);
			SetWifiStopflag(tmpflag);
			return;
		}
		//��ӡ����
		conf_prn();
		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		print_LAN();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		SetWifiStopflag(tmpflag);
		return;
	}
	while (1)
	{	
		keyin = cls_show_msg("1.�������\n"
							"0.LAN����\n"
							"9.��ӡ����");
		switch(keyin)
		{
		case '1':
			print_LAN();
			break;
		case '0':
			switch(conf_conn_LAN())
			{
			case SUCC:
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				//return;		
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				//return;
				break;
			}
			break;
		case '9':
			conf_prn();
			break;			
		case ESC:
			SetWifiStopflag(tmpflag);
			return;
			break;
		default:
			continue;
			break;
		}
	}	
	SetWifiStopflag(tmpflag);
	return;
}

