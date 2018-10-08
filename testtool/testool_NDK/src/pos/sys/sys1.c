/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys1.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_Getlibver�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_Getlibver"//int NDK_Getlibver (char * pszVer):��ȡNDK��汾�� pszVer�汾���ַ���,�����С������8�ֽ�

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
* history 		 		: author			date			remarks
*			  		      linwei	        20130121	 	created
*****************************************************************/
void sys1(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	char szVer[16]={0};
	int ret= -1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�쳣���ԣ��������     
	if((ret=NDK_Getlibver(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��ȡ��ȷ�İ汾��Ϣ
	if((ret=NDK_Getlibver(szVer)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��(Ver:%s)", TESTAPI, szVer);
	return;
}

