/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	USB��/3g���
* file name			: systest69.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: USB��/3g���
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

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"USB��/3g���"

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
* history 				: author			date			remarks
					 	jiangym			20140804	created
*****************************************************************/	
static void USB_WLM(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, j = 0, succ = 0, timeout = SO_TIMEO, slen = 0, rlen = 0, recvLen = 0 ;
	char recvBuf[BUFSIZE_SERIAL+1] = {0}, sendBuf[BUFSIZE_SERIAL+1]={0};
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*process body*/	
	//��ʼ�����ߺ�USB����
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	for(j=0;j<BUFSIZE_SERIAL;j++)
		sendBuf[j] = rand()%256;
	cls_show_msg("�뽫POS��PCͨ��USB�����Ӳ���AccessPort����,���������");
	
	while(1)
	{
		//��������
		NDK_PortClose(PORT_NUM_USB);
		TransDown(h_tcp);
		NetDown();
		
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		
		sleep(5);//ÿ�ιҶ�֮����������Ҫ�ȴ�5��,������оƬ��ѹ��
		
		if(NetUp()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, i);
			continue;
		}
		
		if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��%d�δ�USBʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		cls_show_msg("���AccessPort���ߴ���,���������");
			
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			continue;
		}
		
		if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
		{				
			cls_show_msg1(g_keeptime, "line %d:��%d����USB������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		
		//���߷�������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
			continue;
		}
		
		//usb��������
		if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		
		//���߽�������
		memset(rbuf, 0, sizeof(rbuf));
		if((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�����߽���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			goto ERR;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d����������У��ʧ��", __LINE__, i);
			goto ERR;
		}
		
		//usb��������
		cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ�����������������,���������");
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, recvBuf, timeout*1000, &recvLen)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, BUFSIZE_SERIAL);
			goto ERR;
		}
		//�Ƚ��շ�����
		if(recvLen != BUFSIZE_SERIAL)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, recvLen, BUFSIZE_SERIAL);
			goto ERR;
		}
		if(MemCmp(sendBuf, recvBuf, BUFSIZE_SERIAL))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��USB����У��ʧ��", __LINE__, i);
			goto ERR;
		}
			
		//���߹Ҷ�
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:��%d��TransDownʧ��", __LINE__, i);
			goto ERR;
		}
		//�ر�USB
		NDK_PortClose(PORT_NUM_USB);
		
		if (NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, i);
			goto ERR;
		}
		succ++;
ERR:
		cls_show_msg("��ر�AccessPort���ߴ��ڲ�����շ�������,���������");
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest69(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{
		keyin = cls_show_msg("1.USB��/3g�������\n"
							"0.��������");
		switch(keyin)
		{
		case '1':
			USB_WLM();
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

