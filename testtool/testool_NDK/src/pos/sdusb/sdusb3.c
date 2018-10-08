/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: sdusb
* file name		: 
* Author 			: 
* version			: V1.0
* DATE			: 
* directory 		: 
* description		: ����NDK_DiskGetRootDirName�ܷ��ȡU�̸�Ŀ¼������
 * related document	: ����Ա�ο��ֲ�
 *
 ************************************************************************
 * log			: 
 * Revision 1.0  
 * no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_DiskGetRootDirName"
#define	MAXWAITTIME	30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		
*****************************************************************/
void sdusb3(void)
{
	if(auto_flag==2)
		return;
	/*private & local definition*/
	int ret = 0, diskstate = 0;
	char rootdir[][16] = {{0}};
	int disktype = get_disktype();
	time_t oldtime;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:�������ǰ��ʧ��(ret=%d)", __LINE__, ret);
		return;	
	}	
	
	//case1:��δ��U���豸
	if((ret=NDK_DiskGetRootDirName(disktype,(char **)rootdir))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//�����豸
	cls_show_msg1(5,"��ȷ���Ѳ���%s��,�����������...",(disktype==UDISK)?"U��":"SD��");
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%sʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&diskstate))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(diskstate==DISKMOUNTSUCC)//��⵽�豸���˳�
			break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ(%d)", __LINE__, TESTAPI, diskstate);
			goto ERR;			
		}
		//usleep(100);
		NDK_SysMsDelay(1);
	}
	
	//case2:�Ƿ���������
	if((ret=NDK_DiskGetRootDirName(-3,(char **)rootdir))!=NDK_ERR_USDDISK_NONSUPPORTTYPE)//����-652��NDK_ERR_USDDISK_NONSUPPORTTYPEֵ�ĵ����� ��һ��
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_DiskGetRootDirName(disktype,NULL))!=NDK_ERR_USDDISK_PARAM)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:����ʹ��
	if((ret=NDK_DiskGetRootDirName(disktype,(char **)rootdir))<0)
	{	
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	else
	{
		if(auto_flag==0||auto_flag==2)
		{
			if(cls_show_msg1(MAXWAITTIME, "ȡ��U�̸�Ŀ¼��Ϊ%s.\n"
										"ȷ�ϼ�:һ��\n"
										"������:����\n", rootdir[0])!=ENTER)

			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
			}
		}
		else
			send_result("ȡ��U�̸�Ŀ¼��Ϊ%s.���ж��Ƿ���ȷ", rootdir[0]);
	}
	
	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_DiskClose(disktype);
	return;
}

