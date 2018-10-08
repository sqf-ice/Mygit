/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs14.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsFileSize�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if !K21_ENABLE || OVERSEAS_ENABLE || defined SP610
#define  FILETEST14 "FT14"
#else //K21ƽֻ̨֧�־���·��
#define  FILETEST14 "/appfs/FT14"
#endif
#define 	TESTAPI		"NDK_FsFileSize"

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
*
*****************************************************************/
void fs14(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, ret=0;
	uint punSize=0;
#if !K21_ENABLE
	ulong pulRet=0;
#endif
	char writebuf[200]={0}/*, readbuf[200] = {0}*/;

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_FsDel(FILETEST14);

	//case1: ��"w"��ʽ��һ���ض��ļ�������filelengthȡ�ļ�����(Ӧ�÷���0)
	if((fp=NDK_FsOpen(FILETEST14, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�رո��ļ�������"w"��ʽ�򿪣���д��200�ֽ�����0x18,����NDK_FsFileSize�����ļ���С��Ӧ�ܵõ���Ӧ�Ĵ�С200
	if((fp=NDK_FsOpen(FILETEST14, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x18, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}

	//case3:����fseek���ļ�ָ���Ƶ�100λ��,����NDK_FsFileSizeȡ���ļ��ĳ���(Ӧ�÷���200)
	if((ret=NDK_FsSeek(fp, 100l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�ļ��رպ��ȡ�ļ���СӦ�ñ��ֲ���
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}

	//case5:��"r"��ʽ�򿪸��ļ�,����NDK_FsFileSizeȡ���ļ��ĳ���(Ӧ�÷���200)
	if((fp=NDK_FsOpen(FILETEST14, "r"))==NDK_ERR_OPEN_DEV)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if !K21_ENABLE  //K21ƽ̨��֧��NDK_FsTruncate
	//case6:����NDK_FsTruncate���ø��ļ��ĳ���Ϊ100(Ӧ�óɹ�),��"r"��ʽ�򿪸��ļ�,
	//����NDK_FsSeek���ļ�ָ���Ƶ�300λ��
	//Ȼ��ʹ��NDK_FsTellȡ���ļ�����(Ӧ�÷���300),����NDK_FsFileSizeȡ���ļ��ĳ���(ӦҲΪ300)
	if((ret=NDK_FsTruncate(FILETEST14, 100l))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((fp=NDK_FsOpen(FILETEST14, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	NDK_FsSeek(fp, 300l, SEEK_SET);
	if((ret=NDK_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=300)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=100)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:����NDK_FsTruncate���ø��ļ��ĳ���Ϊ0,��"r"��ʽ�򿪸��ļ������NDK_FsFileSizeȡ���ļ��ĳ���(Ӧ�÷���0)
	if((ret=NDK_FsTruncate(FILETEST14, 0l))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((fp=NDK_FsOpen(FILETEST14, "r"))<0) // ��"r"��ʽ�򿪸��ļ�ʧ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_FsFileSize(FILETEST14, &punSize))!=NDK_OK||punSize!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, punSize);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case7:�쳣����
	if((ret=NDK_FsFileSize(NULL, &punSize))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_FsFileSize(FILETEST14, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Ժ���
	if((ret=NDK_FsDel(FILETEST14))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(FILETEST14);
	return;
}

