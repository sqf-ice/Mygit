/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys22.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����NDK_SysGetPowerInfo�Ƿ���ȷ��ȡ��Դ��Ϣ
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGetPowerInfo"

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
*			  		     
*****************************************************************/
void sys22(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
#if BATTERY_GETOUT_ENABLE
	int i = 0;
#endif
	int ret = -1;
	ST_POWER_INFO power_info, power_info1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�����쳣����:ΪNULL ����NDK_ERR_PARA   
	if((ret=NDK_SysGetPowerInfo(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if( cls_show_msg("�Ƿ�֧��ʹ�õ��?\n[ȷ��]��[����]��") == ENTER)
	{
		//�е�����
		if(cls_show_msg("�ò�������ֱ�׼����/δ������ĵ��,������ESC")==ESC)
		{
			send_result("line %d:%s������ֹ(��׼���õ�����²���)", __LINE__, TESTAPI);
			RETURN;
		}
		
		//case2:���Դ���,������Ĳ��ڳ��,δ��������ڳ��,�ٷֱ�Ϊ0,��Դ����Ϊ3��6
		if(cls_show_msg("ȷ���Ѳ���ӵ�Դ��װ�г�����ĵ��,���������ʼ����,[ȡ��]����������")!=ESC)
		{
			memset(&power_info,0,sizeof(power_info));
			if((ret=NDK_SysGetPowerInfo(&power_info))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((power_info.unIsCharging!=0)||(power_info.unBattryPercent!=0)||((power_info.unPowerType!=3)&&(power_info.unPowerType!=6)))
			{
				send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, power_info.unIsCharging, power_info.unBattryPercent, power_info.unPowerType);
				RETURN;
			}
			if(cls_show_msg("��ȡ������ӵ�Դ����Ϊ:%s,[ȷ��]��ͨ��,������ʧ��",(power_info.unPowerType==3)?"������":"USB����")!=ENTER)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, power_info.unPowerType);
				RETURN;
			}
		}
		else
			send_result("line %d:�û�ȡ��(���+������)��������", __LINE__);
		
		if(cls_show_msg("ȷ���Ѳ���ӵ�Դ��װδ������ĵ��,���������ʼ����,[ȡ��]����������")!=ESC)
		{
			if((ret=NDK_SysGetPowerInfo(&power_info))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((power_info.unIsCharging!=1)||(power_info.unBattryPercent!=0)||((power_info.unPowerType!=3)&&(power_info.unPowerType!=6)))
			{
				send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, power_info.unIsCharging, power_info.unBattryPercent, power_info.unPowerType);
				RETURN;
			}
			if(cls_show_msg("��ȡ������ӵ�Դ����Ϊ:%s,[ȷ��]��ͨ��,������ʧ��",(power_info.unPowerType==3)?"������":"USB����")!=ENTER)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, power_info.unPowerType);
				RETURN;
			}
		}
		else
			send_result("line %d:�û�ȡ��(���+��������)��������", __LINE__);
		
		//case3:�ε���Դ���:���ڳ��,�ٷֱ���0-100��,��Դ����Ϊ2
		if(cls_show_msg("ȷ��������ӵ�Դ��װ�е��,���������ʼ����,[ȡ��]����������")!=ESC)
		{
			memset(&power_info,0,sizeof(power_info));
			if((ret=NDK_SysGetPowerInfo(&power_info))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if((power_info.unIsCharging!=0)||((power_info.unBattryPercent<=0||power_info.unBattryPercent>100))||(power_info.unPowerType!=2))
			{
				send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, power_info.unIsCharging, power_info.unBattryPercent, power_info.unPowerType);
				RETURN;
			}
#if BATTERY_GETOUT_ENABLE//��ؿ��ڲ��ػ�����²�ж�Ĳ������²���
			//case4:��ͬ������ػ�ȡ�����ٷֱȵĲ���
			if(cls_show_msg("�������Ĳ�����Ҫ3�鲻ͬ�����ĵ��,������ESC")!=ESC)
			{
				while(1)
				{
					if(i++)
						cls_show_msg("�����ӵ�Դ��,�������,�ٰѵ�Դ�ε�,���������");
					else
						cls_show_msg("�������POS���ĵ�Դ��USB�ߵȹ����豸�Ƴ�,���������");
					memset(&power_info,0,sizeof(power_info));
					if((ret=NDK_SysGetPowerInfo(&power_info))!=NDK_OK)
					{
						send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
						RETURN;
					}
					cls_show_msg("���¼,�ÿ��ص����ٷֱ�Ϊ%d", power_info.unBattryPercent);
					if(i == 3)
						break;
				}
				if(cls_show_msg("�����ص����ٷֱȺ���,[ȷ��]�� [����]��")!=ENTER)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					RETURN;
				}
			}
#endif	
		}
		else
			send_result("line %d:�û�ȡ��(�������+���)��������", __LINE__);
		
		//case5:�������仯����
		if(cls_show_msg("�����нϳ�ʱ��ĳ�����,������ESC")!=ESC)
		{
			cls_show_msg("ȷ��װ��һ�����������ĵ��,�����������");
			memset(&power_info,0,sizeof(power_info));
			if((ret=NDK_SysGetPowerInfo(&power_info))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			cls_show_msg1(300,"������Դ���ȴ�5����");
			cls_show_msg("������������������");
			if((ret=NDK_SysGetPowerInfo(&power_info1))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
			if(power_info1.unBattryPercent<=power_info.unBattryPercent)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, power_info.unBattryPercent, power_info1.unBattryPercent);
				RETURN;
			}
		}
		else
			send_result("line %d:�û�ȡ��(��ʱ����)��������", __LINE__);
	}
	
#if BATTERY_GETOUT_ENABLE//��ؿ��ڲ��ػ�����²�ж�Ĳ������²���
	//case6:û�е�����:���ڳ��,�ٷֱ�Ϊ0,��Դ����Ϊ1��4
	cls_show_msg("ȷ���Ѳ���ӵ�Դ����û��װ���,�����������");
	memset(&power_info,0,sizeof(power_info));
	if((ret=NDK_SysGetPowerInfo(&power_info))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((power_info.unIsCharging!=0)||(power_info.unBattryPercent!=0)||((power_info.unPowerType!=1)&&(power_info.unPowerType!=4)))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, power_info.unIsCharging, power_info.unBattryPercent, power_info.unPowerType);
		RETURN;
	}
	if(cls_show_msg("��ȡ������ӵ�Դ����Ϊ:%s,[ȷ��]��ͨ��,������ʧ��",(power_info.unPowerType==1)?"������":"USB����")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, power_info.unPowerType);
		RETURN;
	}
#endif

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

