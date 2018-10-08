/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name		: spfs3.c
* Author 			: linying
* version			: 
* DATE			: 20180828
* directory 		: 
* description		: ����NDK_SP_FsSeek�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/

#define  FILETEST3 "/appfs/FT3"
#define	TESTAPI		"NDK_SP_FsSeek"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���3������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                                  linying            20180828       created
*****************************************************************/
void spfs3(void)
{
	/*private & local definition*/
	//int countnum = 0;
	int fp=0, writelen=0, readlen=0, ret=0, seeklen=0;
	int loopcount=0;
	ulong pulRet=0;
	char writebuf[200]={0}, readbuf[200]={0};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_SP_FsDel(FILETEST3);

	//case1:����һ���յ��ļ����Կ��ļ���ָ��SEEK_CURλ��[0/1/-2]��[0/1]Ӧ�ɹ�����,[-2]Ӧʧ�ܷ��ء�
	if((fp=NDK_SP_FsOpen(FILETEST3, "w"))<0) // �����ļ�ʧ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}	
	// �Կ��ļ�������ָ��(SEEK_CUR [0/1/-2])
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_CUR))!=NDK_OK) // ���ļ���ָ��SEEK_CURλ��0,Ӧ�óɹ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SP_FsSeek(fp, 1l, SEEK_CUR))!=NDK_OK) // ���ļ���ָ��SEEK_CURλ��1,Ӧ���سɹ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��ʱλ��EOF֮�����һ���ֽ�λ��,-2l�Թ��쳬���ļ�ͷ�����
	if((ret=NDK_SP_FsSeek(fp, -2l, SEEK_CUR))!=NDK_ERR) // ���ļ���ָ��SEEK_CURλ��-2,Ӧ����ʧ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��ָ�븴λ����ͷ��д��200�ֽ�����Ϊ0x03�����ݣ�Ӧ�ɹ���
	memset(writebuf, 0x03, sizeof(writebuf));
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((writelen=NDK_SP_FsWrite(fp, writebuf, 200))!=200) // д��200�ֽ�����
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		GOTOERR;
	}

	NDK_SP_FsClose(fp);

	//case3:���ѹرյľ��������Ӧʧ�ܷ��ء�
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_SET))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��һ���ǿ��ļ����ƶ��ļ�ָ�롣
	if((fp=NDK_SP_FsOpen(FILETEST3, "w"))<0) // �����ļ�ʧ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	//case4.1:�ǿ��ļ���ָ��SEEK_CURλ��0,Ӧ�óɹ�
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_CUR))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case4.2:�ǿ��ļ���ָ��SEEK_CURλ��200,Ӧ�óɹ�
	if((ret=NDK_SP_FsSeek(fp, 200l, SEEK_CUR))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case4.3:�ǿ��ļ���ָ��SEEK_SETλ��200,Ӧ�óɹ�
	if((ret=NDK_SP_FsSeek(fp, 200l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:�Էǿ��ļ��ƶ�SEEK_SETλ��201����ȡ�ļ���λ�ã�Ӧ�÷�����ȷ��λ��201
	if((ret=NDK_SP_FsSeek(fp, 201l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=200) //ֻ�ᵽ200
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}

	//case6:�Էǿ��ļ���ָ��SEEK_SETλ��0,Ӧ�óɹ���
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:�ǿ��ļ���ָ��SEEK_SETλ��-1,Ӧ��ʧ��
	if((ret=NDK_SP_FsSeek(fp, -1l, SEEK_SET))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=0)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}


	//case8:���ƫ��,���ļ���ָ��ƫ��SEEK_SET����0��200֮��(��Ϊǰ��ֻд��200�ֽڵ�����)�������
	//��ȡ�ļ���λ�ã�Ӧ���������õ��������
	for(loopcount=0;loopcount<20;loopcount++)
	{
		seeklen=(long)(rand()%200);
		if((ret=NDK_SP_FsSeek(fp, seeklen, SEEK_SET))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=seeklen) // ƫ�ƺ�ָ��λ�ò���
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case9:�ǿ��ļ���ָ��SEEK_ENDλ��0,Ӧ�óɹ�
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_END))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=200) // ָ��λ�ô�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}


	//���������case5��Ƴ�ͻ
	//case10:�Էǿ��ļ��ƶ�SEEK_ENDλ��201����ȡ�ļ���λ�ã�Ӧ�÷�����ȷ��λ��201
	if((ret=NDK_SP_FsSeek(fp, 1l, SEEK_END))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=200)

	{
		send_result("line %d: %s����ʧ��(%d��%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}
	//case11:�ǿ��ļ���ָ��SEEK_ENDλ��-200,Ӧ�óɹ�,��ȡ�ļ���λ�ã�Ӧ������λ��Ϊ0.
	if((ret=NDK_SP_FsSeek(fp, -200l, SEEK_END))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=0)
	{
		send_result("line %d: %s����ʧ��(%d��%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}
	//case12:�ǿ��ļ���ָ��SEEK_ENDλ��-201,Ӧ��ʧ��,��ʱ��ȡ�ļ���λ��ӦΪ0��λ�á�
	if((ret=NDK_SP_FsSeek(fp, -201l, SEEK_END))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret= NDK_SP_FsTell(fp, &pulRet))!=NDK_OK||pulRet!=0)
	{
		send_result("line %d: %s����ʧ��(%d��%d)", __LINE__, TESTAPI, ret, pulRet);
		GOTOERR;
	}
	NDK_SP_FsClose(fp);

	//case13:��"r"��ʽ���ļ������ļ�ָ��SEEK_SETλ��100����ȡ�ļ�����200��ʵ�ʶ�ȡӦ����100.
	if((fp=NDK_SP_FsOpen(FILETEST3, "r"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if((ret=NDK_SP_FsSeek(fp, 100l, SEEK_SET))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((readlen=NDK_SP_FsRead(fp, readbuf, 200))!=100) // ��ȡУ������
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, readlen);
		GOTOERR;
	}

	if(memcmp(readbuf, writebuf+100, 100))
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case14:�쳣����
	if((ret=NDK_SP_FsSeek(fp, 0l, SEEK_END+10))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��������
	NDK_SP_FsClose(fp);
	if((ret=NDK_SP_FsDel(FILETEST3))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	send_result("%s����ͨ��", TESTAPI); // ����ͨ��
	return;
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETEST3);
	return;
}

