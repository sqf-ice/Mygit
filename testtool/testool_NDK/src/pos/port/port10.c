/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: portģ��
* file name		: port10.c
* Author 			: 
* version			: 
* DATE			:
* directory 		: 
* description		: ����HID���ڹ���
* related document	: SDK.chm
*
************************************************************************/
#include "mode.h"
#include<stdio.h>

#define TESTAPI	"HID����"
#define MAXWAITTIME	10//30
#define SNDCNT			(71)
#define SNDCNT1			(128)
#define SNDTIMES		(40)//K21 ÿ�η���1K ������Ҫ����40��

/****************************************************************
* function name 	 	: void port6(void)
* functional description 	:USB���ڹ���
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	         sull        20180910    created
*****************************************************************/
/*
* ��߲�����Ҫʹ��rs232����(com1)����������
*/
void port10(void)
{	
	int ret = 0;
	char sendbuf[BUFSIZE_SERIAL]={0}, recbuf[BUFSIZE_SERIAL]={0};
	int len = 0,j = 0;
	char str1[ ] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/";
	
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:�ر�HID����
	if((ret=NDK_PortClose(PORT_NUM_HID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:pos��PC����ʱ,δ�򿪾ͽ��з��ͣ�Ӧ�÷���ʧ��
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_HID, sizeof(sendbuf),sendbuf))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:֧���ϴ���д��Сд�����֡�����
	if ((ret=NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(sendbuf,0,sizeof(sendbuf));
	sprintf(sendbuf,"abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_-+={[]}\\ |\'\";:<,>.?/");
	cls_show_msg1(5,"�����һtxt�ĵ���������ƶ������ĵ�,���Թ������벻Ҫ�ƿ����,5���POS�����%dB���ݵ�PC", sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_HID,sizeof(sendbuf),sendbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}	
	if(cls_show_msg("���!txt�ĵ���СΪ%dB,������Ϊ%s?��[ENTER],��[����]",sizeof(sendbuf),sendbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3:Ҫ����豸�󣬽��������ٻ��ѣ�ֻ��Ҫ��ȷ�������߽ӿڣ������Ƿ������ʵ�����ߣ���Ӧ���������ؽ��������շ�
	if ((ret=NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysGoSuspend();//��ֻ��Ҫ��ȷ�������߽ӿڣ������Ƿ������ʵ�����ߣ�,����Ӧ������Ҫ��Ӧ�ò����������
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ��
	cls_show_msg1(5,"���txt���ݣ�������ƶ������ĵ�,���Թ������벻Ҫ�ƿ����,5���POS�����%dB���ݵ�PC", sizeof(sendbuf));
	cls_printf("POS--->PC(%dB)\n", sizeof(sendbuf));
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_HID, sizeof(sendbuf), sendbuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	if(cls_show_msg("���!txt�ĵ���СΪ%dB,�����ݶ�Ϊ8?��[ENTER],��[����]",sizeof(sendbuf))!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case4:HID����ֻ��д���ܶ���readӦ�÷��ز�֧��
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_HID, sizeof(recbuf), recbuf, MAXWAITTIME*1000, &len)) != NDK_ERR_NOT_SUPPORT)	
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_HID);
	cls_show_msg1(2,"����������ͨ��");

	//case5:pos��PC����,��U��,������������4K����
	if((ret=NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(5,"����ĵ�����,������ƶ������ĵ�,���Թ������벻Ҫ�ƿ����,5���POS�����%dB���ݵ�PC", sizeof(sendbuf));
	cls_printf("POS--->PC(%dB)\n", sizeof(sendbuf));
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_HID, sizeof(sendbuf), sendbuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	if(cls_show_msg("���!txt�ĵ���СΪ%dB,�����ݶ�Ϊ8?��[ENTER],��[����]",sizeof(sendbuf))!= ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case6:�������ͷ���8�ֽ�����
	cls_show_msg1(5,"txt�������,������ƶ������ĵ�,���Թ������벻Ҫ�ƿ����,5���POS�����%dB���ݵ�PC", SNDCNT);
	cls_printf("POS--->PC(%dB)\n", SNDCNT);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_HID, SNDCNT, sendbuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	if(cls_show_msg("���!txt�ĵ���СΪ%dB,�����ݶ�Ϊ8?��[ENTER],��[����]",SNDCNT)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	

	//case7:����64���������ֲ�����4K������
	cls_show_msg1(5,"txt�������,������ƶ������ĵ�,���Թ������벻Ҫ�ƿ����,5���POS�����%dB���ݵ�PC", SNDCNT1);
	cls_printf("POS--->PC(%dB)\n", SNDCNT1);
	memset(sendbuf, 0x38, sizeof(sendbuf));
	if((ret=NDK_PortWrite(PORT_NUM_HID, SNDCNT1, sendbuf)) != NDK_OK)	
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	if(cls_show_msg("���!txt�ĵ���СΪ%dB,�����ݶ�Ϊ8?��[ENTER],��[����]",SNDCNT1)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
		
	memset(recbuf, 0, sizeof(recbuf));
	if((ret=NDK_PortRead(PORT_NUM_HID, SNDCNT1, recbuf, MAXWAITTIME*1000, &len)) != NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:��λ����:HID���ڲ�֧��NDK_PortClrBuf
	if((ret=NDK_PortClrBuf(PORT_NUM_HID))!=NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);				
		GOTOERR;
	}

	//case9:����д���������Ϲرմ��ڣ���Ӧ�ó����쳣(�ܷɻ�������)
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j]=rand()%256;
	if((ret = NDK_PortWrite(PORT_NUM_HID, sizeof(sendbuf), sendbuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortClose(PORT_NUM_HID)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case10:���´򿪴��ڽ������ݷ���Ӧ�óɹ�	
	if((ret=NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(5,"txt�������,������ƶ������ĵ�,���Թ������벻Ҫ�ƿ����,5���POS�����%dB���ݵ�PC�ʹ���", sizeof(sendbuf));
	cls_printf("POS--->PC(%dB)\n", sizeof(sendbuf));
	for (j=0; j<sizeof(sendbuf); j++)
		sendbuf[j] = str1[rand()%(sizeof(str1)-1)];
	if((ret=NDK_PortWrite(PORT_NUM_HID, sizeof(sendbuf), sendbuf)) != NDK_OK)	
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	NDK_PortWrite(PORT_NUM_COM1, sizeof(sendbuf), sendbuf);
	if(cls_show_msg( "���!txt�ĵ���СΪ%dB,�������뷢�͵������ϵ�����һ��?��[ENTER],��[����]",sizeof(sendbuf))!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);	
		
ERR:
	NDK_PortClose(PORT_NUM_HID);
	return;
}


