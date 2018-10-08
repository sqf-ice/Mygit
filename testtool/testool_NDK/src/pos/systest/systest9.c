/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest9.c
* Author 			: chensl
* version			: 
* DATE			: 20130131
* directory 	: 
* description	: �����ۺϲ���
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
#define		BPS_NUM		10
#define	MAXWAITTIME 	150	//30
#define 	PORT		PORT_NUM_COM1	//����N900AZ�������������õĴ���
#define	TESTITEM		"��������,ѹ��"
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
//���ڳ�ʼ��
static int aux_init(void)
{
	/*private & local definition*/
	int port = -1, ret = -1;

	/*process body*/
	port = select_serial();
	if(conf_link(SERIAL)!=SUCC)
		return FAIL;
	if((ret = NDK_PortOpen(port, g_bps)) != NDK_OK)
	{
		cls_show_msg("line %d:��ʼ��AUX(%d)ʧ��\n������:%x\nBPS:%s", __LINE__, port, ret, g_bps);
		return FAIL;
	}	
	serial_clrportbuf_all(port);
	cls_show_msg1(2, "��ʼ��AUX(%d)�ɹ�\n""%s 8-N-1\n", port, g_bps);
	return port;
}

//���ڲ�����ѭ����ʼ��
static void aux_circleinit(void)
{
	/*private & local definition*/
	int i = 0, ret = -1, port = 0;
	char *bps[BPS_NUM] = {"300", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "230400"};

	/*process body*/
	port = select_serial();
	while (1)
	{
		if(ESC==cls_show_msg1(1, "�������ֻ���(%s)...ESC�жϲ���", bps[i]))
			return;
		if((ret=NDK_PortOpen(port, bps[i])) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ʼ������(%d, %s)ʧ��(ret=%d)", __LINE__, port, bps[i], ret);
			return;
		}
		i++;
		if(i == BPS_NUM)
			i = 0;
	}
	
	return;
}

//POS<---PC
static void aux_pctopos(void)
{
	/*private & local definition*/
	int ret = -1, port = -1, datalen = 0;
	long int dataSum = 0;
	char recvBuf[BUFSIZE_SERIAL] = {0}; 
	uchar chksum = 0x00;

	/*process body*/
	if ((port=aux_init()) == FAIL)
		return;
	do
	{
		if(ESC==cls_show_msg1(1, "POS<---PC\n���յ����ݳ���:%d,У����:%2.2x,ESC���жϽ���...", dataSum, chksum))
			break;
		if((ret=NDK_PortRead(port, sizeof(recvBuf), recvBuf, 5*1000, &datalen)) == NDK_OK)
		{
			dataSum += datalen;
			chksum^=MakeLRC((uchar *)recvBuf, datalen);
		}
	}while(datalen >= 0);
	
	cls_show_msg_record(FILENAME,FUNCNAME,"POS<---PC\n���յ����ݳ���:%d,У����:%2.2x", dataSum, chksum);
	return;
}

//PC<->POS
static void aux_comm_pc(void)
{
	/*private & local definition*/
	int ret = -1, port = -1, i = 0, recvLen = 0, succconut = 0;
	char sendBuf[BUFSIZE_SERIAL] = {0}, recvBuf[BUFSIZE_SERIAL] = {0};
	Packet sendpacket;

	/*process body*/	
	if ((port=aux_init()) == FAIL)
		return;
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
	while(1)
	{
		cls_printf("��ʼ��%d��PC<->POSͨѶ(�ѳɹ�%d��)", i+1, succconut);

		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
		{			
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret)==ESC)
				break;
			else
				continue;
		}		
		NDK_ScrDispString(0,48,"POS--->PC",0);
		NDK_ScrRefresh();
		if((ret=NDK_PortWrite(port, sendpacket.len, sendpacket.header))!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		NDK_ScrDispString(0,48,"POS<---PC",0);
		NDK_ScrRefresh();
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(port, sendpacket.len, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�%d (ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, sendpacket.len)==ESC)
				break;
			else
				continue;
		}
		NDK_ScrDispString(0,32, "����У����...",0);
		NDK_ScrRefresh();
		//�Ƚ�
		if(recvLen != sendpacket.len)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i,recvLen,sendpacket.len)==ESC)
				break;
			else
				continue;
		}
		if(MemCmp(sendBuf, recvBuf, sendpacket.len))
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		succconut++;//ͨѶ�ɹ���������	
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶѹ���������,ִ�д���Ϊ%d,�ɹ�%d��", i, succconut);
	return;
}


//�����Է�
static void aux_comm_self(void)
{
	/*private & local definition*/
	int ret = -1, port = -1, i = 0, recvLen = 0, succconut = 0;
	char sendBuf[BUFSIZE_SERIAL] = {0}, recvBuf[BUFSIZE_SERIAL] = {0};
    Packet sendpacket;

	/*process body*/
	if ((port=aux_init()) == FAIL)
		return;
	cls_show_msg("�̽���ѡ���ڵ�23��,���������...");
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);	
	while(1)
	{
		if(cls_show_msg1(1, "���ڽ���AUX(%d)��%d��ͨѶ,��ESC�˳�", port, i+1)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
			continue;
		}
		
		//��
		if((ret=NDK_PortWrite(port, sendpacket.len, sendpacket.header))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		
		//��
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(port, sendpacket.len, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�%d (ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, sendpacket.len);
			continue;
		}

		//�Ƚ�
		if(recvLen != sendpacket.len)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i,recvLen,sendpacket.len);
			continue;
		}
		if(MemCmp(sendBuf, recvBuf, sendpacket.len))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i);
			continue;
		}
		succconut++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"�����Լ����(������%d��),�ɹ���%d��", i, succconut);
	return;
}

int sendrecv_cmd(char *sendbuf, int sendlen, char *recbuf, uint unlen, int *reclen, int timeout)
{
	int ret=0;
	NDK_PortClrBuf(PORT);
	if((ret=NDK_PortWrite(PORT, sendlen, sendbuf)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
		return ret;
	}
	if((ret = NDK_PortRead(PORT, unlen, recbuf, timeout*1000, reclen)) != NDK_OK)	
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
		return ret;
	}
	
	return NDK_OK;
}

void n900_btdesk(void)
{
	/*private & local definition*/
	int ret=0, len=0, len1=0, len2=0, i = 0, btflag = 0;
	char recbuf[128]={0}, old_btname[128]={0}, old_btaddr[128]={0};
	char sendbuf[256]={0};
	char modify_btname[]="BLUE NAME:TestName\r\n";
	char modify_btaddr[]="BLUE ADDR:010203040506\r\n";
	char cmd[][128]={  "AT#BV\r\n", 		   		/*0 ��ȡ����汾��Ϣ*/
				         "AT#BM\r\n", 	   			/*1	��ȡ��������*/
				         "AT#BMTestName\r\n",		/*2 �޸���������ΪTestName*/
				         "AT#BZ\r\n",				/*3 ��ȡ������ַ*/
				         "AT#BZ010203040506\r\n",	/*4 �޸�������ַΪ010203040506*/
				         "AT#BX\r\n",				/*5 �˳�����ģʽ*/
					 "AT#BR1\r\n",				/*6 �޸�����������Ϊ9600*/
					 "AT#BR5\r\n",				/*7 �޸�����������ΪĬ��ֵ115200*/
					 "AT#BR4\r\n",			/*8 �޸�����������Ϊ57600*/
					"AT#WZ\r\n",				/*9 ��ȡWiFi MAC*/		
					"AT#WM\r\n",				/*10 ��ѯWiFi AP����*/		
					"AT#WMAP@#BTtest\r\n",	/*11 �޸�WiFi AP����ΪAP@#BTtest*/
					"AT#WI\r\n", 				/*12��ѯWiFi AP IP��ַ*/
					"AT#WI192.168.1.11\r\n",     /*13�޸�WiFi AP IP��ַ192.168.1.11*/
					"AT#WS\r\n",				/*14��ѯWIFI AP ��������*/
					"AT#WS255.255.255.0\r\n",	/*15�޸�WIFI AP ��������255.255.255.0*/
					"AT#WK\r\n",				/*16��ѯWIFI AP ����*/
					"AT#WK87654321\r\n",		/*17�޸�WIFI AP ����Ϊ87654321*/
					//"AT#WP2",				/*�޸�WIFI AP���ܷ�ʽΪwpa2,���ϲ��*/
					"AT#SN\r\n",				/*18��ѯSN��*/
					"AT#SN0123456789012\r\n",	/*19�޸�SN��0123456789012*/
					"AT#PN\r\n",				/*20��ѯPN��*/
					"AT#PN9876543210123\r\n",	/*21�޸�PN��9876543210123*/
					"AT#BR7\r\n",			 	 /*22 �޸�����������Ϊ380400*/
					"AT#JH\r\n",				/*23��ѯ��������*/
					"AT#JH201707050102\r\n",	/*24�޸Ļ�������*/
					"AT#FWthis is U disk test ,write@#><,over\r\n", /*25дU�̲����ļ�*/
					"AT#FR\r\n",				/*26��U�̲����ļ�*/
					"AT#FS\r\n",				/*27�ϴ�U�̲����ļ�*/
					"AT#LOG\r\n",				/*28��ȡ���Լ�¼�ļ�*/
					"AT#LOG0123456789\r\n",	/*29д���Լ�¼�ļ�*/
					"AT#RESTART\r\n",			/*30��Ч���ò���*/
					"AT#EZ\r\n" ,                /*31��ѯ��̫��mac��ַ*/
					"AT#EZ123456789012\r\n",    /*32�޸���̫��mac12345678901234567*/
					 };
	char wifi_mac[32] = {0},old_wifiname[256] = {0},old_wifi_ipaddr[32] = {0},old_wifimask[32] = {0},old_wifipwd[256] = {0}, old_sn[128]  = {0},old_pn[128]={0}, old_jh[32] = {0}, old_ethaddr[32] = {0};
	char modify_wifiname[] = "WIFI AP NAME:AP@#BTtest\r\n", modify_wifi_ipaddr[] = "WIFI AP IP:192.168.1.11\r\n",modify_wifimask[] = "WIFI AP MASK:255.255.255.0\r\n";
	char modify_wifipwd[] = "WIFI AP PASSWORD:87654321\r\n",modify_sn[] = "SN:0123456789012\r\n",modify_pn[] = "PN:9876543210123\r\n",modify_jh[] = "JH:201707050102\r\n", modify_ethaddr[] = "ETH MAC:123456789012\r\n";
	char udisk_str[] = "file content:\r\nthis is U disk test ,write@#><,over\r\n", udisk_wr[] ="write testfile OK\r\n", old_udisk[256] = {0}, old_record[256] = {0}, new_record[256] = {0} ;
	int reclen = 0;
	char *err_cmd[] = {
					"AT#BMTestName01234567890123456789123\r\n",    //0name����30 command ERROR!!!\r\n
					"AT#BX123\r\n", 				//1�˳�����ģʽָ��ȴ���7command ERROR!!!\r\n
					"AT#BR0\r\n",					//2���������ò����䷶Χ�� error baud rate!\r\n
					"AT#BR8\r\n",					
					"AT#BZ00130485\r\n",  			//������ַ���ô���BLUE ADDR ERROR!!!\r\n					
					"AT#WZ192.192.192.1922\r\n",     //5 wifi ap ip��ַ����  command ERROR!!!\r\n
					"AT#WS255.255.255.0234r\n",      //6 wifi ap �����������ô��� command ERROR!!!\r\n
					"AT#WK1234567\r\n",			//7 wifi ap ���볤��С��7  ����256�Ĳ���command ERROR!!!\r\n
					"AT#WP733\r\n",				//8 wifi ap ����ģʽ���ȴ���1  command ERROR!!!\r\n
					"AT#SN0123456789012345678901234567891201234567890123456789012345678912012345678901234567890123456789120123456789012345678901234567891234\r\n",//9sn���ȴ���128
					"AT#PN0123456789012345678901234567891201234567890123456789012345678912012345678901234567890123456789120123456789012345678901234567891234\r\n",//10pn���ȴ���128
					"AT#JH0123456789012\r\n",		//11 jh���ȴ���12�ֽ�,
					"AT#WP7\r\n",				//12wifi ap ����ģʽ���� error wifi secmode!\r\n
					"AT#BP8\r\n",					//13�޸�����ģʽ���� error blue pair mode!\r\n
					"AT#EZ012345\r\n",              //14�޸���̫��mac��ַ���� ETH ADDR ERROR!!!\r\n
					};
	char err_cmd_return[] = "command ERROR!!!\r\n", err_baud_rate[] = "error baud rate!\r\n", err_bt_addr[] = "BLUE ADDR ERROR!!!\r\n", err_eth_addr[] = "ETH MAC ERROR!!!\r\n";
	char err_secmode[] = "error wifi secmode!\r\n", err_bt_pairmode[] = "error blue pair mode!\r\n";
#define 	MAXTIME		5//10 �޸ĳ�5s��ʱ���ӿ��ٶ�

	//����ǰ��
	if(cls_show_msg("ע��:��������ر�POS����̨,�ѹرհ��������������,δ�رհ�ȡ�����˳�")==ESC)
		return;
	NDK_PortClose(PORT);

	if(cls_show_msg("�Ƿ����N910������������,��[ENTER],��[����]")==ENTER)
		btflag = 1;
	else
		btflag = 0;
	//case11:���빤��ģʽ
        if(btflag)
		cls_show_msg("�뽫POS����1��������������ͨ��������������,�������������ϵĸ�λ��2s����빤��ģʽ");
	else
		cls_show_msg("�뽫POS����1��������������ͨ��������������,�ϵ������������빤��ģʽ��(ʹ�ù���ģʽ���ϵ�),�����������");
	if((ret=NDK_PortOpen(PORT, "115200,8,N,1"))!=NDK_OK)//Ĭ�����
	{
		cls_show_msg1(g_keeptime, "line %d:�򿪴���ʧ��(%d)", __LINE__, ret);
		return;
	}
	//��������һ��ָ��(ȡ�汾),��֤�ѽ��빤��ģʽ(�������sn,��ѵ�һ��ָ������빤��ģʽ��ָ��)
	cls_printf("���빤��ģʽ��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[0]);
	memset(recbuf, 0, sizeof(recbuf));	
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, sizeof(recbuf), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}

	//case10:�˳�����ģʽ��,����ָ��Ӧ�û�ʧ��
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[5]);
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("exit Production Mode!"), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	//���ͻ�ȡ����汾��Ϣָ��Ӧ�û�ʧ��
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[0]);
	memset(recbuf, 0, sizeof(recbuf));
	NDK_PortClrBuf(PORT);
	if((ret=NDK_PortWrite(PORT, strlen(sendbuf), sendbuf)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,sendbuf=%s)", __LINE__, ret, sendbuf);
		return;
	}
	if((ret = NDK_PortRead(PORT, sizeof(recbuf), recbuf, MAXTIME*1000, &len)) == NDK_OK)	
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,recbuf=%s)", __LINE__, ret, recbuf);
		return;
	}
	cls_show_msg("�ɹ��˳�����ģʽ,�������ϵ������������빤��ģʽ��,�����������");

	//��������һ��ָ��(ȡ�汾),��֤�ѽ��빤��ģʽ(�������sn,��ѵ�һ��ָ������빤��ģʽ��ָ��)
	cls_printf("���빤��ģʽ��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[0]);
	memset(recbuf, 0, sizeof(recbuf));	
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, sizeof(recbuf), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	//case0:���Ӳ����쳣����
	cls_show_msg1(3,"�����쳣������...");
	for(i=0;i<2;i++)
	{
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, err_cmd[i]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_cmd_return), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,ret=%d,sendbuf=%s,recbuf=%s)", __LINE__, i, ret, sendbuf, recbuf);
			return;
		}
		if( len!=strlen(err_cmd_return) ||  strcmp(recbuf, err_cmd_return) )
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,len=%d,recbuf:%s)", __LINE__, i, len, recbuf);
			goto ERR;
		}
	}
	for(i=2;i<4;i++)
	{
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, err_cmd[i]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_baud_rate), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,ret=%d,sendbuf=%s,recbuf=%s)", __LINE__, i, ret, sendbuf, recbuf);
			return;
		}
		if( len!=strlen(err_baud_rate) ||  strcmp(recbuf, err_baud_rate) )
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,len=%d,recbuf:%s)", __LINE__, i, len, recbuf);
			goto ERR;
		}
	}
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, err_cmd[4]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_bt_addr), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	if(btflag) //910������������BLUE ADDR ERROR!!!
	{
		if( len!=strlen(err_bt_addr) ||  strcmp(recbuf, err_bt_addr) )
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}
	}
	else  //900������������command ERROR!!!
	{
		if( len!=strlen(err_cmd_return) ||  strcmp(recbuf, err_cmd_return) )
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,len=%d,recbuf:%s)", __LINE__, i, len, recbuf);
			goto ERR;
		}
	}
	if(btflag)
	{
		for(i=5;i<12;i++)
		{
			memset(sendbuf, 0, sizeof(sendbuf));
			strcpy(sendbuf, err_cmd[i]);
			memset(recbuf, 0, sizeof(recbuf));
			len=0;
			if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_cmd_return), &len, MAXTIME))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,ret=%d,sendbuf=%s,recbuf=%s)", __LINE__, i, ret, sendbuf, recbuf);
				return;
			}
			if( len!=strlen(err_cmd_return) ||  strcmp(recbuf, err_cmd_return) )
			{
				cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(i=%d,len=%d,recbuf:%s)", __LINE__, i, len, recbuf);
				goto ERR;
			}
		}
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, err_cmd[12]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_secmode), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			return;
		}
		if( len!=strlen(err_secmode) ||  strcmp(recbuf, err_secmode) )
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, err_cmd[13]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_bt_pairmode), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			return;
		}
		if( len!=strlen(err_bt_pairmode) ||  strcmp(recbuf, err_bt_pairmode) )
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}
		//����mac�����쳣���� sull 20180207
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, err_cmd[14]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret = sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(err_eth_addr), &len, MAXTIME)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			return;
		}
		if(len!=strlen(err_eth_addr) || strcmp(recbuf, err_eth_addr))
		{
			cls_show_msg1(g_keeptime, "line %d:�쳣��������ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}
	}

	//case1:��ȡ����汾��Ϣ
	cls_show_msg1(3,"��ȡԭ��������������汾��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[0]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, sizeof(recbuf), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:��ȡ����汾��Ϣʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	if(cls_show_msg("ȡ���������汾Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", recbuf) != ENTER)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,recbuf=%s)", __LINE__, sendbuf, recbuf);
		return;
	}

	//case2:�޸ĳ�һ����Ͳ�����9600
	cls_show_msg1(3,"�޸ĵ�BPS��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[6]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�޸ĵ�BPSʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	if(strcmp(recbuf, "OK\r\n")) 
	{
		cls_show_msg1(g_keeptime, "line %d:�޸ĵ�BPSʧ��(recbuf:%s)", __LINE__, recbuf);
		goto ERR;
	}
	if((ret=NDK_PortClose(PORT))!=NDK_OK) 
	{
		cls_show_msg1(g_keeptime, "line %d:�رմ���ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if((ret=NDK_PortOpen(PORT, "9600,8,N,1"))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�򿪴���ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case3:��ȡԭ����������
	cls_show_msg1(3,"��ȡԭ������������...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[1]);
	memset(old_btname, 0, sizeof(old_btname));
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_btname, sizeof(old_btname), &len1, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:��ȡԭ��������ʧ��(%d,sendbuf=%s,old_btname=%s)", __LINE__, ret, sendbuf, old_btname);
		goto ERR;
	}
	if(cls_show_msg("��ȡ������������%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_btname) != ENTER)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_btname=%s)", __LINE__, sendbuf, old_btname);
		goto ERR;
	}
	
	//case4:�޸ĳ���������
	cls_show_msg1(3,"�޸�����������...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[2]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_btname), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�޸���������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		goto ERR;
	}
	if( len!=strlen(modify_btname) ||  strcmp(recbuf, modify_btname) )
	{
		cls_show_msg1(g_keeptime, "line %d:�޸���������ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
		goto ERR;
	}
		
	//case5:�޸ĳ�ԭ����
	cls_show_msg1(3,"�޸ĳ�ԭ��������...");
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf,"AT#BM%s",old_btname+10);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, len1, &len, MAXTIME*3))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�޸�ԭ����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		goto ERR;
	}
	if(len!=len1 || strcmp(recbuf, old_btname) )
	{
		cls_show_msg1(g_keeptime, "line %d:�޸�ԭ����ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
		goto ERR;
	}

	//case6:�޸ĳ�һ��������57600
	cls_show_msg1(3,"�޸�BPS��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[8]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�޸�BPSʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		goto ERR;
	}
	if(strcmp(recbuf, "OK\r\n")) 
	{
		cls_show_msg1(g_keeptime, "line %d:�޸�BPSʧ��(recbuf=%s)", __LINE__, recbuf);
		goto ERR;
	}
	if((ret=NDK_PortClose(PORT))!=NDK_OK) 
	{
		cls_show_msg1(g_keeptime, "line %d:�رմ���ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if((ret=NDK_PortOpen(PORT, "57600,8,N,1"))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�򿪴���ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case7:��ȡԭ��BT��ַ
	cls_show_msg1(3,"��ȡԭ��������ַ��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[3]);
	memset(old_btaddr, 0, sizeof(old_btaddr));
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_btaddr, sizeof(old_btaddr), &len2, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:��ȡԭ������ַʧ��(%d,sendbuf=%s,old_btaddr=%s)", __LINE__, ret, sendbuf, old_btaddr);
		goto ERR;
	}
	if(cls_show_msg("��ȡ����������ַΪ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_btaddr) != ENTER)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_btaddr=%s)", __LINE__, sendbuf, old_btaddr);
		goto ERR;
	}
	
	//case8:�޸ĳ�������ַ
	cls_show_msg1(3,"�޸�������ַ��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[4]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_btaddr), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�޸�������ַʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		goto ERR;
	}
	if( len!=strlen(modify_btaddr) || strcmp(recbuf, modify_btaddr) )
	{
		cls_show_msg1(g_keeptime, "line %d:�޸�������ַʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
		goto ERR;
	}

	//case9:�޸ĳ�ԭ��ַ
	cls_show_msg1(3,"�޸ĳ�ԭ������ַ��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf,"AT#BZ%s",old_btaddr+10);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, len2, &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭ������ַʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		goto ERR;
	}
	if(len!=len2 || strcmp(recbuf, old_btaddr) )
	{
		cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭ������ַʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
		goto ERR;
	}
	if(btflag)
	{	
	//case10:��ȡԭ��WIFI MAC
		cls_show_msg1(3,"��ȡWIFI MAC��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[9]);
		memset(wifi_mac, 0, sizeof(wifi_mac));
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), wifi_mac, sizeof(wifi_mac), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡWIFI MACʧ��(%d,sendbuf=%s,wifi_mac=%s)", __LINE__, ret, sendbuf, wifi_mac);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����WIFI MACΪ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", wifi_mac) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,wifi_mac=%s)", __LINE__, sendbuf, wifi_mac);
			goto ERR;
		}
		
		//case11:��ȡԭ��WIFI AP����
		cls_show_msg1(3,"��ȡԭWIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[10]);
		memset(old_wifiname, 0, sizeof(old_wifiname));
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_wifiname, sizeof(old_wifiname), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭWIFI AP����ʧ��(%d,sendbuf=%s,old_wifiname=%s)", __LINE__, ret, sendbuf, old_wifiname);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����WIFI AP����Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_wifiname) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_wifiname=%s)", __LINE__, sendbuf, old_wifiname);
			goto ERR;
		}

		//case12:�޸ĳ�����WIFI AP����
		cls_show_msg1(3,"�޸�WIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[11]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_wifiname), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_wifiname) || strcmp(recbuf, modify_wifiname) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP����ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case13:�޸ĳ�ԭWIFI AP����
		cls_show_msg1(3,"�޸ĳ�ԭWIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#WM%s",old_wifiname+13);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_wifiname) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP����ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}

		//case14:�޸ĳ�Ĭ�ϲ�����115200  
		cls_show_msg1(3,"�޸�BPS��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[7]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�BPSʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(strcmp(recbuf, "OK\r\n")) 
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�BPSʧ��(recbuf=%s)", __LINE__, recbuf);
			goto ERR;
		}
		if((ret=NDK_PortClose(PORT))!=NDK_OK) 
		{
			cls_show_msg1(g_keeptime, "line %d:�رմ���ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		if((ret=NDK_PortOpen(PORT, "115200,8,N,1"))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�򿪴���ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		
		//case15:��ȡԭ��WIFI AP IP ��ַ
		cls_show_msg1(3,"��ȡԭWIFI AP IP ��ַ��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[12]);
		memset(old_wifi_ipaddr, 0, sizeof(old_wifi_ipaddr));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_wifi_ipaddr, sizeof(old_wifi_ipaddr), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭWIFI AP IP ��ַʧ��(%d,sendbuf=%s,old_wifi_ipaddr=%s)", __LINE__, ret, sendbuf, old_wifi_ipaddr);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����WIFI AP IP ��ַΪ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_wifi_ipaddr) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_wifi_ipaddr=%s)", __LINE__, sendbuf, old_wifi_ipaddr);
			goto ERR;
		}

		//case16:�޸ĳ�����WIFI AP IP ��ַ
		cls_show_msg1(3,"�޸�WIFI AP IP ��ַ��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[13]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_wifi_ipaddr), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP IP ��ַʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_wifi_ipaddr) || strcmp(recbuf, modify_wifi_ipaddr) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP IP ��ַʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case17:�޸ĳ�ԭWIFI AP IP ��ַ
		cls_show_msg1(3,"�޸ĳ�ԭWIFI AP IP ��ַ��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#WI%s",old_wifi_ipaddr+11);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP IP ��ַʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_wifi_ipaddr) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP IP ��ַʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}
		
		 //Ŀǰֻ֧�ֵ����115200�Ĳ�����
		//case18:�޸ĳ���߲�����380400  
				
		//case19:��ȡԭ��WIFI AP ��������
		cls_show_msg1(3,"��ȡԭWIFI AP����������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[14]);
		memset(old_wifimask, 0, sizeof(old_wifimask));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_wifimask, sizeof(old_wifimask), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭWIFI AP��������ʧ��(%d,sendbuf=%s,old_wifimask=%s)", __LINE__, ret, sendbuf, old_wifimask);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����WIFI AP��������Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_wifimask) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_wifimask=%s)", __LINE__, sendbuf, old_wifimask);
			goto ERR;
		}

		//case20:�޸ĳ�����WIFI AP��������
		cls_show_msg1(3,"�޸�WIFI AP����������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[15]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_wifimask), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP��������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_wifimask) || strcmp(recbuf, modify_wifimask) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP��������ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case21:�޸ĳ�ԭWIFI AP��������
		cls_show_msg1(3,"�޸ĳ�ԭWIFI AP����������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#WS%s",old_wifimask+13);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP��������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_wifimask) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP��������ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}

		//case22:��ȡԭ��WIFI AP ����
		cls_show_msg1(3,"��ȡԭWIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[16]);
		memset(old_wifipwd, 0, sizeof(old_wifipwd));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_wifipwd, sizeof(old_wifipwd), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭWIFI AP����ʧ��(%d,sendbuf=%s,old_wifipwd=%s)", __LINE__, ret, sendbuf, old_wifipwd);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����WIFI AP����Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_wifipwd) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_wifipwd=%s)", __LINE__, sendbuf, old_wifipwd);
			goto ERR;
		}

		//case23:�޸ĳ�����WIFI AP����
		cls_show_msg1(3,"�޸�WIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[17]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_wifipwd), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_wifipwd) || strcmp(recbuf, modify_wifipwd) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP����ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case24:�޸ĳ�ԭWIFI AP����
		cls_show_msg1(3,"�޸ĳ�ԭWIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#WK%s",old_wifipwd+17);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_wifipwd) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP����ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}
		
		//case25:�޸ĳ�Ĭ�ϲ�����115200  
		cls_show_msg1(3,"�޸�BPS��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[7]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�BPSʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(strcmp(recbuf, "OK\r\n")) 
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�BPSʧ��(recbuf=%s)", __LINE__, recbuf);
			goto ERR;
		}
		if((ret=NDK_PortClose(PORT))!=NDK_OK) 
		{
			cls_show_msg1(g_keeptime, "line %d:�رմ���ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		if((ret=NDK_PortOpen(PORT, "115200,8,N,1"))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�򿪴���ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		
		//case26:��ȡԭ����SN��
		cls_show_msg1(3,"��ȡԭSN����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[18]);
		memset(old_sn, 0, sizeof(old_sn));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_sn, sizeof(old_sn), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭSN��ʧ��(%d,sendbuf=%s,old_sn=%s)", __LINE__, ret, sendbuf, old_sn);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����SN��Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_sn) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_sn=%s)", __LINE__, sendbuf, old_sn);
			goto ERR;
		}

		//case27:�޸ĳ�����SN��
		cls_show_msg1(3,"�޸�SN����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[19]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_sn), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�SN��ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_sn) || strcmp(recbuf, modify_sn) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�SN��ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case28:�޸ĳ�ԭWIFI AP����
		cls_show_msg1(3,"�޸ĳ�ԭSN����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#SN%s",old_sn+3);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭSN��ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_sn) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭSN��ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}
		
		//case29:��ȡԭ����PN��
		cls_show_msg1(3,"��ȡԭPN����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[20]);
		memset(old_pn, 0, sizeof(old_pn));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_pn, sizeof(old_pn), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭPN��ʧ��(%d,sendbuf=%s,old_pn=%s)", __LINE__, ret, sendbuf, old_pn);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����PN��Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_pn) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_pn=%s)", __LINE__, sendbuf, old_pn);
			goto ERR;
		}

		//case30:�޸ĳ�����PN��
		cls_show_msg1(3,"�޸�PN����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[21]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_pn), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�PN��ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_pn) || strcmp(recbuf, modify_pn) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�PN��ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case31:�޸ĳ�ԭPN��
		cls_show_msg1(3,"�޸ĳ�ԭPN����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#PN%s",old_pn+3);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭPN��ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_pn) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭPN��ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}

		//case32:��ȡԭ���Ļ���
		cls_show_msg1(3,"��ȡԭ������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[23]);
		memset(old_jh, 0, sizeof(old_jh));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_jh, sizeof(old_jh), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭ����ʧ��(%d,sendbuf=%s,old_jh=%s)", __LINE__, ret, sendbuf, old_jh);
			goto ERR;
		}
		if(cls_show_msg("��ȡ���Ļ���Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_jh) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_jh=%s)", __LINE__, sendbuf, old_jh);
			goto ERR;
		}
		
		//case33:�޸Ļ���	
		cls_show_msg1(3,"�޸Ļ�����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[24]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_jh), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸Ļ���ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_jh) || strcmp(recbuf, modify_jh) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸Ļ���ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}
		
		//case34:�ָ���ԭ���Ļ���
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#JH%s",old_jh+3);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		cls_show_msg("%s",sendbuf);
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭ������ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_jh) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭ������ʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}
		
		//case35:дU�̲����ļ�
		cls_show_msg("ȷ�����������Ѳ���U��,���������");
		cls_show_msg1(3,"дU�̲����ļ���...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[25]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(udisk_wr), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:дU�̲����ļ�ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(udisk_wr) || strcmp(recbuf, udisk_wr) )
		{
			cls_show_msg1(g_keeptime, "line %d:дU�̲����ļ�ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case36:��U�̲����ļ�
		cls_show_msg1(3,"��U�̲����ļ���...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[26]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(udisk_str), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��U�̲����ļ�ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(udisk_str) || strcmp(recbuf, udisk_str) )
		{
			cls_show_msg1(g_keeptime, "line %d:��U�̲����ļ�ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}

		//case37:�ϴ�U�̲����ļ�
		cls_show_msg1(3,"�ϴ�U�̲����ļ���...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[27]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, sizeof(recbuf), &reclen, MAXTIME+1))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�ϴ�U�̲����ļ�ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����U�̲����ļ�����Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", recbuf) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,recbuf=%s)", __LINE__, sendbuf, recbuf);
			goto ERR;
		}

		//case38:��ȡԭ���Լ�¼�ļ�
		cls_show_msg1(3,"��ȡԭ���Լ�¼�ļ���...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[28]);
		memset(old_record, 0, sizeof(old_record));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), old_record, sizeof(old_record), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭ���Լ�¼�ļ�ʧ��(%d,sendbuf=%s,old_record=%s)", __LINE__, ret, sendbuf, old_record);
			goto ERR;
		}
		if(cls_show_msg("��ȡ����ԭ���Լ�¼�ļ�����Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", old_record) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_record=%s)", __LINE__, sendbuf, old_record);
			goto ERR;
		}

		//case39:д���Լ�¼�ļ�
		cls_show_msg1(3,"д���Լ�¼�ļ���...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[29]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:д���Լ�¼�ļ�ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( strcmp(recbuf, "OK\r\n") )
		{
			cls_show_msg1(g_keeptime, "line %d:д���Լ�¼�ļ�ʧ��(recbuf:%s)", __LINE__, recbuf);
			goto ERR;
		}
		
		//case40:��ȡд��Ĳ��Լ�¼�ļ�
		cls_show_msg1(3,"��ȡд��Ĳ��Լ�¼�ļ���...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[28]);
		memset(new_record, 0, sizeof(new_record));
		reclen = 0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), new_record, sizeof(new_record), &reclen, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭ���Լ�¼�ļ�ʧ��(%d,sendbuf=%s,new_record=%s)", __LINE__, ret, sendbuf, new_record);
			goto ERR;
		}
		if(cls_show_msg("��ȡ�����²��Լ�¼�ļ�����Ϊ%s,����ʵ�ʶ����Ƿ���ȷ,��[ENTER],����[����]", new_record) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,new_record=%s)", __LINE__, sendbuf, new_record);
			goto ERR;
		}
#if 0  //����ȷ����Ч���ò��������ѷ��� ������ 20180208
		//case41:��Ч���ò���:��������������,����Ч
		cls_show_msg1(3,"�޸�WIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[11]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_wifiname), &len, MAXTIME+10))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( len!=strlen(modify_wifiname) || strcmp(recbuf, modify_wifiname) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸�WIFI AP����ʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			goto ERR;
		}
		if(cls_show_msg("ʹ���ֻ���������,���Ƿ��ܹ�������%s��WiFi,��[ENTER],����[����]", modify_wifiname) == ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(Ӧ�ò����������޸ĺ��WiFi AP)", __LINE__);
			goto ERR;
		}
		cls_show_msg1(3,"��Ч���ò�����...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[30]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ч���ò���ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( strcmp(recbuf, "OK\r\n") )
		{
			cls_show_msg1(g_keeptime, "line %d:��Ч���ò���ʧ��(recbuf:%s)", __LINE__, recbuf);
			goto ERR;
		}
		if(cls_show_msg("ʹ���ֻ���������,���Ƿ��ܹ�������%s��WiFi,��[ENTER],����[����]", modify_wifiname) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(Ӧ��Ҫ���������޸ĺ��WiFi AP)", __LINE__);
			goto ERR;
		}
		//�ָ���ԭ����WiFi AP ����
		cls_show_msg1(3,"�޸ĳ�ԭWIFI AP������...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf,"AT#WM%s",old_wifiname+13);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(strcmp(recbuf, old_wifiname) )
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭWIFI AP����ʧ��(sendbuf=%s,recbuf:%s)", __LINE__, sendbuf, recbuf);
			goto ERR;
		}
		//��Ч
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[30]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME+10))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ч���ò���ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if( strcmp(recbuf, "OK\r\n") )
		{
			cls_show_msg1(g_keeptime, "line %d:��Ч���ò���ʧ��(recbuf:%s)", __LINE__, recbuf);
			goto ERR;
		}
#endif
		//case42:������ȡԭ������̫��mac��ַ  sull add  20180208
		cls_show_msg1(3,"��ȡԭ����̫��mac��ַ��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[31]);
		memset(old_ethaddr, 0, sizeof(old_ethaddr));
		reclen = 0;
		if((ret = sendrecv_cmd(sendbuf, strlen(sendbuf), old_ethaddr, sizeof(old_ethaddr), &reclen, MAXTIME)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��ȡԭ��̫��mac��ַʧ��(%d,sendbuf=%s,old_ethaddr=%s)", __LINE__, ret, sendbuf, old_ethaddr);
			goto ERR;
		}
		if(cls_show_msg("��ȡ������̫��mac��ַΪ%s,����ʵ�ʶ����Ƿ���ȷ����[ENTER]������[����]",old_ethaddr) != ENTER)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(sendbuf=%s,old_ethaddr=%s)", __LINE__, sendbuf, old_ethaddr);
			goto ERR;
		}

		//case43:�޸���̫��mac��ַ
		cls_show_msg1(3,"�޸���̫��mac��ַ��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		strcpy(sendbuf, cmd[32]);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret = sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen(modify_ethaddr), &len, MAXTIME)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸���̫��mac��ַʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=strlen(modify_ethaddr) || strcmp(recbuf, modify_ethaddr))
		{
			cls_show_msg1(g_keeptime, "line %d:�޸���̫��mac��ַʧ��(len=%d,recbuf:%s)", __LINE__, len, recbuf);
			GOTOERR;
		}

		//case44:�ָ���ԭ������̫��mac��ַ
		cls_show_msg1(3,"�ָ���ԭ��̫��mac��ַ��...");
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf, "AT#EZ%s", old_ethaddr+8);
		memset(recbuf, 0, sizeof(recbuf));
		len=0;
		if((ret = sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, reclen, &len, MAXTIME)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭ��̫��mac��ַʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
			goto ERR;
		}
		if(len!=reclen || strcmp(recbuf, old_ethaddr))
		{
			cls_show_msg1(g_keeptime, "line %d:�޸ĳ�ԭ��̫��mac��ַʧ��(len=%d,sendbuf=%s,recbuf:%s)", __LINE__, len, sendbuf, recbuf);
			goto ERR;
		}
	}

	cls_show_msg_record(FILENAME,FUNCNAME,"�����������Գɹ�");
	
ERR:
	cls_show_msg1(3,"�ָ�������������Ĭ��BPS��...");	
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, cmd[7]);
	memset(recbuf, 0, sizeof(recbuf));
	len=0;	
	sendrecv_cmd(sendbuf, sizeof(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME);
	if(strcmp(recbuf, "OK\r\n")) 
		cls_show_msg1(g_keeptime, "line %d:�ָ�Ĭ��BPSʧ��,��ͨ�����ڹ��߻ָ�Ĭ�ϲ�����(recbuf:%s)", __LINE__, recbuf);
	NDK_PortClose(PORT);
	return;
}
void setting_bt_pairmode(void)
{
	int nkeyin = 0, ret = -1, len = 0;
	char sendbuf[128] = {0}, recbuf[128] = {0};

	cls_show_msg("�뽫POS����1��������������ͨ��������������,�������������ϵĸ�λ��2s����빤��ģʽ");
	if((ret=NDK_PortOpen(PORT, "115200,8,N,1"))!=NDK_OK)//Ĭ�����
	{
		cls_show_msg1(g_keeptime, "line %d:�򿪴���ʧ��(%d)", __LINE__, ret);
		return;
	}
	//��������һ��ָ��(ȡ�汾),��֤�ѽ��빤��ģʽ(�������sn,��ѵ�һ��ָ������빤��ģʽ��ָ��)
	cls_printf("���빤��ģʽ��...");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, "AT#BV\r\n");
	memset(recbuf, 0, sizeof(recbuf));	
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, sizeof(recbuf), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	cls_show_msg1(2,"�������������ģʽ");
	do
	{
		nkeyin = cls_show_msg(  "1.Just Work\n"
							"2.PIN Code\n"
							"3.SSP PIN\n"
							"4.PassKey"
							);
		switch(nkeyin)
		{	
			case '1':
				strcpy(sendbuf, "AT#BP0\r\n");
				break;
			case '2':
				strcpy(sendbuf, "AT#BP1\r\n");
				break;
			case '3':
				strcpy(sendbuf, "AT#BP2\r\n");
				break;
			case '4':
				strcpy(sendbuf, "AT#BP3\r\n");
				break;
			case ESC:
				return;
				break;
			default:
				continue;
				break;
		} 
	}while(nkeyin!=0 && nkeyin!=ESC && (nkeyin<'1'||nkeyin>'4'));
	memset(recbuf, 0, sizeof(recbuf));	
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("OK\r\n"), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�����������ģʽʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
	if(strcmp(recbuf, "OK\r\n")) 
	{
		cls_show_msg1(g_keeptime, "line %d:�����������ģʽʧ��(recbuf=%s)", __LINE__, recbuf);
		return;
	}
	cls_show_msg1(2,"�����������ģʽ�ɹ�");
	//�˳�����ģʽ	
	cls_show_msg1(2,"�˳�����ģʽ");
	memset(sendbuf, 0, sizeof(sendbuf));
	strcpy(sendbuf, "AT#BX\r\n");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=sendrecv_cmd(sendbuf, strlen(sendbuf), recbuf, strlen("exit Production Mode!"), &len, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,sendbuf=%s,recbuf=%s)", __LINE__, ret, sendbuf, recbuf);
		return;
	}
}
	
void btdesk(void)
{
	int keyin = 0;

	while(1)
	{
		  keyin = cls_show_msg("1.������������\n"
						"2.�����������ģʽ\n"
						);
		switch(keyin) 
		{
		case '1':
			n900_btdesk();
			break;
		case '2':
			setting_bt_pairmode();
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
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		chensl		   20130131	 		created
*****************************************************************/
void systest9(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);	
	while(1)
    {
        keyin = cls_show_msg("1.�������ֻ�\n"
						"2.POS<->PC\n"
						"3.POS<---PC\n"
						"4.�����Է�\n"
						"5.������������");
		switch(keyin) 
		{
		case '1':
			aux_circleinit();
			break;
		case '2':
			aux_comm_pc();
			break;
		case '3':
			aux_pctopos();
			break;
		case '4':
			aux_comm_self();
			break;
		case '5':
			btdesk();
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

