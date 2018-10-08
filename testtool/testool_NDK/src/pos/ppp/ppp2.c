/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: pppģ��
* file name		: ppp2.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PppAddrChange�ӿڹ���
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
#define TESTAPI	"NDK_PppAddrChange"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void ppp2(void)
* functional description 	: ��һ�����ʮ���Ƶ�IPת����һ������������
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void ppp2(void)
{
	//����Ϊ�ֲ�����
	Value addr;
	ulong ulIpAddr=0;
	int nRet=0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	
	//case1:��ȷֵ192.168.2.15, �����ۺ����ֽ���,��ֵӦ��Ϊ192<<24+168<<16+2<<8+15
	memset(&addr, 0, sizeof(addr));
	
	if(LITTLEENDIAN==GetCurrentEndianType())
	{addr.asc[0] = 15;addr.asc[1] = 2;addr.asc[2] = 168;addr.asc[3] = 192;}
	else
	{addr.asc[0] = 192;addr.asc[1] = 168;addr.asc[2] = 2;addr.asc[3] = 15;}
	
	if((nRet = NDK_PppAddrChange("192.168.2.15",&ulIpAddr))!=NDK_OK )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	if(addr.data != ulIpAddr)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	
	//case2:��ȷֵ192.168.2.1
	memset(&addr, 0, sizeof(addr));
	
	if(LITTLEENDIAN==GetCurrentEndianType())
	{addr.asc[0] = 1;addr.asc[1] = 2;addr.asc[2] = 168;addr.asc[3] = 192;}
	else
	{addr.asc[0] = 192;addr.asc[1] = 168;addr.asc[2] = 2;addr.asc[3] = 1;}
	if((nRet = NDK_PppAddrChange("192.168.2.1",&ulIpAddr))!=NDK_OK )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	if(addr.data != ulIpAddr)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	
	//case3:��ȷֵ192.168.2.254
	memset(&addr, 0, sizeof(addr));

	if(LITTLEENDIAN==GetCurrentEndianType())
	{addr.asc[0] = 254;addr.asc[1] = 2;addr.asc[2] = 168;addr.asc[3] = 192;}
	else
	{addr.asc[0] = 192;addr.asc[1] = 168;addr.asc[2] = 2;addr.asc[3] = 254;}
	if((nRet = NDK_PppAddrChange("192.168.2.254",&ulIpAddr))!=NDK_OK )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	if(addr.data != ulIpAddr)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}
	
	//case4:�쳣����
	//memset(&addr, 0, sizeof(addr));
	//addr.data = INADDR_NONE; //���������ַ�������һ���Ϸ���IP��ַ��������INADDR_NONE
	if((nRet = NDK_PppAddrChange("192.168.2.a",&ulIpAddr)) != NDK_ERR )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	
	//case5:�ո�
	if((nRet = NDK_PppAddrChange("192.168.2. 1",&ulIpAddr)) != NDK_ERR )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	
	//case6:�Ƿ�IP
	if((nRet = NDK_PppAddrChange("",&ulIpAddr)) != NDK_ERR )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	
	//case7:������IP:192.168.2.	
	if((nRet = NDK_PppAddrChange("192.168.2.",&ulIpAddr)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}

	//case8:NULL
	if((nRet = NDK_PppAddrChange(NULL,&ulIpAddr)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	if((nRet = NDK_PppAddrChange("192.168.2.1",NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
		return;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

