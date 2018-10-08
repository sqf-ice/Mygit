/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm10c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_WlSetSimSlotѡ��sim����NDK_WlGetSimSlot��ȡ��ǰ���õ�sim���ӿڹ���
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
#define TESTAPI	"NDK_WlSetSimSlot��NDK_WlGetSimSlot"
 #define	WLM_TIMEOUT	2000

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm10void)
* functional description 			:   
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			zhengry     20161207	created
*****************************************************************/
void wlm10(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = 0, ret1 = 0, ret2 = 0, sq = 0, i=0, nKeyin = 0;
	float time1=0, time2=0;
	EM_WLM_SIM_SLOT get_simslot;
	EM_WLM_STATUS emStatus = -1;
	
	/*process body*/
	//case2:�����쳣��NDK_WlSetSimSlot�е�simSlotΪSimSlot_1-1/SIM_SLOT_UNKNOW+1,Ӧ����NDK_ERR_PARA��
	if( ((ret=NDK_WlSetSimSlot(SIM_SLOT1-1)) != NDK_ERR_PARA) || 
		((ret1=NDK_WlSetSimSlot(SIM_SLOT_UNKNOW+1)) != NDK_ERR_PARA) ||
		((ret2=NDK_WlGetSimSlot(NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��ret=%d,ret1=%d, ret2=%d", __LINE__, TESTAPI, ret, ret1, ret2);
		RETURN;
	}
	
	//��װsim��1��δ��װsim��2ʱ
	if(cls_show_msg("�뿪����,δ���ù�SIM����,����SIM1�忨,SIM2δ�忨��,ִ�б�����,�˳�[ESC],��������ʼ����")!=ESC)
	{
		//case1:������δ����set������NDK_WlGetSimSlot��ȡӦ��ʧ��,�һ�ȡ���Ŀ���Ӧ����unknow
		//�����ĳ���δ����set��������ȡ���سɹ����һ�ȡ����ΪSIM1  20180424 sull modify
		//if((ret=NDK_WlGetSimSlot(&get_simslot)) != NDK_ERR || get_simslot !=SIM_SLOT_UNKNOW)
		if((ret=NDK_WlGetSimSlot(&get_simslot)) != NDK_OK || get_simslot !=SIM_SLOT1)
		{
			send_result("line %d:%s����ʧ��ret=%d, get_simslot=%d", __LINE__, TESTAPI, ret, get_simslot);
			RETURN;
		}
		//case3:�������û��ѡ����sim����ϵͳĬ������sim����1����Ϊ��ǰ����sim��,��ʼ��Ӧ�óɹ�
		if ((ret = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_OK||emStatus!=WLM_STATUS_OK)
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, emStatus);
			RETURN;
		}
		sleep(5);//������SIM��1��ģ�鸴λ����Ҫʱ�䣬����ȡ���ź�
		if((ret = NDK_WlModemGetSQ(&sq))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, sq);
			RETURN;
		}
		if( (nKeyin=cls_show_msg("sim1Ӧ�ܳɹ�ȡ���ź�,ȡ�����ź�Ϊ%d��ͨ��[ENTER],ʧ��[����]", sq)) != ENTER)
		{
			send_result("line %d: %s����ʧ��(nKeyin=0x2%x, sq=%d)", __LINE__, TESTAPI, nKeyin, sq);
			RETURN;
		}
		
		//case4:NDK_WlSetSimSlot����SIM2������,NDK_WlGetSimSlot��ȡ��Ϊsim2��,������SIM2δ�忨��ʼ��Ԥ�ڷ����޿�,˵���л��ɹ���
		if((ret=NDK_WlSetSimSlot(SIM_SLOT2)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_WlGetSimSlot(&get_simslot)) != NDK_OK || get_simslot != SIM_SLOT2)
		{
			send_result("line %d:%s����ʧ��ret=%d, get_simslot=%d", __LINE__, TESTAPI, ret, get_simslot);
			RETURN;
		}
		if ((ret = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_ERR_NO_SIMCARD)
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, emStatus);
			RETURN;
		}
		send_result("����������ͨ��");
	}
	
	//case5:�ػ���ȡ��sim��1ʱ������sim��2ʱ��NDK_WlSetSimSlot����SIM2��������NDK_WlGetSimSlot��ȡ��Ϊsim2������ȡ�ź�Ӧ�ܳɹ���
	if(cls_show_msg("��ػ�,ȡ��SIM1�����ϵ�sim��������װ��SIM2�����Ϻ�,ִ�б�����,�˳�[ESC],��������ʼ����")!=ESC)
	{
		if((ret=NDK_WlSetSimSlot(SIM_SLOT2)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_WlGetSimSlot(&get_simslot)) != NDK_OK || get_simslot != SIM_SLOT2)
		{
			send_result("line %d:%s����ʧ��ret=%d, get_simslot=%d", __LINE__, TESTAPI, ret, get_simslot);
			RETURN;
		}
		if ((ret = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_OK||emStatus!=WLM_STATUS_OK)
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, emStatus);
			RETURN;
		}
		
		//case6:NDK_WlSetSimSlot����SIM1������,NDK_WlGetSimSlot��ȡ��Ϊsim1��,������SIM1δ�忨��ʼ��Ԥ�ڷ����޿�,˵���л��ɹ���
		if((ret=NDK_WlSetSimSlot(SIM_SLOT1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_WlGetSimSlot(&get_simslot)) != NDK_OK || get_simslot != SIM_SLOT1)
		{
			send_result("line %d:%s����ʧ��ret=%d, get_simslot=%d", __LINE__, TESTAPI, ret, get_simslot);
			RETURN;
		}
		if ((ret = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_ERR_NO_SIMCARD)
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, emStatus);
			RETURN;
		}
		send_result("����������ͨ��");
	}
	
	//case6:�ػ�������sim��1ʱ������sim��2ʱ��NDK_WlSetSimSlot����SIM1��������NDK_WlGetSimSlot��ȡ��Ϊsim��1������ȡ�ź�Ӧ�ܳɹ�
	//NDK_WlSetSimSlot����SIM2��������NDK_WlGetSimSlot��ȡ��Ϊsim2������ȡ�ź�Ӧ�ܳɹ���
	if(cls_show_msg("��ػ�,�ڽ��������۶�����SIM����,ִ�б�����,�˳�[ESC],��������ʼ����")!=ESC)
	{
		for(i=SIM_SLOT1;i<SIM_SLOT_UNKNOW;i++)
		{
			cls_printf("���ڽ���sim%d����",i+1);
			if(i==1)
				lib_StartStopwatch();
			if((ret=NDK_WlSetSimSlot(i)) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if(i==1)
				time1=lib_ReadStopwatch();
			if((ret=NDK_WlGetSimSlot(&get_simslot)) != NDK_OK || get_simslot != i)
			{
				send_result("line %d:%s����ʧ��ret=%d, get_simslot=%d", __LINE__, TESTAPI, ret, get_simslot);
				RETURN;
			}			
			if ((ret = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_OK||emStatus!=WLM_STATUS_OK)
			{
				send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, emStatus);
				RETURN;
			}
		}
		
		//case8:��������ͬһ������,Ԥ�ڵ�2�����õ�ʱ����Ҫ�ȽϿ�ķ���
		lib_StartStopwatch();
		if((ret=NDK_WlSetSimSlot(SIM_SLOT2)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			RETURN;
		}	
		time2=lib_ReadStopwatch();
		if(time2 > time1)
		{
			send_result("line %d :%s����ʧ��time1=%f, time2=%f", __LINE__, TESTAPI, time1, time2);
			RETURN;
		}
		send_result("����������ͨ��");
	}

#if 0	
	//case7:�����쳣����,�ڲ�֧��˫sim��Ӳ���Ļ����ϣ�����set sim2����
	if(cls_show_msg("��ȡ��֧��˫sim��Ӳ���Ļ����ϼ������ԣ��˳�[ESC],��������ʼ����")!=ESC)
	{
		if((ret=NDK_WlSetSimSlot(SIM_SLOT2)) != NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
			RETURN;
		}
		send_result("����������ͨ��");
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
	return;
}

