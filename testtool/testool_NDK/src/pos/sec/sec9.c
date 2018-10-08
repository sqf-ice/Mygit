/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec7.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecKeyErase�ӿڹ�����ɾ��������Կ��NDK_SecKeyDelete������ɾ��ָ������Կ���͵���Կ
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
#define TESTAPI	"NDK_SecKeyErase,NDK_SecKeyDelete"	

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
*						linwl 		20140813 modified	
*****************************************************************/
void sec9(void)
{
	/*private & local definition*/
	int ret = 0, i = 0;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	KEY_KCV SecLoad[ ]={
		//ǰ6��ΪDES�㷨��Կ
		{(uchar *)"\x13", 																NULL}, 					// 0	//TLK (key=16��0x13, TLK,ID=1)
		{(uchar *)"\xE1\x46\x47\xE8\xA1\x35\x06\x1A\xE1\x46\x47\xE8\xA1\x35\x06\x1A",	(uchar *)"\x3A\xDE\xBB\xE0"}, 	// 1	//TMK SEC_KCV_ZERO��ʽ(TMK ID=2, ������TLK��ɢ,����KEY=16��0x17)
		{(uchar *)"\x15", 																(uchar *)"\x26\x5A\x7A\xBF"},	// 2	//TMK SEC_KCV_ZERO��ʽ(TMK ID=3, ���İ�װ,����KEY=16��0x15)
		{(uchar *)"\x2E\x38\x1D\x92\x01\x88\x58\x22\x2E\x38\x1D\x92\x01\x88\x58\x22",	(uchar *)"\x54\x8C\x54\xFE"},	// 3	//TAK SEC_KCV_VAL��ʽ(TAK ID=4, TMK��ɢ,key����16��0x1f)
		{(uchar *)"\xFF\x4C\xAF\xA1\x33\x09\x76\xEC\xFF\x4C\xAF\xA1\x33\x09\x76\xEC",	(uchar *)"\xCB\xF7\x20\x63"}, 	// 4	//TPK SEC_KCV_VAL��ʽ(TPK ID=5, ��ID=3,type=TMK��ɢ,key����16��0x25)
		{(uchar *)"\x8A\xEE\xE9\x1A\x8C\x2F\x71\x97", 									(uchar *)"\xA4\x70\xF9\x9E"},	// 5	//TDK SEC_KCV_VAL��ʽkey:8λ(TDK ID=6, ��ID=3,type=TMK��ɢ,len=8,key����8��0x29)
		//�м�6��ΪSM4�㷨��Կ
		{(uchar *)"\x19",															   NULL},                       // 6  //TLK (key=16��0x19, TLK,ID=1)
		{(uchar *)"\x51\xE9\x6A\x1A\x86\x9F\x3B\x7C\x9B\xD3\x3F\x6C\x34\x4C\x76\x32",   (uchar *)"\x59\x4F\x26\x2E"}, // 7  //TMK SEC_KCV_ZERO��ʽ(TMK ID=2, ������TLK��ɢ,����KEY=16��0X1F)
		{(uchar *)"\x21",															    (uchar *)"\x6C\xD7\x6C\x59"}, // 8  //TMK SEC_KCV_ZERO��ʽ(TMK ID=3, ���İ�װ,����KEY=16��0X21) 
	        {(uchar *)"\xC0\xD3\xEB\x69\xB1\x06\x47\xEF\x76\xD2\x49\x62\x75\xA4\x8B\xFB",   (uchar *)"\x93\x38\x75\xD7"},	//9  //TAK SEC_KCV_ZERO��ʽ(TAK ID=4, TMK��ɢ,key����16��0X27 )
		{(uchar *)"\x44\x4E\x8D\x2A\x2A\xAA\x91\xB4\xAD\x47\x94\x78\xE6\x7D\x71\x75",   (uchar *)"\x2E\x36\x8F\x3C"},	//10  //TPK SEC_KCV_ZERO��ʽ(TPK ID=5, ��ID=3,type=TMK��ɢ,key����16��0X2D)
	        {(uchar *)"\x08\x48\x69\x83\x77\xF7\x1B\x33\xC5\x4D\x5C\x4F\xF5\x13\xB2\xC7",	(uchar *)"\xA4\x3F\xDD\xA6"},   //11  //TDK SEC_KCV_ZERO��ʽ(TDK ID=11, ��ID=6,type=TMK��ɢ,key����16��0X33) 
		//��6��ΪAES�㷨��Կ
		{(uchar *)"\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13",		   NULL},						// 12  //TLK (key=16��0x19, TLK,ID=1)
		{(uchar *)"\xFD\x5E\x76\xD2\xF0\x7E\xBD\x04\xF0\xAA\x2D\x96\x67\x81\xC6\x73",	(uchar *)"\x18\x19\x45\xEC"}, // 13 //TMK SEC_KCV_ZERO��ʽ(TMK ID=2, ������TLK��ɢ,����KEY=16��0X15)
		{(uchar *)"\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17",	(uchar *)"\x8E\x68\x02\x75"}, // 14 //TMK SEC_KCV_ZERO��ʽ(TMK ID=3, ���İ�װ,����KEY=32��0X17) 
		{(uchar *)"\x02\x3E\xA9\xBD\x97\x9B\xDC\x73\x49\x8E\xC5\x78\x3B\x02\x32\xA9 ",	(uchar *)"\x64\x43\xCD\x29"},	//15  //TAK SEC_KCV_ZERO��ʽ(TAK ID=4, TMK��ɢ,key����16��0X21 )
		{(uchar *)"\xF3\xCC\x71\x93\x89\x93\xE2\xE9\x7B\x28\x65\xE1\xC6\x1A\x2F\x93\x8F\xBC\x69\x18\x11\xD1\x43\x1A\x61\xDD\xDF\xED\x9C\xA5\xA8\xB3",	(uchar *)"\x93\xCC\x9E\xAC"},	//16  //TPK SEC_KCV_ZERO��ʽ(TPK ID=5, ��ID=3,type=TMK��ɢ,key����24��0X27)
		{(uchar *)"\x81\xE9\xF4\xAB\x89\xB9\x4D\xE4\x8D\xA7\x26\x18\xA1\xB6\x8B\xD1",	NULL},	//17  //TDK SEC_KCV_NONE��ʽ(TDK ID=6, ��ID=3,type=TMK��ɢ,key����16��0X33) 
		//����case11  �ظ�ʹ�����������
	//	{(uchar *)"\x11", 																(uchar *)"\x82\xE1\x36\x65"}, 	// 18	//TMK SEC_KCV_ZERO��ʽTLK (TMK ID=1, ���İ�װ,����KEY=16��0x11)
	 // {(uchar *)"\xC0\xB7\x57\x41\x3B\x84\x79\x25\xC0\xB7\x57\x41\x3B\x84\x79\x25",                   (uchar *)"\xA8\xB7\xB5\xBD"}, //19 //TAK SEC_KCV_ZERO��ʽ(TAK ID=3, TMK��ɢ,key����16��0X13 )
	};
#if  SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};
#else
	int algnum = 1;
	int key_flag[2] = {0,0} ;
#endif
#if SEC_AES_ENABLE
	ST_EXTEND_KEYBLOCK stExtendKey;
#endif

	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	for(i = 0; i< algnum; i++)
	{
		//����ǰ�ð�װ������Կ
		//��SEC_KCV_NONE��ʽ��װTLK,ID=1
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;	
		memset(stKeyInfo.sDstKeyValue, *SecLoad[0+i*6].pkey, stKeyInfo.nDstKeyLen);
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//��SEC_KCV_ZERO��ʽ��װTMK(����),ID=2
		stKeyInfo.ucScrKeyIdx=1;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|key_flag[i];
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;	
		memcpy(stKeyInfo.sDstKeyValue, SecLoad[1+i*6].pkey, stKeyInfo.nDstKeyLen);

		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[1+i*6].pkcv, stKcvInfoIn.nLen);	

		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//��SEC_KCV_ZERO��ʽ��װTMK(����),ID=3
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=3;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;	
		memset(stKeyInfo.sDstKeyValue, *SecLoad[2+i*6].pkey, stKeyInfo.nDstKeyLen);

		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, stKcvInfoIn.nLen);

		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//��des�㷨��,��SEC_KCV_VAL��ʽȥ��װTAK(��ID=3,type=TMK��ɢ),ID=4;��sm4�㷨��,��SEC_KCV_ZERO��ʽȥ��װTAK(��ID=3,type=TMK��ɢ),ID=4
		stKeyInfo.ucScrKeyIdx=3;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.ucDstKeyIdx=4;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;	
		memcpy(stKeyInfo.sDstKeyValue, SecLoad[3+i*6].pkey, stKeyInfo.nDstKeyLen);
		
		if(i==0)
			stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		else
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[3+i*6].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//��des�㷨��,��SEC_KCV_VAL��ʽȥ��װTPK(��ID=3,type=TMK��ɢ),ID=5;��sm4�㷨��,��SEC_KCV_ZERO��ʽȥ��װTPK(��ID=3,type=TMK��ɢ),ID=5
		stKeyInfo.ucScrKeyIdx=3;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.ucDstKeyIdx=5;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;	
		memcpy(stKeyInfo.sDstKeyValue, SecLoad[4+i*6].pkey, stKeyInfo.nDstKeyLen);
		if(i==0)
			stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		else
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[4+i*6].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		
#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
		//��des�㷨��,��SEC_KCV_VAL��ʽȥ��װTDK(��ID=3,type=TMK��ɢ),len=8,ID=6;��sm4�㷨��,��SEC_KCV_ZERO��ʽȥ��װTDK(��ID=3,type=TMK��ɢ),len=16,ID=6
		stKeyInfo.ucScrKeyIdx=3;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.ucDstKeyIdx=6;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|key_flag[i];
		memcpy(stKeyInfo.sDstKeyValue, SecLoad[5+i*6].pkey, stKeyInfo.nDstKeyLen);
		if(i==0)
		{
			stKeyInfo.nDstKeyLen=8;
			stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		}
		else
		{
			stKeyInfo.nDstKeyLen=16;
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		}
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[5+i*6].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#endif

#if !(/*defined ME50||defined ME50N||defined ME50C||defined ME50H||*/defined ME15C||defined ME15CHX||defined ME20||defined ME30||defined ME30S/*||defined ME30THM||defined ME32THM*/||defined ME30MH||defined ME15B)
		/*
		//case1:ɾ�������ڵ���Կid=100
		if((ret=NDK_SecKeyDelete(100,SEC_KEY_TYPE_TDK|key_flag[i]))!=NDK_ERR)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//case2:ɾ������ID=256Ϊ�Ƿ�ֵ
		if((ret=NDK_SecKeyDelete(256,SEC_KEY_TYPE_TDK|key_flag[i]))!=NDK_ERR)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		//case3:ɾ����Կ��ID�����Ͳ�ƥ��ID=6����Կ����ΪSEC_KEY_TYPE_TDK
		if((ret=NDK_SecKeyDelete(6,SEC_KEY_TYPE_TMK|key_flag[i]))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		*/
		//case2:ɾ���������Կ����SEC_KEY_TYPE_TLK-1,SEC_KEY_TYPE_TDK+1Ԥ�ڷ���ʧ��
		if((ret=NDK_SecKeyDelete(2,(SEC_KEY_TYPE_TLK|key_flag[i])-1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		if((ret=NDK_SecKeyDelete(2,(SEC_KEY_TYPE_TDK|key_flag[i])+1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		
		//case4:ɾ����ȷ��ID=6������=TDK����Կ���ȡKCVӦ��ʧ��
		if((ret=NDK_SecKeyDelete(6,SEC_KEY_TYPE_TDK|key_flag[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);//ret=-6,i=1
			GOTOERR;
		}
		if(i==0)
			stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		else
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		//ANDIROD_ENABLE	����˵�����óɹ�����Կ���ʱ��Ҳ������K21�жϷ���ʱ�Ƿ���mac��
		//�����óɰ�׿�˵���Կ���ʱ������macУ������K21���жϵģ���׿�������жϵ�������Կ�����ڣ����Ծͷ�����Կ������
#if  defined N900AZ ||defined N910AZ||defined IM81AZ//ANDIROD_ENABLE  �ϲ�Ʒ�������²�Ʒ�������� ͳһ����NDK_ERR_SECKM_READ_REC 20180416 modify  N910P�Ǻ����Ʒû�а�װid6����Կ,û��װ��Կ�����ɾ���Ƿ���-1309,����װ��Կ�����ɾ���᷵��-1311 20180830
		if (g_seckeyowner) //��Կ�ڰ�׿��
		{
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|key_flag[i], 6, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}	
		}
		else  //��Կ��k21��
		{	
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|key_flag[i], 6, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_MAC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
#elif   ( K21_ENABLE && !ANDIROD_ENABLE)  //K21_ENABLE���� IM81  N900
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|key_flag[i], 6, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_MAC)//������ȷ��Ŀǰֻ��IM81����˺���,�һ������ж˲�ͬ,����ֵ��ͬ
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#else
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|key_flag[i], 6, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}	
#endif
		
		//case5:ɾ����ȷ��ID=2������=TMK����Կ���ȡKCVӦ��ʧ��
		if((ret=NDK_SecKeyDelete(2, SEC_KEY_TYPE_TMK|key_flag[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);//ret=-6,i=1
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
#if  defined N900AZ ||defined N910AZ||defined IM81AZ||defined N910P//ANDIROD_ENABLE  �ϲ�Ʒ�������²�Ʒ�������� ͳһ����NDK_ERR_SECKM_READ_REC 20180416 modify
		if (g_seckeyowner) //��Կ�ڰ�׿��
		{	
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
		else  //��Կ��k21��
		{	
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 2, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_MAC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
#elif   ( K21_ENABLE && !ANDIROD_ENABLE)  //K21_ENABLE���� IM81  N900
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 2, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_MAC)//������ȷ��Ŀǰֻ��IM81����˺���,�һ������ж˲�ͬ,����ֵ��ͬ
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#else
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#endif
#else
		if((ret=NDK_SecKeyDelete(6,SEC_KEY_TYPE_TDK|key_flag[i]))!=NDK_ERR_NOT_SUPPORT)//�����豸����֧��NDK_SecKeyDelete����
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#endif
		//case6:ɾ����Ӱ������δɾ����ID����Կ,��ȡID=3������Կ��KCVӦ�óɹ�
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 3, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#if OVERSEAS_ENABLE||defined N910P  //����汾ʹ��3�ֽ�KCV��֤ ���ݹ����ʼ� 20180116 modify by jiangym 
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, 3))
#else			
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, 4))
#endif
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		
		//case7:��������ȫɾ������
		if((ret=NDK_SecKeyErase())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}

		//case8:��֤������Կ�Ѳ���
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
#if !defined N910P		//�߶˲�Ʒ910P ���߱�Ĭ�ϰ�װTLK���� 
		//sdk3.0����Կ�ڹ�����Կ��ʱ�尲ȫ������Կ��Ĭ�ϰ�װTLK��sdk2.0����װ  20180724  modify
		if(!g_seckeyowner &&  i==0 && flag_sdk3)
		{
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TLK|key_flag[i], 1, &stKcvInfoIn))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				RETURN;
			}
		}
		else
		{
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TLK|key_flag[i], 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				RETURN;
			}
		}
#else
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TLK|key_flag[i], 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}
#endif
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|key_flag[i], 3, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		} 
		if(i==0)
			stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		else
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TAK|key_flag[i], 4, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}
		if(i==0)
			stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
		else
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TPK|key_flag[i], 5, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			goto ERR;
		}
	}
	
	//AES ������ȡ���� //aes ��װʹ����չ��ʽ��װ 20180808 modify
#if SEC_AES_ENABLE 
	if(flag_sdk3) //sdk3.0���е���AES���
	{
		i = 2;
		//��SEC_KCV_NONE��ʽ��װTLK,ID=1
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock =(char*)SecLoad[0+i*6].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		stKcvInfoIn.nLen=0;
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//��SEC_KCV_ZERO��ʽ��װTMK(����),ID=2
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=1;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock = (char*)SecLoad[1+i*6].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[1+i*6].pkcv, stKcvInfoIn.nLen);	
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//��SEC_KCV_ZERO��ʽ��װTMK(����),ID=3
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=3;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock = (char*)SecLoad[2+i*6].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//��SEC_KCV_ZERO��ʽȥ��װTAK(��ID=3,type=TMK��ɢ),ID=4
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=3;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=4;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock = (char*)SecLoad[3+i*6].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[3+i*6].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

#if !defined N910P  //  N910P��PCI�汾����ʹ�õ��ֽڰ�װ���ֽ�,�ʲ���װ 20180809 modify
		//��SEC_KCV_ZERO��ʽȥ��װTPK(��ID=3,type=TMK��ɢ),ID=5
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=3;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=5;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 24;
		stExtendKey.pblock = (char*)SecLoad[4+i*6].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[4+i*6].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif

		//��SEC_KCV_ZERO��ʽȥ��װTDK(��ID=3,type=TMK��ɢ),len=16,ID=6
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=3;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=6;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock = (char*)SecLoad[5+i*6].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	
		//case2.1:ɾ���������Կ����SEC_KEY_TYPE_TLK-1,SEC_KEY_TYPE_TDK+1Ԥ�ڷ���ʧ��
		if((ret=NDK_SecKeyDelete(2,(SEC_KEY_TYPE_TLK|SEC_KEY_AES)-1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		if((ret=NDK_SecKeyDelete(2,(SEC_KEY_TYPE_TDK|SEC_KEY_AES)+1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		
		//case4.1:ɾ����ȷ��ID=6������=TDK����Կ���ȡKCVӦ��ʧ��
		if((ret=NDK_SecKeyDelete(6,SEC_KEY_TYPE_TDK|SEC_KEY_AES))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);//ret=-6,i=1
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		//ANDIROD_ENABLE	����˵�����óɹ�����Կ���ʱ��Ҳ������K21�жϷ���ʱ�Ƿ���mac��
		//�����óɰ�׿�˵���Կ���ʱ������macУ������K21���жϵģ���׿�������жϵ�������Կ�����ڣ����Ծͷ�����Կ������
#if  defined N900AZ ||defined N910AZ||defined IM81AZ||defined N910P//ANDIROD_ENABLE  �ϲ�Ʒ�������²�Ʒ�������� ͳһ����NDK_ERR_SECKM_READ_REC 20180416 modify
		if (g_seckeyowner) //��Կ�ڰ�׿��
		{
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|SEC_KEY_AES, 6, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}	
		}
		else  //��Կ��k21��
		{	
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|SEC_KEY_AES, 6, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_MAC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
#else
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TDK|SEC_KEY_AES, 6, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}	
#endif
		
		//case5.1:ɾ����ȷ��ID=2������=TMK����Կ���ȡKCVӦ��ʧ��
		if((ret=NDK_SecKeyDelete(2, SEC_KEY_TYPE_TMK|SEC_KEY_AES))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
#if  defined N900AZ ||defined N910AZ||defined IM81AZ||defined N910P//ANDIROD_ENABLE  �ϲ�Ʒ�������²�Ʒ�������� ͳһ����NDK_ERR_SECKM_READ_REC 20180416 modify
		if (g_seckeyowner) //��Կ�ڰ�׿��
		{	
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
		else  //��Կ��k21��
		{	
			if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_KEY_MAC)
			{
				send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
#else
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#endif

		//case6.1:ɾ����Ӱ������δɾ����ID����Կ,��ȡID=3������Կ��KCVӦ�óɹ�
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 3, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
#if OVERSEAS_ENABLE||defined N910P  //����汾ʹ��3�ֽ�KCV��֤ ���ݹ����ʼ� 20180116 modify by jiangym 
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, 3))
#else			
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, 4))
#endif
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		
		//case7.1:��������ȫɾ������
		if((ret=NDK_SecKeyErase())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}

		//case8.1:��֤������Կ�Ѳ���
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TLK|SEC_KEY_AES, 1, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 3, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		} 
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TAK|SEC_KEY_AES, 4, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			RETURN;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TPK|SEC_KEY_AES, 5, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
			goto ERR;
		}	
	}	
#endif				
	
#if SEC_SM4_ENABLE
	//case9:NDK_SecKeyDeleteӦ����ɾ�������㷨����Կ 
	//��װDES�㷨����Կ:��SEC_KCV_ZERO��ʽ���İ�װTMK
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret); 
		GOTOERR;
	}
	//��װSM4�㷨����Կ
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[8].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[8].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if !(/*defined ME50||defined ME50N||defined ME50C||defined ME50H||*/defined ME50NX||defined ME68||defined ME15C||defined ME15CHX||defined ME20||defined ME30||defined ME30S/*||defined ME30THM||defined ME32THM*/||defined ME30MH||defined ME30GX||defined ME15B)
	//ɾ��SM4�㷨����Կ��DES�㷨����Կ��Ȼ����
	if((ret=NDK_SecKeyDelete(2, SEC_KEY_TYPE_TMK|SEC_KEY_SM4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_DES, 2, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, 4))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//���°�װSM4�㷨����Կ
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[8].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[8].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if !(defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME50NX||defined ME68||defined ME15C||defined ME15CHX||defined ME20||defined ME30||defined ME30S/*||defined ME30THM||defined ME32THM*/||defined ME30MH||defined ME30GX||defined ME15B)	
	//ɾ��DES�㷨����Կ��SM4�㷨����Կ��Ȼ����
	if((ret=NDK_SecKeyDelete(2, SEC_KEY_TYPE_TMK|SEC_KEY_DES))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_SM4, 2, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[8].pkcv, 4))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case10:NDK_SecKeyEraseӦ��ɾ��ָ��IDȫ���㷨����Կ
	//���°�װDES�㷨����Կ:��SEC_KCV_NONE��ʽ���İ�װTMK
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_DES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
		RETURN;
	}
	if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_SM4, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
		RETURN;
	}
#endif
	// case11:�߶˲�ƷӦ���Ǳ߷���ɾ������Կ���κ����kcvУ������Կ�ᱨ���ڣ����ֹ�����Կͬ�����ڸ����⣨�������⣩linying added 20171214	
	//����sm4�Լ�AES��Կ�������֤  20180418 add
#if !(/*defined ME50||defined ME50N||defined ME50C||defined ME50H||*/defined ME15C||defined ME15CHX||defined ME20||defined ME30||defined ME30S/*||defined ME30THM||defined ME32THM*/||defined ME30MH||defined ME15B)
	for(i = 0; i< algnum; i++)
	{
		if(i==0 || i==1)
		{	
			// װ������Կ	
			stKeyInfo.ucScrKeyIdx=0;
			stKeyInfo.ucScrKeyType=0;
			stKeyInfo.ucDstKeyIdx=1;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
			stKeyInfo.nDstKeyLen=16;	
			memset(stKeyInfo.sDstKeyValue,*SecLoad[2+i*6].pkey, stKeyInfo.nDstKeyLen);
			stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
			stKcvInfoIn.nLen=4;
			memcpy(stKcvInfoIn.sCheckBuf,SecLoad[2+i*6].pkcv, stKcvInfoIn.nLen);
			if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
			{
				send_result("line %d:%sװ������Կ����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
			//װ��MAC������ԿTAK,index = 4
			stKeyInfo.ucScrKeyIdx=1;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
			stKeyInfo.ucDstKeyIdx=3;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|key_flag[i];
			stKeyInfo.nDstKeyLen=16;	
			memcpy(stKeyInfo.sDstKeyValue,SecLoad[3+i*6].pkey, stKeyInfo.nDstKeyLen);
			if(i==0)
				stKcvInfoIn.nCheckMode=SEC_KCV_VAL;
			else
				stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
			stKcvInfoIn.nLen=4;
			memcpy(stKcvInfoIn.sCheckBuf,SecLoad[3+i*6].pkcv, stKcvInfoIn.nLen);	
			if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
			{
				send_result("line %d:%sװ��MAC������Կ����ʧ��(ret=%d,i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
		}
		else
		{
#if SEC_AES_ENABLE
			if(flag_sdk3)
			{
				//��SEC_KCV_ZERO��ʽ��װTMK(����),ID=3
				memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
				memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
				memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
				stKeyInfo.ucScrKeyIdx=0;
				stKeyInfo.ucScrKeyType=0;
				stKeyInfo.ucDstKeyIdx=1;
				stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
				stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
				stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
				stExtendKey.len = 16;
				stExtendKey.pblock = (char*)SecLoad[2+i*6].pkey;
				memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

				stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
				stKcvInfoIn.nLen=4;
				memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2+i*6].pkcv, stKcvInfoIn.nLen);
				if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
					GOTOERR;
				}
				//��SEC_KCV_ZERO��ʽȥ��װTAK(��ID=3,type=TMK��ɢ),ID=4
				memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
				memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
				memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
				stKeyInfo.ucScrKeyIdx=1;
				stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
				stKeyInfo.ucDstKeyIdx=3;
				stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
				stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
				stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
				stExtendKey.len = 16;
				stExtendKey.pblock = (char*)SecLoad[3+i*6].pkey;
				memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
			
				stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
				stKcvInfoIn.nLen=4;
				memcpy(stKcvInfoIn.sCheckBuf, SecLoad[3+i*6].pkcv, stKcvInfoIn.nLen);
				if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
					GOTOERR;
				}
			}	
#endif
		}
		//ɾ������Կ����
		if((ret=NDK_SecKeyDelete(1, SEC_KEY_TYPE_TMK)!=NDK_OK))
		{
			send_result("line %d:%s����Կɾ��ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_SecKeyDelete(1, SEC_KEY_TYPE_TMK))!=NDK_OK)
		{
			send_result("line %d:%s����Կɾ��ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//У������Կ
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK, 1, &stKcvInfoIn))==NDK_OK)
		{
			send_result("line %d:%s����ԿУ�����(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//ɾ��mac������Կ
		if((ret=NDK_SecKeyDelete(3, SEC_KEY_TYPE_TAK))!=NDK_OK)
		{
			send_result("line %d:%sɾ��MAC��Կ����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_SecKeyDelete(3, SEC_KEY_TYPE_TAK))!=NDK_OK)
		{
			send_result("line %d:%sɾ��MAC��Կ����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//У��mac��Կ
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TAK, 3, &stKcvInfoIn))==NDK_OK)
		{
			send_result("line %d:%sMAC��ԿУ�����((ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
#endif
	//����AES�����֤20180418 add
#if  SEC_AES_ENABLE
	if(flag_sdk3)
	{
		//case12:NDK_SecKeyDeleteӦ����ɾ�������㷨����Կ 
		//��װDES�㷨����Կ:��SEC_KCV_ZERO��ʽ���İ�װTMK
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_DES;
		stKeyInfo.nDstKeyLen=16;	
		memset(stKeyInfo.sDstKeyValue, *SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret); 
			GOTOERR;
		}
		//��װAES�㷨����Կ
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock = (char *)SecLoad[14].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//ɾ��AES�㷨����Կ��DES�㷨����Կ��Ȼ����
		if((ret=NDK_SecKeyDelete(2, SEC_KEY_TYPE_TMK|SEC_KEY_AES))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_DES, 2, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if defined N910P
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, 3))
#else			
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, 4))
#endif
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//���°�װAES�㷨����Կ
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		stExtendKey.pblock = (char *)SecLoad[14].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//ɾ��DES�㷨����Կ��AES�㷨����Կ��Ȼ����
		if((ret=NDK_SecKeyDelete(2, SEC_KEY_TYPE_TMK|SEC_KEY_DES))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 2, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(memcmp(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, 3))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case10:NDK_SecKeyEraseӦ��ɾ��ָ��IDȫ���㷨����Կ
		//���°�װDES�㷨����Կ:��SEC_KCV_NONE��ʽ���İ�װTMK
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_DES;
		stKeyInfo.nDstKeyLen=16;	
		memset(stKeyInfo.sDstKeyValue, *SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		stKcvInfoIn.nLen=4;
		memcpy(stKcvInfoIn.sCheckBuf, SecLoad[2].pkcv, stKcvInfoIn.nLen);
		if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	
		if((ret=NDK_SecKeyErase())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_DES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_SecGetKcv(SEC_KEY_TYPE_TMK|SEC_KEY_AES, 2, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
	}
#endif

#if ANDIROD_ENABLE&&!defined N910P //N910P�������Ʒ���߱��˹��� �ʲ��� 20180809 modify
	if(flag_sdk3)
	{
		//sdk3.0�尲ȫ������Կ��Ĭ�ϰ�װTLK��sdk2.0����װ  20180724  modify
		//case11:��֤�尲ȫ������Կ���Ĭ�ϰ�װ16�ֽڵ�0x31��TLK  20180620 sull add
		if((ret=NDK_SecKeyErase())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		} 
		if((ret=NDK_SecSetKeyOwner("*"))!=NDK_OK)
		{
			send_result("line %d:������Կ����ʧ��(%d)", __LINE__, ret);
			GOTOERR;
		}
		//��SEC_KCV_NONE��ʽ��װTMK(����),ID=2,����16��0x15 ͨ��getkcv��ȡ������֤
		stKeyInfo.ucScrKeyIdx=1;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_DES;
		stKeyInfo.nDstKeyLen=16;	
		memcpy(stKeyInfo.sDstKeyValue, "\xCB\x62\x65\x94\x48\xAC\x8A\x72\xCB\x62\x65\x94\x48\xAC\x8A\x72", stKeyInfo.nDstKeyLen);//������16�ֽ�0x15��16�ֽ�0x31���ܵĽ��
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
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
			GOTOERR;
		}
	}
#endif
	send_result("%s����ͨ��", TESTAPI);
ERR: 
//�ָ�����ڴ�������
	if(flag_sdk3)
	{
		if(g_seckeyowner)
			NDK_SecSetKeyOwner("");
		else
			NDK_SecSetKeyOwner("*");
	}
	NDK_SecKeyErase();
	return;
}

