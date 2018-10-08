/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys35.c
* Author 			: 
* version			: 
* DATE				: 
* directory 		: 
* description		: ��������NDK_SysGetRtcTime,NDK_SysSetRtcTime�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetRtcTime,NDK_SysSetRtcTime"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	       sull       20171031      created 
*****************************************************************/
void sys35(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, i = 0;
	struct tm stOldRtcTime;
	struct tm stOldPosTime;
	struct tm stGetRtcTime;
	struct tm stGetPosTime;
	struct tm stSetRtcTime, stSetRtcTime0;
	struct tm stSetPosTime;
	char tmp[512] = {0};
	unsigned int unTime1 = 0,unTime2 = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	if(cls_show_msg1(5,"��������Ҫ��rootȨ���½���,[Enter]����,��������" )!=ENTER && auto_flag!=1)
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI);
		return;
	}
	//cls_show_msg("���ڿ���̨��������echo  -n  ""  > /mnt/hwinfo/.delta_time�����Ӳ��ʱ�����������");  //����NDK�ӿ�����ʱ��������Ӳ��ʱ���,������Է��㶨λ
	//������ؿ���̨����:
	//date -s "2017-12-13 15:35:50"  ����ϵͳ���ʱ�䣬����̨����
	//hwclock  -w                    ��ϵͳ���ʱ��д��rtcʱ��
	//hwclock  -r                    �鿴rtcʱ��

	//case1: ȡPOS��ǰӲ�������ʱ��
	if((ret=NDK_SysGetRtcTime(&stOldRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
		if((ret=NDK_SysGetPosTime(&stOldPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//Ӧ����Ҫ��������Ӳʱ����ȵĲ���ǰ�ã����㶨λ 20171213 sull
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = stOldPosTime.tm_year;
	stSetRtcTime.tm_mon = stOldPosTime.tm_mon;
	stSetRtcTime.tm_mday = stOldPosTime.tm_mday;
	stSetRtcTime.tm_hour = stOldPosTime.tm_hour;
	stSetRtcTime.tm_min = stOldPosTime.tm_min;
	stSetRtcTime.tm_sec = stOldPosTime.tm_sec;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2: ������һ��ʱ�䣬�����ø�ʱ��,����ȡ�Ƿ�ڶ������õ� 
	//����RTCʱ��2010.10.10 10:10:10
	memset(&stSetRtcTime0, 0, sizeof(stSetRtcTime0));
	stSetRtcTime0.tm_year = 2010 - 1900;
	stSetRtcTime0.tm_mon = 10 - 1;
	stSetRtcTime0.tm_mday = 10;
	stSetRtcTime0.tm_hour = 10;
	stSetRtcTime0.tm_min = 10;
	stSetRtcTime0.tm_sec = 10;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����RTCʱ��2011.11.11 11:11:11
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stGetRtcTime, 0, sizeof(stGetRtcTime));
	NDK_SysGetRtcTime(&stGetRtcTime);
	if(stSetRtcTime.tm_year != stGetRtcTime.tm_year 
		|| stSetRtcTime.tm_mon != stGetRtcTime.tm_mon
		|| stSetRtcTime.tm_mday != stGetRtcTime.tm_mday
		|| stSetRtcTime.tm_hour != stGetRtcTime.tm_hour
		|| stSetRtcTime.tm_min != stGetRtcTime.tm_min
		/*|| stSetRtcTime.tm_sec != stGetRtcTime.tm_sec*/)
	{
		send_result("line %d:%s����ʧ��(stGetRtcTime = %04d/%02d/%02d %02d:%02d:%02d)", __LINE__, TESTAPI, 
			stGetRtcTime.tm_year+1900, stGetRtcTime.tm_mon+1, stGetRtcTime.tm_mday,stGetRtcTime.tm_hour, stGetRtcTime.tm_min, stGetRtcTime.tm_sec);
		GOTOERR;
	}

	//case3:�������꣬2012.2.29
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2012 - 1900;
	stSetRtcTime.tm_mon = 2 - 1;
	stSetRtcTime.tm_mday = 29;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stGetRtcTime, 0, sizeof(stGetRtcTime));
	NDK_SysGetRtcTime(&stGetRtcTime);
	if(stSetRtcTime.tm_year != stGetRtcTime.tm_year 
		|| stSetRtcTime.tm_mon != stGetRtcTime.tm_mon
		|| stSetRtcTime.tm_mday != stGetRtcTime.tm_mday
		|| stSetRtcTime.tm_hour != stGetRtcTime.tm_hour
		|| stSetRtcTime.tm_min != stGetRtcTime.tm_min
		/*|| stSetRtcTime.tm_sec != stGetRtcTime.tm_sec*/)
	{
		send_result("line %d:%s����ʧ��(stGetRtcTime = %04d/%02d/%02d %02d:%02d:%02d)", __LINE__, TESTAPI,
			stGetRtcTime.tm_year+1900, stGetRtcTime.tm_mon+1, stGetRtcTime.tm_mday,stGetRtcTime.tm_hour, stGetRtcTime.tm_min, stGetRtcTime.tm_sec);
		GOTOERR;
	}

	//case4:�쳣���ԣ��������
	if((ret=NDK_SysGetRtcTime(NULL)) != NDK_ERR/*Ŀǰ�������Ϊ�ĳ�NDK_ERR,����NDK_ERR_PARA*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5: �쳣���ԣ�������������1899
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 1899 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6: �쳣���ԣ�����������·�13
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 13 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7: �쳣���ԣ�����������·�0
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 0 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8: �쳣���ԣ����������2011.02.29
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 2 - 1;
	stSetRtcTime.tm_mday = 29;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:�쳣���ԣ���������2011.11.00
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 00;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:�쳣���ԣ���������2011.10.32
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 10 - 1;
	stSetRtcTime.tm_mday = 32;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case11: �쳣���ԣ����������2011.11.31
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 31;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case12: �쳣���ԣ����������Сʱ24
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 24;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case13: �쳣���ԣ��������������60
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 61;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case14: �쳣���ԣ��������������60
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 60;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case15:�쳣���ԣ��������������2��30
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 1904 - 1900;
	stSetRtcTime.tm_mon = 2 - 1;
	stSetRtcTime.tm_mday = 30;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 35;
	stSetRtcTime.tm_sec = 00;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case16:�쳣�������������������Ϊ����
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = -11;
	stSetRtcTime.tm_hour = -11;
	stSetRtcTime.tm_min = -11;
	stSetRtcTime.tm_sec = -11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case17:�������ʱ�䣬Ӳ��ʱ�䲻Ӧ���Ÿı�
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2011 - 1900;
	stSetRtcTime.tm_mon = 11 - 1;
	stSetRtcTime.tm_mday = 11;
	stSetRtcTime.tm_hour = 11;
	stSetRtcTime.tm_min = 11;
	stSetRtcTime.tm_sec = 11;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2012 - 1900;
	stSetPosTime.tm_mon = 12 - 1;
	stSetPosTime.tm_mday = 12;
	stSetPosTime.tm_hour = 12;
	stSetPosTime.tm_min = 12;
	stSetPosTime.tm_sec = 12;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stGetRtcTime, 0, sizeof(stGetRtcTime));
	NDK_SysGetRtcTime(&stGetRtcTime);
	memset(&stGetPosTime, 0, sizeof(stGetPosTime));
	NDK_SysGetPosTime(&stGetPosTime);
	if(stSetRtcTime.tm_year != stGetRtcTime.tm_year 
		|| stSetRtcTime.tm_mon != stGetRtcTime.tm_mon
		|| stSetRtcTime.tm_mday != stGetRtcTime.tm_mday 
		|| stSetRtcTime.tm_hour != stGetRtcTime.tm_hour
		|| stSetRtcTime.tm_min != stGetRtcTime.tm_min )
	{
		send_result("line %d:%s����ʧ��(stGetRtcTime = %04d/%02d/%02d %02d:%02d:%02d",__LINE__, TESTAPI,
			stGetRtcTime.tm_year+1900, stGetRtcTime.tm_mon+1, stGetRtcTime.tm_mday, stGetRtcTime.tm_hour, stGetRtcTime.tm_min, stGetRtcTime.tm_sec);
		GOTOERR;
	}

	if(stSetPosTime.tm_year != stGetPosTime.tm_year 
		|| stSetPosTime.tm_mon != stGetPosTime.tm_mon
		|| stSetPosTime.tm_mday != stGetPosTime.tm_mday 
		|| stSetPosTime.tm_hour != stGetPosTime.tm_hour
		|| stSetPosTime.tm_min != stGetPosTime.tm_min)
	{
		send_result("line %d:%s����ʧ��(stGetPosTime = %04d/%02d/%02d %02d:%02d:%02d)",__LINE__, TESTAPI,
			stGetPosTime.tm_year+1900, stGetPosTime.tm_mon+1, stGetPosTime.tm_mday, stGetPosTime.tm_hour, stGetPosTime.tm_min, stGetPosTime.tm_sec);
		GOTOERR;
	}

	//case18:����Ӳ��ʱ�䣬���ʱ��Ӧ�ø��Ÿı�
	//case18:������Ϊ������Ӳ��ʱ�䣬���ʱ�䲻Ӧ�ø��Ÿı�  20171213 sull
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2010 - 1900;
	stSetRtcTime.tm_mon = 10 - 1;
	stSetRtcTime.tm_mday = 10;
	stSetRtcTime.tm_hour = 10;
	stSetRtcTime.tm_min = 10;
	stSetRtcTime.tm_sec = 10;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stGetRtcTime, 0, sizeof(stGetRtcTime));
	NDK_SysGetRtcTime(&stGetRtcTime);
	memset(&stGetPosTime, 0, sizeof(stGetPosTime));
	NDK_SysGetPosTime(&stGetPosTime);
	if(stGetRtcTime.tm_year == stGetPosTime.tm_year 
		&& stGetRtcTime.tm_mon == stGetPosTime.tm_mon
		&& stGetRtcTime.tm_mday == stGetPosTime.tm_mday 
		&& stGetRtcTime.tm_hour == stGetPosTime.tm_hour
		&& stGetRtcTime.tm_min == stGetPosTime.tm_min)
	{
		send_result("line %d:%s����ʧ��(stGetRtcTime = %04d/%02d/%02d %02d:%02d:%02d, stGetPosTime = %04d/%02d/%02d %02d:%02d:%02d)",__LINE__, TESTAPI,
			stGetRtcTime.tm_year+1900, stGetRtcTime.tm_mon+1, stGetRtcTime.tm_mday, stGetRtcTime.tm_hour, stGetRtcTime.tm_min, stGetRtcTime.tm_sec,
			stGetPosTime.tm_year+1900, stGetPosTime.tm_mon+1, stGetPosTime.tm_mday, stGetPosTime.tm_hour, stGetPosTime.tm_min, stGetPosTime.tm_sec);
		GOTOERR;
	}
			
	//case19:ͬʱ������Ӳʱ��,�۲����ֵ�Ƿ񱣳��ȶ�
	memset(&stGetRtcTime, 0, sizeof(stGetRtcTime));
	memset(&stGetPosTime, 0, sizeof(stGetPosTime));
	NDK_SysGetRtcTime(&stGetRtcTime);
	NDK_SysGetPosTime(&stGetPosTime);
	unTime1 = abs(stGetRtcTime.tm_sec - stGetPosTime.tm_sec);
	sleep(60);
	memset(&stGetRtcTime, 0, sizeof(stGetRtcTime));
	memset(&stGetPosTime, 0, sizeof(stGetPosTime));
	NDK_SysGetRtcTime(&stGetRtcTime);
	NDK_SysGetPosTime(&stGetPosTime);
	unTime2 = abs(stGetRtcTime.tm_sec - stGetPosTime.tm_sec);
	if((ret = abs(unTime1 - unTime2)) != 0)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__, TESTAPI, ret);
	}
if(auto_flag==0 || auto_flag==2)
{
	//case20:ʱ�������Բ���,��֤ʱ���ڱ仯�����в���ȱ������
	//���磺
	//XX��59��֮��Ӧ����XX+1��00��,����01��;
	//59��59��֮��Ӧ����00��00�룬����01��01��
	cls_show_msg("����USB�߽���������������,��AccessPort���,���������");
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2013 - 1900;
	stSetRtcTime.tm_mon = 5 - 1;
	stSetRtcTime.tm_mday = 7;
	stSetRtcTime.tm_hour = 10;
	stSetRtcTime.tm_min = 58;
	stSetRtcTime.tm_sec = 57;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PortClose(PORT_NUM_USB);
	NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1");
	
	cls_show_msg("��������AccessPort,���������");
	cls_printf("�۲�AccessPort�������");

	for(i=0; i< 100; i++)
	{
		ret=NDK_SysGetRtcTime(&stGetRtcTime);
		sprintf(tmp, "stGetRtcTime=%04d/%02d/%02d %02d:%02d:%02d \n", stGetRtcTime.tm_year+1900,stGetRtcTime.tm_mon+1,\
				stGetRtcTime.tm_mday,stGetRtcTime.tm_hour,stGetRtcTime.tm_min,stGetRtcTime.tm_sec);
		NDK_PortWrite(PORT_NUM_USB, strlen(tmp), tmp);
		sleep(1);
	}

	NDK_PortClose(PORT_NUM_USB);

	if (g_com!=INVALID_COM)
	{
		NDK_PortOpen(g_com, "115200,8,N,1");
	}
		
	if(cls_show_msg("���ʱ���Ƿ�ȱ������,[ȷ��]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
}
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);	
	//NDK_SysSetPosTime(stOldPosTime);//���û�ԭ����ʱ��
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	//return;
	goto ERR;
ERR:
	//������
	NDK_SysSetRtcTime(stOldRtcTime);//���û�ԭ����ʱ�䣬�˴����ڲ���������Ҫִ��Լ3���ӣ���˻��������ǰ��ϵͳʱ����Լ3���ӣ�����BUG
	NDK_SysSetPosTime(stOldPosTime);
	return;
}

