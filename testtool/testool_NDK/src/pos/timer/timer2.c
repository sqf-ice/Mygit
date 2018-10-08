/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: timerģ��
* file name		: timer2.c
* Author 			: huangbs
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_WaitAndFlag��NDK_CancelTimer�ӿڹ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_WaitAndFlag,NDK_CancelTimer"
//#define MAXTIMEOUT 20

/****************************************************************
* function name 	 	: void timer2(void)
* functional description 	: ��ʱ��ʶλ��λ�Լ�ȡ����ʱ����
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  huangbs     20120906	created
*****************************************************************/
void timer2(void)
{
	uint TmrFlag1=0, TmrFlag2=0;
	NDK_HANDLE tmr1=NULL;
	NDK_HANDLE tmr2=NULL;
	int cnt=0;
	time_t oldtime;

	cls_printf("����%s...", TESTAPI); 

	//case1 :����Ƿ�����
	if((tmr1=NDK_WaitAndFlag(ONESECOND*5, NULL))!=NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	NDK_CancelTimer(NULL);       

	//case 3: ��ע��һ��tmr1,ʱ�䵽ʱ��ע��tmr2
	if((tmr1=NDK_WaitAndFlag(ONESECOND*3, &TmrFlag1))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	oldtime=time(NULL);
	while(1)
	{
		// Check for timer expiration
		if(TmrFlag1)
		{
			NDK_CancelTimer(tmr1);
			//ע��tmr2
			if((tmr2=NDK_WaitAndFlag(ONESECOND*5, &TmrFlag2))==NULL)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				return;
			}
			break;
		}
		//��ʱ����ѭ��
		if((time(NULL)-oldtime)>5)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
	}
	oldtime=time(NULL);
	while(1)
	{
		if(TmrFlag2)
		{
			NDK_CancelTimer(tmr2);
			break;
		}
		//��ʱ����ѭ��
		if((time(NULL)-oldtime)>8)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
	}

	//case 4:��NDK_WaitAndFlag��,TmrFlag1Ӧ��0.ʱ��δ��ǰ,������NDK_CancelTime.���ͷź��ٴ�cancel��Ӧ�޸�����
	oldtime=time(NULL);
	if((tmr1=NDK_WaitAndFlag(ONESECOND*10, &TmrFlag1))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if((time(NULL)-oldtime)>2)//�������Ļ�,���û�����ʲôʱ��
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}	
	if(TmrFlag1!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, TmrFlag1);
		RETURN;
	}
	oldtime=time(NULL);
	while(1)
	{
		// Check for timer expiration
		if(TmrFlag1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
		if(cnt++==3)
		{
			NDK_CancelTimer(tmr1);
			break;
		}
		//��ʱ����ѭ��
		if((time(NULL)-oldtime)>12)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
	}	
	NDK_CancelTimer(tmr1);

	//case 5: ͬʱע��2��tmr. ����,tmr1����ʱ��Ϊ3s,tmr2����ʱ��Ϊ20s,tmr1Ӧ�ȵ���,���ں�ȡ��tmr2,���ڱ�־Ӧδ����
	if((tmr1=NDK_WaitAndFlag(ONESECOND*3, &TmrFlag1))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if((tmr2=NDK_WaitAndFlag(ONESECOND*20, &TmrFlag2))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	oldtime=time(NULL);
	while(1)
	{
		// Check for timer expiration
		if(TmrFlag1)
		{
			NDK_CancelTimer(tmr1);
			NDK_CancelTimer(tmr2);
			break;
		}
		if(TmrFlag2)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
		//��ʱ����ѭ��
		if((time(NULL)-oldtime)>22)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return;
		}
	}
	if(TmrFlag2)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}	

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

