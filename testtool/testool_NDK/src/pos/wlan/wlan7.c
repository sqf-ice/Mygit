/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan7.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiGetMac�ӿڹ���
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
#define TESTAPI	"NDK_WiFiGetMac"	
#define APNUM 5

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan7(void)
* functional description 	: ��ȡMac ��ַ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					chensj   201305013	created
*****************************************************************/
void wlan7(void)
{

	/*private & local definition*/
	int ret = 0,j = 0;
	char szMac[64]={0};	

	/*process body*/
	if(auto_flag==2)
		return;
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�豸δ��ʼ������SDK_GetWiFiNetList����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiGetMac(szMac)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE) //NDK_ERR_WIFI_DEVICE_UNAVAILABLE
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
	if ((ret=NDK_WiFiGetMac(NULL)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��������:��ȡmacֵ
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetMac(szMac) == NDK_OK)
		{
			send_result("Line=%d,szMac=%s", __LINE__,szMac);
			break;		
		}
		sleep(1);
	}
	if (j == 3)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
	send_result("%s���Գɹ�", TESTAPI);

	//���Ժ���:WIFI�豸�Ҷ�
ERR:
	NDK_WiFiShutdown();
	return;
}

