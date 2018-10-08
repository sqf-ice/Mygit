/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App8.c
* Author 			: huangjianb
* version			: 
* DATE			: 20140814
* directory 		: 
* description		: ���� NDK_AppGetPathById�ӿ�,��ȡӦ�ù������Ŀ¼
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AppGetPathById"

/*---------------constants/macro definition---------------------*/

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : app8
* functional description : ��ȡӦ�ù������Ŀ¼
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*										 huangjianb 20140814  created
*****************************************************************/
void app8(void)
{
	/*private & local definition*/
	int ret = 0;	
#if !(defined SP630PG||defined SP830PG)//SP630PG�ļ�Ŀ¼���޸�linying 20180322 modified
	char emPathId1[64]={0}, pszPath1[64]={"/appfs/apps/share/"};
	char emPathId2[64]={0}, pszPath2[64]={"/appfs/apps/share/"};
#else 
    char emPathId1[64]={0}, pszPath1[64]={"/app/share/"};
	char emPathId2[64]={0}, pszPath2[64]={"/app/share/"};
#endif
	char emPathId3[64]={0}, pszPath3[64]={"/tmp/"};
	
	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 
	
	//case1:�����Ƿ� emPathId����ö�ٷ�Χ��
	if(( ret = NDK_AppGetPathById(APPPATH_LIARARY-1,emPathId1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret = NDK_AppGetPathById(APPPATH_TMP+1,emPathId1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�����Ƿ� pszPathΪNULL
	if(( ret = NDK_AppGetPathById(APPPATH_LIARARY,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	
	//case4����ȷ��ȡӦ�ÿ���Ŀ¼Ӧ�ɹ�
	if(( ret = NDK_AppGetPathById(APPPATH_LIARARY,emPathId1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp(emPathId1, pszPath1))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, emPathId1);
		RETURN;
	}
	
	//case5:��ȷ��ȡӦ�ù����Ŀ¼Ӧ�ɹ�
	if(( ret = NDK_AppGetPathById(APPPATH_SHARA_LIBRARY,emPathId2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp(emPathId2, pszPath2))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, emPathId2);
		RETURN;
	}
	
	//case6:��ȷ��ȡӦ����ʱĿ¼Ӧ�ɹ�
	if(( ret = NDK_AppGetPathById(APPPATH_TMP,emPathId3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp(emPathId3, pszPath3))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, emPathId3);
		RETURN;
	}
	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
}
