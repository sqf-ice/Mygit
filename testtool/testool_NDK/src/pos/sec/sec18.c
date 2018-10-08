/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec6.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecGetDukptKsn,NDK_SecIncreaseDukptKsn�ӿڹ���
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
#define TESTAPI	"NDK_SecGetDukptKsn,NDK_SecIncreaseDukptKsn"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date		remarks
*			  	  	   linrq       20130228	created
*****************************************************************/
void sec18(void)
{
	int ret = 0, i = 0;
	uchar KsnIn[17]={0},KeyValueIn[17]={0},KsnOut[11]={0};
	ST_SEC_KCV_INFO KcvInfoIn;
#if !(defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B)//�޼����豸��֧����Կ���뺯��
	uchar PinBlockOut[9]={0};
#endif
		
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//���İ�װһ��DUKPT, ID=1, psKsnInΪ10���ֽ�0xff  /*Լ��:����KCV���Բ�У�鷽ʽ��װ*/
	memset(KsnIn, 0xff, 10);
	memset(KeyValueIn, 0x11, 16);
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadTIK(1, 0, 16, KeyValueIn, KsnIn, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
if(auto_flag==0 || auto_flag==1)
{
	//case1:�Ƿ�����
	//case1.1: NDK_SecGetDukptKsn: ucGroupIdx=2,101
	if ((ret=NDK_SecGetDukptKsn(2, KsnOut))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0 //101�Ϸ�
	if ((ret=NDK_SecGetDukptKsn(101, KsnOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.2:NDK_SecGetDukptKsn:psKsnOut =NULL
	if ((ret=NDK_SecGetDukptKsn(1, NULL))!= NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.3: NDK_SecIncreaseDukptKsn: ucGroupIdx=2,101
	if ((ret=NDK_SecIncreaseDukptKsn(2))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0 //101�Ϸ�
	if ((ret=NDK_SecIncreaseDukptKsn(101))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//case2:NDK_SecGetDukptKsn psKsnOut��21��bitӦΪ1(KSNֵ��1��ʼ������)
	if ((ret=NDK_SecGetDukptKsn(1, KsnOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(((KsnOut[7]&0x1f)!=0)||(KsnOut[8]!=0)||(KsnOut[9]!=0x01))
	{
		send_result("line %d:%s����ʧ��(0x%02x,0x%02x,0x%02x)", __LINE__, TESTAPI, KsnOut[7], KsnOut[8], KsnOut[9]);
		GOTOERR;
	}

	//case3:����NDK_SecIncreaseDukptKsn����,��NDK_SecGetDukptKsn, psKsnOut��21��bitֵӦΪ3
	memset(KsnOut, 0x00, sizeof(KsnOut));
	for(i=0; i<2; i++)
	{
		if ((ret=NDK_SecIncreaseDukptKsn(1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
	}
	if ((ret=NDK_SecGetDukptKsn(1, KsnOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(((KsnOut[7]&0x1f)!=0)||(KsnOut[8]!=0)||(KsnOut[9]!=0x03))
	{
		send_result("line %d:%s����ʧ��(0x%02x,0x%02x,0x%02x)", __LINE__, TESTAPI, KsnOut[7], KsnOut[8], KsnOut[9]);
		goto ERR;
	}
}

#if !(defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B)//�޼����豸��֧����Կ���뺯��
if(auto_flag==0 || auto_flag==2)
{
	//case4:��ִ֤��DUKPT����Կ����,	KSNӦ����(KSNֵӦΪ2). ������GETPINΪ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������1234��ȷ��...");
#else
	cls_printf("��������1234��ȷ��...");
#endif
	if((ret=NDK_SecGetPinDukpt(1,  (uchar *)"4,5,6",  (uchar *)"6225885916163157",  KsnOut,  PinBlockOut,  SEC_PIN_ISO9564_0, 120*1000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(0x%x)", __LINE__, TESTAPI, KsnOut);
		GOTOERR;
	}
	memset(KsnOut, 0x00, sizeof(KsnOut));
	if ((ret=NDK_SecGetDukptKsn(1, KsnOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(((KsnOut[7]&0x1f)!=0)||(KsnOut[8]!=0)||(KsnOut[9]!=0x03))
	{
		send_result("line %d:%s����ʧ��(0x%02x,0x%02x,0x%02x)", __LINE__, TESTAPI, KsnOut[7], KsnOut[8], KsnOut[9]);
		goto ERR;
	}
}
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
#endif

	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}

