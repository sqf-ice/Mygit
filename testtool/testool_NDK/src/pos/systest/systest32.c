/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest32.c
* Author 			: chensj
* version			: 
* DATE			: 20130624
* directory 	: 
* description	: 	USB�����ۺϲ���
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
#define	MAXWAITTIME 	30	//30
#define	TESTITEM		"USB�����ۺϲ���"
#define DEFAULT_CNT_VLE	2048//3000
#define	DEFAULT_CNT_STR	"2048"//"3000"

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
*					
*****************************************************************/
//PC--->POS
static void usb_pctopos(void)
{
	/*private & local definition*/
	int ret = -1,  datalen = 0, cnt = 0,succconut = 0, i = 0;
	char recvBuf[BUFSIZE_SERIAL+1] = {0}, CntStr[8] = DEFAULT_CNT_STR, sendBuf[BUFSIZE_SERIAL+1] = {0}; 
	uint unLen = 0;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");	
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");	
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg("line %d:��USBʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
	NDK_PortClrBuf(PORT_NUM_USB);//������������Ӱ����һ�»��� 20131114 zhengdc���
	//�����������ݰ������͸���̨
	cls_show_msg("���AccessPort���ߴ򿪴���,���������");
	cls_show_msg1(2,"����������...");
	for(i=0;i<BUFSIZE_SERIAL;i++)
		sendBuf[i] = rand()%256;
	if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
	if(cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢿����Զ�����,ȷ�ϼ�����")!=ENTER)
	{
		NDK_PortClose(PORT_NUM_USB);
		return;	
	}
	//����ѹ������
	i=0;
	while(1)
	{
		cls_printf("USB���ڽ���ѹ����\n�ܹ�:%d��\n�ѽ���:%d��\n",cnt,i);
		if(i++>=cnt)
			break;
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB, BUFSIZE_SERIAL, recvBuf, MAXWAITTIME*1000, &datalen)) != NDK_OK)
		{
			NDK_PortClrBuf(PORT_NUM_USB);//ʧ�������Ϊ����Ӱ����һ�β��ԣ���һ�½��ջ��� linwl 20140115
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, datalen, BUFSIZE_SERIAL)==ESC)
				break;
			else
				continue;
		}
		if(datalen != BUFSIZE_SERIAL)
		{
			NDK_PortClrBuf(PORT_NUM_USB);//ʧ�������Ϊ����Ӱ����һ�β��ԣ���һ�½��ջ��� linwl 20140115
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, datalen, BUFSIZE_SERIAL)==ESC)
				break;
			else
				continue;
		}
		if(MemCmp(sendBuf, recvBuf,  BUFSIZE_SERIAL))
		{
			NDK_PortClrBuf(PORT_NUM_USB);//ʧ�������Ϊ����Ӱ����һ�β��ԣ���һ�½��ջ��� linwl 20140115
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��,", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		succconut++;//ͨѶ�ɹ���������	
	}

	cls_show_msg("�������,��Ͽ�AccessPort���Ӳ�ֹͣ����,�����������");
	cls_show_msg_record(FILENAME,FUNCNAME,"USB���ڽ���ѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	NDK_PortClose(PORT_NUM_USB);
	return;
}

//PC<--->POS
static void usb_comm_pc(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0, succconut = 0,cnt=0,nFirstRun=0;
	char recvBuf[BUFSIZE_SERIAL+1] = {0};
	char CntStr[8] = DEFAULT_CNT_STR,sendBuf[BUFSIZE_SERIAL+1]={0};
	uint unLen=0;
	
	/*process body*/	
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);

	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");	
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg("line %d:��USBʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return ;
	}
	//�����������ݰ������͸���̨
	cls_show_msg("���AccessPort���ߴ�������,���������");
	for(i=0;i<BUFSIZE_SERIAL;i++)
		sendBuf[i] = rand()%256;
	
	//��дѹ������
	i=0;
	while(1)
	{
		cls_printf("USB���ڶ�дѹ����\n�ܹ�:%d��\n�ѽ���:%d��\n",cnt,i);
		if(i++>=cnt)
			break;
		if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
		{				
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}

		if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		if(!nFirstRun)
		{
			nFirstRun++;
			if(cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢿����Զ�����,ȷ�ϼ�����")!=ENTER)
			{
				NDK_PortClose(PORT_NUM_USB);
				return;	
			}
		}
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, BUFSIZE_SERIAL)==ESC)
				break;
			else
				continue;
		}
		if(recvLen != BUFSIZE_SERIAL)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, recvLen, BUFSIZE_SERIAL)==ESC)
				break;
			else
				continue;
		}
		if(MemCmp(sendBuf, recvBuf,  BUFSIZE_SERIAL))
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��(recvlen=%d)", __LINE__, i, recvLen)==ESC)
				break;
			else
				continue;
		}
		succconut++;//ͨѶ�ɹ���������	
	}

	cls_show_msg("�������,��Ͽ�AccessPort���Ӳ�ֹͣ����,�����������");
	cls_show_msg_record(FILENAME,FUNCNAME,"USB���ڶ�дѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	NDK_PortClose(PORT_NUM_USB);
	return;
}

void usb_openclosepress()
{
	int ret = 0, i = 0, cnt = 0, succ = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen = 0;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	if(cls_show_msg("��ȷ��POS��PC��ͨ��USB������,ȷ�ϼ�����...")!=ENTER)
		return;	

	NDK_PortClose(PORT_NUM_USB);
	
	while(1)
	{
		cls_printf("USB���ڿ���ѹ��������\n�ܹ�:%d��,��ִ��:%d��,�ѳɹ�:%d��",cnt,i,succ);
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;	
		if((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ�USB����ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
#if K21_ENABLE
		sleep(3);
#endif
		if((ret=NDK_PortClose(PORT_NUM_USB))!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر�USB����ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"�ܹ�����%d�ο���USBѹ������,�ѳɹ�%d��",i-1,succ);
	NDK_PortClose(PORT_NUM_USB);
	return;
}

void usb_abnormal(void)
{	
	/*process body*/	
	int ret = 0,i = 0,recvLen = 0;
	char sendBuf[BUFSIZE_SERIAL+1]={0},recvBuf[BUFSIZE_SERIAL+1]={0};
	time_t oldtime = 0;

	NDK_PortClose(PORT_NUM_USB);
	//��ʼ�����ͻ���
	for(i=0; i<sizeof(sendBuf)-1; i++)
		sendBuf[i]=rand()%256;
	
	//case1:�ڴ�֮��β�USB��Ӧ�ò�Ӱ������ͨѶ,�������������쳣
	cls_show_msg("�뽫POS��PCͨ��USB������,������PC�˵�accessport����,���������");
	if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
	{
		cls_show_msg("line %d����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
#if !(defined ME50S ||defined ME50NS)	
	cls_show_msg("���Ȱβ�һ��USB��,�ٴ�PC���ߴ���,�����������");
#else
	cls_show_msg("���Ȱβ�һ��USB��,�����������");
	if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
	{
		cls_show_msg("line %d����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
	cls_show_msg("��PC���ߴ���,�����������");
#endif	
	if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:��������ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢷���,���������");
	memset(recvBuf, 0, sizeof(recvBuf));
	if((ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
	{
		cls_show_msg("line %d:��������ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	if(recvLen != BUFSIZE_SERIAL)
	{
		cls_show_msg("line %d:�����շ����Ȳ�һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, BUFSIZE_SERIAL);
		goto END;
	}
	if(MemCmp(sendBuf, recvBuf,  BUFSIZE_SERIAL))
	{
		cls_show_msg("line %d:����У��ʧ��", __LINE__);
		goto END;
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg("1������������ͨ��,���������");
	
	//case2:�����������͹����аβ�USB��
	cls_show_msg("�뽫POS��PCͨ��USB������,������PC�˵�accessport����,���������");
	if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	cls_show_msg("PC�򿪴���,������POS�������3s��γ�USB��");//���Զ�ΰβ�
	oldtime=time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>MAXWAITTIME)
			break;
		ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf);
		switch(ret)
		{
			case -1://�Ͷ˿��ܷ���-1
			case -2:
			case NDK_ERR_USB_LINE_UNCONNECT://����֮��Ӧ���ܹ���ȡ��-100״̬,�ٲ����ߺ����²�������Ӧ���ܹ���������������
			case NDK_ERR_TIMEOUT://���ڰ��߲�����ʱ�����,Ҳ�����ܷ��س�ʱ
			case -12:     //ME50S��ME50NS���ߺ󷵻ش˷���ֵ
				//cls_show_msg("����USB��,���������");
				NDK_PortClose(PORT_NUM_USB);
				cls_show_msg("�ر�PC�˴��ں�����USB��");
				if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
				{
			    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
			    	goto END;
				}
				cls_show_msg("��PC�˴���,���������");
				break;	
			case NDK_OK:
				cls_printf("���ͳɹ�");
				break;
			default:
				cls_show_msg("line %d:δ֪�ķ���ֵ(ret=%d)", __LINE__, ret);
				goto END;
		}
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg("2������������ͨ��,���������");

#if !(defined ME50S ||defined ME50NS)   //ME50S��ME50NS ����USB���Ʋ�ͬ,read�ε��߲��ܼ��ʲ���
	//case3:�����������չ����аβ�USB��
	cls_show_msg("�뽫POS��PCͨ��USB������,������PC�˵�accessport����,���������");
	if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	cls_show_msg("PC�򿪴���,POS������������%dB���ݵ�PC", BUFSIZE_SERIAL);
	cls_printf("POS--->PC(%dB)\n", sizeof(sendBuf)-1);
	if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
		goto END;		
	}
	show_msg1(3, "���!");
	//cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢿����Զ�����,�����������");
	//cls_show_msg("����PC��������3���ʼ�β�USB�߲���,�����������");
	cls_show_msg("�븴�����ݿ�ʼ�Զ����ͺ��USB,�����������");//��usb���ʱ�䵽��ʾ��Ϣ�������ٲ���usb
	oldtime=time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>MAXWAITTIME)
			break;
		memset(recvBuf, 0, sizeof(recvBuf));
		ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, recvBuf, MAXWAITTIME*1000, &recvLen);
		switch(ret)
		{
			/*case :
			case :
			case :
			case :
			case :
				cls_show_msg("line %d:%s����ʧ��(ret=%d)", __LINE__, ret);
				goto END;*/
			case -1://�Ͷ˿��ܷ���-1
			case -2:
			case NDK_ERR_USB_LINE_UNCONNECT://����֮��Ӧ���ܹ���ȡ��-100״̬,�ٲ����ߺ����²�������Ӧ���ܹ���������������
			case NDK_ERR_TIMEOUT://���ڰ��߲�����ʱ�����,Ҳ�����ܷ��س�ʱ
				//cls_show_msg("����USB��,���������");
				NDK_PortClose(PORT_NUM_USB);
				cls_show_msg("ֹͣ����,�رմ��ں�����USB");
				if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
				{
			    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
			    	goto END;
				}
				cls_show_msg("�򿪴���,�Զ�����,���������");
				break;	
			case NDK_OK:
				cls_printf("���ճɹ�");
				break;
			default:
				cls_show_msg("line %d:δ֪�ķ���ֵ(ret=%d)", __LINE__, ret);
				goto END;
		}
	}
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg("3������������ͨ��,���������");
#endif

	//case4:����USB���ڴ�֮�󿪹�PC��������PC,POS��Ӧ�ó����������쳣
	if((ret = NDK_PortOpen(PORT_NUM_USB,"115200,8,N,1"))!=NDK_OK)
	{
		cls_show_msg("line %d����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
#if K21_ENABLE
	sleep(3);
#endif
	cls_show_msg("���pc�ػ��ٿ�����������,�����������");
	cls_show_msg("����ͨ��");
END:
	NDK_PortClose(PORT_NUM_USB);
	return;
}
//PC<--->POS
static void usb_open_comm_close(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, recvLen = 0, succconut = 0,cnt=0,nFirstRun=0;
	char recvBuf[BUFSIZE_SERIAL+1] = {0};
	char CntStr[8] = DEFAULT_CNT_STR,sendBuf[BUFSIZE_SERIAL+1]={0};
	uint unLen=0;
	
	/*process body*/	
	cls_printf("Ĭ�ϲ��Դ���:\n");
	//Ϊ�˼����޼��̵Ͷ˲�ƷME11 ME15ʹ�÷�װ���� 20141031 jiangym modify
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);

	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_USB);
	
	//�����������ݰ������͸���̨
	for(i=0;i<BUFSIZE_SERIAL;i++)
		sendBuf[i] = rand()%256;

	cls_show_msg("�뽫POS��PCͨ��USB������,���������");	
	
	//��дѹ������
	i=0;
	while(1)
	{
		cls_printf("USB���ڶ�дѹ����\n�ܹ�:%d��\n�ѽ���:%d��\n",cnt,i);
		if(i++>=cnt)
			break;
		
		if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB);
			return;
		}
		if(nFirstRun==0)
			cls_show_msg("���AccessPort���ߴ����Ӵ���,���������");
		else
			cls_show_msg("���AccessPort�򿪴��ڲ�����,���������");
		if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
		{				
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		
		if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		if(!nFirstRun)
		{
			nFirstRun++;
			if(cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢿����Զ�����,ȷ�ϼ�����")!=ENTER)
			{
				NDK_PortClose(PORT_NUM_USB);
				return;	
			}
		}
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, BUFSIZE_SERIAL)==ESC)
				break;
			else
				continue;
		}
		if(recvLen != BUFSIZE_SERIAL)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, recvLen, BUFSIZE_SERIAL)==ESC)
				break;
			else
				continue;
		}
		if(MemCmp(sendBuf, recvBuf,  BUFSIZE_SERIAL))
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		succconut++;//ͨѶ�ɹ���������	
		NDK_PortClose(PORT_NUM_USB);
		cls_show_msg("�ر�AccessPort����,���������");
	}

	cls_show_msg("�������,��Ͽ�AccessPort���Ӳ�ֹͣ����,�����������");
	cls_show_msg_record(FILENAME,FUNCNAME,"USB���ڶ�дѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	NDK_PortClose(PORT_NUM_USB);
	return;
}

//POS->PC
static void usb_send_data(void)
{
	int ret = -1, i = 0, cnt=0;
	char sendBuf[1024+1]={0},CntStr[8] = DEFAULT_CNT_STR;
	uint unLen=0;

	/*process body*/	
	cls_printf("Ĭ��POS�������ݵ�PC����:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_USB);
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");	
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
	//�����������ݰ������͸���̨
	cls_show_msg("���AccessPort���ߴ�������,���������");
	memset(sendBuf, 0, sizeof(sendBuf));
	for(i=0;i<sizeof(sendBuf)-1;i++)
	{
		sendBuf[i] = rand()%256;
	}
	
	//д����
	if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
	{				
		cls_show_msg1(g_keeptime,"line %d:�建����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return; 
	}
	for(i=0;i<cnt;i++)
	{
		if((ret=NDK_PortWrite(PORT_NUM_USB, 1024, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i+1, ret);
			NDK_PortClose(PORT_NUM_USB);
			return; 
		}
	}
	if(cls_show_msg("PC�˽��յ��������Ƿ�Ϊ%d?�ǰ�[ENTER],��Ϊ[����]",cnt*1024)!=ENTER)
	{
		cls_show_msg1(g_keeptime, "line %d:�����������ݲ���ʧ��", __LINE__);
		NDK_PortClose(PORT_NUM_USB);
		return; 
	}
	cls_show_msg("�������,��Ͽ�AccessPort����,�����������");
	NDK_PortClose(PORT_NUM_USB);
	return;

}
//USB��ģʽѹ������
void usb_press()
{
	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	while(1)
    {
        keyin = cls_show_msg("1.USB����ѹ��\n"
						"2.POS<->PC 3.PC->POS\n"
						"4.�쳣 5.����\n"
						"6.POS->PC");
		switch(keyin) 
		{
		case '1':
			usb_openclosepress();
			break;
		case '2':
			usb_comm_pc();
			break;
		case '3':
			usb_pctopos();
			break;
		case '4':
			usb_abnormal();
			break;
		case '5':
			usb_open_comm_close();
			break;
		case '6':
			usb_send_data();
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

#if USBHOST_ENABLE
void usb_host_openclosepress()
{
	int ret = 0, i = 0, cnt = 0, succ = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen = 0;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	if(cls_show_msg("��ȷ����POS�ʹ�POS��ͨ��USB������,ȷ�ϼ�����")!=ENTER)
		return;	

	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_show_msg1(2,"��򿪴�ģʽ����");
	while(1)
	{
		cls_printf("��ģʽUSB���ڿ���ѹ��������\n�ܹ�:%d��,��ִ��:%d��,�ѳɹ�:%d��",cnt,i,succ);
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;	
		if((ret=NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ�USB����ʧ��%d", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
#if K21_ENABLE
		sleep(3);
#endif
		if((ret=NDK_PortClose(PORT_NUM_USB_HOST))!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر�USB����ʧ��%d", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"�ܹ�����%d����ģʽ����USBѹ������,�ѳɹ�%d��",i-1,succ);
	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_show_msg1(2,"��رմ�ģʽ����");
	return;
}

void usb_host_comm_usb()
{
	int ret = -1, i = 0, recvLen = 0, succconut = 0,cnt=0,nFirstRun=0;
	char recvBuf[BUFSIZE_USBHOST+1] = {0};
	char CntStr[8] = DEFAULT_CNT_STR,sendBuf[BUFSIZE_USBHOST+1]={0};
	uint unLen=0;
	
	/*process body*/	
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);

	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");	
	cls_printf("�ȴ�Լ3����ģʽPOS�򿪴���");
	if((ret = NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
	{
		cls_show_msg("line %d:��USBʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return ;
	}
	//��������
	for(i=0;i<BUFSIZE_USBHOST;i++)
		sendBuf[i] = rand()%256;
	//��дѹ������
	if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return;	
	}
	i=0;
	while(1)
	{
		if(!nFirstRun)
		{
			nFirstRun++;
			if(cls_show_msg("������ģʽPOS���շ����ݺ�,ȷ�ϼ�����")!=ENTER)
			{
				NDK_PortClose(PORT_NUM_USB_HOST);
				return;	
			}	
		}
		cls_printf("��ģʽUSB���ڶ�дѹ����\n�ܹ�:%d��\n�ѽ���:%d��\n",cnt,i);
		if(i++>=cnt)
			break;
		if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, sendBuf))!=NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB_HOST,BUFSIZE_USBHOST, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, BUFSIZE_USBHOST)==ESC)
				break;
			else
				continue;
		}
		if(recvLen != BUFSIZE_USBHOST)
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, recvLen, BUFSIZE_USBHOST)==ESC)
				break;
			else
				continue;
		}
		if(MemCmp(sendBuf, recvBuf,  BUFSIZE_USBHOST))
		{
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		succconut++;//ͨѶ�ɹ���������	
	}

	cls_show_msg("�������,��رմ�ģʽPOS����USB,�����������");
	cls_show_msg_record(FILENAME,FUNCNAME,"��ģʽUSB���ڶ�дѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	NDK_PortClose(PORT_NUM_USB_HOST);
	return;
}

void usb_host_recv_data()//usb_postohost
{
	int ret = -1,  datalen = 0, cnt = 0,succconut = 0, i = 0, flag = 1;
	char recvBuf[BUFSIZE_USBHOST+1] = {0}, CntStr[8] = DEFAULT_CNT_STR, sendBuf[BUFSIZE_USBHOST+1] = {0}; 
	uint unLen = 0;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//����ǰ�ã�
	NDK_PortClose(PORT_NUM_USB_HOST);
	if(cls_show_msg("���ԵĴ��豸��pos?,��[Enter],��[����]")==ENTER)
		flag =  1;
	else 
		flag = 0;
	if(flag)
	{
		cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");
		cls_printf("�ȴ�Լ3����ģʽPOS�򿪴���");
	}
	else
		cls_show_msg("�뽫ɨ��ǹ(�����������豸)����POS��USB������,���������");
	//��POS�򿪴���
	if((ret = NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
	{
		cls_show_msg("line %d:��USBʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return ;
	}
	NDK_PortClrBuf(PORT_NUM_USB_HOST);
	memset(sendBuf, 0x38, sizeof(sendBuf));
	//��POS ����ѹ������
	if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return;	
	}
	i=0;
	if(flag)
		cls_show_msg("��ģʽPOS�������������ݺ����������");
	while(1)
	{
		if(cls_show_msg1(1,"USB���ڽ���ѹ����\n�ܹ�:%d��\n�ѽ���:%d��\nESC�˳�",cnt,i)==ESC)
			break;
		if(i++>=cnt)
			break;
		if(flag==0)
			cls_printf("���豸�������ݳɹ���ȴ�(ɨ��ǹɨ������(����������ɨ�ɹ�))");
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, recvBuf, 15*1000, &datalen)) != NDK_OK)/*MAXWAITTIME�ĳ�15:readϣ����BUFSIZE_USBHOST��������,ʵ���������豸����û����ô��������,Ҫ�ȴ�����ʱ���صȴ�ʱ��̫�ù��޸�ʱ��Ϊ15��*/
		{
			NDK_PortClrBuf(PORT_NUM_USB_HOST);//ʧ�������Ϊ����Ӱ����һ�β��ԣ���һ�½��ջ��� linwl 20140115
			if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, datalen, BUFSIZE_USBHOST)==ESC)
				break;
			else
				continue;
		}
		if(flag)
		{
			if(datalen != BUFSIZE_USBHOST)
			{
				NDK_PortClrBuf(PORT_NUM_USB);//ʧ�������Ϊ����Ӱ����һ�β��ԣ���һ�½��ջ��� linwl 20140115
				if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, datalen, BUFSIZE_USBHOST)==ESC)
					break;
				else
					continue;
			}
			if(MemCmp(sendBuf, recvBuf,  BUFSIZE_USBHOST))
			{
				NDK_PortClrBuf(PORT_NUM_USB_HOST);//ʧ�������Ϊ����Ӱ����һ�β��ԣ���һ�½��ջ��� linwl 20140115
				if(cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, i)==ESC)
					break;
				else
					continue;
			}
		}
		else  //���ͽ����PC
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "POS�յ�����:%s", recvBuf);
		succconut++;//ͨѶ�ɹ���������	
	}

	cls_show_msg("�������,�رմ�ģʽPOS����,�����������");
	cls_show_msg_record(FILENAME,FUNCNAME,"��ģʽUSB���ڽ���ѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	NDK_PortClose(PORT_NUM_USB_HOST);
	return;
}

void usb_host_abnormal()
{
	int ret = 0, recvLen = 0, i=0;
	char sendBuf[BUFSIZE_USBHOST+1]={0},recvBuf[BUFSIZE_USBHOST+1]={0};
	time_t oldtime = 0;

	NDK_PortClose(PORT_NUM_USB_HOST);
	//��ʼ�����ͻ���
	for(i=0; i<sizeof(sendBuf)-1; i++)
		sendBuf[i]=rand()%256;
	
	//case1:�ڴ�֮��β�USB���ٽ��д��ڶ�д,�������������쳣
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");
	cls_printf("�ȴ�Լ3����ģʽPOS�򿪴���");
	if((ret = NDK_PortOpen(PORT_NUM_USB_HOST,"115200,8,N,1,20"))!=NDK_OK)
	{
		cls_show_msg("line %d����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return;
	}
	cls_show_msg("���Ȱβ�����ģʽUSB��,�����������");
	//Ԥ��дʧ�ܣ�����������
	if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, sendBuf))!=NDK_ERR_WRITE)
	{
		cls_show_msg("line %d:����ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_printf("����POS���´򿪴��ڶ�дӦ����");
	//�����´���ģʽ���ڽ���ͨѶ������
	if((ret = NDK_PortOpen(PORT_NUM_USB_HOST,"115200,8,N,1,20"))!=NDK_OK)
	{
		cls_show_msg("line %d����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return;
	}
	if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	cls_show_msg("��ģʽPOS�����շ����ݺ�,���������");
	if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, sendBuf))!=NDK_OK)
	{
		cls_show_msg("line %d:��������ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	memset(recvBuf, 0, sizeof(recvBuf));
	if((ret=NDK_PortRead(PORT_NUM_USB_HOST,BUFSIZE_USBHOST, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
	{
		cls_show_msg("line %d:��������ʧ��(%d)",__LINE__,ret);
		goto END;
	}
	if(recvLen != BUFSIZE_USBHOST)
	{
		cls_show_msg("line %d:�����շ����Ȳ�һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, BUFSIZE_USBHOST);
		goto END;
	}	
	if(MemCmp(sendBuf, recvBuf,  BUFSIZE_USBHOST))
	{
		cls_show_msg("line %d:����У��ʧ��", __LINE__);
		goto END;
	}
	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_show_msg("1������������ͨ��,�رմ�ģʽ���ں�,���������");
	
	//case2:�����������͹����аβ�USB��
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");
	cls_printf("�ȴ�Լ3����ģʽ�򿪴���");
	if((ret = NDK_PortOpen(PORT_NUM_USB_HOST,"115200,8,N,1,20"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	cls_show_msg("�����������POS�������3s��γ�USB��");
	oldtime=time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			if(cls_show_msg("�β���������[Enter],��ʱ�俨סδ�˳���[����]")==ENTER)
			{
				break;
			}
			else
			{
				cls_show_msg("line %d:����ʧ��,��ʱ�˳�,���������", __LINE__);
				goto END;
			}
		}
		ret=NDK_PortWrite(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, sendBuf);
		switch(ret)
		{
			case -1:
			case -2:
			case NDK_ERR_WRITE://���豸����֮���ȡ����-100״ֻ̬�ܷ���дʧ��,�ٲ����ߺ����²�������Ӧ���ܹ���������������
			case NDK_ERR_TIMEOUT://���ڰ��߲�����ʱ�����,Ҳ�����ܷ��س�ʱ
				NDK_PortClose(PORT_NUM_USB_HOST);
				cls_show_msg("�رմ�POS���ں�,����USB�ߺ󰴼�����");
				cls_printf("�򿪴�POS����");
				if((ret = NDK_PortOpen(PORT_NUM_USB_HOST,"115200,8,N,1,20"))!=NDK_OK)
				{
			    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
			    	goto END;
				}
				break;	
			case NDK_OK:
				cls_printf("���ͳɹ�");
				break;
			default:
				cls_show_msg("line %d:δ֪�ķ���ֵ(ret=%d)", __LINE__, ret);
				goto END;
		}
	}
	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_show_msg("2������������ͨ��,�رմ�ģʽ���ں�,���������");
	
	//case3:�����������չ����аβ�USB��
	cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");
	cls_printf("��ģʽ�򿪴���");
	if((ret = NDK_PortOpen(PORT_NUM_USB_HOST,"115200,8,N,1,20"))!=NDK_OK)
	{
    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
    	goto END;
	}
	cls_show_msg("��POS������������,�����������3s��POS��USB��");
	//��usb���ʱ�䵽��ʾ��Ϣ�������ٲ���usb
	oldtime=time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			if(cls_show_msg("�β���������[Enter],��ʱ�俨סδ�˳���[����]")==ENTER)
			{
				break;
			}
			else
			{
				cls_show_msg("line %d:����ʧ��,��ʱ�˳�,���������", __LINE__);
				goto END;
			}
		}
		memset(recvBuf, 0, sizeof(recvBuf));
		ret=NDK_PortRead(PORT_NUM_USB_HOST,BUFSIZE_USBHOST, recvBuf, 2*1000, &recvLen);
		switch(ret)
		{
			case -1:
			case -2:
			case NDK_ERR_READ://���豸����֮���ȡ����-100״ֻ̬�ܷ��ض�ʧ��,�ٲ����ߺ����²�������Ӧ���ܹ���������������
			case NDK_ERR_TIMEOUT://���ڰ��߲�����ʱ�����,Ҳ�����ܷ��س�ʱ
				NDK_PortClose(PORT_NUM_USB_HOST);
				cls_show_msg("ESCֹͣ����,�رմ��ں�����USB");
				cls_printf("�򿪴�POS����");
				if((ret = NDK_PortOpen(PORT_NUM_USB_HOST,"115200,8,N,1,20"))!=NDK_OK)
				{
			    	cls_show_msg("line %d:����ʧ��(ret:%d)", __LINE__, ret);
			    	goto END;
				}
				cls_show_msg("��ģʽPOS������������,���������");
				break;	
			case NDK_OK:
				cls_printf("���ճɹ�");
				break;
			default:
				cls_show_msg("line %d:δ֪�ķ���ֵ(ret=%d)", __LINE__, ret);
				goto END;
		}
	}
	NDK_PortClose(PORT_NUM_USB_HOST);
	cls_show_msg("3������������ͨ��,�رմ�ģʽ���ں�,���������");
	
END:
	NDK_PortClose(PORT_NUM_USB_HOST);
	return;
}

//USB��ģʽѹ������
void usb_host_press()
{
	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	while(1)
    {
        keyin = cls_show_msg("1.USB����ѹ��\n"
						"2.��POS<->��POS\n"
						"3.���豸->��POS\n"
						"4.�쳣 ");
		switch(keyin) 
		{
		case '1':
			usb_host_openclosepress();
			break;
		case '2':
			usb_host_comm_usb();
			break;
		case '3':
			usb_host_recv_data();
			break;
		case '4':
			usb_host_abnormal();
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

void usb_host_U()
{
	int ret = -1, recvLen = 0, i=0;
	char recvBuf[BUFSIZE_USBHOST+1] = {0};
	char sendBuf[BUFSIZE_USBHOST+1] = {0};

	while(1)
	{
		if(ESC==cls_show_msg1(2, "���ڽ��е�%d����ģʽ��U�̵��л�����,��ESC�˳�", i+1))
			break;
		if( i++ > 3 )
			break;
		//��������
		memset(sendBuf, 0x38, sizeof(sendBuf));
		//usb ��ģʽ�� ��д �ر� ����
		NDK_PortClose(PORT_NUM_USB_HOST);
		cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");	
		cls_printf("��ģʽPOS�򿪴���");
		if((ret = NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return ;
		}
		if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
		{				
			cls_show_msg1(g_keeptime,"line %d:�建����ʧ��(%d)",__LINE__,ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		cls_show_msg("��ģʽPOS�����շ����ݺ�,���������");
		if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:дUSBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB_HOST,BUFSIZE_USBHOST, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		if(recvLen != BUFSIZE_USBHOST)
		{
			cls_show_msg1(g_keeptime,"line %d:�����շ����Ȳ�һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, BUFSIZE_USBHOST);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		if(MemCmp(sendBuf, recvBuf, BUFSIZE_USBHOST))
		{
			cls_show_msg1(g_keeptime,"line %d:У������ʧ��", __LINE__);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		NDK_PortClose(PORT_NUM_USB_HOST);
		
		//U�̵Ĳ���
		cls_show_msg("��ģʽ������ɣ���ε���POS��USB�߲���U�̣����رմ�ģʽPOS���ں����������");
		//NDK_DiskClose(UDISK);
		if((ret=systest_SDCard(UDISK))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:U�̲���ʧ��(%d)", __LINE__, ret);
			continue;
		}		
	}
}

void host_usb_slave()
{
	int ret = -1, recvLen = 0,  i=0;
	char recvBuf[BUFSIZE_USBHOST+1] = {0};
	char sendBuf[BUFSIZE_USBHOST+1] = {0};
	
	while(1)
	{
		if(ESC==cls_show_msg1(2, "���ڽ��е�%d������ģʽ�л�����,��ESC�˳�", i+1))
			break;
		if( i++ > 3 )
			break;
		//��������
		memset(sendBuf, 0x38, sizeof(sendBuf));
		//usb��ģʽ�Ĳ���	
		NDK_PortClose(PORT_NUM_USB_HOST);
		cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");
		cls_printf("�ȴ�Լ3����ģʽPOS�򿪴���");
		if((ret = NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1,20")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return ;
		}
		if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
		{				
			cls_show_msg1(g_keeptime,"line %d:�建����ʧ��(%d)",__LINE__,ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		cls_show_msg("��ģʽPOS�����շ����ݺ�,���������");
		if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:дUSBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB_HOST, BUFSIZE_USBHOST, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		if(recvLen != BUFSIZE_USBHOST)
		{
			cls_show_msg1(g_keeptime,"line %d:�����շ����Ȳ�һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, BUFSIZE_USBHOST);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		if(MemCmp(sendBuf, recvBuf, BUFSIZE_USBHOST))
		{
			cls_show_msg1(g_keeptime,"line %d:У������ʧ��", __LINE__);
			NDK_PortClose(PORT_NUM_USB_HOST);
			return;
		}
		NDK_PortClose(PORT_NUM_USB_HOST);
		cls_show_msg("��ģʽ������ɣ���رմ�ģʽPOS���ں����������");
	
		//usb��ģʽ�Ĳ���
		cls_show_msg("�뽫����ģʽ��POS��PCͨ��USB������,���������");	
		NDK_PortClose(PORT_NUM_USB);
		if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB);
			return ;
		}
		cls_show_msg("��PC��AccessPort����,���򿪴���,���������");
		memset(sendBuf, 0x38, sizeof(sendBuf));
		if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_USBHOST, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:дUSBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB);
			return;
		}
		cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢷���,���������");	
		memset(recvBuf, 0, sizeof(recvBuf));
		if((ret=NDK_PortRead(PORT_NUM_USB, BUFSIZE_USBHOST, recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_USB);
			return;
		}
		if(recvLen != BUFSIZE_USBHOST)
		{
			cls_show_msg1(g_keeptime,"line %d:�����շ����Ȳ�һ��(ʵ��:%d,Ԥ��:%d)", __LINE__, recvLen, BUFSIZE_USBHOST);
			NDK_PortClose(PORT_NUM_USB);
			return;
		}
		if(MemCmp(sendBuf, recvBuf, BUFSIZE_USBHOST))
		{
			cls_show_msg1(g_keeptime,"line %d:У������ʧ��", __LINE__);
			NDK_PortClose(PORT_NUM_USB);
			return;
		}
		NDK_PortClose(PORT_NUM_USB);
		cls_show_msg("��ģʽ������ɣ���ر�PC���ߴ��ں����������");	
	}
}

void usb_change_press()
{
	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	while(1)
    {
        keyin = cls_show_msg("1.U�̺�USB��\n"
						"2.USB����USB��\n");
		switch(keyin) 
		{
		case '1':
			usb_host_U();
			break;
		case '2':
			host_usb_slave();
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
#else
void usb_host_openclosepress(){return;}
void usb_host_comm_usb(){return;}
void usb_host_recv_data(){return;}
void usb_host_abnormal(){return;}
void usb_host_press(){cls_show_msg1(5,"���豸��֧��USB��ģʽ,���˹��ж��Ƿ�֧��");return;}
void usb_host_U(){return;}
void host_usb_slave(){return;}
void usb_change_press(){cls_show_msg1(5,"���豸��֧��USB��ģʽ,���˹��ж��Ƿ�֧��");return;}
#endif

void usb_client()
{
	/*private & local definition*/
	int keyin = 0, ret = 0;
	char sendbuf[BUFSIZE_USBHOST]={0}, recbuf[BUFSIZE_USBHOST]={0} ;
	int len = 0;

	/*process body*/
	while(1)
    {
        keyin = cls_show_msg("1.��USB����\n"
						"2.�շ�����\n"
						"3.����������\n"
						"4.�ر�USB����");
		switch(keyin) 
		{
		case '1':
			if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(ret:%d)", __LINE__, TESTITEM, ret);
				NDK_PortClose(PORT_NUM_USB);
				return;
			}
			cls_show_msg1(1,"��USB���ڳɹ�");
			break;
		case '2':	
			memset(recbuf, 0, sizeof(recbuf));
			if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)	/*����ջ���*/
			{				
				cls_show_msg1(g_keeptime,"line %d:�建����ʧ��(%d)",__LINE__,ret);
				NDK_PortClose(PORT_NUM_USB);
				return;
			}
			cls_printf("�����շ���.");
			while(1)
			{			
				//������
				ret = NDK_PortRead(PORT_NUM_USB,sizeof(recbuf),recbuf,5000,&len);
				if(ret==NDK_ERR_TIMEOUT || cls_show_msg1(1,"�����շ���.ESC�˳�")==ESC)
					break;
				else if(ret==NDK_OK)
				{
					//������
					if((ret=NDK_PortWrite(PORT_NUM_USB, len, recbuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(ret:%d)", __LINE__, TESTITEM, ret);
						NDK_PortClose(PORT_NUM_USB);
						return; 	
					}
				}
				else
				{
					if(ret!=-100)
						cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(ret:%d)", __LINE__, TESTITEM, ret);
					NDK_PortClose(PORT_NUM_USB);
					return;
				}						
			}
			break;
		case '3':	
			memset(sendbuf, 0x38, sizeof(sendbuf));
			//����������
			while(1)
			{
				if(cls_show_msg1(1,"��������������,ESC�˳�")==ESC)
					break;
				ret = NDK_PortWrite(PORT_NUM_USB, sizeof(sendbuf), sendbuf);/*�����ڷ��Ͳ�������*/
				if(ret == NDK_ERR_USB_LINE_UNCONNECT)
				{
					cls_show_msg("USB�߱�����,���������");
					break;
				}
				else if(ret== NDK_OK)	
					continue;
				else
				{
					cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(ret:%d)", __LINE__, TESTITEM, ret);
					NDK_PortClose(PORT_NUM_USB);
					return;		
				}
			}
			break;
		case '4':
			if((ret=NDK_PortClose(PORT_NUM_USB))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
				return;
			}
			cls_show_msg1(1,"�ر�USB���ڳɹ�");
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
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		chensj		   20130624	 		created
					jiangym 		 20140220			added
*****************************************************************/
void systest32(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	//EM_PORT_NUM usbport = PORT_NUM_USB;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);	
	while(1)
    {
        keyin = cls_show_msg("1.USB��ģʽ����\n"
							"2.USB��ģʽ����\n"
							"3.USB�л�����\n"
							"4.�ͻ��˲���");
		switch(keyin) 
		{
		case '1':
			usb_press();
			break;
		case '2':
			usb_host_press();
			break;
		case '3':
			usb_change_press();
			break;
		case '4':
			usb_client();
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


