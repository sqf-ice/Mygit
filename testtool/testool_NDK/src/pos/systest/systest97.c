/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest97.c
* Author 			: 
* version			: 
* DATE			: 
* directory 	: 
* description	: 	HID�����ۺϲ���
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
#define	TESTITEM		"HID���ڲ���"
#define DEFAULT_CNT_VLE	2048//3000
#define	DEFAULT_CNT_STR	"2048"//"3000"
#define DEFAULT_TEST_VLE  10
#define DEFAULT_TEST_STR  "10"

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
*					          sull              20180910          created
*****************************************************************/
static void hid_openclosepress()
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
	
	//ע�⣺���Թ����У��൱�ڼ��̴���һֱ�β��״̬�����Դ���ʱ�����
	if(cls_show_msg("��ȷ��POS��PC��ͨ��USB������,ȷ�ϼ�����...")!=ENTER)
		return; 

	//����ǰ��:�ر�HID����
	NDK_PortClose(PORT_NUM_HID);

	while(1)
	{
		cls_printf("HID���ڿ���ѹ��������\n�ܹ�:%d��,��ִ��:%d��,�ѳɹ�:%d��",cnt,i,succ);
		if(i++>=cnt)//�ﵽ���Դ������˳�
			break;	
		if((ret=NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
		{
			if(cls_show_msg1(g_keeptime, "line %d:��%d�δ�HID����ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		sleep(3);
		if((ret=NDK_PortClose(PORT_NUM_HID))!=NDK_OK)
		{
			if(cls_show_msg1(g_keeptime, "line %d:��%d�ιر�HID����ʧ��", __LINE__, i)==ESC)
				break;
			else
				continue;
		}
		succ++;
	}
	
	cls_show_msg1(g_keeptime,"�ܹ�����%d�ο���HIDѹ������,�ѳɹ�%d��",i-1,succ);
	NDK_PortClose(PORT_NUM_HID);
	return;

}

static void hid_open_comn_close()
{
	/*private & local definition*/
	int ret = -1, i = 0, succconut = 0,cnt=0;
	char CntStr[8] = DEFAULT_CNT_STR,sendBuf[BUFSIZE_SERIAL+1]={0};
	uint unLen=0;
	char str1[ ] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/";
	
	/*process body*/	
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);

	//����ǰ�ã��ر�HID����
	NDK_PortClose(PORT_NUM_HID);
	
	//�����������ݰ������͸���̨
	for(i=0;i<BUFSIZE_SERIAL;i++)
		sendBuf[i] = str1[rand()%(sizeof(str1)-1)];

	cls_show_msg1(5,"�����һtxt�ĵ�,��������Ƶ���λ��,���Թ������벻Ҫ�������,5s��ʼ��������");	
	
	//��дѹ������
	i=0;
	while(1)
	{
		cls_printf("HID����дѹ����\n�ܹ�:%d��\n�ѽ���:%d��\n",cnt,i);
		if(i++>=cnt)
			break;
		
		if((ret = NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��HIDʧ��(%d)", __LINE__, ret);
			NDK_PortClose(PORT_NUM_HID);
			return;
		}
		if((ret=NDK_PortWrite(PORT_NUM_HID, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
		{
			if(cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret)==ESC)
				break;
			else
				continue;
		}
		succconut++;//ͨѶ�ɹ���������	
		NDK_PortClose(PORT_NUM_HID);
	}
	cls_show_msg1(g_keeptime,"HID����дѹ���������,ִ�д���:%d��,�ɹ�%d��", i-1, succconut);
	NDK_PortClose(PORT_NUM_HID);
	return;

}

static void hid_send_data()
{
	int ret = -1, i = 0, cnt=0, succ = 0;
	char sendBuf[1024+1]={0},CntStr[8] = DEFAULT_CNT_STR;
	uint unLen=0;
	char str1[ ] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/";

	/*process body*/	
	cls_printf("Ĭ��POS�������ݵ�PC����:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//����ǰ��:�ر�HID����
	NDK_PortClose(PORT_NUM_HID);
	
	cls_show_msg1(5,"�����һtxt�ĵ���������Ƶ���λ�ã����Թ������벻Ҫ�������,5s��ʼ��������");	
	if((ret = NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��HIDʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_HID);
		return;
	}
	//�����������ݰ������͸���̨
	memset(sendBuf, 0, sizeof(sendBuf));
	for(i=0;i<sizeof(sendBuf)-1;i++)
	{
		sendBuf[i] = str1[rand()%(sizeof(str1)-1)];
	}
	//д����
	NDK_PortWrite(PORT_NUM_COM1, 1024, sendBuf);
	for(i=0;i<cnt;i++)
	{
		if((ret=NDK_PortWrite(PORT_NUM_HID, 1024, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i+1, ret);
			continue; 
		}
		succ++;
	}
	if(cls_show_msg("PC�˽��յ��������Ƿ�Ϊ%dB?�ǰ�[ENTER],��Ϊ[����]",cnt*1024)!=ENTER)
	{
		cls_show_msg1(g_keeptime, "line %d:�����������ݲ���ʧ��", __LINE__);
		NDK_PortClose(PORT_NUM_HID);
		return; 
	}
	cls_show_msg1(2,"�������");
	NDK_PortClose(PORT_NUM_HID);
	return;

}

static void hid_usb_test()
{
	char CntStr[8] = DEFAULT_TEST_STR;
	char sendBuf[BUFSIZE_SERIAL+1]={0}, recvBuf[BUFSIZE_SERIAL+1]={0};
	uint unLen = 0;
	int ret = 0, cnt = 0, i = 0, j = 0, recvLen = 0;
	int succ1 = 0, succ2 = 0;
	char str1[ ] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/";

	/*process body*/	
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_TEST_VLE;
	else
		cnt = atoi(CntStr);

	while(i<cnt*2)
	{
		i++;
		memset(sendBuf, 0, sizeof(sendBuf));
		for(j=0;j<sizeof(sendBuf)-1;j++)
		{
			sendBuf[j] = str1[rand()%(sizeof(str1)-1)];
		}
		if(i%2)
		{
			cls_printf("��%d��HID����...", i/2+1);
			if((ret = NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
			{
				cls_show_msg1(g_keeptime,"line %d:��%d�δ�HIDʧ��(%d)", __LINE__, i,ret);
				NDK_PortClose(PORT_NUM_HID);
				continue;
			}
			cls_show_msg1(5,"�����һtxt�ĵ���������Ƶ���λ�ã����Թ������벻Ҫ�������,5s�󽫷���%dB����",sizeof(sendBuf));
			if((ret=NDK_PortWrite(PORT_NUM_HID, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
				NDK_PortClose(PORT_NUM_HID);
				continue;
			}
			//Ϊ����֤HID���͵������Ƿ���ȷ������COM1�ڷ�����ͬ�����Խ��бȽ�
			NDK_PortWrite(PORT_NUM_COM1,BUFSIZE_SERIAL,sendBuf);
			if((ret = NDK_PortClose(PORT_NUM_HID)) != NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�ιر�HID����ʧ��(%d)", __LINE__, i, ret);
				NDK_PortClose(PORT_NUM_HID);
				continue;
			}
			succ1++;
		}
		else
		{
			cls_printf("��%d��USB����...", i/2);
			if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
			{
				cls_show_msg1(g_keeptime,"line %d:��%d�δ�USB����ʧ��(%d)", __LINE__, i,ret);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			cls_show_msg("���accessport���ڣ������������");
			if((ret = NDK_PortClrBuf(PORT_NUM_USB)) != NDK_OK)
			{				
				cls_show_msg1(g_keeptime,"line %d:��%d���建����ʧ��(%d)", __LINE__, i,ret);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, sendBuf))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			cls_show_msg("�뽫AccessPort���յ������ݸ��Ƶ����Ϳ򲢷��ͣ������������");
			memset(recvBuf, 0, sizeof(recvBuf));
			if((ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, recvBuf, 10*1000, &recvLen)) != NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, recvLen, BUFSIZE_SERIAL);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			if(recvLen != BUFSIZE_SERIAL)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, recvLen, BUFSIZE_SERIAL);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			if(MemCmp(sendBuf, recvBuf,  BUFSIZE_SERIAL))
			{
				cls_show_msg1(g_keeptime, "line %d:��%d������У��ʧ��(recvlen=%d)", __LINE__, i, recvLen);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			if((ret = NDK_PortClose(PORT_NUM_USB)) != NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d�ιر�USB����ʧ��(%d)", __LINE__, i, ret);
				NDK_PortClose(PORT_NUM_USB);
				continue;
			}
			cls_show_msg("��ر�accessport���ڣ������������");
			succ2++;
		}
	}
	cls_show_msg("HID��ͨѶ����%d�Σ��ɹ�%d��", cnt, succ1);
	cls_show_msg("USB��ͨѶ����%d�Σ��ɹ�%d��", cnt, succ2);
	return;		
}

static void com_hid_test()
{
	int ret = 0, j = 0, cnt = 0, i =0, succ = 0;
	char buf[BUFSIZE_SERIAL+1]={0}, rbuf[BUFSIZE_SERIAL+1]={0};
	int len = 0;
	char CntStr[8] = DEFAULT_TEST_STR;
	uint unLen = 0;
	char str1[ ] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/";

	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_TEST_VLE;
	else
		cnt = atoi(CntStr);

	//������������
	memset(buf, 0, sizeof(buf));
	for(j=0;j<sizeof(buf)-1;j++)
	{
		buf[j] = str1[rand()%(sizeof(str1)-1)];
	}

	//�򿪴���
	if((ret = NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(1,"line %d:��HIDʧ��(%d)", __LINE__, ret);
		return;
	}
	if((ret=NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1"))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_HID);
		return;
	}

	while(i++ <cnt)
	{
		//�建����
		if((ret = NDK_PortClrBuf(PORT_NUM_COM1)) != NDK_OK)
		{				
			cls_show_msg1(1,"line %d:��%d���建����ʧ��(%d)", __LINE__, i,ret);
			continue;
		}
		//COM1д����
		if((ret=NDK_PortWrite(PORT_NUM_COM1, BUFSIZE_SERIAL, buf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
			continue;
		}
		cls_show_msg("�򿪴��ڹ���,�����͵����ݸ��Ƶ����ڹ��߷��Ϳ�,������,�����������");
		//COM1��������
		memset(rbuf, 0, sizeof(rbuf));
		if((ret=NDK_PortRead(PORT_NUM_COM1,BUFSIZE_SERIAL, rbuf, 10*1000, &len)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, len, BUFSIZE_SERIAL);
			continue;
		}
		//���ݱȽ�
		if(len != BUFSIZE_SERIAL)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, len, BUFSIZE_SERIAL);
			continue;
		}
		if(MemCmp(buf, rbuf,  BUFSIZE_SERIAL))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d������У��ʧ��(recvlen=%d)", __LINE__, i, len);
			continue;
		}
		//дHID
		cls_show_msg1(5, "���txt�ĵ�,��������ƶ�����λ��,���Թ������벻Ҫ�������,5s�󽫷�������");
		if((ret=NDK_PortWrite(PORT_NUM_HID, BUFSIZE_SERIAL, buf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		succ++;
	}
	//�ر�HID����
	if((ret=NDK_PortClose(PORT_NUM_HID))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�ر�HID����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_HID);
		return;
	}

	cls_show_msg1(g_keeptime,"ͨѶ�ܴ���%d�Σ��ɹ�%d��", cnt, succ);
	return;
}

static void com_usb_test()
{
	int ret = 0, j = 0, cnt = 0, i =0, succ = 0;
	char buf1[BUFSIZE_SERIAL+1]={0}, buf2[BUFSIZE_SERIAL+1]={0},rbuf[BUFSIZE_SERIAL+1]={0};
	int len = 0;
	char CntStr[8] = DEFAULT_TEST_STR;
	uint unLen = 0;
	char str1[ ] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/";

	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		cnt = DEFAULT_TEST_VLE;
	else
		cnt = atoi(CntStr);

	memset(buf1, 0, sizeof(buf1));
	for(j=0;j<sizeof(buf1)-1;j++)
	{
		buf1[j] = str1[rand()%(sizeof(str1)-1)];
	}
	memset(buf2, 0, sizeof(buf2));
	for(j=0;j<sizeof(buf2)-1;j++)
	{
		buf2[j] = str1[rand()%(sizeof(str1)-1)];
	}

	//�򿪴���
	if((ret = NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��HIDʧ��(%d)", __LINE__, ret);
		return;
	}
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��HIDʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}
	cls_show_msg("���accessport�ʹ��ڹ��߲�����,���������ʼ����");

	while(i++ <cnt)
	{	
		cls_show_msg("�����accessport�ʹ��ڹ��߷��Ϳ�����,���������������");
		//д����
		if((ret=NDK_PortWrite(PORT_NUM_COM1, BUFSIZE_SERIAL, buf1))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
			continue;
		}
		if((ret=NDK_PortWrite(PORT_NUM_USB, BUFSIZE_SERIAL, buf2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
			continue;
		}
		//������
		cls_show_msg("�����ڽ��յ������ݸ��Ƶ����ڹ��߲����ͣ������������");
		memset(rbuf, 0, sizeof(rbuf));
		if((ret=NDK_PortRead(PORT_NUM_COM1,BUFSIZE_SERIAL, rbuf, 10*1000, &len)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, len, BUFSIZE_SERIAL);
			continue;
		}
		if(len != BUFSIZE_SERIAL)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, len, BUFSIZE_SERIAL);
			continue;
		}
		if(MemCmp(buf1, rbuf,  BUFSIZE_SERIAL))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d������У��ʧ��(recvlen=%d)", __LINE__, i, len);
			continue;
		}
		cls_show_msg("��accessport���յ������ݸ��Ƶ�accessport���Ϳ򲢷��ͣ������������");
		memset(rbuf, 0, sizeof(rbuf));
		if((ret=NDK_PortRead(PORT_NUM_USB,BUFSIZE_SERIAL, rbuf, 10*1000, &len)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, i, ret, len, BUFSIZE_SERIAL);
			continue;
		}
		if(len != BUFSIZE_SERIAL)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�������շ����Ȳ�һ��(ʵ��%d, Ԥ��%d)", __LINE__, i, len, BUFSIZE_SERIAL);
			continue;
		}
		if(MemCmp(buf2, rbuf,  BUFSIZE_SERIAL))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d������У��ʧ��(recvlen=%d)", __LINE__, i, len);
			continue;
		}
		succ++;
	}
	if((ret = NDK_PortClose(PORT_NUM_USB)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:�ر�USB����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB);
		return;
	}

	cls_show_msg1(g_keeptime,"�������,��ͨѶ����%d��,�ɹ�%d��", cnt, succ);
	return;
}

void systest97(void)
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
		keyin = cls_show_msg("1.����ѹ�� 2.����ѹ��\n"
						     "3.POS->PC   4.HID/USB\n"  
						     "5.COM1/USB  6.COM1/HID\n");
		switch(keyin) 
		{
			case '1':
				hid_openclosepress();
				break;
			case '2':
				hid_open_comn_close();
				break;
			case '3':
				hid_send_data();
				break;
			case '4':
				hid_usb_test();
				break;
			case '5':
				com_usb_test();
				break;
			case '6':
				com_hid_test();
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



