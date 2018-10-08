/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt7.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnGetVersion�ӿڹ���
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
#define TESTAPI	"NDK_PrnGetVersion"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt7(void)
* functional description 	:��ȡ��ӡ�����İ汾��Ϣ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt7(void)
{
	//����Ϊ�ֲ�����
	int nRet = 0;
	char VerBuf[32]={0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	
	//case1:����һ��buf�ܷ���ķ�����ȷ�İ汾��
	if((nRet = NDK_PrnGetVersion(VerBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
		return ;
	}
	
	//case2:����bufΪ��,Ӧ���ش�����
	if((nRet = NDK_PrnGetVersion(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
		return ;
	}
	
	//���Խ���	
	send_result("%s����ͨ��(ver:%s)", TESTAPI,VerBuf);
	return;
}

