/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys37.c
* Author 			: 
* version			: 
* DATE				: 
* directory 		: 
* description		: ��������NDK_SysGetTimeZone,NDK_SysSetTimeZone�ӿ�(�����Ʒ֧��)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetTimeZone,NDK_SysSetTimeZone"

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
*			  	  	       sull       20180110      created 
*****************************************************************/
void sys37(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	struct tm stOldRtcTime, stOldPosTime;
	struct tm stOldPosTime0,stOldRtcTime0;
	struct tm stGetRtcTime, stSetRtcTime;
	struct tm stGetPosTime, stSetPosTime;
	EM_SYS_TIMEZONE stOldTimeZone,stOldTimeZone0;
	EM_SYS_TIMEZONE stGetTimeZone;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	//�ڷ�rootȨ���£�NDK_SysGetTimeZone��NDK_SysSetTimeZone���ý��Ӧ����ȷ
	if(cls_show_msg1(5,"��������Ҫ��rootȨ���½���,[Enter]����,��������" )!=ENTER && auto_flag!=1)
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI); 
		return;
	}
	//����ǰ��:��ȡ��ǰʱ����Ӳʱ�����ʱ��
	if((ret = NDK_SysGetTimeZone(&stOldTimeZone)) != NDK_OK) 
	{
	 	send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SysGetRtcTime(&stOldRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SysGetPosTime(&stOldPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	
	//case1:�����Ƿ�
	if((ret = NDK_SysGetTimeZone(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_WEST_12-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_UNKNOW+1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�Ⱥ���������ʱ������ȡʱ�����۲��Ƿ�ڶ���ʱ��������Ч
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_WEST_6)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_ZERO)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret = NDK_SysGetTimeZone(&stGetTimeZone)) != NDK_OK) || (stGetTimeZone != SYS_TIMEZONE_ZERO))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, stGetTimeZone);
		GOTOERR;
	}
	
	//case3:������1��ʱ������ȡ��ʱ�䣬�۲��Ƿ��һ��Сʱ
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_EAST_5)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret = NDK_SysGetTimeZone(&stGetTimeZone)) != NDK_OK) || (stGetTimeZone != SYS_TIMEZONE_EAST_5))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, stGetTimeZone);
		GOTOERR;
	}
	if((ret = NDK_SysGetPosTime(&stOldPosTime0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_EAST_6)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret = NDK_SysGetTimeZone(&stGetTimeZone)) != NDK_OK) || (stGetTimeZone != SYS_TIMEZONE_EAST_6))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, stGetTimeZone);
		GOTOERR;
	}
	if((ret = NDK_SysGetPosTime(&stGetPosTime)) == NDK_OK)
	{
		if(stGetPosTime.tm_hour == 0)
		{
			if(stGetPosTime.tm_hour+24-stOldPosTime0.tm_hour != 1 ||
				stGetPosTime.tm_min != stOldPosTime0.tm_min ||
				(abs(stGetPosTime.tm_sec - stOldPosTime0.tm_sec))>1)
			{
				send_result("line %d:%s����ʧ��(%d),stOldPosTime0=%04d/%02d/%02d %02d:%02d:%02d, stGetPosTime=%04d/%02d/%02d %02d:%02d:%02d", __LINE__, TESTAPI, ret,
					stOldPosTime0.tm_year+1900, stOldPosTime0.tm_mon+1, stOldPosTime0.tm_mday, stOldPosTime0.tm_hour, stOldPosTime0.tm_min, stOldPosTime0.tm_sec,
					stGetPosTime.tm_year+1900, stGetPosTime.tm_mon+1, stGetPosTime.tm_mday, stGetPosTime.tm_hour, stGetPosTime.tm_min, stGetPosTime.tm_sec);
				GOTOERR;
			}
		}
		else
		{
			if(stGetPosTime.tm_hour - stOldPosTime0.tm_hour != 1 ||
				stGetPosTime.tm_min != stOldPosTime0.tm_min ||
				stGetPosTime.tm_sec != stOldPosTime0.tm_sec)
			{
				send_result("line %d:%s����ʧ��(%d),stOldPosTime0=%04d/%02d/%02d %02d:%02d:%02d, stGetPosTime=%04d/%02d/%02d %02d:%02d:%02d", __LINE__, TESTAPI, ret,
				stOldPosTime0.tm_year+1900, stOldPosTime0.tm_mon+1, stOldPosTime0.tm_mday, stOldPosTime0.tm_hour, stOldPosTime0.tm_min, stOldPosTime0.tm_sec,
				stGetPosTime.tm_year+1900, stGetPosTime.tm_mon+1, stGetPosTime.tm_mday, stGetPosTime.tm_hour, stGetPosTime.tm_min, stGetPosTime.tm_sec);
				GOTOERR;
			}
		}
	}
	else
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�ı�ʱ������ӦӰ��Ӳʱ��
	if((ret = NDK_SysGetRtcTime(&stOldRtcTime0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysSetTimeZone(SYS_TIMEZONE_WEST_12)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysGetTimeZone(&stGetTimeZone) != NDK_OK) || (stGetTimeZone != SYS_TIMEZONE_WEST_12))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysGetRtcTime(&stGetRtcTime)) != NDK_OK ||
		stGetRtcTime.tm_year != stOldRtcTime0.tm_year ||
		stGetRtcTime.tm_mon != stOldRtcTime0.tm_mon ||
		stGetRtcTime.tm_mday != stOldRtcTime0.tm_mday ||
		stGetRtcTime.tm_hour != stOldRtcTime0.tm_hour ||
		stGetRtcTime.tm_min != stOldRtcTime0.tm_min ||
		(abs(stGetRtcTime.tm_sec - stOldRtcTime0.tm_sec))>1)
	{
		send_result("line %d:%s����ʧ��(%d),stOldRtcTime0=%04d/%02d/%02d %02d:%02d:%02d, stGetRtcTime=%04d/%02d/%02d %02d:%02d:%02d", __LINE__, TESTAPI, ret,
		stOldRtcTime0.tm_year+1900, stOldRtcTime0.tm_mon+1, stOldRtcTime0.tm_mday, stOldRtcTime0.tm_hour, stOldRtcTime0.tm_min, stOldRtcTime0.tm_sec,
		stGetRtcTime.tm_year+1900, stGetRtcTime.tm_mon+1, stGetRtcTime.tm_mday, stGetRtcTime.tm_hour, stGetRtcTime.tm_min, stGetRtcTime.tm_sec);
		GOTOERR;
	}

	//case5:�ı�Ӳʱ�䣬��ӦӰ��ʱ��
	if((ret = NDK_SysGetTimeZone(&stOldTimeZone0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2010 - 1900;
	stSetRtcTime.tm_mon = 10 - 1;
	stSetRtcTime.tm_mday = 10;
	stSetRtcTime.tm_hour = 10;
	stSetRtcTime.tm_min = 10;
	stSetRtcTime.tm_sec = 10;
	if((ret = NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret = NDK_SysGetTimeZone(&stGetTimeZone)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if(stGetTimeZone != stOldTimeZone0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, stGetTimeZone, stOldTimeZone0);
		GOTOERR2;
	}

	//case6:�ı���ʱ�䣬��ӦӰ��ʱ��
	if((ret = NDK_SysGetTimeZone(&stOldTimeZone0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
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
		GOTOERR2;
	}
	if((ret = NDK_SysGetTimeZone(&stGetTimeZone)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if(stGetTimeZone != stOldTimeZone0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, stGetTimeZone, stOldTimeZone0);
		goto ERR2;
	}
	send_result("%s����ͨ��", TESTAPI);
	
ERR2:
	NDK_SysSetTimeZone(stOldTimeZone);
	NDK_SysSetRtcTime(stOldRtcTime);
	NDK_SysSetPosTime(stOldPosTime);
	return;
	
ERR:
	NDK_SysSetTimeZone(stOldTimeZone);
	return;

}

