/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys36.c
* Author 			:  
* version			: 
* DATE				:  
* directory 		: 
* description		: ����NDK_szGetCapability�ܷ�������ȡ�Ƿ�֧���⿨����(���߶�֧��)
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGetCapability" 

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
*			  		     jiangym				20171229		created
*****************************************************************/
void sys36(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	char caps[16] = {0};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�쳣��������(nSizeOfCap��Ҫ>=5)        ֮ǰnSizeOfCap��>=3�����ڸ���Ϊ>=5�� linying 20180503 modified
	if((ret=NDK_SysGetCapability(3,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetCapability(4,caps)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1:������ȡ
	if((ret=NDK_SysGetCapability(sizeof(caps)/*5*/,caps)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strlen(caps) >= 5)
	{
		if(caps[2]=='Y')
		{	
			// ��ǰ�汾֧���⿨
			cls_show_msg1(10,"��ǰ�汾֧���⿨�����������");
	 	 }
	 	else
		{
			// ��ǰ�汾��֧���⿨
			cls_show_msg1(10,"��ǰ�汾��֧���⿨�����������");
		}
		if(caps[3]=='Y')
		{	
			// ��ǰ�汾֧�ִ�ӡ
			cls_show_msg1(10,"��ǰ�汾֧�ִ�ӡ�����������");
		 }
		else
		{
			// ��ǰ�汾��֧�ִ�ӡ
			cls_show_msg1(10,"��ǰ�汾��֧�ִ�ӡ�����������");
		}
		if(caps[4]=='Y')
		{	
			// ��ǰ�汾֧����Ƶ
			cls_show_msg1(10,"��ǰ�汾֧����Ƶ�����������");
	 	 }
	 	else
		{
			// ��ǰ�汾��֧����Ƶ
			cls_show_msg1(10,"��ǰ�汾��֧����Ƶ�����������");
		}
		//������ȡ�Ƿ�֧��IC���ʹſ� 20180509  sull add
		if(caps[5]=='Y')
		{	
			// ��ǰ�汾֧��IC��
			cls_show_msg1(10,"��ǰ�汾֧��IC�������������");
	 	 }
	 	else
		{
			// ��ǰ�汾��֧��IC��
			cls_show_msg1(10,"��ǰ�汾��֧��IC�������������");
		}
		if(caps[6]=='Y')
		{	
			// ��ǰ�汾֧�ִſ�
			cls_show_msg1(10,"��ǰ�汾֧�ִſ������������");
	 	 }
	 	else
		{
			// ��ǰ�汾��֧�ִſ�
			cls_show_msg1(10,"��ǰ�汾��֧�ִſ������������");
		}
	}
	else
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, strlen(caps));
		RETURN;
	}	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

