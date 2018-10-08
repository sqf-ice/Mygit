/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec3.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecLoadKey�ӿڹ���
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
#define TESTAPI	"NDK_SecLoadKey"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct key_kcv {
	uchar *pkey;
	uchar *pkcv;
} KEY_KCV;

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
void sec3(void)
{
	int ret = -1, ret1 = -1;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
#if TR31_ENABLE
	ST_EXTEND_KEYBLOCK* p;
	unsigned char block[512]={0};
	unsigned char mkey[17]={0};
	tr31_key_t tr31_st;
	unsigned int len = 0;
#endif	
	KEY_KCV SecLoad[ ]={
		{(uchar *)"\x11",																							NULL}, 					// 0		//case2.1
		{(uchar *)"\x13", 																							NULL}, 					// 1		//case2.2	(key=16��0x13, TLK,ID=1)
		{(uchar *)"\xE1\x46\x47\xE8\xA1\x35\x06\x1A\xE1\x46\x47\xE8\xA1\x35\x06\x1A", 								(uchar *)"\x3A\xDE\xBB\xE0"}, 		// 2		//case2.4  	SEC_KCV_ZERO��ʽ(TMK id=2, ������TLK��ɢ,����KEY=16��0x17)
		{(uchar *)"\x15", 																							(uchar *)"\x26\x5A\x7A\xBF"},		// 3		//case2.20	SEC_KCV_ZERO��ʽ
		{(uchar *)"\xA5\x94\x96\x77\x49\x26\x1C\xDF\xA5\x94\x96\x77\x49\x26\x1C\xDF", 								NULL},					// 4		//case2.5:��SEC_KCV_NONE��ʽȥ��װTAK,����key 0x19(��ID=4,type=TMK��ɢ),ID=3
		{(uchar *)"\x1b", 																							(uchar *)"\x4D\x01\xE5\x75"},		// 5		//case2.6 		SEC_KCV_VAL��ʽ
		{(uchar *)"\xA6\x9F\xC5\x1F\x50\x3C\xED\xE6\xA6\x9F\xC5\x1F\x50\x3C\xED\xE6", 								(uchar *)"\x45\xFA\x42\xA2"},		// 6		//case2.7		SEC_KCV_VAL��ʽ(TMK��ɢ,key����16��0x1d,ʧ��)
		{(uchar *)"\x2E\x38\x1D\x92\x01\x88\x58\x22\x2E\x38\x1D\x92\x01\x88\x58\x22", 								(uchar *)"\x54\x8C\x54\xFE"},		// 7		//case2.8/case2.22		SEC_KCV_VAL��ʽ(TMK��ɢ,key����16��0x1f   ��װTAK��ID=3)
		{(uchar *)"\x97\x70\xAF\x26\xA8\x22\xA2\xE3\x97\x70\xAF\x26\xA8\x22\xA2\xE3", 								(uchar *)"\x70\x8D\xE9\xF7"},		// 8		//case2.9		SEC_KCV_VAL��ʽ(��װTPK,��TAK ��ɢ,key����16���ֽ�0x21)
		{(uchar *)"\xF9\x1B\xFD\x82\x5F\x5F\x7B\x41\xF9\x1B\xFD\x82\x5F\x5F\x7B\x41", 								(uchar *)"\x55\x30\x7D\xF8"},		// 9		//case2.10	SEC_KCV_VAL��ʽ(��װTMK,��TAK��ɢ��key����16��0x23 )
		{(uchar *)"\xFF\x4C\xAF\xA1\x33\x09\x76\xEC\xFF\x4C\xAF\xA1\x33\x09\x76\xEC",								(uchar *)"\xCB\xF7\x20\x63"}, 		// 10	//case2.11	SEC_KCV_VAL��ʽ(��װTPK(��ID=2,type=TMK��ɢ) ID=4  ,key����16��0x25)
		{(uchar *)"\xDC\xEE\xF5\xC2\x06\x5E\x27\x5E", 																(uchar *)"\x0F\x35\x9E\x84"},		// 11	//case2.12	SEC_KCV_VAL��ʽ(��װTDK(��ID=2,type=TMK��ɢ),len=20(�Ƿ�)  ʧ��  KEY 0x27)
		{(uchar *)"\x8A\xEE\xE9\x1A\x8C\x2F\x71\x97", 																(uchar *)"\xA4\x70\xF9\x9E"},		// 12	//case2.13	SEC_KCV_VAL��ʽkey:8λ(��װTDK(��ID=2,type=TMK��ɢ),len=8,ID=5  , key����8��0x29)
		{(uchar *)"\x79\x3C\xA1\xEA\x4F\xC8\x5C\xFB\x79\x3C\xA1\xEA\x4F\xC8\x5C\xFB", 								(uchar *)"\x16\x55\x7C\x70"},		// 13	//case2.14	SEC_KCV_VAL��ʽ(��װTDK(��ID=2,type=TMK��ɢ),len=16,ID=5  , key����16��0x2b)
		{(uchar *)"\xDE\xD9\x76\x77\x94\xFC\xD9\x05\xDE\xD9\x76\x77\x94\xFC\xD9\x05\xDE\xD9\x76\x77\x94\xFC\xD9\x05",(uchar *)"\xDD\xA4\x94\xE6"},		// 14	//case2.15	SEC_KCV_VAL��ʽkey:24λ(��װTDK(��ID=2,type=TMK��ɢ),len=24,ID=5  , key����24��0x2d)
		{(uchar *)"\x7F\x9D\x4F\x7D\x2F\x17\x34\x4D\x7F\x9D\x4F\x7D\x2F\x17\x34\x4D", 								(uchar *)"\xBE\xBA\xC0\x29"},		// 15	//case2.19	SEC_KCV_VAL��ʽ��װtype=100(��ID=2,type=TMK��ɢ,����key=16��0x2f)
		{(uchar *)"\x47\x7C\xD6\x84\x20\x43\xFE\xC0\x47\x7C\xD6\x84\x20\x43\xFE\xC0",								NULL},    					// 16	//case2.21	SEC_KCV_NONE(��װTDK(id=7), TAK(id=8)(��TMK��ɢ)   key����16��0x31)
		{(uchar *)"\x33", 																							NULL},					// 17	//case3.2:	SEC_KCV_NONE(��װTAK(����),ID=4,len=8)
		{(uchar *)"\x35", 																							(uchar *)"\xD2\xDB\x51\xF1"}, 		// 18	//case3.3:       SEC_KCV_ZERO(��װTDK(����),ID=5,len=16)
		{(uchar *)"\x37",																							(uchar *)"\x52\xA1\xC0\x88"},		// 19	//case3.4:   	SEC_KCV_VAL(TPK(����),ID=3,len=24)
		{(uchar *)"\x39",																							NULL},					// 20	//case3.5:   	�Ƿ���ʽ(100) ��װTPK(����),ID=3,len=24
		{(uchar *)"\x3b", 																							NULL},					// 21	//case3.6:	SEC_KCV_NONE(��װTDK(id=9), TAK(id=10)(����))
		{(uchar *)"\x3d",																							NULL},					// 22	//case3.7		SEC_KCV_NONE(����)
		{(uchar *)"\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f", 								(uchar *)"\xCE\xE8\xAC\x95"},		// 23	//case2.16:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ)
		{(uchar *)"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41",								(uchar *)"\x49\x17\xA0\xA3"}, 		//24		//case2.17:��װTDK(��ID=2,type=TMK��ɢ),len=sizeof(ST_EXTEND_KEYBLOCK) ,ST_SEC_KEY_INFO.len=1,ID=5
		{(uchar *)"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43", 								(uchar *)"\xB3\xE4\x23\x06"},		//25		//case2.18:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=sizeof(ST_EXTEND_KEYBLOCK) ,ID=5
	};
	uchar ch = 0x00;

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
			
	//case1: ����Ƿ�����
	if(((ret = NDK_SecLoadKey(NULL, &stKcvInfoIn)) != NDK_ERR_PARA)
	||((ret1= NDK_SecLoadKey(&stKeyInfo, NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, ret1);
		RETURN;
	}

	//Լ��:���������������˵��,�򶼰�װ16�ֽڳ�key
	//case2:MK/SK��ϵ
	//case2.1:TLK�������ڷ�Χ��id=2 (��������1��1)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[0].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;

	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2.2:��SEC_KCV_NONE��ʽ��װTLK,ID=1
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[1].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.3:TMK�������ڷ�Χ��
	//��Χ�ѵ���Ϊ0-255,�ʴ˲��Ե���Ч,�ݲ�ʵ��
		
	//case2.4:��SEC_KCV_ZERO��ʽ��װTMK(����),ID=2
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, stKcvInfoIn.nLen);	

	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.20:��SEC_KCV_ZERO��ʽ��װTMK(����),ID=2
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[3].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[3].pkcv, stKcvInfoIn.nLen);

	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.5:��SEC_KCV_NONE��ʽȥ��װTAK(��ID=4,type=TMK��ɢ),ID=3
	stKeyInfo.ucScrKeyIdx=4;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[4].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_READ_REC) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if 0 //ID=101�ѺϷ�,������
	//case2.6:��SEC_KCV_VAL ��ʽȥ��װTAK(��ID=101,type=TMK��ɢ),ID=3
	stKeyInfo.ucScrKeyIdx=101;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[5].uckey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[5].loadkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != 100)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;
	}
#endif	

	//case2.7:��SEC_KCV_VAL ��ʽȥ��װTAK(��ID=2,type=100��ɢ),ID=3
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=100;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[6].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[6].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)	  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.8:��SEC_KCV_VAL��ʽȥ��װTAK(��ID=2,type=TMK��ɢ),ID=3
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[7].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[7].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.22:��SEC_KCV_VAL��ʽȥ��װTAK(��ID=2,type=TMK��ɢ),ID=3(�ظ�case2.8,����֤���ǰ�װ,������Կֵһ��.UPDATE��ʽ)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[7].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[7].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.9:��SEC_KCV_VAL��ʽȥ��װTPK(��ID=3,type=TAK��ɢ),ID=4
	stKeyInfo.ucScrKeyIdx=3;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[8].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[8].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) !=NDK_ERR_SECP_PARAM)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.10:��SEC_KCV_VAL��ʽȥ��װTMK(��ID=3,type=TAK��ɢ),ID=4
	stKeyInfo.ucScrKeyIdx=3;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[9].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[9].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.11:��SEC_KCV_VAL��ʽȥ��װTPK(��ID=2,type=TMK��ɢ),ID=4
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[10].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[10].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.12:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=6(�Ƿ�),ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=6;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[11].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[11].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.13:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=8,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=8;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[12].pkey, stKeyInfo.nDstKeyLen);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[12].pkcv, stKcvInfoIn.nLen);
#if OVERSEAS_ENABLE	//����汾��֧��8�ֽ���Կ,����Ҫ��װʧ��
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
#elif defined ME30S ||defined N910P//ME30s���ز����Ƿ�-6
    if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_PARA)
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.14:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=16,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[13].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[13].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.15:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=24,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=24;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[14].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);
#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)	//����汾�̼���֧�ֵͳ�����Կ��װ�߳�����Կ
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_KEY_LEN)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//case2.16:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=sizeof(ST_EXTEND_KEYBLOCK) ,ST_SEC_KEY_INFO.format!=TR31,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	
	memset(mkey, 0x15, 16);//��Ӧ��TMK ����key SecLoad[2].uckey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[23].pkey;
	tr31_st.keylen = 16;
	
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31+1;
	p->len=len;
	p->pblock=(char *)block;
	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[23].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_PARA)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.17:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=sizeof(ST_EXTEND_KEYBLOCK) ,ST_SEC_KEY_INFO.len=1,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x15, 16);//��Ӧ��TMK ����key SecLoad[2].uckey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[24].pkey;
	tr31_st.keylen = 16;
	
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=1;
	p->pblock=(char *)block;
	
	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[24].pkcv, 4);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_PARAM)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.23:��SEC_KCV_NONE��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=sizeof(ST_EXTEND_KEYBLOCK) ,ST_EXTEND_KEYBLOCK.len=72,ST_EXTEND_KEYBLOCK.pblockΪ72��0x00,ID=5	
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);

	memset(mkey, 0x15, 16);//��Ӧ��TMK ����key SecLoad[2].uckey
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[25].pkey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	p=(ST_EXTEND_KEYBLOCK*)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	memset(p->pblock, 0x00, len);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.18:��SEC_KCV_VAL��ʽȥ��װTDK(��ID=2,type=TMK��ɢ),len=sizeof(ST_EXTEND_KEYBLOCK) ,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x15, 16);//��Ӧ��TMK ����key SecLoad[2].uckey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[25].pkey;
	tr31_st.keylen = 16;
	
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	p=(ST_EXTEND_KEYBLOCK*)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	p->pblock=(char *)block;

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[25].pkcv, stKcvInfoIn.nLen);
	
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case2.19:��SEC_KCV_VAL��ʽȥ��װtype=100(��ID=2,type=TMK��ɢ),ID=6
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.ucDstKeyType=100;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[15].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[15].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)   
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.21:ʹ��ͬһ��KEY����װTDK(id=7), TAK(id=8)(��TMK��ɢ)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=7;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[16].pkey, stKeyInfo.nDstKeyLen);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	stKeyInfo.ucDstKeyIdx=8;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
#if ANDIROD_ENABLE&!defined N910P//defined IM81||defined N900|| 
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*������ȷ������IM81������᷵�سɹ� 20150414 ���ú͹���ȷ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC)	/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:FIXED��ϵ
	//case3.1:��SEC_KCV_NONE��ʽȥ��װTAK(����),ID=4,len=sizeof(ST_EXTEND_KEYBLOCK)  //len=sizeof(ST_EXTEND_KEYBLOCK) ����MK/SK��ϵ,��Ӧ����
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_PARA)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3.2:��SEC_KCV_NONE��ʽȥ��װTAK(����),ID=4,len=16
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[17].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3.3:��SEC_KCV_ZERO��ʽȥ��װTDK(����),ID=5,len=16
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[18].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[18].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3.4:��SEC_KCV_VAL��ʽȥ��װTPK(����),ID=3,len=24
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=24;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[19].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[19].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3.5:��100��ʽȥ��װTPK(����),ID=3,len=24
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=100;
	stKeyInfo.nDstKeyLen=24;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[20].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=100;
	stKcvInfoIn.nLen=4;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3.6:ʹ��ͬһ��KEY����װTDK(id=9), TAK(id=10)(����)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[21].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	stKeyInfo.ucDstKeyIdx=10;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
#if ANDIROD_ENABLE&!defined N910P//defined IM81||defined N900||
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*������ȷ������IM81������᷵�سɹ�*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3.7:��key����żУ��λ��ͬ,Ӧ��Ϊͬһ��key,��װTDK(id=9), TAK(id=10)(����)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[22].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	ch = (*(SecLoad[22].pkey)&0xfe)|(*(SecLoad[22].pkey)&0x01?0x00:0x01);
	memset(stKeyInfo.sDstKeyValue, ch/**(SecLoad[22].pkey)-1*/, stKeyInfo.nDstKeyLen);
	stKeyInfo.ucDstKeyIdx=10;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
#if ANDIROD_ENABLE&!defined N910P//defined IM81||defined N900||
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)/*������ȷ������IM81������᷵�سɹ�*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC )/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//case4:DUKPT��ϵ	
	//��NDK_SecLoadTIK�����в���
	send_result("%s����ͨ��", TESTAPI);
	
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}

