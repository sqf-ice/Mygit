/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest95.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			:devmgr/dbus����
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"
	
/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"DBUS���߳�"
#define TestApp1    "test1.NLD"
#define TestApp2    "test2.NLD"
#define TestApp3    "test3.NLD"
#define TestApp4    "test4.NLD"
	
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 		:
* functional description	: 
* input parameter		:
* output parameter		: 
* return value			:
* history				: author			date			remarks
*					      sull			  2018/04/11		created
*****************************************************************/
//���ũ��MISӦ�������˳���������������������
//dbus�������comserv��secp��kpserv��libpms��devmgr , kpserv�ݲ�����
static void info_vol()
{
	/*private & local definition*/	
	int i = 0, cnt = 200, ret = -1, fd = 0;
	
	/*process body*/
	if((cls_show_msg1(30,"��ȷ��������%s���ǰ�������������в��ԣ�����ESC�˳�", TestApp1)) == ESC)
		return;
	if((ret = NDK_AppLoad(TestApp1, 0)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:װ����Ӧ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	for(i=1;i<=cnt;i++)
	{
		cls_printf("��%d��������...\n",i);
		fprintf(stderr,"���е�%d��\n",i);
		if((fd = NDK_FsOpen("/root/flag","w")) <0)
		{
			cls_show_msg1(g_keeptime, "line %d:�����ļ�ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�رվ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_AppRun("test1")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:������Ӧ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if( NDK_FsExist("/root/flag")==NDK_OK)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:��Ӧ���쳣�˳�\n", __LINE__);
			goto ERR;
		}

	}
	cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:ȡӲ����Ϣ/ȡ������Ӧ�ò���ͨ��\n", __LINE__);

ERR:
	NDK_FsClose(fd);
	NDK_FsDel("/root/flag");
	return;
}

static void vol_sec()
{
	/*private & local definition*/	
	int i = 0, cnt = 200, ret = -1, fd = 0;
	
	/*process body*/
	if((cls_show_msg1(30,"��ȷ��������%s���ǰ�������������в��ԣ�����ESC�˳�", TestApp2)) == ESC)
		return;
	if((ret = NDK_AppLoad((const char *)TestApp2, 0)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:װ����Ӧ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	for(i=1;i<=cnt;i++)
	{
		cls_printf("��%d��������...\n",i);
		fprintf(stderr,"���е�%d��\n",i);
		if((fd = NDK_FsOpen("/root/flag","w")) <0)
		{
			cls_show_msg1(g_keeptime, "line %d:�����ļ�ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�رվ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_AppRun("test2")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:������Ӧ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if( NDK_FsExist("/root/flag")==NDK_OK)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:��Ӧ���쳣�˳�\n", __LINE__);
			goto ERR;
		}

	}
	cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:ȡӲ����Ϣ/��ȫ��Ӧ�ò���ͨ��\n", __LINE__);

ERR:
	NDK_FsClose(fd);
	NDK_FsDel("/root/flag");
	return;
}

static void sec_wlm()
{
	/*private & local definition*/	
	int i = 0, cnt = 200, ret = -1, fd = 0;
	
	/*process body*/
	if((cls_show_msg1(30,"��ȷ��������%s,�ǰ�������������в��ԣ�����ESC�˳�", TestApp3)) == ESC)
		return;
	if((ret = NDK_AppLoad((const char *)TestApp3, 0)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:װ����Ӧ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	for(i=1;i<=cnt;i++)
	{
		cls_printf("��%d��������...\n",i);
		fprintf(stderr,"���е�%d��\n",i);
		if((fd = NDK_FsOpen("/root/flag","w")) <0)
		{
			cls_show_msg1(g_keeptime, "line %d:�����ļ�ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�رվ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_AppRun("test3")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:������Ӧ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if( NDK_FsExist("/root/flag")==NDK_OK)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:��Ӧ���쳣�˳�\n", __LINE__);
			goto ERR;
		}

	}
	cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:ȡӲ����Ϣ/wifi��Ӧ�ò���ͨ��\n", __LINE__);
	
ERR:
	NDK_FsClose(fd);
	NDK_FsDel("/root/flag");
	return;
}

static void info_wlm()
{
	/*private & local definition*/	
	int i = 0, cnt = 200, ret = -1, fd = 0;
	
	/*process body*/
	if((cls_show_msg1(30,"��ȷ��������%s,�ǰ�������������в��ԣ�����ESC�˳�", TestApp4)) == ESC)
		return;
	if((ret = NDK_AppLoad((const char *)TestApp4, 0)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:װ����Ӧ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	for(i=1;i<=cnt;i++)
	{
		cls_printf("��%d��������...\n",i);
		fprintf(stderr,"���е�%d��\n",i);
		if((fd = NDK_FsOpen("/root/flag","w")) <0)
		{
			cls_show_msg1(g_keeptime, "line %d:�����ļ�ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:�رվ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if((ret = NDK_AppRun("test4")) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:������Ӧ��ʧ��(%d)", __LINE__, ret);
			NDK_FsClose(fd);
			NDK_FsDel("/root/flag");
			continue;
		}
		if( NDK_FsExist("/root/flag")==NDK_OK)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:��Ӧ���쳣�˳�\n", __LINE__);
			goto ERR;
		}

	}
	cls_show_msg1_record(FILENAME ,FUNCNAME, g_keeptime, "line %d:ȡӲ����Ϣ/wifi��Ӧ�ò���ͨ��\n", __LINE__);
	
ERR:
	NDK_FsClose(fd);
	NDK_FsDel("/root/flag");
	return;
}

void systest95(void)
{	
	/*private & local definition*/	
	int nKeyin = 0;
	
	/*process body*/
	while(1)
    {
		nKeyin = cls_show_msg( "1.ȡӲ����Ϣ/ȡ����\n"
						       "2.ȡ����/��ȫ\n"
						       "3.��ȫ/����\n"
						       "4.ȡӲ����Ϣ/����\n");
		switch(nKeyin) 
		{
			case '1':
				info_vol();
				break;
			case '2':
				vol_sec();
				break;
			case '3':
				sec_wlm();
				break;
			case '4':
				info_wlm();
				break;
			case ESC:
				return;
				break;
			default:
				continue;
				break;
		}
	}
	return;
}
