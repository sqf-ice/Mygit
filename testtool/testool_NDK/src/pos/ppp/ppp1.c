/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: pppģ��
* file name		: ppp1.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PppSetCfg,NDK_PppDial,NDK_PppHangup�ӿڹ���
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
#define TESTAPI	"NDK_PppSetCfg,NDK_PppDial,NDK_PppHangup"
#define MAXTIMEOUT	60

/*----------global variables declaration("extern")--------------*/

/*------------global variables definition-----------------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
//����atstr���ȴ���Ӧ(Ԥ��res)
static int WLM_get_rsp(EM_WLM_CMD AtCmdNo, char* atstr, EM_WLM_STATUS rsp, char *rspbuf)
{
	/*private & local definition*/
	ST_ATCMD_PACK st_ATCMD_PACK;
	char szOutput[1024]={0};
	EM_WLM_STATUS emStatus=0;

	/*process body*/
	memset(&st_ATCMD_PACK,0,sizeof(ST_ATCMD_PACK));
	st_ATCMD_PACK.AtCmdNo=AtCmdNo;
	strcpy(st_ATCMD_PACK.pcAtCmd,atstr);
	st_ATCMD_PACK.pcAddParam=NULL;

	if(NDK_WlSendATCmd(&st_ATCMD_PACK,szOutput,sizeof(szOutput),30000,&emStatus)!=NDK_OK)
		return FAIL;
	if(rsp!=emStatus)
		return FAIL;
	if(rspbuf!=NULL && !strstr(szOutput, rspbuf))
		return FAIL;

	return SUCC;
}

//��ǿ��WLM_get_rsp,����������/��ʱ����
static int WLM_get_rsp_Ex(EM_WLM_CMD AtCmdNo, char* atstr, EM_WLM_STATUS rsp, char *rspbuf)
{
	/*private & local definition*/
	int ret = FAIL;
	time_t diff = 0, oldtime = time(NULL);//��ʹ�ò������뺯��StartStopwatch()

	/*process body*/
	while((ret=WLM_get_rsp(AtCmdNo, atstr, rsp, rspbuf))!=SUCC)
	{
		if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
			break;
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}

	show_stopwatch(DISABLE, 0);
	return ret;
}

static int set_apn(void)
{
	/*private & local definition*/
	char ApnStr[96] = {0}; 
	
	/*process body*/
	sprintf(ApnStr, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//���ô����APN,Ҳ���ܷ���OK,����֮���PPP�����б���

	return WLM_get_rsp(WLM_CMD_UNDEFINE, ApnStr, WLM_STATUS_OK, NULL);
}

static int WLMDial(void)
{	
	time_t diff = 0, oldtime = 0;
	LinkType type = getlinktype();

	//����GPRS,CDMA����
	cls_printf("����%s������...\n", (type==GPRS)?"GPRS":((type==CDMA)?"CDMA":"TD"));

	//����·�����ñ�Ҫ�Ĳ���(��֮��������ʹ��)
	//���Ǳ����!WLM_get_res(WM_CGATT1, NULL, OK);
	if((type==GPRS||type==TD) && set_apn()!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:����APNʧ��!", __FILE__, __LINE__);
		return FAIL;
	}
	
	NDK_ScrDispString(0,16,"��ѯ��PIN��...  ",0);//16���ַ�
	if(WLM_get_rsp_Ex(WLM_CMD_UNDEFINE, "+CPIN?", WLM_STATUS_OK, "READY")!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:��ѯ��PINʧ��!", __FILE__, __LINE__);
		return FAIL;
	}

	//��DIALǰ,Ҫȷ��ģ���Ѹ��ŵ�������
	NDK_ScrDispString(0,16,"����������...  ",0);
	oldtime = time(NULL);//��ʹ�ò������뺯��StartStopwatch()
	while(((type==GPRS||type==TD) && WLM_get_rsp(WLM_CMD_UNDEFINE, "+CREG?", WLM_STATUS_OK, "1")!=SUCC && WLM_get_rsp(WLM_CMD_UNDEFINE, "+CREG?", WLM_STATUS_OK, "5")!=SUCC)
		||(type==CDMA && WLM_get_rsp(WLM_CMD_UNDEFINE, "+CAD?", WLM_STATUS_OK, "1")!=SUCC))
	{
		if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
		{
			cls_show_msg1(10, "%s, line %d:����ģ�鸽������ʧ��!", __FILE__, __LINE__);
			return FAIL;
		}
		show_stopwatch(ENABLE, diff);
		//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
	}
	show_stopwatch(DISABLE, 0);

	NDK_ScrDispString(0,16,"�κ���...         ",0);
	if(WLM_get_rsp_Ex(WLM_CMD_DIAL, NULL, WLM_STATUS_CONNECT, "CONNECT")!=SUCC)//"D*99***1#" "D#777",API�ײ���Զ�ѡ��
	{
		cls_show_msg1(10, "%s, line %d:����%s����ʧ��!", __FILE__, __LINE__, (type==GPRS)?"GPRS":((type==CDMA)?"CDMA":"TD"));
		return FAIL;
	}
	
	return SUCC;
}

/****************************************************************
* function name 	 				: void ppp1(void)
* functional description 	: ����PPP����,���ţ��Ҷ�
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void ppp1(void)
{
	/*private & local definition*/
	int nRet = 0;
	ulong ulLocalAddr=0;
	EM_PPP_STATUS emStatus=0;
	ST_PPP_CFG stPPPCfg;
	char szIpAddr[20]={0};
	LinkType type = getlinktype();

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//�����������Ϊ�˲���NetPPPHangup,һ��Ҷ�һ��ѡ������ʽ�Ҷ�NetPPPHangup(1),ʹ�÷�����ʽ�Ҷ�ʱҪ�ж�PPP״̬
	//Ĭ����ֻ�������ߵ�ppp��,����MODEM��PPPҪ���ðκź���,������̫һ��,�ʷֲ�ͬ�ķ�֧ȥ����

	//ModemDial��˵��(20130319):
	//�ײ��Ϊlinux��,Ӧ�ô��ĺ�����ַ,�ײ��޷�������Ӧ�ĺ�����.�ʶ����߶���,��ʹModemDial��NULL,��ʵ�Եײ����,Ч���봫NULLһ��.ModemDial��Ϊһ�����ݲ���(ʵ����������)
	//�����ߺ������첽è����,��NULLʱ,�ײ���Ӧ�ý�����·��İκ�����,���첽è����,����ModemDial���ݵĵ�ַ,�ײ������,����,����
	//�����,����Ӧ�ò������·��İκŵ��ò���,����������ͨ������ModemDial,�õײ�������.

	memset((void *)&stPPPCfg, 0, sizeof(stPPPCfg));
	
	switch(type)
	{	
	case CDMA:
		//case1:�����USR/PWD
		if((nRet=NDK_PppDial("newland_chensj", "newland_chensj"))!=NDK_OK/*NDK_ERR_PARA*/)//NDK�е�ʵ��ֻ�Ǹ����˺ţ����봫�ͳ�ȥ����������PPPserv����ģ������WaitPPPState�ǲ���ɹ��� 20131022 Ϳ־�����
		{	
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL))==SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
	case GPRS:
	case TD:
		//case2
		//��֤:�ڵ���NetPPPDialǰ������NetPPPCfg���ײ�Ĭ����pppoPenǰ�����Ŷ�������������ʽ�Ҷϣ�Ӧ�ɹ�
		//�ʲ��ص���linkup,����NetPPPDialȥ���
		if((nRet=NDK_PppDial(PPPUSR, PPPPWD))!=NDK_OK)//GPRS�����û������޹�,�ʹ���card/card
		{	
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL)) != SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		
		//��һ�������붯̬IP�Ƿ�ɹ�(��ʾ������̬ͨ) 
		if((nRet = lib_GetLocalAddr(szIpAddr,NULL,NULL,NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}
#if  !(CPU5810X_ENABLE||defined ME50NX||defined ME68) //50��֧�ַ������Ҷ�
		//case4:����NetPPPHangup(1)�ܷ�ɹ�����ʽ�Ҷ�
		if((nRet = NDK_PppHangup(1)) !=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		//����ʽ�ҶϺ�,Ӧ������⵽PPP_STATUS_DISCONNECT,��Ҫ��PPP_STATUS_DISCONNECT
		if((nRet = NDK_PppCheck(&emStatus, NULL))!=NDK_OK||emStatus!=PPP_STATUS_DISCONNECT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}
#else
		if((nRet = NDK_PppHangup(0)) !=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		//������ʽ�ҶϵȻ�����ȫ�Ҷ�
		if((nRet = WaitPPPState(PPP_STATUS_DISCONNECT, NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}
#endif		
		//case5(�쳣���Ծ�������case5����չ)
		//����NetPPPDialǰ����NetPPPCfg�������Լ��Ĳ��ź�������ô�ײ㻹��Ĭ����pppoPenǰ�����Ŷ��������÷�����ʽ�Ҷϣ�Ӧ�ɹ�
		//���ò�����Ϣ
		if(type==GPRS||type==TD)
		{
			strcpy(stPPPCfg.szDailNum, "*99***1#");
		}
		else if(type==CDMA)
		{
			strcpy(stPPPCfg.szDailNum, "#777");
		}
			
		stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)	
		stPPPCfg.nPPPFlag = LCP_PPP_KEEP;
		sprintf(stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(stPPPCfg.szApn, "\"%s\"", WLMAPN);
#else 
		stPPPCfg.nPPPFlag = LCP_PPP_UNAUTOCONNECT; //nPPPFlag��������Ϊ:�Զ�������־,��Ԫ���Բ��Զ�����,�Զ������������ۺϵ��쳣�����в���
		sprintf(stPPPCfg.szApn, WLMAPN);	//�Ͷ�PPP����APNֻ��ֱ������,��������CGD���ַ���
#endif
		stPPPCfg.ModemDial = WLMDial;
		stPPPCfg.PPPIntervalTimeOut = 600;

		//case6:NDK_PppSetCfg�쳣����
		if((nRet = NDK_PppSetCfg(NULL,0))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
#if  !(CPU5810X_ENABLE||defined ME50NX||defined ME68) //�Ͷ˲�Ʒ���Ȳ�����Ч,��������
		if((nRet = NDK_PppSetCfg(&stPPPCfg, sizeof(stPPPCfg)-1))!=NDK_ERR_PPP_PARAM)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
#endif
		if((nRet = NDK_PppSetCfg(&stPPPCfg, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		
		//case7:NDK_PppDial�쳣����
		if((nRet = NDK_PppDial(NULL, NULL))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		
		//����
		NDK_PppDial(PPPUSR, PPPPWD);//���ﲻ��NetPPPDial��,�ʲ��з���
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}
		
		//��һ�������붯̬IP�Ƿ�ɹ�(��ʾ������̬ͨ) 
		if((nRet = lib_GetLocalAddr(szIpAddr,NULL,NULL,NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}	

		//case3:����NetPPPHangup �쳣����
		if((nRet = NDK_PppHangup(-1)) !=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		if((nRet = NDK_PppHangup(10)) !=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		
		//case8:����NetPPPHangup(0)�ܷ�ɹ��������Ҷ�
		if((nRet = NDK_PppHangup(0)) !=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}

		//case9:���Է������Ҷ�ȡ״̬ӦΪ���ڹҶ�
#if !(defined ME50NX ||defined ME68) //ME50NXֻ֧�������ӣ�δ���ӣ������������ӣ����ڹҶϻ���֧�� sull 20171124
		if((nRet = NDK_PppCheck(&emStatus, NULL))!=NDK_OK||emStatus!=PPP_STATUS_DISCONNECTING)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI,nRet,emStatus);
			goto ppp_err;	
		}
#endif

		//������ʽ�ҶϵȻ�����ȫ�Ҷ�
		if((nRet = WaitPPPState(PPP_STATUS_DISCONNECT, NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}
#if  !(CPU5810X_ENABLE||defined ME50NX||defined ME68)  //50N �� nPPPFlag����Ϊ�Ƿ�֧���Զ����� ���ۺ��쳣�����в�
		//case10:����nPPPFlag�ܷ�֧�ֶ�����
		if(type==GPRS||type==TD)
		{
			strcpy(stPPPCfg.szDailNum, "*99***1#");
		}	
		else if(type==CDMA)
		{
			strcpy(stPPPCfg.szDailNum, "#777");
		}
		stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
		stPPPCfg.nPPPFlag = LCP_PPP_UNKEEPED;//��ά�ֳ�����LCP_PPP_UNKEEPED
		sprintf(stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(stPPPCfg.szApn, "\"%s\"", WLMAPN);
	//	strcpy(stPPPCfg. szUserName,"card");
	//	strcpy(stPPPCfg. szUserPasswd,"card");
		
		stPPPCfg.ModemDial = NULL;//���֮ǰ�����ù�,��ʱ������NULL,����ʹ��֮ǰ�İκź���
		stPPPCfg.PPPIntervalTimeOut = 600;

		if((nRet = NDK_PppSetCfg (&stPPPCfg, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;
		}
		
		nRet = NDK_PppDial(PPPUSR, PPPPWD);//���ﲻ��NetPPPDial��,�ʲ��з���
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}
		
		//��һ�������붯̬IP�Ƿ�ɹ�(��ʾ������̬ͨ) 
		if((nRet = lib_GetLocalAddr(szIpAddr,NULL,NULL,NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}
		NDK_PppHangup(1);
		if((nRet = NDK_PppCheck(&emStatus, NULL))!=NDK_OK||emStatus!=PPP_STATUS_DISCONNECT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}		
		
		//case11:����nPPPFlag�ܷ�֧�ֳ�����
		if(type==GPRS||type==TD)
		{
			strcpy(stPPPCfg.szDailNum, "*99***1#");
		}	
		else if(type==CDMA)
		{
			strcpy(stPPPCfg.szDailNum, "#777");
		}
		stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
		stPPPCfg.nPPPFlag = LCP_PPP_KEEP;//ά�ֳ�����
		sprintf(stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(stPPPCfg.szApn, "\"%s\"", WLMAPN);
	//	strcpy(stPPPCfg. szUserName,"card");
	//	strcpy(stPPPCfg. szUserPasswd,"card");
		
		stPPPCfg.ModemDial = NULL;
		stPPPCfg.PPPIntervalTimeOut = 600;

		if((nRet=NDK_PppSetCfg (&stPPPCfg, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}

		nRet = NDK_PppDial(PPPUSR, PPPPWD);	//���ﲻ��NetPPPDial��,�ʲ��з���
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}
		
		//��һ�������붯̬IP�Ƿ�ɹ�(��ʾ������̬ͨ) 
		if((nRet = NDK_PppGetAddr(&ulLocalAddr,NULL))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}
		NDK_PppHangup(1);
		if((nRet = NDK_PppCheck(&emStatus, NULL))!=NDK_OK||emStatus!=PPP_STATUS_DISCONNECT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;	
		}		
#endif
		//test over
		send_result("%s����ͨ��", TESTAPI);
		goto ppp_err;
		break;
	case ASYN:
		//�ײ�Ĭ�ϲ���æ������PPP���ź������ܣ������ҪӦ�ò���Ա�Լ����벦�ź�����
		//case12:���Ե��� ��cfg��δ����κź���ʱ,NetPPPDiaLӦʧ��(Ŀǰ�����(�κ��Ƿ�������)����ʹ����Ϊ��NetPPPDiaL�ɹ�,��֮�� ȡ״̬Ӧȡ����CNNTED̬)
		strcpy(stPPPCfg.szDailNum, MDMDialStr);
		stPPPCfg.nDevType = 1;//0��ʾ ����,1Ϊ����
		//stPPPCfg.nPPPFlag = 1;
		//strcpy(stPPPCfg.szApn, "CMNET");
		//strcpy(stPPPCfg. szUserName,"card");
		//strcpy(stPPPCfg. szUserPasswd,"card");
		
		stPPPCfg.ModemDial = NULL;
		stPPPCfg.PPPIntervalTimeOut = 600;

		if((nRet=NDK_PppSetCfg (&stPPPCfg, sizeof(stPPPCfg)/*0*/))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}

		if((nRet=NDK_PppDial(DEFAULT_ASYN_USR, DEFAULT_ASYN_PWD))!=NDK_OK)//>=0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL))==SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;		
		}
		
		//case13:���Ե��� ��cfg�Ҵ���κź���ʱ,NetPPPDiaLӦ�ɹ�(������Ƹ�ΪӦ���Ƚ��аκ�,�ٵ���NDK_PppDial.by zhangnw20130319)
		strcpy(stPPPCfg.szDailNum, MDMDialStr);
		stPPPCfg.nDevType = 1;//0��ʾ ����,1Ϊ����
		//stPPPCfg.nPPPFlag = 1;
		//strcpy(stPPPCfg.szApn,"CMNET");
		//strcpy(stPPPCfg. szUserName,"card");
		//strcpy(stPPPCfg. szUserPasswd,"card");
		
		stPPPCfg.ModemDial = NULL;//ASYNDial;
		stPPPCfg.PPPIntervalTimeOut = 600;

		if((nRet=NDK_PppSetCfg (&stPPPCfg, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}

		if((nRet=ASYNDial4PPP())!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;		
		}
		if((nRet=WM_PPP_16300_DIAL())<0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;		
		}
		
		if((nRet=NDK_PppDial(DEFAULT_ASYN_USR, DEFAULT_ASYN_PWD))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}
		if((nRet = WaitPPPState(PPP_STATUS_CONNECTED, NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}
		if((nRet=NDK_PppDial(DEFAULT_ASYN_USR, DEFAULT_ASYN_PWD))!=NDK_ERR_PPP_OPEN)//�Ѿ�����֮����ȥ����NDK_PppDialӦ�÷���NDK_ERR_PPP_OPEN
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}

		//��һ�������붯̬IP�Ƿ�ɹ�(��ʾ������̬ͨ) 
		if((nRet = NDK_PppGetAddr(&ulLocalAddr,NULL))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}
		//����NetPPPHangup(1)�ܷ�ɹ�����ʽ�Ҷ�
		if((nRet=NDK_PppHangup(1))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;
		}
#if 1
		if((nRet = NDK_PppCheck(&emStatus, NULL))!=NDK_OK||emStatus!=PPP_STATUS_DISCONNECT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			goto ppp_err;	
		}
#else		
		//zhangnw201105add
		if((nRet = WaitPPPState(PPP_STATUS_DISCONNECT , NULL))!=SUCC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
			goto ppp_err;		
		}
#endif

#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE
		NDK_MdmHangup();//��Ӧ�����ASYNDial����
#endif		
		//test over
		send_result("%s����ͨ��", TESTAPI);
		goto ppp_err;
		break;
	default:
		send_result("%s����ʧ��:δ֪����·����(%d)", TESTAPI, type);
		goto ppp_err;
		break;
	}
ppp_err:
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
	NDK_PppHangup(1);
#else  //50N��֧�ַ������Ҷ�
	NDK_PppHangup(0);
#endif
	PppSetUsrCfg();
#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE
	if(type==ASYN)
		NDK_MdmHangup();//��Ӧ�����ASYNDial����	
#endif	
	return;
}

