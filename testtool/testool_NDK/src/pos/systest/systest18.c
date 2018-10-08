/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ӡ/���߽������
* file name		: systest18.c
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/02/21
* directory 		: 
* description		: ��ӡ/���߽������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  2013/02/21 chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��ӡ/���߽���"
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
*			  		  chensj 		   	2013/02/21		created
*****************************************************************/
//��д����ע�⣺�������Ҫ������������Ƚ���!����,ҲҪ��һ����ǿ�ȡ�
static void print_wireless(void)
{
	/*private & local definition*/
	int i = 0, succ = 0, timeout = SO_TIMEO, slen = 0, rlen = 0;
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	
	/*process body*/	
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)//if (FAIL==clrprintbuf())
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ��ʹ֮ǰ��������Чmodify 20131122 jiangym
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}
	
	//����ͨѶ
	while(1)
	{
		//��������
		TransDown(h_tcp);
		NetDown();
		
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		
		sleep(10);//ÿ�ιҶ�֮����������Ҫ�ȴ�10��,������оƬ��ѹ��

		if(NetUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:NetUpʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//UP
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:����ʧ��slen=%d", __LINE__, slen);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
			continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:У��ʧ��", __LINE__);
			continue;
		}
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
		{
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
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		NetDown();
		print_bill();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��ӡ��״̬�쳣!", __LINE__);
			continue;                         
		}
		
		succ++;
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s������� �ܴ���:%d �ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest18(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		//��������
		if(conf_conn_WLM(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WLM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		//��ӡ����
		conf_prn();

		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		print_wireless();		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while (1)
	{	
		keyin = cls_show_msg("1.�������\n"
							"0.��������\n"
							"9.��ӡ����");
		switch(keyin)
		{
		case '1':
			print_wireless();
			break;
		case '0':
			switch(conf_conn_WLM(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
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
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

