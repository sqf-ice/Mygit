/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 2.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanSet�Ƿ������������ɨ��ͷ��صĲ���
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define    TESTAPI		"NDK_ScanSet"
#define    MAXTIMEOUT  10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan2
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*									jiangym			20130917		created
*
*****************************************************************/
void scan2(void)
{
	/*private & local definition*/
	int ret = 0, plen = 0, ret2 = 0, i=0;
	char pvalue[500] = {0};
	char* displaystr[] = {"��˸","������","���볤��",};
	char* displaystr2[] = {"��˸,ɨ��������Ϩ��","ʼ�ն���Ϩ���","Ϩ��,ɨ�������Խ��ƿ���"};
	char* displaystr3[] = {"��˸","�޶Խ�","��Ӧ",};
	char sBuf[128]={0};
	uint len = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��ģ���ʼ��
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO,&len,sBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((sBuf[3]==0x03) || (sBuf[3]==0x04) || (sBuf[3]==0x05) || (sBuf[3]==0x06))
	{
		//case1:��������,emScanSet �����䷶Χ��,��֧�ֵ�Ӳ��Ԥ�ڷ��ز�֧��
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT-1,1))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FACTORYDEFAULT+1,1))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	else
	{
		//case1:��������,emScanSet �����䷶Χ��,֧�ֵ�Ӳ��Ԥ�ڷ��ز�������
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT-1,1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FACTORYDEFAULT+1,1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	//�Ƚ���һ�����ã��ж�ɨ��ͷ�Ƿ�֧�ָò���
	if((ret=NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT,1))!=NDK_ERR_NOT_SUPPORT)
	{
		//case1:������unSetValue��ֵ����0-2��Χ 
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT,-1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT,3))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case2:�����������������õ�ֵΪ0--��˸,1--������,2--���볤��
		for(i=0;i<3;i++)
		{
			if((ret=NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT,i))==NDK_OK)
			{
				cls_printf("����10���ڽ���ɨ��");
				if((ret2=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret2);
					GOTOERR;
				}
				if(cls_show_msg("ɨ�������������%s,ɨ��������Ϩ��,[Enter]ͨ��,����ʧ��", displaystr[i])!=ENTER)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					GOTOERR;
				}
			}
			else if(ret==NDK_ERR_NOT_SUPPORT)
			{
				if(cls_show_msg("ɨ��ͷ�Ƿ�֧������������%s,�ǰ�[ȷ��],����[����]", displaystr[i])!=ENTER)
				{
					send_result("line %d: %s�����û�ȡ���˲���", __LINE__, TESTAPI);
					GOTOERR;
				}
			}
			else
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
	}
	else
	{
		if(cls_show_msg("ɨ��ͷ�Ƿ�֧������������,�ǰ�[ȷ��],����[����]")!=ENTER)
		{
			send_result("line %d: %s�����û�ȡ���˲���", __LINE__, TESTAPI);
			GOTOERR;
		}
	}

	//�Ƚ���һ�����ã��ж�ɨ��ͷ�Ƿ�֧�ָò���
	if((ret=NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT,1))!=NDK_ERR_NOT_SUPPORT)
	{
		//case3:�Խ���unSetValue��ֵ����0-2��Χ 
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT,-1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT,3))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case4:�����������������õ�ֵΪ0--��˸,1--�޶Խ�,2--��Ӧ
		for(i=0;i<3;i++)
		{
			if((ret=NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT,i))==NDK_OK)
			{
				cls_printf("����10���ڽ���ɨ��");
				if((ret2=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret2);
					GOTOERR;
				}
				if(cls_show_msg("ɨ������жԽ���%s,[Enter]ͨ��,����ʧ��", displaystr2[i])!=ENTER)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					GOTOERR;
				}
			}
			else if(ret==NDK_ERR_NOT_SUPPORT)
			{
				if(cls_show_msg("ɨ��ͷ�Ƿ�֧�ֶԽ�������%s,�ǰ�[ȷ��],����[����]", displaystr3[i])!=ENTER)
				{
					send_result("line %d: %s�����û�ȡ���˲���", __LINE__, TESTAPI);
					GOTOERR;
				}
			}
			else
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
	}
	else
	{
		if(cls_show_msg("ɨ��ͷ�Ƿ�֧�ֶԽ�������,�ǰ�[ȷ��],����[����]")!=ENTER)
		{
			send_result("line %d: %s�����û�ȡ���˲���", __LINE__, TESTAPI);
			goto ERR;
		}
	}

	//�Ƚ���һ�����ã��ж�ɨ��ͷ�Ƿ�֧�ָò���
	if((ret=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,1))!=NDK_ERR_NOT_SUPPORT)
	{
		//case5:����������unSetValue��ֵ����1-20��Χ
		if((ret=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,0))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,21))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case6:��������������,���õ�ֵ�ֱ�Ϊ1��20,����ɨ���ٶȵıȽ�
		//��ֵԽ��������Խ��
		if((ret=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,20))==NDK_OK)
		{
			cls_printf("�����ɨ��");
			if((ret=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
		else
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret2=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,1))==NDK_OK)
		{
			cls_printf("�����ɨ��");
			if((ret=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret2);
				GOTOERR;
			}
		}
		else
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret2);
			GOTOERR;
		}
		if(ret==NDK_OK&&ret2==NDK_OK)
		{
			if(cls_show_msg("�ڶ���ɨ��о��ȵ�һ��ɨ���,[Enter]ͨ��,����ʧ��")!=ENTER)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI);
				GOTOERR;
			}
		}
	}
	else
	{
		if(cls_show_msg("ɨ��ͷ�Ƿ�֧������������,�ǰ�[ȷ��],����[����]")!=ENTER)
		{
			send_result("line %d: %s�����û�ȡ���˲���", __LINE__, TESTAPI);
			goto ERR;
		}
	}

	//�Ƚ���һ�����ã��ж�ɨ��ͷ�Ƿ�֧�ָò���
	if((ret=NDK_ScanSet(SCAN_SETTYPE_FACTORYDEFAULT,1))!=NDK_ERR_NOT_SUPPORT)
	{
		//case7:����������unSetValue��ֵ����1-20��Χ
		if((ret=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,0))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY,21))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case8:�ָ���������Ӧ�óɹ�
		if((ret=NDK_ScanSet(SCAN_SETTYPE_FACTORYDEFAULT,11))==NDK_OK)
		{
			cls_printf("�����ɨ��");
			if((ret=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			//����Ĭ������(�����Խ�Ϊ��˸��������Ϊ11))
			if(cls_show_msg("ɨ������������Խ���Ϊ��˸,[Enter]ͨ��,����ʧ��")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
			}
		}
		else
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	else
	{
		if(cls_show_msg("ɨ��ͷ�Ƿ�֧�ֳ���Ĭ������,�ǰ�[ȷ��],����[����]")!=ENTER)
		{
			send_result("line %d: %s�����û�ȡ���˲���", __LINE__, TESTAPI);
			GOTOERR;
		}
	}

	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScanSet(SCAN_SETTYPE_FACTORYDEFAULT,11);
#if SCAN_SOFT_ENABLE	
	NDK_ScanExit();
#endif
	return;
}

