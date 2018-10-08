/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			:  
* version			: 
* DATE				: 	
* directory 			: 
* description		: ���� NDK_IccGetProtocol�Ƿ��ܹ�������ȡ��ʹ�õĴ���Э��,N900 N910����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_IccGetProtocol"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern void icpowerdown_all(void);

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: icc7
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	 		jiangym		20170905    created
*				  	 
*****************************************************************/
void icc8(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	//����Ϊ�ֲ�����
	int ret = 0, pnAtrlen = 0;
	uchar buf[256] = {0};
	int protocol = 0;
	
	//����Ϊ������
	cls_printf("%s������...", TESTAPI);

	//����ǰ��
	icpowerdown_all();

	//case1:�쳣��������
	if((ret=NDK_IccGetProtocol(ICTYPE_IC-1,&protocol))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccGetProtocol(ICTYPE_M_1_64+1,&protocol))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//������ܷɿ��� ������,������������޷�����
	if((ret=NDK_IccGetProtocol(ICTYPE_IC,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��������:��ȡIC���Ĵ���Э��,��ȡ��ֵ������Ϊ0
	cls_show_msg("�����IC����,���������...");
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_OK)
	{	
		send_result("line %d:IC���ϵ�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	if((ret=NDK_IccGetProtocol(ICTYPE_IC,&protocol))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(protocol)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, protocol);
		GOTOERR;
	}
	//IC�µ�
	if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{				
		send_result("line %d:IC���µ�ʧ��(%d)", __LINE__,  ret);
		GOTOERR;
	}

	//case:�������� ����Э�鷵��1�����Ŀǰ�޿�,���Ȳ���,�����п�������������

	//case3:�쳣����:ʹ��memory������,����ʧ��
	cls_show_msg("��ȷ���Ѳ���at24c���͵�memory��,���������...");
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_M_1,buf,&pnAtrlen))!=NDK_OK)
	{	
		send_result("line %d:IC���ϵ�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	if((ret=NDK_IccGetProtocol(ICTYPE_M_1,&protocol))!=/*NDK_ERR_IOCTL*/NDK_ERR_PARA)//����˵����IC ����SAM��������-6
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//IC�µ�
	if ((ret=NDK_IccPowerDown(ICTYPE_M_1))!=NDK_OK)
	{				
		send_result("line %d:IC���µ�ʧ��(%d)", __LINE__,  ret);
		goto ERR;
	}

	send_result("%s����ͨ��", TESTAPI);		
ERR://���Ժ���	
	icpowerdown_all();
	return;
}

