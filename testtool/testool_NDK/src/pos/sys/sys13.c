/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys13.c
* Author 			: linw
* version			: 
* DATE				: 20130109
* directory 		: 
* description		: ��������NDK_SysGetPosTime,NDK_SysSetPosTime�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetPosTime,NDK_SysSetPosTime"

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
*			  	  	      linw          20130109	created
*****************************************************************/
void sys13(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, i = 0;
	struct tm stOldPosTime;
	struct tm stGetPosTime;
	struct tm stSetPosTime, stSetPosTime0;
	char tmp[512] = {0};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	if(cls_show_msg1(5,"��������Ҫ��rootȨ���½���,[Enter]����,��������" )!=ENTER && auto_flag!=1)
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI);
		return;
	}

	//case1: ȡPOS��ǰʱ��
	if((ret=NDK_SysGetPosTime(&stOldPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2: ������һ��ʱ�䣬�����ø�ʱ��,����ȡ�Ƿ�ڶ������õ� 20140603 added by jiangym
	//����POSʱ��2010.10.10 10:10:10
	memset(&stSetPosTime0, 0, sizeof(stSetPosTime0));
	stSetPosTime0.tm_year = 2010 - 1900;
	stSetPosTime0.tm_mon = 10 - 1;
	stSetPosTime0.tm_mday = 10;
	stSetPosTime0.tm_hour = 10;
	stSetPosTime0.tm_min = 10;
	stSetPosTime0.tm_sec = 10;
	if((ret=NDK_SysSetPosTime(stSetPosTime0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����POSʱ��2011.11.11 11:11:11
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stGetPosTime, 0, sizeof(stGetPosTime));
	NDK_SysGetPosTime(&stGetPosTime);
	if(stSetPosTime.tm_year != stGetPosTime.tm_year 
		|| stSetPosTime.tm_mon != stGetPosTime.tm_mon
		|| stSetPosTime.tm_mday != stGetPosTime.tm_mday
		|| stSetPosTime.tm_hour != stGetPosTime.tm_hour
		|| stSetPosTime.tm_min != stGetPosTime.tm_min
		/*|| stSetPosTime.tm_sec != stGetPosTime.tm_sec*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�������꣬2012.2.29
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2012 - 1900;
	stSetPosTime.tm_mon = 2 - 1;
	stSetPosTime.tm_mday = 29;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(&stGetPosTime, 0, sizeof(stGetPosTime));
	NDK_SysGetPosTime(&stGetPosTime);
	if(stSetPosTime.tm_year != stGetPosTime.tm_year 
		|| stSetPosTime.tm_mon != stGetPosTime.tm_mon
		|| stSetPosTime.tm_mday != stGetPosTime.tm_mday
		|| stSetPosTime.tm_hour != stGetPosTime.tm_hour
		|| stSetPosTime.tm_min != stGetPosTime.tm_min
		/*|| stSetPosTime.tm_sec != stGetPosTime.tm_sec*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:�쳣���ԣ��������
	if((ret=NDK_SysGetPosTime(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5: �쳣���ԣ�������������1899
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 1899 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6: �쳣���ԣ�����������·�13
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 13 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7: �쳣���ԣ�����������·�0
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 0 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8: �쳣���ԣ����������2011.02.29
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 2 - 1;
	stSetPosTime.tm_mday = 29;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:�쳣���ԣ���������2011.11.00
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 00;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:�쳣���ԣ���������2011.10.32
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 10 - 1;
	stSetPosTime.tm_mday = 32;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case11: �쳣���ԣ����������2011.11.31
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 31;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case12: �쳣���ԣ����������Сʱ24
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 24;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case13: �쳣���ԣ��������������60
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 61;
	stSetPosTime.tm_sec = 11;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case14: �쳣���ԣ��������������60
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2011 - 1900;
	stSetPosTime.tm_mon = 11 - 1;
	stSetPosTime.tm_mday = 11;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 11;
	stSetPosTime.tm_sec = 60;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case15:�쳣���ԣ��������������2��30
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 1904 - 1900;
	stSetPosTime.tm_mon = 2 - 1;
	stSetPosTime.tm_mday = 30;
	stSetPosTime.tm_hour = 11;
	stSetPosTime.tm_min = 35;
	stSetPosTime.tm_sec = 00;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

if(auto_flag==0 || auto_flag==2)
{
	//case16:ʱ�������Բ���,��֤ʱ���ڱ仯�����в���ȱ������
	//���磺
	//XX��59��֮��Ӧ����XX+1��00��,����01��;
	//59��59��֮��Ӧ����00��00�룬����01��01��
#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG||defined ME50S||defined ME50NS)//K21û�д��ڣ��ʽ����Խ����USB��������
	cls_show_msg("���������ʼʱ�������Բ���,��鿴����1���������ʱ���Ƿ��ȱ������");
#elif ANDIROD_ENABLE
	cls_printf("ʱ�������Բ���,��鿴���������ʱ���Ƿ��ȱ������");
#else
	cls_show_msg("����USB�߽���������������,��AccessPort���,���������");
#endif
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2013 - 1900;
	stSetPosTime.tm_mon = 5 - 1;
	stSetPosTime.tm_mday = 7;
	stSetPosTime.tm_hour = 10;
	stSetPosTime.tm_min = 58;
	stSetPosTime.tm_sec = 57;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG||defined ME50S||defined ME50NS)||defined IM81||defined ME20//�д���1
	NDK_PortClose(PORT_NUM_COM1);
	NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1");
#elif ANDIROD_ENABLE
	//NDK_AZPortClose(PORT_NUM_COM2);
	//NDK_AZPortOpen(PORT_NUM_COM2, "115200,8,N,1");
#elif defined ME15C//defined N900||
	NDK_PortClose(PORT_NUM_COM2);
	NDK_PortOpen(PORT_NUM_COM2, "115200,8,N,1");
#else //����USB��
	NDK_PortClose(PORT_NUM_USB);
	NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1");
	
	cls_show_msg("��������AccessPort,���������");
	cls_printf("�۲�AccessPort�������");
#endif
	for(i=0; i< 100; i++)
	{	
		ret=NDK_SysGetPosTime(&stGetPosTime);
		/*fprintf(stderr," stGetPosTime=%d/%d/%d %d:%d:%d \n", stGetPosTime.tm_year+1900,stGetPosTime.tm_mon+1,\
			stGetPosTime.tm_mday,stGetPosTime.tm_hour,stGetPosTime.tm_min,stGetPosTime.tm_sec);*/
		//K21ƽ̨��֧�ֿ���̨,�޸���Ϣ�Ӵ������ 20130831 linwl
		sprintf(tmp, "stGetPosTime=%04d/%02d/%02d %02d:%02d:%02d \n", stGetPosTime.tm_year+1900,stGetPosTime.tm_mon+1,\
				stGetPosTime.tm_mday,stGetPosTime.tm_hour,stGetPosTime.tm_min,stGetPosTime.tm_sec);
#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG||defined ME50S||defined ME50NS)||defined IM81||defined ME20
		NDK_PortWrite(PORT_NUM_COM1, strlen(tmp), tmp);
#elif ANDIROD_ENABLE
		//NDK_AZPortWrite(PORT_NUM_COM2, strlen(tmp), tmp);
		cls_printf("%s",tmp); //ֱ����pos���������
#elif defined ME15C//defined N900||
		NDK_PortWrite(PORT_NUM_COM2, strlen(tmp), tmp);
#else
		NDK_PortWrite(PORT_NUM_USB, strlen(tmp), tmp);
#endif
		sleep(1);
	}

#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG||defined ME50S||defined ME50NS)||defined IM81||defined ME20
	NDK_PortClose(PORT_NUM_COM1);
#elif 0//ANDIROD_ENABLE
	NDK_AZPortClose(PORT_NUM_COM2);
#elif defined ME15C//defined N900||
	NDK_PortClose(PORT_NUM_COM2);
#else
	NDK_PortClose(PORT_NUM_USB);
#endif

	if (g_com!=INVALID_COM)
	{
#if 0//ANDIROD_ENABLE
		NDK_AZPortOpen(g_com, "115200,8,N,1");
#else
		NDK_PortOpen(g_com, "115200,8,N,1");
#endif
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
	NDK_SysSetPosTime(stOldPosTime);//���û�ԭ����ʱ�䣬�˴����ڲ���������Ҫִ��Լ3���ӣ���˻��������ǰ��ϵͳʱ����Լ3���ӣ�����BUG
	return;
}

