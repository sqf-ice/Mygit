/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����Wifi ģ��
* file name		: wlan2.c
* Author 		: chensj
* version		: 
* DATE			: 20130513
* directory 		: 
* description		: ����NDK_WiFiSignalCover�ӿڹ���
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
#define TESTAPI	"NDK_WiFiSignalCover"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlan2(void)
* functional description 	: Wifi ��ȡ�ź�ֵ 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  					chensj   201305013	created
*****************************************************************/

void wlan2(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0, numList = 0;
	int signal = 0;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];

	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�豸δ��ʼ������SDK_WiFiSignalCover����Ӧ�÷���ʧ��
	if ((ret=NDK_WiFiSignalCover("XXX", &signal)) != NDK_ERR_WIFI_DEVICE_UNAVAILABLE)
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

	//ɨ��AP
	for (j=0; j<3; j++) 
	{
		if ((NDK_WiFiGetNetList(ESSIDlist, &numList)) == NDK_OK) 
		{
			if (numList > 0) 
			{
				for (i=0; i<numList; i++)
					cls_show_msg("��ɨ�赽%d��AP,AP[%d]=%s", numList, i, ESSIDlist1[i]);
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
	if ((ret=NDK_WiFiSignalCover(NULL, &signal)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if ((ret=NDK_WiFiSignalCover(ESSIDlist1[0], NULL)) != NDK_ERR_WIFI_INVDATA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	
	//case4:��������:AP�����ֲ���ȷʱ,��ȡ�ź�Ӧ��ʧ��
	if ((ret=NDK_WiFiSignalCover("XXX", &signal)) != NDK_ERR_WIFI_DEVICE_NOTOPEN)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��������:��������Ӧ���ܹ���ȡ��ɨ�赽��AP���źţ�������Ļ����ʾ
	for(i=0;i<numList;i++)
	{
		//�������п���ʧ�ܣ���Ϊ���֮ǰɨ�赽��AP��ĳ��AP�źŲ�ǿ����ȡ�ź�ʱ���ȡ����
		if ((ret=NDK_WiFiSignalCover(ESSIDlist1[i], &signal)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��AP[%d]=%s,ret=%d", __LINE__, TESTAPI, i, ESSIDlist1[i], ret);
			GOTOERR;
		}
		if(cls_show_msg1(30, "AP[%d]:%s���ź�Ϊ:%d,ȷ�ϼ�ͨ��,����ʧ��", i, ESSIDlist1[i], signal)!=ENTER)
		{
			send_result("line %d:%s����ʧ��AP[%d]=%s,signal=%d", __LINE__, TESTAPI, i, ESSIDlist1[i], signal);
			GOTOERR;
		}
	}

	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
ERR:
	//���Ժ���:WIFI�豸�Ҷ�
	NDK_WiFiShutdown();
	return;
}


