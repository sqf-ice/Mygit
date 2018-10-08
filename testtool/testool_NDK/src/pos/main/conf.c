
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ���Գ�������һ�庯����
* file name		: conf.c
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20101217
* directory 		: .\SRC\main\
* description		: 
* related document	: 
* 
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	MAXWAITTIME 60
#define	WAITTIME  5
/*---------------functions declaration--------------------------*/
#if !(defined ME15CHX||defined ME15B||defined ME66)//ME15CΪ��ʡ����ռ������������ 20170213 linwl
static int conf_WLAN_lnk(void);
int use_wifi_conf(int ApID);
#endif
/*--------------------ͨѶģ���Ӧ�Ĳ��ұ�--------------------------*/
uint g_conf_flag = 0; //Ϊ1��ʱ��ʹ�������ļ�

/*---------------functions definition---------------------------*/
#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)	//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ���
#if PRN_ENABLE
#if PRNGREY_ENABLE
uint g_PrnGrey = DEFAULT_GREY_VLE;
static int conf_prn_grey(void)//ע��:��ӡ����λ��Ӧ���ҶȲ����ָ�
{
	/*private & local definition*/
	char grey[4] = {0};
	int ret = 0;
	uint len = 0;

	/*process body*/
	if(ENTER!=cls_show_msg1(10, "�������ô�ӡ�Ҷ���?\n"                                                   
											"[ȷ��]��[����]��\n"))
		return SUCC;
	do
	{
		cls_printf("������Ҷ�ֵ(%d-%d):", GREY_MINVALID, GREY_MAXVALID);
		sprintf(grey, "%d", g_PrnGrey); 
		if ((ret=lib_kbgetinput(grey, 0, sizeof(grey)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			g_PrnGrey = DEFAULT_GREY_VLE;
		else
			g_PrnGrey = atoi(grey);
	}while(g_PrnGrey<GREY_MINVALID || g_PrnGrey>GREY_MAXVALID);
/*	
	if ((ret=NDK_PrnSetGreyScale(g_PrnGrey))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:���ô�ӡ�Ҷ�ʧ��", __FILE__, __LINE__);
		return ret;
	}
*/	
	cls_show_msg1(2, "���ô�ӡ�Ҷ�(%d��)�ɹ�!", g_PrnGrey);
	return SUCC;
}
#else
static int conf_prn_grey(void){return FAIL;}
#endif   //��ӦPRNGREY_ENABLE

#if PRNUNDERLINE_EN
EM_PRN_UNDERLINE_STATUS g_PrnUnderline = PRN_UNDERLINE_STATUS_CLOSE;//0
static void conf_prn_underline(void)
{
	/*private & local definition*/
	int nKeyin = 0;//, ret = 0;

	/*process body*/
	nKeyin = cls_show_msg1(10, "�Ƿ�����ӡ�»��߹���?\n"                                                   
											"[ȷ��]��[����]��\n");
	g_PrnUnderline = (nKeyin==ENTER)?PRN_UNDERLINE_STATUS_OPEN:PRN_UNDERLINE_STATUS_CLOSE;
/*
	if ((ret=NDK_PrnSetUnderLine(g_PrnUnderline))!=NDK_OK)
	{
		cls_show_msg1(10, "%s, line %d:���ô�ӡ�»��߹���ʧ��", __FILE__, __LINE__);
		return;
	}
*/	
	cls_show_msg1(2, "%s��ӡ�»��߹��ܳɹ�!", g_PrnUnderline==PRN_UNDERLINE_STATUS_OPEN?"����":"�ر�");
	return;
}
#else
static void conf_prn_underline(void){}
#endif   //��ӦPRNUNDERLINE_EN

uint g_PrnSwitch = 0;//��ӡ���ͱߴ򿪹�,Ĭ�Ϲر�
static void conf_prn_switch(void)
{
	/*private & local definition*/

	/*process body*/
	g_PrnSwitch = (cls_show_msg1(10, "�Ƿ������ͱߴ���?\n[ȷ��]��[����]��\n")==ENTER)?1:0;
	return;
}

void conf_prn(void)
{
	/*private & local definition*/

	/*process body*/
	conf_prn_switch();
	conf_prn_grey();
	conf_prn_underline();
}

int lib_initprn(uint unSwitch)
{
	int ret = 0;

	//��ʼ��
	if((ret=NDK_PrnInit(unSwitch)) != NDK_OK)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ��ʼ��ʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;
	}	
	//�����»�������
#if PRNUNDERLINE_EN
	if ((ret=NDK_PrnSetUnderLine(g_PrnUnderline))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:���ô�ӡ�»��߹���%dʧ��(%d)", __FILE__, __LINE__, g_PrnUnderline,ret);
		return ret;
	}
#endif
	//���ûҶ�
#if PRNGREY_ENABLE
	if ((ret=NDK_PrnSetGreyScale(g_PrnGrey))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:���ô�ӡ�Ҷ�%dʧ��(%d)", __FILE__, __LINE__, g_PrnGrey,ret);
		return ret;
	}
#endif
	return ret;
}

#else
void conf_prn(void){return;}
int lib_initprn(uint unSwitch){return SUCC;}
#endif

#if PPP_ENABLE
int PppSetUsrCfg(void)
{
	/*private & local definition*/
	//int ret = 0;
	ST_PPP_CFG tmp_stPPPCfg;
	LinkType type = getlinktype();	

	/*process body*/
	memset((void *)&tmp_stPPPCfg, 0, sizeof(tmp_stPPPCfg));
	if(type==ASYN)
	{	
		memset(&tmp_stPPPCfg,0,sizeof(tmp_stPPPCfg));
		strcpy(tmp_stPPPCfg.szDailNum, MDMDialStr);
		tmp_stPPPCfg.nDevType = 1;//0��ʾ ����,1Ϊ����
		//tmp_stPPPCfg.nPPPFlag = 1;
		//strcpy(tmp_stPPPCfg.szApn, "CMNET");
		tmp_stPPPCfg.ModemDial = NULL;//ASYNDial;//�ײ�Ĭ�ϲ���æ������PPP���ź������ܣ������ҪӦ�ò���Ա�Լ����벦�ź�����
		tmp_stPPPCfg.PPPIntervalTimeOut = 600;

		//Ŀǰ��conf_MDM_lnk��conf_PPP�о��޶�����2���첽PPP����������,���ڴ˴�д��
		strcpy(PPPUSR, DEFAULT_ASYN_USR);
		strcpy(PPPPWD, DEFAULT_ASYN_PWD);
	}
	else
	{
		memset(&tmp_stPPPCfg,0,sizeof(tmp_stPPPCfg));//20141201 3Gģ��֧�ֺ��޸�Ӧ�ò���봫����ȷ���Ŵ������������ʹ���ϴεĲ��Ŵ� 
		if(type==CDMA)
			strcpy(tmp_stPPPCfg.szDailNum, "#777");
		else
			strcpy(tmp_stPPPCfg.szDailNum, "*99***1#");
		tmp_stPPPCfg.nDevType = 0;//0��ʾ ����,1Ϊ����
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)//�Ͷ�֧��PPP���ܽ�֧��APN����,����������Ч 20160527
		tmp_stPPPCfg.nPPPFlag = LCP_PPP_UNKEEPED;//������
		sprintf(tmp_stPPPCfg.szApn, "+CGDCONT=1,\"IP\",\"%s\"", WLMAPN);//sprintf(tmp_stPPPCfg.szApn, "\"%s\"", WLMAPN);
#else
		tmp_stPPPCfg.nPPPFlag = LCP_PPP_UNAUTOCONNECT; //���ò��Զ�����
		sprintf(tmp_stPPPCfg.szApn, WLMAPN);
#endif
		tmp_stPPPCfg.ModemDial = NULL;
		//tmp_stPPPCfg.PPPIntervalTimeOut = 600;
	}
	
#if 0
	if((ret=NDK_PppSetCfg(&tmp_stPPPCfg, sizeof(tmp_stPPPCfg)))!=NDK_OK)//if((ret=NetPPPCfg(&tmp_stPPPCfg, 0))<0)
	{
		//cls_show_msg1(10, "%s, line %d:��·���������ʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;//FAIL;
	}
#else
	return NDK_PppSetCfg(&tmp_stPPPCfg, sizeof(tmp_stPPPCfg));
#endif
}

//APN,USR/PWD���ڰ���PPPʱʹ�õĲ���
//APNΪGPRS��TD����ʱ��֤ʹ��,USR/PWDΪCDMA��ASYN����ʱ��֤ʹ��
static int conf_PPP(void)
{
	/*private & local definition*/
	int ret = 0;
	uint len = 0;
	LinkType type = getlinktype();	
	char pInfo[64] = {0};
	char wlm_apn[64] = {0}, ppp_usr[8] = {0}, ppp_pwd[8] = {0};

	/*process body*/
	if(g_conf_flag)
	{
		if(type==GPRS||type==TD)
		{
			if(GetParaInfo("WLMAPN",pInfo)!=SUCC)
				return FAIL;
			if(trim_string(pInfo,wlm_apn)!=SUCC)
				return FAIL;
			strcpy(WLMAPN,wlm_apn);
		}
		if(type!=ASYN)
		{
			if(GetParaInfo("PPPUSR",pInfo)!=SUCC)
				return FAIL;
			if(trim_string(pInfo,ppp_usr)!=SUCC)
				return FAIL;
			strcpy(PPPUSR,ppp_usr);
			if(GetParaInfo("PPPPWD",pInfo)!=SUCC)
				return FAIL;
			if(trim_string(pInfo,ppp_pwd)!=SUCC)
				return FAIL;
			strcpy(PPPPWD,ppp_pwd);
		}
		cls_show_msg1(1,"PPP�������óɹ�");
	}
	else
	{
		if(type==GPRS||type==TD)
		{
			cls_printf("��ǰAPN:\n");
			//if ((ret=getreturnline(gprsAPN, sizeof(gprsAPN)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
			if ((ret=lib_kbgetinput(WLMAPN, 0, sizeof(WLMAPN)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT/*INPUT_CONTRL_LIMIT*/))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				strcpy(WLMAPN, DEFAULT_WLM_APN);//strcpy(gprsAPN, DEFAULT_WLM_APN);
		}
		//����ΪPPP���ù��е�
		if(type!=ASYN)//ASYN�����ݲ�����,ֱ��д��.��ʹ���õĻ�,��PppSetCfg��Ҳ�ᱻд��
		{
		cls_printf("��ǰPPP�û���:\n");
		//if ((ret=getreturnline(gprsusr, sizeof(gprsusr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
		if ((ret=lib_kbgetinput(PPPUSR, 0, sizeof(PPPUSR)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT/*INPUT_CONTRL_LIMIT*/))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(PPPUSR, (type==ASYN)?DEFAULT_ASYN_USR:DEFAULT_PPP_USR);//strcpy(gprsusr, DEFAULT_WLM_USR);

		cls_printf("��ǰPPP����:\n");
		//if ((ret=getreturnline(gprspwd, sizeof(gprspwd)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
		if ((ret=lib_kbgetinput(PPPPWD, 0, sizeof(PPPPWD)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT/*INPUT_CONTRL_LIMIT*/))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(PPPPWD, (type==ASYN)?DEFAULT_ASYN_PWD:DEFAULT_PPP_PWD);//strcpy(gprspwd, DEFAULT_WLM_PWD);
		}
	}
	if((ret=PppSetUsrCfg())!=SUCC)
	{
		cls_show_msg1(10, "%s, line %d:PPP��������ʧ��(%d)", __FILE__, __LINE__, ret);
		return ret;//FAIL;
	}
		
	return SUCC;
}

int conf_conn_PPP(void)
{
	/*process body*/
	int nKeyin = 0, nkeyin1 = ENTER, i = 0;
	LinkType type[] = {GPRS, CDMA, ASYN, TD};
	char *typestr[] = {"GPRS", "CDMA", "ASYN", "TD"};
	int enable_flag[] ={PPP_ENABLE, PPP_ENABLE, ASYN_ENABLE, TD_ENABLE};
	int conf_flag = 0;	
	char pInfo[64] = {0}, temptype[16] = {0};
	
	/*process body*/	
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		g_conf_flag = 1;
		if(GetParaInfo("WLMType",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,temptype)!=SUCC)
				return FAIL;
		for(i=0;i<SZ_ARRAY(typestr);i++)
		{
			if(!strcmp(temptype,typestr[i]))
			{
				if(type[i]==ASYN)
				{
					setlinktype(ASYN);
					strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);//ֱ��д��,���ص���conf_link
				}
				else
				{
					if(conf_link(type[i])!=SUCC)
						return FAIL;
				}
				break;
			}
		}
		if(i==SZ_ARRAY(typestr))
		{
			cls_show_msg("file:%s,line:%d���������ļ���ͨѶ�������ô���)",__FILE__ ,__LINE__);
			return FAIL;
		}
    }
	else
	{
		g_conf_flag = 0;
		do
		{
			nKeyin = cls_show_msg("ѡ�����ӷ�ʽ:\n"
									"1.GPRS 2.CDMA\n"
									"3.ASYN 4.TD\n");
#define MAXMENUITEM ('4')

			conf_flag = enable_flag[nKeyin-'1'];//��ֵ���ñ�־
			switch (nKeyin)
			{
			//case 0://0Ϊ��ʱ,Ĭ������GPRS
			case '1':
			case '2':
			case MAXMENUITEM://'4':
				if(conf_flag)//֧�ֱ�־Ϊ1˵��֧����ʼ����,����ֱ����ʾ��֧�ֺ��˳�
				{
					if(conf_link(type[nKeyin-'1'])!=SUCC)
						return FAIL;
					break;
				}
				else
				{
					cls_show_msg1(10, "����Ʒ��֧�ֱ����ӷ�ʽ,������ѡ��");
					return NDK_ERR_QUIT;
				}
			case '3':
				if(conf_flag)
				{
					setlinktype(type[nKeyin-'1']);//linktype = ASYN;
					strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);//ֱ��д��,���ص���conf_link
					break;
				}
				else
				{
					cls_show_msg1(10, "����Ʒ��֧�ֱ����ӷ�ʽ,������ѡ��");
					return NDK_ERR_QUIT;
				}
			case ESC:
				return NDK_ERR_QUIT;//QUIT;
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM	//ʹ����ͽ��ͷŶ���
	}
	return SUCC;
}
#else
int PppSetUsrCfg(void){return FAIL;}
static int conf_PPP(void){return FAIL;}
int conf_conn_PPP(void){return FAIL;}
#endif

#if (SDLC_ENABLE||ASYN_ENABLE)
static int conf_MDM_lnk(void)
{
	/*private & local definition*/
	int ret = 0;
	uint len = 0;
	char tmp[2] = {0};
	char pInfo[64] = {0}, dialstr[32] = {0};
	int typenum = 0;
	LinkType type = getlinktype();

	/*process body*/

	if(g_conf_flag)
	{	
		if(GetParaInfo("MDMPatchType",pInfo)!=SUCC)
				return FAIL;
		if(trim_string2int(pInfo,&typenum)!=SUCC)
				return FAIL;
		MDMPatchType=typenum;
		if(type==SYNC)
		{
			if(GetParaInfo("SYNCDialStr",pInfo)!=SUCC)
				return FAIL;
			if(trim_string(pInfo,dialstr)!=SUCC)
				return FAIL;
			strcpy(MDMDialStr,dialstr);
		}
		else
		{
			if(GetParaInfo("ASYNDialStr",pInfo)!=SUCC)
				return FAIL;
			if(trim_string(pInfo,dialstr)!=SUCC)
				return FAIL;
			strcpy(MDMDialStr,dialstr);
		}
		cls_show_msg1(1,"MDM���óɹ�");
	}
	else
	{
		// 1.����ѡ��/����ѡ��
		cls_printf("��ǰ������(%d-%d):\n", MDM_PatchType1, MDM_PatchType5);
		sprintf(tmp, "%d", MDMPatchType);
		//if ((ret=getnumstr(tmp, sizeof(tmp)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
		if ((ret=lib_kbgetinput(tmp, 0, sizeof(tmp)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			MDMPatchType = MDM_PatchType5;
		else
			MDMPatchType = atoi(tmp);
		
		// 2.���ðκŴ�
		cls_printf("��ǰ���Ŵ�:\n");
		//if ((ret=getreturnline(MDMDialStr, sizeof(MDMDialStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default   //��ʹ��getnumstr����getline/getreturnline,�Կ���������ĸ
		if ((ret=lib_kbgetinput(MDMDialStr, 0, sizeof(MDMDialStr)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);
	}
	// 3.(�첽)PPP��������
	//������Щ��������ֻ�����첽����Ӧ����(�ҿ��ò���һ���ǹ̶���),Ŀǰ�ݲ����ڴ˴�����,����Ҫ�ĵط�ֱ��д������

	return SUCC;
}

int sdlc_init_flag = 0;
int g_sdlc_bps = EM_MDM_BPS_1200;
#if defined SP50||defined SP80||defined GP720//SP5080�Ļ����Ѿ�����
int set_sdlc_bps(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	char pInfo[64] = {0};
	
	/*process body*/
	if(g_conf_flag==1)
	{
		if(GetParaInfo("MDMBps",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&g_sdlc_bps)!=SUCC)
			return FAIL;
		if(g_sdlc_bps<EM_MDM_BPS_1200||g_sdlc_bps>EM_MDM_BPS_9600)
			g_sdlc_bps=EM_MDM_BPS_1200;
	}
	else
	{
		nKeyin = cls_show_msg("���õĲ�����\n"
								"1.1200\n"
								"2.2400\n"
								"3.9600\n");
		switch(nKeyin)
		{
		case '1':
		case '2':
		case '3':
			g_sdlc_bps = nKeyin-'0';
			break;
		default:
		case ESC:
			g_sdlc_bps = EM_MDM_BPS_1200;
			break;
		}
	}
	
	return SUCC;
}
#else
int set_sdlc_bps(void)
{
	return SUCC;
}
#endif

int conf_conn_MDM(void)
{
	/*private & local definition*/
	int nKeyin = 0, nkeyin1 = ENTER;
	LinkType type[] = {SYNC, ASYN};
	char pInfo[64] = {0}, temptype[16] = {0};
	
	/*process body*/
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		g_conf_flag = 1;
		if(GetParaInfo("MDMType",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,temptype)!=SUCC)
			return FAIL;
		if(!strcmp(temptype,"SYNC"))
		{
			if(conf_link(SYNC)!=SUCC)
				return FAIL;
		}
		else if(!strcmp(temptype,"ASYN"))
		{
			if(conf_link(ASYN)!=SUCC)
				return FAIL;
		}
		else
		{
			cls_show_msg("file:%s,line:%dModem�������ʹ���)",__FILE__ ,__LINE__);
			return FAIL; 
		}
	}
	else
	{
		g_conf_flag = 0;
		do
	    {
	        nKeyin = cls_show_msg("ѡ�����ӷ�ʽ:\n"
									"1.ͬ��MODEM\n"
									"2.�첽MODEM\n");
#define MAXMENUITEM ('2')
			switch(nKeyin) 
			{
			case '1':
			case MAXMENUITEM://'2':
				if(conf_link(type[nKeyin-'1'])!=SUCC)//����MODEM ���Ų���
					return FAIL;
				break;
			case ESC:
				return NDK_ERR_QUIT;//QUIT;
				break;
			default:
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

		nkeyin1 = 0;
		if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]Ĭ�ϳ�ʼ������,[����]���ô����������õĳ�ʼ������"))==ENTER || nkeyin1==0)
			sdlc_init_flag=0;
		else
			sdlc_init_flag=1;
			
	}

	if(getlinktype()==SYNC&&sdlc_init_flag==1)
		set_sdlc_bps();
	
	return SUCC;
}
#else
static int conf_MDM_lnk(void){return FAIL;}
int conf_conn_MDM(void){return FAIL;}
#endif
#endif

#if !defined ME66 //ME66 �ռ�С ������
int select_serial(void)
{
	/*private & local definition*/
	int nKeyin = 0, port = 0;

	/*process body*/
	while(1)
	{
#if COMNUM==3
#if defined GP710
	nKeyin = cls_show_msg("��ѡ�񴮿�:\n���ֻ�����1\n����������1\n����������2\n");
#else
	nKeyin = cls_show_msg("��ѡ�񴮿�:\n�ٴ���1 �ڴ���2\n�۴���3\n");
#endif
#elif COMNUM==2
	nKeyin = cls_show_msg("��ѡ�񴮿�:\n�ٴ���1 �ڴ���2\n");
#else //Ĭ��ֻ��һ������
#if defined ME15C
	nKeyin = cls_show_msg("��ѡ�񴮿�:\n�ٴ���2\n");
#else
	nKeyin = cls_show_msg("��ѡ�񴮿�:\n�ٴ���1\n");
#endif
#endif

	switch (nKeyin)
	{
	case '1':
#if defined ME15C
		port = PORT_NUM_COM2;
#else
		port = PORT_NUM_COM1;		
#endif
		break;
#if COMNUM>1
	case '2':
		port = PORT_NUM_COM2;
		break;
#endif
#if COMNUM>2
	case '3':
		port = PORT_NUM_COM3;
		break;
#endif
	default:
		continue;
		break;
	}
	return port;
	}
}

char *conf_serial(int *bpsid)
{
	/*private & local definition*/
	/*const */char *menulst[] =
	{
		"1.300   2.1200\n3.2400  4.4800\n5.9600  6.19200\n7.38400 8.��ҳ\n",
		"1.57600 2.115200\n3.230400\n\n        8.��ҳ\n"
	};
	/*const */char *baud[] = 
	{	
		"300","1200","2400","4800","9600","19200","38400","57600","115200","230400"
	};
	int page = 0, baudid = 0, maxpage = SZ_ARRAY(menulst),  nKeyin = 0;

	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg1(MAXWAITTIME, menulst[page]);
		if (nKeyin=='8')
		{
			if (++page==maxpage)
				page = 0;
		}
		else if (nKeyin>='1' && nKeyin<='7')
		{
			if ((baudid=page*(8-1)+nKeyin-'0'-1)>=SZ_ARRAY(baud))
				continue;
						
			break;
		}
		else if (nKeyin==ESC||nKeyin==0)//��ʱfor�Զ�����
		{
			baudid = 4;//9600 id
			break;
		}
		else
			continue;
	}
	
	if(bpsid!=NULL)
		*bpsid = baudid;
	cls_show_msg1(2, "BPS������Ϊ%s...", baud[baudid]);
	return baud[baudid];//return baudlst[baudid];//g_bps = baudlst[baudid];
}

#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B)	//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ���
/*
confһ�庯�������(����,confһ�庯���������ö���,�ı�linktype���²���conf����):
step1:���ж�type,������/��֧��,ֱ��return(�����ִ��Ч��)
step2:�Ժ����type,�ٴ���ʾ��ȷ�Ϻ�,ת������֧����
*/
int conf_link(LinkType type)
{
	/*private & local definition*/
	int bpsid = 0;
#if DOUBLESIM_ENABLE
	int ret = -1;
	char pInfo[128] = {0};
	int simslot = 0;
#endif

	
	/*process body*/
	setlinktype(type);
	switch(type)
	{
	case ETH://����������,ֱ�ӷ���SUCC
#if defined ME50C
		NDK_NetAddRouterTable(COMM_TYPE_ETH,"0.0.0.0");
#endif
		return SUCC;
		break;
	//��Ҫ���õĶ��ŵ�����ʼ,�ٴ�ȷ�Ϻ�,ת���Դ���
	case GPRS:
	case CDMA:
#if DOUBLESIM_ENABLE
		if(g_conf_flag==1)
		{
			//�������ļ��е�SIM���۱�־
			if(GetParaInfo("SIMSLOT",pInfo)!=SUCC)
				return FAIL;
			if(trim_string2int(pInfo,&simslot)!=SUCC)
				return FAIL;
			SIMSLOT = simslot;
		}
		else
		{
			//�ֶ�����sim ���۱�־��������set����
			ret = cls_show_msg("ѡ��SIM����:\n1.SIM1����\n2.SIM2����\n3.������");
			if(ret=='1')
				SIMSLOT = SIM_SLOT1;
			else if(ret=='2')
				SIMSLOT = SIM_SLOT2;
			else if(ret=='3')
				SIMSLOT = SIM_SLOT_UNKNOW;
		}
		if(SIMSLOT==SIM_SLOT1||SIMSLOT==SIM_SLOT2)
		{
			cls_printf("�л������߿���%d��", SIMSLOT+1);
			if((ret=NDK_WlSetSimSlot(SIMSLOT))!=NDK_OK)
			{
				cls_show_msg1(10, "%s, line %d:�����������߿���%dʧ��(ret=%d)", __FILE__, __LINE__, SIMSLOT+1, ret);
				return FAIL;
			}
		}
#endif
	case TD:
#if defined E90
		cls_printf("˫ģ�л���...");
		//˫ģ�л�����Ҫ�Ƿ����õ�ȷ��,�ʷ�������ȷ��֮ǰ
		//Ҳ������ȡ״̬�پ����Ƿ�����,��Ϊ����ǰ���������õ�״̬��,��ʹȥ����һ��,Ҳ��ܿ췵��,��Ӱ��Ч��
		if((type==GPRS && (WLM_SetNetMode(E_NETWORK_TYPE_GPRS_E)!=SUCC ||(ret=WLM_GetNetStatus_Ex(E_MMI_GMMREG_RAT_GPRS))!=E_MMI_GMMREG_RAT_GPRS))
		||(type==TD && (WLM_SetNetMode(E_NETWORK_TYPE_TD_E)!=SUCC||(ret=WLM_GetNetStatus_Ex(E_MMI_GMMREG_RAT_3G))!=E_MMI_GMMREG_RAT_3G)))
		{
			cls_show_msg1(10, "%s, line %d:������������ģʽʧ��(%d)", __FILE__, __LINE__, ret);
			return FAIL;
		}
#endif
	case ASYN:
	case SYNC:
	case SERIAL:
	case BT:
	case WLAN:
		if(!g_conf_flag)
		{
			if(ENTER!=cls_show_msg1(MAXWAITTIME, "����������·������?\n"                                                   
													"[ȷ��]��[����]��\n"))
				return SUCC;	
		}
		switch(type)//�ٴθ���type���зַ�����
		{
		case ASYN:
			if(conf_MDM_lnk()!=SUCC)
				return FAIL;
			return PppSetUsrCfg();//return conf_PPP();
			break;			
		case SYNC:
			return conf_MDM_lnk();
			break;
		case SERIAL:
			strcpy(g_bps, conf_serial(&bpsid));//g_bps = conf_serial();
			break;
		case BT:
			return conf_bt();
			break;
		case GPRS:
		case CDMA:
		case TD:
#if CPU5810X_ENABLE||defined ME50NX||defined ME68//50��50N��Ҫ���øú�������·�ɱ�
			NDK_NetAddRouterTable(COMM_TYPE_PPP,"0.0.0.0");
#endif
			return conf_PPP();
			break;
		case WLAN:
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
			NDK_NetAddRouterTable(COMM_TYPE_WIFI,"0.0.0.0");
#endif
			if(g_conf_flag)
				return use_wifi_conf(0);
			else
				return conf_WLAN_lnk();
			break;
		default://never reach here
			break;
		}
		return SUCC;
		break;
	default:
		cls_show_msg1(10, "%s, line %d:δ֪����·����(%d)", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}

	return SUCC;
}
#else
int conf_link(LinkType type)//ME15C�ռ������ҽ�֧�ִ��ں�����,���������ú���
{
	/*private & local definition*/
	int bpsid = 0;
	
	/*process body*/
	setlinktype(type);
	switch(type)
	{
		case SERIAL:
		case BT:
			if(!g_conf_flag)
			{
				if(ENTER!=cls_show_msg1(MAXWAITTIME, "����������·������?\n"                                                   
														"[ȷ��]��[����]��\n"))
					return SUCC;	
			}
			switch(type)//�ٴθ���type���зַ�����
			{
				case SERIAL:
					strcpy(g_bps, conf_serial(&bpsid));//g_bps = conf_serial();
					break;
				case BT:
					return conf_bt();
					break;
				default://never reach here
					break;
			}
			return SUCC;
			break;
		default:
			cls_show_msg1(10, "%s, line %d:δ֪����·����(%d)", __FILE__, __LINE__, type);
			return FAIL;
			break;
	}
	return SUCC;
}
#endif
#endif

#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)	//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ���
int conf_local(void)
{
	/*private & local definition*/
	int ret = 0;
	uint len = 0;
	LinkType type = getlinktype();
	char pInfo[64] = {0};
	char localip[16] = {0}, localmask[16] = {0}, localgateway[16] = {0};
	int dhcpflag = 0;

	/*process body*/
	switch(type)
	{
	//��Ҫ���õĶ��ŵ�����ʼ,�ٴ�ȷ�Ϻ�,ת���Դ���
	case ETH:
	case WLAN:
		if(g_conf_flag)//�������ļ��ж�ȡ������
		{
			if(type == ETH)
			{
				if(GetParaInfo("EthDHCPenable",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&dhcpflag)!=SUCC)
					return FAIL;
				if(dhcpflag)//Ϊ1 ʹ�� ʹ���Զ����䱾�ص�ַ
				{
					netsetting.DHCPenable = ENABLE;
				}
				else  //Ϊ0��Ҫ�������ļ��ж�ȡ���ص�ַ
				{
					netsetting.DHCPenable = DISABLE;
					if(GetParaInfo("EthLocalIP",pInfo)!=SUCC)
						return FAIL;
					if(trim_string(pInfo,localip)!=SUCC)
						return FAIL;
					strcpy(netsetting.LocalIP.Str,localip);
					if(GetParaInfo("EthLocalMask",pInfo)!=SUCC)
						return FAIL;
					if(trim_string(pInfo,localmask)!=SUCC)
						return FAIL;
					strcpy(netsetting.LocalMask.Str,localmask);
					if(GetParaInfo("EthLocalGatway",pInfo)!=SUCC)
						return FAIL;
					if(trim_string(pInfo,localgateway)!=SUCC)
						return FAIL;
					strcpy(netsetting.LocalGateway.Str,localgateway);		
					netsetting.LocalIP.value.data = INET_ADDR(netsetting.LocalIP.Str);
					netsetting.LocalMask.value.data = INET_ADDR(netsetting.LocalMask.Str);
					netsetting.LocalGateway.value.data = INET_ADDR(netsetting.LocalGateway.Str);
					cls_show_msg1(1,"��������������óɹ�");
				}
			}
			else
			{
				if(GetParaInfo("WifiDHCPenable",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&dhcpflag)!=SUCC)
					return FAIL;
				if(dhcpflag)//Ϊ1 ʹ�� ʹ���Զ����䱾�ص�ַ
				{
					netsetting.DHCPenable = ENABLE;
				}
				else  //Ϊ0��Ҫ�������ļ��ж�ȡ���ص�ַ
				{
					netsetting.DHCPenable = DISABLE;
					if(GetParaInfo("WifiLocalIP",pInfo)!=SUCC)
						return FAIL;
					if(trim_string(pInfo,localip)!=SUCC)
						return FAIL;
					strcpy(netsetting.LocalIP.Str,localip);
					if(GetParaInfo("WifiLocalMask",pInfo)!=SUCC)
						return FAIL;
					if(trim_string(pInfo,localmask)!=SUCC)
						return FAIL;
					strcpy(netsetting.LocalMask.Str,localmask);
					if(GetParaInfo("WifiLocalGatway",pInfo)!=SUCC)
						return FAIL;
					if(trim_string(pInfo,localgateway)!=SUCC)
						return FAIL;
					strcpy(netsetting.LocalGateway.Str,localgateway);	
					netsetting.LocalIP.value.data = INET_ADDR(netsetting.LocalIP.Str);
					netsetting.LocalMask.value.data = INET_ADDR(netsetting.LocalMask.Str);
					netsetting.LocalGateway.value.data = INET_ADDR(netsetting.LocalGateway.Str);
					cls_show_msg1(1,"��������������óɹ�");
				}
			}
		}
		else  //�ֶ�����
		{
			if(ENTER!=cls_show_msg1(MAXWAITTIME, "�������ñ������������?\n"
													"[ȷ��]��[����]��\n"))
				return SUCC;
			if(ENTER==cls_show_msg1(MAXWAITTIME, "_ѡ�����÷�ʽ_\n"
													"[ȷ��]��̬����\n"
													"[����]��̬����\n"))
			{
				cls_printf("��ǰ����IP:\n");
				//if ((ret=getnumstr(netsetting.LocalIP.Str, sizeof(netsetting.LocalIP.Str)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default      
				if ((ret=lib_kbgetinput(netsetting.LocalIP.Str, 0, sizeof(netsetting.LocalIP.Str)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
					strcpy(netsetting.LocalIP.Str, DEFAULT_LOCAL_IP_STR);       

				netsetting.LocalIP.value.data = INET_ADDR(netsetting.LocalIP.Str);
									
				cls_printf("��ǰ��������:\n");
				//if ((ret=getnumstr(netsetting.LocalMask.Str, sizeof(netsetting.LocalMask.Str)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default    
				if ((ret=lib_kbgetinput(netsetting.LocalMask.Str, 0, sizeof(netsetting.LocalMask.Str)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
					strcpy(netsetting.LocalMask.Str, DEFAULT_LOCAL_MASK_STR);
				
				netsetting.LocalMask.value.data = INET_ADDR(netsetting.LocalMask.Str);
								
				cls_printf("��ǰ��������:\n");
				//if ((ret=getnumstr(netsetting.LocalGateway.Str, sizeof(netsetting.LocalGateway.Str)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default 
				if ((ret=lib_kbgetinput(netsetting.LocalGateway.Str, 0, sizeof(netsetting.LocalGateway.Str)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
					strcpy(netsetting.LocalGateway.Str, DEFAULT_LOCAL_GATEWAY_STR);
			 
				netsetting.LocalGateway.value.data = INET_ADDR(netsetting.LocalGateway.Str);
				netsetting.DHCPenable = DISABLE;			
			}
			else
				netsetting.DHCPenable = ENABLE;
		}
		return SUCC;
		break;
	//�Զ���ȡ(NETUPʱ),�����ֹ�����,ֱ��return
	case GPRS:
	case CDMA:
	case TD:
	case ASYN:
		return SUCC;
		break;
	case SERIAL:
	case BT:
	case SYNC:
		return FAIL;
		break;
	default:
		cls_show_msg("%s, line %d:δ֪�ı�����������(%d)", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}

	return SUCC;
}

int conf_server(void)
{
	/*private & local definition*/
	char tmp[8] = {0};
	int ret = 0;
	uint len = 0;
	LinkType type = getlinktype();	
	char pInfo[64] = {0}, svrip[16] = {0};
	int tcpport = 0, udpport = 0;  

	/*process body*/
	switch(type)
	{
	//��Ҫ���õĶ��ŵ�����ʼ,�ٴ�ȷ�Ϻ�,ת���Դ���
	case GPRS:
	case CDMA:
	case TD:
	case ETH:
	case ASYN:
	case WLAN:
		if(g_conf_flag)
		{
			if(type==GPRS || type==CDMA ||type==TD || type==ASYN)
			{
				if(GetParaInfo("WlmSvrIP",pInfo)!=SUCC)
					return FAIL;
				if(trim_string(pInfo,svrip)!=SUCC)
					return FAIL;
				strcpy(netsetting.SvrIP.Str,svrip);
				if(GetParaInfo("WlmSvrTCP_PORT",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&tcpport)!=SUCC)
					return FAIL;
				netsetting.SvrTcpPort=(ushort)tcpport;
				if(GetParaInfo("WlmSvrUDP_PORT",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&udpport)!=SUCC)
					return FAIL;
				netsetting.SvrUdpPort=(ushort)udpport;
			}
			else if(type==ETH)
			{
				if(GetParaInfo("EthSvrIP",pInfo)!=SUCC)
					return FAIL;
				if(trim_string(pInfo,svrip)!=SUCC)
					return FAIL;
				strcpy(netsetting.SvrIP.Str,svrip);
				if(GetParaInfo("EthSvrTCP_PORT",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&tcpport)!=SUCC)
					return FAIL;
				netsetting.SvrTcpPort=(ushort)tcpport;
				if(GetParaInfo("EthSvrUDP_PORT",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&udpport)!=SUCC)
					return FAIL;
				netsetting.SvrUdpPort=(ushort)udpport;
			}
			else 
			{
				if(GetParaInfo("WifiSvrIP",pInfo)!=SUCC)
					return FAIL;
				if(trim_string(pInfo,svrip)!=SUCC)
					return FAIL;
				strcpy(netsetting.SvrIP.Str,svrip);
				if(GetParaInfo("WifiSvrTCP_PORT",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&tcpport)!=SUCC)
					return FAIL;
				netsetting.SvrTcpPort=(ushort)tcpport;
				if(GetParaInfo("WifiSvrUDP_PORT",pInfo)!=SUCC)
					return FAIL;
				if(trim_string2int(pInfo,&udpport)!=SUCC)
					return FAIL;
				netsetting.SvrUdpPort=(ushort)udpport;
			}			
			cls_show_msg1(1,"�������������óɹ�");
		}
		else
		{
			if(ENTER!=cls_show_msg1(MAXWAITTIME, "�������÷�����������?\n"                                                   
													"[ȷ��]��[����]��\n"))
				return SUCC;                                                                                                                       
			
			//���繫������
			cls_printf("��ǰ������IP:\n");
			//if ((ret=getnumstr(netsetting.SvrIP.Str, sizeof(netsetting.SvrIP.Str)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
			if ((ret=lib_kbgetinput(netsetting.SvrIP.Str, 0, sizeof(netsetting.SvrIP.Str)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				strcpy(netsetting.SvrIP.Str, DEFAULT_SVR_IP_STR);
			
			netsetting.SvrIP.value.data = INET_ADDR(netsetting.SvrIP.Str);
					
			//itoa(netsetting.SvrPort, tmp, 10);                                         
			cls_printf("��ǰ������TCP�˿�:\n");
			sprintf(tmp, "%d", netsetting.SvrTcpPort);
			//if ((ret=getnumstr(tmp, sizeof(tmp)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
			if ((ret=lib_kbgetinput(tmp, 0, sizeof(tmp)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				netsetting.SvrTcpPort = DEFAULT_SVR_TCP_PORT;	                     
			else                                                                         
				netsetting.SvrTcpPort = atoi(tmp);
		                                                                                                     
			cls_printf("��ǰ������UDP�˿�:\n");
			sprintf(tmp, "%d", netsetting.SvrUdpPort);
			//if ((ret=getnumstr(tmp, sizeof(tmp)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
			if ((ret=lib_kbgetinput(tmp, 0, sizeof(tmp)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				netsetting.SvrUdpPort = DEFAULT_SVR_UDP_PORT;
			else                                                                         
				netsetting.SvrUdpPort = atoi(tmp);
		}
		return SUCC;
		break;
	case SERIAL:
	case BT:
	case SYNC:	
		return FAIL;
		break;
	default:
		cls_show_msg1(10, "%s, line %d:δ֪����������(%d)", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}
	
	return SUCC;
}

int conf_network(void)
{
 	/*private & local definition*/

 	/*process body*/
	if(conf_local()!=SUCC)
 		return FAIL;
 	if(conf_server()!=SUCC)
 		return FAIL;

	return SUCC;
}

#if NDKSSL_ENABLE
static void conf_SSL(void)
{
 	/*private & local definition*/
 	int nKeyin = 0;

 	/*process body*/
	//EM_SSL_HANDSHAKE_OPT
	do
	{
		nKeyin = cls_show_msg("SSL����ѡ��:\n"
								"1.SSLv2 2.SSLv23\n"
								"3.SSLv3 4.TLSv1\n");
#define MAXMENUITEM ('4')
		
		switch (nKeyin)
		{
		case ESC:
		case '1':
			cls_show_msg1(2, "��ʹ��SSLv2...");
			netsetting.SSLHsOpt = HANDSHAKE_SSLv2;
			break;
		case '2':
			cls_show_msg1(2, "��ʹ��SSLv23...");
			netsetting.SSLHsOpt = HANDSHAKE_SSLv23;
			break;
		case '3':
			cls_show_msg1(2, "��ʹ��SSLv3...");
			netsetting.SSLHsOpt = HANDSHAKE_SSLv3;
			break;
		case MAXMENUITEM://'4':
			cls_show_msg1(2, "��ʹ��TLSv1...");
			netsetting.SSLHsOpt = HANDSHAKE_TLSv1;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

 	//EM_SSL_AUTH_OPT
	do
	{
		nKeyin = cls_show_msg("SSL��֤ѡ��:\n"
								"1.AUTH_NONE\n"
								"2.AUTH_CLIENT\n"
								//����������"3.AUTH_CLI_FAIL\n"
								);
#define MAXMENUITEM ('2')		
		
		switch (nKeyin)
		{
		case ESC:
		case '1':
			cls_show_msg1(2, "��ʹ��AUTH_NONE...");
			netsetting.SSLAuthOpt = SSL_AUTH_NONE;
			break;
		case MAXMENUITEM://'2':
			cls_show_msg1(2, "��ʹ��AUTH_CLIENT...");
			netsetting.SSLAuthOpt = SSL_AUTH_CLIENT;
			break;
#if 0
		case '3':
			cls_show_msg1(2, "��ʹ��AUTH_CLIENT_FAIL...");
			netsetting.SSLAuthOpt = SSL_AUTH_CLIENT_FAIL;
			break;
#endif
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM	
 	
 #if 0
 	//EM_SSL_FILE_FORMAT
	do
	{
		nKeyin = cls_show_msg("SSL֤���ʽ:\n"
								"1.PEM 2.DER\n");
		
		switch (nKeyin)
		{
		case ESC:
		case '1':
			cls_show_msg1(2, "��ʹ��PEM��ʽ...");
			netsetting.SSLFileFmt = FILE_PEM;
			break;
		case '2':
			cls_show_msg1(2, "��ʹ��DER��ʽ...");
			netsetting.SSLFileFmt = FILE_DER;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>'2'));
#else	//����,��Ӱ�����
	netsetting.SSLFileFmt = SSL_FILE_PEM;
 #endif
}
#else
static void conf_SSL(void){}
#endif

//������Ͽ�,conf_transһ��������ۺϲ���,���������ڵ�Ԫ����.��Ϊ���Ĳ��̫����
#if (NDKSOCK_ENABLE||NDKSSL_ENABLE)
int conf_trans(void)
{
 	/*private & local definition*/
 	char *sockname[] = {"NDKTCP", "NDKUDP", "NDKSSL", NULL};
	sock_t socktype[] = {SOCK_NDKTCP, SOCK_NDKUDP, SOCK_NDKSSL};
 	int i = 0, nKeyin = 0;
 	LinkType type = getlinktype();
	char pInfo[64] = {0}, temptype[8] = {0};

 	/*process body*/
	switch(type)
	{
	//��Ҫ���õĶ��ŵ�����ʼ,�ٴ�ȷ�Ϻ�,ת���Դ���
	case ETH:
	case GPRS:
	case CDMA:
	case TD:
	case ASYN:
	case WLAN:
		if(g_conf_flag)
		{
			if(GetParaInfo("SockType",pInfo)!=SUCC)
				return FAIL;
			if(trim_string(pInfo,temptype)!=SUCC)
				return FAIL;
			for(i=0;i<(SZ_ARRAY(sockname)-1);i++)
			{
				if(!strcmp(temptype,sockname[i]))
				{
					netsetting.socktype = socktype[i];
				}
				break;
			}
			if(i==(SZ_ARRAY(sockname)-1))
			{
				
				cls_show_msg("file:%s,line:%d���������ļ��д��������������ʹ���)",__FILE__ ,__LINE__);
				return FAIL; 
			}
			cls_show_msg1(1,"�����������óɹ�");
		}
		else
		{
			if(ENTER!=cls_show_msg1(MAXWAITTIME, "�������ô�������?\n"                                                   
													"[ȷ��]��[����]��\n"))
				return SUCC;

			do
			{
				NDK_ScrClrs();//clrscr();
				i = 0;
				while(sockname[i])
				{
					if(netsetting.socktype==(i))
						NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE, NULL);//settextattr(REVERSE_TEXT);
					
					NDK_ScrPrintf("%d.%s\n", i+1, sockname[i]);//printf("%d.%s\n", i+1, sockname[i]);

					if(netsetting.socktype==(i))
						NDK_ScrSetAttr(TEXT_ATTRIBUTE_NORMAL, NULL);//settextattr(NORMAL_TEXT);
					
					i++;
				}
				NDK_ScrRefresh();

				nKeyin = lib_getch();//getch();
				if(nKeyin>'0' && nKeyin<'0'+SZ_ARRAY(sockname))
					netsetting.socktype = (sock_t)(nKeyin-'1');
			}while(nKeyin!=ESC && nKeyin!=ENTER);
		}

		if(netsetting.socktype==SOCK_NDKSSL&& ENTER==cls_show_msg1(MAXWAITTIME, "��������SSL�����?\n"                                                   
												"[ȷ��]��[����]��\n"))
			conf_SSL();
		
		return SUCC;
		break;
	case SERIAL:
	case BT:
	case SYNC:
		return FAIL;
		break;
	default:
		cls_show_msg1(10, "%s, line %d:δ֪�ı�����������(%d)", __FILE__, __LINE__, type);
		return FAIL;
		break;
	}

	return SUCC;
}
#else
int conf_trans(void){return FAIL;}
#endif

#if ETH_ENABLE
int conf_conn_ETH(int IsTransConfed)
{
	/*private & local definition*/
	int nkeyin = ENTER;
	
	/*process body*/
	if((nkeyin=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin==0)
	{	
		if(NDK_FsExist(PARACONF)!=NDK_OK)
		{	
			cls_show_msg1(10,"�޲��������ļ�,�������ֶ�����");
			g_conf_flag = 0;
		}
		else
			g_conf_flag = 1;
	}	
	else
		g_conf_flag = 0;
	//�ֹ�������·����
	if(conf_link(ETH)!=SUCC)//setlinktype(ETH);//linktype = ETH;
		return FAIL;
	if(cls_show_msg1(5,"��ȷ����TCP/UDP��̨!!!\n")==ESC)
		return NDK_ERR_QUIT;//QUIT;
	//�ֹ������������
	if(conf_network()!=SUCC)
		return FAIL;
	if(IsTransConfed==TRUE)
	{
		if(conf_trans()!=SUCC)
			return FAIL;
	}
	
	//�������,Ҫ����һ��������ͨ��
#if 0
	//�������,Ҫ����һ��������ͨ��
	return (IsNetConnected()==TRUE)?SUCC:FAIL;
#else
	return SUCC;
#endif

}
#else
int conf_conn_ETH(int IsTransConfed){return FAIL;}
#endif

#if (NDKSOCK_ENABLE||NDKSSL_ENABLE)
int conf_conn_sock(void)
{
 	/*private & local definition*/
	int nKeyin = 0, nkeyin1 = ENTER, i = 0;
	LinkType type[] = {GPRS, CDMA, ASYN, ETH, TD, WLAN};
	char *typestr[] = {"GPRS", "CDMA", "ASYN", "ETH", "TD", "WLAN"};
	int enable_flag[] ={PPP_ENABLE, PPP_ENABLE, ASYN_ENABLE, ETH_ENABLE, TD_ENABLE, WLAN_ENABLE};
	int conf_flag = 0;
	char pInfo[64] = {0}, temptype[16] = {0};

 	/*process body*/	
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		g_conf_flag = 1;
		if(GetParaInfo("COMMType",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,temptype)!=SUCC)
			return FAIL;
		for(i=0;i<SZ_ARRAY(typestr);i++)
		{
			if(!strcmp(temptype,typestr[i]))
			{			
				if(conf_link(type[i])!=SUCC)
					return FAIL;
				if(getlinktype()==ASYN)//��Ϊ�첽,����һ������
				{
					strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);	
					//׷�Ӷ�PPPUSR,PPPPWD������
					strcpy(PPPUSR, DEFAULT_ASYN_USR);
					strcpy(PPPPWD, DEFAULT_ASYN_PWD);
				}
				break;
			}
			
		}
		if(i==SZ_ARRAY(typestr))
		{
			cls_show_msg("file:%s,line:%d���������ļ���socketͨѶ�������ô���)",__FILE__ ,__LINE__);
			return FAIL;
		}
	}
	else
	{
		g_conf_flag = 0;
		do
		{
			nKeyin = cls_show_msg("ѡ�����ӷ�ʽ:\n"
									"1.GPRS 2.CDMA\n"
									"3.ASYN 4.Eth\n"
									"5.TD 6.WLAN\n");
#define MAXMENUITEM ('6')

			conf_flag = enable_flag[nKeyin-'1'];
			switch (nKeyin)
			{
			//case 0://0Ϊ��ʱ,Ĭ������GPRS
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case MAXMENUITEM://'6':
				if(conf_flag)
				{
					if(conf_link(type[nKeyin-'1'])!=SUCC)
						return FAIL;
					if(getlinktype()==ASYN)//��Ϊ�첽,����һ������
					{
						strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);	
						//׷�Ӷ�PPPUSR,PPPPWD������
						strcpy(PPPUSR, DEFAULT_ASYN_USR);
						strcpy(PPPPWD, DEFAULT_ASYN_PWD);
					}
					break;
				}
				else
				{
					cls_show_msg1(10, "����Ʒ��֧�ֱ����ӷ�ʽ,������ѡ��");
					return NDK_ERR_QUIT;
				}
			case ESC:
				return NDK_ERR_QUIT;//QUIT;
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
	}

 	if(cls_show_msg1(MAXWAITTIME,"��ȷ����sock����ssl���Ժ�̨!!!\n")==ESC)
		return NDK_ERR_QUIT;//QUIT;
	if(conf_network()!=SUCC)
		return FAIL;

#if NEW_WIFI_ENABLE
	if(getlinktype()==WLAN)
	{
		cls_show_msg1(5,"����AP�б�...");
		//����ȫ�ֲ�����AP�б��ļ���,ͬʱ���ú��б�
		if(set_addedapinfo_fromgloabvar()!= SUCC)
		{
			cls_show_msg1(10, "%s, line %d:AP�б�����ʧ��!(%d)", __FILE__, __LINE__);
			return FAIL;
		}
	}
#endif

#if 0
	//�������,Ҫ����һ��������ͨ��
	return (IsNetConnected()==TRUE)?SUCC:FAIL;
#else
	return SUCC;
#endif
}
#else
int conf_conn_sock(void){return FAIL;}
#endif

#if (WLM_ENABLE && PPP_ENABLE)
int conf_conn_WLM(int IsCnncted)
{
	/*private & local definition*/
	int nKeyin = 0, nkeyin1 = ENTER, i = 0;
	LinkType type[] = {GPRS, CDMA, TD};
	char *typestr[] = {"GPRS", "CDMA", "TD"};
	int enable_flag[] ={PPP_ENABLE, PPP_ENABLE, TD_ENABLE};
	int conf_flag = 0;
	char pInfo[64] = {0}, temptype[16] = {0};
	
	/*process body*/	
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		g_conf_flag = 1;
		if(GetParaInfo("WLMType",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,temptype)!=SUCC)
			return FAIL;
		for(i=0;i<SZ_ARRAY(typestr);i++)
		{
			if(!strcmp(temptype,typestr[i]))
			{
				if(conf_link(type[i])!=SUCC)
					return FAIL;
				break;
			}
		}
		if(i==SZ_ARRAY(typestr))
		{
			cls_show_msg("file:%s,line:%d���������ļ��������������ô���)",__FILE__ ,__LINE__);
			return FAIL;
		}
	}
	else
	{
		g_conf_flag = 0;
		do
		{	
			nKeyin = cls_show_msg("ѡ�����ӷ�ʽ:\n"
								"1.GPRS 2.CDMA\n"
								"3.TD\n");
#define MAXMENUITEM ('3')

			conf_flag = enable_flag[nKeyin-'1'];
			switch(nKeyin)
			{
			//case 0://0Ϊ��ʱ,Ĭ������GPRS
			case '1':
			case '2':
			case MAXMENUITEM://'3':
				if(conf_flag)
				{
					if(conf_link(type[nKeyin-'1'])!=SUCC)
						return FAIL;
					break;
				}
				else
				{
					cls_show_msg1(10, "����Ʒ��֧�ֱ����ӷ�ʽ,������ѡ��");
					return NDK_ERR_QUIT;
				}	
			case ESC:
				return NDK_ERR_QUIT;//QUIT;
				break;
			default:
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
	}
	if(IsCnncted==TRUE)//IsCnncted==FALSE�Ļ�,ֻ���ñ�����·����
	{
		if(cls_show_msg1(5,"��ȷ����TCP/UDP��̨!!!\n")==ESC)
			return NDK_ERR_QUIT;//QUIT;
		if(conf_network()!=SUCC)
			return FAIL;
		if(conf_trans()!=SUCC)
			return FAIL;
		
#if 0
		//�������,Ҫ����һ��������ͨ��
		if(!IsNetConnected())
			return FAIL;
#endif
	}

	return SUCC;
}
#else
int conf_conn_WLM(int IsCnncted){return FAIL;}
#endif

int conf_conn_DNS(void)
{
#if 1
	char pInfo[64] = {0};
	int type = 0, keyin = 0, ret = 0;

	if((keyin=cls_show_msg1(10,"dns���Է�ʽѡ��:[Enter]ʹ���Զ�����,[����]�ֶ�����"))==ENTER || keyin==0)
	{
		if(GetParaInfo("DNSType",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&type)!=SUCC)
			return FAIL;
		if(type==1)
			keyin='1';
		else if(type==2)
			keyin='2';
		else if(type==3)
			keyin='3';
		else
			return FAIL;
	}
	else
		keyin=cls_show_msg("��ѡ��dns���Է�ʽ\n"
					"1.ETH\n"
					"2.PPP\n"
					"3.WIFI");

	switch(keyin) 
	{
		case '1':
			ret=conf_conn_ETH(FALSE);//ETH��WIFI��ֱ����ö�̬IP�;�̬IP���в���
			break;
		case '2':
			ret=conf_conn_PPP();
			break;
		case '3':
			ret=conf_conn_WLAN();
			break;
		default:
			ret=conf_conn_ETH(FALSE);
			break;
	}

	return ret;
#else
	/*private & local definition*/
	int nKeyin = 0;
	LinkType type[] = {GPRS, CDMA, ASYN, ETH, TD, WLAN};
	
	/*process body*/
	do
	{
		nKeyin = cls_show_msg("ѡ�����ӷ�ʽ:\n"
								"1.GPRS 2.CDMA\n"
								"3.ASYN 4.Eth\n"
								"5.TD 6.WLAN\n");
#define MAXMENUITEM ('6')

		switch (nKeyin)
		{
		//case 0://0Ϊ��ʱ,Ĭ������GPRS
		case '1':
		case '2':
		case '3':			
		case '4':
		case '5':
		case MAXMENUITEM://'6':
			if(conf_link(type[nKeyin-'1'])!=SUCC)
				return FAIL;
			if(getlinktype()==ASYN)//��Ϊ�첽,����һ������
			{
				strcpy(MDMDialStr, DEFAULT_MDM_DIAL_STR);//ֱ��д��,���ص���conf_link,����,����Ҫ����conf_network������DNS������
				//׷�Ӷ�PPPUSR,PPPPWD������
				strcpy(PPPUSR, DEFAULT_ASYN_USR);
				strcpy(PPPPWD, DEFAULT_ASYN_PWD);
			}			
			if(conf_local()!=SUCC)
		 		return FAIL;		
			break;
		case ESC:
			return NDK_ERR_QUIT;//QUIT;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

	return SUCC;
#endif
}

#if  WLAN_ENABLE
//�������ļ��е���wifi�Ĳ���
int use_wifi_conf(int ApID)
{
	/*private & local definition*/
	char pInfo[128] = {0},  wifissid[128] = {0}, wifiPwd[65] = {0};
#if NEW_WIFI_ENABLE
	char wifimactemp[64] = {0};
	uchar wifimac[12] = {0};
	int macflag, hiddenflag;
#else
	int encmode, keytype, eapmode;
	char Identity[128] = {0};
#endif
	char tmp[32] = {0}, tmp_left[32] = {0};

	//��ȡssid
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp, "WlanEssid%d", ApID);
	if(GetParaInfo(tmp,pInfo)!=SUCC)
		return FAIL;
	//����ȡ=��ߵ�������ssid���бȽ�,�������ssid�ټ���,�����ǳ�����ʾ20171101 add by jym
	trim_left_string(pInfo,tmp_left);
	if(strcmp(tmp,tmp_left))
	{	
		cls_show_msg1_record(FILENAME,FUNCNAME, 3, "�����ļ�����%s������", tmp);
		return FAIL;
	}
	if(trim_string(pInfo,wifissid)!=SUCC)
		return FAIL;
	strcpy(WlanEssid,wifissid);
	
#if !NEW_WIFI_ENABLE
	//��ȡ����ģʽ
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp, "WlanEncMode%d", ApID);
	if(GetParaInfo(tmp,pInfo)!=SUCC)
		return FAIL;
	if(trim_string2int(pInfo,&encmode)!=SUCC)
		return FAIL;
	WlanEncMode = encmode;
	//����ģʽΪ802.1X�������ȡEAPģʽ����֤�û���
	if(WlanEncMode==WIFI_NET_SEC_WPA_EAP)
	{
		//��ȡEAPģʽ
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp, "Wlan8021XEapMode%d", ApID);
		if(GetParaInfo(tmp,pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&eapmode)!=SUCC)
			return FAIL;
		Wlan8021XEapMode = eapmode;
		//��ȡ��֤�û���
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp, "Wlan8021XIdentity%d", ApID);
		if(GetParaInfo(tmp,pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,Identity)!=SUCC)
			return FAIL;
		strcpy(Wlan8021XIdentity,Identity);
	}
	//��ȡ��������
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp, "WlanKeyType%d", ApID);
	if(GetParaInfo(tmp,pInfo)!=SUCC)
		return FAIL;
	if(trim_string2int(pInfo,&keytype)!=SUCC)
		return FAIL;
	WlanKeyType = keytype;
	//��ȡ����ֵ
	if(WlanKeyType) //����һ���ж�,������������Ͳ���ȡ����ֵ,�����òŻ�ȡ20170925
#endif		
	{//��WiFi��ֱ�ӻ�ȡ����ֵ�����ж�20170925
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp, "WlanPwd%d", ApID);
		if(GetParaInfo(tmp,pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,wifiPwd)!=SUCC)
			;//return FAIL;
		//�Ͷ�WiFi����Ϊ�վͲ����ж�20171107modify
		strcpy(WlanPwd,wifiPwd);
	}
#if NEW_WIFI_ENABLE
	//����MAC��ַ,����Ϊ�� 
	if(strcmp(WlanPwd,"")) //����Ϊ��ʱ����MacΪ�վͲ�ȥ��ȡMac 20171107 modify
	{
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp, "WifiMac%d", ApID);
		if(GetParaInfo(tmp,pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,wifimactemp)!=SUCC)
			return FAIL;
		if(trim_char2uchar(wifimactemp,wifimac)!=SUCC)
			return FAIL;
		memcpy((char *)WifiMac,(char *)wifimac,6);
		//��MAC �Ͱ�mac ��־λ��1
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp, "WifiMacFlag%d", ApID);
		if(GetParaInfo(tmp,pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&macflag)!=SUCC)
			return FAIL;
		WifiMacFlag = macflag;
	}
	//�������ȼ�  ���Ժ���Ӱ�?�����Ҫ���ȼ�ʱ��һ���Ǵ���2��AP,��ô���ܸĶ��ĵط�����
	//��ȡ����AP��־λ
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp, "WifiHiddenFlag%d", ApID);
	if(GetParaInfo(tmp,pInfo)!=SUCC)
		return FAIL;
	if(trim_string2int(pInfo,&hiddenflag)!=SUCC)
		return FAIL;
	WifiHiddenFlag = hiddenflag;
#endif
	cls_show_msg1(1,"WLAN�������óɹ�");		
	return SUCC;
	
}

//��conf_WLAN_lnk���������ڲ�����APɨ����ѡ��,������һЩ�������������
void conf_WLAN_lnk1(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	uint unLen=0;
#if !NEW_WIFI_ENABLE
	char *tip[] = {"WIFI_NET_SEC_NONE", "WIFI_NET_SEC_WEP_OPEN", "WIFI_NET_SEC_WEP_SHARED", "WIFI_NET_SEC_WPA", "WIFI_NET_SEC_WPA2", "WIFI_NET_SEC_WPA_EAP"}, *tip1[] = {"NOKEY", "HEX", "ASCII"};

	/*process body*/
	// 2.����ģʽѡ��
	do
	{
		nKeyin = cls_show_msg("WLAN����ģʽ:\n"
								"0.NONE 1.OPEN\n"
								"2.SHARED 3.WPA\n"
								"4.WPA2 5.802.1x\n");//��Ŵ�0ʼ,��Ϊ�˱�����ö��ֵһ��

#define MAXMENUITEM ('5')

		switch (nKeyin)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
			WlanEncMode= nKeyin-'0';
			cls_show_msg1(2, "������%s����ģʽ...", tip[WlanEncMode]);
			break;
		case MAXMENUITEM:
			WlanEncMode= WIFI_NET_SEC_WPA_EAP;
			cls_show_msg1(2, "������%s����ģʽ...", tip[nKeyin-'0']);
			break;
		//case 0://��ʱ,ESC,��ʹ��Ĭ��
		case ESC:
			cls_show_msg1(2, "��ʹ��%s����ģʽ...", tip[WlanEncMode]);
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'0'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

	//����ģʽ��802.1x��֤ģʽʱ������EAPģʽ���û���
	if(WlanEncMode==WIFI_NET_SEC_WPA_EAP)
	{
		//����802.1x��EAPģʽ
		do
		{
			nKeyin = cls_show_msg("EAPģʽ:\n"
									"0.EAP_MODE_MD5\n"
									"1.EAP_MODE_PEAP\n"
									"2.EAP_MODE_TTLS\n");//��Ŵ�0ʼ,��Ϊ�˱�����ö��ֵһ��

#define MAXMENUITEM ('2')

			switch (nKeyin)
			{
			case '0':
			case '1':
			case MAXMENUITEM:
				Wlan8021XEapMode= nKeyin-'0';
				cls_show_msg1(2, "������%s����ģʽ...", tip[Wlan8021XEapMode]);
				break;
			case ESC:
				cls_show_msg1(2, "��ʹ��%s����ģʽ...", tip[Wlan8021XEapMode]);
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'0'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
		
		//����802.1x��֤�û���
		cls_printf("��ǰ802.1x��֤�û���:\n");
		if ((nKeyin=lib_kbgetinput(Wlan8021XIdentity, 0,sizeof(Wlan8021XIdentity)-1,&unLen, INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||nKeyin==NDK_ERR_TIMEOUT)
			strcpy(Wlan8021XIdentity, DEFAULT_WLAN_8021XIDENTITY);
	}
	
	// 3.��������ѡ��
	do
	{
		nKeyin = cls_show_msg("WLAN��������:\n"
								"0.������1.HEX\n"
								"2.ASCII\n");//��Ŵ�0ʼ,��Ϊ�˱�����ö��ֵһ��
#define MAXMENUITEM ('2')

		switch (nKeyin)
		{
		case '0':
		case '1':
		case MAXMENUITEM://'2':
			WlanKeyType= nKeyin-'0';
			cls_show_msg1(2, "��������������:%s...", tip1[WlanKeyType]);
			break;
		//case 0://��ʱ,ESC,��ʹ��Ĭ��
		case ESC:
			cls_show_msg1(2, "��ʹ����������:%s...", tip1[WlanKeyType]);
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}	
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'0'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
#endif

	// 4.WLAN��������
#if NEW_WIFI_ENABLE
	cls_printf("��ǰWLAN����:\n");
	if ((nKeyin=lib_kbgetinput(WlanPwd, 0,sizeof(WlanPwd)-1,&unLen, INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||nKeyin==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default
		strcpy(WlanPwd, DEFAULT_WLAN_PWD);//�����������,����ȡ������,ҲҪ����Ĭ��ֵ
#else
	if(WlanKeyType==0)
		strcpy(WlanPwd, DEFAULT_WLAN_PWD);
	else
	{
		cls_printf("��ǰWLAN����:\n");
		if ((nKeyin=lib_kbgetinput(WlanPwd, 0,sizeof(WlanPwd)-1,&unLen, INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||nKeyin==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default
			strcpy(WlanPwd, DEFAULT_WLAN_PWD);//�����������,����ȡ������,ҲҪ����Ĭ��ֵ
	}
#endif
	return;
}

#if NEW_WIFI_ENABLE
int set_addedapinfo_fromgloabvar(void)
{
	int ret = 0;
	uint len =2, dhcpflag = 0, macflag= 0, priflag = 0, hiddenflag = 0;
	char tmp[4] = {0};
	ST_WIFI_PROFILE_LIST stored_list;
	ST_WIFI_AP_ENTRY_T store_ap;
	stored_list.stored_ap = &store_ap;
	
	//��ȫ�ֱ����е�SSID\���뱣�浽������AP�б��е�ssid[WIFI_SSID_MAX]
	strcpy(stored_list.stored_ap->ssid, WlanEssid);
	strcpy(stored_list.stored_ap->pwd, WlanPwd);
	dhcpflag = netsetting.DHCPenable&0x01;
	if(dhcpflag==0)
	{
		strcpy(stored_list.stored_ap->ip.ip,netsetting.LocalIP.Str);	
		strcpy(stored_list.stored_ap->ip.netmask,netsetting.LocalMask.Str);	
		strcpy(stored_list.stored_ap->ip.gateway,netsetting.LocalGateway.Str);
		strcpy(stored_list.stored_ap->ip.dns_primary, netsetting.DNSHost[0]);
		strcpy(stored_list.stored_ap->ip.dns_secondary, netsetting.DNSHost[1]);
	}

	//����ȫ�ֱ����ı�־�����Ƿ񱣴�MAC
	if(WifiMacFlag)
	{
		memcpy(stored_list.stored_ap->mac.octet, WifiMac, 6);
		macflag = WIFI_AP_FLAG_MAC_SET;
	}
	else
	{
		memset(stored_list.stored_ap->mac.octet, 0, 6);
		macflag = 0;
	}
	//��ʹ�������ļ�ʱ�ֹ�����
	if(!g_conf_flag)
	{
		if(ENTER==cls_show_msg1(MAXWAITTIME, "�Ƿ��������ȼ�\n"
											"[ȷ��]����\n"
											"[����]������\n"))
		{
			priflag = WIFI_AP_FLAG_PRI_SET;

			cls_printf("�������������ȼ�,��Χ(0~255)��ֵԽ��Խ��:\n");
			sprintf(tmp, "%d", 0);
			if ((ret=lib_kbgetinput(tmp, 0, sizeof(tmp)-1, &len, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				stored_list.stored_ap->priority = 1;
			else
				stored_list.stored_ap->priority = atoi(tmp);

			if(stored_list.stored_ap->priority>=255)
				stored_list.stored_ap->priority=255;
		}
		else
			priflag = 0;

		if(ENTER==cls_show_msg1(MAXWAITTIME, "���ӵ�AP�Ƿ�Ϊ����AP\n"
											"[ȷ��]��\n"
											"[����]����\n"))
		{
			hiddenflag = WIFI_AP_FLAG_HIDDEN_SET;
		}
		else
			hiddenflag = 0;
	}
	else
	{
		if(WifiHiddenFlag)
			hiddenflag = WIFI_AP_FLAG_HIDDEN_SET;
		else
			hiddenflag = 0;
	}
	//��4����־λ��ϳ�״̬��־λflag
	stored_list.stored_ap->flag = dhcpflag|macflag|priflag|hiddenflag;
	stored_list.stored_num = 1;
	if((ret = NDK_WifiProfileSet(&stored_list)) != NDK_OK)
	{
		cls_show_msg("%s line %d:����AP�б�ʧ��(%d)", __FILE__, __LINE__, ret);
		return FAIL;
	}
	return SUCC;
}
#endif

//ѡ��ͨ��ɨ��ķ�ʽ������Ե�SSID��
static int select_scan_SSID(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0;
#if NEW_WIFI_ENABLE
	ST_WIFI_AP_LIST list_scan;
	ST_WIFI_AP_INFO ap[32];
	ST_WIFI_STATUS status;
	char ssidout[32][33];
	uint ssidoutlen = 0;
#else
	int signal = 0, numList = 0;
	char *ESSIDlist[64];
	char ESSIDlist1[64][64];
	ST_WIFI_APINFO ESSIDlist_info[64];
#endif
	
	/*process body*/
	cls_printf("ɨ��AP��,���Ժ�...");
#if NEW_WIFI_ENABLE
	//NDK_WifiGetFunc������Ҫ��start(wifi��������)����״̬ҪΪ�ȶ�(�����ա�ʧ�ܡ��ɹ�����״̬)ʱ������
	NDK_WifiStart();
	if((ret = lib_getwifistatus(&status)) != NDK_OK)
	{
		cls_show_msg("%s line %d:��ȡwifi״̬ʧ��(%d)", __FILE__, __LINE__,status.status);
		return FAIL;
	}
	list_scan.num = 32;
	list_scan.list = ap;
	for (j=0; j<3; j++) 
	{
		if ((NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan)) == NDK_OK) 
		{
			if (list_scan.num > 0) 
				break;
			else
			{
				cls_show_msg("%s line %d:δɨ�赽AP", __FILE__, __LINE__);
				return FAIL;
			}
		}
		sleep(1);
	}
	if (j == 3) 
	{
		cls_show_msg("%s line %d:WIFIɨ��ʧ��", __FILE__, __LINE__);
		return FAIL;
	}
	for (i=0; i<list_scan.num; i++)
	{
		if(!strcmp(ap[i].ssid,""))//���������ap  ֱ�Ӵ������buf
			strcpy(ssidout[i],ap[i].ssid);
		else
		{
			if((ret=NDK_Utf8ToGbk((uchar *)ap[i].ssid,strlen(ap[i].ssid),(uchar *)ssidout[i],&ssidoutlen))!=NDK_OK)
			{
				cls_show_msg("%s line %d:utf8ת��gbkʧ��(%d)", __FILE__, __LINE__, ret);
				return FAIL;
			}
		}
		if(cls_show_msg("�Ƿ�ѡAP%d:%s(%02x:%02x:%02x:%02x:%02x:%02x)Ϊ����AP,��[ȷ��],��[����]", i, ssidout[i],ap[i].mac.octet[0],ap[i].mac.octet[1],ap[i].mac.octet[2],ap[i].mac.octet[3],ap[i].mac.octet[4],ap[i].mac.octet[5])==ENTER)//ѡ��SSID��
			break;
	}
	if(i>=list_scan.num)
	{
		cls_show_msg("%s line %d:δɨ�赽������AP������ɨ��", __FILE__, __LINE__);
		return FAIL;
	}
	strcpy(WlanEssid, ap[i].ssid);
	
	//���ӱ���MAC��ַ�Ĳ��� ,����ȫ�ֱ����ͳ�ʼ��Ĭ��ֵ��
	if(cls_show_msg("�Ƿ񱣴�AP:%s��MAC��ַ%02x:%02x:%02x:%02x:%02x:%02x", ap[i].ssid,ap[i].mac.octet[0],ap[i].mac.octet[1],ap[i].mac.octet[2],ap[i].mac.octet[3],ap[i].mac.octet[4],ap[i].mac.octet[5])==ENTER)
	{
		memcpy(WifiMac, ap[i].mac.octet, 6);
		WifiMacFlag = 1;
	}
	else
	{
		memset(WifiMac, 0, sizeof(WifiMac));
		WifiMacFlag = 0;
	}
	NDK_WifiStop();
#else
	memset(ESSIDlist_info,0,sizeof(ESSIDlist_info));
	for (i=0; i<64; i++)
		ESSIDlist[i]=ESSIDlist1[i];
	if ((ret=NDK_WiFiInit()) != NDK_OK)
	{
		cls_show_msg("%s line %d:WIFI��ʼ��ʧ��ret=%d", __FILE__, __LINE__, ret);
		return FAIL;
	}
/*	
	//��ȡ�����е�AP��Ϣ
	for (j=0; j<3; j++)
	{
		if (NDK_WiFiGetNetInfo(ESSIDlist_info,64, &numList) == NDK_OK)
		{
			if (numList > 0)
			{
				for (i=0; i<numList; i++)
				    cls_show_msg("Line=%d\n AP[%d]->Essid:%s\n sSignal:%s", __LINE__,i, ESSIDlist_info[i].sEssid,ESSIDlist_info[i].sSignal);
				break;
			}
		}
		sleep(1);
	}	
*/	
	//ɨ��AP
	for (j=0; j<3; j++) 
	{
		if (NDK_WiFiGetNetList(ESSIDlist, &numList) == NDK_OK) 
		{
			if (numList > 0) 
				break;
			else
			{
				cls_show_msg("%s line %d:δɨ�赽AP", __FILE__, __LINE__);
				NDK_WiFiShutdown();
				return FAIL;
			}
		}
		sleep(1);
	}
	if (j == 3) 
	{
		cls_show_msg("%s line %d:WIFIɨ��ʧ��", __FILE__, __LINE__);
		return FAIL;
	}
	for (i=0; i<numList; i++)
	{
		
		ret=NDK_WiFiSignalCover(ESSIDlist1[i], &signal);
		if(cls_show_msg("��%d��AP,�Ƿ�ѡ��AP[%d]:%s�ź�Ϊ:%d��Ϊ����AP,�ǰ�[ȷ��],��������", numList, i, ESSIDlist1[i],signal)==ENTER)//ѡ��SSID��
			break;
	}
	if(i>=numList)
	{
		cls_show_msg("%s line %d:δɨ�赽������AP������ɨ��", __FILE__, __LINE__);
		NDK_WiFiShutdown();
		return FAIL;
	}
	strcpy(WlanEssid, ESSIDlist1[i]);
	if((ret=NDK_WiFiShutdown()) != NDK_OK)
	{
		cls_show_msg("%s line %d:NDK_WiFiShutdownʧ��ret=%d", __FILE__, __LINE__, ret);
		return FAIL;
	}
#endif

	return SUCC;
}

static int conf_WLAN_lnk(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin = 0;
	uint unLen=0;
#if NEW_WIFI_ENABLE
	char wifimactemp[18]={0};
#endif
	/*process body*/
	// 1.ɨ�貢ѡ������AP
	//ɨ��ǰ,��Ҫ���л���wifi�豸//��Ȼ��,ͨ����˵,SelectNetDev��Ҫ������conf.c��,���ŵ�layer��
	do
	{
		nKeyin = cls_show_msg("SSID���뷽��:\n"
								"1.����\n"
								"2.ɨ��\n");
		switch(nKeyin)
		{
		case '1':
			cls_printf("��ǰSSID:\n");
			if((ret=lib_kbgetinput(WlanEssid,0, sizeof(WlanEssid)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
				strcpy(WlanEssid, DEFAULT_WLAN_ESSID);//�������SSID,����ȡ������,ҲҪ����Ĭ��ֵ
#if NEW_WIFI_ENABLE
			//�����Ҫ��:����MAC����
			if(cls_show_msg("�Ƿ�����·����MAC,�ǰ�[ȷ��],����������")==ENTER)
			{
				cls_printf("������MAC�ַ���(��ʽXXXXXXXXXXXX):\n");
				if((ret=lib_kbgetinput(wifimactemp,0, sizeof(wifimactemp)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
					strcpy(wifimactemp, "A8:57:4E:95:2E:88");
				
				if(trim_char2uchar(wifimactemp,WifiMac)!=SUCC)
				{
					cls_show_msg("%s line %d:MAC���ô���", __FILE__, __LINE__);
					return FAIL;
				}
				WifiMacFlag = 1;
			}
			else
			{
				memset(WifiMac, 0, sizeof(WifiMac));
				WifiMacFlag = 0;
			}
#endif			
			break;
		case '2':
			if(select_scan_SSID()!=SUCC)
				return FAIL;
			break;
		case ESC:
			cls_show_msg1(2, "��ʹ��Ĭ��SSID:%s...", DEFAULT_WLAN_ESSID);		
			strcpy(WlanEssid, DEFAULT_WLAN_ESSID);//�������SSID,����ȡ������,ҲҪ����Ĭ��ֵ
			memset(WifiMac, 0, sizeof(WifiMac));
			WifiMacFlag = 0;
			break;	
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>'2'));
	
	conf_WLAN_lnk1();
	return SUCC;
}

int conf_conn_WLAN(void)
{
	/*private & local definition*/
	int nkeyin = ENTER;
	
	/*process body*/
	if((nkeyin=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin==0)
	{	
		if(NDK_FsExist(PARACONF)!=NDK_OK)
		{	
			cls_show_msg1(10,"�޲��������ļ�,�������ֶ�����");
			g_conf_flag = 0;
		}
		else
			g_conf_flag = 1;
	}	
	else
		g_conf_flag = 0;
	
	//�ֹ�������·����
	if(conf_link(WLAN)!=SUCC)
		return FAIL;
	if(cls_show_msg1(5,"��ȷ����TCP/UDP��̨!!!\n")==ESC)
		return NDK_ERR_QUIT;//QUIT;
	//�ֹ������������
	if(conf_network()!=SUCC)
		return FAIL;
	
#if NEW_WIFI_ENABLE
	//����ȫ�ֲ�����AP�б��ļ���,ͬʱ���ú��б�
	if(set_addedapinfo_fromgloabvar()!= SUCC)
	{
		cls_show_msg1(10, "%s, line %d:AP�б�����ʧ��!(%d)", __FILE__, __LINE__);
		return FAIL;
	}
#endif

	if(conf_trans()!=SUCC)
		return FAIL;

//	if(cls_show_msg1(5, "�Ƿ�Ҫ������������ͨ�ԣ��״�����wifiģ�����Ҫ��һ��")!=ENTER)
//		return SUCC;
		
	//�������,Ҫ����һ��������ͨ��
#if 0
	//�������,Ҫ����һ��������ͨ��
	return (IsNetConnected()==TRUE)?SUCC:FAIL;
#else
	return SUCC;
#endif

}
#else
int use_wifi_conf(int ApID) {return FAIL;}
void conf_WLAN_lnk1(void){}
#if NEW_WIFI_ENABLE
int set_addedapinfo_fromgloabvar(void){return FAIL;}
#endif
static int conf_WLAN_lnk(void){return FAIL;}
int conf_conn_WLAN(void){return FAIL;}
#endif

#if WIFIAP_ENABLE
//����WIFI AP���ܲ���
int conf_wifi_AP(void)  
{
	/*private & local definition*/
	int nKeyin = 0,ret = 0, nkeyin1 = ENTER;
	uint unLen=0;
	char *tip[] = {"WIFI_NET_SEC_NONE", "WIFI_NET_SEC_WEP_OPEN", "WIFI_NET_SEC_WEP_SHARED", "WIFI_NET_SEC_WPA", "WIFI_NET_SEC_WPA2"};
	char pInfo[70] = {0}, apssid[33] = {0}, apkey[65] = {0}, apip[16] = {0};
	int apsecmode = 0, aphideden = 0;

	/*process body*/	
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		//����ģʽ
		if(GetParaInfo("WifiApSecMode",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&apsecmode)!=SUCC)
			return FAIL;
		WifiApSecMode = apsecmode;
		//����
		if(GetParaInfo("WifiApKey",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,apkey)!=SUCC)
			return FAIL;
		strcpy(WifiApKey,apkey);
		// wifi ap ssid
		if(GetParaInfo("WifiApSsid",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,apssid)!=SUCC)
			return FAIL;
		strcpy(WifiApSsid,apssid);
		//�Ƿ�����
		if(GetParaInfo("WifiApHidden",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&aphideden)!=SUCC)
			return FAIL;
		WifiApHidden = (uchar)aphideden;
		//�ȵ�IP
		if(GetParaInfo("WifiApIp",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,apip)!=SUCC)
			return FAIL;
		strcpy(netsetting.WifiApIp.Str,apip);
		netsetting.WifiApIp.value.data = INET_ADDR(netsetting.WifiApIp.Str);
		cls_show_msg1(1,"AP�������óɹ�");
	}
	else
	{
	//2.����ģʽѡ��
		do
		{
			nKeyin = cls_show_msg("WIFI AP����ģʽ:\n"
									"0.NONE 1.OPEN\n"
									"2.SHARED 3.WPA\n"
									"4.WPA2\n");//��Ŵ�0ʼ,��Ϊ�˱�����ö��ֵһ��

#define MAXMENUITEM ('4')

			switch (nKeyin)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case MAXMENUITEM:
				WifiApSecMode = nKeyin-'0';
				cls_show_msg1(2, "������%s����ģʽ...", tip[WifiApSecMode]);
				break;
			case ESC:
				cls_show_msg1(2, "��ʹ��%s����ģʽ...", tip[WifiApSecMode]);
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'0'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

		//3.���ݲ�ͬ�ļ���ģʽд����Ӧ���ȵ�����
		if(WifiApSecMode==WIFI_NET_SEC_NONE)
			memset(WifiApKey,0,sizeof(WifiApKey));
		if(WifiApSecMode==WIFI_NET_SEC_WEP_OPEN)//WIFI_NET_SEC_WEP_OPEN:5��ASCII�ַ�����10��ʮ�������ַ���0-9��A-F��
			strcpy(WifiApKey, "01234");
		if(WifiApSecMode==WIFI_NET_SEC_WEP_SHARED)//WIFI_NET_SEC_WEP_ SHARED: 13��ASCII�ַ�����26��ʮ�������ַ���0-9��A-F��
			strcpy(WifiApKey, "0123456789012");
		if(WifiApSecMode==WIFI_NET_SEC_WPA||WifiApSecMode==WIFI_NET_SEC_WPA2)//WIFI_NET_SEC_WPA����WIFI_NET_SEC_WPA2����������8-64�������ַ���
			strcpy(WifiApKey, DEFAULT_WLANAP_KEY);

		//4.����WIFI�ȵ��SSID
		cls_printf("��ǰAP:\n");
		if((ret=lib_kbgetinput(WifiApSsid,0, sizeof(WifiApSsid)-1, &unLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(WifiApSsid, DEFAULT_WLANAP_SSID);//�������SSID,����ȡ������,ҲҪ����Ĭ��ֵ
		
		//5.�����Ƿ�����ssid
		do
		{
			nKeyin = cls_show_msg("�Ƿ�����SSID:\n"
									"0.������\n"
									"1.����\n");//��Ŵ�0ʼ,��Ϊ�˱�����ö��ֵһ��
#define MAXMENUITEM ('1')

			switch (nKeyin)
			{
			case '0':
				WifiApHidden = 0;
				cls_show_msg1(1, "������SSID������");
				break;
			case MAXMENUITEM:
				WifiApHidden = 1;
				cls_show_msg1(1, "������SSID����");
				break;
			case ESC:
				WifiApHidden = 0;
				cls_show_msg1(1, "������SSID������");
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}	
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'0'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM

		//����WiFi�ȵ�IP
		cls_printf("��ǰWiFi�ȵ�IP:\n");
		if ((ret=lib_kbgetinput(netsetting.WifiApIp.Str, 0, sizeof(netsetting.WifiApIp.Str)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(netsetting.WifiApIp.Str, DEFAULT_WIFIAP_IP_STR);  
		
		netsetting.WifiApIp.value.data = INET_ADDR(netsetting.WifiApIp.Str);
	}
	return SUCC;
}

int conf_conn_AP(void)
{
	/*private & local definition*/
	int nKeyin = 0, nkeyin1 = ENTER;
	char *tip[] = {"ETH", "WLM"};
	char pInfo[64] = {0};
	int apsharedev = 0;
	
	/*process body*/	
	//1.WiFi�ȵ��������緽ʽѡ��
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		g_conf_flag = 1;
		if(GetParaInfo("WifiApShareDev",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&apsharedev)!=SUCC)
			return FAIL;
		WifiApShareDev = apsharedev;
		cls_show_msg1(1,"AP���뷽ʽ���óɹ�");
	}
	else
	{
		g_conf_flag = 0;
		do
		{
			nKeyin = cls_show_msg("WIFI AP���뷽ʽ:\n"
									"0.ETH��ʽ\n"
									"1.WLM��ʽ\n");//��Ŵ�0ʼ,��Ϊ�˱�����ö��ֵһ��
#define MAXMENUITEM ('1')

			switch (nKeyin)
			{
			case '0':
			case MAXMENUITEM:
				WifiApShareDev = nKeyin-'0';
				cls_show_msg1(2, "������WIFI AP���뷽ʽ:%s...", tip[WifiApShareDev]);
				break;
			case ESC:
				cls_show_msg1(2, "��ʹ��WIFI AP���뷽ʽ:%s...", tip[WifiApShareDev]);
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}	
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'0'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
	}

	//���ݹ�����������������·��
	if(WifiApShareDev==WIFIAP_SHARE_DEV_ETH)
	{
		switch(conf_conn_ETH(TRUE))
		{
		case SUCC:
			break;
		case FAIL:
			cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
			break;
		case NDK_ERR_QUIT://�û�����
		default:
			break;
		}
	}
	else if(WifiApShareDev==WIFIAP_SHARE_DEV_PPP)
	{
		switch(conf_conn_WLM(TRUE))
		{
		case SUCC:
			cls_show_msg1(2, "�������óɹ�!");
			break;
		case FAIL:
			cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);	
			//return;
			break;
		case NDK_ERR_QUIT://�û�����
		default:
			//return;
			break;
		}
	}
	return SUCC;
}
#else
int conf_wifi_AP(void){return FAIL;}
int conf_conn_AP(void){return FAIL;}
#endif

#if USB_ENABLE||SD_ENABLE
static int g_disktype = UDISK;

//g_disktype���ݵĲ�������
void set_disktype(int type){g_disktype = type;}
int get_disktype(void){return g_disktype;}

int conf_disktype(void)//��������disk�豸����,����usb��sd������,linwl 20130311
{
	int nKeyin = 0, nkeyin1 = ENTER;
	char pInfo[64] = {0};
	int type = 0;
	
	/*process body*/
	if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	{
		if(GetParaInfo("DiskType",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&type)!=SUCC)
			return FAIL;
		if(type==1)
		{	
			set_disktype(SDDISK);
			cls_show_msg1(2, "ѡ���豸����ΪSD��");
		}
		else
		{
			set_disktype(UDISK);
			cls_show_msg1(2, "ѡ���豸����ΪU��");
		}
	}
	else
	{
		do
		{
			nKeyin = cls_show_msg("ѡ���豸����:\n"
									"1.USB 2.SDCard\n");

#define MAXMENUITEM ('2')		
			switch (nKeyin)
			{
			case '1':
				set_disktype(UDISK);
				cls_show_msg1(2, "ѡ���豸����ΪU��");
				break;
			case MAXMENUITEM://'2':
				set_disktype(SDDISK);
				cls_show_msg1(2, "ѡ���豸����ΪSD��");
				break;
			case ESC:
				return NDK_ERR_QUIT;//QUIT;
				break;
			default:
				cls_show_msg1(2, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}
		}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>MAXMENUITEM));
#undef MAXMENUITEM
	}
	return SUCC;
}

#else
void set_disktype(int type){}
int get_disktype(void){return FAIL;}
int conf_disktype(void){return FAIL;}
#endif
#endif

#if !defined ME66 //ME66 �ռ�С ��֧������ ������
#if BT_ENABLE
int conf_conn_BT(void)
{
#if !(defined ME15CHX||defined ME15B)//ME15C����治֧���ļ�ϵͳ 20170213 linwl
	/*private & local definition*/
	int nkeyin = ENTER;
	/*process body*/
	if((nkeyin=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin==0)
	{	
		if(NDK_FsExist(PARACONF)!=NDK_OK)
		{	
			cls_show_msg1(10,"�޲��������ļ�,�������ֶ�����");
			g_conf_flag = 0;
		}
		else
			g_conf_flag = 1;
	}	
	else
		g_conf_flag = 0;
#endif
	//�ֹ�������·����
	if(conf_link(BT)!=SUCC)
		return FAIL;
	
	return SUCC;
}

int g_at_enable = 0;//����ʼֵ
int g_pair_mode = 0;
int g_btdiscoverflag = 0;//�ֶ������㲥��־λ 1:��ʾ��Ҫ�ֶ������㲥  0:��ʾ����Ҫ�ֶ������㲥
int conf_bt(void)
{
	/*private & local definition*/
	int ret = -1;//, nkeyin1 = ENTER;
	uint unLen = 0;
	int nkey = 0;
	char sBuf[65]={0};
#if !(defined ME15CHX||defined ME15B)//ME15C����治֧���ļ�ϵͳ 20170213 linwl	
	char pInfo[32] = {0}, btname[16] = {0}, btpin[7] = {0};
#endif
	char *pairmodestr[]={"Just Work","PIN Code","SSP PIN","PassKey","Just Work SC"};

	/*process body*/
	//�����������ڲ�����
	//strcpy(btbps, conf_serial(&bpsid));

	//��ȡ����ģ��
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO, &unLen, sBuf)) != NDK_OK)
	{
		cls_show_msg("file:%s,line:%d��ȡ����ģ��ʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
		return FAIL;
	}
	switch(sBuf[13])
	{
#if K21_ENABLE   //�еͶ�����ģ�������� 20180815 modify sull
		case 0x01:
			cls_printf("����ģ��ΪBM77");//Ŀǰ��BM77ģ��֧��AT����ģʽ,Ҫ���������շ����ȵ���NDK_BTEnterCommand����AT����ģʽ,NDK_BTExitCommand�˳�����ģʽ��������ģʽ���������շ�
			g_at_enable = 1;//AT����ģʽ��־λ
			break;
		case 0x02:
			cls_printf("����ģ��ΪBT24");
			g_at_enable = 0;
			break;
		case 0x03:
			cls_printf("����ģ��ΪAP6210B");
			g_at_enable = 0;
			break;
		case 0x04:
			cls_printf("����ģ��ΪALLTEC20706");
			g_at_enable = 0;
			break;
		case 0x05:
			cls_printf("����ģ��ΪYC1021");
			g_at_enable = 0;
			g_btdiscoverflag = 1;
			break;
		case 0x06:
			//cls_printf("����ģ��ΪDA14681");
			cls_printf("����ģ��ΪTC35661");
			g_at_enable = 0;
			break;
		case 0x07:
			cls_printf("����ģ��Ϊ20707-A2");
			g_at_enable = 0;
			break;
#else
		case 0x01:
			cls_printf("����ģ��ΪSD8787");
			g_at_enable = 0;
			break;
		case 0x02:
			cls_printf("����ģ��ΪAP6210");
			g_at_enable = 0;
			break;
		case 0x03:
			cls_printf("����ģ��ΪAP6210B");
			g_at_enable = 0;
			break;
		case 0x04:
			cls_printf("����ģ��ΪAP6212");
			g_at_enable = 0;
			break;
		case 0x05:
			cls_printf("����ģ��ΪAP6236");
			g_at_enable = 0;
			break;
		case 0x06:
			cls_printf("����ģ��ΪAP6255");
			g_at_enable = 0;
 			break;
#endif
 		default:
			cls_printf("δ֪������ģ��");
			g_at_enable = 0;
			return FAIL;
			break;
	}
	//����������
	//if((ret=NDK_PortOpen(PORT_NUM_BT,btbps))!=NDK_OK)
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		cls_show_msg("file:%s,line:%d����������ʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
		return FAIL;
	}
	//������������ģʽ
	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		cls_show_msg("file:%s,line:%d������������ģʽʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
		NDK_PortClose(PORT_NUM_BT);
		return FAIL;
	}
	
#if !(defined ME15CHX||defined ME15B)//ME15C����治֧���ļ�ϵͳ 20170213 linwl
	//if((nkeyin1=cls_show_msg1(WAITTIME,"[Enter]ʹ�ò��������ļ�,[����]�ֶ�����"))==ENTER || nkeyin1==0)
	if(g_conf_flag==1)
	{
		if(GetParaInfo("BtName",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,btname)!=SUCC)
			return FAIL;
		strcpy(BtName,btname);
		if(GetParaInfo("BtPin",pInfo)!=SUCC)
			return FAIL;
		if(trim_string(pInfo,btpin)!=SUCC)
			return FAIL;
		strcpy(BtPin,btpin);
		if(GetParaInfo("BtPairMode",pInfo)!=SUCC)
			return FAIL;
		if(trim_string2int(pInfo,&g_pair_mode)!=SUCC)
			return FAIL;
		if(g_pair_mode<0||g_pair_mode>4)//�����ļ����õ����ģʽ���Ϸ�ʱ����ΪJust Workģʽ
			g_pair_mode=0;
		cls_show_msg1(1,"�������óɹ�");
	}
	else//�ֶ�����
#endif
	{
		//������������
		cls_printf("��ǰ��������:\n");
		if ((ret=lib_kbgetinput(BtName,0, sizeof(BtName)-1, &unLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(BtName, DEFAULT_BT_NAME);//�������BtName,����ȡ������,ҲҪ����Ĭ��ֵ
		
		//��������PIN
		cls_printf("��ǰ����PIN:\n");
		if ((ret=lib_kbgetinput(BtPin,0, sizeof(BtPin)-1, &unLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_NOLIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			strcpy(BtPin, DEFAULT_BT_PIN);//�������BtPin,����ȡ������,ҲҪ����Ĭ��ֵ	

		//�����������ģʽ
		cls_show_msg1(2,"�������������ģʽ(��ʾ:ƻ���ֻ�6s���°汾ֻ������just workģʽ)");
		do
		{
			nkey = cls_show_msg(  "1.Just Work\n"
								"2.PIN Code\n"
								"3.SSP PIN\n"
								"4.PassKey\n"
								"5.Jusk Work SC"
								);
			switch(nkey)
			{	
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':	
					g_pair_mode=nkey-'1';
					break;
				case ESC:
				default:
#if BTPCI_ENABLE
					g_pair_mode=2;
#else
					g_pair_mode=0;
#endif
					break;
			}
		}while(nkey!=0 && nkey!=ESC && (nkey<'1'||nkey>'5'));
		
	}
	//cls_show_msg("������������:%s,����PIN:%s,Ϊ%sģʽ",BtName ,BtPin ,pairmodestr[g_pair_mode]);
	
	//������������
	if((ret=NDK_BTSetLocalName(BtName))!=NDK_OK)
	{
		cls_show_msg("file:%s,line:%d������������ʧ��(ret=%d,name=%s)",__FILE__ ,__LINE__, ret, BtName);
		NDK_BTExitCommand();
		NDK_PortClose(PORT_NUM_BT);
		return FAIL;
	}
#if !(defined ME20|| BTPCI_ENABLE)//ME20 ��֧��BLEģʽ�ú�������-18 //PCI��֤�汾��֧�ִ˺���
	//��������PIN
	if((ret=NDK_BTSetPIN(BtPin))!=NDK_OK)
	{
		cls_show_msg("file:%s,line:%d��������PINʧ��(ret=%d,pin=%s)",__FILE__ ,__LINE__, ret, BtPin);
		NDK_BTExitCommand();
		NDK_PortClose(PORT_NUM_BT);
		return FAIL;
	}
#endif
	//�������ģʽ
#if JUSTWORK_FLAG
	g_pair_mode = 0;//ֻ֧��JustWork���豸��ֱ���޸����ģʽΪJUSTWORK
#endif
	NDK_BTSetPairingMode(g_pair_mode);
	cls_show_msg1(1,"����Ϊ%sģʽ",pairmodestr[g_pair_mode]);
	
	//�˳���������ģʽ
	if((ret=NDK_BTExitCommand())!=NDK_OK)
	{
		cls_show_msg("file:%s,line:%d�˳���������ģʽʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
		NDK_PortClose(PORT_NUM_BT);
		return FAIL;
	}
	NDK_BTDisconnect();
	if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
	{
		cls_show_msg("file:%s,line:%d�ر���������ʧ��(ret=%d)",__FILE__ ,__LINE__, ret);
		return FAIL;
	}
	
	return SUCC;
}
#else
int conf_conn_BT(void) {return FAIL;}
int conf_bt(void) {return FAIL;}
#endif
#endif

#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)	//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ��� 20170213 linwl
#if NFC_ENABLE
int  (*Nfc_Activate)(uchar *psRecebuf,int *pnRecvlen,int nSeekCnt) = NDK_NfcUMS_Activate; 
void conf_nfc_datatype(void)
{
	/*private & local definition*/
	int nKeyin = 0;

	/*process body*/
	do
	{
		nKeyin = cls_show_msg("ѡ��NFC���ݸ�ʽ:\n"
								"1.NDK_NfcUMS_Activate\n"
								"2.NDK_Nfc_Activate\n");
		
		switch (nKeyin)
		{
		case ESC:
		case '1':
			cls_show_msg1(2, "��ʹ��NDK_NfcUMS_Activate...");
			Nfc_Activate = NDK_NfcUMS_Activate;
			break;
		case '2':
			cls_show_msg1(2, "��ʹ��NDK_Nfc_Activate...");
			Nfc_Activate = NDK_Nfc_Activate;
			break;
		default:
			cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
			continue;
			break;
		}
	}while(nKeyin!=0 && nKeyin!=ESC && (nKeyin<'1'||nKeyin>'2'));

	return;
}
#else
void conf_nfc_datatype(void){cls_show_msg("��֧��NFC");return;}
#endif
#endif

#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B)	//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ��� 20170213 linwl
int g_ScanType=0;
#if CAMERA_ENABLE
int g_ShowImage = 1, g_Saturation = 10, g_Sharpness = 5, g_Contrast = 12;
//����ͷ�������ú���
void conf_camera(void)
{
	int ret=0;
	uint unLen=0;
	char CntStr[8]={0};

	/*//�������ƶ���systest33
	cls_printf("ѡ��ɨ��ģʽ:0.ͬ��ɨ�� 1.�첽ɨ��:");
	strcpy(CntStr, "1");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		g_ScanType = 1;
	else
		g_ScanType = atoi(CntStr);*/
	
	cls_printf("�Ƿ������ͷ(0Ϊ�ر�,1Ϊ��):");
	strcpy(CntStr, "1");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		g_ShowImage = 1;
	else
		g_ShowImage = atoi(CntStr);
	
	cls_printf("��������ͷ����(��ЧֵX~X):");
	strcpy(CntStr, "10");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		g_Saturation = 10;
	else
		g_Saturation = atoi(CntStr);
	
	cls_printf("��������ͷ�ع�(��ЧֵX~X):");
	strcpy(CntStr, "5");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		g_Sharpness = 5;
	else
		g_Sharpness = atoi(CntStr);
	
	cls_printf("��������ͷ֡��(��ЧֵX~X):");
	strcpy(CntStr, "12");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		g_Contrast = 12;
	else
		g_Contrast = atoi(CntStr);

	return;
}
#else
void conf_camera(void){cls_show_msg("�ò�Ʒ��֧������ͷ");return;}
#endif

#if SCAN_ENABLE||SCANASYCN_ENABLE
int g_Floodlight = 0, g_Focuslight = 0, g_Sensitivity = 11;
//ɨ��ͷ�������ú���
void conf_scan(void)
{
	/*private & local definition*/
	int ret=0;
	uint unLen=0;
	char CntStr[8]={0};

	/*process body*/
	g_ScanType = 0;//ɨ��ͷ��֧������ʽ����
	if(cls_show_msg("�Ƿ�Ĭ������,��[ENTER],����[����]")==ENTER)
	{
		NDK_ScanSet(SCAN_SETTYPE_FACTORYDEFAULT,11);
		g_Floodlight = 0;
		g_Focuslight = 0;
		g_Sensitivity = 11;
	}
	else
	{
		cls_printf("����������(0.��˸ 1.������ 2.���볣��):");
		strcpy(CntStr, "0");
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			g_Floodlight = 0;
		else
			g_Floodlight = atoi(CntStr);
			
		cls_printf("���öԽ���(0.��˸ 1.�޶Խ� 2.��Ӧ):");
		strcpy(CntStr, "0");
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			g_Focuslight = 0;
		else
			g_Focuslight = atoi(CntStr);
		
		cls_printf("����������(1-20):");
		strcpy(CntStr, "11");
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			g_Sensitivity = 11;
		else
			g_Sensitivity = atoi(CntStr);
	}

	return;
}
#else
void conf_scan(void){cls_show_msg("�ò�Ʒ��֧��ɨ��ͷ");return;}
#endif
#endif

