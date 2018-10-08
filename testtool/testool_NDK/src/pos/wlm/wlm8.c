/************************************************************************
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm8.c
* Author 		: jiangym
* version		: 
* DATE			: 20130821
* directory 		:
* description		: ����NDK_WlGetStationInfo�ӿڹ���
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
#define TESTAPI	"NDK_WlGetStationInfo"	
#define	MAXWAITTIME	30
#define	WAITTIME	2

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm8(void)
* functional description 	: ��ȡ���߻�վ��Ϣ
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  				
*****************************************************************/
void wlm8(void)
{
	if(auto_flag==1)
	{
		send_result("%s�Զ����Բ�����Ϊ���ղ��Խ���������ֶ�������֤", TESTAPI);
		//return;
	}
	/*private & local definition*/
	int ret = 0, i = 1;
	ST_MOBILE_STATION_INFO pstinfo;
	char *typestr[] = {"GPRSģ��", "CDMAģ��", "WCDMAģ��", "TDSCDMAģ��","4Gģ��"};
   	 ST_STATION_INFO p;
    	ST_STATION_INFO *p1;
	ST_CDMA_STATION_INFO c;
	float getinfo_time = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	cls_show_msg1(MAXWAITTIME, "���β���Ҫ���ڲ忨��δ�忨��������·ֱ���ԣ���Ҫ�ڹػ�״̬�½��аβ忨", TESTAPI);
	//case1:�쳣����
	if((ret = NDK_WlGetStationInfo(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�����Ƿ�ɹ�,���ٶ�λ��ʱ����Ӧ����2s
	memset(&pstinfo,0,sizeof(pstinfo)); 
	lib_StartStopwatch();
	if((ret = NDK_WlGetStationInfo(&pstinfo))!=NDK_OK)
	{
		send_result("line %d: %s��ȡ��վ��Ϣʧ��(ע��:CDMA�ڲ��忨�����ʧ����������)(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((getinfo_time=lib_ReadStopwatch())> WAITTIME)
	{
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(getinfo_time,str);
		send_result("line %d: %s����ʧ��,��λʱ�䳬��2��(%s)", __LINE__, TESTAPI, str);
#else
		send_result("line %d: %s����ʧ��,��λʱ�䳬��2��(%f)", __LINE__, TESTAPI, getinfo_time);
#endif
		RETURN;
	}
	else
	{
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(getinfo_time,str);
		send_result("���ٶ�λʱ��Ϊ%s", str);
#else
		send_result("���ٶ�λʱ��Ϊ%f", getinfo_time);
#endif
	}
	
	//case3:��������:GPRS�в忨��δ�忨��Ӧ�ܻ�ȡ������վ��ȫ�����ڻ�վ����Ϣ;CDMA�в忨��Ӧ�ܻ�ȡ������վ��Ϣ,��δ�忨�����,Ӧ���ܻ�ȡ������վ����Ϣ
	if(pstinfo.emModuleType<MODULE_TYPE_GPRS||pstinfo.emModuleType>MODULE_TYPE_LTE)
	{
		send_result("line %d: %s����ʧ��(δ֪��ģ������:%d)", __LINE__, TESTAPI, pstinfo.emModuleType);
		RETURN;
	}
	send_result("��ȡ����ģ������Ϊ:%s,���ж��Ƿ���ȷ", typestr[pstinfo.emModuleType]);
	switch(pstinfo.emModuleType)
	{
		case MODULE_TYPE_GPRS:
			cls_show_msg1(1,"GPRSģ��");
            p=pstinfo.ModuleStationInfo.GPRS_STATION.stMainStation;
			send_result("����վ��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",p.unMCC,p.unMNC,p.unLac,p.unCi,p.ndbm);
            p1=pstinfo.ModuleStationInfo.GPRS_STATION.stNeighborStation;
            for(;i<7;i++,p1++)
				send_result("���ڻ�վ%d��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",i,(*p1).unMCC,(*p1).unMNC,(*p1).unLac,(*p1).unCi,(*p1).ndbm); 
            break;
		case MODULE_TYPE_CDMA:
			cls_show_msg1(2,"CDMAģ��");
			c=pstinfo.ModuleStationInfo.CDMA_STATION;
			send_result("����վ��Ϣ,���Һ�:%d,�����:%d,�������:%d,Ƶ��:%d,ϵͳʶ����:%d,����ʶ����:%d,�����е�ĳС��:%d,���ж��Ƿ���ȷ", c.unMCC, c.unMNC, c.unBandClass, c.unChannel, c.unBid, c.unSid, c.unNid);
			break;
		case MODULE_TYPE_WCDMA:
			cls_show_msg1(2,"WCDMAģ��");
			break;
		case MODULE_TYPE_TDSCDMA:
			cls_show_msg1(2,"TDSCDMAģ��");
			break;
		case MODULE_TYPE_LTE:			
			cls_show_msg1(1,"4Gģ��");
         	   	p=pstinfo.ModuleStationInfo.GPRS_STATION.stMainStation;
			send_result("����վ��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",p.unMCC,p.unMNC,p.unLac,p.unCi,p.ndbm);
            		p1=pstinfo.ModuleStationInfo.GPRS_STATION.stNeighborStation;
            		for(;i<7;i++,p1++)
				send_result("���ڻ�վ%d��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",i,(*p1).unMCC,(*p1).unMNC,(*p1).unLac,(*p1).unCi,(*p1).ndbm); 
            		break;
		default:
			cls_show_msg("�����ڵ�ģ��");
			break;
	}

	//���Խ���
	send_result("%s����ͨ��,GPRS��4GӦ��ʾ1������վ��6�����ڻ�վ��Ϣ,CDMA��ʾһ������վ��Ϣ", TESTAPI);
	return;
}


