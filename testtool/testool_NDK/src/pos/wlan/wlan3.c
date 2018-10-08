/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan3.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiGetSec�ӿڹ���
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
#define TESTAPI	"NDK_WiFiGetSec"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan3(void)
* functional description 	: ��ȡָ��AP�İ�ȫģʽ 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					chensj   201305013	created
*****************************************************************/
void wlan3(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0, numList = 0;
	EM_WIFI_NET_SEC sec;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];

	char *sectypestr[] = {"WIFI_NET_SEC_NONE","WIFI_NET_SEC_WEP_OPEN","WIFI_NET_SEC_WEP_SHARED","WIFI_NET_SEC_WPA","WIFI_NET_SEC_WPA2"};
	EM_WIFI_NET_SEC secresult[5]={WIFI_NET_SEC_NONE, WIFI_NET_SEC_WEP_OPEN, WIFI_NET_SEC_WEP_OPEN, WIFI_NET_SEC_WPA, WIFI_NET_SEC_WPA2}; //	WIFI_NET_SEC_WEP_OPEN ��WIFI_NET_SEC_WEP_SHAREDģʽ������	
	
	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�豸δ��ʼ������SDK_GetWiFiSec����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiGetSec("XXX", &sec)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//����ǰ��:WIFI�豸��ʼ��
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//ɨ��AP,����ɨ��3��
	for (j=0; j<3; j++) 
	{
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList)) == NDK_OK) 
		{
			if (numList > 0) 
			{
				for (i=0; i<numList; i++)
					cls_show_msg("��ɨ�赽%d��AP,Line=%d,AP[%d]=%s", numList, __LINE__,i, ESSIDlist1[i]);
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

	//case2:�쳣�������ԣ������������ΪNULL,Ӧ�÷��ز�������
	if ((ret=NDK_WiFiGetSec(WlanEssid, NULL)) != NDK_ERR_WIFI_INVDATA )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiGetSec(NULL, &sec)) != NDK_ERR_WIFI_INVDATA )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�����ܹ�ʶ�����еļ���ģʽ����
	for(i=0;i<SZ_ARRAY(sectypestr);i++)
	{
		if (cls_show_msg("�뽫%s�ļ���ģʽ�޸�Ϊ%s��[ȷ��]����,[����]�˳�", WlanEssid,sectypestr[i]) == ENTER)
		{
			if ((ret=NDK_WiFiGetNetList(ESSIDlist, &numList)) != NDK_OK) //�ײ��޸�NDK_WiFiGetSecʱ�����Զ�ɨ��,����������Ҫ����ɨ��AP�Ķ���
			{
				send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if ((ret=NDK_WiFiGetSec(WlanEssid, &sec)) != NDK_OK)//�����testwifi�����ǲ�����WIFI������
			{
				send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if(sec!=secresult[i])//��֤��ȡ���ļ���ģʽ�����õ�һ��
			{
				send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, sec);
				GOTOERR;
			}
		}
	}

	//case5:��������:AP�����ֲ���ȷʱ,��ȡ����ģʽӦ��ʧ��
	if ((ret=NDK_WiFiGetSec("XXX", &sec)) != NDK_ERR_WIFI_DEVICE_NOTOPEN )
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��������:��������Ӧ���ܹ���ȡ��ɨ�赽��AP�ļ���ģʽ,������Ļ����ʾ
	for(i=0;i<numList;i++)
	{	
		//�������п���ʧ�ܣ���Ϊ���֮ǰɨ�赽��AP��ĳ��AP�źŲ�ǿ����ȡ����ģʽʱ���ȡ����
		if ((ret=NDK_WiFiGetSec(ESSIDlist1[i], &sec)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��AP[%d]:%s,ret=%d", __LINE__, TESTAPI, i, ESSIDlist1[i], ret);
			GOTOERR;
		}
		if(cls_show_msg1(30, "Line=%d,AP[%d]=%s�ļ���ģʽΪ:%s,sec=%d,ȷ�ϼ�ͨ��,����ʧ��",__LINE__, i, ESSIDlist1[i], sectypestr[sec],sec)!=ENTER)
		{
			send_result("line %d:%s����ʧ��AP[%d]:%s,sec=%d", __LINE__, TESTAPI, i, ESSIDlist1[i], sec);
			GOTOERR;
		}
	}

	send_result("%s����ͨ��,�뽫·���������޸ĳ�POS���õĲ����������������", TESTAPI);
	goto ERR;
ERR:
	//���Ժ���:WIFI�豸�Ҷ�
	NDK_WiFiShutdown();
	return;
}


