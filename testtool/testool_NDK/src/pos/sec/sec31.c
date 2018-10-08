/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name		: sec31.c
* Author 		: 
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_SecGetDrySR��ȡ��ȫ�Ĵ���ֵ��NDK_SecClear�尲ȫ�Ĵ���ֵ����
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
#define TESTAPI	"NDK_SecGetDrySR,NDK_SecClear"	

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
*			  	  	      jiangym		20160603  created  
*****************************************************************/
void sec31(void)
{
	/*private & local definition*/
	int ret = -1, val = 0;
#if ANDIROD_ENABLE//defined N850
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
#endif
	
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	
	if(cls_show_msg("�Ƿ��ǲ������������,�ǰ�[Enter],��[����]")!=ENTER)
	{
		//case1:�����쳣����
		if((ret = NDK_SecGetDrySR(NULL)) != NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//case2:δ��ȫ������ȡ��ȫ�Ĵ�����ֵΪ0
		if(((ret = NDK_SecGetDrySR(&val)) != NDK_OK) || val != 0)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, val);
			RETURN;
		}

		cls_show_msg("�ػ������в��,�������װ��,ʹ�ð�ȫ�������ٽ��������");
	}
	else
	{
		//case3:��Ϊ��ȫ����(�������װ��)��ȡ��ȫ�Ĵ���ֵ��Ϊ0,֮���尲ȫ�Ĵ���Ӧ�óɹ�,ͬʱ��ȡ��ֵӦ��Ϊ0
		if(((ret = NDK_SecGetDrySR(&val)) != NDK_OK) || val == 0)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, val);
			RETURN;
		}
		if((ret = NDK_SecClear()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(((ret = NDK_SecGetDrySR(&val)) != NDK_OK) || val != 0)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, val);
			RETURN;
		}
#if !defined N910P		 //N910P���߱��˹���
		if(flag_sdk3)//sdk3.0�尲ȫ������Կ��Ĭ�ϰ�װTLK��sdk2.0����װ  20180724  modify
		{
			//case4:�尲ȫ������Կ���Ĭ�ϰ�װTLK  20180620 sull add 
			if((ret=NDK_SecSetKeyOwner("*"))!=NDK_OK)
			{
				send_result("line %d:������Կ����ʧ��(%d)", __LINE__, ret);
				GOTOERR;
			}
			//��SEC_KCV_ZERO��ʽ��װTMK(����),ID=2,����16��0x15
			stKeyInfo.ucScrKeyIdx=1;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
			stKeyInfo.ucDstKeyIdx=2;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_DES;
			stKeyInfo.nDstKeyLen=16;	
			memcpy(stKeyInfo.sDstKeyValue, "\xCB\x62\x65\x94\x48\xAC\x8A\x72\xCB\x62\x65\x94\x48\xAC\x8A\x72", stKeyInfo.nDstKeyLen);
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
			stKcvInfoIn.nLen=4;
			memcpy(stKcvInfoIn.sCheckBuf, "\x26\x5A\x7A\xBF", stKcvInfoIn.nLen);	
			if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_DES, 2, &stKcvInfoIn))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if(memcmp(stKcvInfoIn.sCheckBuf, "\x26\x5A\x7A\xBF", 4)) 
			{
				send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			}
		}
#endif
	}
//#endif
		send_result("%s����ͨ��", TESTAPI); 
	if(flag_sdk3)
	{
ERR:
		if(g_seckeyowner)
			NDK_SecSetKeyOwner("");
		else
			NDK_SecSetKeyOwner("*");
	}
	return;
}
