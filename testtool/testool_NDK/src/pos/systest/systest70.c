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
#define MAX_SIZE  1024 //BUFSIZE_SERIAL 

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
					 	jiangym			20140808	created
*****************************************************************/	
static void HOST_USB_WLM(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, j = 0, succ = 0, timeout = SO_TIMEO, slen = 0, rlen = 0, recvLen = 0 ;
	char recvBuf[MAX_SIZE+1] = {0}, sendBuf[MAX_SIZE+1]={0};
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*process body*/
	//��ʼ�����ߺ�USB����
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	for(j=0;j<MAX_SIZE;j++)
		sendBuf[j] = rand()%256;
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");		
	
	while(1)
	{
		//��������
		NDK_PortClose(PORT_NUM_USB_HOST);
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
		//��ģʽ��USB����
		if((ret = NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��%d�δ�USBʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		cls_show_msg("��ģʽPOS�򿪴���,���������");
		
		//����
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			goto ERR;
		}

		//��USB
		if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
		{				
			cls_show_msg1(g_keeptime, "line %d:��%d����USB������ʧ��(%d)", __LINE__, i, ret);
			goto ERR;
		}
		
		//���߷�������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
			goto ERR;
		}
		
		//usb��ģʽ��������
		cls_show_msg("��ģʽPOS�����շ����ݺ�,��pos�������������");
		if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, MAX_SIZE, sendBuf))!=NDK_OK)
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
		
		//��usb��������
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB_HOST,MAX_SIZE, recvBuf, timeout*1000, &recvLen)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, MAX_SIZE);
			goto ERR;
		}
		//�Ƚ��շ�����
		if(recvLen != MAX_SIZE)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, recvLen, MAX_SIZE);
			goto ERR;
		}
		if(MemCmp(sendBuf, recvBuf, MAX_SIZE))
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
		
		//�رմ�USB
		cls_show_msg("�ȴ���POS�˳��շ����ݺ�ر�USB����,���������");
			
		if (NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NetDownʧ��", __LINE__, i);
			continue;
		}
		//�ر���USB
		NDK_PortClose(PORT_NUM_USB_HOST);		
		
		succ++;
		continue;
	ERR:
		cls_show_msg("�رմ�ģʽPOS��USB����,���������");
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest70(void)
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
	
	cls_show_msg("��POS����systest32,�򿪿ͻ��˲���,���������");
	
	while (1)
	{
		keyin = cls_show_msg("1.USB��/3g�������\n"
							"0.��������");
		switch(keyin)
		{
		case '1':
			HOST_USB_WLM();
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

