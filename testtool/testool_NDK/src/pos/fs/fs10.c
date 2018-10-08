/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs10.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsTruncate�ӿ��ļ��ض̹���
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if !K21_ENABLE || OVERSEAS_ENABLE || defined SP610
#define  FILETEST10 "FT10"
#define  FILETESTA  "FTA" 
#define  FILETESTB  "FTB"
#else
#define  FILETEST10 "/appfs/FT10"
#define  FILETESTA  "/appfs/FTA"
#define  FILETESTB  "/appfs/FTB"
#endif
#define 	TESTAPI		"NDK_FsTruncate"

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
*
*****************************************************************/
void fs10(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, ret=0, fp1=0, writelen1=0,readlen1=0;
	char writebuf[200]={0},writebuf1[200]={0},readbuf1[200]={0};
	uint punSize=0;
#if !K21_ENABLE
	int readlen = 0;
	char readbuf[200]={0};
#endif

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ�ã������������ļ�������200������0x12
	NDK_FsDel(FILETEST10);
	if((fp=NDK_FsOpen(FILETEST10, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x12, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}

	//case1: ���������ļ����ȵ����ļ��Լ�ԭ������200�����������Ӧ�óɹ�
	if((ret=NDK_FsTruncate(FILETEST10, 200))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}

	//case2: �������ø��ļ��ĳ���Ϊ100(����Ӧ�óɹ�),�ٻ�ȡ�ļ�����Ӧ��Ӧ�ı�Ϊ100
	if((ret=NDK_FsTruncate(FILETEST10, 100))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=100)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
#if !K21_ENABLE
	//case3:�������ø��ļ��ĳ���Ϊ200����100(����Ӧ�óɹ�),�ٻ�ȡ�ļ�����Ӧ��Ӧ�ı�Ϊ200���һᲹ��0xff
	if((ret=NDK_FsTruncate(FILETEST10, 200))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��֤����0xff
	if((fp=NDK_FsOpen(FILETEST10, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x12, sizeof(writebuf));//����ǰ100�ֽ�0x12��100�ֽ�0xff
	memset(writebuf+100, 0xff, 100);
	memset(readbuf, 0, sizeof(readbuf));
	if((readlen=NDK_FsRead(fp, readbuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;
	}
	if(memcmp(writebuf, readbuf, 200))
	{
		send_result("line %d:У�����ݳ���", __LINE__);
		GOTOERR;
	}
#endif
	//case4: ����NDK_FsTruncate���ø��ļ��ĳ���Ϊ0(�ļ�)
	if((ret=NDK_FsTruncate(FILETEST10, 0l))!=NDK_OK) //GP730��E80�������óɹ� �ļ����ȱ�Ϊ0
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST10, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Ժ���
	if((ret=NDK_FsDel(FILETEST10))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:��һ�������ڵ��ļ���NDK_FsTruncate,Ӧʧ�ܷ���
	if((ret=NDK_FsTruncate(FILETEST10, 200l))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�ļ�·��ΪNULL
	if((ret=NDK_FsTruncate(NULL, 200l))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:��һ�����Ϸ��ļ������ļ�����
#if CPU5810X_ENABLE/*defined SP610*///5810ƽ̨ͳһ����-4 �����ν��� 20180427 modify
	if((ret=NDK_FsTruncate("/", 200l))!=NDK_ERR_OPEN_DEV)
/*#elif defined ME50S //�������50s��/��ֻ�ɶ���ԭ�����ǿɶ�д������ֵ��һ����
	if((ret=NDK_FsTruncate("/", 200l))!=NDK_ERR_PATH)*///50SҲ�Ƿ���-1
#else
	if((ret=NDK_FsTruncate("/", 200l))!=NDK_ERR)
#endif
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case8:��������:����A �ļ�,��Aд���ݺ���нض�,Ȼ�󴴽�B�ļ�,��B�ļ�д����,ͬʱ��A�ļ�Ҳд����,Ȼ���B�ļ����ݽ��бȽ�Ӧ����д�������һ��(��sp610���ֹ�B�ļ��ᱻ�޸ĵ�����Ȼ�������˲��Ե�) sull add 20180207
	if((fp = NDK_FsOpen(FILETESTA, "w")) <0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x13, sizeof(writebuf));
	if((writelen = NDK_FsWrite(fp, writebuf, 200)) != 200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsTruncate(FILETESTA, 0l))!=NDK_OK) 
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETESTA, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((fp1 = NDK_FsOpen(FILETESTB, "w")) <0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp1);
		GOTOERR;
	}
	memset(writebuf1, 0x11, sizeof(writebuf1));
	if((writelen1=NDK_FsWrite(fp1, writebuf1, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen1);
		GOTOERR;
	}
	memset(writebuf, 0x12, sizeof(writebuf));
	if((writelen = NDK_FsWrite(fp, writebuf, 200)) != 200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsSeek(fp1, 0l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(readbuf1, 0, sizeof(readbuf1));
	if((readlen1=NDK_FsRead(fp1, readbuf1, 200))!=200)
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
	NDK_FsClose(fp);
	NDK_FsClose(fp1);
	NDK_FsDel(FILETEST10);
	NDK_FsDel(FILETESTA);
	NDK_FsDel(FILETESTB);
	return;
}

