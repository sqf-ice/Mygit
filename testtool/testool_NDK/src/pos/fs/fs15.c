/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs15.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_Directory������
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_FsCreateDirectory,NDK_FsRemoveDirectory"

#define TESTDIR     "./test1"
#define TESTSUBDIR  "./test1/test2"

/****************************************************************
* function name 	 	: void fs15(void)
* functional description 	: ����NDK_Directory������
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121217	created
*****************************************************************/
void fs15(void)
{
	/*private & local definition*/
	int ret= -1;

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 

	//����ǰ�ã�
	NDK_FsRemoveDirectory(TESTSUBDIR);	
	NDK_FsRemoveDirectory(TESTDIR);

	//case1 :�Ƿ�����
	if((ret=NDK_FsCreateDirectory(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsRemoveDirectory(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	 
	//case2:������Ŀ¼�����ڵ���Ŀ¼Ӧ��ʧ��
	if((ret=NDK_FsCreateDirectory(TESTSUBDIR))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:ɾ��.��..������Ŀ¼,Ӧ���ɹ�
	if((ret=NDK_FsRemoveDirectory("."))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_FsRemoveDirectory(".."))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:������ǰĿ¼��Ӧ���ɹ�;        
	if((ret=NDK_FsCreateDirectory("./"))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:���������ڵ���Ŀ¼Ӧ�ɹ�
	if((ret=NDK_FsCreateDirectory(TESTDIR))!=NDK_OK) //Ӧ�ɹ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�����Ѵ��ڵ�Ŀ¼��Ӧ���ɹ�; 
	if((ret=NDK_FsCreateDirectory(TESTDIR))!=NDK_ERR) //Ӧʧ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:������Ŀ¼�Ѵ��ڵ���Ŀ¼Ӧ�óɹ�
	if((ret=NDK_FsCreateDirectory(TESTSUBDIR))!=NDK_OK) //Ӧ�ɹ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	
	//case8:����ɾ��һ��������Ŀ¼��Ӧ�ɹ����ء�
	if((ret=NDK_FsRemoveDirectory(TESTSUBDIR))!=NDK_OK) //Ӧ�ɹ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}   

	//case9:����ɾ��һ��Ŀ¼��Ӧʧ�ܷ���
	if((ret=NDK_FsRemoveDirectory(TESTDIR))!=NDK_OK) //Ӧ�ɹ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}   
	     
	//case10:ɾ�������ڵ�Ŀ¼Ӧʧ��
	if((ret=NDK_FsRemoveDirectory(TESTDIR))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
	return;	
ERR:
	//���������Ժ��ã�  
	NDK_FsRemoveDirectory(TESTSUBDIR);    
	NDK_FsRemoveDirectory(TESTDIR);	
	return;
}

