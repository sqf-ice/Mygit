/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec4.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecGetKcv�ӿڹ���
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
#define TESTAPI	"NDK_SecGetKcv"	

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
void sec4(void)
{
	int ret = -1, i = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
//	uchar *pZKcv=(uchar *)"\x82\xE1\x36\x65";   	//SEC_KCV_ZERO��ʽ:TMK��ԿΪ16��0x11����8���ֽ�0x00����DES���㣬�õ���ǰ�ĸ��ֽ�
	uchar *pVKcv=(uchar *)"\xD1\xE5\x07\x48";    	//SEC_KCV_VAL��ʽ:TMK��ԿΪ16��0x11����8���ֽ�\x12\x34x56\x78\x90\x12\x34\x56����DES���㣬�õ���ǰ�ĸ���
	uchar *errKcv=(uchar *)"\x26\x5A\x77\xBB";		//"\x26\x5A\x7A\xBF";(��ȷKCV)	// 2		//SEC_KCV_ZERO��ʽ(TMK id=2,����KEY=16��0x15)
	uchar *pZKcv[]={
				//DES�㷨��Ӧ��KCVֵ
				(uchar *)"\x82\xE1\x36\x65",		//SEC_KCV_ZERO��ʽ:TMK��ԿΪ16��0x11����8���ֽ�0x00����DES���㣬�õ���ǰ�ĸ��ֽ�
				//SM4�㷨��Ӧ��KCVֵ
				(uchar *)"\xF8\xD0\x68\x70",		//SEC_KCV_ZERO��ʽ:TMK��ԿΪ16��0x11����16���ֽ�0x00����SM4���㣬�õ���ǰ�ĸ��ֽ�
				//AES�㷨��Ӧ��KCVֵ
				(uchar *)"\xEE\x23\xD8\x1C"			//SEC_KCV_ZERO��ʽ:TMK��ԿΪ16��0x11������kcv���㣬�õ���ǰ�ĸ��ֽ�
					};
#if SEC_SM4_ENABLE  //DES+SM4
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};
#else  //DES
	int algnum = 1;
	int key_flag[2] = {0,0} ;
#endif
#if SEC_AES_ENABLE  //AES
	ST_EXTEND_KEYBLOCK stExtendKey;
	char KeyValue[16] = {0};
#endif

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 
	
	//����ǰ��
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
		return;
	}
	
	for(i = 0;i<algnum;i++)
	{	
		//case1:δ��װ��ԿӦ����ʧ��
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}
		
		//��װ��ԿTMK id=1
		stKeyInfo.ucScrKeyIdx=0;/*��ʾ���İ�װ*/	
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.nDstKeyLen=16;
		memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}
		
		//case2:��ȡKCV(ģʽSEC_KCV_ZERO)Ӧ������ȷ
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 1, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#if OVERSEAS_ENABLE||defined N910P  //����汾ʹ��3�ֽ�KCV��֤ ���ݹ����ʼ� 20180116 modify by jiangym 
		if(memcmp(stKcvInfoIn.sCheckBuf, pZKcv[0+i], 3))//pZKcv
#else
		if(memcmp(stKcvInfoIn.sCheckBuf, pZKcv[0+i], 4))//pZKcv
#endif
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}

		//case8:������ȡidΪ2��kcvԤ��ʧ����ȥ��ȡidΪ1��kcvԤ�ڳɹ�(���ڵ�bug:��װ��id1��ȡid2��kcvԤ��ʧ���ٶ�ȡid1Ԥ�ڳɹ�ʵ�ʷ���ʧ��)20180413 add 
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 1, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}

		//case7:��ģʽSEC_KCV_VAL��ȡ��KCV,SM4�㷨��֧��Ӧ����ʧ��;DES�㷨֧��Ӧ���سɹ�
		stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		if(i==1)
		{
			
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 1, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
		else
		{
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 1, &stKcvInfoIn))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
#if OVERSEAS_ENABLE||defined N910P  //����汾ʹ��3�ֽ�KCV��֤ ���ݹ����ʼ� 20180116 modify by jiangym 
			if(memcmp(stKcvInfoIn.sCheckBuf, pVKcv, 3))
#else
			if(memcmp(stKcvInfoIn.sCheckBuf, pVKcv, 4))
#endif
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;
			}
		}
		
		//case4:��ȡKCV(ģʽSEC_KCV_NONE)Ӧ������ȷ
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 1, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		
		//case6:��SEC_KCV_ZERO��ʽ��װTMK(����),ID=3�����Դ����KCV����ȷӦ�ð�װʧ�ܣ����һ�ȡKCVҲӦ��ʧ�� 20140813 addedby linwl
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=3;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;	
		memset(stKeyInfo.sDstKeyValue, 0x15, stKeyInfo.nDstKeyLen);
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, errKcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_KCV_CHK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 3, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//cls_show_msg("0x%2x,0x%2x,0x%2x,0x%2x", stKcvInfoIn.sCheckBuf[0], stKcvInfoIn.sCheckBuf[1], stKcvInfoIn.sCheckBuf[2], stKcvInfoIn.sCheckBuf[3]);
	}
#if SEC_AES_ENABLE
	//AES��Կ��صĲ��Ե�����ȡ����,�������,�е���ֻ֧��DES��AES�� 20180808 modify
	if(flag_sdk3)	//sdk3.0�ŵ���AES  
	{
		//case1.1:δ��װ��ԿӦ����ʧ��
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		
		//��װ��ԿTMK id=1
		stKeyInfo.ucScrKeyIdx=0;/*��ʾ���İ�װ*/	
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memset(KeyValue, 0x11, stExtendKey.len);
		stExtendKey.pblock = KeyValue;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));		
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		
		//case2.1:��ȡKCV(ģʽSEC_KCV_ZERO)Ӧ������ȷ
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 1, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if OVERSEAS_ENABLE||defined N910P  //����汾ʹ��3�ֽ�KCV��֤ ���ݹ����ʼ� 20180116 modify by jiangym 
		if(memcmp(stKcvInfoIn.sCheckBuf, pZKcv[2], 3))//pZKcv
#else
		if(memcmp(stKcvInfoIn.sCheckBuf, pZKcv[2], 4))//pZKcv
#endif
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case8.1:������ȡidΪ2��kcvԤ��ʧ����ȥ��ȡidΪ1��kcvԤ�ڳɹ�(���ڵ�bug:��װ��id1��ȡid2��kcvԤ��ʧ���ٶ�ȡid1Ԥ�ڳɹ�ʵ�ʷ���ʧ��)20180413 add 
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 1, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case7.1:��ģʽSEC_KCV_VAL��ȡ��KCV,SM4�㷨��֧��Ӧ����ʧ��;DES�㷨֧��Ӧ���سɹ�
		stKcvInfoIn.nCheckMode=SEC_KCV_VAL;		
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 1, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		//case4.1:��ȡKCV(ģʽSEC_KCV_NONE)Ӧ������ȷ
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 1, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		//case6.1:��SEC_KCV_ZERO��ʽ��װTMK(����),ID=3�����Դ����KCV����ȷӦ�ð�װʧ�ܣ����һ�ȡKCVҲӦ��ʧ�� 20140813 addedby linwl
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=3;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memset(KeyValue, 0x15, stExtendKey.len);
		stExtendKey.pblock = KeyValue;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, errKcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_KCV_CHK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 3, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
#endif	
	
	//case5:����Ƿ�����
	if((ret=NDK_SecGetKcv(100, 1, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_TYPE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(flag_sdk3 && !g_seckeyowner)  //sdk3.0�����ڹ�����Կ��ʱ�尲ȫ������Կ��Ĭ�ϰ�װTLK��sdk2.0����װ  20180724  modify
	{
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TAK, 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	else
	{
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TLK, 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC )
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0	//101�ѺϷ�
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK, 101, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK, 1, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	stKcvInfoIn.nCheckMode=100;
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK, 1, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Ժ���:����������Կ
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_SecKeyErase();
	return;
}


