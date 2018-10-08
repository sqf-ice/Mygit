/************************************************************************
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm7.c
* Author 		: jiangym	
* version		: 
* DATE			: 20130821
* directory 		: 
* description		: ����NDK_WlGetInfo�ӿڻ�ȡccid,imei,imsi�Ĺ���
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
#define TESTAPI	"NDK_WlGetInfo"	
#define	MAXWAITTIME	30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm7(void)
* functional description 	: ��ȡ���� ccid,imei,imsi
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  				
*****************************************************************/
void wlm7(void)
{
	/*private & local definition*/
	int ret = 0;
	char pszvalue[25] = {0};
	
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣����:emType�쳣,pszvalueΪNULL,unBufLen���Ȳ���
	if((ret = NDK_WlGetInfo(WLM_INFO_CCID-1,pszvalue,sizeof(pszvalue)))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetInfo(WLM_INFO_UNDEFINE,pszvalue,sizeof(pszvalue)))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetInfo(WLM_INFO_CCID,NULL,sizeof(pszvalue)))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetInfo(WLM_INFO_CCID,pszvalue,19))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetInfo(WLM_INFO_IMSI,pszvalue,14))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetInfo(WLM_INFO_IMEI,pszvalue,8))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�������Ի�ȡccid
	memset(pszvalue,0,sizeof(pszvalue));
	if((ret = NDK_WlGetInfo(WLM_INFO_CCID,pszvalue,sizeof(pszvalue)))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, pszvalue);
		RETURN;
	}
	cls_show_msg1(2,"CCID=%s",pszvalue);
	if(strlen(pszvalue)!=20)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, strlen(pszvalue));
		RETURN;
	}
	//case3:�������Ի�ȡIMSI
	memset(pszvalue,0,sizeof(pszvalue));
	if((ret = NDK_WlGetInfo(WLM_INFO_IMSI,pszvalue,sizeof(pszvalue)))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, pszvalue);
		RETURN;
	}
	cls_show_msg1(2,"imsi=%s",pszvalue);
	if(strlen(pszvalue)>15 || (is_digit_str(pszvalue))!=TRUE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, strlen(pszvalue));
		RETURN;
	}
	//case4:�������Ի�ȡIMEI
	memset(pszvalue,0,sizeof(pszvalue));
	if((ret = NDK_WlGetInfo(WLM_INFO_IMEI,pszvalue,sizeof(pszvalue)))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, pszvalue);
		RETURN;
	}
	cls_show_msg1(2,"imei=%s",pszvalue);
	if(strlen(pszvalue)>20/*15 || (is_digit_str(pszvalue))!=TRUE*/)//NDK 4.4.1�汾���ĵ����:IMEI�ܳ�������20����0-F���
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, strlen(pszvalue));
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

