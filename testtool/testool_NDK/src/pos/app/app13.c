/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name			: App13.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ���� NDK_NlpVerifyУ��NLP �ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_NlpVerify"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���2������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*						jiangym           20170622          created
*****************************************************************/
void app13(void)
{
	/*private & local definition*/
	int ret = 0;
	int  errnum = 0;
	char nlpbuf[950] = {0}; //50*19

	/*process body*/
	if(cls_show_msg1(30, "����%s...��ȷ������NLP����,ESC�˳�", TESTAPI)==ESC)
		return;
	
	cls_printf("����У��NLP��...");
	//case1:�����쳣
	if(( ret = NDK_NlpVerify(NULL,&errnum))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret = NDK_NlpVerify(nlpbuf,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������
	if(( ret = NDK_NlpVerify(nlpbuf,&errnum))!=NDK_OK)
		cls_show_msg1(30,"������ret=%d,�����NLP��%d��,Ϊ:%s,���������", ret, errnum, nlpbuf);
	else
		cls_show_msg1(30,"û�г����NLP��,���������");
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

