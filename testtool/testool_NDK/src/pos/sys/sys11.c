/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys11.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysGetPowerVol�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGetPowerVol"

#define	MAXWAITTIME 30

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
*			  		      linwei	        20130121	 	created
*****************************************************************/
void sys11(void)
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
	unsigned int unVol = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//case1: �쳣���ԣ��������
	if((ret=NDK_SysGetPowerVol(NULL))!= NDK_ERR_PARA)//Ӧ�÷��ش���
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if( cls_show_msg("�Ƿ�֧��ʹ�õ��?\n[ȷ��]��[����]��") == ENTER)
	{
#if BATTERY_GETOUT_ENABLE
		//case3: �в��Դ��ȡ��ص�ѹ
		cls_show_msg("ȷ���Ѳ��Դ��δװ���,���������ʼ");
		unVol = 0;
#if defined ME31 //��Ӳ����Աȷ��ME31 ���ŵ�Դ�Ƿ�������ѹֵ20150604 modify 
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol == 0)
#else
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol != 0)
#endif
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,unVol);
			RETURN;
		}
#endif

		//case4: �в��Դ�������е�أ�ȡ��ص�ѹ//�ĵ������� "����в��Դ��Ϊ0" ��� "ֻ���Դ��Ϊ0" linwl20131216 NDK 4.4.3
		cls_show_msg("ȷ���Ѳ��Դ��װ�е��,���������ʼ");
		unVol = 0;
//#if  !(defined ME31)//ME30���ŵ�Դ��Ҳ�е�ط��ص�unVol��Ϊ0
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol == 0)//��Ӧ���޸�unVol != 0 �� unVol == 0 linwl20131216
//#else  //ME31���ŵ�Դ���е�ط��ص�unVol��Ϊ0  modify 20150604
//		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol != 0)//��Ӧ���޸�unVol != 0 �� unVol == 0 linwl20131216
//#endif
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,unVol);
			RETURN;
		}

#if (defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME66)//ME30 ME11����usb ���е�����λΪ1
		//case6: ���Դ,���δ���ڳ��
		//cls_show_msg("ȷ���Ѳ��Դ��װ�е��,���������ʼ");
		unVol = 0;
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || ((unVol >> 31)!=1))
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,unVol);
			RETURN;
		}
#endif

		//case5: û���Դ��ȡ��ص�ѹ(���е��)
		//me11��Ҫ�����ֻ���Ƶ��,Ȼ��ʹ��appʹ����ʹ�õ�ض���ʹ��usb����
		cls_show_msg("ȷ��δ���Դ��װ�е��,���������ʼ");
		unVol = 0;
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol == 0)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,unVol);
			RETURN;
		}
	}
	else
	{
		//case2: �в��Դ���޵��
		unVol = 0;
#if defined ME66 //me66���λ����USB���룬����1�������λΪ1����Ϊ0������һ���ܴ����  20180816  modify
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK)
#else
  		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol != 0)
#endif
		{
			send_result("line %d:%s����ʧ��(%d,%u", __LINE__, TESTAPI, ret,unVol);
			RETURN;
		}
		send_result("%s����ͨ��", TESTAPI);
		return; //��֧�ֵ�������������
	}

#if BATTERY_GETOUT_ENABLE //��ؿ��ڲ��ػ�����²�ж�Ĳ������²���
	//case3:����������֤�ܹ���ȷ��ʶ��3�鲻ͬ��ѹ�ĵ��  20140702��֣��ȷ�ϸú������ص�ֵԼ�ǵ�ѹ��10�� linwl
	if(cls_show_msg("�������Ĳ�����Ҫ3�鲻ͬ��ѹ�ĵ��,������ESC")!=ESC)
	{
		while(1)
		{
			if(i++)
				cls_show_msg("�����ӵ�Դ��,�������,�ٰѵ�Դ�ε�,���������");
			else
				cls_show_msg("�������POS���ĵ�Դ��USB�ߵȹ����豸�Ƴ�,���������");
			
			unVol = 0;
			if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK || unVol == 0)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,unVol);
				RETURN;
			}
			cls_show_msg("���¼,�ÿ��ص�ѹΪ%d",unVol);
			if(i == 3)
				break;
		}
		if(cls_show_msg("�����ص�ѹ����,[ȷ��]�� [����]��")!=ENTER)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,unVol);
			RETURN;
		}
	}
#endif

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

