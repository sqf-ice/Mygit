/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs17.c
* Author 			: chensj
* version			: 
* DATE			: 20140218
* directory 		: 
* description		: �����ļ�������ȶ�����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if defined SP610
#define  FILETEST17_1 "FT17+11111"
#define  FILETEST17_2 "FT17+2222222" //�ļ�������Ϊ12
#define  FILETEST17_3 "FT17+33333333"  //�ļ�������Ϊ13
#else
#define  FILETEST17_1 "/appfs/FT17+11111"
#define  FILETEST17_2 "/appfs/FT17+2222222" //�ļ�������Ϊ12
#define  FILETEST17_3 "/appfs/FT17+33333333"  //�ļ�������Ϊ13
#endif
#define	TESTAPI		"�ļ�������ȶ�����"
#define	MAXFD	5

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
*					chensj			20140218	created
*****************************************************************/
void fs17(void)
{
	/*private & local definition*/
	int fp = 0,writelen = 0,readlen = 0;
	char writebuf[101]={0},readbuf[101]={0};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("%s������...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	memset(writebuf,'a',sizeof(writebuf)-1);

	//case 1:�������ļ�������Ϊ10���ļ���Ӧʧ��
	if ((fp=NDK_FsOpen(FILETEST17_1, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((writelen=NDK_FsWrite(fp,writebuf, 100))!=100)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	NDK_FsClose(fp);
	if ((fp=NDK_FsOpen(FILETEST17_1, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(readbuf,0,sizeof(readbuf));
	NDK_FsSeek(fp, 0, SEEK_SET);
	if((readlen=NDK_FsRead(fp,readbuf, 100))!=100)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;		
	}
	if(strcmp(writebuf,readbuf))
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, strlen(writebuf), strlen(readbuf));
		GOTOERR;	
	}
	NDK_FsClose(fp);

	//case 2:�������ļ�������Ϊ12���ļ���Ӧʧ��
	if ((fp=NDK_FsOpen(FILETEST17_2, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((writelen=NDK_FsWrite(fp,writebuf, 100))!=100)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	NDK_FsClose(fp);
	if ((fp=NDK_FsOpen(FILETEST17_2, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(readbuf,0,sizeof(readbuf));
	NDK_FsSeek(fp, 0, SEEK_SET);
	if((readlen=NDK_FsRead(fp,readbuf, 100))!=100)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;		
	}
	if(strcmp(writebuf,readbuf))
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, strlen(writebuf), strlen(readbuf));
		GOTOERR;	
	}
	NDK_FsClose(fp);

	//case 3:�������ļ�������Ϊ����12���ļ���ʧ��
	if ((fp=NDK_FsOpen(FILETEST17_3, "w"))>=0) 
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		goto ERR;
	}
	NDK_FsClose(fp);
	
	send_result("%s����ͨ��", TESTAPI); // ����ͨ��
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(FILETEST17_1);
	NDK_FsDel(FILETEST17_2);
	NDK_FsDel(FILETEST17_3);
	return;
}

