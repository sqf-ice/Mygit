/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: portģ��
* file name		      : port7.c
* Author 			: 
* version			: 
* DATE				: 20140220
* directory 			: 
* description			: ����USB���ڹ���
* related document	: SDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"USB���ٴ���"
#define MAXWAITTIME	10//30
#define SNDCNT			(71)
#define SNDCNT1			(128)

/****************************************************************
* function name 	 	: void port7(void)
* functional description 	:USB���ڹ���
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	 	jiangym 		20140221   created
*****************************************************************/
void host_usb_test(void)
{
	int ret = 0;
	char sendbuf[BUFSIZE_BT]={0}, recbuf[BUFSIZE_BT]={0};
	int len = 0;
	//time_t oldtime=0;
	
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:�ر�Usb������
	if((ret=NDK_PortClose(PORT_NUM_USB_HOST))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:��pos���POS����ʱ,��POSδ�򿪴��ھͽ��з��ͣ�Ӧ�÷���NDK_ERR_OPEN_DEV
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB_HOST, sizeof(sendbuf),sendbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��pos���POS����ʱ,��POSδ�򿪴��ھͽ��н��գ�Ӧ�÷���NDK_ERR_OPEN_DEV
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST,sizeof(recbuf),recbuf, MAXWAITTIME*1000,&len))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"����������ͨ��");
	
	//case3:������ģʽ�򿪺�,��ģʽδ���ڳ�ʱʱ�������ӳɹ�,Ӧ��Ҫ���س�ʱ
	cls_show_msg("�뽫��POS���POS�Ͽ�USB����,���������");//���ǵ�һ���Ի������������˻�ɹ�����Ҫ�ε���
	if((ret=NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,2")) != NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:��pos���POS����,�򿪴���,���������շ�4K����
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,�򿪴�POS����,���������");
	if((ret=NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("������POS���շ����ݺ�,��POS������������%dkB���ݵ���POS", sizeof(sendbuf)/1024);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, sizeof(sendbuf), sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	cls_show_msg1(2,"��pos�����յ���POS������");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST, sizeof(recbuf), recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"�������");
	if((len!=sizeof(sendbuf))||memcmp(sendbuf, recbuf, len))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, sizeof(sendbuf));
		GOTOERR;
	}
	
	//case5:��pos���POS����,�򿪴���,���������շ�71�ֽ�����
	cls_show_msg("��POS������������%dB���ݵ���POS", SNDCNT);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, SNDCNT, sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}	
	cls_show_msg1(2,"��pos�����յ���POS������");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=NDK_PortRead(PORT_NUM_USB_HOST, SNDCNT, recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
   	}
	cls_show_msg1(2,"�������");
	if((len!=SNDCNT)||memcmp(sendbuf, recbuf, SNDCNT))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, SNDCNT);
		GOTOERR;
	}
	
	//case6:��pos���POS����,�򿪴���,���������շ�128�ֽ�����
	cls_show_msg("��POS������������%dB���ݵ���POS", SNDCNT1);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, SNDCNT1, sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	cls_show_msg1(2,"��pos�����յ���POS������");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=NDK_PortRead(PORT_NUM_USB_HOST, SNDCNT1, recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"�������");
	if((len!=SNDCNT1)||memcmp(sendbuf, recbuf, SNDCNT1))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, SNDCNT1);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);	
		
ERR:
	NDK_PortClose(PORT_NUM_USB_HOST);
	return;
}

void slave_usb()
{
	/*private & local definition*/
	int keyin = 0, ret = 0;
	char recbuf[BUFSIZE_BT]={0} ;
	int len = 0, reclen=0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTAPI);	
	
	while(1)
    {
        keyin = cls_show_msg("1.��USB����\n"
							"2.�շ�����\n"
							"3.�ر�USB����");
		switch(keyin) 
		{
		case '1':
			if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
				NDK_PortClose(PORT_NUM_USB);
				return;
			}
			break;
		case '2':	
			memset(recbuf, 0, sizeof(recbuf));
			if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
			{				
				cls_show_msg1(5,"line %d:�建����ʧ��(%d)",__LINE__,ret);
				NDK_PortClose(PORT_NUM_USB);
				return;
			}
			while(1)
			{	
				NDK_PortReadLen(PORT_NUM_USB,&reclen);	/*ȡ�������ֽ���*/
				if(reclen>0)
				{	
					//������
					cls_printf("����%d�ֽ�������",reclen);
					if((ret = NDK_PortRead(PORT_NUM_USB,reclen,recbuf,20000,&len)) != NDK_OK)
					{
						send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
						NDK_PortClose(PORT_NUM_USB);
						return;
					}
					
					//������
					cls_printf("����%d�ֽ�������",len);
					if((ret=NDK_PortWrite(PORT_NUM_USB, len, recbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
					{
						send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
						NDK_PortClose(PORT_NUM_USB);
						return;		
					}
				}
				if(cls_show_msg1(2,"�ȴ������շ���,ESC�˳�")==ESC)
					break;
			}
			break;
		case '3':
			if((ret=NDK_PortClose(PORT_NUM_USB))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;
			}
			break;
		case ESC:
			NDK_PortClose(PORT_NUM_USB);
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;

}

void port7(void)
{	
	/*private & local definition*/
	int keyin = 0;
	//EM_PORT_NUM usbport = PORT_NUM_USB;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTAPI);	
	while(1)
    {
        keyin = cls_show_msg("1.USB��ģʽ����\n"
							"2.USB��ģʽ����");
		switch(keyin) 
		{
		case '1':
			host_usb_test();
			break;
		case '2':
			slave_usb();
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


