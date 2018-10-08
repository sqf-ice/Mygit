/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: scr22.c
* Author 			: jiangym
* version			: 
* DATE			: 20130218
* directory 		: 
* description		: ���� NDK_ScrFontAdd �ܷ���ȷ����Զ�������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScrFontAdd"
//�ڰ����Զ����ֿ�
#define     PCPATH      "pcc16x16.bin"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513
#define     PAPATH      "pca8x16.bin"
//�����Զ����ֿ�
#define     PCPATHCP    "hz24x24"
#define     PAPATHCP    "yw12x24"
#define     MAXWAITTIME 10//30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr22
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	    jiangym         20130224  	   created
*
*****************************************************************/
void scr22(void)
{
	/*private & local definition*/
	int ret = 0,i = 0;
	uint punWidth = 0, punHeight = 0, puncd = 0;
	char cPath[64] = {0}, aPath[64] = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	if((ret=NDK_ScrGetColorDepth(&puncd))!=NDK_OK)//��ȡ��Ļ������Ϊ�������Ե�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if(puncd!=16)
	{
		if(NDK_FsExist(PCPATH)!=NDK_OK&&NDK_FsExist(PAPATH)!=NDK_OK)
		{
			send_result("%s�ֿ��ļ�������,����POS�Ĳ��Գ���·��Ŀ¼�����Զ�������ֿ����²���", TESTAPI);
			return;
		}
		strcpy(cPath,PCPATH);//���Ժڰ����ֿ�
		strcpy(aPath,PAPATH);
	}
	else
	{
		if(NDK_FsExist(PCPATHCP)!=NDK_OK&&NDK_FsExist(PAPATHCP)!=NDK_OK)
		{
			send_result("%s�ֿ��ļ�������,����POS�Ĳ��Գ���·��Ŀ¼�����Զ�������ֿ����²���", TESTAPI);
			return;
		}
		strcpy(cPath,PCPATHCP);//���Բ������ֿ�
		strcpy(aPath,PAPATHCP);
	}
	
	//����ǰ��:	
	//case1:�����쳣:pcCpath��pcApath����ͬʱΪNULL
	if((ret=NDK_ScrFontAdd(NULL,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:pcCpath��pcApath����һ��ΪNULL������Ӧ�óɹ�
	if((ret=NDK_ScrFontAdd(NULL,aPath))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrFontAdd(cPath,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
			
	//case3:����û��Զ������壬ͬʱ�����û��Զ�������,����ȡ����Ŀ�͸�:�Զ���ģʽ�ݲ�֧��,���ö���ģʽ֧��
	if((ret=NDK_ScrFontAdd(cPath,aPath))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetFontType(DISPFONT_USER))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(puncd!=16)
	{
		//�ڰ���Ԥ�ڽ��
		if(punWidth != 16 || punHeight != 16)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
			GOTOERR;
		}
		if(cls_show_msg1(MAXWAITTIME, "��ʾ��չ����,�����Ƿ�Ϊ16X16:[ȷ��]��,[����]��")!=ENTER&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	else
	{
		//����Ԥ�ڽ��
		if(punWidth != 24 || punHeight != 24)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
			GOTOERR;
		}
		if(cls_show_msg1(MAXWAITTIME, "��ʾ��չ����,�����Ƿ�Ϊ24X24:[ȷ��]��,[����]��")!=ENTER&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}	
	
	//case4:��������û��Զ�����������޸ĳ�Ĭ������,������ʾӦȫΪĬ������
	if((ret=NDK_ScrSetFontType(DISPFONT_CUSTOM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=0;i<4;i++)  
	{
		if(puncd!=16)
		{
			if(cls_show_msg1(MAXWAITTIME, "��%d����ʾĬ������,�����Ƿ�Ϊ12X12[ȷ��]��,[����]��",i+1)!=ENTER&&auto_flag!=1)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
		}
		else
		{
			if(cls_show_msg1(MAXWAITTIME, "��%d����ʾĬ������,�����Ƿ�Ϊ16X16[ȷ��]��,[����]��",i+1)!=ENTER&&auto_flag!=1)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
		}
	}

	send_result("%s����ͨ��", TESTAPI);
	//�ָ�Ĭ������
	goto ERR;
ERR:
	NDK_ScrSetFontType(DISPFONT_CUSTOM);
	return;
}

