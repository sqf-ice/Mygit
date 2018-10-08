/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec8.c
* Author 		: huangjianb
* version		: 
* DATE			: 20140821
* directory 		: 
* description		: ����NDK_SecSetCfg,NDK_SecGetCfg�ӿڹ���
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
#define TESTAPI	"NDK_SecSetCfg,NDK_SecGetCfg"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author					date				remarks
*			  	  	  		 huangjianb     20140821		created
*****************************************************************/
void sec8(void)
{
	int ret = 0;
	uint punCfgInfo = 0, punCfg = 0;
	
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI);
	
	//����ǰ��
	if(((ret=NDK_SecGetCfg(&punCfgInfo))!=NDK_OK)||(punCfgInfo!=6)) //��ȡ��ǰ��ȫ�ȼ�
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punCfgInfo);
		GOTOERR;
	}
	
	//�Ƿ�����
	if((ret=NDK_SecGetCfg(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//�ر�����
	if((ret=NDK_SecSetCfg(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_SecGetCfg(&punCfg))!=NDK_OK)||(punCfg!=0))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punCfg);
		GOTOERR;
	}
	
	//������ԿΨһ�Կ���λ
	if((ret=NDK_SecSetCfg(2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_SecGetCfg(&punCfg))!=NDK_OK)||(punCfg!=2))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punCfg);
		GOTOERR;
	}
	
	//������Կר���Կ���λ
	if((ret=NDK_SecSetCfg(4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_SecGetCfg(&punCfg))!=NDK_OK)||(punCfg!=4))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punCfg);
		GOTOERR;
	}
	
	//������Կ���д�������
	if((ret=NDK_SecSetCfg(8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_SecGetCfg(&punCfg))!=NDK_OK)||(punCfg!=8))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punCfg);
		GOTOERR;
	}
	
	//������Կ����ǿ��
	if((ret=NDK_SecSetCfg(16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_SecGetCfg(&punCfg))!=NDK_OK)||(punCfg!=16))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punCfg);
		goto ERR;
	}
	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_SecSetCfg(punCfgInfo);
	return;
}
