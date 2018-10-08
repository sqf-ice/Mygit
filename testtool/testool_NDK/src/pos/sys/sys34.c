/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys34.c
* Author 			:  
* version			: 
* DATE				:  
* directory 		: 
* description		: ����NDK_SysGetPubkeyInfo�����ܷ��ȡ��Կ��Ϣ(���Ͷ�֧��SP610���ȵ���)
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetPubkeyInfo"	 

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	       linwl  	20170111   created
*****************************************************************/
void sys34(void)
{
	//����Ϊ�ֲ�����
	int ret = 0;
	char pinfo[128] = {0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//case1:�����쳣����
	if((ret=NDK_SysGetPubkeyInfo(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_SysGetPubkeyInfo(pinfo))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	send_result("%s�������,��ȡ���Ĺ�Կ��Ϣ:%s,���˹��ж��Ƿ���ȷ", TESTAPI, pinfo);
	return;
}
