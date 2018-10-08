/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: port9.c
* Author 			: 
* version			: 
* DATE				:
* directory 	: 
* description	: ����HID����
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include<stdio.h>

/*---------------constants/macro definition---------------------*/
#define		TESTAPI		"��������ɨ��"
#define		MAXTIME 30//���յȴ�ʱ��

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
* history 		 		: author			date			remarks
*			  			    sull              20180910		created
*****************************************************************/
/*
* ��߲�����Ҫʹ��rs232����(com1)����������
*/
void port9(void)
{
	//����Ϊ�ֲ�����
	int ret = 0, cnt = 20, bak = 0, plen = 0, succ = 0;
	char wbuf[500] = {0}, str[31] = {0};
	uint  a = 0, a1 = 0;
	float ScanTime0 = 0, ScanTime = 0;
	char pvalue[500] = {0};
	time_t oldtime = 0;
	
	//����Ϊ������
	cls_printf("%s������...", TESTAPI);
	if(cls_show_msg1(MAXTIME, "��ȷ�ϼ�����HID����ɨ�����,����������")==ENTER)
	{

		//����ǰ��:�򿪴���		
		if((ret = NDK_PortOpen(PORT_NUM_HID, "115200,8,N,1")) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case1:��������20��ɨ�룬����HID�������͵�txt��
		cls_show_msg1(5,"���һtxt����������Ƶ���λ�ã�5s��ʼɨ�룬ɨ���������͵�txt");
		bak = cnt;
		while(cnt--)
		{		
			plen=0;
			ScanTime0 = 0;
			memset(pvalue,0,sizeof(pvalue));
			NDK_SysReadWatch(&a1);
			if((ret=NDK_ScanStart()) != NDK_OK )
			{
				NDK_ScanStop();
				cls_show_msg1(3, "line %d:��%d������ͷ����ɨ��ʧ��(%d)", __LINE__, bak-cnt, ret);
				continue;
			}   
			oldtime = time(NULL);
			while(1)
			{	
				if((ret=NDK_ScanState(pvalue, &plen)) == NDK_OK)
				{
					NDK_SysReadWatch(&a);
					ScanTime0 = (a - a1)/(1000*1.0);
					ScanTime += ScanTime0;
					succ++;
					memset(str,0,sizeof(str));
					ftos(ScanTime0,str);
					memset(wbuf, 0, sizeof(wbuf));
					sprintf(wbuf, "%d:data:%s;   len:%d;    time:%ss\r\n", bak-cnt, pvalue, plen, str);
					NDK_PortWrite(PORT_NUM_HID, sizeof(wbuf), wbuf);
					break;
				}
				NDK_SysMsDelay(10);
				if((time(NULL) - oldtime)>MAXTIME)
				{
					NDK_ScanStop();
					cls_show_msg1(3,"line %d:ɨ�賬ʱ", __LINE__);
					break;
				}
			}
			if((ret = NDK_ScanStop())!=NDK_OK)
			{
				cls_show_msg1(3,"line %d:�ر�����ͷʧ��(%d)", __LINE__, ret);
				continue;
			}
		}
		send_result("ͨѶ�ܴ���%d,�ɹ�����%d\n", bak, succ);
		memset(str,0,sizeof(str));
		ftos(ScanTime/succ,str);
		send_result("ɨ������:%ss/��\n", str);
		NDK_PortClose(PORT_NUM_HID);
	}

	if(cls_show_msg1(MAXTIME, "��ȷ�ϼ�����USB����ɨ�����,����������")==ENTER)
	{

		//����ǰ��:�򿪴���	
		NDK_PortClose(PORT_NUM_USB);
		if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cnt = 20;
		succ = 0;
		ScanTime = 0;

		//case2:��������20��ɨ�룬����USB �������͵�������
		cls_show_msg("���accessport������,���������ʼɨ�裬ɨ���������͵�accessport");
		bak = cnt;
		while(cnt--)
		{		
			plen=0;
			ScanTime0 = 0;
			memset(pvalue,0,sizeof(pvalue));
			NDK_SysReadWatch(&a1);
			if((ret=NDK_ScanStart()) != NDK_OK )
			{
				NDK_ScanStop();
				cls_show_msg1(3, "line %d:��%d������ͷ����ɨ��ʧ��(%d)", __LINE__, bak-cnt, ret);
				continue;
			}   
			oldtime = time(NULL);
			while(1)
			{	
				if((ret=NDK_ScanState(pvalue, &plen)) == NDK_OK)
				{
					NDK_SysReadWatch(&a);
					ScanTime0 = (a - a1)/(1000*1.0);
					ScanTime += ScanTime0;
					succ++;
					memset(str,0,sizeof(str));
					ftos(ScanTime0,str);
					memset(wbuf, 0, sizeof(wbuf));
					sprintf(wbuf, "%d:data:%s;   len:%d;    time:%ss\r\n", bak-cnt, pvalue, plen, str);
					NDK_PortWrite(PORT_NUM_USB, sizeof(wbuf), wbuf);
					break;
				}
				NDK_SysMsDelay(10);
				if((time(NULL) - oldtime)>MAXTIME)
				{
					NDK_ScanStop();
					cls_show_msg1(3,"line %d:ɨ�賬ʱ", __LINE__);
					break;
				}
			}
			if((ret = NDK_ScanStop())!=NDK_OK)
			{
				cls_show_msg1(3,"line %d:�ر�����ͷʧ��(%d)", __LINE__, ret);
				continue;
			}
		}
		send_result("ͨѶ�ܴ���%d,�ɹ�����%d\n", bak, succ);
		memset(str,0,sizeof(str));
		ftos(ScanTime/succ,str);
		send_result("ɨ������:%ss/��\n", str);
		NDK_PortClose(PORT_NUM_USB);
	}

	if(cls_show_msg1(MAXTIME, "��ȷ�ϼ�����COM1����ɨ�����,����������")==ENTER)
	{

		//����ǰ��:�򿪴���
		NDK_PortClose(PORT_NUM_COM1);
		if((ret = NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1")) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cnt = 20;
		succ = 0;
		ScanTime = 0;

		//case3:��������20��ɨ�룬����COM1�������͵�������
		cls_show_msg("���SecuCRT������,���������ʼɨ�裬ɨ���������͵�SecureCRT");
		bak = cnt;
		while(cnt--)
		{		
			plen=0;
			ScanTime0 = 0;
			memset(pvalue,0,sizeof(pvalue));
			NDK_SysReadWatch(&a1);
			if((ret=NDK_ScanStart()) != NDK_OK )
			{
				NDK_ScanStop();
				cls_show_msg1(3, "line %d:��%d������ͷ����ɨ��ʧ��(%d)", __LINE__, bak-cnt, ret);
				continue;
			}   
			oldtime = time(NULL);
			while(1)
			{	
				if((ret=NDK_ScanState(pvalue, &plen)) == NDK_OK)
				{
					NDK_SysReadWatch(&a);
					ScanTime0 = (a - a1)/(1000*1.0);
					ScanTime += ScanTime0;
					succ++;
					memset(str,0,sizeof(str));
					ftos(ScanTime0,str);
					memset(wbuf, 0, sizeof(wbuf));
					sprintf(wbuf, "%d:data:%s;   len:%d;    time:%ss\r\n", bak-cnt, pvalue, plen, str);
					NDK_PortWrite(PORT_NUM_COM1, sizeof(wbuf), wbuf);
					break;
				}
				NDK_SysMsDelay(10);
				if((time(NULL) - oldtime)>MAXTIME)
				{
					NDK_ScanStop();
					cls_show_msg1(3,"line %d:ɨ�賬ʱ", __LINE__);
					break;
				}
			}
			if((ret = NDK_ScanStop())!=NDK_OK)
			{
				cls_show_msg1(3,"line %d:�ر�����ͷʧ��(%d)", __LINE__, ret);
				continue;
			}
		}
		send_result("ͨѶ�ܴ���%d,�ɹ�����%d\n", bak, succ);
		memset(str,0,sizeof(str));
		ftos(ScanTime/succ,str);
		send_result("ɨ������:%ss/��\n", str);
		NDK_PortClose(PORT_NUM_COM1);
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}


