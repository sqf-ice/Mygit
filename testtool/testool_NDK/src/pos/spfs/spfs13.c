/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name		: spfs13.c
* Author 			: linying
* version			: 
* DATE			: 20180829
* directory 		: 
* description		: �����ļ������þ����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/

#define  FILETEST16 "/appfs/FT16"
#define	TESTAPI		"�����þ����"
#define	MAXFD	5


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
*					        linying		   20180829     	created
*****************************************************************/
void spfs13(void)
{
	//�˰���ֻ���K21���
	/*private & local definition*/
	int i = 0, countnum = 0, ret = 0;
	int fp[6] = {-1, -1, -1, -1, -1, -1};

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("%s������...", TESTAPI); // ��Ļ��ʾ��ǰ��������ģ��������

	NDK_SP_FsDel(FILETEST16);

	//step (2)��"w"��ʽ��һ���ض��ļ�
	if ((fp[0]=NDK_SP_FsOpen(FILETEST16, "w"))<0) // �����ļ�����
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp[0]);
		GOTOERR;
	}
	
	//step (3)�رո��ļ�
	NDK_SP_FsClose(fp[0]);

	//step (4)ѭ���򿪸��ļ�ֱ��ʹ�����������
	for(countnum=0; countnum<MAXFD; countnum++)
	{
		if ((fp[countnum]=NDK_SP_FsOpen(FILETEST16, "r"))<0) // ���ļ�ʧ��
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp[countnum]);
			GOTOERR;
		}
	}
	//step (5)������ļ����Ƿ�Ϊ5
	if((fp[countnum]=NDK_SP_FsOpen(FILETEST16, "r"))>=0) // ���ļ����ʧ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp[countnum]);
		GOTOERR;
	}

	//���ֵ�Ƿ����[256, 260],ME32�ֿ�ռ��2�����ֵ�����ֵ����[258, 262]
	for(countnum=0; countnum<MAXFD; countnum++)
	{
		if ((fp[countnum]<256)||(fp[countnum]>260))
		{
			send_result("line %d:�����Χ����(fp(%d):%d)", __LINE__, countnum, fp[countnum]);
			GOTOERR;
		}
	}

	//step (6)�ر����о��
	for(i = 0; i<MAXFD; i++) 
	{
		if((ret=NDK_SP_FsClose(fp[i]))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d, %d)", __LINE__, TESTAPI, fp[i], ret);
			GOTOERR;
		}
	}

	//��������
	if((ret=NDK_SP_FsDel(FILETEST16))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	

	send_result("%s����ͨ��", TESTAPI); // ����ͨ��
	return;
ERR:
	for(countnum=0; countnum<SZ_ARRAY(fp); countnum++)
		NDK_SP_FsClose(fp[countnum]);
	NDK_SP_FsDel(FILETEST16);
	return;
}


