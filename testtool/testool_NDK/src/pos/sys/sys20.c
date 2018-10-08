/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys20.c
* Author 			: chensj
* version			: 
* DATE				: 20130829
* directory 		: 
* description		: ����NDK_InitStatisticsData,NDK_GetStatisticsData�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysInitStatisticsData,NDK_SysGetStatisticsData"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		   chensj	          20130829	 	created
*****************************************************************/
#if !(defined ME50N||defined ME50C||defined ME50H||defined SP610||defined ME62||defined ME50NX||defined ME68)
void sys20(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int i = 0, nRet = 0;
	ulong stOutPut = 0;
#if OVERSEAS_ENABLE
	EM_SS_DEV_ID devid[] = {SS_KEYBOARD_ENTER, SS_KEYBOARD_BASP,SS_KEYBOARD_ESC,SS_KEYBOARD_ZMK,SS_KEYBOARD_ZERO,SS_KEYBOARD_DOT,\
		SS_KEYBOARD_NINE,SS_KEYBOARD_EIGHT,SS_KEYBOARD_SEVEN,SS_KEYBOARD_SIX,SS_KEYBOARD_FIVE,SS_KEYBOARD_FOUR,SS_KEYBOARD_THREE,\
		SS_KEYBOARD_TWO,SS_KEYBOARD_ONE,SS_KEYBOARD_F1,SS_KEYBOARD_F2,SS_KEYBOARD_F3,SS_KEYBOARD_F4,SS_KEYBOARD_0_ID,SS_KEYBOARD_1_ID,\
		SS_KEYBOARD_2_ID,SS_KEYBOARD_3_ID,/*SS_KEYBOARD_4_ID,SS_KEYBOARD_5_ID,SS_KEYBOARD_6_ID,SS_KEYBOARD_7_ID,SS_KEYBOARD_11_ID,\
		SS_KEYBOARD_15_ID,SS_KEYBOARD_19_ID,SS_KEYBOARD_27_ID,*/SS_KEYBOARD_TOTAL,SS_PRT_PAPER_ID,SS_PRT_HEAT_ID,SS_PRT_STITCH_ID,SS_MAG_TIMES_ID,\
		SS_ICCARD_BASE_ID,SS_RFID_TIMES_ID,SS_MODEM_TIMES_ID,SS_MODEM_FAILTIMES_ID,SS_MODEM_SDLCTIME_ID,SS_MODEM_ASYNTIME_ID,\
		SS_WLS_TIMES_ID,SS_WLS_FAILTIMES_ID,SS_WLS_PPPTIME_ID,SS_WIFI_TIMES_ID,SS_WIFI_TIME_ID,SS_POWER_TIMES_ID,SS_POWERUP_TIME_ID,\
		SS_BATTERY_CHARGE_ID,SS_BUTTON_CELL_VOLT};
#else
	EM_SS_DEV_ID devid[] = {SS_KEYBOARD_ENTER, SS_KEYBOARD_BASP,SS_KEYBOARD_ESC,SS_KEYBOARD_ZMK,SS_KEYBOARD_ZERO,SS_KEYBOARD_DOT,\
		SS_KEYBOARD_NINE,SS_KEYBOARD_EIGHT,SS_KEYBOARD_SEVEN,SS_KEYBOARD_SIX,SS_KEYBOARD_FIVE,SS_KEYBOARD_FOUR,SS_KEYBOARD_THREE,\
		SS_KEYBOARD_TWO,SS_KEYBOARD_ONE,SS_KEYBOARD_F1,SS_KEYBOARD_F2,SS_KEYBOARD_F3,SS_KEYBOARD_F4,SS_KEYBOARD_0_ID,SS_KEYBOARD_1_ID,\
		SS_KEYBOARD_2_ID,SS_KEYBOARD_3_ID,/*SS_KEYBOARD_4_ID,SS_KEYBOARD_5_ID,SS_KEYBOARD_6_ID,SS_KEYBOARD_7_ID,SS_KEYBOARD_11_ID,\
		SS_KEYBOARD_15_ID,SS_KEYBOARD_19_ID,SS_KEYBOARD_27_ID,*/SS_KEYBOARD_TOTAL,SS_PRT_PAPER_ID,SS_PRT_HEAT_ID,SS_PRT_STITCH_ID,SS_MAG_TIMES_ID,\
		SS_ICCARD_BASE_ID,SS_RFID_TIMES_ID,SS_MODEM_TIMES_ID,SS_MODEM_FAILTIMES_ID,SS_MODEM_SDLCTIME_ID,SS_MODEM_ASYNTIME_ID,\
		SS_WLS_TIMES_ID,SS_WLS_FAILTIMES_ID,SS_WLS_PPPTIME_ID,SS_WIFI_TIMES_ID,SS_WIFI_TIME_ID,SS_POWER_TIMES_ID,SS_POWERUP_TIME_ID};
#endif
#if OVERSEAS_ENABLE
	char *devstr[] = {"ȷ�ϼ�","�˸��","ȡ����","��ĸ��","���ּ�0","С�����",\
		"���ּ�9","���ּ�8","���ּ�7","���ּ�6","���ּ�5","���ּ�4","���ּ�3",\
		"���ּ�2","���ּ�1","�����F1","�����F2","�����F3","�����F4","δ�����0","δ�����1",\
		"δ�����2","δ�����3",/*"δ�����4","δ�����5","δ�����6","δ�����7","δ�����11",\
		"δ�����15","δ�����19","δ�����27",*/"���м�","��ӡ������","����ͷ���Ⱥ�����","��������","ˢ������",\
		"�忨����","Ѱ������","MDM���Ӵ���","MDM����ʧ�ܴ���","ͬ��MDMʱ��","�첽MDMʱ��",\
		"�������Ӵ���","��������ʧ�ܴ���","�������Ӻ�����","WIFI���Ӵ���","WIFI���Ӻ�����","���ػ�����","����ʱ��",\
		"��س�ŵ����","Ŧ�۵�ص���"
	};
#else
	char *devstr[] = {"ȷ�ϼ�","�˸��","ȡ����","��ĸ��","���ּ�0","С�����",\
		"���ּ�9","���ּ�8","���ּ�7","���ּ�6","���ּ�5","���ּ�4","���ּ�3",\
		"���ּ�2","���ּ�1","�����F1","�����F2","�����F3","�����F4","δ�����0","δ�����1",\
		"δ�����2","δ�����3",/*"δ�����4","δ�����5","δ�����6","δ�����7","δ�����11",\
		"δ�����15","δ�����19","δ�����27",*/"���м�","��ӡ������","����ͷ���Ⱥ�����","��������","ˢ������",\
		"�忨����","Ѱ������","MDM���Ӵ���","MDM����ʧ�ܴ���","ͬ��MDMʱ��","�첽MDMʱ��",\
		"�������Ӵ���","��������ʧ�ܴ���","�������Ӻ�����","WIFI���Ӵ���","WIFI���Ӻ�����","���ػ�����","����ʱ��"};
#endif

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�������Թ���
	for(i=0;i<(sizeof(devid)/sizeof(devid[0]));i++)
	{
		stOutPut = 0;
		if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut))!=NDK_OK)
		{
			send_result("line %d:%s��ȡ%s����ʧ��(%d)", __LINE__, TESTAPI, devstr[i], nRet);
			RETURN;
		}
		if(cls_show_msg("%sͳ�ƴ���Ϊ%ld\n[ȷ��]ͨ��,[����]ʧ��", devstr[i], stOutPut) != ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
	}

	//case2:�쳣����
	if((nRet = NDK_SysGetStatisticsData(devid[0],NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}	
	if((nRet = NDK_SysGetStatisticsData(-1,&stOutPut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//case 2.2 ����ö�ٷ�Χ��
	if((nRet = NDK_SysGetStatisticsData(SS_KEYBOARD_0_ID-1,&stOutPut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
#if OVERSEAS_ENABLE
	if((nRet = NDK_SysGetStatisticsData(SS_BUTTON_CELL_VOLT+1,&stOutPut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
#else
	if((nRet = NDK_SysGetStatisticsData(SS_POWERUP_TIME_ID+1,&stOutPut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
#endif
	
	//case3:����ͳ����չ���(������ͳ�����ۺϲ�����ɺ��ڽ��д˲���)
	if(cls_show_msg("�Ƿ����ͳ����Ϣ��չ���\n[ȷ��]��  [����]��")==ENTER)
	{
		//��֮ǰ�Ƚ���ȷ�ϼ��� ���м���ͳ�ƴ������ݿ�,Ȼ�������Ķ���,�Ų���Ӱ������ͳ��
		stOutPut = 0;
		if((nRet = NDK_SysGetStatisticsData(SS_KEYBOARD_ENTER,&stOutPut))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		stOutPut = 0;
		if((nRet = NDK_SysGetStatisticsData(SS_KEYBOARD_TOTAL,&stOutPut))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		cls_printf("������֤�Ƿ�����,���𰴼�");
		//��������
		if((nRet = NDK_SysInitStatisticsData())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		//�ж��Ƿ�����
		for(i=0;i<(sizeof(devid)/sizeof(devid[0]));i++)
		{
			if(devid[i] == SS_POWERUP_TIME_ID)   /* ����ʱ��������forѭ�����ǻ���*/
				continue ;
			
			stOutPut = 255;
			if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
				RETURN;
			}
			if(stOutPut!=0)
			{
				send_result("line %d:%sͳ��δ���(%ld)", __LINE__, devstr[i], stOutPut);
				RETURN;
			}
		}

		//����ͳ�Ƶ�ʱ�䲻ӦΪ0����ʹ��պ�ҲӦ�����Ͽ�ʼ����
		stOutPut = 0;
		if((nRet = NDK_SysGetStatisticsData(SS_POWERUP_TIME_ID,&stOutPut))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		if(stOutPut==0)
		{
			send_result("line %d:%s����ʧ��(%ld)", __LINE__,  devstr[i], stOutPut);
			RETURN;
		}
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}
#else
void sys20(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int i = 0, ret = 0;
	gprs_erf_info_t Gprs_Info;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 
	
	memset(&Gprs_Info,0,sizeof(gprs_erf_info_t));
	//case1:�쳣����
	if((ret = NDK_SysGetStatisticsData(SS_WLS_50N_GPRS, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret = NDK_SysGetStatisticsData(-1, &Gprs_Info))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case 1.2 ����ö�ٷ�Χ��
	if((ret = NDK_SysGetStatisticsData(SS_WLS_50N_GPRS-1, &Gprs_Info))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SysGetStatisticsData(SS_WLS_50N_GPRS+1, &Gprs_Info))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�������Թ���,Ӧ����ʾ��Ӧ����Ϣ(6����Ŀ�ֱ�Ϊ:RF��Ƶ,PO ppp open,SO sckt open,PC ppp close,SC sckt open,SS sckt send)
	if( (ret=NDK_SysGetStatisticsData(SS_WLS_50N_GPRS, &Gprs_Info)) != NDK_OK )
	{
		send_result("line %d:%s��ȡGPRS��Ϣ����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	for(i=0; i<6; i++)
	{
		if(cls_show_msg("%s��ִ�д���:%d\n�ܳɹ�����:%d\n��fail_1����:%d\n��fail_2����:%d\n[ȷ��]ͨ��,[����]ʧ��", &Gprs_Info.details[i].item, Gprs_Info.details[i].n_total, Gprs_Info.details[i].n_suc, Gprs_Info.details[i].n_fail_1, Gprs_Info.details[i].n_fail_2) != ENTER)
		{
			send_result("line %d:%s����ʧ��(item=%s, total=%d, succ=%d, fail1=%d, fail2=%d)", __LINE__, TESTAPI, &Gprs_Info.details[i].item, Gprs_Info.details[i].n_total, Gprs_Info.details[i].n_suc, Gprs_Info.details[i].n_fail_1, Gprs_Info.details[i].n_fail_2);
			RETURN;
		}
	}
		
	//case3:����ͳ����չ���
	if(cls_show_msg("�Ƿ����GPRSͳ����Ϣ��չ���\n[ȷ��]��  [����]��")==ENTER)
	{
		cls_printf("������֤�Ƿ�����");
		//��������
		if((ret = NDK_SysInitStatisticsData())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		memset(&Gprs_Info,0,sizeof(gprs_erf_info_t));
		//�ж��Ƿ�����
		for(i=0; i<6; i++)
		{
			if( (ret=NDK_SysGetStatisticsData(SS_WLS_50N_GPRS, &Gprs_Info)) != NDK_OK )
			{
				send_result("line %d:%s��ȡGPRS��Ϣ����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((Gprs_Info.details[i].n_total!=0) || (Gprs_Info.details[i].n_suc!=0) || (Gprs_Info.details[i].n_fail_1!=0) || (Gprs_Info.details[i].n_fail_2!=0))
			{
				send_result("line %d:%s����ʧ��(item=%s, total=%d, succ=%d, fail1=%d, fail2=%d)", __LINE__, TESTAPI, &Gprs_Info.details[i].item, Gprs_Info.details[i].n_total, Gprs_Info.details[i].n_suc, Gprs_Info.details[i].n_fail_1, Gprs_Info.details[i].n_fail_2);
				RETURN;
			}
		}
	}
	
	//���Խ���
	send_result("%s����ͨ��,����������ۺϲ��Ժ�ִ�б�����,Ӧ�ܻ�ȡ����ȷ����Ϣ.", TESTAPI);
	return;
}
#endif
