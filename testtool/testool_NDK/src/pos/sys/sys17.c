/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys17.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysGetConfigInfo�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGetConfigInfo"

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
*			  		        linwei	           20130121	 	created
*****************************************************************/
void sys17(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int nvalue,ret= -1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
#if !K21_ENABLE //K21ֻ֧��SYS_CONFIG_LANGUAGE
	//case1:�쳣���ԣ��������     
	if((ret=NDK_SysGetConfigInfo(SYS_CONFIG_SLEEP_ENABLE,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetConfigInfo(16,&nvalue)) != NDK_ERR_PARA)
	{//16����EM_SYS_CONFIGö�����
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��ȡϵͳ������Ϣ
#if SLEEP_ENABLE	
	//case2.1:��ȡ����ʹ��
	if((ret=NDK_SysGetConfigInfo(SYS_CONFIG_SLEEP_ENABLE, &nvalue))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("����ʹ��:%s,���ж��Ƿ���ȷ" ,nvalue==0?"��ֹ":(nvalue == 1?"����":"δ֪"));
	
	//case2.2:��ȡ��������ʱ��ǰ����ʱ��
	if((ret=NDK_SysGetConfigInfo(SYS_CONFIG_SLEEP_TIME, &nvalue))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("��������ʱ��ǰ����ʱ��:%d,���ж��Ƿ���ȷ",nvalue);
	
	//case2.3:��ȡ����ģʽ
	if((ret=NDK_SysGetConfigInfo(SYS_CONFIG_SLEEP_MODE, &nvalue))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("����ģʽ:%s,���ж��Ƿ���ȷ" ,nvalue==1?"ǳ����":(nvalue == 2?"������":"δ֪"));
	
#endif	
#endif

	//case2.4:��ȡ��ȡϵͳ����
	if((ret=NDK_SysGetConfigInfo(SYS_CONFIG_LANGUAGE, &nvalue))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if !K21_ENABLE//K21ƽ̨�ײ�0����Ӣ�� 1��������(������ƽ̨�෴)20140515 linwl
	send_result("��ȡϵͳ����:%s,���ж��Ƿ���ȷ",nvalue==0?"����":(nvalue == 1?"english":"δ֪"));
#else
	send_result("��ȡϵͳ����:%s,���ж��Ƿ���ȷ",nvalue==0?"english":(nvalue == 1?"����":"δ֪"));
#endif
	

#if !K21_ENABLE
	//case2.5:��ȡ�����Զ��������س���
	if((ret=NDK_SysGetConfigInfo(SYS_CONFIG_APP_AUTORUN, &nvalue))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	send_result("�����Զ��������س���:%s,���ж��Ƿ���ȷ",nvalue==0?"����":(nvalue == 1?"����":"δ֪"));
#endif

	//���Խ���
	send_result("%s����ͨ��(���޸����ò����زⱾ��)", TESTAPI);
	return;
}

