/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name			: spfs12.c
* Author 			: wangk
* version			: 
* DATE				: 20180829
* directory 		: 
* description		: ����NDK_SP_FsFileSize�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI		"NDK_SP_FsFileSize"
#define FILETEST14	"/appfs/FT14"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���18������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                          wangk          20180829        created
*****************************************************************/
void spfs12(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, ret=0;
	uint punSize=0;
	
	char writebuf[200]={0}/*, readbuf[200] = {0}*/;

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_SP_FsDel(FILETEST14);

	//case1: ��"w"��ʽ��һ���ض��ļ�������filelengthȡ�ļ�����(Ӧ�÷���0)
	if((fp=NDK_SP_FsOpen(FILETEST14, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�رո��ļ�������"w"��ʽ�򿪣���д��200�ֽ�����0x18,����NDK_SP_FsFileSize�����ļ���С��Ӧ�ܵõ���Ӧ�Ĵ�С200
	if((fp=NDK_SP_FsOpen(FILETEST14, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x18, sizeof(writebuf));
	if((writelen=NDK_SP_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}

	//case3:����fseek���ļ�ָ���Ƶ�100λ��,����NDK_SP_FsFileSizeȡ���ļ��ĳ���(Ӧ�÷���200)
	if((ret=NDK_SP_FsSeek(fp, 100l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�ļ��رպ��ȡ�ļ���СӦ�ñ��ֲ���
	if((ret=NDK_SP_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}

	//case5:��"r"��ʽ�򿪸��ļ�,����NDK_SP_FsFileSizeȡ���ļ��ĳ���(Ӧ�÷���200)
	if((fp=NDK_SP_FsOpen(FILETEST14, "r"))==NDK_ERR_OPEN_DEV)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}


	//case7:�쳣����
	if((ret=NDK_SP_FsFileSize(NULL, &punSize))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_SP_FsFileSize(FILETEST14, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Ժ���
	if((ret=NDK_SP_FsDel(FILETEST14))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST14);
	return;
}