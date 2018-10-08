/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name			: spfs10.c
* Author 			: wangk
* version			: 
* DATE				: 20180829
* directory 		: 
* description		: ����NDK_SP_FsFormat�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SP_FsFormat" 
#define 	FILETEST111 	"/appfs/FT111"
#define 	FILETEST112		"/appfs/FT112"



/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���N������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                          wangk          20180829        created
*****************************************************************/
void spfs10(void)
{
	/*private & local definition*/
	int fp=0, fp1=0, writelen=0, ret=0;
	char writebuf[200]={0};

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������
	
	if(cls_show_msg("�״ν���˰���,��[Enter]����,�ٴν���ʱ��[����]")==ENTER)
	{
		//����ǰ��
		NDK_SP_FsDel(FILETEST111);
		NDK_SP_FsDel(FILETEST112);

		//case1:��������format���е����̸�ʽ��,��ʽ��������ļ�Ӧ�ö�ʧ
		//step1:���������ļ�����д��һ������
		cls_printf("���ڴ��������ļ�...");
		if((fp=NDK_SP_FsOpen(FILETEST111, "w"))<0)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
			GOTOERR2;
		}
		memset(writebuf, 0x05, sizeof(writebuf));
		if((writelen=NDK_SP_FsWrite(fp, writebuf, 200))!=200)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
			GOTOERR2;
		}
		if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR2;
		}
		
		if((fp1=NDK_SP_FsOpen(FILETEST112, "w"))<0)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp1);
			GOTOERR;
		}
		memset(writebuf, 0x06, sizeof(writebuf));
		if((writelen=NDK_SP_FsWrite(fp1, writebuf, 200))!=200)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
			GOTOERR;
		}
		if((ret=NDK_SP_FsClose(fp1))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//step2:����NDK_SP_FsFormat�������и�ʽ��
		cls_show_msg("��ʽ�����豸���Զ�����,��Ҫ�ٽ���˰�����֤,���������");
		cls_printf("���ڸ�ʽ��������...");
		if((ret=NDK_SP_FsFormat())!=NDK_OK)
		{
			send_result("line %d:��ʽ��������ʧ��(ret=%d)", __LINE__, ret);
			GOTOERR;
		}
	}

	//step3:��ʽ��������ļ�Ӧ�ò�����
	cls_printf("\n��ʽ�������̺���֤�ļ�...");
	if(NDK_SP_FsExist(FILETEST111)==NDK_OK||NDK_SP_FsExist(FILETEST112)==NDK_OK)
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_SP_FsClose(fp1);
	NDK_SP_FsDel(FILETEST112);
	goto ERR2;
ERR2:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST111);
	return;
}