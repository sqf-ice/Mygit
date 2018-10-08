/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs16.c
* Author 			: chensj
* version			: 
* DATE			: 20140218
* directory 		: 
* description		: �����ļ������þ����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if defined SP610
#define  FILETEST16 "FT16"
#else
#define  FILETEST16 "/appfs/FT16"
#endif
#define	TESTAPI		"�����þ����"
#if defined ME68||defined ME50NX
#define	MAXFD	10
#else
#define	MAXFD	5
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���16������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*					chensj			20140218	created
*****************************************************************/
void fs16(void)
{
	//�˰���ֻ���K21���
	/*private & local definition*/
	int i = 0, countnum = 0, ret = 0;
#if defined ME68||defined ME50NX
	int fp[11] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
#else
	int fp[6] = {-1, -1, -1, -1, -1, -1};
#endif

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("%s������...", TESTAPI); // ��Ļ��ʾ��ǰ��������ģ��������

	NDK_FsDel(FILETEST16);

	//step (2)��"w"��ʽ��һ���ض��ļ�
	if ((fp[0]=NDK_FsOpen(FILETEST16, "w"))<0) // �����ļ�����
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp[0]);
		GOTOERR;
	}
	
	//step (3)�رո��ļ�
	NDK_FsClose(fp[0]);

	//step (4)ѭ���򿪸��ļ�ֱ��ʹ�����������
	for(countnum=0; countnum<MAXFD; countnum++)
	{
		if ((fp[countnum]=NDK_FsOpen(FILETEST16, "r"))<0) // ���ļ�ʧ��
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp[countnum]);
			GOTOERR;
		}
	}
	//step (5)������ļ����Ƿ�Ϊ5
	if((fp[countnum]=NDK_FsOpen(FILETEST16, "r"))>=0) // ���ļ����ʧ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp[countnum]);
		GOTOERR;
	}

	//���ֵ�Ƿ����[256, 260],ME32�ֿ�ռ��2�����ֵ�����ֵ����[258, 262]
	for(countnum=0; countnum<MAXFD; countnum++)
	{
#if defined ME32||defined ME32THM||defined ME65||defined ME30THM
		if ((fp[countnum]<258)||(fp[countnum]>262))
		{
			send_result("line %d:�����Χ����(fp(%d):%d)", __LINE__, countnum, fp[countnum]);
			GOTOERR;
		}
#elif defined ME68||defined ME50NX
		if ((fp[countnum]<256)||(fp[countnum]>265))
		{
			send_result("line %d:�����Χ����(fp(%d):%d)", __LINE__, countnum, fp[countnum]);
			GOTOERR;
		}
#else
		if ((fp[countnum]<256)||(fp[countnum]>260))
		{
			send_result("line %d:�����Χ����(fp(%d):%d)", __LINE__, countnum, fp[countnum]);
			GOTOERR;
		}
#endif
	}

	//step (6)�ر����о��
	for(i = 0; i<MAXFD; i++) 
	{
		if((ret=NDK_FsClose(fp[i]))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d, %d)", __LINE__, TESTAPI, fp[i], ret);
			GOTOERR;
		}
	}

	//��������
	if((ret=NDK_FsDel(FILETEST16))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	

	send_result("%s����ͨ��", TESTAPI); // ����ͨ��
	return;
ERR:
	for(countnum=0; countnum<SZ_ARRAY(fp); countnum++)
		NDK_FsClose(fp[countnum]);
	NDK_FsDel(FILETEST16);
	return;
}


