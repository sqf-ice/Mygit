/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: wlmͨѶģ��
* file name			: 
* Author 			: liny
* version			: 
* DATE				: 20171201
* directory 			: 
* description			: ����wlm ͨѶģ�����������
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"
#define	WLM_TIMEOUT	2000
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
#define	CHECK_TIMEOUT	2000
#endif

/*---------------constants/macro definition---------------------*/

#define	TESTAPI	"wlmģ����������"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
  	WlInit,
	WlModemGetSQ,
	WlSendATCmd,	
	WlModemReset,
	WlModemClose,
	WlGetInfo,
	WlGetStationInfo,
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
    	WLCheckSIM,
#endif
#if !K21_ENABLE //����֧�ִ˺��������йر�
    	WlGetStationInfoGeneral,
#endif
	MaxNum,
}EM_WLM;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: wlm ͨѶģ�������Ϻ���
* functional description 	: ����wlm ͨѶģ�����������
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*                                 linying      20180211       cteated
*****************************************************************/
static int wlm_randtest(int funnum)
{ 
    EM_WLM_STATUS emStatus = -1;
	ST_MOBILE_STATION_INFO pstinfo;
	int sq =0;
	ST_ATCMD_PACK STatcmdpack = {0};
	char szOutbuf[1024] = {0};
	char pszvalue[25] = {0};
	uchar AtCmdNo[]={ WLM_CMD_E0,WLM_CMD_CSQ,WLM_CMD_CREG,WLM_CMD_CPIN,WLM_CMD_CPIN0,WLM_CMD_CGATT0,WLM_CMD_CGATT1,WLM_CMD_DIAL,WLM_CMD_D2,WLM_CMD_COPS,WLM_CMD_CGMR,WLM_CMD_CGSN,WLM_CMD_CCID,				/**<��ȡ����SIM�����к�*/
    	WLM_CMD_END,WLM_CMD_UNDEFINE,WLM_CMD_PUBLIC};
	uchar WLM_TYPE_INFO[]={ WLM_INFO_CCID,WLM_INFO_IMSI,WLM_INFO_IMEI};
#if !K21_ENABLE
	uchar STATION_TYPE[]={STATION_TYPE_DEFAULT,STATION_TYPE_GSM,STATION_TYPE_LTE,STATION_TYPE_TDSCDMA,STATION_TYPE_WCDMA,STATION_TYPE_CDMAHDR};
#endif

	srand((unsigned)time(NULL));
    	switch(funnum)
	{	
	    case WlInit:
			NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus);
			break;
		case WlModemGetSQ:
			NDK_WlModemGetSQ(&sq);
			break;
		case WlSendATCmd:
			memset(szOutbuf, 0, sizeof(szOutbuf));
			STatcmdpack.AtCmdNo = AtCmdNo[rand()%sizeof(AtCmdNo)];
		        STatcmdpack.pcAddParam = NULL;
		        STatcmdpack.pcAtCmd = NULL;
		        NDK_WlSendATCmd(&STatcmdpack, szOutbuf,sizeof(szOutbuf), 30000,&emStatus);	
			break;
		case WlModemReset:
			NDK_WlModemReset();
			break;
		case WlModemClose:
			NDK_WlModemClose();
			break;
		case WlGetInfo:
			memset(pszvalue,0,sizeof(pszvalue));
	        	NDK_WlGetInfo(WLM_TYPE_INFO[rand()%3],pszvalue,sizeof(pszvalue));
			break;
		case WlGetStationInfo:
			NDK_WlGetStationInfo(&pstinfo);
			break;
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
        case WLCheckSIM:
			NDK_WLCheckSIM("1234",CHECK_TIMEOUT);
			break;
#endif

#if !K21_ENABLE  //����֧�ִ˺��������йر�
		case WlGetStationInfoGeneral:
			NDK_WlGetStationInfoGeneral(STATION_TYPE[rand()%sizeof(STATION_TYPE)],&pstinfo);
#endif
    }
    return SUCC;
}

void wlm200(void)
{
	/*private & local definition*/
	int num[50] ={0}, i = 0, bak = 0;
	int ret = -1,cnt =50, len = 0,j=0;//cnt ����
#if CPU5810X_ENABLE||defined ME68||defined ME50NX
	char *wlmstr[] = {"NDK_WlInit","NDK_WlModemGetSQ","NDK_WlSendATCmd","NDK_WlModemReset","NDK_WlModemClose","NDK_WlGetInfo","NDK_WlGetStationInfo","NDK_WLCheckSIM"};	
#endif
#if !K21_ENABLE
    char *wlmstr[] = {"NDK_WlInit","NDK_WlModemGetSQ","NDK_WlSendATCmd","NDK_WlModemReset","NDK_WlModemClose","NDK_WlGetInfo","NDK_WlGetStationInfo","NDK_WlGetStationInfoGeneral"};	
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
			if((ret = wlm_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",wlmstr[num[j]]);
				goto ERR;
			}
		}	
		
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NetDown();
	NDK_WlModemReset();//Ӧ�����������Ӹ�λҪ�󣬷�ֹģ��رպ󣬲�����Ա���������߲�����������bug������20180329 linying added
	return;
}


