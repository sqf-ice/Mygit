/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm2.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_WlModemGetSQ�ӿڹ���
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
#define TESTAPI	"NDK_WlModemGetSQ"	
#define	MAXWAITTIME	30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm4(void)
* functional description 	: ��ȡ����MODEM�ź�ǿ�ȣ�ȡ�����ź�ǿ�ȣ�ȡ����ֵ 0-31 Ϊ�ɹ���99 Ϊδ֪,-1 Ϊʧ�� 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void wlm2(void)
{
	/*private & local definition*/
	int nRet = 0, sq = 0;
	LinkType type = getlinktype();

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�쳣����
	if((nRet = NDK_WlModemGetSQ(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}	

	//case2:��������
	if((nRet = NDK_WlModemGetSQ(&sq))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet, sq);
		RETURN;
	}
	if(((type==GPRS||type==CDMA) && (sq>SQ_2G_MAX||sq<SQ_2G_MIN))
	||(type==TD && (sq>SQ_3G_MAX||sq<SQ_3G_MIN)))
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, sq);
		RETURN;
	}
	else
	{
		nRet = show_msg1(MAXWAITTIME, "ȡ���ź�%d,��ȷ��ȷ��,����������", sq);
		if(nRet==0||nRet==ENTER)
		{
			send_result("%s����ͨ��(�ź�ǿ��:%d)", TESTAPI, sq);
			RETURN;
		}
		else
		{
			send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
			return;
		}
	}
}

