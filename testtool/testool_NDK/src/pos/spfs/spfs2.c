/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name		: spfs2.c
* Author 			: linying
* version			: 
* DATE			: 20180828
* directory 		: 
* description		: ����NDK_SP_FsClose�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/

#define  FILETEST2 "/appfs/FT2"
#define 	TESTAPI		"NDK_SP_FsClose"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���2������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                                 linying              20180828       created
*****************************************************************/
void spfs2(void)
{
	/*private & local definition*/
	volatile int countnum=0;
	int fp=0, writelen=0, ret=0;
	char writebuf[200]={0};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_SP_FsDel(FILETEST2); // ȷ���򿪵���һ���յĲ����ļ�

	//case1:����һ�����ļ���ɾ��һ���մ������ļ���Ӧ�óɹ����ء�
	if((fp=NDK_SP_FsOpen(FILETEST2, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��"w"��ʽ�򿪸մ������ļ���д��һ�����ݣ��رշǿ��ļ�Ӧ�óɹ����ء�
	if((fp=NDK_SP_FsOpen(FILETEST2, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x22, sizeof(writebuf)); // ��������
	if((writelen=NDK_SP_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��"r"��ʽ�򿪸մ������ļ����ر���"r"��ʽ�򿪵��ļ���Ӧ�ɹ����ء�
	if((fp=NDK_SP_FsOpen(FILETEST2, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�ر��Ѿ��رյ��ļ���Ӧʧ�ܷ��ء�
	if((ret=NDK_SP_FsClose(fp))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:�ر���������Χ�����������Ӧʧ�ܷ��ء�
	if((ret=NDK_SP_FsClose(-1))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�ر�һ��û�д򿪹��ľ����Ӧʧ�ܷ��ء�
	if((ret=NDK_SP_FsClose(113))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��������
	if((ret=NDK_SP_FsDel(FILETEST2))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if((ret=NDK_SP_FsClose(MIN_SIGNINT))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_SP_FsClose(MAX_SIGNINT))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 1//!K21_ENABLE//�Ͷ�ƽ̨������ԭ������forѭ��������BUG��Ŀǰ�޷����
	for(countnum=0, fp=MIN_SIGNINT;countnum<21;fp+=MAX_SIGNINT/10, countnum++)
	{
		if((ret=NDK_SP_FsClose(fp))!=NDK_ERR)
		{
			send_result("line %d: %s����ʧ��(%d, %d)", __LINE__, TESTAPI, fp, ret);
			goto ERR;
		}
	}
#endif
	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST2);
	return;
}

