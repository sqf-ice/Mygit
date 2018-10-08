/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs4.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsWrite�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if !K21_ENABLE || OVERSEAS_ENABLE || defined SP610
#define  FILETEST4 "FT4"
#else	//K21ƽֻ̨֧�־���·��
#define  FILETEST4 "/appfs/FT4"
#endif
#define	TESTAPI		"NDK_FsWrite"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���4������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
void fs4(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, readlen=0, ret=0, punSize=0;
	char writebuf[200]={0}, readbuf[200]={0};
#if CPU5810X_ENABLE//�Ͷ�30\32�ڴ治�����ᵼ�»�������,�ж����ۺϲ����в�,ac35ƽ̨���ֻ��д20K,�ʴ˴�ֻ���Ӷ�5810xƽ̨��Ʒ����20180515 modify
	char writebuf1[21504]={0}, readbuf1[21504]={0};
#endif
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_FsDel(FILETEST4);

	//case1: ��"w"��ʽ��һ���ض��ļ�,��д��200�ֽ�����0x41����д0�����ݽ�ȥ����д200�����ݣ�Ӧ�óɹ����ء�
	if((fp=NDK_FsOpen(FILETEST4, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf, 0x41, sizeof(writebuf));
	if((writelen=NDK_FsWrite(fp, writebuf, 0))!=0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=200)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}

	//case2: �������еĳ�������Ϊ-1����д�����
	if((writelen=NDK_FsWrite(fp, writebuf, -1))!=NDK_ERR_WRITE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:дһ���Ѿ��رյ��ļ������Ӧ��ʧ�ܷ���
	if((ret=NDK_FsWrite(fp, writebuf, 200))!=NDK_ERR_WRITE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4: ��"r"��ʽ�򿪸��ļ�,д��200�ֽ�����0x42(Ӧ�ò���д��),ʧ�ܷ���
	memset(writebuf, 0x42, sizeof(writebuf));
	if((fp=NDK_FsOpen(FILETEST4, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((writelen=NDK_FsWrite(fp, writebuf, 200))!=NDK_ERR_WRITE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5: ��"r"��ʽ���ļ������Լ��飬У�����ݣ�ǰ����д������Ӧ��������������Ӧһ�¡�
	if((fp=NDK_FsOpen(FILETEST4, "r"))<0)
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
	memset(writebuf, 0x41, sizeof(writebuf));
	if(memcmp(writebuf, readbuf, 200))
	{
		send_result("line %d: У�����ݳ���", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�쳣����
	if((fp=NDK_FsOpen(FILETEST4, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((writelen=NDK_FsWrite(fp, NULL, 200))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}	
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsDel(FILETEST4))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if CPU5810X_ENABLE
    //case7:����һ���Կ���д��21K������20180502 linying added
    if((fp=NDK_FsOpen(FILETEST4, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	memset(writebuf1, 0x13, sizeof(writebuf1));
	if((writelen=NDK_FsWrite(fp, writebuf1, 21504))!=21504)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}
	NDK_FsClose(fp);
	if ((fp=NDK_FsOpen(FILETEST4, "r"))<0) 
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if ((ret=NDK_FsFileSize(FILETEST4,&punSize))!=NDK_OK||punSize!=21504)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,punSize);
		GOTOERR;
	}
	memset(readbuf1, 0, sizeof(readbuf1));
	if((readlen=NDK_FsRead(fp,readbuf1, 21504))!=21504)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;
	}
	else
	{
		if (memcmp(writebuf1, readbuf1,21504)) // У��д����������ݵ�����
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}		
	}
#endif
	send_result("%s����ͨ��", TESTAPI);

ERR:
	NDK_FsClose(fp);
	NDK_FsDel(FILETEST4);
	return;
}

