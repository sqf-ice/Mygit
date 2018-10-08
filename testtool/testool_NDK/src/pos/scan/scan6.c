/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 6.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanStart��ʼ����ͷɨ�衢NDK_ScanState��ȡ����ͷɨ��״̬��NDK_ScanStopֹͣ����ͷɨ�蹦��(������ʽ)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScanStart,NDK_ScanState,NDK_ScanStop"
#define 	MAXTIME     60
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan6
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*							zhengry			20160525		created
*
*****************************************************************/
void scan6(void)
{
	/*private & local definition*/
	int ret=0, plen=0;
	uchar pvalue[500]={0};
	uchar pvaluein[35] = {0x3a, 0x37, 0x37, 0x3a, 0x74, 0x30, 0x3a, 0x2f, 0x3a, 0x30, 0x30, 0x2e, 0x2e, 0x74, 0x30, 0x3a, 0x2e, 0x74, 0x3a, 0x33, 0x74, 0x30, 0x2e, 0x33, 0x2e, 0x2e, 0x2e, 0x3a, 0x30, 0x30, 0x3a, 0x2e, 0x33, 0x37, 0x33};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	 //case1:δ��ʼ������ͷ,Ӧ��������ͷδ��ʼ��
	if((ret=NDK_ScanStart()) != NDK_ERR_CAMERA_INIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR_CAMERA_INIT )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_ScanStop()) != NDK_ERR_CAMERA_INIT )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	
	//case2:δ��ʼ����ͷɨ��,��ȡɨ��״̬Ӧ����δ�����첽ɨ��״̬NDK_ERR_CAMERA_NOT_ASYNC
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR_CAMERA_NOT_ASYNC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��ʼɨ���,��û��ɨ��ͼ��,Ӧ��������ɨ��NDK_ERR_CAMERA_SCANNING
	if((ret=NDK_ScanStart()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR_CAMERA_SCANNING)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:��δֹͣ����ͷɨ��,����ͷ���¿�ʼɨ��Ӧ�����豸æ
	if((ret=NDK_ScanStart()) != NDK_ERR_CAMERA_BUSY)
	{
		NDK_ScanStop();
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:��δֹͣ����ͷɨ��,����ͷֱ�ӹر�Ӧ�����豸æ
	if((ret=NDK_ScanClose()) != NDK_ERR_CAMERA_BUSY )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��δֹͣ����ͷɨ��,Ӧ�ܼ�������ɨ��
	cls_show_msg("��׼���ô�ɨ���ͼ��,���������");
	memset(pvalue,0,sizeof(pvalue));
	lib_StartStopwatch();
	while(1)
	{	
		if(lib_ReadStopwatch()>MAXTIME)
		{
			NDK_ScanStop();
			send_result("line %d:%sɨ�賬ʱ", __LINE__, TESTAPI);
			break;
		}
		ret=NDK_ScanState(pvalue, &plen);
		switch(ret)
		{
			case NDK_ERR_CAMERA_SCANNING:
				continue;
				break;
			case NDK_OK:
				NDK_ScanStop();//���Ҫ����Ļ����ʾӦ�õĽ��棬��Ҫ�Ȱ�ɨ��ͣ��
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
				{
					send_result("line %d:%s ����ɨ��������Ԥ�ڲ���", __LINE__, TESTAPI);
					goto ERR;
				}
				break;
				/*if((plen!=35)||!memcmp(pvaluein,pvalue,plen))
				{
					send_result("line %d:%s����ʧ��(plen=%d, pvalue=%s)", __LINE__, TESTAPI, plen, pvalue);
					goto ERR2;
				}
				break;
				*///Ŀǰ��һ����ά��ÿ�ν���Ľ�����ǲ�ͬ��
			default:
				NDK_ScanStop();
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
		}
		if(ret == NDK_OK)
			break;
	}

	//case7:���¿�ʼɨ���Ӧ������ɨ��
	cls_show_msg("��׼������һ����ɨ���ͼ��,���������");
	if((ret=NDK_ScanStart()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	plen=0;
	memset(pvalue,0,sizeof(pvalue));
	lib_StartStopwatch();
	while(1)
	{	
		if(lib_ReadStopwatch()>MAXTIME)
		{
			NDK_ScanStop();
			send_result("line %d:%sɨ�賬ʱ", __LINE__, TESTAPI);
			break;
		}
		ret=NDK_ScanState(pvalue, &plen);
		switch(ret)
		{
			case NDK_ERR_CAMERA_SCANNING:
				continue;
				break;
			case NDK_OK:
				NDK_ScanStop();
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
				{
					send_result("line %d:%s ����ɨ��������Ԥ�ڲ���", __LINE__, TESTAPI);
					goto ERR;
				}
				break;
			default:
				NDK_ScanStop();
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
		}
		if(ret == NDK_OK)
			break;
	}
	
	//NDK_ScanStart: NDK_ERR_CAMERA_THREAD   �����߳�ʧ��
	//NDK_ScanStop: NDK_ERR_CAMERA_SYNC   ����ͬ��ɨ��״̬  ��ͬ�������쳣��״̬�޷��ָ���ʱ����
	//NDK_ScanStop: NDK_ERR_CAMERA_STOP   �߳��޷�ֹͣ 
	//NDK_ScanState: NDK_ERR_CAMERA_READY_STOP   �߳̽�����ֹ
	//NDK_ScanState: NDK_ERR_CAMERA_STATE     δ֪״̬
	
	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScanClose();
	return;
}

