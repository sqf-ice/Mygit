/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt20.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterScan,NDK_BTMasterStopScan �Ƿ��������ɨ��õ�ɨ���������Ҹ�����Ҫֹͣɨ��
* related document	: NDK.chm
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterScan,NDK_BTMasterStopScan"	
#define EXPMAXNUM 10
#define MAXWAITTIME 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt20
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150529		created
*****************************************************************/
void bt20(void)	//�޲����޷���
{
	/*private & local definition*/
	ST_BT_DEV sresult[50],sresult1[50];
	int ret=0,snum=0,j=0,i=0,snum1=0;
	time_t oldtime=0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1����������δ�򿪣�ɨ����ȡɨ������ֹͣɨ�趼Ӧ��ʧ�ܣ�����ֵΪ��NDK_ERR_OPEN_DEV��
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;	
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;	
	}

	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//case2�������쳣Ӧ���ز�������
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_BREDR-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case3����������:�ֱ�ɨ�赥ģBREDR����ģBLE��˫ģ����������ɨ������������豸����Ӧ��֮һ��	
	for(j=BT_DEVICE_TYPE_BREDR;j<BT_DEVICE_TYPE_DUMO+1;j++)
	{
		cls_printf("���ڲ���%dģʽ��...", j);
		if((ret=NDK_BTMasterScan(j))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,j);
			GOTOERR; 
		}
		oldtime=time(NULL);	
		while(1)
		{
			sleep(5);
			memset(sresult, 0, sizeof(sresult));
			if((ret=NDK_BTMasterGetScanResults(NULL,sresult,EXPMAXNUM,&snum))!=NDK_OK)//Ӧ���ܹ��ɹ���ȡ��ɨ����Ϣ
			{
				send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, snum,j);
				GOTOERR;
			}
			if(snum>0)//ɨ�赽����һ�����˳�ɨ��
				break;
			if((time(NULL)-oldtime)>MAXWAITTIME)
			{
				send_result("line %d:%s��ʱ��δɨ�赽������Ϣ(%d,%d)", __LINE__, TESTAPI, snum,j);		
				break;
			}
		}
		if(j!=BT_DEVICE_TYPE_DUMO)//ɨ��˫ģ�᷵��3�����͵��豸�������豸���ͽ��бȽ�
		{
			for(i=0;i<snum;i++)
			{
				//cls_show_msg1(1,"snum=%d,j=%d,i=%d", snum, j, i);				
				if(sresult[i].emBtType!=j&&sresult[i].emBtType!=BT_DEVICE_TYPE_DUMO)//ɨ�赥ģʱӦ����Ҫɨ��ĵ�ģ���ͻ���˫ģ
				{					
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, sresult[i].emBtType,j);					
					GOTOERR;
				}
			}
		}
		//���Թ����г��ֹ�ֹͣɨ��ʧ�ܵ����
		if((ret=NDK_BTMasterStopScan())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,j);
			GOTOERR; 
		}
	}

	//case4:�������ε���NDK_BTMasterScan��Ԥ��Ӧ���ǵ�һ����Ч����ȡ����ɨ��ģʽӦ�ú�Ԥ��һ��
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_BREDR))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	//ʵ�ʲ���:�������ε��ã��м����û�е���NDK_BTMasterStopScan����2��NDK_BTMasterScan����-1  �÷����Ѿ���ȡ�� 20151009 Ϳ־��
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_BLE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);
	memset(sresult, 0, sizeof(sresult));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,EXPMAXNUM,&snum))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	for(i=0;i<snum;i++)
	{				
		if(sresult[i].emBtType!=BT_DEVICE_TYPE_BREDR&&sresult[i].emBtType!=BT_DEVICE_TYPE_DUMO)
		{					
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, sresult[i].emBtType);					
			GOTOERR;
		}
	}
	//���Ҫ��ɶ�ʹ��
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}
	
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_BLE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);
	memset(sresult, 0, sizeof(sresult));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,EXPMAXNUM,&snum))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	for(i=0;i<snum;i++)
	{				
		if(sresult[i].emBtType!=BT_DEVICE_TYPE_BLE&&sresult[i].emBtType!=BT_DEVICE_TYPE_DUMO)
		{					
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, sresult[i].emBtType);					
			GOTOERR;
		}
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}
	
	//case5����������:ɨ��˫ģ����ֹͣɨ������Ӽ�̨�豸��ɨ���������֮ǰ�����Ա�	
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);
	memset(sresult, 0, sizeof(sresult));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,EXPMAXNUM,&snum))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}
	cls_show_msg("�������һ̨���̨�������豸����Ӻ������������");
	sleep(10);
	memset(sresult1, 0, sizeof(sresult1));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult1,EXPMAXNUM,&snum1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}
	if(snum!=snum1)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, snum,snum1);
		goto ERR;
	}
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTMasterStopScan();
	NDK_PortClose(PORT_NUM_BT);
	return;
}

