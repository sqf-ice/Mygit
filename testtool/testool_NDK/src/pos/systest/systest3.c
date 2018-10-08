/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest3.c
* Author 			: chensl
* version			: 
* DATE			: 20130124
* directory 	: 
* description	: ��̫���ۺϲ���
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
#define 	TESTITEM	"ETH����,ѹ��"
#define		PACKETLIFE	(50)//(1000)	//����1.6�淶�����޸� 20131103 linwl
#define	DEFAULT_CNT_STR	"100"//"200"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(200)

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
static void create_eth_ability_packet(Packet *packet, char *buf)
{
	/*private & local definition*/
	
	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len= PACKMAXLEN;//1024;//����1.6�淶�����޸� 20131103 linwl
	packet->lifecycle = PACKETLIFE;//1000;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}

//��̫�����ܲ���:ͨѶ����1000��,�������,����1024
static void eth_ability(void)
{
	/*private & local definition*/
	/*SOCKET*/
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	int timeout = SO_TIMEO;
	float commtimes = 0.0;
	int slen = 0, rlen = 0;//, bak_life = 0;
	Packet sendpacket;
#if K21_ENABLE
	char str[32] = {0};
#endif

	/*process body*/
	create_eth_ability_packet(&sendpacket, buf);
	//bak_life = sendpacket.lifecycle;
		
	//����ǰ��
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:NetUpʧ��", __LINE__);
		return;
	}
		
	if(TransUp(&h_tcp)!=SUCC)
	{
		NetDown();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:TransUpʧ��", __LINE__);
		return;
	}
		
	while (1)
	{
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
				
		//����ͨѶ(��һ�η�һ��)
		lib_StartStopwatch();
		slen = sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout);
		memset(rbuf, 0, sizeof(rbuf));
		rlen = sock_recv(h_tcp, rbuf, slen, timeout);
		commtimes+=lib_ReadStopwatch();
	}
		
	//���Ժ���
	TransDown(h_tcp);
	NetDown();

#if K21_ENABLE
	memset(str,0,sizeof(str));
	ftos((sendpacket.len*2/1024)*PACKETLIFE/commtimes,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����%sKB/s", str);
#else
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����%fKB/s", (sendpacket.len*2/1024)*PACKETLIFE/commtimes);
#endif
	return;
}

static void eth_comm_press(void)
{
	/*private & local definition*/
	int key = '0';
	uint h_tcp = -1;
	void (*pf[3])(uint) = {send_recv_press, recv_only_press, send_only_press};
	
	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			if( ++key == '2' ) /*�˴�ֻ��˫���շ�����Ϊ˫��͵���ĺ�̨��һ��,����2��3��Ҫ�������*/
			{
				return ;
			}
			cls_show_msg1(10, "�Զ����Խ�ִ��˫���շ�ѹ��");
		}
		else
		{
			key = cls_show_msg("_ETH�շ�ѹ��_\n"
								"1.˫���շ�ѹ��\n"
								"2.�������ѹ��\n"
								"3.������ѹ��\n");
		}
		switch(key)
		{
		case '1':
		case '2':
		case '3':
			if(NetUp()!=SUCC)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
				return;
			}
			
			if(TransUp(&h_tcp)!=SUCC)
			{
				NetDown();
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:TransUpʧ��", __LINE__);
				return;
			}
			pf[key-'1'](h_tcp);
			TransDown(h_tcp);
			NetDown();
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

static void eth_cnnt_press(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0;
	uint h_tcp = 0;

	/*process body*/
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}
	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d������ѹ��(�ѳɹ�%d��),��ESC�˳�", i+1, succ_count)==ESC)//�ֶ��˳�ѹ������
			break;
		
		if(g_SequencePressFlag&&(GetCycleValue()==i))  //�Զ�ѹ�����Դﵽѹ���������˳�
			break;
		
		i++;
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransDownʧ��", __LINE__);
			continue;
		}
		succ_count++;
	}
	
	NetDown();
	
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����:%d,�ɹ�����:%d", i, succ_count);
	return;
}

static void cnnt_comm_press(void)
{
	/*private & local definition*/
	int i = 0, succ_count = 0, j = 0, loop = 0, relaykey = 0;
	int slen = 0, rlen = 0, timeout = SO_TIMEO;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	uint  h_tcp = 0;

	/*process body*/
	if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
	{
		relaykey = cls_show_msg1(5,"ѹ�����Ƿ�Ҫ�м��:[ENTER]�� [����]��")==ENTER?TRUE:FALSE;
	}
	else
	{
		relaykey = cls_show_msg("ѹ�����Ƿ�Ҫ�м��:[ENTER]�� [����]��")==ENTER?TRUE:FALSE;
	}
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}
	
BEGIN:	
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d�ν���/ͨѶ���ѹ��(�ѳɹ�%d��),���ν��շ�%d��", i+1, succ_count, (loop=(rand()%10+1)))==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		
		if(relaykey==TRUE) 
		{
			if(i%5==0)
			{	
				j = rand();
				if(j%3==1)
					cls_show_msg1(120,"�ȴ�120��");
				else if(j%3==2)
					cls_show_msg1(180,"�ȴ�180��");
				else //if(j%3==0)
					cls_show_msg1(240,"�ȴ�240��");
			}
		}
		
		//UP
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		
		for(j=0; j<loop; j++)
		{
			//��������
			if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
			{
				TransDown(h_tcp);
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
				goto BEGIN;//continue;
			}
			//��������
			memset(rbuf, 0, sizeof(rbuf));
			if ((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
			{
				TransDown(h_tcp);
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
				goto BEGIN;//continue;
			}
			//�Ƚ��շ�
			if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
			{
				TransDown(h_tcp);
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��ʧ��", __LINE__);
				goto BEGIN;//continue;
			}
		}
		
		//DOWN
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:TransDownʧ��", __LINE__);
			continue;
		}
		succ_count++;
	}
	
	NetDown();

	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ����:%d,�ɹ�����:%d", i, succ_count);
	return;
}

static void eth_updown_press(void)
{
	/*private & local definition*/
	int i = 0, succ = 0, ret = -1, disflag = 0;
	uint h_tcp = 0;

	if(cls_show_msg1(5,"����ѹ��ʱ�Ƿ����NDK_EthDisConnect?��[ENTER],��[����]")==ENTER)//Ĭ�ϲ�����
		disflag = 1;
	/*process body*/
	while (1)
	{
		if(cls_show_msg1(2, "���ڽ��е�%d����̫��оƬѹ��(�ѳɹ�%d��)", i+1, succ)==ESC)
			break;
		
		if(g_SequencePressFlag&&(GetCycleValue()==i))  //�Զ�ѹ�����Դﵽѹ���������˳�
			break;
		
		i++;
		
		if(NetUp()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, i);
			continue;
		}
		if(TransUp(&h_tcp)!=SUCC)//��POS�������ʵ�ʷ���һ������,�Բ�����̫�����ض����ӵ�Ӱ��
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			continue;
		}
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��TransDownʧ��", __LINE__, i);
			continue;
		}		
		if(NetDown()!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, i);
			continue;
		}

		//��֤������NDK_EthDisConnect������ж����̫������ʱ����Ӱ���������� 20140616 linwl added
		if(disflag==1)
		{
			if((ret=NDK_EthDisConnect())!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NDK_EthDisConnectʧ��(%d)", __LINE__, i, ret);
				NDK_EthDisConnect();
				continue;
			}
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"ѹ������:%d,�ɹ�����:%d", i, succ);
	return;
}

static void ping_press(void)
{
	/*private & local definition*/
	int ret = 0, succ = 0, i = 0, cnt = DEFAULT_CNT_VLE;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint len;

	/*process body*/
	if(g_SequencePressFlag)  
	{
		cnt = GetCycleValue();
	}
	else
	{
		cls_printf("Ĭ��ѹ�����Դ���:");
		if ((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
			cnt = DEFAULT_CNT_VLE;
		else
			cnt = atoi(CntStr);
	}
	
	if(NetUp()!=SUCC)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NetUpʧ��", __LINE__);
		return;
	}

	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "pingѹ��������\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", cnt, succ))
			break;
		cnt--;
		i++;

		switch(ret=NDK_NetPing(netsetting.SvrIP.Str,5))
		{
		case 0:
			succ++;
			break;
		default:
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "��%d�δ��󷵻�%d", i,ret);
			break;
		}
		
	}
	NetDown();

	cls_show_msg_record(FILENAME,FUNCNAME,"pingѹ������%d��,�ɹ�%d��", i, succ);
	return;
} 

static void eth_press(void)
{
	/*private & local definition*/
	int keyin='0';
	
	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			//��Ӧ����Ĳ˵���+1���˵����Ӵ˴�ҲҪ��Ӧ������
			if((++keyin == '6')||(cls_show_msg1(2, "��������%s����ѹ������,ESC�˳�",TESTITEM)==ESC))
				return ;
		}
		else
		{
			keyin = cls_show_msg("1.����2.����\n"
								"3.����+����\n"
								"4.����ѹ��\n"
								"5.pingѹ��");
		}
		
		switch(keyin)
		{
		case '1':
			eth_comm_press();
			break;
		case '2':
			eth_cnnt_press();
			break;
		case '3':
			cnnt_comm_press();
			break;
		case '4':
			eth_updown_press();
			break;			
		case '5':
			ping_press();
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

void systest3(void)
{
	//����Ϊ�ֲ�����
	int nKey = 0;

	if(auto_flag==1)//autotest_flag
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_ETH(TRUE)!=SUCC)
			return;
		
		g_CycleTime = 100;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��ѹ������
		eth_press();
		//ִ�����ܲ���
		eth_ability();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	//����Ϊ�˹����Ժ�����
	cls_show_msg1(1, "%s������...", TESTITEM);
	while (1)
	{
		nKey = cls_show_msg("1.ETHѹ������\n"
							"2.ETH���ܲ���\n"
							"3.ETH�쳣����\n"
							"0.ETH����");
		switch(nKey)
		{
		case '1':
			eth_press();
			break;
		case '2':
			eth_ability();
			break;
		case '3':
			tcp_abnormal();//20140422 linwl ������̫��ǿ�ƹҶϺ�Э��Ҷ��쳣��������
			cls_show_msg("������δ������̨�ķ�������ַ,Ӧ������ͨ!");
			break;
		case '0':
			switch(conf_conn_ETH(TRUE))
			{
			case SUCC:
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				break;
			case NDK_ERR_QUIT://�û�����
			default:
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

