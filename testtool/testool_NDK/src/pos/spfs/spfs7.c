/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name			: spfs7.c
* Author 			: wangk
* version			: 
* DATE				: 20180829
* directory 		: 
* description		: ����NDK_SP_FsTell�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SP_FsTell"
#define FILETEST7	"/appfs/FT7"


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
void spfs7(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, ret=0;
	ulong pulRet=0;
	char writebuf[200]={0};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ�ã���"w"��ʽ��һ���ض��ļ�����д��200�ֽ�����0x09
	NDK_SP_FsDel(FILETEST7);
	if((fp=NDK_SP_FsOpen(FILETEST7, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}

	memset(writebuf, 0x09, sizeof(writebuf));
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

	//case1: ��"r"��ʽ��һ���ض��ļ�,����NDK_SP_FsTellȡ�õ�ǰָ��λ��,��ǰλ��Ӧ��Ϊ0
	if((fp=NDK_SP_FsOpen(FILETEST7, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}

	//case2: �ƶ��ļ�ָ�뵽��SEEK_SET��-1��λ�ã��ٵ���NDK_SP_FsTellȡ��ǰָ��λ��
	NDK_SP_FsSeek(fp, -1l, SEEK_SET);
	if((ret=NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}

	//case3: �ƶ��ļ�ָ�뵽��SEEK_SET��200��λ�ã��ٵ���NDK_SP_FsTellȡ��ǰָ��λ��,Ӧ��200��λ��
	NDK_SP_FsSeek(fp, 200l, SEEK_SET);
	if((ret=NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}

	//case4: �ƶ��ļ�ָ�뵽��SEEK_SET��201��λ�ã��ٵ���ftellȡ��ǰָ��λ��,Ӧ��201��λ��
	NDK_SP_FsSeek(fp, 201l, SEEK_SET);
	if((ret=NDK_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=200)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}

	//case5:�쳣����
	if((ret=NDK_SP_FsTell(fp, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}	

	//case6:��һ���ѹرյĻ򲻴���ļ���NDK_SP_FsTell��Ӧ��ʧ�ܷ��ء�	
	if((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsTell(fp, &pulRet))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}

	//���Ժ���
	if((ret=NDK_SP_FsDel(FILETEST7))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_SP_FsTell(fp, &pulRet))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		goto ERR;
	}


	send_result("%s����ͨ��", TESTAPI); // ����ͨ��
	return;
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST7);
	return;
}