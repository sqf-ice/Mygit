/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			:  
* version			: 
* DATE				: 	
* directory 			: 
* description		: ���� NDK_IccSetPowerUpMode�Ƿ��ܹ����������ϵ�����,N900 N910����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_IccSetPowerUpMode"

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
void icc7(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	//����Ϊ�ֲ�����
	int ret=0, pnAtrlen=0, i = 0;
	uchar buf[256]={0};
	uchar sendbuf[]={'\x00', '\x84', '\x00', '\x00', 8};
	
	//����Ϊ������
	cls_printf("%s������...", TESTAPI);

	//����ǰ��
	icpowerdown_all();

	//case1:�쳣��������
	if((ret=NDK_IccSetPowerUpMode(ICC_EMV_MODE-1,1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccSetPowerUpMode(ICC_ISO7816_DEFAULT_MODE+1,1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccSetPowerUpMode(ICC_EMV_MODE,ICC_VOL_OFF))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccSetPowerUpMode(ICC_EMV_MODE,ICC_VOL_1P8V+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�쳣����:ʹ���籣������,���÷��籣��ģʽ,Ӧ���ϵ�ʧ��
	cls_show_msg("������籣����,���������...");
	if((ret=NDK_IccSetPowerUpMode(ICC_BPS_384_MODE,ICC_VOL_3V))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_ERR_IOCTL)
	{	
		send_result("line %d:IC���ϵ�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}

	//case3:�쳣����:ũ�����п�ʹ��3V�ĵ�ѹ,����1.8v ,�ϵ�Ӧ��ʧ��
	cls_show_msg("������ѹ��֧��1.8V��ũ�����п���,���������...");
	if((ret=NDK_IccSetPowerUpMode(ICC_EMV_MODE,ICC_VOL_1P8V))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_ERR_IOCTL)
	{	
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}

	//case3:����IC����������:EMVģ�� 3V��ѹ(��ͬ���п�ʹ�õĵ�ѹ���ܲ�ͬ)
	cls_show_msg("������ѹ֧��3V�Ľ������п���,���������...");
	if((ret=NDK_IccSetPowerUpMode(ICC_EMV_MODE,ICC_VOL_3V))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_OK)
	{	
		send_result("line %d:IC���ϵ�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	//IC��д
	memset(buf,0,sizeof(buf));
	pnAtrlen = 0;
	if((ret=NDK_Iccrw(ICTYPE_IC,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
	{		
		send_result("line %d:IC������ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	//���п���֧��ȡ�����,��ͬ�Ŀ�����ֵ��ͬ,���ܷ���6d00���ܷ���6e00
	if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)//&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6e00
	{
		send_result("line %d:IC��ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
		GOTOERR;
	}	
	//IC�µ�
	if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{				
		send_result("line %d:IC���µ�ʧ��(%d)", __LINE__,  ret);
		goto ERR;
	}
	send_result("%s����ͨ��", TESTAPI);		
ERR://���Ժ���	
	icpowerdown_all();
	return;
}

