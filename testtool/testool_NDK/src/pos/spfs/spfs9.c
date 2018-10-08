/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name			: spfs9.c
* Author 			: wangk
* version			: 
* DATE				: 20180829
* directory 		: 
* description		: ����NDK_SP_FsTruncate�ӿ��ļ��ض̹���
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SP_FsTruncate"
#define  FILETEST10 "/appfs/FT10"
#define  FILETESTA  "/appfs/FTA"
#define  FILETESTB  "/appfs/FTB"


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
void spfs9(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, ret=0, fp1=0, writelen1=0,readlen1=0;
	char writebuf[200]={0},writebuf1[200]={0},readbuf1[200]={0};
	uint punSize=0;
	

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ�ã������������ļ�������200������0x12
	NDK_SP_FsDel(FILETEST10);
	if((fp=NDK_SP_FsOpen(FILETEST10, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x12, sizeof(writebuf));
	if((writelen=NDK_SP_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}

	//case1: ���������ļ����ȵ����ļ��Լ�ԭ������200�����������Ӧ�óɹ�
	if((ret=NDK_SP_FsTruncate(FILETEST10, 200))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}

	//case2: �������ø��ļ��ĳ���Ϊ100(����Ӧ�óɹ�),�ٻ�ȡ�ļ�����Ӧ��Ӧ�ı�Ϊ100
	if((ret=NDK_SP_FsTruncate(FILETEST10, 100))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=100)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	
	//case4: ����NDK_SP_FsTruncate���ø��ļ��ĳ���Ϊ0(�ļ�)
	if((ret=NDK_SP_FsTruncate(FILETEST10, 0l))!=NDK_OK) //GP730��E80�������óɹ� �ļ����ȱ�Ϊ0
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Ժ���
	if((ret=NDK_SP_FsDel(FILETEST10))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:��һ�������ڵ��ļ���NDK_SP_FsTruncate,Ӧʧ�ܷ���
	if((ret=NDK_SP_FsTruncate(FILETEST10, 200l))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�ļ�·��ΪNULL
	if((ret=NDK_SP_FsTruncate(NULL, 200l))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:��һ�����Ϸ��ļ������ļ�����
	if((ret=NDK_SP_FsTruncate("/", 200l))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case8:��������:����A �ļ�,��Aд���ݺ���нض�,Ȼ�󴴽�B�ļ�,��B�ļ�д����,ͬʱ��A�ļ�Ҳд����,Ȼ���B�ļ����ݽ��бȽ�Ӧ����д�������һ��(��sp610���ֹ�B�ļ��ᱻ�޸ĵ�����Ȼ�������˲��Ե�) sull add 20180207
	if((fp = NDK_SP_FsOpen(FILETESTA, "w")) <0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x13, sizeof(writebuf));
	if((writelen = NDK_SP_FsWrite(fp, writebuf, 200)) != 200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_SP_FsTruncate(FILETESTA, 0l))!=NDK_OK) 
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsFileSize(FILETESTA, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((fp1 = NDK_SP_FsOpen(FILETESTB, "w")) <0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp1);
		GOTOERR;
	}
	memset(writebuf1, 0x11, sizeof(writebuf1));
	if((writelen1=NDK_SP_FsWrite(fp1, writebuf1, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen1);
		GOTOERR;
	}
	memset(writebuf, 0x12, sizeof(writebuf));
	if((writelen = NDK_SP_FsWrite(fp, writebuf, 200)) != 200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_SP_FsSeek(fp1, 0l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(readbuf1, 0, sizeof(readbuf1));
	if((readlen1=NDK_SP_FsRead(fp1, readbuf1, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen1);
		GOTOERR;
	}
	if((readlen1 != writelen1)|| (memcmp(writebuf1, readbuf1, readlen1)))
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, writelen1,readlen1);
		goto ERR;
	}
	
	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsClose(fp1);
	NDK_SP_FsDel(FILETEST10);
	NDK_SP_FsDel(FILETESTA);
	NDK_SP_FsDel(FILETESTB);
	return;
}