/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: fs8.c
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: ����NDK_FsGetDiskSpace�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if OVERSEAS_ENABLE	|| defined SP610
#define  FILETEST8 "FT8"
#else
#define  FILETEST8 "/appfs/FT8"
#endif
#define 	TESTAPI		"NDK_FsGetDiskSpace"
#if defined ME20||defined ME32
#define	LOOP	50  //ME20��ME32�ļ�ϵͳ���ÿռ�ֻ��68K  �ʸ�Сд�Ĵ���
#elif defined ME30THM||defined ME32THM
#define LOOP 15
#else
#define	LOOP	100//10 //���⿪̫������Ͷ�ƽ̨��ջ���
#endif
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
*
*****************************************************************/
void fs8(void)
{
	/*private & local definition*/
	int fp=0, writelen=0, loop=0, ret=0;
	char writebuf[1024/**10*/]={0};//���⿪̫������Ͷ�ƽ̨��ջ���
	ulong spaceofdisk1=0, spaceofdisk11=0, spaceofdisk2=0, spaceofdisk21=0;
#if !(CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE)
	ulong diff = 0;
#endif
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	NDK_FsDel(FILETEST8);
	
	memset(writebuf, 0x10, sizeof(writebuf));

	//����NDK_FsGetDiskSpaceȡ������ʣ��ռ���Ѿ�ʹ�õĴ��̿ռ�
	NDK_FsGetDiskSpace(1, &spaceofdisk1);
	NDK_FsGetDiskSpace(0, &spaceofdisk2);
	if((fp=NDK_FsOpen(FILETEST8, "w"))<0)
	{
		send_result("line %d:���ļ�����(%d)", __LINE__, fp);
		GOTOERR;
	}
	//ѭ��д��100KB���ݣ�ME32��ME20д��50KB����
	for(loop=0;loop<LOOP;loop++)
	{
		if((writelen=NDK_FsWrite(fp, writebuf, sizeof(writebuf)))!=sizeof(writebuf))
		{
			send_result("line %d:д�ļ����ݳ���[%d, %d]", __LINE__, loop, writelen);
			GOTOERR;
		}
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//UBI�ļ�ϵͳ�ݲ�֧�ָð���������5892ƽ̨�رյ����̿ռ���ز��԰�����20140109 ֣�������linwl�޸�
#if !(CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE)
	//case1: �ٴε���NDK_FsGetDiskSpaceȡ������ʣ��ռ�
	//�����һ���ļ���С,�����һ��ҳ��С(��,128M flash��ҳ��2KB,64M����512B,32M����?...).��Щ����Ϊ����Ԥ��
	NDK_FsGetDiskSpace(1, &spaceofdisk11);
	if((diff=spaceofdisk1-spaceofdisk11)!=sizeof(writebuf)*LOOP
	&&diff!=sizeof(writebuf)*LOOP+512
	&&diff!=sizeof(writebuf)*LOOP+2048
	&&diff!=sizeof(writebuf)*LOOP+4096
	&&diff!=sizeof(writebuf)*LOOP+1024
	)//�����һ���ļ���С,�����һ��ҳ��С(��,128M flash��ҳ��2KB,64M����512B,32M����?...).��Щ����Ϊ����Ԥ��
	{
		send_result("line %d:ȡ������ʣ��ռ����[ԭ:%ld, ��:%ld]", __LINE__, spaceofdisk1, spaceofdisk11);
		GOTOERR;
	}

	//case2:�ٴε���NDK_FsGetDiskSpaceȡ�Ѿ�ʹ�õĴ��̿ռ�
	NDK_FsGetDiskSpace(0, &spaceofdisk21);
	if((diff=spaceofdisk21-spaceofdisk2)!=sizeof(writebuf)*LOOP
	&&diff!=sizeof(writebuf)*LOOP+512
	&&diff!=sizeof(writebuf)*LOOP+2048
	&&diff!=sizeof(writebuf)*LOOP+4096
	&&diff!=sizeof(writebuf)*LOOP+1024)//�����һ���ļ���С,�����һ��ҳ��С(��,128M flash��ҳ��2KB,64M����512B,32M����?...).��Щ����Ϊ����Ԥ��
	{
		send_result("line %d:ȡ���������ÿռ����[ԭ:%ld, ��:%ld]", __LINE__, spaceofdisk2, spaceofdisk21);
		GOTOERR;
	}
#endif

	//case3:ɾ���ļ�,ɾ���ļ�������̿ռ�Ӧ�ûָ���֮ǰ(�ļ���ϢӦ��ȫ�ͷ�)����ʣ�����ʹ�õĶ�Ҫ��֤�ָ�
	if((ret=NDK_FsDel(FILETEST8))!=NDK_OK)
	{
		send_result("line %d:ɾ���ļ�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
#if K21_ENABLE//Ŀǰ�ж˲�Ʒ���ļ�ϵͳ�ǻ���Linux��,�ռ䲢���������ͷ�,ֻ����85�Ļ��ߵͶ˲�Ʒ���Ҫ���������20170809 �������jym�޸�
#if 0//!K21_ENABLE
	sync();sleep(3);//���ݿ���Ҫ���ڻ�ȡ�ļ�ϵͳ�ռ��Сǰ�����ڴ�ռ� 20140915 linwl
#endif
	NDK_FsGetDiskSpace(1, &spaceofdisk11);
	if(spaceofdisk1!=spaceofdisk11)
	{
		send_result("line %d:�����̿ռ�δ��ȫ�ͷ�(%ld,%ld)", __LINE__, spaceofdisk1, spaceofdisk11);
		GOTOERR;
	}
	NDK_FsGetDiskSpace(0, &spaceofdisk21);
	if(spaceofdisk2!=spaceofdisk21)
	{
		send_result("line %d:�����̿ռ�δ��ȫ�ͷ�(%ld,%ld)", __LINE__, spaceofdisk2, spaceofdisk21);
		GOTOERR;
	}
#endif

	//case4:�쳣����
	if((ret=NDK_FsGetDiskSpace(1, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	// ����ͨ��
	send_result("%s����ͨ��(%ld)", TESTAPI, spaceofdisk1);
	return;
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(FILETEST8);
	return;
}

