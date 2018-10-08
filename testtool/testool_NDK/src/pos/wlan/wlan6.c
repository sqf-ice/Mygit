/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan6.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiGetNetInfo�ӿڹ���
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
#define TESTAPI	"NDK_WiFiGetNetInfo"	
#define APNUM 5
#define APNUM1 64

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan6(void)
* functional description 	: ��ȡɨ�赽��AP��ȫ����Ϣ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					chensj   201305013	created
*****************************************************************/
void wlan6(void)
{

	/*private & local definition*/
	int ret = 0, i = 0, j = 0, numList = 0;
	ST_WIFI_APINFO ESSIDlist[64];
	
	memset(ESSIDlist,0,sizeof(ESSIDlist));
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�豸δ��ʼ������SDK_GetWiFiNetList����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiGetNetInfo(ESSIDlist,APNUM,&numList)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
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

	//case2:��ʼ���쳣�������ԣ������������ΪNULL,Ӧ�÷��ز�������
	if ((ret=NDK_WiFiGetNetInfo(NULL,APNUM, &numList)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiGetNetInfo(ESSIDlist,APNUM, NULL)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiGetNetInfo(ESSIDlist,0, &numList)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��������:��������Ӧ���ܹ���ȡ�����е�AP��Ϣ��������Ļ����ʾ
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetInfo(ESSIDlist,APNUM, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg("Line=%d\nAP[%d]->Essid:%s\nsKeyModeStr:%s\nnFrequency:%d\nsSignal:%s", __LINE__,i, ESSIDlist[i].sEssid,ESSIDlist[i].sKeyModeStr,ESSIDlist[i].nFrequency,ESSIDlist[i].sSignal);
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
	//��APNUMС�ڻ�ȡ����numListʱ,����APNUM�������,��������һ���ж� 20141028 add jiangym
	if(cls_show_msg("��ʾ��AP�����Ƿ�Ϊ5��,��[Enter],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	//case3.1 �趨nMaxNum ����ɨ�赽��APʱ��,ͬʱ�� ��ϣ�����������AP����ʾ���� 
	memset(ESSIDlist,0,sizeof(ESSIDlist));
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetInfo(ESSIDlist,APNUM1, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg("Line=%d\nAP[%d]->Essid:%s\nsKeyModeStr:%s\nnFrequency:%d\nsSignal:%s", __LINE__,i, ESSIDlist[i].sEssid,ESSIDlist[i].sKeyModeStr,ESSIDlist[i].nFrequency,ESSIDlist[i].sSignal);
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
	//��APNUM���ڻ�ȡ����numListʱ,����numList����(������ɨ�赽�����е�AP����)���,��������һ���ж� 20141028 add jiangym
	if(cls_show_msg("��ʾ��AP�����Ƿ�Ϊ������ɨ�赽��AP����,��[Enter],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	

	//case3: AP�����仯,scan���Է�����һ�仯(����Ҫ�˹���һ����AP���ȷ��)
	cls_show_msg("��ر�ĳ̨AP,�䲻Ӧ�ٱ�ɨ�赽,�ر�15�����Һ����������...");
	memset(ESSIDlist,0,sizeof(ESSIDlist));
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetInfo(ESSIDlist,APNUM1, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg("AP[%d]:%s", i, ESSIDlist[i].sEssid);
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
		send_result("%s����ʧ��", TESTAPI);
	else
		send_result("%s����ͨ��", TESTAPI);

	//���Ժ���:WIFI�豸�Ҷ�
ERR:
	NDK_WiFiShutdown();

	return;
}

