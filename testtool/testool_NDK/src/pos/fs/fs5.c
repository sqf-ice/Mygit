/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs5.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsDel�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if !K21_ENABLE || OVERSEAS_ENABLE || defined SP610
#define  FILETEST5 "FT5"
#else	//K21ƽֻ̨֧�־���·��
#define  FILETEST5 "/appfs/FT5"
#endif
#define	TESTAPI		"NDK_FsDel"

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
* return value		 	 : ����fdelɾ���ļ��Ĺ���
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
void fs5(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, readlen=0, ret=0;
	char writebuf[200]={0}, readbuf[200]={0};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ��	
	NDK_FsDel(FILETEST5);
#if 0
	//case1: ��"w"��ʽ��һ���ض��ļ�,����NDK_FsDelɾ����"w"��ʽ�򿪲���δ�رյĿ��ļ���Ӧ��ʧ�ܷ��ء�
	if((fp=NDK_FsOpen(FILETEST5, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if ((ret=NDK_FsDel(FILETEST5))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if 0
	//case2:��"r"��ʽ�򿪸ÿ��ļ�,����NDK_FsDelɾ����"r"��ʽ�򿪲���δ�رյĿ��ļ���Ӧ��ʧ�ܷ��ء�
	if((fp=NDK_FsOpen(FILETEST5, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if ((ret=NDK_FsDel(FILETEST5))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if 0
	//case3: ����"w"��ʽ�򿪸��ļ���д��200�ֽ�����0x05,����fdelɾ����"w"��ʽ���Ҹ�δ�رյķǿ��ļ�(Ӧ�ó���)
	if((fp=NDK_FsOpen(FILETEST5, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x05, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if ((ret=NDK_FsDel(FILETEST5))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if 0
	//case4:��"r"���ļ�,����fdelɾ����"r"��ʽ���Ҹ�δ�رյķǿ��ļ�(Ӧ�ó���)
	if((fp=NDK_FsOpen(FILETEST5, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if ((ret=NDK_FsDel(FILETEST5))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//����ǰ��	
	NDK_FsDel(FILETEST5);
	if((fp=NDK_FsOpen(FILETEST5, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x05, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case5: ��"r"��ʽ�򿪸��ļ�����ȡ200�ֽڵ����ݲ�����У��
	if((fp=NDK_FsOpen(FILETEST5, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
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
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6: ����fdelɾ�����ļ�(ɾ�����ڵ��ļ�),��ɾ�����ļ�Ӧ�ò�������
	if((ret=NDK_FsDel(FILETEST5))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsExist(FILETEST5))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7: �ٴε���NDK_FsDelȥɾ���ڲ���6���Ѿ�ɾ���˵��ļ�(�˴�ɾ�����ǲ����ڵ��ļ�,Ӧ��ʧ��)
	if((ret=NDK_FsDel(FILETEST5))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8:����NDK_FsDelһ�����Ϸ����ļ���,Ӧ�ó���
#if CPU5810X_ENABLE//!defined SP610	//֣������SP610�ײ��������"appfs/"���Ͷ�ƽ̨"appfs//"�ǺϷ��ļ���,��SP610�����Ը�����
	//5810ƽ̨��Ʒ���Ƿ���-4(�������ME50N) �����ν��� 20180427 modify
	if((ret=NDK_FsDel("/"))!=NDK_ERR_OPEN_DEV)
#else
	if((ret=NDK_FsDel("/"))!=NDK_ERR)
#endif
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:�쳣����
	if((ret=NDK_FsDel(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	

	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(FILETEST5);
	return;
}

