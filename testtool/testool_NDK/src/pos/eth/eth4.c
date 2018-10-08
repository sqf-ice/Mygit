/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth4.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_NetPing�ӿڹ���
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_NetPing"
#define		TIMEOUT		3.0
#define		WUCHASEC	0.2 //���ֵ����


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
*			  						chensl		   20130105	 		created
*****************************************************************/
void eth4(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1;
	float tmp = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//����ǰ��,������̫����ַ��������̫��ģ��
	//if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, NULL)) != NDK_OK)
	//{
	//	send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
	//	return;
	//}
	if(NetUp()!=SUCC)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
	//case1:����Ƿ������������ַ�������ڵ�ַ
	if(((ret=NDK_NetPing(NULL, 0)) != NDK_ERR_PARA)
		||((ret1=NDK_NetPing("192.168.114.2066", 0)) != NDK_ERR_PARA)){
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
	
	//case2:ping������IP
	if ((ret=NDK_NetPing(netsetting.SvrIP.Str, TIMEOUT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3:ping��������һ��ip
	if ((ret=NDK_NetPing(netsetting.LocalGateway.Str, TIMEOUT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0	//II��֣����������ȷ��:û��������� ���Բ���ping���
	//case4:ping��ѯ��ַ127.0.0.1
	if ((ret=NDK_NetPing("127.0.0.1", TIMEOUT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	
	//case5:��ʱ����
	lib_StartStopwatch();
	if ((ret=NDK_NetPing("192.166.3.44", TIMEOUT)) != NDK_ERR || (tmp=fabs(lib_ReadStopwatch()-TIMEOUT))>WUCHASEC)//����ķ���ֵ  Ӧ��Ҫ���س�ʱ
	{
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(tmp,str);
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, str);
#else		
		send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
#endif
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NetDown();
	return;
}

