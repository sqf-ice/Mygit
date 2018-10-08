/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs9.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsRename�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  TESTAPI		"NDK_FsRename"
#if !K21_ENABLE || OVERSEAS_ENABLE || defined SP610
#define NAMECZ1    "nameCZ1"
#define NAMECZ2    "nameCZ2"
#define NAMENEW1   "nameNEW1"
#define NAMENEW2   "nameNEW2"
#define INPUTHZ    "123�Ї�����"//SP610 Ҳֻ֧��12�ֽ�
#else //ME31ֻ֧�־���·��
#define NAMECZ1    "/appfs/nameCZ1"
#define NAMECZ2    "/appfs/nameCZ2"
#define NAMENEW1   "/appfs/nameNEW1"
#define NAMENEW2   "/appfs/nameNEW2"
#define INPUTHZ    "/appfs/123�Ї�����" //K21ƽ̨�ļ���ֻ֧�����12���ֽ� // "123�Ї����Ї�"
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���11������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
void fs9(void)
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
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_FsDel(NAMECZ1);
	NDK_FsDel(NAMECZ2);
	NDK_FsDel(NAMENEW1);
	NDK_FsDel(NAMENEW2);
	if((fp=NDK_FsOpen(NAMECZ1, "w"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x11, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��"w"��ʽ��һ���ض��ļ�(nameCZ2),��д��200�ֽ�����:0x12
	if((fp=NDK_FsOpen(NAMECZ2, "w"))<0) // �����ļ�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x12, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	/**********************���϶����Ǵ������������ļ�**************************************************/
	//case1:��һ���Ѵ��ڵ��ļ����Լ�������Ϊ�Լ�,Ӧ�ɹ�
	if((ret=NDK_FsRename(NAMECZ1, NAMECZ1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2: ����NDK_FsRename���ļ�nameCZ1������ΪnameCZ2(Ŀ��������,��������Ӧ��ʧ��)
#if 0
	if ((ret=NDK_FsRename("nameCZ1", "nameCZ2"))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case3: ɾ���ļ�nameCZ1,����Դ�ļ������ڵ����
	if((ret=NDK_FsDel(NAMECZ1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsRename(NAMECZ1, "123"))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if  !(defined ME30MH ||defined ME30GX ||defined ME50NX ||defined ME68||defined ME32||defined ME65||defined ME32GX) //ME30MH �����ж϶���������ͬ�ͻ᷵�سɹ�
	if((ret=NDK_FsRename(NAMECZ1, NAMECZ1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case4:���Ը�����Ŀ�������Ϸ������(Դ�ļ��Ϸ�Ҳ����)
#if CPU5810X_ENABLE//֣������SP610�ײ��������"appfs/"���Ͷ�ƽ̨"appfs//"�ǺϷ��ļ���,��SP610�����Ը����� 
	//5810xƽ̨ͳһ����-4 �����ν��� 20180426
	if((ret=NDK_FsRename(NAMECZ2, "/"))!=NDK_ERR_OPEN_DEV)
#else
	if((ret=NDK_FsRename(NAMECZ2, "/"))!=NDK_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4: ����NDK_FsRename�����ļ�nameCZ2ΪnameNEW1(�Ϸ�Ŀ����������,��������Ӧ�óɹ�)
	if((ret=NDK_FsRename(NAMECZ2, NAMENEW1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5: ɾ���ļ�nameCZ2(�ļ��ѱ�����ΪnameNEW1,ɾ������Ӧ��ʧ��)
	if((ret=NDK_FsDel(NAMECZ2))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6: ��"r"��ʽ��nameNEW1�ļ�(��������ļ�,����Ӧ�óɹ�)
	if((fp=NDK_FsOpen(NAMENEW1, "r"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	//YAFFS�ļ�ϵͳ����Դ򿪵��ļ����и���������Ӧ�óɹ�
	if((ret=NDK_FsRename(NAMENEW1, NAMENEW2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7: ��"r"��ʽ��nameNEW2,����֤�����ɹ�,��ȡ200�ֽ����ݲ�У��(ӦΪ:0x12)
	if((fp=NDK_FsOpen(NAMENEW2, "r"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(readbuf, 0, sizeof(readbuf));
	if((readlen=NDK_FsRead(fp, readbuf, 200))!=200)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;
	}

	if(memcmp(writebuf, readbuf, 200))
	{
		send_result("line %d: У�����ݳ���", __LINE__);
		GOTOERR;
	}
	NDK_FsClose(fp);
	//����Ϊ�Ϸ�������(��/��)
	if((ret=NDK_FsRename(NAMENEW2, INPUTHZ))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsRename(INPUTHZ, NAMENEW2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsDel(NAMENEW2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8:Դ�ļ������Ϸ�
#if !CPU5810X_ENABLE/*defined SP610*/ //5810xƽ̨ͳһ�Ƿ���-4���ν��� 20180426
	if((ret=NDK_FsRename("/", "123"))!=NDK_ERR)
#else
	if((ret=NDK_FsRename("/", "123"))!=NDK_ERR_OPEN_DEV)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:�쳣����
	if((ret=NDK_FsRename(NULL, NAMECZ1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_FsRename(NAMECZ1, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(NAMECZ1);
	NDK_FsDel(NAMECZ2);
	NDK_FsDel(NAMENEW1);
	NDK_FsDel(NAMENEW2);
	return;
}

