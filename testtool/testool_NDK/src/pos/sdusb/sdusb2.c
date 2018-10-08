/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: usb
* file name		: 
* Author 			: 
* version			: V1.0
* DATE			: 
* directory 		: 
* description		: ����NDK_DiskGetState�Ƿ������ȷ��ʾ״̬
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
#define	TESTAPI		"NDK_DiskGetState"
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
void sdusb2(void)
{
	/*private & local definition*/
	int ret = 0, ret1 = 0, diskstate = 0;
	int disktype = get_disktype();;
	time_t oldtime = 0;	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;			
	}
	
	//DISKDRIVERLOADFAIL=3�޷�����

	//case1:����DISKNOTOPEN:δopen,Ӧ�÷���DISKNOTOPEN
if(auto_flag==0)	
	cls_show_msg("��ȷ���Ѳ���%s,�����������...",(disktype==UDISK)?"U��":"SD��");
if(auto_flag==0 || auto_flag==1)
{
	if((ret=NDK_DiskGetState(disktype,&diskstate))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(diskstate!=DISKNOTOPEN)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, diskstate);
		GOTOERR;
	}	
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, ret);	
		GOTOERR;		
	}
	cls_show_msg1(2,"����������ͨ��....");
	
	//case2:�Ƿ���������
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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
	if(((ret=NDK_DiskGetState(-3,&diskstate))!=NDK_ERR_USDDISK_NONSUPPORTTYPE)||((ret1=NDK_DiskGetState(disktype,NULL))!=NDK_ERR_USDDISK_PARAM))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, ret);	
		GOTOERR;		
	}
	cls_show_msg1(2,"����������ͨ��....");
}
if(auto_flag==0 || auto_flag==2)	
{
	//case3:����DISKTIMEOUT��DISKNOEXIT��δ�忨��δ��U�̣���ʱ��ȡ״̬��SD������DISKNOEXIT��U�̷���DISKMOUNTING��
	cls_show_msg("��ȷ��δ����%s,�����������...",(disktype==UDISK)?"U��":"SD��");
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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
		if( ( (disktype==UDISK) && (diskstate==DISKMOUNTING) )||( (disktype==SDDISK) && (diskstate==DISKNOEXIT) ))
				break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ(%d)", __LINE__, TESTAPI, diskstate);
			goto ERR;			
		}
		//usleep(100);
		NDK_SysMsDelay(1);
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, ret);	
		GOTOERR;		
	}
	cls_show_msg1(2,"����������ͨ��....");
	
	//case4:����U�̻���SD������������open�󣬵���NDK_DiskGetState
	cls_show_msg("��ȷ��δ����%s,�����������...",(disktype==UDISK)?"U��":"SD��");
	if((ret=NDK_DiskOpen(disktype,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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
		if( ( (disktype==UDISK) && (diskstate==DISKMOUNTING) )||( (disktype==SDDISK) && (diskstate==DISKNOEXIT) ))
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
	cls_show_msg1(2,"����������ͨ��....");
}
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
	
	//case5:Sd��Ҫ��DISKTIMEOUT״̬��Ҫ��ȷ���Ѳ�SD����Ȼ���ٲⳬʱ״̬(��״̬���ײ⣬��Ϊ��ʱ�Ѿ���sd�����п��ܷ�������״̬DISKMOUNTSUCC��DISKMOUNTING��DISKTIMEOUT)?
	//U�̲�DISKTIMEOUT״̬���ڹ涨ʱ���ڻ��򿪳ɹ���ʱ����DISKTIMEOUT״̬.
if(auto_flag==0)
	cls_show_msg("��ȷ���Ѳ���%s,�����������...",(disktype==UDISK)?"U��":"SD��");	
if(auto_flag==0 || auto_flag==1)
{
	if((ret=NDK_DiskOpen(disktype,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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
		if( ( (disktype==UDISK) && (diskstate==DISKTIMEOUT) )||( (disktype==SDDISK) && (diskstate==DISKTIMEOUT||diskstate==DISKMOUNTSUCC) ))
			break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ(%d)", __LINE__, TESTAPI, diskstate);
			goto ERR;			
		}
		//usleep(100);
		NDK_SysMsDelay(1);
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, ret);	
		GOTOERR;		
	}										
	cls_show_msg1(2,"����������ͨ��.....");
}
	//case6:�Ѳ�SD����U�̣����Ի�ȡȥ��SD����U��״̬DISKMOUNTSUCC����DISKMOUNTING
if(auto_flag==0)	
	cls_show_msg("��ȷ���Ѳ���%s,�����������...",(disktype==UDISK)?"U��":"SD��");
if(auto_flag==0 || auto_flag==1)
{
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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
		if(diskstate==DISKMOUNTING)
			cls_printf("%s������...",(disktype==UDISK)?"U��":"SD��");
		if(diskstate==DISKMOUNTSUCC)
			break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ(%d)", __LINE__, TESTAPI, diskstate);
			goto ERR;					
		}		
		//usleep(100);
		NDK_SysMsDelay(1);
	}
}	
	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_DiskClose(disktype);
	return;
}

