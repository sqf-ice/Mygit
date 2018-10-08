/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: pppģ��
* file name		: ppp4.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PppCheck�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_PppCheck"	
#define MAXTIMEOUT	60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void ppp3(void)
* functional description 	: ��ȡPPP״̬
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void ppp4(void)
{
	/*private & local definition*/
	int nRet = 0;
	EM_PPP_STATUS emStatus=0;
	char szIpAddr[30]={0};
	int nErr=100;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
		
	//����PPP����ǰ����
	if((getlinktype()!=ASYN&&LinkUp()!=SUCC)
	||(getlinktype()==ASYN&&LinkUp4AsynPPP()!=SUCC))//if(LinkUp()!=SUCC)
	{
		send_result("line %d:������·ʧ��", __LINE__);
		return;
	}
	if((nRet = PppSetUsrCfg())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;	
	}

	//status����ΪNULL����·��ز�������
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	if((nRet=NDK_PppCheck(NULL, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;	
	}
#endif
	
	//�����ļ���״̬Ҫ�ڵײ��в��ܼ�⵽,���Բ��Գ���ֻ������Ӧ�ò��ܼ�⵽������״̬PHASE_DEAD��PHASE_NETWORK
	//zhangnw201105add δ��PPP,Ӧ�õ�PPP_STATUS_DISCONNECT
	if((nRet=NDK_PppCheck(&emStatus, &nErr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;	
	}
#if !(defined ME50NX||defined ME68||CPU5810X_ENABLE) //5810ƽ̨nErr��������  20180408 sull modify
	if(emStatus!=PPP_STATUS_DISCONNECT||nErr==100)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, emStatus, nErr);
		goto ppp_err;	
	}	
#else  //����50Nû��errֵ
	if(emStatus!=PPP_STATUS_DISCONNECT)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, emStatus, nErr);
		goto ppp_err;	
	}
#endif
	if(getlinktype()==ASYN&&(nRet=WM_PPP_16300_DIAL())<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;
	}	

	//����PPP_STATUS_CONNECTING��PPP_STATUS_CONNECTED̬
	if((nRet = NDK_PppDial(PPPUSR,PPPPWD))!=NDK_OK)
	{    
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;             
	}

	if((nRet=NDK_PppCheck(&emStatus, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;	
	}
#if defined ME68 || defined ME50NX //ac35ƽֻ̨֧�������Ӻ�δ����״̬
	if(emStatus!=PPP_STATUS_DISCONNECT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, emStatus);
		goto ppp_err;	
	}
#else
	if(emStatus!=PPP_STATUS_CONNECTING)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, emStatus);
		goto ppp_err;	
	}
#endif

	//������ΪgetPPPphase�ĵ�Ԫ����,��ͨ��GetPPPphase������,����Ҫͨ��WaitPPPState������
	//��Ԫ���Ե�ԭ��:����ֱ��ʹ��API�ӿ�,��Ҫʹ�����з�װ�ĺ���
	lib_StartStopwatch();
	while(1)
	{	
		NDK_SysDelay(10);//����ʱ��Ϊ�˽���������״̬��оƬ��ѹ��
		if((nRet=NDK_PppCheck(&emStatus, NULL))!=NDK_OK)
		{    
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;             
		}
		if(PPP_STATUS_CONNECTED==emStatus)
			break;
			
		if(lib_ReadStopwatch()>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, emStatus);
			goto ppp_err;
		}
	}

	//��һ�������붯̬IP�Ƿ�ɹ�(����һ��ȡ����,��ȡ����)         
	if((nRet = lib_GetLocalAddr( szIpAddr, NULL, NULL, NULL))!=SUCC)
	{    
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;             
	}

	//������һ�����Ժ�Ļ�����Ϊ��ʱ�Ĳ���ǰ��,����PHASE_DEAD
	if((nRet=NDK_PppHangup(0))!=NDK_OK)//ʹ�÷�����ʽ�Ҷ�
	{    
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;             
	}

	if((nRet=NDK_PppCheck(&emStatus, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		goto ppp_err;	
	}
#if !(defined ME50NX ||defined ME68)//ME50NXֻ֧�������ӣ�δ���ӣ������������ӣ����ڹҶϻ���֧�� sull 20171124
	if(emStatus!=PPP_STATUS_DISCONNECTING)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, emStatus);
		goto ppp_err;	
	}	
#endif
	
	//�еĻ��ͷ��ؿ�,�е���,���Ǹɴ�һֱ��״ֱ̬���õ�PHASE_DEAD,����ʱ����
	//p����ѹر�,״̬Ӧ�ûع��ʼ̬
	lib_StartStopwatch();
	while(1)
	{	
		NDK_SysDelay(10);//����ʱ��Ϊ�˽���������״̬��оƬ��ѹ��
		if((nRet=NDK_PppCheck(&emStatus, NULL))!=NDK_OK)
		{    
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;             
		}
		if(PPP_STATUS_DISCONNECT==emStatus)
			break;
			
		if(lib_ReadStopwatch()>MAXTIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, emStatus);
			goto ppp_err;
		}
	}
	
	//PPP FSM��״̬ԾǨ��˳����ȷ������ݲ�����
	//�����Ѳ�

	//test over
	send_result("%s����ͨ��", TESTAPI);	
ppp_err:
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
	NDK_PppHangup(1);
#else  //50N��֧�ַ������Ҷ�
	NDK_PppHangup(0);
#endif
	LinkDown();
	return;
}

