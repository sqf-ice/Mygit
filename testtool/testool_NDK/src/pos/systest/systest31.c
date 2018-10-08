/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: Ӳ����Ϣ����
* file name			: systest31.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: Ӳ����Ϣ����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"
#define  MAXWAITTIME 30

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"Ӳ����Ϣ"

/*------------global variables definition-----------------------*/
#define	ITEMPERPAGE	(4)

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
//case 1.1:ˢ������
void CardCountTest(void)
{
	unsigned int unLen=0;
	char sBuf1[128]={0};
	char sBuf2[128]={0};
	int ret = 0;

#if K21_ENABLE//K21ƽ̨��֧��ȡ�ſ�ˢ������
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_CREDITCARD_COUNT, &unLen, sBuf1))!=NDK_ERR_NOT_SUPPORT)
	{	
		cls_show_msg("line %d:ˢ��ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf1);
		return;
	}
	cls_show_msg1(2,"line %d:�û�����֧�ֻ�ȡˢ������,�ʲ����������",__LINE__);
	return;
#endif	
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_CREDITCARD_COUNT, &unLen, sBuf1))!=NDK_OK)
	{	
		cls_show_msg("line %d:ˢ��ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf1);
		return;
	}
	if(cls_show_msg("��ǰˢ������Ϊ:%s��,[ȷ��]��ȷ,[����]����",sBuf1) != ENTER)
	{
		cls_show_msg("line %d:ˢ��ͳ��ʧ��(%s��)",__LINE__,sBuf1);
		return;
	}
	MagcardReadTest(TK2, FALSE, 0);
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_CREDITCARD_COUNT, &unLen, sBuf2))!=NDK_OK)
	{	
		cls_show_msg("line %d:ˢ��ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf2);
		return;
	}
	if(atoi(sBuf2) - atoi(sBuf1) !=  1)
	{
		cls_show_msg("line %d:ˢ��ͳ��ʧ��(%s,%s)",__LINE__,sBuf2,sBuf1);
		return;
	}

	cls_show_msg1(2, "ˢ��ͳ�Ƴɹ�");
	return;
}

#if PRN_ENABLE
//case 1.2:��ӡ�ܳ���
void PrintLenTest(void)
{
	unsigned int unLen=0;
	char sBuf1[128]={0};
	char sBuf2[128]={0};
	int ret = 0;
	
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_PRN_LEN, &unLen, sBuf1))!=NDK_OK)
	{	
		cls_show_msg("line %d:��ȡ��ӡ����ʧ��(%d,%s)",__LINE__,ret,sBuf1);
		return;
	}
	if(cls_show_msg("��ǰ��ӡ�ܳ���:%smm,[ȷ��]��ȷ,[����]����",sBuf1) != ENTER)
	{
		cls_show_msg("line %d: ��ȡ��ӡ����ʧ��", __LINE__);
		return;
	}
	//��ֹ����ֱ�ӽ��д�ӡ����δ���г�ʼ������������ܷɣ����ӳ�ʼ������ 20130531 zhengdc
	if (FAIL==NDK_PrnInit(g_PrnSwitch))
	{
		cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
	{
		cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}
	print_bill();
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_PRN_LEN, &unLen, sBuf2))!=NDK_OK)
	{	
		cls_show_msg("line %d:��ȡ��ӡ����ʧ��(%d,%s)",__LINE__,ret,sBuf2);
		return;
	}
	if(cls_show_msg("���ֹ�������ӡ��Ʊ�ݳ����Ƿ�Ϊ:%d mm,[ȷ��]��ȷ,[����]����",atoi(sBuf2) - atoi(sBuf1)) != ENTER)//�޸��жϷ�ʽΪ�˹��ж� linwl20130717
	{
		cls_show_msg("line %d: :��ӡͳ��ʧ��(%s,%s)",__LINE__,sBuf2,sBuf1);
		return;
	}
	/*
	if (atoi(sBuf2) - atoi(sBuf1) != 632)
	{
		cls_show_msg("line %d:��ӡͳ��ʧ��(%s,%s)",__LINE__,sBuf2,sBuf1);
		return;
	}
	*/
	cls_show_msg1(2,"��ӡͳ�Ƴɹ�");
	return;
}
#else
void PrintLenTest(void){return;}
#endif

//case 1.3:��������ʱ��
void RunTimeTest(void)
{
	unsigned int unLen=0;
	char sBuf1[128]={0};
	char sBuf2[128]={0};
	int ret = 0;
	
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_RUNTIME, &unLen, sBuf1))!=NDK_OK)
	{	
		cls_show_msg("line %d:��������ʱ��ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf1);
		return;
	}
	if(cls_show_msg("��������ʱ��:%s��,[ȷ��]��ȷ,[����]����",sBuf1) != ENTER)
	{
		cls_show_msg("line %d:��������ʱ��ͳ��ʧ��(%s)",__LINE__,sBuf1);
		return;
	}
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_RUNTIME, &unLen, sBuf1))!=NDK_OK)
	{	
		cls_show_msg("line %d:��������ʱ��ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf1);
		return;
	}
	NDK_SysMsDelay(3000);
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_RUNTIME, &unLen, sBuf2))!=NDK_OK)
	{	
		cls_show_msg("line %d:��������ʱ��ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf2);
		return;
	}
	if (atoi(sBuf2) - atoi(sBuf1) < 2 || atoi(sBuf2) - atoi(sBuf1) > 4)
	{
		cls_show_msg("line %d:��������ʱ��ͳ��ʧ��(%s,%s)",__LINE__,sBuf2,sBuf1);
		return;
	}

	cls_show_msg1(2,"��������ʱ��ͳ�Ƴɹ�");
	return;
}

//case 1.4:��������
void KeyCountTest(void)
{
	unsigned int unLen=0;
	char sBuf1[128]={0};
	char sBuf2[128]={0};
	int ret = 0;
	
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_KEY_COUNT, &unLen, sBuf1))!=NDK_OK)
	{	
		cls_show_msg("line %d:����ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf1);
		return;
	}
	if(cls_show_msg("��ǰ��������:%s,[ȷ��]��ȷ,[����]����",sBuf1) != ENTER)
	{
		cls_show_msg("line %d:����ͳ��ʧ��(%s)",__LINE__,sBuf1);
		return;
	}
	if((ret = NDK_SysGetPosInfo(SYS_HWINFO_GET_KEY_COUNT, &unLen, sBuf2))!=NDK_OK)
	{	
		cls_show_msg("line %d:����ͳ��ʧ��(%d,%s)",__LINE__,ret,sBuf2);
		return;
	}
	if (atoi(sBuf2) - atoi(sBuf1) != 1)
	{
		cls_show_msg("line %d:����ͳ��ʧ��(%s,%s)",__LINE__,sBuf2,sBuf1);
		return;
	}

	cls_show_msg1(2,"����ͳ�Ƴɹ�");
	return;
}

/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		      linwei	        20130121	 	created
*****************************************************************/
void systest31(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	//����Ϊ�ֲ�����
	int nKeyin = 0;
	
	cls_show_msg1(1, "%s������...", TESTITEM);
	while(1)
	{
		nKeyin=cls_show_msg("1.ˢ������\n"		
					 	 	"2.��ӡ�ܳ���\n"
					 		"3.��������ʱ��\n"	
					 		"4.��������\n");
		switch(nKeyin)
		{
		case '1':
				CardCountTest();
				break;
		case '2':
				PrintLenTest();
				break;
		case '3':
				RunTimeTest();
				break;
		case '4':
				KeyCountTest();
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

