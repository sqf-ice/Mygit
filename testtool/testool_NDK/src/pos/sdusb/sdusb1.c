/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: sdusb
* file name		: 
* Author 			: 
* version			: V1.0
* DATE			: 
* directory 		: 
* description		: ����NDK_DiskOpen�ܷ��U��/SD��,NDK_DiskClose�ܷ�ر�U��/SD��
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
#define	TESTAPI		"NDK_DiskOpen,NDK_DiskClose"
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
void sdusb1(void)
{
	/*private & local definition*/
	int ret = 0, ret1 = 0, ret2 = 0,diskstate;
	int disktype = get_disktype();
	 time_t oldtime = 0;
	 
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ�� 
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;			
	}
	
	//case1:�쳣����
	if( ((ret=NDK_DiskOpen(-1,1))!=NDK_ERR_USDDISK_NONSUPPORTTYPE) || 
		((ret1=NDK_DiskOpen(disktype,-3))!=NDK_ERR_USDDISK_PARAM)  ||	//����-652��NDK_ERR_USDDISK_NONSUPPORTTYPEֵ�ĵ����� ��һ��
		((ret2=NDK_DiskClose(-3))!=NDK_ERR_USDDISK_NONSUPPORTTYPE) 	//����-652��NDK_ERR_USDDISK_NONSUPPORTTYPEֵ�ĵ����� ��һ��
		)
	{		
		send_result("line %d:%s����ʧ��(ret=%d,ret1=%d,ret2=%d)", __LINE__, TESTAPI, ret, ret1 ,ret2);
		GOTOERR;		
	}
if(auto_flag==0 || auto_flag==2)	
{
	//case2:δ��U�̻�SD�����Open�ɹ�����,��ȡSD��״̬ΪDISKNOEXIT��U�̵�״̬ΪDISKMOUNTING��DISKTIMEOUT
	cls_show_msg("��ȷ��δ����%s,�����������...", (disktype==UDISK)?"U��":"SD��");
	if( (ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);	
		GOTOERR;		
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&diskstate))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if( ( (disktype==SDDISK) && (diskstate==DISKNOEXIT) )||( (disktype==UDISK) && (diskstate==DISKTIMEOUT||diskstate==DISKMOUNTING) ))
			break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ", __LINE__, TESTAPI);		
			goto ERR;					
		}	
		//usleep(100);
		NDK_SysMsDelay(1);
	}	
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;			
	}
}
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
	
	//case3:��������
if(auto_flag==0)	
	cls_show_msg("��ȷ���Ѳ���%s,�����������...",(disktype==UDISK)?"U��":"SD��");
if(auto_flag==0 || auto_flag==1)
{
	if((ret=NDK_DiskOpen(disktype,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
	
	//case4:�ظ�����Ӧ�óɹ�
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);		
		GOTOERR;		
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&diskstate))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		} 
		if(diskstate==DISKMOUNTSUCC)
			break;	
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ", __LINE__, TESTAPI);		
			goto ERR;					
		}	
		//usleep(100);
		NDK_SysMsDelay(1);
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;			
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
}
	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_DiskClose(disktype);
	return;
}

