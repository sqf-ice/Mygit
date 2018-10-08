/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs12.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsDir�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if OVERSEAS_ENABLE || defined SP610
#define  FILETEST12 "DIRFILE"
#else
#define  FILETEST12 "/appfs/DIRFILE"
#endif
#define 	TESTAPI		"NDK_FsDir"
#define	MAXWAITTIME	(30)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���12������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
void fs12(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int ret= -1, fp=0, i=0, nKeyin=0;
	char s[3000]={0}, discfilesdir[100][21];
	uint dirnum=0;

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	NDK_FsDel(FILETEST12);
	if((fp=NDK_FsOpen(FILETEST12, "w"))<0)
	{
		send_result("line %d:�����ļ�ʧ��", __LINE__);
		GOTOERR;
	}
	NDK_FsClose(fp);

	//case1:�쳣����:����ΪNULL���
	if((ret=NDK_FsDir(NULL, s, &dirnum))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsDir("./", NULL, &dirnum))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsDir("./", s, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����һ�������ڵ�·��,Ӧ���طǷ�·�� 20140923 jiangym add
	if((ret=NDK_FsDir("./non", s, &dirnum))!=NDK_ERR_PATH)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:������ȡ��ǰĿ¼�������ļ����������ʾ
	//ÿ20���ֽڴ洢һ���ļ��� ǰ19 Ϊ�ļ����������Զ��ض̡���20�ֽ����Ϊ1���ʾ���ļ�Ϊ�ļ��У�0Ϊ��ͨ�ļ� Ҫ��������֤
	memset(s, 0, sizeof(s));
	memset(discfilesdir, 0, sizeof(discfilesdir));
#if !(K21_ENABLE||OVERSEAS_ENABLE)
	if((ret=NDK_FsDir("/appfs", s, &dirnum))!=NDK_OK)
#else
	if((ret=NDK_FsDir("./", s, &dirnum))!=NDK_OK)
#endif
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=0;i<dirnum;i++)
	{
		memcpy(discfilesdir[i], s+i*20, 20); // �洢��ǰ�ļ�Ŀ¼
		cls_show_msg1(5, "dir�����ʾ:\n%s %-s\n", s+i*20, s[i*20+19]==1?"�ļ���":(s[i*20+19]==0?"��ͨ�ļ�":"����ʧ��:��ȡ��������ļ�����"));
	}

	//case3:��֤�������ļ�����
	for(i=0;i<dirnum;i++)
	{
#if !K21_ENABLE
		if(!memcmp(&discfilesdir[i], "DIRFILE", 7))
#elif defined SP610
		if(!memcmp(&discfilesdir[i], "/appfs/DIRFILE", 14))
#else
		if(!memcmp(&discfilesdir[i], FILETEST12, sizeof(FILETEST12)))
#endif
			break;
	}
	if(i==dirnum)
	{
		send_result("line %d:%s����ʧ��,�����ļ�������", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:��֤�ļ����͵���ȷ�ԡ�
	if(discfilesdir[i][19]!=0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, discfilesdir[i][19]);
		goto ERR;
	}
	//case5:��֤�ļ���������ȷ��
	nKeyin=cls_show_msg1(MAXWAITTIME, "����ȡ��%d���ļ����ļ���\n[ENTER]ͨ��,[����]��ͨ��", dirnum);
	if(nKeyin==0||nKeyin==ENTER)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

	//���Ժ���:ɾ���ļ�
	NDK_FsDel(FILETEST12);
	return;
ERR:
	NDK_FsDel(FILETEST12);
	return;
}

