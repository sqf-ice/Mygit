/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: PPP ͨѶģ��
* file name			: 
* Author 			: liny
* version			: 
* DATE				: 
* directory 			: 
* description			: ����PPP ͨѶģ�����������
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/

#define	TESTAPI	"PPPģ����������"
#define MAXTIMEOUT 60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
    PppSetCfg,
    PppDial,
    PppHangup,
    PppCheck,
#if !K21_ENABLE
    PppAddrChange,
    PppGetAddr,
    NetGetAddr,    
#endif
#if CPU5810X_ENABLE
    PppGetNetAddr,
#endif
	MaxNum,
}EM_PPP;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*
*****************************************************************/
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
	
	NDK_ScrDispString(0,16,"��ѯ��PIN��...	",0);//16���ַ�
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

	NDK_ScrDispString(0,16,"�κ���...		  ",0);
	if(WLM_get_rsp_Ex(WLM_CMD_DIAL, NULL, WLM_STATUS_CONNECT, "CONNECT")!=SUCC)//"D*99***1#" "D#777",API�ײ���Զ�ѡ��
	{
		cls_show_msg1(10, "%s, line %d:����%s����ʧ��!", __FILE__, __LINE__, (type==GPRS)?"GPRS":((type==CDMA)?"CDMA":"TD"));
		return FAIL;
	}
	
	return SUCC;
}

static int ppp_randtest(int funnum)
{ 
	ST_PPP_CFG stPPPCfg;
	EM_PPP_STATUS emStatus=0;
	int nErr=100;
#if !K21_ENABLE 
	ulong ulIpAddr=0, ulHostAddr=0, ulLocalAddr=0;
	char szGateway[16] = {0}, szMaskaddr[16] = {0}, pszDns[16] = {0}, szIPaddr[16] = {0};
#elif CPU5810X_ENABLE
	char LocalIPAddr[17] = {0};
#endif
	LinkType type = getlinktype();

    	switch(funnum)
	{	
		case PppSetCfg:  
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
			sprintf(stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);
#else 
			stPPPCfg.nPPPFlag = LCP_PPP_UNAUTOCONNECT; //nPPPFlag��������Ϊ:�Զ�������־,��Ԫ���Բ��Զ�����,�Զ������������ۺϵ��쳣�����в���
			sprintf(stPPPCfg.szApn, WLMAPN);	//�Ͷ�PPP����APNֻ��ֱ������,��������CGD���ַ���
#endif
			stPPPCfg.ModemDial = WLMDial;
			stPPPCfg.PPPIntervalTimeOut = 600;
			NDK_PppSetCfg(&stPPPCfg, 0);
			break;
		case PppDial:
			NDK_PppDial(PPPUSR, PPPPWD);
			break;
		case PppHangup:
			NDK_PppHangup(rand()&2);
			break;
		case PppCheck:
			NDK_PppCheck(&emStatus, &nErr);
			break;

#if !K21_ENABLE
        	case PppAddrChange:
		        NDK_PppAddrChange("192.168.2.1",&ulIpAddr);
			break;
		case PppGetAddr:
			NDK_PppGetAddr(&ulLocalAddr, &ulHostAddr);
			break;
		case NetGetAddr:    
			NDK_NetGetAddr(COMM_TYPE_PPP, szIPaddr, szMaskaddr, szGateway, pszDns);
			break;
#endif

#if CPU5810X_ENABLE
        case PppGetNetAddr:
			NDK_PppGetNetAddr(LocalIPAddr);
			break;
#endif		

	}
	return SUCC;
}

void ppp200(void)
{
	/*private & local definition*/
	int num[50] ={0}, i = 0, bak = 0;
	int ret = -1,cnt =50, len = 0,j=0;//cnt ����
#if CPU5810X_ENABLE
		char *pppstr[] = {"NDK_PppSetCfg","NDK_PppDial","NDK_PppHangup","NDK_PppCheck","NDK_PppGetNetAddr"};
#endif
#if defined ME68||defined ME50NX
		char *pppstr[] = {"NDK_PppSetCfg","NDK_PppDial","NDK_PppHangup","NDK_PppCheck"};
#endif
#if !K21_ENABLE
		char *pppstr[] = {"NDK_PppSetCfg","NDK_PppDial","NDK_PppHangup","NDK_PppAddrChange","NDK_PppGetAddr","NDK_PppCheck"};
#endif		

	/*process body*/
	len = (MaxNum>20)?MaxNum:20;
	while(cnt)
	{	
		cnt--;
		bak++;

		//���������
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
		}
		cls_printf("��%d�����˳��:",bak);
		for(i=0;i<len;i++)
		{
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
			
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret = ppp_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",pppstr[num[j]]);
				goto ERR;
			}
		}	
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
	LinkDown();
	return;
}


