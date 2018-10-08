/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan1.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiGetNetList,NDK_WiFiInit�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_WiFiGetNetList,NDK_WiFiInit"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan1(void)
* functional description 	: Wifi ��ȡɨ�赽��AP��SSID��Ϣ 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					chensj   201305013	created
*****************************************************************/
void wlan1(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0, numList = 0;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];

	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�豸δ��ʼ������SDK_GetWiFiNetList����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiGetNetList(ESSIDlist, &numList)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//����ǰ��:WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:����������ʼ����Ӧ�óɹ�
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��ʼ���쳣�������ԣ������������ΪNULL,Ӧ�÷��ز�������
	if ((ret=NDK_WiFiGetNetList(NULL, &numList)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiGetNetList(ESSIDlist, NULL)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��������:��������Ӧ���ܹ���ȡ�����е�AP��Ϣ��������Ļ����ʾ
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetList(ESSIDlist, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg1(3,"��ɨ�赽%d��AP,Line=%d,AP[%d]:%s", numList, __LINE__,i, ESSIDlist1[i]);
				break;
			}
		}
		sleep(1);
	}
	if (j == 3)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	if(auto_flag==0||auto_flag==2)
	{
		//case5: AP�����仯,scan���Է�����һ�仯(����Ҫ�˹���һ����AP���ȷ��)
		cls_show_msg("��ر�ĳ̨AP,�䲻Ӧ�ٱ�ɨ�赽,�ر�15�����Һ����������...");
		for (j=0; j<3; j++)
		{
			if (NDK_WiFiGetNetList(ESSIDlist, &numList) == NDK_OK)
			{
				if (numList > 0)
				{
					for (i=0; i<numList; i++)
					    cls_show_msg("��ɨ�赽%d��AP,AP[%d]:%s", numList, i, ESSIDlist1[i]);
					break;
				}
			}
			sleep(1);
		}
		if (j == 3)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
		
		if(ENTER!=cls_show_msg("�رյ�AP���ٱ�ɨ�赽,ͨ����[ENTER],����[����]", TESTAPI))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
	}
	else
		send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
	
	send_result("%s����ͨ��", TESTAPI);

	//���Ժ���:WIFI�豸�Ҷ�
ERR:
	NDK_WiFiShutdown();
	return;
}

