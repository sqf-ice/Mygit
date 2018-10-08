/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc1.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_RfidVersion�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidVersion"

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
*			  		    chenfm	        20130108	 	created
*****************************************************************/
void rfc1(void)
{
	//����Ϊ�ֲ�����
	uchar version[128]={0};
	int ret= -1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��

	//case1:�쳣���ԣ��������     
	if((ret=NDK_RfidVersion(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��ȡ��ȷ�İ汾��Ϣ
	if((ret=NDK_RfidVersion(version))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��(Ver:%s)", TESTAPI, version);
	return;
}

