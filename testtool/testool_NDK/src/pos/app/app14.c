/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App14.c
* Author 			:
* version			: 
* DATE			:
* directory 		: 
* description		: ���Է��л�����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#define  TESTAPI	  "���л�����"
#define  FILETEST1   "ErrApppk.NLP"
#define  FILETEST2   "Apppk.NLP"
#define  FILETEST3   "Apppk2.NLP"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���10������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*						sull					20180605		created
*****************************************************************/
/**
* ��֤���л���������������:
* 1.��apppk���������ݲ���ʱNDK_AppLoad��NDK_AppUpdateֱ�ӱ�������ʹ��ϵͳ��Կ��ǩ
* 2.apppk�Ƴ�����������apppk�Ѿ�����ʱNDK_AppLoad��NDK_AppUpdate�����԰�װ�����ͻ���Կ
**ע��:����ʱ����Ҫ�Ѵ��������(�����Ҫʹ��master.key)�������ǩ��,ǩ�����Ӧ�ý������ز���
*/
static int app_transdata(const char *pszName, int offseterrflag, int buferrflag)
{
	int  ret = 0, offset=0, fp=0, tmp = 0;
	uint len = 0;
	uchar buf[1024]={0};
	if((fp=NDK_FsOpen(pszName, "r"))<0)
	{
		cls_show_msg1(5, "line %d:%s�ļ���ʧ��(%d)", __LINE__, pszName, fp);
		return NDK_ERR;
	}
	while(1)
	{
		if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
		{
			cls_show_msg1(5, "line %d:%s�ļ���ȡ����ʧ��(%d)", __LINE__, pszName, len);
			NDK_FsClose(fp);
			return NDK_ERR;
		}
		if(buferrflag==1)
		{
			tmp = rand()%(len-272)+272;
			if(buf[tmp]==0)
				buf[tmp]=0xff;
			else
				buf[tmp]=0;//�����޸ĵ�һ�ζ��������е�����һ���ֽ�Ϊ0,�����������ݴ���
			buferrflag = 0;
		}
		if((ret=NDK_AppDataSave(offset, buf, len))!=NDK_OK)
		{
			cls_show_msg1(5, "line %d:%s�ļ�App���ݴ���ʧ��(%d)", __LINE__, pszName, ret);
			NDK_FsClose(fp);
			return NDK_ERR;
		}
		if(offseterrflag!=0)
		{
			offset+=offseterrflag;//���׵�ַ�޸ĳ�offseterrflag,ʹ��Ե�ַ����ȷ�������ݱ������
			offseterrflag=0;
		}
		offset+=len;
		
		if(len<1024)
			break;
	}
	NDK_FsClose(fp);
	return NDK_OK;
}

void app14(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	if(auto_flag==1)//����ܲ�֧���Զ�����
	{
		send_result("%s��֧���Զ�����,��ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if((cls_show_msg("�Ƿ�Ϊ��һ��ִ�и������һ�����δ��apppk?��[ENTER],��[����]")) == ENTER)
	{
		//case1:�ڻ�δװ��apppk������£�Ӧ����NDK_AppLoad��NDK_Update����װ��
		if((ret = NDK_AppLoad((char *)FILETEST2, 0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//case2:��װapppk֮��NDK_AppLoad�������ظ���װ��ͬ��Կ
		if((ret = NDK_AppLoad((char *)FILETEST2, 0)) != NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//case3:��װapppk֮��NDK_AppUpdate�������ظ���װ��ͬ��Կ
		if((ret=app_transdata(FILETEST2, 0, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}	

		//case4:��װapppk֮��NDK_AppLoad�����ٰ�װ��������Կ
		if((ret = NDK_AppLoad((char *)FILETEST3, 0)) != NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//case5:��װapppk֮��NDK_AppUpdate�����ٰ�װ��������Կ
		if((ret=app_transdata(FILETEST3, 0, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}	
		
		//case6:��װapppk֮��apppk��ֵ����ʱ��NDK_AppLoadӦ��ֱ�ӱ���
		if((ret = NDK_AppLoad((char *)FILETEST1, 0)) != NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//case7:��װapppk֮��apppk��ֵ����ʱ��NDK_AppUpdateӦ��ֱ�ӱ���
		if((ret=app_transdata(FILETEST1, 0, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_DECRYPT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		send_result("%s����ͨ��(��ͨ����������apppk,���˳�Ӧ��,���´��Ӧ��)", TESTAPI);
	}
	else
	{
		cls_show_msg("��ȷ����ɾ��apppk����ִ�и�����,��������˳�����");
		return;
	}			
}

