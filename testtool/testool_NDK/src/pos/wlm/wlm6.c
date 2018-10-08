/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm6.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_WlModemReset�ܷ�λ����ģ�飬NDK_WlModemClose�ܷ�ر�����ģ��
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
#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
#define TESTAPI	"NDK_WlModemReset,NDK_WlModemClose"	
#else
#define TESTAPI	"NDK_WlModemReset"	
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm6(void)
* functional description 	: �ر�����ģ��
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void wlm6(void)
{
	//����Ϊ�ֲ�����
	int nRet = 0,nSq=0;
	LinkType type = getlinktype();
	
	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	NDK_WlModemClose();

	//case1:�������߸�λ���ܹ�������ȡ�ź�
	if((nRet = NDK_WlModemReset())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	NDK_SysDelay(150/*100*/);//��λʱ��,��ԭ����100�޸ĳ�150,ԭ�� CDMAģ�鸴λ����ҪԼ11����ܻ�ȡ���ź�,ģ�鸴λ���� 20140212 zhengjd
	if((nRet = NDK_WlModemGetSQ(&nSq))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	if(((type==GPRS||type==CDMA) && (nSq>SQ_2G_MAX||nSq<SQ_2G_MIN))
	||(type==TD && (nSq>SQ_3G_MAX||nSq<SQ_3G_MIN)))
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nSq);
		GOTOERR;
	}
	cls_show_msg1(3, "��ȡ�����ź���:(%d)",nSq);

#if !(CPU5810X_ENABLE||defined ME50NX||defined ME68)
	//case2:���Թر����ߺ��ȡ�ź�Ӧ��ʧ��
	if((nRet = NDK_WlModemClose())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	if((nRet = NDK_WlModemGetSQ(&nSq))!=NDK_ERR)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
#endif

	//case3:�������¸�λ���ܹ�������ȡ�ź�
	if((nRet = NDK_WlModemReset())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	NDK_SysDelay(150/*100*/);//��λʱ��,��ԭ����100�޸ĳ�150,ԭ�� CDMAģ�鸴λ����ҪԼ11����ܻ�ȡ���ź�,ģ�鸴λ���� 20140212 zhengjd
	if((nRet = NDK_WlModemGetSQ(&nSq))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		GOTOERR;
	}
	if(((type==GPRS||type==CDMA) && (nSq>SQ_2G_MAX||nSq<SQ_2G_MIN))
	||(type==TD && (nSq>SQ_3G_MAX||nSq<SQ_3G_MIN)))
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nSq);
		goto ERR;
	}
	cls_show_msg1(3, "��ȡ�����ź���:(%d)",nSq);
	
	//���Խ���	
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_WlModemReset();
	return;
}
