/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			:    spfsģ��
* file name			:  
* Author 			: linying
* version			: 
* DATE				: 20180829
* directory 		: 
* description		: ��������NDK_CopyFileToSecMod��pad�˵�ָ���ļ�������k21��/appfs/Ŀ¼��
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_CopyFileToSecMod"	 
#define DESTFILE "/appfs/test"
#define DESTFILE1 "/appfs/test111111111"//���ִ���12���ֽ�
#define DESTFILE2 "/data/test"//����APPS�µ�
#define DESTFILE3 "/appfs/a"//��ͬ���ļ�
#define SOURCEFILE  "/mnt/sdcard/sourcefile"//"/data/sourcefile"  //�豾�ش��� //ԭ����·����Ȩ�޹��޸� 20171101
#define SOURCEFILE1 "/appfs/sourcefile" //����pad���µ�Դ�ļ����豾�ش���
#define SOURCEFILE4000   "/mnt/sdcard/sourcefile2"//"/data/sourcefile2"  //���ļ�������4000�ֽڵ�A


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
*			  	  	      linying        20180829       created
*****************************************************************/
void spfs15(void)//�޲����޷���
{
	/*private & local definition*/
	int ret=0, fp=0, i=0, writelen=0, readlen=0, size=0, size1=0;
	char writebuf[4000]={0}, readbuf[4000]={0};//�������ļ���С��ԭ����1024���ӵ�4000
	char str[]="��12345`~!\"@$#[^%]*() ��-_=+|\\&{}:;?/><.,½", str2[4000];
	
	/*process body*/
	if(auto_flag==1)
	{
		//send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);  
	NDK_SP_FsDel(DESTFILE);
	NDK_SP_FsDel(DESTFILE3);
	NDK_SP_FsDel(SOURCEFILE1);

	//��pad�˴���Դ�ļ�
	cls_show_msg("����pad��/mnt/sdcard/·���´���һ����Ϊsourcefile,����Ϊ%s��Դ�ļ�������������������...", str);
	cls_show_msg("����pad��/mnt/sdcard/·���´���һ����Ϊsourcefile2,����4000��A��Դ�ļ�������������������...");
	
	//case1:�쳣���ԣ�Ŀ���ļ������ȳ���12���ֽڣ�Ӧ����ʧ��
	if((ret=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�쳣���ԣ�Ŀ���ļ�������/appfs/·���£��򿽱�Ӧ��ʧ�ܡ�
	if((ret=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE2))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:�쳣���ԣ�Դ�ļ�������pad���У��򿽱�Ӧ��ʧ�ܡ�
	//������pad�˴���Դ�ļ�
	if((fp=NDK_SP_FsOpen(SOURCEFILE1, "w"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		RETURN;
	}
	NDK_SP_FsClose(fp);
	if((ret=NDK_CopyFileToSecMod(SOURCEFILE1, DESTFILE))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_SP_FsDel(SOURCEFILE1);
		RETURN;
	}
	NDK_SP_FsDel(SOURCEFILE1);
	
	//case4:�������ԣ�pad�е�ָ���ļ���������������k21��/appfs/Ŀ¼�£�Դ�ļ���Ӧ�ô��ڣ�
	if((ret=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//pad���µ�Դ�ļ�Ӧ����
	if(cls_show_msg("��鿴pad�˵��µ�Դ�ļ�%s�Ƿ񻹴���?����[ENTER],������[����]��", SOURCEFILE)!=ENTER)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI);
		RETURN;
	}
	//�ж������ļ������Ƿ�һ��
	if((ret=NDK_SP_FsFileSize(DESTFILE, &size))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, size);
		RETURN;
	}
	if((fp=NDK_SP_FsOpen(DESTFILE, "r"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		RETURN;
	}
	if((readlen=NDK_SP_FsRead(fp, readbuf, size))!=size)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		NDK_SP_FsClose(fp);
		NDK_SP_FsDel(DESTFILE);
		RETURN;
	}
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(DESTFILE);
	if(memcmp(str, readbuf, strlen(str)))
	{	
		send_result("line %d:%s����ʧ��(str=%s, readbuf=%s)", __LINE__, TESTAPI, str, readbuf);
		RETURN;
	}

  	//case5:�������ԣ���/appfs/�д��һ��"a"�ļ�����pad�е�ָ���ļ�������k21��/appfs/Ŀ¼��ʱ��Ŀ���ļ�ҲΪ"a"�ļ�����ԭ���ļ���������
	if((fp=NDK_SP_FsOpen(DESTFILE3, "w"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		RETURN;
	}
	for (i=0; i<sizeof(writebuf); i++)
		writebuf[i]=rand()%256;
	if((writelen=NDK_SP_FsWrite(fp, writebuf, 1024))!=1024)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		NDK_SP_FsClose(fp);
		NDK_SP_FsDel(DESTFILE3);
		RETURN;
	}
	NDK_SP_FsClose(fp);
	if((ret=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_SP_FsDel(DESTFILE3);
		RETURN;
	}
	if((ret=NDK_SP_FsFileSize(DESTFILE3, &size1))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, size1);
		NDK_SP_FsDel(DESTFILE3);
		RETURN;
	}
	NDK_SP_FsDel(DESTFILE3);
 	if(size1!=size)
 	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, size, size1);
		RETURN;
 	}

	//case6:�������ԣ�pad�е�ָ���ļ���������������k21��/appfs/Ŀ¼�£�Դ�ļ���Ӧ�ô��ڣ�,���ļ�����4000�ֽ�
	if((ret=NDK_CopyFileToSecMod(SOURCEFILE4000, DESTFILE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�ж������ļ������Ƿ�һ��
	if((ret=NDK_SP_FsFileSize(DESTFILE, &size))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, size);
		RETURN;
	}
	if((fp=NDK_SP_FsOpen(DESTFILE, "r"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		RETURN;
	}
	if((readlen=NDK_SP_FsRead(fp, readbuf, size))!=sizeof(str2))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		NDK_SP_FsClose(fp);
		NDK_SP_FsDel(DESTFILE);
		RETURN;
	}
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(DESTFILE);
	memset(str2,'A',sizeof(str2));
	if(memcmp(str2, readbuf, sizeof(str2)))
	{	
		send_result("line %d:%s����ʧ��,��Ƚ������ļ�", __LINE__, TESTAPI);
		RETURN;
	}

	//���Ժ���	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


