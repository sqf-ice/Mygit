/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name			: spfs11.c
* Author 			: wangk
* version			: 
* DATE				: 20180829
* directory 		: 
* description		: ����NDK_SP_FsExist�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI		"NDK_SP_FsExist"
#define FILETEST13	"/appfs/FT13"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���17������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                          wangk          20180829        created
*****************************************************************/
void spfs11(void)
{
	/*private & local definition*/
	int fp=0, ret=0;

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_SP_FsDel(FILETEST13);

	// case1: ��"w"��ʽ��һ���ض��ļ�,����NDK_SP_FsExist�жϸ��ļ��Ƿ����(Ӧ�ô���)
	if((fp=NDK_SP_FsOpen(FILETEST13, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsExist(FILETEST13))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2:�ر��ļ������ж��ļ��Ƿ����(Ӧ�ô���)
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsExist(FILETEST13))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3: ɾ�����ļ�,����NDK_SP_FsExist�жϸ��ļ��Ƿ����(Ӧ�ò�����)
	if((ret=NDK_SP_FsDel(FILETEST13))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsExist(FILETEST13))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�Ƿ���������
	if((ret=NDK_SP_FsExist(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST13);
	return;
}