/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App9.c
* Author 			: huangjianb
* version			: 
* DATE			: 20140814
* directory 		: 
* description		: ���� NDK_AppGetPathByAppName�ӿ�,��ȡӦ�ó���װĿ¼
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AppGetPathByAppName"

#if K21_ENABLE
#define	TESTAPP1	"App_NULL"
#define	TESTAPP2	"�Լ�1"
#define FILETEST1   "/appfs/App_NULL.NLD" 
#define FILETEST2   "/appfs/�Լ�1.NLD" 
#elif OVERSEAS_ENABLE
#define	TESTAPP1	"typeAunMaster"
#define	FILETEST1 	"typeAunMaster.NLD"  //Ϊ�˲��Դ�������Ϊ��ǰ·�� 20180423
#elif defined ME50S|| defined ME50NS
#define	TESTAPP1	"App_A_NonMaster"
#define	TESTAPP2	"a~!@#%^_=+,.[{]}��"
#define FILETEST1   "/appfs/apps/download/App_A_NonMaster.NLD"
#define FILETEST2   "/appfs/apps/download/a~!@#%^_=+,.[{]}��.NLD"
#else
#define	TESTAPP1	"App_A_NonMaster"
#define	TESTAPP2	"a~!@#%^_=+,.[{]}��"
#define FILETEST1   "/appfs/download/App_A_NonMaster.NLD"
#define FILETEST2   "/appfs/download/a~!@#%^_=+,.[{]}��.NLD"
#endif

/*---------------constants/macro definition---------------------*/

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : app8
* functional description : ��ȡӦ�ó���װĿ¼
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*										 huangjianb 20140814  created
*****************************************************************/
void app9(void)
{
	/*private & local definition*/
	int ret = 0;

#if K21_ENABLE
	char pszAppName1[64]={"App_NULL"}, sharepath1[64]={"/appfs/apps/App_NULL/"},pszAppPath[64]={"0"};
	char pszAppName2[64]={"�Լ�1"}, sharepath2[64]={"/appfs/apps/�Լ�1/"};
	char pszAppName3[64]={"NoExist"};//����Ҫ���أ�����Ӧ�ò�����
#elif defined SP630PG||defined SP830PG
	char pszAppName1[64]={"typeAunMaster"}, sharepath1[64]={"/app/typeAunMaster/"},pszAppPath[64]={"0"};
	char pszAppName3[64]={"NoExist"};//����Ҫ���أ�����Ӧ�ò�����
#elif OVERSEAS_ENABLE&&!defined SP630PG&&!defined SP830PG
	char pszAppName1[64]={"typeAunMaster"}, sharepath1[64]={"/appfs/apps/typeAunMaster/"},pszAppPath[64]={"0"};
	char pszAppName3[64]={"NoExist"};//����Ҫ���أ�����Ӧ�ò�����
#else
	char pszAppName1[64]={"App_A_NonMaster"}, sharepath1[64]={"/appfs/apps/App_A_NonMaster/"},pszAppPath[64]={"0"};
	char pszAppName2[64]={"a~!@#%^_=+,.[{]}��"}, sharepath2[64]={"/appfs/apps/a~!@#%^_=+,.[{]}��/"};
	char pszAppName3[64]={"NoExist"};//����Ҫ���أ�����Ӧ�ò�����
#endif

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 
	
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_ENABLE  //����汾��֧�������ַ���NLD��
	if(( ret = NDK_AppLoad((uchar *)FILETEST2,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif		
	//case1:�����Ƿ�(pszAppNameΪNULL,pszAppPathΪNULL)
	if(( ret = NDK_AppGetPathByAppName(NULL,pszAppPath))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(( ret = NDK_AppGetPathByAppName(pszAppName1,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:�����Ƿ�(Ӧ�ò�����)
	if(( ret = NDK_AppGetPathByAppName(pszAppName3,pszAppPath))!=NDK_ERR_APP_FILE_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:�����Ƿ�
	if(( ret = NDK_AppGetPathByAppName("",pszAppPath))!=NDK_ERR_APP_FILE_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:������ȷ,����Ӧ����
	memset(pszAppPath, 0, sizeof(pszAppPath));
	if(( ret = NDK_AppGetPathByAppName(pszAppName1,pszAppPath))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(pszAppPath, sharepath1))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, pszAppPath);
		goto ERR;
	}
#if !OVERSEAS_ENABLE  //����汾��֧�������ַ���NLD��
	//case5:������ȷ,Ӧ����Ϊ�����ַ����
	memset(pszAppPath, 0, sizeof(pszAppPath));
	if(( ret = NDK_AppGetPathByAppName(pszAppName2,pszAppPath))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(pszAppPath, sharepath2))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, pszAppPath);
		goto ERR;
	}
#endif 	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_AppDel(TESTAPP1);
#if !OVERSEAS_ENABLE 	
	NDK_AppDel(TESTAPP2);
#endif
	return;
}
