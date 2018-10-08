/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: ���� NDK_IccPowerUp,NDK_IccPowerDown�Ƿ����µ�ɹ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_IccPowerUp,NDK_IccPowerDown"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern void icpowerdown_all(void);

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: icc4
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	 	 chenfm     20130107	created
*				  	 
*****************************************************************/
void icc4(void)
{
	//if(auto_flag==2)
	//	return; //����case����Ҫ�ֶ�����20171011

	//����Ϊ�ֲ�����
#include "icsam.h"

	int i=0, maxid=ICSAMNUM+1, rlen=0, pnAtrlen=0;
	int ret=0, ret1=0, ret2=0, ret3=0;
	uchar buf[300]={0};
	uchar sendbuf[]={'\x00', '\x84', '\x00', '\x00', 8};
	uchar req0[] = { 0x00,0xA4,0x00,0x00,0x02,0x00,0x11};
	uchar req1[] = { 0x80, 0x30, 0x00,0x00,0x80, 
				0x22,0x22, 0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				0x22,0x22,0x22,0x22,0x22, 0x22,0x22,0x22,0x22,
				 0x22,0x22,0x00};
	uchar req6100[] = { 0x80,0x30,0x00,0x80, 0x80, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22, 
				0x22,0x22,0x00};
	
	//����Ϊ������
	cls_printf("%s������...", TESTAPI);

	//����ǰ��
	icpowerdown_all();

	//case1:�쳣��������
	if((ret=NDK_IccPowerUp(-1, buf, &pnAtrlen))!=NDK_ERR_PARA/*NDK_ERR_OPEN_DEV*/
   	||(ret1=NDK_IccPowerUp(ICTYPE_IC, NULL, &pnAtrlen))!=NDK_ERR_PARA
   	||(ret2=NDK_IccPowerUp(ICTYPE_IC, buf, NULL))!=NDK_ERR_PARA
   	||(ret3=NDK_IccPowerDown(-1))!=NDK_ERR_PARA/*NDK_ERR_OPEN_DEV*/)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2, ret3);
		goto ERR;
	}

	//case2:IC/SAM����������
	cls_show_msg1(3, "�����Ӵ�ʽIC����SAM���۵�����SAM��,���������...");
	i=0;
	do
	{
		//case1:IC���ϵ����
		//case2:IC���µ����
		memset(buf, 0, sizeof(buf));
		if((ret=NDK_IccPowerUp(CardNo[i], buf, &pnAtrlen))!=NDK_OK||(ret1=NDK_IccPowerUp(CardNo[i], buf, &pnAtrlen))!=NDK_OK)//������
		{
			if(CardNo[i]!=0xff)
			{
				send_result("line %d:%s����ʧ��(%s:%d,%d)", __LINE__, TESTAPI, CardNoStr[i], ret, ret1);
				GOTOERR;
			}
		}
		else
		{
			//case3:�Ƿ��������ԣ��ϵ�Ӧʧ��
			if(CardNo[i]==0xff)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
		}
		if((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK||(ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)//������
		{
			if(CardNo[i]!=0xff)
			{
				send_result("line %d:%s����ʧ��(%s:%d,%d)", __LINE__, TESTAPI, CardNoStr[i], ret, ret1);
				GOTOERR;
			}
		}
		else//�µ��ѳɹ�
		{
			if(CardNo[i]==0xff)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
			else if((ret=NDK_Iccrw(CardNo[i], sizeof(sendbuf), sendbuf, &rlen, buf))!=NDK_ERR_IOCTL)//zhangnw201105add �µ���дӦʧ��
			{
				//NDK_IccPowerDown(CardNo[i]);//��һ��
				send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
				GOTOERR;
			}
		}
	}
	while(++i<=(maxid-1));
	
#if	IC1_ENABLE&&(SAM1_ENABLE||SAM2_ENABLE)//֧��SAM1���Ĳ�Ʒ�Ž��н������²���//��֧��IC���Ų��� 20180510
	//case 3:IC���ϵ��SAMҲ�ϵ磬����IC����д��Ȼ��SAM����д����(����SAM���������)��IC���µ�
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_OK)
	{	
		send_result("line %d:IC1�ϵ�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
	}
	//SAM������
	i=1;
	do
	{
		memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(CardNo[i],buf,&pnAtrlen))!=NDK_OK)
		{	
			send_result("line %d:%s�ϵ�ʧ��(%d)", __LINE__, CardNoStr[i], ret);
			GOTOERR;
		}

		//IC��д
		memset(buf,0,sizeof(buf));
		if((ret=NDK_Iccrw(ICTYPE_IC,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
		{		
			send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
			GOTOERR;															
		}
		if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
		{
			send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
			GOTOERR;								
		}	
		
		//sam��д
		memset(buf,0,sizeof(buf));
		if((ret=NDK_Iccrw(CardNo[i],sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
		{						
			send_result("line %d:%s����ʧ��(%d)", __LINE__, CardNoStr[i], ret);
			GOTOERR;															
		}
		if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
		{
			send_result("line %d:%sȡ�����ʧ��(SW:0x%04x)", __LINE__, CardNoStr[i], ret);
			GOTOERR;								
		}	
		
		//sam���µ�
		if ((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
		{				
			send_result("line %d:%s�µ�ʧ��(0x%x)", __LINE__, CardNoStr[i], ret);
			RETURN;
		}
		//sam����������
	}while(++i<(maxid-1));
	//IC�µ�
	if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{				
		send_result("line %d:%s�µ�ʧ��(0x%x)", __LINE__, CardNoStr[0], ret);
		RETURN;
	}

	//case 4:IC��SAM���ϵ磬IC����д���µ磬Ȼ��SAM����д���µ磻	
	i=1;
	do
	{
		memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_OK)
		{	
			send_result("line %d:IC1�ϵ�ʧ��(%d)", __LINE__, ret);
			GOTOERR;
		}
		memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(CardNo[i],buf,&pnAtrlen))!=NDK_OK)
		{	
			send_result("line %d:%s�ϵ�ʧ��(%d)", __LINE__,CardNoStr[i], ret);
			GOTOERR;
		}
		//IC��д
		memset(buf,0,sizeof(buf));
		pnAtrlen = 0;
		if((ret=NDK_Iccrw(ICTYPE_IC,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
		{		
			send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
			GOTOERR;															
		}
		if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
		{
			send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
			GOTOERR;								
		}	
		//IC�µ�
		if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{				
			send_result("line %d:%s�µ�ʧ��(%d)", __LINE__, CardNoStr[0], ret);
			RETURN;
		}
		//sam��д
		memset(buf,0,sizeof(buf));
		pnAtrlen = 0;
		if((ret=NDK_Iccrw(CardNo[i],sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
		{						
			send_result("line %d:%s����ʧ��(%d)", __LINE__,CardNoStr[i], ret);
			GOTOERR;															
		}
		if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
		{
			send_result("line %d:%sȡ�����ʧ��(SW:0x%04x)", __LINE__,CardNoStr[i],ret);
			GOTOERR;								
		}	
		//sam���µ�
		if ((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
		{				
			send_result("line %d:%s�µ�ʧ��(%d)", __LINE__, CardNoStr[i], ret);
			RETURN;
		}
		//sam����������
	}while(++i<(maxid-1));
	
	//case5:IC��SAM���ϵ磬SAM����д���µ磬Ȼ��IC����д���µ磻
	i=1;
	do
	{
		//ic���ϵ�
		memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&pnAtrlen))!=NDK_OK)
		{	
			send_result("line %d:IC1�ϵ�ʧ��(%d)", __LINE__, ret);
			GOTOERR;
		}
		//sam���ϵ�
		memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(CardNo[i],buf,&pnAtrlen))!=NDK_OK)
		{	
			send_result("line %d:%s�ϵ�ʧ��(%d)", __LINE__, CardNoStr[i], ret);
			GOTOERR;
		}
		//sam��д
		memset(buf,0,sizeof(buf));
		pnAtrlen = 0;
		if((ret=NDK_Iccrw(CardNo[i],sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
		{						
			send_result("line %d:%s����ʧ��(%d)", __LINE__, CardNoStr[i], ret);
			GOTOERR;
		}
		if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
		{
			send_result("line %d:%sȡ�����ʧ��(SW:0x%04x)", __LINE__, CardNoStr[i], ret);
			GOTOERR;
		}	
		//sam���µ�
		if ((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
		{				
			send_result("line %d:%s�µ�ʧ��(%d)", __LINE__, CardNoStr[i], ret);
			RETURN;
		}
		//IC��д
		memset(buf,0,sizeof(buf));
		pnAtrlen = 0;
		if((ret=NDK_Iccrw(ICTYPE_IC,sizeof(sendbuf),sendbuf,&pnAtrlen,buf))!=NDK_OK)
		{		
			send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
			GOTOERR;
		}
		if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
		{
			send_result("line %d:IC1ȡ�����ʧ��(SW:0x%04x)", __LINE__,ret);
			GOTOERR;
		}	
		//IC�µ�
		if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{				
			send_result("line %d:%s�µ�ʧ��(%d)", __LINE__, CardNoStr[0], ret);
			GOTOERR;
		}
	}while(++i<(maxid-1));
#endif
#if	SAM1_ENABLE
	if(auto_flag==0 || auto_flag==2)
	{
		if(cls_show_msg("�Ƿ����6100SAM������?��:[ȷ��],��:[����]")==ENTER)
		{
			//case6:���6100SAM����ִ��һϵ���ض���apduָ��Ӧ�ö��ܹ��ɹ�(ԭ����bug��ִ�����һ��ָ��᷵���쳣)20171011���ݸ߶˴�������
			cls_show_msg("�����6100��SAM�������������");
			//SAM�ϵ�
			memset(buf,0,sizeof(buf));
			if ((ret=NDK_IccPowerUp(ICTYPE_SAM1,buf,&pnAtrlen))!=NDK_OK)
			{	
				send_result("line %d:IC1�ϵ�ʧ��(%d)", __LINE__, ret);
				GOTOERR;
			}
			//SAM APDU  req0  ������У��
			memset(buf,0,sizeof(buf));
			pnAtrlen = 0;
			if((ret=NDK_Iccrw(ICTYPE_SAM1,sizeof(req0),req0,&pnAtrlen,buf))!=NDK_OK)
			{						
				send_result("line %d:SAM������ʧ��(%d)", __LINE__, ret);
				GOTOERR;
			}
			if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
			{
				send_result("line %d:SAM��ȡ�����ʧ��(SW:0x%04x)", __LINE__, ret);
				GOTOERR;
			}	
			//SAM APDU  req1  ������У��
			memset(buf,0,sizeof(buf));
			pnAtrlen = 0;
			if((ret=NDK_Iccrw(ICTYPE_SAM1,sizeof(req1),req1,&pnAtrlen,buf))!=NDK_OK)
			{						
				send_result("line %d:SAM������ʧ��(%d)", __LINE__, ret);
				GOTOERR;
			}
			if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
			{
				send_result("line %d:SAM��ȡ�����ʧ��(SW:0x%04x)", __LINE__, ret);
				GOTOERR;
			}	
			//SAM APDU  req6100  ������У��
			memset(buf,0,sizeof(buf));
			pnAtrlen = 0;
			if((ret=NDK_Iccrw(ICTYPE_SAM1,sizeof(req6100),req6100,&pnAtrlen,buf))!=NDK_OK)
			{						
				send_result("line %d:SAM������ʧ��(%d)", __LINE__, ret);
				GOTOERR;
			}
			if((ret=SMART_GETSW(buf+pnAtrlen-2))!=0x9000&&(ret=SMART_GETSW(buf+pnAtrlen-2))!=0x6d00)
			{
				send_result("line %d:SAM��ȡ�����ʧ��(SW:0x%04x)", __LINE__, ret);
				GOTOERR;
			}	
			//�µ�
			if ((ret=NDK_IccPowerDown(ICTYPE_SAM1))!=NDK_OK)
			{				
				send_result("line %d:SAM���µ�ʧ��(%d)", __LINE__,  ret);
				GOTOERR;
			}
		}
	}
#endif
	send_result("%s����ͨ��", TESTAPI);		
ERR://���Ժ���	
	icpowerdown_all();
	//NDK_IccSetType(ICTYPE_IC);//�ָ�Ĭ��
	return;
}

