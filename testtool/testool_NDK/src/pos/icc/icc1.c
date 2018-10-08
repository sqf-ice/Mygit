/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: ����NDK_IccGetVersion�Ƿ������ȷ�Ļ�ð汾��Ϣ
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_IccGetVersion"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: icc1
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	 	 chenfm     20130107	created
*				  	 
*****************************************************************/
void icc1(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	char version[128]={0};
	int ret= -1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��

	//case1:�쳣���ԣ��������     
	if((ret=NDK_IccGetVersion(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��ȡ��ȷ�İ汾��Ϣ
	if((ret=NDK_IccGetVersion(version))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��(Ver:%s)", TESTAPI, version);
	return;
}

