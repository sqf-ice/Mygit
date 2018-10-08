/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys32.c
* Author 			:  
* version			: 
* DATE				:  
* directory 		: 
* description		: ����NDk_SysGetK21Version ��ȡk21��ָ��汾�ŵĹ���
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDk_SysGetK21Version"	 

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
*			  	  	       jiangym  	20160603   created
*****************************************************************/
void sys32(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int ret = -1;
	char verstr[32] = {0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//case1:�쳣����:�����Ƿ� 
	if((ret=NDk_SysGetK21Version(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��������:��ȡk21��ָ��汾��
	if((ret=NDk_SysGetK21Version(verstr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//���Խ���
	send_result("%s����ͨ��(ָ�Ver:%s)", TESTAPI, verstr);
	return;	
}

