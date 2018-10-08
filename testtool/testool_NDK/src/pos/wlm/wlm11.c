/************************************************************************
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm11.c
* Author 		:  
* version		: 
* DATE			:
* directory 		:
* description		: ����NDK_WlGetStationInfoGeneral�ӿڹ���
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
#define TESTAPI	"NDK_WlGetStationInfoGeneral"	
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
void wlm11(void)
{
	if(auto_flag==1)
	{
		send_result("%s�Զ����Բ�����Ϊ���ղ��Խ���������ֶ�������֤", TESTAPI);
		//return;
	}
	/*private & local definition*/
	int ret = 0, i = 1;
	ST_MOBILE_STATION_INFO_GENERAL pstinfo;
	char *typestr[] = {"GPRSģ��","CDMAģ��","WCDMAģ��" ,"TDSCDMAģ��" ,"4Gģ��"};
   	 ST_STATION_INFO p;
    	ST_STATION_INFO *p1;
	ST_CDMA_STATION_INFO c;
	ST_STATION_INFO_IMPROVE g ;
	ST_STATION_INFO_IMPROVE *g1 ;
	float getinfo_time = 0.0;

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	cls_show_msg1(MAXWAITTIME, "���β���Ҫ���ڲ忨��δ�忨��������·ֱ����", TESTAPI);
	//case1:�쳣����
	if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_DEFAULT-1,&pstinfo))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_CDMAHDR+1,&pstinfo))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_DEFAULT,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�����Ƿ�ɹ�,���ٶ�λ��ʱ����Ӧ����2s
	memset(&pstinfo,0,sizeof(pstinfo)); 
	lib_StartStopwatch();
	if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_DEFAULT,&pstinfo))!=NDK_OK)
	{
		send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((getinfo_time=lib_ReadStopwatch())> WAITTIME)
	{
		send_result("line %d: %s����ʧ��,��λʱ�䳬��2��(%f)", __LINE__, TESTAPI, getinfo_time);
		RETURN;
	}
	else
	{
		send_result("���ٶ�λʱ��Ϊ%f", getinfo_time);
	}
	
	//case3:��������:Station_typeΪ0���ܻ�ȡ4Gģ���վ���ܻ�ȡ2Gģ���վ
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
            		for(i=1;i<7;i++,p1++)
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
            		g=pstinfo.ModuleStationInfo.LTE_STATION.stMainStation;
			send_result("����վ��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",g.unMCC,g.unMNC,g.unLac,g.unCi,g.ndbm);
          		g1=pstinfo.ModuleStationInfo.LTE_STATION.stNeighborStation;
            		for(i=1;i<11;i++,g1++)
				send_result("���ڻ�վ%d��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",i,(*g1).unMCC,(*g1).unMNC,(*g1).unLac,(*g1).unCi,(*g1).ndbm); 
			break;
		default:
			cls_show_msg("�����ڵ�ģ��");
			break;
	}
	
	//case4:���EM_MODULE_TYPEΪMODULE_TYPE_LTE��ô�Ϳ�����Station_type���Բ�ͬ�����
	if(pstinfo.emModuleType==MODULE_TYPE_LTE)
	{	
		//station_type = 1 ��ȡ4Gģ��GSM��վ��Ϣ
		memset(&pstinfo,0,sizeof(pstinfo)); 
		memset(&g,0,sizeof(g)); 
		if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_GSM,&pstinfo))!=NDK_OK)
		{
			send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		g=pstinfo.ModuleStationInfo.LTE_STATION.stMainStation;
		send_result("����վ��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",g.unMCC,g.unMNC,g.unLac,g.unCi,g.ndbm);
		g1=pstinfo.ModuleStationInfo.LTE_STATION.stNeighborStation;
		for(i=1;i<11;i++,g1++)
			send_result("����GSM��վ%d��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",i,(*g1).unMCC,(*g1).unMNC,(*g1).unLac,(*g1).unCi,(*g1).ndbm); 

		//station_type = 2 ��ȡ4Gģ��LTE��վ��Ϣ (�����Ѿ���ȡ��4G/LTE��Ϣ�����ﲻ�ٻ�ȡ)
		//station_type = 3 ��ȡ4Gģ��TDSCDMA��վ��Ϣ
		memset(&pstinfo,0,sizeof(pstinfo)); 
		if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_TDSCDMA,&pstinfo))!=NDK_OK)
		{
			send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		g1=pstinfo.ModuleStationInfo.LTE_STATION.stNeighborStation;
		for(i=1;i<11;i++,g1++)
			send_result("����TDSCDMA��վ%d��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",i,(*g1).unMCC,(*g1).unMNC,(*g1).unLac,(*g1).unCi,(*g1).ndbm); 

		//station_type = 5 ��ȡ4Gģ��STATION_TYPE_CDMAHDR��վ��Ϣ
		memset(&pstinfo,0,sizeof(pstinfo)); 
		if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_CDMAHDR,&pstinfo))!=NDK_OK)
		{
			send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		g1=pstinfo.ModuleStationInfo.LTE_STATION.stNeighborStation;
		for(i=1;i<11;i++,g1++)
			send_result("����CDMAHDR��վ%d��Ϣ,���Һ�:%d,�����:%d,λ��������:%d,С������:%d,�ź�ǿ��:%d,���ж��Ƿ���ȷ",i,(*g1).unMCC,(*g1).unMNC,(*g1).unLac,(*g1).unCi,(*g1).ndbm); 
	}
	else // ���Ϊ2Gģ�� �����0��Ĳ������᷵�ز���ʧ��
	{	
		memset(&pstinfo,0,sizeof(pstinfo)); 
		if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_GSM,&pstinfo))!=NDK_ERR_PARA)
		{
			send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_TDSCDMA,&pstinfo))!=NDK_ERR_PARA)
		{
			send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_WlGetStationInfoGeneral(STATION_TYPE_CDMAHDR,&pstinfo))!=NDK_ERR_PARA)
		{
			send_result("line %d: %s��ȡ��վ��Ϣʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}




