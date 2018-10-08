/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: portģ��
* file name		: port6.c
* Author 			: linrq
* version			: 
* DATE			: 20121022
* directory 		: 
* description		: ����USB���ڹ���
* related document	: SDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"USB���ٴ���"
#define MAXWAITTIME	10//30
#define SNDCNT			(71)
#define SNDCNT1			(128)

#if !K21_ENABLE
#define SNDTIMES		(10)//5892 ÿ�η���4K ������Ҫ����10��
#else
#define SNDTIMES		(40)//K21 ÿ�η���1K ������Ҫ����40��
#endif

/****************************************************************
* function name 	 	: void port6(void)
* functional description 	:USB���ڹ���
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  linrq      20121022	created
*****************************************************************/
void port6(void)
{	
	int ret = 0;
	char sendbuf[BUFSIZE_SERIAL]={0}, recbuf[BUFSIZE_SERIAL]={0};
	int len = 0,j = 0;
	time_t oldtime=0;
	
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:�ر�Usb����
	if((ret=NDK_PortClose(PORT_NUM_USB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:pos��PC����ʱ,δ�򿪾ͽ��з��ͣ�Ӧ�÷���NDK_ERR_OPEN_DEV
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB, sizeof(sendbuf),sendbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:pos��PC����ʱ,δ�򿪾ͽ��н��գ�Ӧ�÷���NDK_ERR_OPEN_DEV
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB,sizeof(recbuf),recbuf, MAXWAITTIME*1000,&len))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2,"����������ͨ��");

#if !K21_ENABLE//���ݻ������۽�� K21��֧��open�ж�USBδ����
	//case3:pos��PCδ����ʱ,��U��Ӧʧ�ܷ���NDK_ERR_USB_LINE_UNCONNECT
	cls_show_msg("���USB��,���������");
	if((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_ERR_USB_LINE_UNCONNECT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");
#endif
	
	//case4:pos��pc���Ӻ���ճ�ʱNDK_ERR_TIMEOUT
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if K21_ENABLE
	sleep(3);
#endif
	if((ret = NDK_PortRead(PORT_NUM_USB, sizeof(recbuf), recbuf, 1*1000, &len))!=NDK_ERR_TIMEOUT)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");

	//case5:�ε�USB�ߺ�������ʱ,Ӧʧ��,����NDK_ERR_USB_LINE_UNCONNECT
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���USB��,�����������");
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB,sizeof(sendbuf),sendbuf))!=NDK_ERR_USB_LINE_UNCONNECT)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");

	//case6:�ε�USB�ߺ��������ʱ,Ӧʧ��,����NDK_ERR_USB_LINE_UNCONNECT
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���USB��,�����������");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_ERR_USB_LINE_UNCONNECT)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");
	
	//case14:����USB�򿪺�PC�˹���δ�򿪣���������Ӧ�û�selectʧ�ܷ���NDK_ERR
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if K21_ENABLE
	sleep(3);//ME30 OPEN��PC��Ҫ3��ʱ������������
#endif
	memset(sendbuf, 0x38, sizeof(sendbuf));
	for(j = 0;j<SNDTIMES;j++)//���ݹ���Ľ��� USB���ߺ�ʹPC�˹���û�д򿪣���PC�˿�����USB���ڻ��壬����ǰ����д���ܻ�ɹ������޸��������д��ֻҪ����һ��дʧ�����ʾ����ͨ��
	{
		if((ret = NDK_PortWrite(PORT_NUM_USB,sizeof(sendbuf),sendbuf))==NDK_ERR)
			break;
	}
	if(j==SNDTIMES)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
/*	if((ret = NDK_PortWrite(PORT_NUM_USB,sizeof(sendbuf),sendbuf))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}*/
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");
	
	//case7:��usb�豸���ڷ�������ʱ���£�Ӧ����ʧ��NDK_ERR_USB_LINE_UNCONNECT
	cls_show_msg("�뽫POS��PCͨ��USB������,������PC�˵�accessport����,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
	    send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
	    GOTOERR;
	}
	cls_show_msg("PC�򿪴���,�������POS���������3s�����USB��");
	memset(sendbuf, 0x38, sizeof(sendbuf));
	oldtime=time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
		ret=NDK_PortWrite(PORT_NUM_USB,SNDCNT1,sendbuf);
		switch(ret)
		{
			case -1://�Ͷ˿��ܷ���-1 20150906
			case NDK_ERR_USB_LINE_UNCONNECT:
				break;	
			case NDK_OK:
				cls_printf("���ͳɹ�");
				break;
			default:
				send_result("line %d:%sδ֪�ķ���ֵ(ret=%d)", __LINE__, TESTAPI, ret);
				goto ERR;
		}
		if( ret== NDK_ERR_USB_LINE_UNCONNECT||ret==-1)
			break;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");

	//case8:��usb�豸���ڽ�������ʱ���£�Ӧ����ʧ��NDK_ERR_USB_LINE_UNCONNECT
	cls_show_msg("�뽫POS��PCͨ��USB������,�ر�PC���ں�����PC�˵�accessport����,���������");//��ע:K21��Ҫ�Ȱ������ܲ���PC��
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("PC�򿪴���,�����������");
	cls_printf("���USB��");
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_ERR_USB_LINE_UNCONNECT)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");

#if !K21_ENABLE
	//case9:Ҫ����豸�󣬽��������ٻ��ѣ�ֻ��Ҫ��ȷ�������߽ӿڣ������Ƿ������ʵ�����ߣ���Ӧ���������ؽ��������շ�
	cls_show_msg("�뽫POS��PCͨ��USB������,������PC�˵�accessport����,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysGoSuspend();//��ֻ��Ҫ��ȷ�������߽ӿڣ������Ƿ������ʵ�����ߣ�,����Ӧ������Ҫ��Ӧ�ò����������
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ��
	cls_show_msg("PC�򿪴���,POS������������%dB���ݵ�PC", sizeof(sendbuf));
	cls_printf("POS--->PC(%dB)\n", sizeof(sendbuf));
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB, sizeof(sendbuf), sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	show_msg1(3, "���!");

	cls_show_msg("PC����[����],pos�����������%ds�ڽ���PC������", MAXWAITTIME);
	cls_printf("POS<---PC(%dB)\n", sizeof(sendbuf));
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB, sizeof(recbuf), recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	show_msg1(3, "���!");
	if((len!=sizeof(sendbuf))||memcmp(sendbuf, recbuf, len))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, sizeof(sendbuf));
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg1(2,"����������ͨ��");
#endif

	//case10:pos��PC����,��U��,���������շ�4K����
	cls_show_msg("�뽫POS��PCͨ��USB������,������PC�˵�accessport����,���������");
	if((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("PC�򿪴���,POS������������%dB���ݵ�PC", sizeof(sendbuf));
	cls_printf("POS--->PC(%dB)\n", sizeof(sendbuf));
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB, sizeof(sendbuf), sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	show_msg1(3, "���!");
	
	cls_show_msg("PC����[����],pos�����������%ds�ڽ���PC������", MAXWAITTIME);
	cls_printf("POS<---PC(%dB)\n", sizeof(sendbuf));
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB, sizeof(recbuf), recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	show_msg1(3, "���!");
	if((len!=sizeof(sendbuf))||memcmp(sendbuf, recbuf, len))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, sizeof(sendbuf));
		GOTOERR;
	}
	
	//case11:�����շ�����8�ֽ�����
	cls_show_msg("PC�������,POS������������%dB���ݵ�PC", SNDCNT);
	cls_printf("POS--->PC(%dB)\n", SNDCNT);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB, SNDCNT, sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	show_msg1(3, "���!");
	
	cls_show_msg("PC����[����],POS�����������%ds�ڽ���PC������", MAXWAITTIME);
	cls_printf("POS<---PC(%dB)\n", SNDCNT);
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=NDK_PortRead(PORT_NUM_USB, SNDCNT, recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
   	}
	if((len!=SNDCNT)||memcmp(sendbuf, recbuf, SNDCNT))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, SNDCNT);
		GOTOERR;
	}

	//case12:����64���������ֲ�����4K������
	cls_show_msg("PC�������,POS������������%dB���ݵ�PC", SNDCNT1);
	cls_printf("POS--->PC(%dB)\n", SNDCNT1);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_USB, SNDCNT1, sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	show_msg1(3, "���!");
	
	cls_show_msg("PC����[����],POS�����������%ds�ڽ���PC������", MAXWAITTIME);
	cls_printf("POS<---PC(%dB)\n", SNDCNT1);
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=NDK_PortRead(PORT_NUM_USB, SNDCNT1, recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	show_msg1(3, "���!");
	if((len!=SNDCNT1)||memcmp(sendbuf, recbuf, SNDCNT1))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, SNDCNT1);
		GOTOERR;
	}
	
	//case13:��λ����
	cls_show_msg("PC:ȷ�����Ϳ������ݲ�����[����]����һ�����ݺ�[ֹͣ],�����������");
	sleep(1);
	if((ret=NDK_PortClrBuf(PORT_NUM_USB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);				
		GOTOERR;
	}
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=NDK_PortRead(PORT_NUM_USB, sizeof(recbuf)-1, recbuf, MAXWAITTIME*1000, &len)) != NDK_ERR_TIMEOUT)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((len!=0))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
		GOTOERR;
	}

	//case14:����д���������Ϲرմ��ڣ���Ӧ�ó����쳣(�ܷɻ�������)
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret = NDK_PortWrite(PORT_NUM_USB, sizeof(sendbuf), sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortClose(PORT_NUM_USB)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case15:���´򿪴��ڽ��������շ�Ӧ�óɹ�	
	cls_show_msg("������PC�˵�accessport����,���������");
	if((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("PC�򿪴���,POS������������%dB���ݵ�PC", sizeof(sendbuf));
	cls_printf("POS--->PC(%dB)\n", sizeof(sendbuf));
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret=NDK_PortWrite(PORT_NUM_USB, sizeof(sendbuf), sendbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	show_msg1(3, "���!");
	
	cls_show_msg("PC����[����],pos�����������%ds�ڽ���PC������", MAXWAITTIME);
	cls_printf("POS<---PC(%dB)\n", sizeof(sendbuf));
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB, sizeof(recbuf), recbuf, MAXWAITTIME*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	show_msg1(3, "���!");
	if((len!=sizeof(sendbuf))||memcmp(sendbuf, recbuf, len))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, sizeof(sendbuf));
		goto ERR;
	}
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);	
		
ERR:
	NDK_PortClose(PORT_NUM_USB);
	return;
}

