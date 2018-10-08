/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App15.c
* Author 			:
* version			: 
* DATE			:
* directory 		: 
* description		: ���Բ������豸���Ի�����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#define  TESTAPI	  "�������豸���Ի�����"
#define  FILETEST1   "ENABLEA.NLP"  //���й�ԿA
#define  FILETEST2   "MAPPKEY.NLP"  //mapp��Ӧ�Ĺ�Կ��ҲΪ���й�Կ
//#define  FILETEST2   "ENABLEB.NLP"  //���й�ԿB
#define  FILETEST3   "DISABLE.NLP"  //�����й�Կ

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : Ӧ�ù���ģ���15������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*						sull							created
*****************************************************************/
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

void app15(void)
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
	if((cls_show_msg("�Ƿ����������1����?�ǰ���������в���,����ESC��������,��ȷ��������δ���κι�Կ�ٽ��иò���")) != ESC)
	{
		if((cls_show_msg("�Ƿ�Ϊ��һ��ִ�и��������һ�����δ��apppk?��[ENTER],��[����]")) == ENTER)
		{
			//case1:NDK_AppLoad�����еĶ��ƹ�Կ����������������Կ(���ܱ�־Ϊ0����Ϊ1�Ĺ�Կ)
			if((ret = NDK_AppLoad((char *)FILETEST3, 0)) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			//���幫ԿǰNDK_AppLoad������ԿӦ��ʧ��
			if((ret = NDK_AppLoad((char *)FILETEST1, 0)) != NDK_ERR_APP_SIGN_DECRYPT)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			//���幫ԿǰNDK_AppLoad��ͬ��ԿӦ������ʧ��
			if((ret = NDK_AppLoad((char *)FILETEST3, 0)) != NDK_ERR_APP_SIGN_DECRYPT)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			cls_show_msg("����ĸpos����Ȩ���幫Կ�����������1��������");
		}
		else
		{
			if((ret = NDK_AppLoad((char *)FILETEST1, 0)) != NDK_ERR_APP_PUBKEY_NOT_MATCH)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret = NDK_AppLoad((char *)FILETEST2, 0)) != NDK_ERR_APP_PUBKEY_NOT_MATCH)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			
			//case2:��ͬ�Ĳ����еĶ��ƹ�Կ�����ظ�NDK_AppLoad
			if((ret = NDK_AppLoad((char *)FILETEST3, 0)) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
		}
		cls_show_msg("������1����ͨ��,���ô����²����й�Կ,��ͨ��ĸpos����Ȩ���幫Կ�����������2����,�����������");
		NDK_SysReboot();
	}
	
	if((cls_show_msg("�Ƿ����������2����?�ǰ���������в���,����ESC��������,��ȷ��������δ���κι�Կ�ٽ��иò���")) != ESC)
	{
		if((cls_show_msg("�Ƿ�Ϊ��һ��ִ�и��������һ�����δ��apppk?��[ENTER],��[����]")) == ENTER)
		{
			//case3 :NDK_AppUpdate�����еĶ��ƹ�Կ����������������Կ(���ܱ�־Ϊ0����Ϊ1�Ĺ�Կ)
			if((ret=app_transdata(FILETEST3, 0, 0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppDataVerify())!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppUpdate(0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}	

			//���幫ԿǰNDK_AppUpdate������ԿӦ��ʧ��
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

			//���幫ԿǰNDK_AppUpdate��ͬ��ԿӦ������ʧ��
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
			cls_show_msg("����ĸpos����Ȩ���幫Կ�����������2��������");
		}
		else
		{
			if((ret=app_transdata(FILETEST1, 0, 0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppDataVerify())!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_PUBKEY_NOT_MATCH)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}	
			
			if((ret=app_transdata(FILETEST2, 0, 0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppDataVerify())!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_PUBKEY_NOT_MATCH)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}	
			
			//case4:��ͬ�Ĳ����еĶ��ƹ�Կ�����ظ�NDK_AppUpdate
			if((ret=app_transdata(FILETEST3, 0, 0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppDataVerify())!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((ret=NDK_AppUpdate(0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}

		}
		cls_show_msg("������2����ͨ��,���ô����²����й�Կ�����л��ɿ����豸�����ѿ��в�������һ��(ʹ�����ع���)�������������");
	}
	
	send_result("%s����ͨ��", TESTAPI);			
}


