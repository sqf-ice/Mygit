/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 3.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanDoScan�Ƿ������������ɨ��ͷ/����ͷ����ʽɨ�蹦��(����ʽ)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScanDoScan"
#define		MAXTIMEOUT 10
#define		WUCHASEC	0.5//0.03 5810xƽ̨��Ʒ��ʱǰ�����˳�ʼ����Ҫ340ms���ж˲�Ʒ��ʱ�����,�����ʱ�ȸĳ�0.5s  20180516 modify

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan3
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*										jiangym			20130917		created
*
*****************************************************************/
void scan3(void)
{
	/*private & local definition*/
	int ret = 0, plen = 0, poutlen = 0;
	char pvalue[500] = {0}, poutvalue[500] = {0};//50�ĳ�500,����ɨ���������ݳ���50�ֽ������ܷ�
	char pvaluein[17] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x00,0x31,0x32,0x33,0x34,0x35,0x36,0x0d,0x0a};
	float tmp = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��ģ���ʼ��
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case1:�������� nTimeOut=-1,pszValue,pnLen=NULL
	if((ret=NDK_ScanDoScan(-1,pvalue,&plen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScanDoScan(MAXTIMEOUT,NULL,&plen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScanDoScan(MAXTIMEOUT,pvalue,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2: ����ntimeout=0,����ɨ�賬ʱ
	if((ret=NDK_ScanDoScan(0,pvalue,&plen))!=NDK_ERR_TIMEOUT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:���Գ�ʱʱ�����δɨ�赽�����˳�,�ҳ�ʱ�������������Χ��
	cls_printf("����ɨ��,�ȴ���ʱ�˳�");
	lib_StartStopwatch();
	if((ret=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_ERR_TIMEOUT||((tmp = lib_ReadStopwatch()-MAXTIMEOUT)>WUCHASEC))
	{
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(tmp,str);
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, str);
#else	
		send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
#endif
		GOTOERR;
	}

	//case5:��������֧�ֶ�ά�����������д�0x00�ֽ����ݵ�ɨ��֧�֣�0x00�޷���ʾ�����Բ����ڴ�Ƚϵķ������бȽϣ���Ҫɨ���ض��Ķ�ά��  20150317 linwl
	cls_printf("����20���ڽ���ɨ���0x00�ֽڵĶ�ά��");
	plen=0;
	memset(pvalue,0,sizeof(pvalue));
	if((ret=NDK_ScanDoScan(MAXTIMEOUT*2,pvalue,&plen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((plen!=15&&plen!=17)||memcmp(pvaluein,pvalue,plen))
	{
		send_result("line %d:%s����ʧ��(plen=%d)", __LINE__, TESTAPI, plen);
		//for(i=0;i<plen;i++)
		//	cls_printf("0x%2x ", pvalue[i]);
		//wait_anykey(0);
		GOTOERR;
	}
	
	//case4:��������,Ӧ���ܹ���ȡ��ɨ�����ݺ�ɨ�����ݳ���
	//�˴����������ϵҪ����Ҫ�Բ��Բ��ռ����ĸ������͵����붼�Ѿ�ɨ����֤
	while(1)
	{
		if(cls_show_msg1(2,"����20���ڽ���ɨ��,ȡ�����˳�")==ESC)
			break;
		
		plen=0;
		memset(pvalue,0,sizeof(pvalue));
		memset(poutvalue,0,sizeof(poutvalue));
		if((ret=NDK_ScanDoScan(MAXTIMEOUT*2,pvalue,&plen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if defined ME50||defined ME50N||defined ME50C||defined ME50H		
		if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", poutlen, poutvalue)!=ENTER)
#else
		if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
#endif
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	
	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
#if SCAN_SOFT_ENABLE
	NDK_ScanExit();
#endif
	return;
}

