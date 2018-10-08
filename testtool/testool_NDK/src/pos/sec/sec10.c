/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec3.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecSetIntervaltime�ӿڹ���
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
#define TESTAPI	"NDK_SecSetIntervaltime"	

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
void sec10(void)
{
	int ret = 0, i = 0, j =0;
	uchar *szPan=(uchar *)"6225885916163157";
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar szDataIn[9]={0},szMac[9]={0},szPinOut[8+1]={0};
	
	cls_show_msg1(2, "����%s...", TESTAPI); 

#if SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};
	uchar macmode = SEC_MAC_SM4;
	uchar pinmode = SEC_PIN_SM4_1;
#else
	int algnum = 1;
	int key_flag[2] = {0,0};
	uchar macmode = SEC_MAC_X99;
	uchar pinmode = SEC_PIN_ISO9564_0;
#endif

	for(j = 0;j<algnum;j++)
	{
		//����ǰ��
		if((ret=NDK_SecKeyErase())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}

		//��װTAK, ID=1
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;
		memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//��װTPK, ID=2
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;
		memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		
		//�������μ���MAC֮����С���ʱ��
		if(auto_flag==0 || auto_flag==1)
		{
			//case1:unTAKIntervalTimeMs=60000,����60���ִ��1��,240����ֻ��ִ��4��,����ѭ��5�ε���NDK_SecGetMac,ǰ�Ĵγɹ�,�����ʧ��
			memset(szDataIn, 0x11, 8);
			if((ret=NDK_SecSetIntervaltime(10, 60000))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			for(i=0; i<4; i++)
			{
				if((ret=NDK_SecGetMac(1, szDataIn, 8, szMac, macmode))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
					GOTOERR;
				}
			}

			if((ret=NDK_SecGetMac(1, szDataIn, 8, szMac, macmode)) != NDK_ERR_SECP_OVERRUN)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			
			//case2:unTAKIntervalTimeMs=1,MAC���������ó�1ms,����ѭ��10�ζ�Ӧ�ɹ�
			if((ret=NDK_SecSetIntervaltime(10, 1))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			for(i=0; i<10; i++)
			{
				if((ret=NDK_SecGetMac(1, szDataIn, 8, szMac, macmode))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
					GOTOERR;
				}
			}

			//case3:unTAKIntervalTimeMs=0xFFFFFFFF,��ʾ���ı�ԭֵ,����ѭ��10�ζ�Ӧ�ɹ�
			if((ret=NDK_SecSetIntervaltime(10, 0xFFFFFFFF))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			for(i=0; i<10; i++)
			{
				if((ret=NDK_SecGetMac(1, szDataIn, 8, szMac, macmode))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
					GOTOERR;
				}
			}
		}

		//�������μ���PINBlock֮����С���ʱ��
		if(auto_flag==0 || auto_flag==2)
		{
			//case4:PINBLOCK���ʱ��ļ��㷽ʽ�� Ĭ��Ϊ120����ֻ�ܵ���4��,��TPKIntervalTimeMsĬ��ֵΪ30��,���øú����������ú�,����Ϊ4*TPKIntervalTimeMsʱ����ֻ�ܵ���4�� 
			//unTPKIntervalTimeMs=0��ʾʹ��Ĭ��ֵ,����ѭ��5�ε���NDK_SecGetPin,ÿ�ζ�ֱ�ӻس�,ǰ�Ĵγɹ�,�����ʧ��
			cls_printf("��������[ȷ��]...");
			if((ret=NDK_SecSetIntervaltime(0, 100))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}

			for(i=0; i<4; i++)
			{
				if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, pinmode, 2*1000))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
					GOTOERR;
				}
			}
			if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, pinmode, 2*1000))!= NDK_ERR_SECP_OVERRUN)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			}
		}
		else
			send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
		
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecSetIntervaltime(0,0);
	NDK_SecKeyErase();
	return;
}

