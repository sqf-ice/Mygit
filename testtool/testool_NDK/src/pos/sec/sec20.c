/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec5.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecGetMacDUKPT�ӿڹ���
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
#define TESTAPI	"NDK_SecGetMacDUKPT"	

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
*						jiangym			20150125	modified
*						zhengry			20170425	modified						
*****************************************************************/
void sec20(void)
{
	int ret = 0, i=0;
	ST_SEC_KCV_INFO KcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar DataIn[33]={0},MacOut[9]={0},KSN1[11]={0},KSN2[11]={0},KSN3[11]={0},KeyValue[17]={0},PreOut[9]={0};
#if TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	uchar mkey[17]={0},block[512]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKeyIn;
	uchar *uModeulus1=(uchar *)"4FB2D08918F766B30B2144FE25D09A160FFCF256086D82773FB04D4BD7E4772F2557FC25EE130A0B984467995BE80EE1751F577BD3E5CAFF60712722F33D0479";
#endif
	uchar DukptKeyValue[16]={0xD5,0xD4,0x4F,0xF7,0x20,0x68,0x3D,0x0D,0x70,0xBE,0x25,0x28,0x18,0xE2,0xA1,0x8A};//DUKPY��ʼ������Կ����Կ�����������ο�sec21������ע��
	uchar KSN[11]={0};//��ʼKSN
#if OVERSEAS_ENABLE
	int num=2;
	int sec_mac_x99[2]={SEC_MAC_X99, SEC_MAC_RESP_X99};
	int sec_mac_x9191[2]={SEC_MAC_X919, SEC_MAC_RESP_X919};
	int sec_mac_ecb[2]={SEC_MAC_ECB, SEC_MAC_RESP_ECB};
	int sec_mac_9606[2]={SEC_MAC_9606, SEC_MAC_RESP_9606};
#else
	int num=1;
	int sec_mac_x99[1]={SEC_MAC_X99};
	int sec_mac_x9191[1]={SEC_MAC_X919};
	int sec_mac_ecb[1]={SEC_MAC_ECB};
	int sec_mac_9606[1]={SEC_MAC_9606};
#endif

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//ע�ͱ�����ʹ��Ĭ��KSNΪ10�ֽ�00�����3���ֽ�ΪKSN����Ϊ000000
	//Ĭ������DUKPT�ĳ�ʼ��ԿΪD5D44FF720683D0D70BE252818E2A18A
	//��װ�ú������ǰ��ԿΪ6ef92795e512714dcdeb767b33f1a9bd  ��ӦKSNΪ000001
	//ͨ�����¹�������������DUKPT��Կ
	/*
	case SEC_KEY_TYPE_TPK:		//����PIN KEY   	6ef92795e51271b2cdeb767b33f1a942
		key[7] ^= 0xff;
		key[15] ^= 0xff;
		break;
	case SEC_KEY_TYPE_TAK:		//����MAC KEY 		6ef92795e5128e4dcdeb767b33f156bd
		key[6] ^= 0xff;
		key[14] ^= 0xff;
		break;
	case SEC_KEY_TYPE_TDK:		//�������ݼӽ���KEY 6ef92795e5ed714dcdeb767b330ea9bd    ��׿ƽ̨��Ʒ��Ҫ���Լ������ݺ���Կ����3DES����  BFA9CA10BF791459 482339A090BF7CC9 Ϊʵ����Կ
		key[5] ^= 0xff;
		key[13] ^= 0xff;
	case SEC_KEY_TYPE_RESP_TAK: //����MAC RESPONSE  6EF927951A12714DCDEB767BCCF1A9BD  
		key[4] ^= 0xff;
		key[12] ^= 0xff;
	case SEC_KEY_TYPE_RESP_TDK: //�������ݼӽ���KEY RESPONSE    6EF9276AE512714DCDEB768433F1A9BD    
		key[3] ^= 0xff;
		key[11] ^= 0xff;		
		*/
	//KSN�ԼӺ����ɵ�����ǰ��ԿΪ 5635b184dd5d8f415617b6600c6e47cf
	/*���ں���汾Ҫ��װ����Կ����ͬid2 id3��Ӧ���޸���DUKPT�ĳ�ʼ��Կ
		id=2�ĳ�ʼDUKPT��ԿΪ:4B82DBD972BBBF20BCBA8632CF9AB57C
		��װ�ú������ǰ��ԿΪbae698c215018dd5e3cf99433dd1560e  ��ӦKSNΪ000001
		id=3�ĳ�ʼDUKPT��ԿΪ:25AB8A24A244883CB2CA76D7617BC60B
		��װ�ú������ǰ��ԿΪ9b53578c8110f7c6f3bf8ca3bbff6358  ��ӦKSNΪ000001
		������������,���ﲻ�����г�
	*/
	//����ǰ��:����������Կ 
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//���İ�װһ��DUKPT, ID=6, psKsnInΪ10���ֽ�0x00  /*Լ��:����KCV���Բ�У�鷽ʽ��װ*/
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);//��ʼKSN
	if ((ret=NDK_SecLoadTIK(6, 0, 16, DukptKeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTLK, ID=1  �������İ�װ
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	 //���ں���汾��ֹ��װ��ͬ��DES��Կ (����DES��Կ,DUKPT ��ʼ��ԿTIK) ��������,������ʹ�ò�ͬ����Կ��װʹ��������ƽ̨��Ʒ 20180116 modify
	//���İ�װһ��DUKPT, ID=2, KSNΪ10���ֽ�0x00 \xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23 ���������� ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	//�˴�����������Ϊ:4B82DBD972BBBF20BCBA8632CF9AB57C��16λ0x11���ܺ�Ľ��:1A5DA388BAB978A8AA577F104AF0FC9B
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memcpy(KeyValue, "\x1A\x5D\xA3\x88\xBA\xB9\x78\xA8\xAA\x57\x7F\x10\x4A\xF0\xFC\x9B", 16);
	if ((ret=NDK_SecLoadTIK(2, 1, 16, KeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0  //���ﰲװ���id2��id6��һ������Կ 
	//���İ�װһ��DUKPT, ID=2, KSNΪ10���ֽ�0x00 \xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23 ��DukptKeyValue ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memcpy(KeyValueIn, "\xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23", 16);
	if ((ret=NDK_SecLoadTIK(2, 1, 16, KeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//TR-31���İ�װһ��DUKPT, ID=3, KSNΪ10���ֽ�0x00,������Ľ��  block ��װ��ID=3��
	memset(KSN, 0x00, 10);
	memset(mkey, 0x11, 16);
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = 3;
	memcpy(tr31_st.usage, KT_BDK_TYPE, 4);	/*DUKPT*/
	 //���ں���汾��ֹ��װ��ͬ����Կ������ĳ�ʹ�ò�ͬ�����İ�װ20180116 modify by jym
	//��װ�������޸�Ϊ25AB8A24A244883CB2CA76D7617BC60B
	memcpy(DukptKeyValue, "\x25\xAB\x8A\x24\xA2\x44\x88\x3C\xB2\xCA\x76\xD7\x61\x7B\xC6\x0B", 16);
	tr31_st.key = DukptKeyValue;//��DUKPT������Կ����TR31��ʽ���м����õ�block
	tr31_st.keylen = 16;
	
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��DUKPT������Կ����TR31��ʽ���м����õ�block��Ȼ����а�װ
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadTIK(3, 1, len, block, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//����25AB8A24A244883CB2CA76D7617BC60B��16λ0x11des���ܽ��(�ֳ�ǰ��8�ֽ�)3CC6B71F7117702C3850C279A2CF834D
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);
	memcpy(KeyValue, "\x3C\xC6\xB7\x1F\x71\x17\x70\x2C\x38\x50\xC2\x79\xA2\xCF\x83\x4D", 16);
	if ((ret=NDK_SecLoadTIK(3, 1, 16, KeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if SEC_RSA_ENABLE
	//���İ�װRSA, ID=4
	RsaKeyIn.usBits=RSA_KEY_LEN_512;
	strcpy((char *)RsaKeyIn.sExponent, "03");
	memcpy(RsaKeyIn.sModulus, uModeulus1, RsaKeyIn.usBits*2/8);
	if((ret=NDK_SecLoadRsaKey(4, &RsaKeyIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//���İ�װTPK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x21\x21\x21\x21\x21\x21\x21\x21\x23\x23\x23\x23\x23\x23\x23\x23", stKeyInfo.nDstKeyLen);
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &KcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:�Ƿ�����
	//case1.1:psDataIn==NULL
	if ((ret=NDK_SecGetMacDukpt(6, NULL, 8, MacOut, KSN1, SEC_MAC_X99))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.2:psMacOut==NULL
	memset(DataIn, 0x12, 32);
	if ((ret=NDK_SecGetMacDukpt(6, DataIn, 8, NULL, KSN1, SEC_MAC_X99))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.3:ucMod �Ƿ�
	if ((ret=NDK_SecGetMacDukpt(6, DataIn, 8, MacOut, KSN1, 100))!=NDK_ERR_SECCR_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:Datalen<0
	if ((ret=NDK_SecGetMacDukpt(6, DataIn, -1, MacOut, KSN1, SEC_MAC_X99))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:ucGroupIdx������(10, 101)
	if ((ret=NDK_SecGetMacDukpt(10, DataIn, 8, MacOut, KSN1, SEC_MAC_X99))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.6:psKsnOut=NULL
	if ((ret=NDK_SecGetMacDukpt(6, DataIn, 8, MacOut, NULL, SEC_MAC_X99))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	for(i=0;i<num;i++)
	{
		//case2:��SEC_MAC_X99ģʽ,7�ֽ���������,DUKPT6
		memset(DataIn, 0x22, sizeof(DataIn));
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(6, DataIn, 7, MacOut, KSN1, sec_mac_x99[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ʹ�ù��߼����:319AD5A162A11685  (����Mac��Կ��22222222222222  3DES����)
			memcpy(PreOut, "\x31\x9A\xD5\xA1\x62\xA1\x16\x85", 8);
		else
			//����汾����SEC_MAC_RESP_X99,��SEC_MAC_RESP_X99ģʽ,������ԿΪ6EF927951A12714DCDEB767BCCF1A9BD��22222222 22222200��3DES����
			memcpy(PreOut, "\x46\x17\x7F\x79\x78\x43\x16\xF9", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR; 
		}

		//case3:��SEC_MAC_X99ģʽ,8�ֽ���������,DUKPT2
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(2, DataIn, 8, MacOut, KSN, sec_mac_x99[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		} 
		if(i==0)
			//(ID2����Mac��Կbae698c2150172d5e3cf99433dd1a90e��2222222222222222  3DES)ʹ�ù��߼����:6D8D05AB755327F0   
			memcpy(PreOut, "\x6D\x8D\x05\xAB\x75\x53\x27\xF0", 8);
		else
			//ID2����MAC RESP��Կbae698c2ea018dd5e3cf9943c2d1560e��22222222 22222222��3DES�����FA3F038599099ACA
			memcpy(PreOut, "\xFA\x3F\x03\x85\x99\x09\x9A\xCA", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		//case4:��SEC_MAC_X99ģʽ,10�ֽ���������,DUKPT3
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(3, DataIn, 10, MacOut, KSN1, sec_mac_x99[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//���������Ϊ10�ֽڣ���Ϊ2��block��ÿ��blockΪ8�ֽڣ�����ʱ���油0��������Կ���һ������block���ܣ��õ��Ľ����ڶ�������block����򣬵õ��Ľ����������Կ�����ܣ��õ����ļ���ֵ��(ʵ���ü��㹤�߾Ϳɵõ����)
			//(ID3����Mac��Կ:9b53578c811008c6f3bf8ca3bbff9c58)ʹ�ù��߼����:3FE28BE4A5E6EA0F
			memcpy(PreOut, "\x3F\xE2\x8B\xE4\xA5\xE6\xEA\x0F", 8);
		else
			//(ID3����Mac RESP��Կ:9b53578c7e10f7c6f3bf8ca344ff6358)ʹ�ù��߼����:67ECF89CE8067F2D
			memcpy(PreOut, "\x67\xEC\xF8\x9C\xE8\x06\x7F\x2D", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		/**************************
		SEC_MAC_X919ģʽ:�������������Ϊ18�ֽ�,���Ϊ3��block,ÿ��blockΪ8�ֽ�,����ʱ���油0,18�ֽڵĻ���Ϊ8�ֽ�8�ֽ���2�ֽ�
		2�ֽں��油0��8�ֽ�,ͬʱ��ԿҲ��Ϊ8�ֽڵ�block,�������Կ�ɷ�Ϊ2��block,Ȼ���һ������block���һ����Կblock����
		des����,���ܺ�Ľ����ڶ�������block�������,����Ľ�������һ����Կblock����des����,���ܺ�Ľ�������һ��
		block�������,����Ľ����������Կ����3des����,���ܽ������macout��������(���ֻ����������block,��ô��һ������
		block���ܺ�Ľ����ڶ�������block����Ľ����������Կ����3des����)�Դ�����.......
		���ϲ�������ͨ�������е�des�� xor ����
		***************************/
		//case5:��SEC_MAC_X919/SEC_MAC_RESP_X919ģʽ,13�ֽ���������,DUKPT6
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(6, DataIn, 13, MacOut, KSN1, sec_mac_x9191[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//4617F3B494C22554 
			memcpy(PreOut, "\x46\x17\xF3\xB4\x94\xC2\x25\x54", 8);
		else
			//��SEC_MAC_RESP_X919ģʽ,��һ������blockΪ8��0x22,�ڶ�������blockΪ5��0x22+3��0x00,ǰ�˸��ֽ�������Կ����block1����,���ܽ����block2�����,���Ľ����16���ֽڵ�������Կ������,�õ����ܽ��
			memcpy(PreOut, "\xC1\x6C\x45\xC4\x4E\xA1\x70\x5E", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		//case6:��SEC_MAC_X919/SEC_MAC_RESP_X919ģʽ,16�ֽ���������,DUKPT2
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(2, DataIn, 16, MacOut, KSN1, sec_mac_x9191[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ID2����Mac��Կbae698c2150172d5e3cf99433dd1a90e  ��F43027C8D1725AE9
			memcpy(PreOut, "\xF4\x30\x27\xC8\xD1\x72\x5A\xE9", 8);
		else
			//ID2����MAC RESP��Կbae698c2ea018dd5e3cf9943c2d1560e ��84EB6857EEDFB7B4
			//��SEC_MAC_RESP_X919ģʽ,��һ������blockΪ8��0x22,�ڶ�������blockΪ8��0x22,ǰ�˸��ֽ�������Կ����block1����,���ܽ����block2�����,���Ľ����16���ֽڵ�������Կ������,�õ����ܽ��
			memcpy(PreOut, "\x84\xEB\x68\x57\xEE\xDF\xB7\xB4", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		//case7:��SEC_MAC_X919/SEC_MAC_RESP_X919ģʽ,18�ֽ���������,DUKPT3
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(3, DataIn, 18, MacOut, KSN1, sec_mac_x9191[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ID3����Mac��Կ:9b53578c811008c6f3bf8ca3bbff9c58  ��1AF1B490219AD23E
			memcpy(PreOut, "\x1A\xF1\xB4\x90\x21\x9A\xD2\x3E", 8);
		else
			//ID3����Mac RESP��Կ:9b53578c7e10f7c6f3bf8ca344ff6358 ��63DFC061C3169F47
			//��SEC_MAC_RESP_X919ģʽ,��һ������blockΪ8��0x22,�ڶ�������blockΪ8��0x22,����������blockΪ2��0x22+6��0x00,ǰ�˸��ֽ�������Կ����block1����,���ܽ����block2�����,
			//���Ľ����ǰ�˸��ֽ�������Կ������,�õ������block3���������,�������������16�ֽ�������Կ������,�õ����ܽ��
			memcpy(PreOut, "\x63\xDF\xC0\x61\xC3\x16\x9F\x47", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}
			
		/**************************
		��û���ߣ���SEC_MAC_ECB/SEC_MAC_RESP_ECBģʽ��ԭ������:
		step1:�����ݷ�Ϊ8���ֽ�һ��,�໥��򣬵õ�8���ֽڵ������
		step2:��8���ֽڵ������ת��ΪASCII��õ�16�ֽڵ������
		step3:��������Կ��step2�õ���ǰ�˸��ֽ���3des����
		step4:��step3�õ��ļ��ܽ����step2��˸��ֽ������,�õ�8���ֽڵ������
		step5:��step4�õ����������������Կ��3des����,�õ�8���ֽڵ���Կ���
		step6:��step5�õ���8���ֽ���Կ���ת��Ϊ16�ֽڵ�ASCII�룬ȡǰ8���ֽ�Ϊ������
		***************************/
		//case8:��SEC_MAC_ECB/SEC_MAC_RESP_ECBģʽ,19�ֽ���������,DUKPT6
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(6, DataIn, 19, MacOut, KSN1, sec_mac_ecb[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ʹ�ù��߼����:3333384146333538
			memcpy(PreOut, "\x33\x33\x38\x41\x46\x33\x35\x38", 8);
		else
			//ʹ�ù��߼����:3745434637393945
			memcpy(PreOut, "\x37\x45\x43\x46\x37\x39\x39\x45", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		//case9:��SEC_MAC_ECB/SEC_MAC_RESP_ECBģʽ,24�ֽ���������,DUKPT2
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(2, DataIn, 24, MacOut, KSN1, sec_mac_ecb[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ID2����Mac��Կbae698c2150172d5e3cf99433dd1a90eʹ�ù��߼����:3534304237453033
			memcpy(PreOut, "\x35\x34\x30\x42\x37\x45\x30\x33", 8);
		else
			//ID2����MAC RESP��Կbae698c2ea018dd5e3cf9943c2d1560eʹ�ù��߼����:3239433134303532
			memcpy(PreOut, "\x32\x39\x43\x31\x34\x30\x35\x32", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}
			
		//case10:��SEC_MAC_ECB/SEC_MAC_RESP_ECBģʽ,26�ֽ���������,DUKPT3
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(3, DataIn, 26, MacOut, KSN1, sec_mac_ecb[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ID3����Mac��Կ:9b53578c811008c6f3bf8ca3bbff9c58ʹ�ù��߼����:3246353833313337
			memcpy(PreOut, "\x32\x46\x35\x38\x33\x31\x33\x37", 8);
		else
			//ID3����Mac RESP��Կ:9b53578c7e10f7c6f3bf8ca344ff6358 ʹ�ù��߼����:3546423335463032
			memcpy(PreOut, "\x35\x46\x42\x33\x35\x46\x30\x32", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}
			
		/**************************		
		��û���ߣ���SEC_MAC_9606/SEC_MAC_RESP_9606ģʽ��ԭ������:
		�����ݷ�Ϊ8���ֽ�һ��,�໥��򣬵õ�8���ֽڵ������,���������������Կ������
		***************************/
		//case11:��SEC_MAC_9606/SEC_MAC_RESP_9606ģʽ,30�ֽ���������,DUKPT6
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(6, DataIn, 30, MacOut, KSN1, sec_mac_9606[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ʹ�ù��߼����:41CC9D655690CA9E
			memcpy(PreOut, "\x41\xCC\x9D\x65\x56\x90\xCA\x9E", 8);
		else
			//ʹ�ù��߼����:6C903CA8FE843E82
			memcpy(PreOut, "\x6C\x90\x3C\xA8\xFE\x84\x3E\x82", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		//case12:��SEC_MAC_9606/SEC_MAC_RESP_9606ģʽ,31�ֽ���������,DUKPT2
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(2, DataIn, 31, MacOut, KSN1, sec_mac_9606[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ID2����Mac��Կbae698c2150172d5e3cf99433dd1a90eʹ�ù��߼����:36F2DE096FD18506
			memcpy(PreOut, "\x36\xF2\xDE\x09\x6F\xD1\x85\x06", 8);
		else
			//ID2����MAC RESP��Կbae698c2ea018dd5e3cf9943c2d1560eʹ�ù��߼����:6E69B5D79979BB92
			memcpy(PreOut, "\x6E\x69\xB5\xD7\x99\x79\xBB\x92", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}

		//case13:��SEC_MAC_9606/SEC_MAC_RESP_9606ģʽ,32�ֽ���������,DUKPT3
		memset(MacOut, 0, sizeof(MacOut));
		memset(PreOut, 0, sizeof(PreOut));
		if ((ret=NDK_SecGetMacDukpt(3, DataIn, 32, MacOut, KSN1, sec_mac_9606[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
		if(i==0)
			//ID3����Mac��Կ:9b53578c811008c6f3bf8ca3bbff9c58ʹ�ù��߼����:A15F00C8AF304190
			memcpy(PreOut, "\xA1\x5F\x00\xC8\xAF\x30\x41\x90", 8);
		else
			//ID3����Mac RESP��Կ:9b53578c7e10f7c6f3bf8ca344ff6358ʹ�ù��߼����:8DC1B7534F1AC8BF
			memcpy(PreOut, "\x8D\xC1\xB7\x53\x4F\x1A\xC8\xBF", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}
		
		//case16:��SEC_MAC_X919/SEC_MAC_RESP_X919ģʽ,13�ֽ���������,MAC��ʹ��TAKʵ����ID=5��װ�ص���TPK,Υ����Կר����,Ӧʧ��
		if ((ret=NDK_SecGetMacDukpt(5, DataIn, 13, MacOut, KSN1, sec_mac_x9191[i]))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		}
	}

	//case14:��֤GETMAC��,KSN����
	//getksn
	if((ret=NDK_SecGetDukptKsn(2, KSN1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//getmac
	if ((ret=NDK_SecGetMacDukpt(3, DataIn, 13, MacOut, KSN2, SEC_MAC_9606))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//getksn	
	if((ret=NDK_SecGetDukptKsn(2, KSN3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(KSN1, KSN2, 10)||memcmp(KSN1, KSN3, 10))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
#if SEC_RSA_ENABLE
	//case15:��SEC_MAC_X919ģʽ,13�ֽ���������,MAC��ʹ��TAKʵ����ID=4��װ�ص���RSA,Υ����Կר����,Ӧʧ��
	if ((ret=NDK_SecGetMacDukpt(4, DataIn, 13, MacOut, KSN1, SEC_MAC_X919))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case17:KSN�ԼӺ���Ӧ������ǰ����Կ���Զ����£�����KSNҪ�Զ�����
	if ((ret=NDK_SecIncreaseDukptKsn(6))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=0;i<num;i++)
	{
		//�ԼӺ�����ǰ��Կ���:5635b184dd5d8f415617b6600c6e47cf���ɴ�������MAC��Կ5635b184dd5d70415617b6600c6eb8cf���ɴ�������MAC RESPONSE��Կ5635B184225D8F415617B660F36E47CF
		memset(KSN1, 0x00, sizeof(KSN1));
		memset(MacOut, 0, sizeof(MacOut));
		if ((ret=NDK_SecGetMacDukpt(6, DataIn, 8, MacOut, KSN1, sec_mac_x99[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret,i);
			GOTOERR;
		} 
		if(i==0)
			//��Ԥ�ڽ��ʹ��16�ֽ���Կ5635b184dd5d70415617b6600c6eb8cf��8�ֽ�����(2222222222222222)���м��ܼ������3FCBC4E7C2FE492C
			memcpy(PreOut, "\x3F\xCB\xC4\xE7\xC2\xFE\x49\x2C", 8);
		else
			//��Ԥ�ڽ��ʹ��16�ֽ���Կ5635B184225D8F415617B660F36E47CF��8�ֽ�����(2222222222222222)���м��ܼ������D711C99288DA4B5C
			memcpy(PreOut, "\xD7\x11\xC9\x92\x88\xDA\x4B\x5C", 8);
		if(memcmp(MacOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(i=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, i, MacOut[0], MacOut[1], MacOut[2], MacOut[3], MacOut[4], MacOut[5], MacOut[6], MacOut[7]);
			GOTOERR;
		}
		if( KSN1[7]!=0x00 || KSN1[8]!=0x00 || KSN1[9]!=0x02)
		{
			send_result("line %d:%s����ʧ��(KSN1[7]=0x%2x,KSN1[8]=0x%2x,KSN1[9]=0x%2x)", __LINE__, TESTAPI, KSN1[7],KSN1[8],KSN1[9]);
			goto ERR;
		}
	}
	send_result("%s����ͨ��", TESTAPI);
	
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;

}

