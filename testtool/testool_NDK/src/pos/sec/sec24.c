/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name		: sec24.c
* Author 		:  jiangym
* version		: 
* DATE			:  
* directory 		: ����ͨ����ȫ���ÿ�������NDK_SecCalcDesDukpt�ӽ��ܺ���ʹ�ø���������Կ
* description		: ��ȫ����
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
#define		TESTAPI		"��ȫ����DUKPT�ӽ��ܺ���"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: �ṩ��ȫ�ȼ����������ȡ����,��ԿΨһ��,��Կר����,��Կ����ǿ�ȵ���֤
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author			date				remarks
*			  	  	  	jiangym 			20150306	      created
*****************************************************************/
void sec24(void)
{
	/*private & local definition*/
	int ret = 0;
	uint punCfgInfo;
	uchar DataIn[33]={0}, DataOut[33]={0}, KSN[11]={0}, KSN1[11]={0};
	ST_SEC_KCV_INFO KcvInfoIn;
	uchar DukptKeyValue[16]={0xD5,0xD4,0x4F,0xF7,0x20,0x68,0x3D,0x0D,0x70,0xBE,0x25,0x28,0x18,0xE2,0xA1,0x8A};//DUKPY��ʼ������Կ	
	memset(DataIn, 0x33, sizeof(DataIn));//��ʼ������

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI);

	//����ǰ��
	NDK_SecSetKeyOwner("");
	if(((ret=NDK_SecGetCfg(&punCfgInfo))!=NDK_OK)||(punCfgInfo!=0x06)) //Ĭ�ϰ�ȫ�ȼ�Ϊ0x06
	{
		send_result("line %d:%s����ʧ��(%d,%x)", __LINE__, TESTAPI, ret, punCfgInfo);
		return;
	}
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	
	if((ret=NDK_SecSetCfg(0x18))!=NDK_OK) //�޸İ�ȫ�ȼ���Ĭ��ֵ�෴ 0x18
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	//���İ�װһ��DUKPT, ID=6, KSNΪ10���ֽ�0x00 /*Լ��:����KCV���Բ�У�鷽ʽ��װ*/
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);//��ʼKSN
	if ((ret=NDK_SecLoadTIK(1, 0, 16, DukptKeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//DUKPT ��װ��ʼ��Կ��Ψһ��,��ͬһ��Կ����NDK_SecLoadTIK����ͬid��
	//case1:��֤��ԿΩһ�����Ʊ��رգ�ͬһ����Կֵ�ظ���װ����ͬID  
	
	//case2:��Կר�������ƹر�:����ʹ������Կ��PIN��Կ��MAC��Կ�����������ݵļӽ��ܲ���
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);
	if ((ret=NDK_SecLoadTIK(6, 0, 16, DukptKeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2.1:DUKPT������des��Կ��DES�ӽ���
	memset(DataOut, 0, sizeof(DataOut));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, NULL, 8, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if defined ME31S
	//ʹ��8�ֽ���ԿBFA9CA10BF791459(����Կ����������Կ6ef92795e5ed714dcdeb767b330ea9bd �����Լ��ܵõ�)��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����20180511 modify �򿪷������µİ�ȫ��
	if(memcmp(DataOut,"\x0C\x54\x7B\x0E\x8F\x24\x27\x5F", 8))
#else
	//ʹ��8�ֽ���Կ6ef92795e5ed714d(����Կ��16�ֽ�DUKPT��Կ��ǰ8�ֽ���Կ���)��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����
	if(memcmp(DataOut,"\xE4\x87\xC1\x16\xA5\x48\xB9\x2C", 8))
#endif
	{
		send_result("line %d:%s����ʧ��(0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7]);
		GOTOERR;
	}
	memset(DataOut, 0, sizeof(DataOut));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, NULL, 8, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if defined ME31S
	//ʹ��8�ֽ���ԿBFA9CA10BF791459(����Կ����������Կ6ef92795e5ed714dcdeb767b330ea9bd �����Լ��ܵõ�)��8�ֽ�����3333333333333333���н��ܺ�õ����
	if(memcmp(DataOut,"\x05\x26\x90\x36\x97\x17\x19\x01", 8))
#else
	//��Ԥ�ڽ��ʹ��8�ֽ���Կ6ef92795e5ed714d��8�ֽ�����3333333333333333�ڼӽ��ܹ���\PosDebug���߽��ܵó�1EB577120A087B59
	if(memcmp(DataOut,"\x1E\xB5\x77\x12\x0A\x08\x7B\x59", 8))
#endif
	{
		send_result("line %d:%s����ʧ��(0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7]);
		GOTOERR;
	}
	
	//case2.2:PIN��Կ��DES�ӽ���
	memset(DataOut, 0, sizeof(DataOut));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TPK, NULL, 16, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ʹ�ð�װ��16�ֽ�DUKPT��Կ������PIN��Կ6ef92795e51271b2cdeb767b33f1a942��Կ��2��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����AE7CF265E0D6E0B8
	if(memcmp(DataOut,"\xAE\x7C\xF2\x65\xE0\xD6\xE0\xB8\xAE\x7C\xF2\x65\xE0\xD6\xE0\xB8", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(DataOut, 0, sizeof(DataOut));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TPK, NULL, 16, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ʹ�ð�װ��16�ֽ�DUKPT��Կ������PIN��Կ6ef92795e51271b2cdeb767b33f1a942��Կ��2��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����5EEEE57CE7262C1B
	if(memcmp(DataOut,"\x5E\xEE\xE5\x7C\xE7\x26\x2C\x1B\x5E\xEE\xE5\x7C\xE7\x26\x2C\x1B", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case2.3:MAC��Կ��DES�ӽ���
	memset(DataOut, 0, sizeof(DataOut));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TAK, NULL, 24, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ʹ�ð�װ��16�ֽ�DUKPT��Կ������MAC��Կ6ef92795e5128e4dcdeb767b33f156bd��Կ��3��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����090C8A5EDB362B8D
	if(memcmp(DataOut,"\x09\x0C\x8A\x5E\xDB\x36\x2B\x8D\x09\x0C\x8A\x5E\xDB\x36\x2B\x8D\x09\x0C\x8A\x5E\xDB\x36\x2B\x8D", 24))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(DataOut, 0, sizeof(DataOut));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TAK, NULL, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ʹ�ð�װ��16�ֽ�DUKPT��Կ������MAC��Կ6ef92795e5128e4dcdeb767b33f156bd��Կ��3��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����15066CFCCBAE36FF
	if(memcmp(DataOut,"\x15\x06\x6C\xFC\xCB\xAE\x36\xFF\x15\x06\x6C\xFC\xCB\xAE\x36\xFF\x15\x06\x6C\xFC\xCB\xAE\x36\xFF", 24))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
	//DUKPT��ϵû���ж϶���Կ�Ƿ���ܳ���Կ (��������˵)
	//case3:��Կ����ǿ�����ƴ򿪣�Ӧ�ò�����ʹ�ö̳��ȵ���Կ��װ�����ȵ���Կ
	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	
ERR://���Ժ���:����������Կ
	NDK_SecSetCfg(punCfgInfo);
	NDK_SecKeyErase();
	return;
}

