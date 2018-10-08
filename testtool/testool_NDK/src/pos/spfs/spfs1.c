/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name		: spfs1.c
* Author 			: linying
* version			: 
* DATE			: 20180828
* directory 		: 
* description		: ����NDK_SP_FsOpen�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/

#define  FILETEST1	"/appfs/fsys1"  
#define  APPFSFILE	"/appfs/appfsfsys"	//����·��
#define	 NONLICET	"/"
#define 	TESTAPI		"NDK_SP_FsOpen"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �߶��ļ�ϵͳģ���1������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                                 linying              20180828       created
*****************************************************************/
void spfs1(void)
{
	/*private & local definition*/
	int fp = -1, fp2 = -1;
	int writelen = 0, readlen = 0, ret = 0;
	uint punSize=0;
	char writebuf[210] = {0};
	char readbuf[210] = {0};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��
	NDK_SP_FsDel(FILETEST1);
	NDK_SP_FsDel(APPFSFILE);
	
	//case1:��ֻ���ķ�ʽ�򿪲����ڵ��ļ�
	if ((fp=NDK_SP_FsOpen(FILETEST1, "r"))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}

	//case2:���������ļ���д���ļ����ر��ļ���ɾ���ļ�
	if ((fp=NDK_SP_FsOpen(FILETEST1, "w"))<0) // �ر��ļ�����д���ļ��������ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST1);
	if ((fp=NDK_SP_FsOpen(FILETEST1, "w"))<0) // ��д���ļ��򿪸ո�ɾ�����ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x55, 200);
	if((writelen=NDK_SP_FsWrite(fp,writebuf, 200))!=200) // д��һ������(0x55)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	NDK_SP_FsClose(fp);
	if ((fp=NDK_SP_FsOpen(FILETEST1, "w"))<0) // ��д���ļ��򿪸ոմ������ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x88, 100);
	if((writelen=NDK_SP_FsWrite(fp,writebuf, 100))!=100) // ����д������(0x88)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	NDK_SP_FsClose(fp);
	if ((fp=NDK_SP_FsOpen(FILETEST1, "r"))<0) // ����Ƿ��ܴ򿪳������й����д������ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}

	//zhangnw20110420add ��NL��fopen("w")���ԵĲ���:fopen("w")�������½��ļ�
	if ((ret=NDK_SP_FsFileSize(FILETEST1,&punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,punSize);
		GOTOERR;
	}
	memset(readbuf, 0, sizeof(readbuf));
	if((readlen=NDK_SP_FsRead(fp,readbuf, 200))!=200)//�˴���ʵҲ��֤��fopen("w")�������½��ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;
	}
	else
	{
		if (memcmp(writebuf, readbuf, 200)) // У��д����������ݵ�����
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}		
	}
	NDK_SP_FsClose(fp);
	
	//case4:��2�������Ƿ������
	if ((fp=NDK_SP_FsOpen(FILETEST1, "abc"))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if ((fp=NDK_SP_FsOpen(FILETEST1, NULL))!=NDK_ERR_PARA)
	{	
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
		
	//case5:���Դ�һ�����Ϸ��ļ������ļ�
	if ((fp=NDK_SP_FsOpen(NONLICET, "w"))!=NDK_ERR_OPEN_DEV)
	{	
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		NDK_SP_FsClose(fp); //Ŀǰ���ִ�"/"�ļ���ɹ�����رոþ�������������Ի���Ӱ�� 20160729 linwl
		GOTOERR;
	}
	if ((fp=NDK_SP_FsOpen(NULL, "w"))!=NDK_ERR_PARA)
	{	
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if ((ret=NDK_SP_FsDel(FILETEST1))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:���Ծ���·���ļ�������
	if ((fp2=NDK_SP_FsOpen(APPFSFILE, "w"))<0)
	{	
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp2);
		GOTOERR2;
	}
	memset(writebuf, 0x88, 100);
	if((writelen=NDK_SP_FsWrite(fp2,writebuf, 100))!=100) // ����д������(0x88)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR2;
	}
	NDK_SP_FsClose(fp2);
	if ((fp2=NDK_SP_FsOpen(APPFSFILE, "r"))<0) // ����Ƿ��ܴ򿪳������й����д������ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp2);
		GOTOERR2;
	}

	if ((ret=NDK_SP_FsFileSize(APPFSFILE,&punSize))!=NDK_OK||punSize!=100)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,punSize);
		GOTOERR2;
	}
	memset(readbuf, 0, sizeof(readbuf));
	if((readlen=NDK_SP_FsRead(fp2,readbuf, 100))!=100)//�˴���ʵҲ��֤��fopen("w")�������½��ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR2;	
	}
	else
	{
		if (memcmp(writebuf, readbuf, 100)) // У��д����������ݵ�����
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			GOTOERR2;
		}		
	}
	NDK_SP_FsClose(fp2);
	if ((ret=NDK_SP_FsDel(APPFSFILE))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}

	//case7:����������ͬһ���ļ�,�����򿪵��ļ����Ӧ�ö�����Ч
	if ((fp=NDK_SP_FsOpen(FILETEST1, "w"))<0)//����������ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((writelen=NDK_SP_FsWrite(fp,"0123456789", 10))!=10) // д��һ������"0123456789"
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;	
	}
	NDK_SP_FsClose(fp);
	
	if ((fp=NDK_SP_FsOpen(FILETEST1, "r"))<0)//��һ�δ��ļ�,���fp
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(readbuf,0,sizeof(readbuf));
	if((readlen=NDK_SP_FsRead(fp,readbuf, 5))!=5)//��ȡ5���ֽ�����
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;		
	}
	if (memcmp(readbuf, "01234", 5)) //У��д����������ݵ�����
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	if ((fp2=NDK_SP_FsOpen(FILETEST1, "r"))<0)//�ڶ��δ��ļ�,���fp2
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(readbuf,0,sizeof(readbuf));
	if((readlen=NDK_SP_FsRead(fp2,readbuf, 10))!=10)//��ȡ10���ֽ�����
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;		
	}
	if (memcmp(readbuf, "0123456789", 10)) //У��д����������ݵ�����
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp2))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	//return;//���Խ�������л������� 20171213 modify
ERR://��������
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST1);
	NDK_SP_FsDel(NONLICET);
	//return;
	goto ERR2;
ERR2:
	NDK_SP_FsClose(fp2);
	NDK_SP_FsDel(APPFSFILE);
	return;
}

