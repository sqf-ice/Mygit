/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 
* Author 			: zhengry
* version			: V1.0
* DATE			: 2016/06/02
* directory 		: 
* description		: ����ͷ/USB�����潻��
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2016/06/02  zhengry
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"����ͷ/USB������"
#define MAXTIME		60
#define MAX_SIZE  1024 //BUFSIZE_SERIAL 

/*----------global variables declaration("extern")--------------*/
static int nConfInit = 0;	 /*�Ƿ����ù��˲���:0δ���� 1������*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
void camera_usb_host(void)
{
	/*private & local definition*/
	int i = 0, j=0, succ = 0, recvLen = 0 , ret=0, timeout = SO_TIMEO;
	char recvBuf[BUFSIZE_SERIAL+1] = {0}, sendBuf[BUFSIZE_SERIAL+1]={0};

	/*����ǰ��*/
	if(nConfInit == 0)
	{
		cls_show_msg1(g_keeptime, "����ͷ����δ����");
		return;
	}
	
	while(1)
	{
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		i++;
		sleep(5);//ÿ�ιҶ�֮����������Ҫ�ȴ�5��,������оƬ��ѹ��

		//����ͷ����
		if((ret=NDK_ScanInit())!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			continue;
		}
		NDK_ScanSet(CAMERA_SETTYPE_IMAGE, g_ShowImage);
		NDK_ScanSet(CAMERA_SETTYPE_SATURATION, g_Saturation);
		NDK_ScanSet(CAMERA_SETTYPE_SHARPNESS, g_Sharpness);
		NDK_ScanSet(CAMERA_SETTYPE_GAMMA, g_Contrast);
		if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			if(g_ScanType==1)
				NDK_ScanClose();
			continue;
		}
		if(g_ScanType==1)
			NDK_ScanClose();
	
		//��ʼ��USB����
		for(j=0;j<MAX_SIZE;j++)
			sendBuf[j] = rand()%256;
		cls_show_msg("�뽫��POS�ʹ�POSͨ��USB������,���������");		
		NDK_PortClose(PORT_NUM_USB_HOST);
		if((ret = NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,20")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:��%d�δ�USBʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		cls_show_msg("��ģʽPOS�򿪴���,���������");
		if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	//����ջ���
		{				
			cls_show_msg1(g_keeptime, "line %d:��%d����USB������ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//usb��ģʽ��������
		cls_show_msg("��ģʽPOS�����շ����ݺ�,��pos�������������");
		if((ret=NDK_PortWrite(PORT_NUM_USB_HOST, MAX_SIZE, sendBuf))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, i, ret);
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
	
		//�رմ�USB
		cls_show_msg("�ȴ���POS�˳��շ����ݺ�ر�USB����,���������");

		succ++;

		//�ر���USB
ERR:
		NDK_PortClose(PORT_NUM_USB_HOST);		
		cls_show_msg("�رմ�ģʽPOS��USB����,���������");
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest88(void)
{
	int nKeyin=0;

	cls_show_msg1(2, "%s������...", TESTITEM);
	while(1)
	{
		nKeyin=cls_show_msg("1.�������\n"
							"0.����ͷ����\n");
		switch(nKeyin)
		{
			case '1':
				camera_usb_host();
				break;
		    case '0':
				conf_camera();
				nConfInit=1;
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
