/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 7.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanStartɨ��ͷ��ʼɨ�衢NDK_ScanState��ȡɨ��״̬��NDK_ScanStopֹͣɨ�蹦��(�Ͷ˷�����ʽ)
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
*							zhengry			20170221		created
*
*****************************************************************/
void scan7(void)
{
	/*private & local definition*/
	int ret=0, plen=0, poutlen = 0, i = 0;
	uchar pvalue[500]={0}, poutvalue[500]={0};
	char pvaluein[17] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x00,0x31,0x32,0x33,0x34,0x35,0x36,0x0d,0x0a};
#if defined ME66
	time_t oldtime = 0;
#endif
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	 //case1:δ����ɨ��ͷ,ScanState��ȡɨ������Ӧ��ʧ��
#if CPU5810X_ENABLE	
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR_READ)
#else
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:��ʼɨ���,��û��ɨ��ͼ��,Ӧ����ʧ��
	if((ret=NDK_ScanStart()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if CPU5810X_ENABLE	
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR_READ)
#else
	if((ret=NDK_ScanState(pvalue, &plen)) != NDK_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��δֹͣɨ��,�ٴε��ÿ�ʼɨ��Ӧ���سɹ�
	if((ret=NDK_ScanStart()) != NDK_OK)
	{
		NDK_ScanStop();
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;//ʵ��scanstateɨ���������
	}

	//case4:��δֹͣɨ��,Ӧ�ܼ�������ɨ��
	cls_show_msg("��׼���ô�ɨ���ͼ��,���������");
	memset(pvalue,0,sizeof(pvalue));
	memset(poutvalue,0,sizeof(poutvalue));
#if defined ME66
	oldtime = time(NULL);
#else
	lib_StartStopwatch();
#endif
	while(1)
	{	
#if defined ME66
		if((time(NULL)-oldtime)>MAXTIME)
#else
		if(lib_ReadStopwatch()>MAXTIME)
#endif
		{
			NDK_ScanStop();
			send_result("line %d:%sɨ�賬ʱ", __LINE__, TESTAPI);
			break;
		}
		ret=NDK_ScanState(pvalue, &plen);
		switch(ret)
		{
#if CPU5810X_ENABLE
			case NDK_ERR_READ:
#else
			case NDK_ERR:
#endif
				continue;
				break;
			case NDK_OK:
				NDK_ScanStop();
#if defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME50NX||defined ME68
				if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", poutlen, poutvalue)!=ENTER)
#else
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
#endif
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

	//case7:���¿�ʼɨ���Ӧ������ɨ��
	cls_show_msg("��׼������һ����ɨ���ͼ��,���������");
	if((ret=NDK_ScanStart()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	plen=0;
	memset(pvalue,0,sizeof(pvalue));
	memset(poutvalue,0,sizeof(poutvalue));
#if defined ME66
	oldtime = time(NULL);
#else
	lib_StartStopwatch();
#endif
	while(1)
	{	
#if defined ME66
		if((time(NULL)-oldtime)>MAXTIME)
#else
		if(lib_ReadStopwatch()>MAXTIME)
#endif
		{
			NDK_ScanStop();
			send_result("line %d:%sɨ�賬ʱ", __LINE__, TESTAPI);
			break;
		}
		ret=NDK_ScanState(pvalue, &plen);
		switch(ret)
		{
#if CPU5810X_ENABLE
			case NDK_ERR_READ:
#else
			case NDK_ERR:
#endif
				continue;
				break;
			case NDK_OK:
				NDK_ScanStop();
#if defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME50NX||defined ME68
				if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", poutlen, poutvalue)!=ENTER)
#else
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
#endif
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

	//case8:��������֧�ֶ�ά�����������д�0x00�ֽ����ݵ�ɨ��֧�֣�0x00�޷���ʾ�����Բ����ڴ�Ƚϵķ������бȽϣ���Ҫɨ���ض��Ķ�ά��
	cls_show_msg("��ɨ��0x00�ֽڵĶ�ά�룬���������");
	if((ret=NDK_ScanStart()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	plen=0;
	memset(pvalue,0,sizeof(pvalue));
	memset(poutvalue,0,sizeof(poutvalue));
#if defined ME66
	oldtime = time(NULL);
#else
	lib_StartStopwatch();
#endif
	while(1)
	{	
#if defined ME66
		if((time(NULL)-oldtime)>MAXTIME)
#else
		if(lib_ReadStopwatch()>MAXTIME)
#endif
		{
			NDK_ScanStop();
			send_result("line %d:%sɨ�賬ʱ", __LINE__, TESTAPI);
			break;
		}
		ret=NDK_ScanState(pvalue, &plen);
		switch(ret)
		{
#if CPU5810X_ENABLE
			case NDK_ERR_READ:
#else
			case NDK_ERR:
#endif
				continue;
				break;
			case NDK_OK:
				NDK_ScanStop();
#if defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME50NX||defined ME68
				if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
					goto ERR;
				}
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", poutlen, poutvalue)!=ENTER)
#else
				if(cls_show_msg("����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
#endif
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
	if((plen!=15&&plen!=17)||memcmp(pvaluein,pvalue,plen))
	{
		send_result("line %d:%s����ʧ��(plen=%d)", __LINE__, TESTAPI, plen);
		//for(i=0;i<15;i++)
			//send_result("0x%2x ", pvalue[i]);
		goto ERR;
	}
		
	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
	return;
}


