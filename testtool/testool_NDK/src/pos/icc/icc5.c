/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: ���� NDK_IccDetect��ȡ��Ƭ״̬
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_IccDetect"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern void icpowerdown_all(void);

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: icc5
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	 	 chenfm     20130107	created
*				  	 
*****************************************************************/
void icc5(void)
{
	//����Ϊ�ֲ�����
	int pnSta= -1, ret= -1, sta = 0x00;
#if !K21_ENABLE||ANDIROD_ENABLE
	int pnAtrlen=0;
	uchar buf[200]={0};
	uchar sendbuf[]={'\x00', '\x84', '\x00', '\x00', 8};
#endif	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 	

	//����ǰ��
	icpowerdown_all();

	//case1:�쳣��������
	if((ret=NDK_IccDetect(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}			

#if IC1_ENABLE
	//case2:�Ѳ�IC��,��δ�ϵ�
	if(auto_flag==0||auto_flag==2)	//�ֶ�����ʱҪ��ʾ,�Զ������Ѿ����ſ�������ʾ
	{
		cls_show_msg("��ȷ��IC�Ѳ���,���������...");
	}
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=0x01)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
#endif

#if !K21_ENABLE//||ANDIROD_ENABLE/* 20140523 ������ȷ��K21 NDK_IccDetect����ֻ֧��ȡIC���忨״̬,����״̬��֧��*/
	//��׿��Ʒ������֧�ֿ�Ƭ��ͬ״̬��� 20180523 modify  ��׿��Ʒ�ֲ���֧�� 20180706 modify
#if IC1_ENABLE
	//case3:IC�ϵ�
	if((ret=NDK_IccPowerUp(ICTYPE_IC, buf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sta = sta | 0x03;
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=sta)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
#endif	

#if SAM1_ENABLE
	if((ret=NDK_IccPowerUp(ICTYPE_SAM1, buf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sta = sta | 0x10;
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=sta)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
	memset(buf,0,sizeof(buf));
	pnAtrlen = 0;
	if((ret=NDK_Iccrw(ICTYPE_SAM1,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
	{						
		send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
		GOTOERR;															
	}
	if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
	{
		send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
		GOTOERR;								
	}	
#endif

#if SAM2_ENABLE
	if((ret=NDK_IccPowerUp(ICTYPE_SAM2, buf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
	sta = sta | 0x20;
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=sta)
//#if SAM_POWERON_MEANWHILE
//    ||(pnSta&0xff)!=0x33)
//#else
//	||(pnSta&0xff)!=0x23)
//#endif
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
	memset(buf,0,sizeof(buf));
	pnAtrlen = 0;
	if((ret=NDK_Iccrw(ICTYPE_SAM2,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
	{						
		send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
		GOTOERR;															
	}
	if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
	{
		send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
		GOTOERR;								
	}
#endif
    
#if SAM3_ENABLE
	if((ret=NDK_IccPowerUp(ICTYPE_SAM3, buf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sta = sta | 0x40;
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=sta)
//#if SAM_POWERON_MEANWHILE	
//	||(pnSta&0xff)!=0x73)
//#else
//	||(pnSta&0xff)!=0x43)
//#endif	
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}	
	memset(buf,0,sizeof(buf));
	pnAtrlen = 0;
	if((ret=NDK_Iccrw(ICTYPE_SAM3,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
	{						
		send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
		GOTOERR;															
	}
	if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
	{
		send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
		GOTOERR;								
	}
#endif

#if SAM4_ENABLE
	if((ret=NDK_IccPowerUp(ICTYPE_SAM4, buf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sta = sta | 0x80;
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=sta)
//#if SAM_POWERON_MEANWHILE
//#if SAM3_ENABLE
//	||(pnSta&0xff)!=0xf3)
//#else
//	||(pnSta&0xff)!=0x73)
//#endif
//#else
//	||(pnSta&0xff)!=0x83)
//#endif	
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
	memset(buf,0,sizeof(buf));
	pnAtrlen = 0;
	if((ret=NDK_Iccrw(ICTYPE_SAM4,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
	{						
		send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
		GOTOERR;															
	}
	if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
	{
		send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
		GOTOERR;								
	}
#endif
#endif

	//case4:�µ����п�����ȡ�����µ�״̬��Ӧ����1��bit0��ʾIC���Ƿ��������bitλ��Ϊ0��		
	icpowerdown_all();
#if IC1_ENABLE
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=0x01)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}

	if(auto_flag==0||auto_flag==2)
	{
		//case5:�γ�IC������ȡ��״̬Ӧ����0��bit0ӦΪ0��
		cls_show_msg("��ȷ��IC���Ƴ�,���������...");
		if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=0x00)
		{
			send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
			goto ERR;
		}

		//���Ժ���
		cls_show_msg("��ȷ��IC�Ѳ���,���������...");
	}
	else
	{
		send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
	}
#endif
	send_result("%s����ͨ��", TESTAPI);
ERR:
	icpowerdown_all();
	return;
}

