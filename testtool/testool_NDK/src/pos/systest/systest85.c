/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ������ģʽ�ۺ�	
* file name			: systest85.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ������ģʽ�ۺϲ���
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20131202  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"
#include <pthread.h>

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"������ģʽ�ۺ�"
#define MAXWAITTIME		30
#define DEFAULT_CNT_VLE	20
#define	DEFAULT_CNT_STR	"20"
#define	PACKETLIFE		(10)
#define	MAX_SIZE	BUFSIZE_BT-MIN_BTCMD_LEN

/*------------global variables definition-----------------------*/
int thread_exit_flag = 0;
int connect_type = 0;
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
* history 				: author			date			remarks
					 	jiangym			20150611	created
*****************************************************************/	
#if 1
static void create_bt_ability_packet(Packet *packet, char *buf)
{
	/*process body*/
	memset(packet, 0, sizeof(Packet));
	packet->header = buf;//���ð�ͷ
	packet->len = packet->orig_len= BUFSIZE_BT;
	packet->lifecycle = PACKETLIFE;//10;
	packet->forever = FALSE;
	packet->IsLenRec = FALSE;
	packet->IsDataRnd = TRUE;

	return;
}
#endif
#if 0
//ɨ���ȡmac
int lib_btmaster_scan(char *pszMac, int dectype)
{
	int ret = 0,snum=0,i=0;
	ST_BT_DEV sresult[15];
	
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		cls_show_msg1(5, "line %d:���豸ɨ��ʧ��(%d)", __LINE__, ret);
		return ret;	
	}
	
	memset(sresult,0,sizeof(sresult));
	while(1)
	{
		if(cls_show_msg1(1,"����ɨ��������ģʽ�豸,ESC���˳�")==ESC)
			break;
		if((ret=NDK_BTMasterGetScanResults(NULL,sresult,15,&snum))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸��ȡɨ����ʧ��(%d)", __LINE__, ret);
			NDK_BTMasterStopScan();
			return ret;
		}
		if (snum > 0)
		{
			for (i=0; i<snum; i++)
			{
				if(cls_show_msg("�Ƿ�ѡ��BT[%d]:%s MACΪ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x��Ϊ���Դ��豸,�ǰ�[ȷ��],��������", i, sresult[i].sBtName,sresult[i].sBtMac[0],sresult[i].sBtMac[1],sresult[i].sBtMac[2],sresult[i].sBtMac[3],sresult[i].sBtMac[4],sresult[i].sBtMac[5])==ENTER)
				{
					strcpy(pszMac,sresult[i].sBtMac);
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

//�������豸�Ĳ���(�����ͻ���)
static void bt_client(void)
{
	int ret = -1, keyin = 0;
	int link_status =0, accept = 1, flag = 0, ulen = 0;
	char pkey[7] = {0};
	char recbuf[BUFSIZE_BT]={0} ;
	int len = 0;
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	/*process body*/
	cls_show_msg("�����ͻ����Ƚ��������������ģʽ,���������");
	while(1)
    {
        keyin = cls_show_msg("1.������\n"
							 "2.�����շ�\n"
							 "3.�Ͽ��ر�\n"
							 "0.����");
		switch(keyin) 
		{
		case '1':
			if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
			{
				cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
				return ;
			}
			cls_show_msg1(1,"�������ɹ�");
			if(g_pair_mode==2||g_pair_mode==3)
			{
				while(1)
				{
					accept = 1;
					NDK_BTStatus(&link_status);
					if(!link_status)//�жϳ������� ֱ�ӷ���??
					{
						cls_show_msg("���豸����豸������,���������");
						return;
					}
					else
					{	ret = NDK_BTGetPairingStatus(pkey,&flag);
						if(ret == NDK_ERR_NOT_SUPPORT)//������֧�ָù���
						{
							cls_show_msg("��ȷ�ϴ�ģʽ����ģ���Ƿ�֧�ִ����ģʽ");
							return;
						}
						else if(ret == NDK_ERR)//���ģʽ��ҪΪSSP��PassKeyģʽ
						{
							cls_show_msg("���õ����ģʽ��֧�ֱ�����,������SSP��PassKeyģʽ");
							return;
						}
						else if(ret == NDK_OK)
						{
							switch(flag)
							{
								case 0:
									continue;
									break;
								case 1:
									if(pkey[0] == '\0')
									{
										cls_printf("�������豸����ʾ�������:\n");
										if((ret=lib_kbgetinput(pkey, 0, 6, &ulen, INPUTDISP_PASSWD, 0, INPUT_CONTRL_LIMIT_ERETURN)) == NDK_ERR || ret == NDK_ERR_TIMEOUT)
										{
											accept = 0;
										}
									}
									else
									{
										if(cls_show_msg("[Enter]ȷ�����,[����]ȡ�����")==ENTER)
											accept = 1;
										else
											accept = 0;
									}
									NDK_BTConfirmPairing(pkey, accept);
									break;
								case 2:
									cls_show_msg1(5,"��Գɹ�,���������");
									//return ret;
									break;
								case 3:
									if(cls_show_msg("���ʧ��,[ESC]�˳�,[����]�����������")==ESC)
										return;
									cls_printf("�뽫��POS���POS�����������");
									memset(pkey,0,sizeof(pkey));
									break;
								default:
									cls_show_msg("��⵽δ֪�����״̬(%d)",flag);
									return;
									break;
							}
						}
						else
						{
							cls_show_msg1(5,"δ֪�ķ���ֵ(%d)",ret);
							return;
						}
					}
				}
			}
			else	
				cls_show_msg("��POS�ȴ���POS��֮���,��Գɹ������������");
			break;
		case '2':
			memset(recbuf, 0, sizeof(recbuf));
			if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)	 
			{				
				cls_show_msg("line %d:�建����ʧ��(%d)",__LINE__,ret);
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
			while(1)
			{			
				//������
				ret = NDK_PortRead(PORT_NUM_BT,sizeof(recbuf),recbuf,8000,&len);
				if(ret==NDK_ERR_TIMEOUT || cls_show_msg1(1,"�����շ���.ESC�˳�")==ESC)
					break;
				else if(ret==NDK_OK)
				{
					//������
					update_BT_packet(recbuf,len);
					if((ret=NDK_PortWrite(PORT_NUM_BT, len, recbuf)) != NDK_OK)	/*���������豸��������*/
					{
						cls_show_msg("line %d:���豸��������ʧ��(%d)", __LINE__, ret);
						NDK_PortClose(PORT_NUM_BT);
						return; 	
					}
				}
				else
				{
					cls_show_msg("line %d:���豸��������ʧ��(%d)", __LINE__, ret);
					NDK_PortClose(PORT_NUM_BT);
					return;
				}						
			}
			break;
		case '3':
			if((ret=NDK_BTDisconnect()) != NDK_OK)
			{
				cls_show_msg("line %d:�Ͽ�����ʧ��(%d)", __LINE__, ret);
				return ;
			}
			if((ret=NDK_PortClose(PORT_NUM_BT)) != NDK_OK)
			{
				cls_show_msg("line %d:�ر���������ʧ��(%d)", __LINE__, ret);
				return ;
			}
			cls_show_msg1(1,"�Ͽ��ر������ɹ�");
			break;
		case '0':
			conf_conn_BT();
			break;
		case ESC:
			return;
		default:
			continue;
			break;	

		}
	}
	return;
}
#endif
//���ݴ���(˫��):���豸����豸�Ķ�дѹ������
static void bt_master_read_write(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, succ = 0;
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};
	int len = 0;
	char bt_mac[7] = {0};
	Packet sendpacket;
	
	/*process body*/
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//����(���豸��ʾ����)
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸���Ӵ��豸ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//δ��ԵĻ�������,����Եľ�ֱ��������
	if((ret=connect_pair_process(bt_mac,1))!=NDK_OK)
	{
		cls_show_msg("line %d:�����豸�������ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	cls_show_msg("�����ͻ��˴������շ�,���������");
	while(1)
	{
		if(cls_show_msg1(2,"��ʼ��%d�����������豸��дѹ��(�ѳɹ�%d��),ESC�˳�", ++i, succ)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
		{
			cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		//�����豸д����
		if((ret=NDK_BTMasterWrite(bt_mac,BUFSIZE_BT,sendBuf))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		//���豸�����豸����
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_BTMasterRead(bt_mac, BUFSIZE_BT, recvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)	
		{				
			cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, len);
			continue;
		}
		//�Ƚ������豸������
		if(len != BUFSIZE_BT)
		{
			cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, len, BUFSIZE_BT);
			continue;
		}
		if(memcmp(sendBuf, recvBuf, len))
		{
			cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
			continue;
		}
		succ++;
	}	
	//���豸�Ͽ�����
	if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	cls_show_msg("���������豸��дѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succ);
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}
//��·������
static void bt_master_connect(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, cnt =0, succ = 0;
	char bt_mac[7] = {0};
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen = 0;

	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//���豸�ȴ��������(���豸��ʾ����)
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	while(1)
	{
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;
		if(cls_show_msg1(2,"������·������ѹ��������\n�ܹ�:%d��,��ִ��:%d��,�ѳɹ�:%d��,��ESC�˳�",cnt,i-1,succ)==ESC)
			break;			
		//����
		if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸���Ӵ��豸ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		//δ��ԵĻ�������,����Եľ�ֱ��������
		if((ret=connect_pair_process(bt_mac,1))!=NDK_OK)
		{
			cls_show_msg("line %d:�����豸�������ʧ��(%d)", __LINE__, ret);
			continue;
		}
		//���豸�Ͽ�����
		if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		succ++;
	}
	cls_show_msg("�ܹ�����%d��������·������ѹ������,�ѳɹ�%d��",i-1,succ);
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}
//��·������+����������ѹ��
static void bt_master_flow(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, succ = 0;
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};
	int len = 0;
	char bt_mac[7] = {0};
	Packet sendpacket;
	
	/*process body*/
	init_snd_packet(&sendpacket, sendBuf);
	set_snd_packet(&sendpacket);
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	while(1)
	{
		if(cls_show_msg1(2,"��ʼ��%d�����豸��������ѹ������(�ѳɹ�%d��),ESC�˳�", ++i, succ)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		
		cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
		//����
		if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸���Ӵ��豸ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		//δ��ԵĻ�������,����Եľ�ֱ��������
		if((ret=connect_pair_process(bt_mac,1))!=NDK_OK)
		{
			cls_show_msg("line %d:�����豸�������ʧ��(%d)", __LINE__, ret);
			continue;
		}

		cls_show_msg("�����ͻ��˴������շ�,���������");
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
		{
			cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		//�����豸д����
		if((ret=NDK_BTMasterWrite(bt_mac,BUFSIZE_BT,sendBuf))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		//���豸�����豸����
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_BTMasterRead(bt_mac, BUFSIZE_BT, recvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)	
		{				
			cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, len);
			continue;
		}
		//�Ƚ������豸������
		if(len != BUFSIZE_BT)
		{
			cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, len, BUFSIZE_BT);
			continue;
		}
		if(memcmp(sendBuf, recvBuf, len))
		{
			cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
			continue;
		}
		//���豸�Ͽ�����
		if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
			continue;	
		}
		succ++;
	}	
	cls_show_msg("�������豸����ѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succ);
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}
//���豸����ѹ������������
static void bt_master_press()
{
	/*private & local definition*/
	int key=0;
	
	/*process body*/
	while (1)
	{
		key = cls_show_msg("1.����ѹ��\n"
							"2.����ѹ��\n"
							"3.����+����ѹ��");
		
		switch(key)
		{
		case '1':
			bt_master_read_write();
			break;
		case '2':	
			bt_master_connect();
			break;	
		case '3':
			bt_master_flow();
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
#if 1
//���豸����豸�Ķ�д����
static void bt_master_ability()
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0;
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};
	char bt_mac[7] = {0};
	Packet sendpacket;
	float commtimes = 0, rate = 0;
	
	/*process body*/
	create_bt_ability_packet(&sendpacket, sendBuf);
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return ;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//����(���豸��ʾ����)
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸���Ӵ��豸ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//δ��ԵĻ�������,����Եľ�ֱ��������
	if((ret=connect_pair_process(bt_mac,1))!=NDK_OK)
	{
		cls_show_msg("line %d:�����豸�������ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	cls_show_msg("�����ͻ��˴������շ�,���������");
	cls_printf("���ܲ�����..");
	while(1)
	{
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
		{
			cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		lib_StartStopwatch();
		//�����豸д����
		if((ret=NDK_BTMasterWrite(bt_mac,sendpacket.len,sendBuf))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		//���豸�����豸����
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_BTMasterRead(bt_mac, sendpacket.len, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)	
		{				
			cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, recvLen);
			goto ERR;
		}
		commtimes+=lib_ReadStopwatch();
		//�Ƚ������豸������
		if(recvLen != sendpacket.len)
		{
			cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, sendpacket.len);
			goto ERR;
		}
		if(MemCmp(sendBuf, recvBuf, sendpacket.len))
		{
			cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
			goto ERR;
		}
	}	
	//���豸�Ͽ�����
	if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	
	rate = (float)sendpacket.len/(commtimes/PACKETLIFE);
	cls_show_msg("���������豸��д����ͨѶ����Ϊ%fB/s", rate);
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}
#endif
//�����豸�����ݶ�д�߳�
void *pthread_master(char *mac)
{
	/*private & local definition*/
	int ret = -1, j = 0, k = 0,  len = 0;
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};
	
	//���ɲ�������
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	
	while(!thread_exit_flag)
	{	
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(mac)) != NDK_OK)
		{
			cls_show_msg("line %d:���豸��[MACΪ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x]���豸����������ʧ��(%d)", __LINE__, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], ret);
			NDK_PthreadJoin(pthread_self(),NULL);//�˳��̵߳Ĳ���
	        return (void *)0;
		}
		//�����豸д����
		if((ret=NDK_BTMasterWrite(mac,BUFSIZE_BT,sendBuf))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�����豸[MACΪ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x]д����ʧ��(%d)", __LINE__, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], ret);
			NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;	
		}
		//���豸�����豸����
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_BTMasterRead(mac, BUFSIZE_BT, recvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)	
		{				
			cls_show_msg("line %d:���豸��ȡ���豸[MACΪ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x]����ʧ��(%d,%d)", __LINE__, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], ret, len);
			NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
		}
		//�Ƚ������豸������
		if(len != BUFSIZE_BT)
		{
			cls_show_msg("line %d:���豸����豸[MACΪ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x]���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], len, BUFSIZE_BT);
			NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
		}
		if(memcmp(sendBuf, recvBuf, len))
		{
			cls_show_msg("line %d:���豸����豸[MACΪ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x]�շ�����У��ʧ��", __LINE__, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
		}
		if(k++ == 10)//ͨѶ�������Ըı�  ͨѶ������20�θĳ�10�� 
			thread_exit_flag = 1;
	}	

	//�˳��߳�
	return (void *)0;
}

//���豸��Ϊ���豸�����ݶ�д�߳�
void *pthread_slave(void *arg)
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0, j = 0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	int cmdlen = 0;
    char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	/*process body*/
	//���ɲ�������
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	update_BT_packet(sendBuf,MAX_SIZE);
	while(!thread_exit_flag)
	{
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK) 	//����ջ���
	    {				 
		    cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__, ret);		    
			NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
	    }
	    memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	    bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	    cmdlen=MAX_SIZE+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
	    if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	    {
		    cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);		    
			NDK_PthreadJoin(pthread_self(),NULL);//����ʧ�ܺ�ر��߳�
	        return (void *)0;
	    }
	    memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	    if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	    {
		    cls_show_msg("line %d:���մ�(%d)(ʵ��%d, Ԥ��%d)", __LINE__, ret, recvLen, cmdlen);
			NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
	    }
	    memset(recvBuf,0,sizeof(recvBuf));
	    bt_cmd_uppacket(cmdrecvBuf, MAX_SIZE, recvBuf);
	    if(memcmp(sendBuf, recvBuf, MAX_SIZE))
	    {
		    cls_show_msg("line %d:����У��ʧ��", __LINE__);
		    NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
	    }
	    memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	    bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	    if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	    {
		    cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		    NDK_PthreadJoin(pthread_self(),NULL);
	        return (void *)0;
	    }
		if(i++ == 10)
			thread_exit_flag= 1;
	}	
	//�˳��߳�
	return (void *)0;
}

//���豸���������豸,ͬʱ���豸����Ϊ���豸���ֻ�����,ʹ���ĸ��߳�ͬʱ����
//ȱ��:��������������,ֻ����ʾĳ̨���豸ʧ��,�����ܼ�ʱ�������߳̽���,��Ҫ��ִ������ܽ���
static void bt_master_slave()
{
	int ret = -1;
	char bt_mac[3][6] ={{0},{0},{0} };
	char outbuf[16] = {0};
	pthread_t tidp[3], tidp1;
	pthread_attr_t attr;
	int error = -1, error1 = -1, i = 0;
	
	
	/*process body*/
	//����ǰ��
	thread_exit_flag=0;
	NDK_PortClose(PORT_NUM_BT);
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return;
	}
	//����ɨ��������̨���豸
	cls_show_msg("���豸������̨���豸,���������");
	for(i=0;i<3;i++)
	{	//ɨ���ȡmac
		if((ret=lib_btmaster_scan(bt_mac[i], connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
			goto ERR;	
		}
		cls_show_msg("����[%d]�ͻ��˴򿪵ȴ����,���������",i+1);
		//�������
		if((ret=NDK_BTMasterBond(bt_mac[i]))!=NDK_OK)
	    {
		    cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		    goto ERR;
	    }
		//�������
	    if((ret=lib_btmaster_pairing(1))!=2)
	    {
		    cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		    goto ERR;
	    }
		//����
		if((ret=NDK_BTMasterConnect(bt_mac[i],connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
			goto ERR;	
		}
	} 
    cls_show_msg("�Ѵ��豸��Ϊ���豸���в���,���������");
	//�Ƿ�Ҫ���ò�ͬ��ģʽ
    memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		cls_show_msg("line %d:��ȡ��������ʧ��(%d)", __LINE__, ret);
		goto ERR2;
	}
    //����������������ΪSSPģʽ��  ����NDK_BTMasterBond�󣬵ײ����ģʽ�Ѿ��޸�ΪSSPģʽ����Ҫ���½������ã�ֻ֧������ģʽһ��
    NDK_BTSetPairingMode(PAIRING_MODE_SSP);
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
	    cls_show_msg("line %d:���ʧ��,���������(%d)", __LINE__,ret);
		goto ERR2;
	}
	cls_show_msg("��̨���豸�����ͻ��˴������շ������������");	
	sleep(1);
	cls_printf("�����շ����߳̽�����..");
	//Ԥ���ڴ�й©
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	//����3�������豸�����շ����̼߳�һ����ӵ�ģʽ
	for(i=0;i<3;i++)
	{
		if((error=NDK_PthreadCreate(&tidp[i],&attr,(void *)pthread_master,bt_mac[i]))!=0) 
		{
			cls_show_msg("line %d:�����豸�����շ��߳�ʧ��(%d)\n", __LINE__, error);
			goto ERR2;
		}
	}	
	//����1�����豸��Ϊ���豸���߳�
	if((error1=NDK_PthreadCreate(&tidp1,NULL,(void *)pthread_slave,NULL))!=0) 
	{
		cls_show_msg("line %d:���豸��Ϊ���豸�����շ��߳�ʧ��(%d)\n", __LINE__, error1);
		goto ERR2;
	}
	while(1)
	{
		if(thread_exit_flag==1)	
			break;
	}
	//�ȴ����߳̽���
	for(i=0;i<3;i++)
	{
		 NDK_PthreadJoin(tidp[i],NULL);
	}
	NDK_PthreadJoin(tidp1,NULL);
	cls_show_msg("����������ͨ��,���������");
ERR2:
	NDK_BTDisconnect();
ERR:
	for(i=0;i<3;i++)
		NDK_BTMasterDisconnect(bt_mac[i]);
	NDK_PortClose(PORT_NUM_BT);
	return;
}
//���豸���Ӷ�̨���豸��һ����Ӳ���
static void master_more_slave()
{	
	int ret = -1, i = 0, j = 0, recvLen = 0;
	char bt_mac[3][6] ={{0},{0},{0}};
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������

	/*process body*/
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	//���ɲ�������
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return;
	}
	//����ɨ��������̨���豸
	for(i=0;i<3;i++)
	{	//ɨ���ȡmac
		cls_show_msg("���豸����豸[%d]�������ӡ������շ�,���������",i+1);
		if((ret=lib_btmaster_scan(bt_mac[i], connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
			goto ERR;	
		}
		cls_show_msg("���豸[%d]�����ͻ��˴򿪵ȴ����,���������",i+1);
		//�������
	    if((ret=NDK_BTMasterBond(bt_mac[i]))!=NDK_OK)
	    {
		    cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		    goto ERR;
	    }
		//�������
	    if((ret=lib_btmaster_pairing(1))!=2)
	    {
		    cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		    goto ERR;
	    }
		//����
		if((ret=NDK_BTMasterConnect(bt_mac[i],connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
			goto ERR;	
		}
		cls_show_msg("���豸[%d]�����ͻ��˴������շ�,���������",i+1);
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(bt_mac[i])) != NDK_OK)
		{
			cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		//�����豸д����
		if((ret=NDK_BTMasterWrite(bt_mac[i],BUFSIZE_BT,sendBuf))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		//���豸�����豸����
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_BTMasterRead(bt_mac[i], BUFSIZE_BT, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)	
		{				
			cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, recvLen);
			goto ERR;
		}
		//�Ƚ������豸������
		if(recvLen != BUFSIZE_BT)
		{
			cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, BUFSIZE_BT);
			goto ERR;
		}
		if(MemCmp(sendBuf, recvBuf, BUFSIZE_BT))
		{
			cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
			goto ERR;
		}	   
	}
	cls_show_msg("����������ͨ��,���������");
ERR:
	for(i=0;i<3;i++)
		NDK_BTMasterDisconnect(bt_mac[i]);
	NDK_PortClose(PORT_NUM_BT);
	return;
}
//����ģʽ�������(����Ϊ���豸���ֻ�ͨѶ,����Ϊ���豸����豸����ͨѶ)
static void master_slave_coexist()
{
	/*private & local definition*/
	int ret = -1, j = 0;
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};
	int len = 0, recvLen = 0;
	char bt_mac[7] = {0};
	char outbuf[16] = {0};
    int cmdlen = 0;
    char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	NDK_PortClose(PORT_NUM_BT);
	//���ɲ�������
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	update_BT_packet(sendBuf,BUFSIZE_BT);
	
	//pos��Ϊ���豸���ж�д
	cls_show_msg("�Ѵ�pos��Ϊ���豸���в���,���������");
	conf_conn_BT();
	//conf_bt();//�Ƿ�ʹ��������ú��������û��������װ���򵥵�?
	if((ret = NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
		return;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		cls_show_msg("line %d:��ȡ��������ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if(g_pair_mode == 2 || g_pair_mode == 3)  // SSP PIN��PassKey�������ģʽ ��Ҫ�������״̬��ѯ�����ȷ�Ϻ���������Բ���
	{	
		cls_show_msg("��֮ǰ����Ե�����ȡ����Ժ����������");
		cls_printf("�ֻ�ʹ��BTFWATS�����豸����:%s�������",outbuf);
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg("line %d:���ʧ��,���������(%d)", __LINE__,ret);
			goto ERR;
		}
	}
	else //Just Work �� PIN Code�������ģʽ����ֱ�Ӳ������
		cls_show_msg("�ֻ�ʹ��BTFWATS,�����豸����:%s���Ӻ����������",outbuf);
	cls_show_msg1(3,"����%dB���ݶ�д",MAX_SIZE); 
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK) 	//����ջ���
	{				 
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		cls_show_msg("line %d:���մ�(%d)(ʵ��%d, Ԥ��%d)", __LINE__, ret, recvLen, cmdlen);
		goto ERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, MAX_SIZE, recvBuf);
	if(memcmp(sendBuf, recvBuf, MAX_SIZE))
	{
		cls_show_msg("line %d:����У��ʧ��", __LINE__);
		goto ERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	cls_show_msg("�Ѵ�pos��Ϊ���豸���в���,���������");
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	//�������
	if((ret=NDK_BTMasterBond(bt_mac))!=NDK_OK)
	{
		cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		goto ERR2;
	}
	//�������
	if((ret=lib_btmaster_pairing(1))!=2)
	{
		cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		goto ERR2;
    }
	//����
    if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
    {
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
		goto ERR2;	
	}
	cls_show_msg("�����ͻ��˴������շ�,���������");
	//��ָ���豸������NDK_BTMasterClrBuf
	if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
	{
		cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
		goto ERR2;	
	}
	//�����豸д����
	if((ret=NDK_BTMasterWrite(bt_mac,BUFSIZE_BT,sendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
		goto ERR2;	
	}
	//���豸�����豸����
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret=NDK_BTMasterRead(bt_mac, BUFSIZE_BT, recvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)	
	{				
		cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, len);
		goto ERR2;
	}
	//�Ƚ������豸������
	if(len != BUFSIZE_BT)
	{
		cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, len, BUFSIZE_BT);
		goto ERR2;
	}
	if(memcmp(sendBuf, recvBuf, len))
	{
		cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
		goto ERR2;
	}
	cls_show_msg("����������ͨ��,���������");
ERR2:
	NDK_BTMasterDisconnect(bt_mac);
ERR:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//����ģʽ�л�����
static void master_slave_change()
{
	/*private & local definition*/
	int ret = -1, i = 0, j = 0;
	char sendBuf[BUFSIZE_BT] = {0}, recvBuf[BUFSIZE_BT] = {0};
	int len = 0, recvLen = 0;
	char bt_mac[7] = {0};
	char outbuf[16] = {0};
    int cmdlen = 0;
    char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	/*process body*/
	while(1)
	{
		if(ESC==cls_show_msg1(2, "���ڽ��е�%d������ģʽ�л�����,��ESC�˳�", i+1))
			break;
		if( i++ > 3 )
			break;	
		
		//���ɲ�������
		for (j=0; j<sizeof(sendBuf); j++)
			sendBuf[j]=rand()%256;
		update_BT_packet(sendBuf,BUFSIZE_BT);
		
		//pos��Ϊ���豸����豸���ж�д
		//����ǰ��
		cls_show_msg("pos��Ϊ���豸���в���,���������");
		NDK_PortClose(PORT_NUM_BT);
		if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
		{
			cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
			return;
		}
		//ɨ���ȡmac
		if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_BT);
			return;
		} 
		cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
		//�������
		if((ret=NDK_BTMasterBond(bt_mac))!=NDK_OK)
	        {
		 	cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		 	NDK_PortClose(PORT_NUM_BT);
			return;
	         }
		//�������
	       if((ret=lib_btmaster_pairing(1))!=2)
	       {
		    	cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		    	NDK_PortClose(PORT_NUM_BT);
		   	return;
	        }
		//����
		if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
			NDK_PortClose(PORT_NUM_BT);
			return;	
		}
		cls_show_msg("�����ͻ��˴������շ�,���������");
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
		{
			cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_BT);
			return;	
		}
		//�����豸д����
		if((ret=NDK_BTMasterWrite(bt_mac,BUFSIZE_BT,sendBuf))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_BT);
			return;	
		}
		//���豸�����豸����
		memset(recvBuf,0,sizeof(recvBuf));
		if((ret=NDK_BTMasterRead(bt_mac, BUFSIZE_BT, recvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)	
		{				
			cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, len);
			NDK_PortClose(PORT_NUM_BT);
			return;
		}
		//�Ƚ������豸������
		if(len != BUFSIZE_BT)
		{
			cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, len, BUFSIZE_BT);
			NDK_PortClose(PORT_NUM_BT);
			return;
		}
		if(memcmp(sendBuf, recvBuf, len))
		{
			cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
			NDK_PortClose(PORT_NUM_BT);
			return;
		}
		//���豸�Ͽ�����
		if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_BT);
			return;	
		}
		NDK_PortClose(PORT_NUM_BT);

		//pos��Ϊ���豸���ж�д
		cls_show_msg("�Ѵ�pos��Ϊ���豸���в���,���������");
		conf_conn_BT();
		//conf_bt();
		if((ret = NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
			return;
		}
		memset(outbuf,0,sizeof(outbuf));
		if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
		{
			cls_show_msg("line %d:��ȡ��������ʧ��(%d)", __LINE__, ret);
			NDK_BTDisconnect();
			NDK_PortClose(PORT_NUM_BT);
			return;
		}
		if(g_pair_mode == 2 || g_pair_mode == 3)   
		{	
			cls_show_msg("��֮ǰ����Ե�����ȡ����Ժ����������");
			cls_printf("�ֻ�ʹ���Զ�������BTFWATS��POS:%s�������",outbuf);
			if((ret=lib_bt_pairing(1))!=NDK_OK)
			{
				cls_show_msg("line %d:���ʧ��,���������(%d)", __LINE__,ret);
				NDK_BTDisconnect();
				NDK_PortClose(PORT_NUM_BT);
				return;
			}
		}
		else //Just Work �� PIN Code�������ģʽ����ֱ�Ӳ������
			cls_show_msg("�ֻ�ʹ��BTFWATS,��POS:%s���Ӻ����������",outbuf);
	cls_show_msg1(3,"����%dB���ݶ�д",MAX_SIZE);
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)	//����ջ���
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__, ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		cls_show_msg("line %d:���մ�(%d)(ʵ��%d, Ԥ��%d)", __LINE__, ret, recvLen, cmdlen);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, MAX_SIZE, recvBuf);
	if(memcmp(sendBuf, recvBuf, MAX_SIZE))
	{
		cls_show_msg("line %d:����У��ʧ��", __LINE__);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		NDK_BTDisconnect();
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	//�Ͽ���������
	if((ret = NDK_BTDisconnect()) != NDK_OK)
	{
		cls_show_msg("line %d:�Ͽ���������ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_BT);
		return;
	}
	NDK_PortClose(PORT_NUM_BT);
	}
	cls_show_msg("�����л�����ͨ��,���������");
	return;
}

//��ʱ����ú���ʹ����������Ӧ�����ӳɹ�
static void master_abnormal_test1()
{
	int ret = -1;
	char bt_mac[7] = {0};
	
	cls_show_msg("��ʱ����û�����,�ٰ����������");
	if((ret = NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
    	return;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	} 
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	//�������
	if((ret=NDK_BTMasterBond(bt_mac))!=NDK_OK)
	{
	    cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		goto ERR;
	}
	//�������
    if((ret=lib_btmaster_pairing(1))!=2)
	{
		cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		goto ERR;
	}
	//����
    if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
		goto ERR;	
    }
	NDK_SysDelay(30);//��ֹ�����豸��δ��Ӧ����״̬ǰ����ֱ��ִ�жϿ����ӵĶ���
	//���豸�Ͽ�����
	if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	cls_show_msg("����������ͨ��,���������");
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//����(�ر�����)����֮������������Ӧ�ܳɹ�
static void master_abnormal_test2()
{
	int ret = -1;
	char bt_mac[7] = {0};
	
	cls_show_msg("�����������������,���ߺ󰴼�����");
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		return;
	}
	NDK_SysDelay(5); 
	if((ret = NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
    	return;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	} 
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	//�������
	if((ret=NDK_BTMasterBond(bt_mac))!=NDK_OK)
	{
	    cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		goto ERR;
	}
	//�������
    if((ret=lib_btmaster_pairing(1))!=2)
	{
		cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		goto ERR;
	}
	//����
    if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
		goto ERR;	
    }
	sleep(1);//�����Ƿ�����,������Ҫʱ��,����ʱ����Ͽ��᷵��-1  20180514 modify
	//���豸�Ͽ�����
	if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	cls_show_msg("����������ͨ��,���������");
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//����(���ر�����)���Ѻ�������ͨѶ�ɹ�
static void master_abnormal_test3()
{
	int ret = -1, len = 0;
	char bt_mac[7] = {0};
	char sendBuf[12]={0},recvBuf[12]={0};
	
	if((ret = NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
    	return;
	}
	//ɨ���ȡmac
	if((ret=lib_btmaster_scan(bt_mac, connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
		goto ERR;	
	} 
	cls_show_msg("�����ͻ��˴򿪵ȴ����,���������");
	//�������
	if((ret=NDK_BTMasterBond(bt_mac))!=NDK_OK)
	{
	    cls_show_msg("line %d:��Է���ʧ��(%d)", __LINE__,ret);
		goto ERR;
	}
	//�������
    if((ret=lib_btmaster_pairing(1))!=2)
	{
		cls_show_msg("line %d:���ʧ��(%d)", __LINE__,ret);
		goto ERR;
	}
	//����
    if((ret=NDK_BTMasterConnect(bt_mac,connect_type))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__,ret);
		goto ERR;	
    }
	cls_show_msg("��ȴ�Լ5��������������,���ߺ󰴼�����");
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	memset(sendBuf, 0x38, sizeof(sendBuf));
	cls_show_msg("�����ͻ��˴������շ�");
	//��ָ���豸������NDK_BTMasterClrBuf
	if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
	{
		cls_show_msg("line %d:���豸�建��������ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//�����豸д����
	if((ret=NDK_BTMasterWrite(bt_mac,sizeof(sendBuf),sendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�����豸д����ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	//���豸�����豸����
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret=NDK_BTMasterRead(bt_mac, sizeof(sendBuf), recvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)	
	{				
		cls_show_msg("line %d:���豸��ȡ����ʧ��(%d,%d)", __LINE__, ret, len);
		goto ERR;
	}
	//�Ƚ������豸������
	if(len != sizeof(sendBuf))
	{
		cls_show_msg("line %d:���豸���ݳ����շ���һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, len, BUFSIZE_BT);
		goto ERR;
	}
	if(memcmp(sendBuf, recvBuf, len))
	{
		cls_show_msg("line %d:���豸�շ�����У��ʧ��", __LINE__);
		goto ERR;
	}
	//���豸�Ͽ�����
	if((ret=NDK_BTMasterDisconnect(bt_mac))!=NDK_OK)
	{
		cls_show_msg("line %d:���豸�Ͽ�����ʧ��(%d)", __LINE__, ret);
		goto ERR;	
	}
	cls_show_msg("����������ͨ��,���������");
ERR:
	//�رմ���
	NDK_PortClose(PORT_NUM_BT);
	return;
}

//��ģʽ�����쳣����
static void bt_master_abnormal()
{
	int keyin = 0;
	
	while(1)
	{
		keyin = cls_show_msg("1.��ʱ�����\n"
							 "2.���߹ر�����\n"
							 "3.���߲��ر�����");
		switch(keyin) 
		{
			case '1':
				master_abnormal_test1();
				break;
			case '2':	
				master_abnormal_test2();
				break;	
			case '3':
				master_abnormal_test3();
				break;
			case ESC:
				return;
			default:
				continue;
				break;
		}
	}	
	return;
}

void bt_master_set(void)
{
	int keyin = 0;

	/*process body*/
	while(1)
	{
		keyin = cls_show_msg("��ѡ�����ӷ�ʽ:"
							"1.Ĭ�Ϸ�ʽ\n"
							 "2.SPP��ʽ\n" 
							 "3.BLE	��ʽ");
		switch(keyin) 
		{
			case '1':
				connect_type = 0;
				return;
				break;
			case '2':	
				connect_type = 1;
				return;
				break;	
			case '3':
				connect_type = 2;
				return;
				break;
			default:
				connect_type = 0;
				return;
				break;
		}
	}	
	return;
}

//��ģʽ��������
void systest85(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int keyin = 0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	
	setlinktype(BT);//����ͨѶ����ΪBT���������ݰ�̫������ʧ��
	while(1)
	{
		keyin = cls_show_msg("1.ѹ�� 2.�����л�\n"
							 "3.���Ӷ��߳� 8.����\n"   //3���Բ��5��6
							 "4.�쳣 5.���ӹ���\n"
							 "6.һ����� 7.���� ");
		switch(keyin) 
		{
			case '1':
				bt_master_press();
				break;
			case '2':	
				master_slave_change();
				break;	
			case '3':
				bt_master_slave();
				break;
			case '4':
				bt_master_abnormal();
				break;	
			case '5':
				master_slave_coexist();
				break;	
			case '6':
				master_more_slave();
				break;
			case '7': 
				bt_master_ability();
				break;
			case '8': 
				bt_master_set();
				break;
			case ESC:
				return;
			default:
				continue;
				break;
		}
	}	
	return;
}


