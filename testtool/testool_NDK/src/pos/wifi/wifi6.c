/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��Wifi ģ��
* file name		: wifi6.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_NetAddRouterTable��ʹ��socket֮ǰ���øú���,ѡ������ͨ��WiFi����GPRS�����շ���
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
#define TESTAPI	"NDK_NetAddRouterTable"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 				: void wifi6(void)
* functional description 			:  
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			   zhengry      20160321    created 		 
*****************************************************************/
void wifi6(void)
{
	/*private & local definition*/
	int ret=0;
	
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	//case1:�����쳣����
	/*	�Ͷ˴���ʵ���ϵ�2��IP����û��ʵ��ʹ��,�ʲ�������Ƿ���� 20170303 chenjs����
	if((ret = NDK_NetAddRouterTable(COMM_TYPE_WIFI, NULL) ) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_NetAddRouterTable(COMM_TYPE_WIFI, "0.0.0.1") ) != NDK_ERR_NET_INVALIDIPSTR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}*/
	if((ret = NDK_NetAddRouterTable(COMM_TYPE_ETH, "0.0.0.0") ) != NDK_ERR_NET_UNKNOWN_COMMTYPE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_NetAddRouterTable(COMM_TYPE_BTH, "0.0.0.0") ) != NDK_ERR_NET_UNKNOWN_COMMTYPE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

#if 0
	//case2:��������,�������Ƶ�conf.c��
	if((ret = NDK_NetAddRouterTable(COMM_TYPE_WIFI, "0.0.0.0") ) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
	return;
}
