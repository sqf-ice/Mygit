/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 13.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrGetLcdSize�ܷ��ȡҺ�����ߴ�,NDK_ScrGetColorDepth�ܷ��ȡҺ����ɫ��
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrGetLcdSize,NDK_ScrGetColorDepth"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr13
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr13(void)
{
	/*private & local definition*/
	int ret = 0;
	uint punWidth = 0, punHeight = 0;
#if !K21_ENABLE
	int nKeyin = ENTER;
	uint puncd = 0;
#endif
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣:punWidth,punHeight,puncd�ֱ�ΪNULL
	if((ret=NDK_ScrGetLcdSize(NULL,&punHeight))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetLcdSize(&punWidth,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if !K21_ENABLE //K21 ��֧��NDK_ScrGetColorDepth����
	if((ret=NDK_ScrGetColorDepth(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	//case2:��ȡҺ�����ߴ�
	if((ret=NDK_ScrGetLcdSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("�û���Ļ��СΪ��:%d,��:%d,���˹��ж��Ƿ����ͨ��", punWidth, punHeight);

#if !K21_ENABLE
	//case3:��ȡҺ����ɫ��,�˹��ж�	
	if((ret=NDK_ScrGetColorDepth(&puncd))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(puncd!=1&&puncd!=4&&puncd!=16)//����Һ����ɫ�1----�ڰ���ɫ, 16----16λɫ������  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, puncd);
		RETURN;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "�û�Ϊ%s��,��[ȷ��]����ͨ��,[����]ʧ��",(puncd!=16?"�ڰ�":"��"));
	if (nKeyin==ENTER || nKeyin==0)
		;
	else
		send_result("%s����ʧ��", TESTAPI);
#endif	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

