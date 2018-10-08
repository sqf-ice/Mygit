/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys38.c
* Author 			: 
* version			: 
* DATE				: 
* directory 		: 
* description		: ��������NDK_SysUpdateBootLogo�ӿ�(�����Ʒ֧��)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define   ERR_SIZE   "errlogo1.bmp"  //240*320��С��bmpͼƬ,λ���24
#define   TESTJPG    "errlogo2.jpg"  //320*240��С��jpgͼƬ,λ���24
#define   TESTPNG    "errlogo3.png"  //320*240��С��pngͼƬ,λ���24
#define   ERR_DEPTH  "errlogo4.bmp"  //320*240��С��bmpͼƬ,λ���8
#define   NOT_EXIST  "nonelogo.bmp"
#define   TESTLOGO1  "testlogo1.bmp"
#define   TESTLOGO2  "testlogo2.bmp"
#define   NLLOGO     "newland.bmp"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysUpdateBootLogo"

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
* history 		 		: author		date		remarks
*			  	  	       sull       20180109      created 
*****************************************************************/
void sys38(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	if(cls_show_msg1(5,"��ȷ�����ڵ�ǰ·�����������в���ͼƬ,[Enter]����,��������" )!=ENTER && auto_flag!=1)
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI);
		return;
	}

	//case1�������Ƿ�
	if((ret = NDK_SysUpdateBootLogo(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2������һ�Ų����ڵ�ͼƬ������NDK_ERR
	if((ret = NDK_SysUpdateBootLogo(NOT_EXIST)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:�������ͼƬ��Ϊ320*240��λ���Ϊ16��24��bmpͼƬ�����ش���
	if((ret = NDK_SysUpdateBootLogo(ERR_SIZE)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,ERR_SIZE);
		GOTOERR;
	}
	if((ret = NDK_SysUpdateBootLogo(TESTJPG)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysUpdateBootLogo(TESTPNG)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysUpdateBootLogo(ERR_DEPTH)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4����logoͼƬ��ԭ��ͼƬһ��������NDK_ALREADY_DONE
	//ֻҪһ����NDK_SysUpdateBootLogo�ɹ���flash��ͻ���������ͼƬ������Ҫ�������ɲ��Ը�����
	if((ret = NDK_SysUpdateBootLogo(TESTLOGO1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret = NDK_SysUpdateBootLogo(TESTLOGO1)) != NDK_ALREADY_DONE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if(auto_flag == 0 || auto_flag == 2)
	{
		if((cls_show_msg1(5,"��һ�ν���ð���?\n��[ENTER]��������������\n����ʱ�۲쿪��logo�Ƿ����Ϊ%s.�ָ�Ĭ��logo���ٴν���˰���.",TESTLOGO2)) == ENTER)
		{
			//case5����������logo�������۲��Ƿ���Ч
			if((ret = NDK_SysUpdateBootLogo(TESTLOGO2)) != NDK_OK)
			{
		 		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			}
			NDK_SysReboot();
		}
		send_result("%s����ͨ��", TESTAPI);
	}
	else	
		send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);

ERR:
	//���Ժ���:��Ϊԭ�����´�½logo
	NDK_SysUpdateBootLogo(NLLOGO);
	return;
}


