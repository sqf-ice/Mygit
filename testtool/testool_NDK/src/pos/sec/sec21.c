/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec7.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecCalcDesDUKPT�ӿڹ���
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
#define TESTAPI	"NDK_SecCalcDesDUKPT"	


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
*						linwl			20150125	modified
*****************************************************************/
void sec21(void)
{
	int ret = 0, i = 0, j=0;
	ST_SEC_KCV_INFO KcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar DataIn[1024+8]={0},KeyValue[17]={0},KSN1[11]={0},KSN2[11]={0},KSN3[11]={0},sIV[9]={0}, DataOut[1024+8]={0}, En_DataOut[1024]={0}, PreOut[1024+8]={0};
#if TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	uchar mkey[17]={0},block[512]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKeyIn;
	uchar *uModeulus1=(uchar *)"4FB2D08918F766B30B2144FE25D09A160FFCF256086D82773FB04D4BD7E4772F2557FC25EE130A0B984467995BE80EE1751F577BD3E5CAFF60712722F33D0479";
#endif
	uchar DukptKeyValue[16]={0xD5,0xD4,0x4F,0xF7,0x20,0x68,0x3D,0x0D,0x70,0xBE,0x25,0x28,0x18,0xE2,0xA1,0x8A};//DUKPY��ʼ������Կ
	uchar KSN[11]={0};//��ʼKSN
#if OVERSEAS_ENABLE
	int num=3;
	int sec_des_mode[3]={0, SEC_DES_MODE_ECB, SEC_DES_MODE_CBC};
	int sec_tdk_key_type[3]={SEC_KEY_TYPE_TDK, SEC_KEY_TYPE_RESP_TDK, SEC_KEY_TYPE_RESP_TDK};
	int sec_tak_key_type[3]={SEC_KEY_TYPE_TAK, SEC_KEY_TYPE_RESP_TAK, SEC_KEY_TYPE_RESP_TAK};
	char en_out_1024[1024]={0x2d,0xc0,0xda,0x58,0xdc,0x03,0xfe,0x24,0x25,0x0d,0x0d,0x38,0x5b,0x7e,0xc5,0x62,0xf7,0x28,0x7a,0xc0,0x95,0xb2,0xba,0xd0,0xa7,0xff,0xfb,0x37,0x46,0x37,0x3c,0xc1,0x6d,0xd4,0x9e,0x8e,0x0c,0x94,0xbb,0x3f,0xe5,0x79,0x03,0x82,0xb0,0x07,0x90,0xb1,0x29,0x70,0x0a,0xfa,0x39,0xfc,0x4e,0xe1,0x0b,0x53,0x4a,0x4e,0x2d,0x72,0x86,0x9b,
							0x09,0x33,0xa3,0x65,0xb7,0x37,0x42,0x9c,0xcf,0x25,0xb9,0x1f,0xa6,0x7e,0xcf,0xcf,0xe6,0xc8,0x5f,0x29,0xee,0x23,0x0f,0x31,0x91,0x95,0x13,0xa4,0x88,0xd5,0xe3,0x1d,0x91,0x81,0x06,0x1f,0x9c,0xcb,0x6c,0x5f,0xe8,0xd7,0x64,0x6c,0x60,0xe0,0x2f,0x8a,0xae,0x61,0x1e,0x77,0x6a,0x43,0x80,0x66,0x42,0xa0,0x02,0x66,0xce,0x17,0x0d,0x32,
							0x35,0x30,0x5a,0xdf,0x4d,0xf5,0xb1,0xdd,0xa9,0x56,0xc3,0x61,0xc4,0x8b,0x4a,0xa3,0x26,0x7f,0xb9,0x61,0x68,0xb1,0x29,0xfc,0x92,0x04,0x68,0x9f,0xff,0xfc,0x68,0x24,0xde,0x1d,0x27,0x13,0x84,0x1c,0x4c,0x8a,0x48,0x5d,0x3f,0xfb,0x61,0xee,0xb2,0xf7,0x9e,0x5c,0x89,0xf9,0x38,0xf6,0x2c,0x82,0xf8,0x3b,0xbc,0x41,0xb5,0x67,0xdc,0x00,
							0x73,0x81,0x46,0x97,0x07,0x5d,0xe4,0x18,0xfb,0xed,0x54,0xac,0x1f,0xaf,0xf1,0x15,0x82,0xc5,0xd4,0x02,0x05,0xcf,0x6d,0xd7,0x22,0x09,0x6c,0x72,0x5b,0x12,0x8f,0x66,0xe4,0xe2,0x10,0xdc,0x22,0xb9,0xd1,0x08,0x81,0x32,0xe6,0x4a,0x4f,0x37,0xdf,0xb7,0x98,0xeb,0x3a,0xfa,0xf6,0x7f,0x30,0x89,0xc6,0x60,0x14,0x51,0x0f,0x59,0x45,0x63,
							0xf6,0x04,0xc8,0x56,0x9d,0x5a,0xa2,0x87,0x7e,0x6f,0x71,0x6a,0x57,0xfe,0x03,0x74,0x04,0x7e,0xb6,0xa0,0x6f,0x98,0x87,0x64,0x99,0x9b,0xd7,0x88,0x38,0x66,0x00,0x83,0xf3,0xfa,0x3f,0x19,0x22,0x6c,0xc7,0xbb,0x0c,0xeb,0xbb,0x41,0xa2,0x0e,0x22,0xe8,0x9c,0xbd,0x3d,0x79,0x13,0x15,0xc7,0x01,0x19,0x91,0x82,0x7d,0x3d,0x85,0x20,0x69,
							0xf4,0x28,0xf6,0xdd,0xab,0x44,0xba,0x53,0x02,0xec,0x5e,0x6a,0x19,0xc5,0x2e,0x53,0xac,0xa0,0xa4,0xa8,0x47,0x2f,0x7a,0x44,0x7f,0x19,0xd5,0x72,0x61,0xd6,0x9b,0xc5,0xcd,0x74,0x7a,0xec,0x9b,0x51,0x97,0x51,0x44,0x08,0x16,0x1c,0x8c,0x73,0xfc,0x31,0x20,0xbe,0x45,0x92,0xb4,0x96,0x4f,0x9c,0xcf,0xfa,0xe5,0x6f,0xa0,0xce,0xca,0x61,
							0xee,0xf4,0x02,0xb1,0x6f,0x70,0xce,0x9e,0x7e,0x44,0x03,0xc6,0xaa,0x33,0x50,0xe4,0xd3,0x3b,0x36,0x52,0xa4,0xc1,0xb0,0x9a,0xf9,0xae,0x70,0x36,0x85,0x09,0xe1,0x3f,0xc2,0x4c,0xdf,0x9a,0xbf,0x85,0xbd,0x04,0x6f,0x3b,0xb3,0xa8,0x86,0x50,0x10,0x43,0xbe,0xd8,0x4f,0x4a,0x14,0x21,0x00,0xf4,0xa5,0x17,0x4c,0xf0,0x61,0xe7,0x32,0xca,
							0x8b,0x3f,0xef,0x8a,0xa0,0x14,0x1c,0x28,0x9e,0xc2,0xfe,0x1f,0x12,0x5e,0xaf,0x77,0xe0,0x89,0xb1,0x21,0x45,0x61,0x36,0x53,0xf2,0xab,0x5f,0xb3,0xe0,0x08,0x95,0x0c,0x24,0xe9,0x1e,0x1f,0xd2,0x95,0xf7,0xd7,0x57,0x7a,0xe3,0x62,0x39,0xbe,0x45,0x0f,0x4d,0x26,0x6a,0xa0,0x8a,0xbe,0xe9,0xca,0x3d,0x2f,0x15,0x22,0x87,0x0a,0x64,0x35,
							0x0d,0x65,0x2c,0xba,0xa2,0xfd,0x06,0xe0,0xff,0xf9,0xad,0xd7,0x0a,0x42,0xd0,0xc5,0x87,0x44,0x80,0x7f,0x98,0x05,0x1d,0x56,0xef,0xf6,0x47,0x27,0xa6,0xca,0xca,0xa2,0xa3,0x54,0x13,0x60,0x5d,0x8c,0x0a,0x74,0x27,0xdc,0x8b,0x61,0x32,0x22,0x15,0xd0,0x46,0xae,0x72,0x13,0x9c,0x20,0xb7,0xe9,0x15,0x00,0x0f,0x0f,0xd0,0x21,0x2a,0xd3,
							0xd7,0x99,0xa6,0xfb,0x91,0x0c,0xb2,0xb5,0x40,0x2b,0x9f,0xfc,0xaa,0x91,0x9b,0x3d,0x09,0x67,0xdd,0x6f,0xd9,0xa6,0xb6,0xd8,0x9d,0x39,0x57,0x4a,0xd7,0xc1,0xcc,0x02,0xae,0xdc,0x1e,0xab,0xab,0xbf,0x43,0xac,0xf4,0x73,0xe5,0xb2,0x45,0xdf,0xe2,0x60,0xa4,0x5a,0xf8,0x8b,0x0e,0xb8,0xc5,0x5c,0xc9,0x90,0x5f,0xe1,0xf5,0x35,0xfa,0x48,
							0x21,0x52,0x1b,0x64,0x05,0x2b,0x3e,0xb2,0xd2,0x1a,0x27,0x97,0x9f,0xa5,0xd0,0x37,0x03,0xd3,0xb4,0xa0,0xa9,0x68,0x0e,0x0e,0xff,0x00,0x4c,0x8e,0x57,0xd8,0x2d,0xa8,0x6c,0x8b,0x4d,0x35,0x25,0x8c,0xb0,0x5b,0x58,0xf5,0xf5,0x24,0xad,0xfe,0x9d,0x49,0x4b,0x74,0x67,0xb0,0x68,0x75,0x88,0xc9,0x6d,0xe9,0x24,0xdd,0x98,0xff,0xaa,0xa7,
							0xda,0x5a,0x5e,0xeb,0x69,0xbd,0x5c,0xb7,0xc7,0x53,0x17,0x18,0x75,0x29,0x14,0xec,0xc4,0x56,0x3f,0x6e,0xed,0xc5,0x56,0x12,0xa5,0xe5,0x2d,0x86,0x7c,0xe8,0xc2,0x81,0x86,0x4d,0xe1,0xe4,0xf5,0xe3,0x7f,0x8e,0xba,0x3d,0xc0,0xe7,0xaa,0xaa,0xed,0xc2,0x7e,0xb0,0x30,0x2b,0x4f,0x57,0x0f,0x28,0x5f,0xdb,0xb2,0x59,0x98,0x03,0x02,0x4e,
							0x2a,0x64,0xcf,0xf9,0x73,0x12,0xea,0x49,0x7d,0x24,0x8a,0xc3,0x52,0x65,0x4e,0x60,0x51,0x26,0xc5,0x5e,0xc9,0x6d,0x6e,0xb9,0xf7,0x8c,0x13,0x0c,0x56,0xa6,0x3c,0x2f,0xf2,0x59,0x9f,0x70,0x93,0x4c,0x07,0xee,0x59,0xf2,0xdb,0xdb,0xa7,0x1f,0x06,0xbe,0x2d,0x8b,0x22,0xc0,0xea,0x6f,0x5d,0xef,0x7d,0xaf,0x6c,0x19,0x60,0x78,0x41,0x2c,
							0xb4,0x0d,0x11,0x22,0xca,0xb7,0x28,0xe3,0x2c,0x88,0xb0,0xa8,0x0f,0x8f,0x36,0xc7,0x46,0x9c,0xac,0x9d,0x69,0x7e,0x42,0xe1,0x37,0x2c,0xbb,0x0a,0x26,0x4d,0x6e,0x1d,0xca,0xc7,0xcd,0x23,0x4c,0xfe,0x7f,0x60,0xb1,0xed,0xd3,0x97,0xc4,0xa2,0xd8,0xa7,0x0e,0xd2,0x7c,0x2c,0xa1,0x3a,0xff,0x5c,0xcc,0x07,0x7e,0x23,0x02,0xa2,0x5e,0xdc,
							0xf7,0x13,0x35,0x9e,0x9e,0x64,0x74,0xc4,0x63,0xce,0x22,0x06,0x14,0xaa,0x73,0x9e,0x22,0x6b,0xce,0x08,0x40,0x4c,0x34,0xb2,0x6d,0x20,0x87,0xac,0x4f,0x2f,0xa0,0x34,0xe8,0x99,0xc8,0xfb,0x42,0x85,0xb0,0xfb,0x29,0xcf,0x74,0x76,0xc8,0xd2,0xfc,0x8d,0x5c,0x1d,0x21,0xe5,0x1b,0x2e,0xd8,0x04,0x4e,0x75,0x50,0x8d,0x93,0x8f,0x04,0x6c,
							0x14,0x53,0x07,0xec,0x06,0x71,0xbf,0xd7,0xbf,0xbc,0xe9,0xce,0x1c,0x80,0xea,0x1f,0x24,0x8f,0x5d,0x74,0x47,0x03,0x17,0xd5,0x72,0xd2,0xa6,0x40,0xb5,0x17,0x96,0x7e,0x92,0x19,0x4c,0x28,0xf5,0x02,0x60,0xa0,0xd2,0xd5,0xd8,0x46,0xcb,0x4a,0xa3,0x6e,0xa4,0x1d,0x4c,0x1d,0xd7,0x19,0x0e,0x18,0xb5,0x5f,0x09,0x0f,0xeb,0x1c,0x7d,0xc2,};
#else
	int num=1;
	int sec_des_mode[1]={0};
	int sec_tdk_key_type[1]={SEC_KEY_TYPE_TDK};
	int sec_tak_key_type[1]={SEC_KEY_TYPE_TAK};
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
	case SEC_KEY_TYPE_TDK:		//�������ݼӽ���KEY 6ef92795e5ed714dcdeb767b330ea9bd    ���в�Ʒ½������:��Ҫ���Լ������ݺ���Կ����3DES����  BFA9CA10BF791459 482339A090BF7CC9 Ϊʵ����Կ
		key[5] ^= 0xff;
		key[13] ^= 0xff;
	case SEC_KEY_TYPE_RESP_TAK: //����MAC RESPONSE  6EF927951A12714DCDEB767BCCF1A9BD    
		key[4] ^= 0xff;
		key[12] ^= 0xff;
	case SEC_KEY_TYPE_RESP_TDK: //�������ݼӽ���KEY RESPONSE    6EF9276AE512714DCDEB768433F1A9BD  ����ƽ̨��Ʒ��Ҫ���Լ������ݺ���Կ����3DES���� B3EBB63165DC301B B0A878B8686A67C6 Ϊʵ����Կ
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

	//���İ�װһ��DUKPT, ID=6, KSNΪ10���ֽ�0x00 /*Լ��:����KCV���Բ�У�鷽ʽ��װ*/
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);//��ʼKSN
	if ((ret=NDK_SecLoadTIK(6, 0, 16, DukptKeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTLK, ID=1,����Կ�������İ�װ��ID=2��DUKPT��Կ
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
	memcpy(KeyValue, "\xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23", 16);
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

	//���İ�װTDK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x21, stKeyInfo.nDstKeyLen);
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &KcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//Լ��:��������˵��sIVȡȫ��(��NULL)
	memset(sIV, 0x00, sizeof(sIV));//��ʼ��������������չ���Ժ����ʵ��֧��cbc����ģʽ�ȣ�Ŀǰ��Ч

	//case1:�Ƿ�����
	//case1.1:psDataIn==NULL��Ӧ�÷���NDK_ERR_PARA
	memset(DataIn, 0x33, sizeof(DataIn));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, sIV, 8, NULL, DataOut, KSN1, SEC_DES_ENCRYPT/*|SEC_DES_KEYLEN_8|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.2:psDataOut==NULL��Ӧ�÷���NDK_ERR_PARA
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, NULL, KSN1, SEC_DES_ENCRYPT/*|SEC_DES_KEYLEN_8|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.3:ucMod �Ƿ�// ��������ucModͨ���жϱ���λ,�ʲ����ڷǷ�ֵ
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, DataOut, KSN1, 100))!=NDK_OK/*NDK_ERR_PARA*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.4:uckeyIdx������(10,101)
	if((ret=NDK_SecCalcDesDukpt(10,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR
	}
	if((ret=NDK_SecCalcDesDukpt(101,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:nDataInLen��8������(nDataInLen=5)
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, sIV, 5, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//GOTOERR;
	}

	//case1.6:psDataOut=NULL
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, NULL, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.7:psKsnOut=NULL
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, DataOut, NULL, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if ANDIROD_ENABLE
	//case1.8:��������psDataIn����1024ʱ���ش���	//20160531����N900 3G�汾 V1.1.16��������
	//memset(DataIn, 5, sizeof(DataIn));
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TDK, NULL, 1024+8, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	/**************************
	SEC_DES_MODE_ECBģʽ����ԭ��:
	����������8�ֽ�Ϊһ��,��Կ�����Ŀ�0��des���ܵõ����Ŀ�0,��Կ�����¸����Ŀ�x�ֱ���des���ܵõ���Ӧ�����Ŀ�x��,�����Ŀ�����������õ���Ӧ������
	SEC_DES_MODE_CBCģʽ����ԭ��:
	����������8�ֽ�Ϊһ��,��һ��IV�����Ŀ�0����Ľ��������Կ��des���ܵõ����Ŀ�0,�õ���������������¸����Ŀ�x����Ľ������Կ��des���ܵõ���Ӧ�����Ŀ�x,�����Ŀ�����������õ���Ӧ������
	**************************/
	for(j=0; j<num; j++)
	{
		//case2.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,DUKPT6,psIV=NULL
		//case2.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,DUKPT6,psIV=NULL
		//case2.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_MODE_CBCģʽ, 8�ֽ���������,DUKPT6,psIV=NULL
		/***�������쳣DUKPT��Կ����16���ȵģ���ʵ�ʴ���SEC_DES_KEYLEN_8��ʹ��8�ֽڽ���DES���У������ط��������⣬�������Ͳ��޸�**/
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(6, sec_tdk_key_type[j] , NULL, 8, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|sec_des_mode[j]))!=NDK_OK)  
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM) //ME 31s�����°�ȫ��������� 20180511 modify //ME30S��Ȼ�Ǻ����Ʒ����֧��SEC_KEY_TYPE_RESP_TDK��SEC_KEY_TYPE_RESP_TAK
			//ʹ��8�ֽ���Կ6ef92795e5ed714d(����Կ��16�ֽ�DUKPT��Կ��ǰ8�ֽ���Կ���)��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����
			memcpy(PreOut,"\xE4\x87\xC1\x16\xA5\x48\xB9\x2C", 8);
#else
			//ʹ��8�ֽ���ԿBFA9CA10BF791459(����Կ��16�ֽ�DUKPT��Կ��ǰ8�ֽ���Կ���)��8�ֽ�����3333333333333333����des���ܺ�ƴ�ӵõ����0C547B0E8F24275F
			memcpy(PreOut,"\x0C\x54\x7B\x0E\x8F\x24\x27\x5F", 8);
#endif
		}
		else if(j==1 || j==2)
			//��IVΪNULLʱ,��������Ϊ8�ֽ�ʱ,SEC_DES_MODE_ECB��SEC_DES_MODE_CBCģʽ�£����ܽ��һ��
			//��SEC_DES_MODE_ECB/SEC_DES_MODE_CBCģʽ,ʹ��8�ֽ���ԿB3EBB63165DC301B(����Կ��16�ֽ��������ݼӽ���KEY��ǰ8�ֽ���Կ���)��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����
			memcpy(PreOut,"\x7C\xA5\x3C\x79\x1E\xD2\xF4\x6F", 8);
		if(memcmp(DataOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7]);
			GOTOERR;
		}

		//case3.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT6,psIV=8��0x00
		//case3.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT6,psIV=8��0x00
		//case3.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_MODE_CBCģʽ, 16�ֽ���������,DUKPT6,psIV=8��0x00
		memset(sIV, 0x00, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(6,sec_tdk_key_type[j], sIV, 16, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|sec_des_mode[j]))!=NDK_OK)  //linrq NDK_OK
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//��Ԥ�ڽ��ʹ��16�ֽ���Կ6ef92795e5ed714dcdeb767b330ea9bd��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\xC6\xDB\x29\xAB\xCB\x5D\x0C\x32\xC6\xDB\x29\xAB\xCB\x5D\x0C\x32", 16);
#else
			//��Ԥ�ڽ��ʹ��16�ֽ���ԿBFA9CA10BF791459482339A090BF7CC9��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�16�ֽڽ��60DEF45C79ED5B06
			memcpy(PreOut,"\x60\xDE\xF4\x5C\x79\xED\x5B\x06\x60\xDE\xF4\x5C\x79\xED\x5B\x06", 16);
#endif
		}
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,ʹ��16�ֽ��������ݼӽ���KEY:B3EBB63165DC301B B0A878B8686A67C6��16�ֽ����ݷ�����8�ֽ�(3333333333333333)����3des���ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\x85\x95\x78\xE2\xBD\x3C\xA6\x6A\x85\x95\x78\xE2\xBD\x3C\xA6\x6A", 16);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��16�ֽ��������ݼӽ���KEY:B3EBB63165DC301B B0A878B8686A67C6��16�ֽ�����(0x33)ǰ���ֽ���3des���ܵõ�859578E2BD3CA66A�����ܽ�������ֽ����������õ�B6A64BD18E0F9559��������Կ��3des���㣬�õ����ܽ��
			memcpy(PreOut,"\x85\x95\x78\xE2\xBD\x3C\xA6\x6A\x99\x67\xBD\x7C\x10\x63\x51\xEF", 16);
		if(memcmp(DataOut, PreOut, 16))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15]);
			GOTOERR;
		}

		//case4.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ, 24�ֽ���������,DUKPT6,psIV=��0ֵ
		//case4.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ, 24�ֽ���������,DUKPT6,psIV=��0ֵ
		//case4.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_MODE_CBCģʽ, 24�ֽ���������,DUKPT6,psIV=��0ֵ
		memset(sIV, 0x12, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(6,sec_tdk_key_type[j], sIV, 24, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//ʹ��24�ֽ���Կ6ef92795e5ed714dcdeb767b330ea9bd6ef92795e5ed714d(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��3��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����
			memcpy(PreOut,"\xC6\xDB\x29\xAB\xCB\x5D\x0C\x32\xC6\xDB\x29\xAB\xCB\x5D\x0C\x32\xC6\xDB\x29\xAB\xCB\x5D\x0C\x32", 24);
#else
			//ʹ��24�ֽ���ԿBFA9CA10BF791459482339A090BF7CC9BFA9CA10BF791459(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��3��8�ֽ�����3333333333333333���м��ܺ�ƴ�ӵõ����
			memcpy(PreOut,"\x60\xDE\xF4\x5C\x79\xED\x5B\x06\x60\xDE\xF4\x5C\x79\xED\x5B\x06\x60\xDE\xF4\x5C\x79\xED\x5B\x06", 24);
#endif
		}
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,ʹ��24���������ݼӽ���KEY:B3EBB63165DC301B B0A878B8686A67C6(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��3��8�ֽ�����3333333333333333����3des���ܼ����ƴ�ӳɽ��
			memcpy(PreOut,"\x85\x95\x78\xE2\xBD\x3C\xA6\x6A\x85\x95\x78\xE2\xBD\x3C\xA6\x6A\x85\x95\x78\xE2\xBD\x3C\xA6\x6A", 24);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��24���������ݼӽ���KEY:B3EBB63165DC301B B0A878B8686A67C6(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��24�ֽ�����(0x33)ǰ���ֽ���IV���������3DES����F0A8F0600FAF7F89�����ܽ�����м�8�ֽ����������ټ���B0B7D4B2F14830C0�����ܽ���������8�ֽ����������ټ���3D7AC8138C7D83D4
			memcpy(PreOut,"\xF0\xA8\xF0\x60\x0F\xAF\x7F\x89\xB0\xB7\xD4\xB2\xF1\x48\x30\xC0\x3D\x7A\xC8\x13\x8C\x7D\x83\xD4", 24);
		if(memcmp(DataOut, PreOut, 24))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15], DataOut[16], DataOut[17], DataOut[18], DataOut[19], DataOut[20], DataOut[21], DataOut[22], DataOut[23]);
			GOTOERR; 
		}

		//case18.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 1024�ֽ���������,DUKPT6,psIV=��0ֵ,Ϊ���Ը߶�AZƽ̨�����ݼ��ܶ����ӵ�1024�ֽ����ݼ��ܲ���20170220
		//case18.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 1024�ֽ���������,DUKPT6,psIV=��0ֵ
		//case18.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_MODE_CBCģʽ, 1024�ֽ���������,DUKPT6,psIV=��0ֵ
		memset(sIV, 0x14, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(En_DataOut, 0, sizeof(En_DataOut));
		if((ret=NDK_SecCalcDesDukpt(6,sec_tdk_key_type[j], sIV, 1024, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		for(i=0;i<128;i++)
		{
			if(j==0)					
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
				//��Ԥ�ڽ��ʹ��16�ֽ���Կ6ef92795e5ed714dcdeb767b330ea9bd��1024�ֽ����ݷ�128��8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�1024�ֽڽ��
				memcpy(En_DataOut+i*8, "\xC6\xDB\x29\xAB\xCB\x5D\x0C\x32", 8);
#else
				//��Ԥ�ڽ��ʹ��16�ֽ���ԿBFA9CA10BF791459482339A090BF7CC9��1024�ֽ����ݷ�128��8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�1024�ֽڽ��
				memcpy(En_DataOut+i*8, "\x60\xDE\xF4\x5C\x79\xED\x5B\x06", 8);
#endif
			else if(j==1)
				//��SEC_DES_MODE_ECBģʽ,ʹ��16���������ݼӽ���KEY:B3EBB63165DC301B B0A878B8686A67C6��1024�ֽ�����(128��8�ֽ�����3333333333333333)����3des���ܼ����ƴ�ӳɽ��
				memcpy(En_DataOut+i*8, "\x85\x95\x78\xE2\xBD\x3C\xA6\x6A", 8);
		}
#if OVERSEAS_ENABLE
		if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��16���������ݼӽ���KEY:6EF9276AE512714DCDEB768433F1A9BD��1024�ֽ�����(128��8�ֽ�����3333333333333333)���м��ܼ����ƴ�ӳɽ��
			memcpy(En_DataOut, en_out_1024, 1024);
#endif
		if(memcmp(DataOut,En_DataOut, 1024))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			//GOTOERR;
		}

		//case5.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,DUKPT2,psIV=NULL
		//case5.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,DUKPT2,psIV=NULL
		//case5.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_8|SEC_DES_MODE_CBCģʽ, 8�ֽ���������,DUKPT2,psIV=NULL
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(2,sec_tdk_key_type[j], NULL, 8, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_8|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
			//id2 ����TDK:bae698c215fe8dd5e3cf99433d2e560e ����respTDK:bae6983d15018dd5e3cf99bc3dd1560e
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//��Ԥ�ڽ��ʹ��8�ֽ���Կbae698c215fe8dd5��8�ֽ�����3333333333333333�ڼӽ��ܹ���\PosDebug���߽��ܵó�1B732966E2E8CC15
			memcpy(PreOut,"\x1B\x73\x29\x66\xE2\xE8\xCC\x15", 8);
#else	
			//id2��tdk�������Լ����ܵ�:7C878556EE105C36 0E4784D9303A432E  
			//��Ԥ�ڽ��ʹ��8�ֽ���Կ7C878556EE105C36��8�ֽ�����3333333333333333�ڼӽ��ܹ���\PosDebug���߽��ܵó�A26910A96DE82334
			memcpy(PreOut,"\xA2\x69\x10\xA9\x6D\xE8\x23\x34", 8);
#endif
		}
		else if(j==1 || j==2)
			//respTDK�Լ����Լ����ܺ��:CE69D74207D3FCBC EE5A3F0E73FF9EC2
			//��SEC_DES_MODE_ECB/SEC_DES_MODE_CBCģʽ,ʹ��8�ֽ���ԿCE69D74207D3FCBC(����Կ��16�ֽ��������ݼӽ���KEY��ǰ8�ֽ���Կ���)��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����48B84BE0F1D66133
			memcpy(PreOut,"\x48\xB8\x4B\xE0\xF1\xD6\x61\x33", 8);
		if(memcmp(DataOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7]);
			GOTOERR;
		}
	
		//case6.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT2,psIV=8��0x00
		//case6.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT2,psIV=8��0x00
		//case6.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_MODE_CBCģʽ, 16�ֽ���������,DUKPT2,psIV=8��0x00
		memset(sIV, 0x00, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));		
		if((ret=NDK_SecCalcDesDukpt(2,sec_tdk_key_type[j], sIV, 16, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//��Ԥ�ڽ��ʹ��16�ֽ���Կbae698c215fe8dd5e3cf99433d2e560e��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���н��ܼ����ƴ�ӳ�16�ֽڽ��5E4774EB6BA03BD35E4774EB6BA03BD3
			memcpy(PreOut,"\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3", 16);
#else
			//��Ԥ�ڽ��ʹ��16�ֽ���Կ7C878556EE105C36 0E4784D9303A432E��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���н��ܼ����ƴ�ӳ�16�ֽڽ��A35E54145BBD3063A35E54145BBD3063
			memcpy(PreOut,"\xA3\x5E\x54\x14\x5B\xBD\x30\x63\xA3\x5E\x54\x14\x5B\xBD\x30\x63", 16);
#endif
		}
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,ʹ��16�ֽ��������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2��16�ֽ����ݷ�����8�ֽ�(3333333333333333)����3des���ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\x94\x4F\x5C\xD5\x22\xE5\xD1\x33\x94\x4F\x5C\xD5\x22\xE5\xD1\x33", 16);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��16�ֽ��������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2��16�ֽ�����(0x33)ǰ���ֽ���3des���ܵõ�944F5CD522E5D133�����ܽ�������ֽ����������õ�A77C6FE611D6E200
			memcpy(PreOut,"\x94\x4F\x5C\xD5\x22\xE5\xD1\x33\xA7\x7C\x6F\xE6\x11\xD6\xE2\x00", 16);
		if(memcmp(DataOut, PreOut, 16))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15]);
			GOTOERR;
		}
		
		//case7.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT2,psIV=8��0x00	
		//case7.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT2,psIV=8��0x00
		//case7.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_MODE_CBCģʽ, 16�ֽ���������,DUKPT2,psIV=8��0x00
		memset(sIV, 0x00, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(2,sec_tdk_key_type[j], sIV, 16, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//��Ԥ�ڽ��ʹ��16�ֽ���Կbae698c215fe8dd5e3cf99433d2e560e��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���н��ܼ����ƴ�ӳ�16�ֽڽ��5E4774EB6BA03BD35E4774EB6BA03BD3
			memcpy(PreOut,"\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3", 16);
#else
			//��Ԥ�ڽ��ʹ��16�ֽ���Կ7C878556EE105C36 0E4784D9303A432E��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���н��ܼ����ƴ�ӳ�16�ֽڽ��A35E54145BBD3063A35E54145BBD3063
			memcpy(PreOut,"\xA3\x5E\x54\x14\x5B\xBD\x30\x63\xA3\x5E\x54\x14\x5B\xBD\x30\x63", 16);
#endif
		}
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,ʹ��16�ֽ��������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2��16�ֽ����ݷ�����8�ֽ�(3333333333333333)����3des���ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\x94\x4F\x5C\xD5\x22\xE5\xD1\x33\x94\x4F\x5C\xD5\x22\xE5\xD1\x33", 16);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��16�ֽ��������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2��16�ֽ�����(0x33)ǰ���ֽ���3des���ܵõ�944F5CD522E5D133�����ܽ�������ֽ����������õ�A77C6FE611D6E200
			memcpy(PreOut,"\x94\x4F\x5C\xD5\x22\xE5\xD1\x33\xA7\x7C\x6F\xE6\x11\xD6\xE2\x00", 16);
		if(memcmp(DataOut, PreOut, 16))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15]);
			GOTOERR;
		}
	
		//case8.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_24|SEC_DES_CBCMODEģʽ, 24�ֽ���������,DUKPT2,psIV=��0ֵ
		//case8.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ, 24�ֽ���������,DUKPT2,psIV=��0ֵ
		//case8.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_24|SEC_DES_MODE_CBCģʽ, 24�ֽ���������,DUKPT2,psIV=��0ֵ
		memset(sIV, 0x12, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(2,sec_tdk_key_type[j], sIV, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_24|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//ʹ��24�ֽ���Կbae698c215fe8dd5e3cf99433d2e560ebae698c215fe8dd5(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��3��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����
			memcpy(PreOut,"\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3", 24);
#else
			//ʹ��24�ֽ���Կ7C878556EE105C360E4784D9303A432E7C878556EE105C36(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��3��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����
			memcpy(PreOut,"\xA3\x5E\x54\x14\x5B\xBD\x30\x63\xA3\x5E\x54\x14\x5B\xBD\x30\x63\xA3\x5E\x54\x14\x5B\xBD\x30\x63", 24);
#endif
		}
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,ʹ��24���������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)��3��8�ֽ�����3333333333333333����3des���ܼ����ƴ�ӳɽ��
			memcpy(PreOut,"\x94\x4F\x5C\xD5\x22\xE5\xD1\x33\x94\x4F\x5C\xD5\x22\xE5\xD1\x33\x94\x4F\x5C\xD5\x22\xE5\xD1\x33", 24);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��24���������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2(����Կ��16�ֽ�DUKPT��Կ+ǰ8�ֽ���Կ���)�Ե�һ�����Ŀ�(24�ֽ�����(0x33)ǰ���ֽ�)��3des���ܣ����ܽ����IV������865D4EC730F7C321����Կ�Եڶ������Ŀ�(�м�˸��ֽ�0x33)���������һ�����Ŀ�(ǰ�˸��ֽ�0x33)���A77C6FE611D6E200����Կ�Ե��������Ŀ�(��˸��ֽ�0x33)��������ڶ������Ŀ�(�м�˸��ֽ�0x33)���A77C6FE611D6E200��
			memcpy(PreOut,"\x86\x5D\x4E\xC7\x30\xF7\xC3\x21\xA7\x7C\x6F\xE6\x11\xD6\xE2\x00\xA7\x7C\x6F\xE6\x11\xD6\xE2\x00", 24);
		if(memcmp(DataOut, PreOut, 24))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15], DataOut[16], DataOut[17], DataOut[18], DataOut[19], DataOut[20], DataOut[21], DataOut[22], DataOut[23]);
			GOTOERR;
		}
	
		//case19.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_CBCMODEģʽ, 1024�ֽ���������,DUKPT2,psIV=8��0x00,Ϊ���Ը߶�AZƽ̨�����ݽ����ܶ����ӵ�1024�ֽ����ݽ��ܲ���20170220
		//case19.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 1024�ֽ���������,DUKPT6,psIV=8��0x00
		//case19.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_MODE_CBCģʽ, 1024�ֽ���������,DUKPT6,psIV=8��0x00
		memset(sIV, 0x00, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(En_DataOut, 0, sizeof(En_DataOut));
		if((ret=NDK_SecCalcDesDukpt(2,sec_tdk_key_type[j], sIV, 1024, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		for(i=0;i<128;i++)
			if(j==0)
			{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
				//��Ԥ�ڽ��ʹ��16�ֽ���Կbae698c215fe8dd5e3cf99433d2e560e��1024�ֽ����ݷ�128��8�ֽ�(3333333333333333)���н��ܼ����ƴ�ӳ�1024�ֽڽ��5E4774EB6BA03BD3 128��
				memcpy(En_DataOut+i*8, "\x5E\x47\x74\xEB\x6B\xA0\x3B\xD3", 8);
#else
				//��Ԥ�ڽ��ʹ��16�ֽ���Կ7C878556EE105C36 0E4784D9303A432E��1024�ֽ����ݷ�128��8�ֽ�(3333333333333333)���н��ܼ����ƴ�ӳ�1024�ֽڽ��
				memcpy(En_DataOut+i*8, "\xA3\x5E\x54\x14\x5B\xBD\x30\x63", 8);
#endif
			}				
			else if(j==1)
				//��SEC_DES_MODE_ECBģʽ,ʹ��16���������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2��1024�ֽ�����(128��8�ֽ�����3333333333333333)����3des���ܼ����ƴ�ӳɽ��
				memcpy(En_DataOut+i*8, "\x94\x4F\x5C\xD5\x22\xE5\xD1\x33", 8);
		if(j==2)
		{	
			//��SEC_DES_MODE_CBCģʽ,ʹ��16���������ݼӽ���KEY:CE69D74207D3FCBC EE5A3F0E73FF9EC2��1024�ֽ����ݷ�128��8�ֽ�(3333333333333333)���н��ܣ���Կ���һ�����Ŀ�(ǰ���ֽ�)��3des����Ϊ944F5CD522E5D133����Կ�Եڶ������Ŀ�(��һ���˸��ֽ�0x33)���������һ�����Ŀ�(ǰ�˸��ֽ�0x33)���A77C6FE611D6E200����Կ�Ե��������Ŀ�(��һ���˸��ֽ�0x33)��������ڶ������Ŀ�(�м�˸��ֽ�0x33)���A77C6FE611D6E200,���ѭ��
			memcpy(En_DataOut, "\x94\x4F\x5C\xD5\x22\xE5\xD1\x33", 8);
			for(i=1;i<128;i++)
				memcpy(En_DataOut+i*8, "\xA7\x7C\x6F\xE6\x11\xD6\xE2\x00", 8);
		}
		if(memcmp(DataOut, En_DataOut, 1024))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
			
		//case9:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,DUKPT3,psIV=NULL
		//case9.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,DUKPT3,psIV=NULL
		//case9.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_MODE_CBCģʽ, 8�ֽ���������,DUKPT3,psIV=NULL
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(3,sec_tdk_key_type[j], sIV, 8, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
		//id3������tdk:9b53578c81eff7c6f3bf8ca3bb006358  ����resp TDK:9b5357738110f7c6f3bf8c5cbbff6358 ʹ��8�ֽ���Կ���м���
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			memcpy(PreOut,"\xC8\x90\x4F\xFC\x51\x31\xA7\x12", 8);
#else
		//tdk�Լ����Լ����ܺ��:5554FBAAC1171CCBC4443DAED3F2AC2C
			memcpy(PreOut,"\x4B\xB0\x49\xD8\xF1\x13\x18\x26", 8);
#endif
		}
		//resp tdk�Լ����Լ����ܺ��:33B498402A9C1DF5796200121F73183F
		else if(j==1||j==2)
			memcpy(PreOut,"\x28\x1E\x6F\x28\xE9\x27\xC7\xD7", 8);
		if(memcmp(DataOut, PreOut, 8))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7]);
			GOTOERR;
		}
	
		//case10.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ,16�ֽ���������,DUKPT3,psIV=8��0x00
		//case10.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 16�ֽ���������,DUKPT3,psIV=8��0x00
		//case10.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_MODE_CBCģʽ, 16�ֽ���������,DUKPT3,psIV=8��0x00
		memset(sIV, 0x00, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(3,sec_tdk_key_type[j], sIV, 16, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
			//id3������tdk:9b53578c81eff7c6f3bf8ca3bb006358  ����resp TDK:9b5357738110f7c6f3bf8c5cbbff6358 
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			memcpy(PreOut,"\xD7\xD8\x41\x7C\xCB\xD9\xB2\x9A\xD7\xD8\x41\x7C\xCB\xD9\xB2\x9A", 16);
#else
			//5554FBAAC1171CCBC4443DAED3F2AC2C
			memcpy(PreOut,"\x2C\x32\xD6\xE7\xB8\x98\x49\x64\x2C\x32\xD6\xE7\xB8\x98\x49\x64", 16);
#endif
		}
		//respTDK:33B498402A9C1DF5796200121F73183F ��3333333333333333���ܵ�60792F7B9851A7C1 Ȼ��ƴ��
		else if(j==1)
			memcpy(PreOut,"\x60\x79\x2F\x7B\x98\x51\xA7\xC1\x60\x79\x2F\x7B\x98\x51\xA7\xC1", 16);
		else if(j==2)
		//33B498402A9C1DF5796200121F73183F ��3333333333333333���ܵ�60792F7B9851A7C1 Ȼ�������ֽ����Ľ��������Կ���ܵ�0C97B5747942DA68
			memcpy(PreOut,"\x60\x79\x2F\x7B\x98\x51\xA7\xC1\x0C\x97\xB5\x74\x79\x42\xDA\x68", 16);
		if(memcmp(DataOut, PreOut, 16))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15]);
			GOTOERR;
		}
		
		//case11.1:��Կ����ΪSEC_KEY_TYPE_TDK,�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ,24�ֽ���������,DUKPT3,psIV=��0ֵ
		//case11.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ, 24�ֽ���������,DUKPT3,psIV=��0ֵ
		//case11.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TDK, �� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_MODE_CBCģʽ, 24�ֽ���������,DUKPT3,psIV=��0ֵ
		memset(sIV, 0x12, sizeof(sIV));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(3,sec_tdk_key_type[j], sIV, 24, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
			//id3������tdk:9b53578c81eff7c6f3bf8ca3bb0063589b53578c81eff7c6  ��3333333333333333���ܵ�D7D8417CCBD9B29A Ȼ��ƴ��
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			memcpy(PreOut,"\xD7\xD8\x41\x7C\xCB\xD9\xB2\x9A\xD7\xD8\x41\x7C\xCB\xD9\xB2\x9A\xD7\xD8\x41\x7C\xCB\xD9\xB2\x9A", 24);
#else
			//5554FBAAC1171CCBC4443DAED3F2AC2C5554FBAAC1171CCB ��3333333333333333���ܵ�2C32D6E7B8984964 Ȼ��ƴ��
			memcpy(PreOut,"\x2C\x32\xD6\xE7\xB8\x98\x49\x64\x2C\x32\xD6\xE7\xB8\x98\x49\x64\x2C\x32\xD6\xE7\xB8\x98\x49\x64", 24);
#endif
		}
		else if(j==1)
			//33B498402A9C1DF5796200121F73183F33B498402A9C1DF5 ��3333333333333333���ܵ�60792F7B9851A7C1Ȼ��ƴ��
			memcpy(PreOut,"\x60\x79\x2F\x7B\x98\x51\xA7\xC1\x60\x79\x2F\x7B\x98\x51\xA7\xC1\x60\x79\x2F\x7B\x98\x51\xA7\xC1", 24);
		else if(j==2)
			//3333333333333333��IV��������Ľ����33B498402A9C1DF5796200121F73183F33B498402A9C1DF5 ��3des���ܵ�405CF4A0072A4F3A,���ܽ�������Ŀ�2����������Կ���м��ܵ�42F9E19ABF90E842�����������������������������Կ���ܵ�18B57A5D1E96514C ���ѭ��
			memcpy(PreOut,"\x40\x5C\xF4\xA0\x07\x2A\x4F\x3A\x42\xF9\xE1\x9A\xBF\x90\xE8\x42\x18\xB5\x7A\x5D\x1E\x96\x51\x4C ", 24);
		if(memcmp(DataOut, PreOut, 24))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15], DataOut[16], DataOut[17], DataOut[18], DataOut[19], DataOut[20], DataOut[21], DataOut[22], DataOut[23]);
			GOTOERR;
		}

		//case12.1:��Կ����ΪSEC_KEY_TYPE_TAK,��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_ECBMODEģʽ,ucKeyIdx=DUKPT1,ucKeyType=TAK,Ӧ�ɹ�(���Ź���),psIV=NULL
		//case12.2:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TAK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_ECBMODEģʽ,ucKeyIdx=DUKPT1,ucKeyType=TAK,Ӧ�ɹ�(���Ź���),psIV=NULL
		//case12.3:(�����Ʒ)��Կ����ΪSEC_KEY_TYPE_RESP_TAK, �� SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_MODE_CBCģʽ,ucKeyIdx=DUKPT1,ucKeyType=TAK,Ӧ�ɹ�(���Ź���),psIV=NULL
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(6,sec_tak_key_type[j], NULL, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|sec_des_mode[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
			//ʹ�ð�װ��16�ֽ�DUKPT��Կ������MAC��Կ6ef92795e5128e4dcdeb767b33f156bd��Կ��3��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����15066CFCCBAE36FF
			memcpy(PreOut,"\x15\x06\x6C\xFC\xCB\xAE\x36\xFF\x15\x06\x6C\xFC\xCB\xAE\x36\xFF\x15\x06\x6C\xFC\xCB\xAE\x36\xFF", 24);
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,ʹ��16�ֽ�DUKPT��Կ������MAC RESPONSE��Կ:6EF927951A12714DCDEB767BCCF1A9BD��24�ֽ����ݷ�����8�ֽ�(3333333333333333)����3des���ܼ����ƴ�ӳ�24�ֽڽ��
			memcpy(PreOut,"\x70\xEA\x01\x51\x66\x2A\x82\x83\x70\xEA\x01\x51\x66\x2A\x82\x83\x70\xEA\x01\x51\x66\x2A\x82\x83", 24);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��16�ֽ�DUKPT��Կ������MAC RESPONSE��Կ:6EF927951A12714DCDEB767BCCF1A9BD��24�ֽ�����(0x33)��һ�����Ŀ�(ǰ���ֽ�)��3des����Ϊ70EA0151662A8283����Կ�Եڶ������Ŀ�(��һ���˸��ֽ�0x33)���������һ�����Ŀ�(ǰ�˸��ֽ�0x33)���43D932625519B1B0����Կ�Ե��������Ŀ�(��һ���˸��ֽ�0x33)��������ڶ������Ŀ�(�м�˸��ֽ�0x33)���43D932625519B1B0
			memcpy(PreOut,"\x70\xEA\x01\x51\x66\x2A\x82\x83\x43\xD9\x32\x62\x55\x19\xB1\xB0\x43\xD9\x32\x62\x55\x19\xB1\xB0", 24);			
		if(memcmp(DataOut, PreOut, 24))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15], DataOut[16], DataOut[17], DataOut[18], DataOut[19], DataOut[20], DataOut[21], DataOut[22], DataOut[23]);
			GOTOERR;
		}

		//case15:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=TDK's ID,ucKeyType=DUKPT��TDK,ȴ����DUKPT���ݼӽ��ܣ�Ӧʧ��,psIV=NULL
		if((ret=NDK_SecCalcDesDukpt(5,sec_tdk_key_type[j], NULL, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|sec_des_mode[j]))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
	}	

#if !K21_ENABLE
	//case13:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=DUKPT1,ucKeyType=TPK,ȴ�������ݼӽ���Ӧʧ��,psIV=NULL
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TPK, NULL, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//case13:�Ͷ�ƽ̨��Ʒ����ͨ��DES����ӿ�������PIN��Կ��չDES���� ucKeyIdx=DUKPT1,ucKeyType=TPK����DES���㣬Ӧ�óɹ� 
	if((ret=NDK_SecCalcDesDukpt(6,SEC_KEY_TYPE_TPK, NULL, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ʹ�ð�װ��16�ֽ�DUKPT��Կ������PIN��Կ6ef92795e51271b2cdeb767b33f1a942��Կ��3��8�ֽ�����3333333333333333���н��ܺ�ƴ�ӵõ����\x5E\xEE\xE5\x7C\xE7\x26\x2C\x1B
	if(memcmp(DataOut,"\x5E\xEE\xE5\x7C\xE7\x26\x2C\x1B\x5E\xEE\xE5\x7C\xE7\x26\x2C\x1B\x5E\xEE\xE5\x7C\xE7\x26\x2C\x1B", 24))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

#if SEC_RSA_ENABLE
	//case14:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=RSA's ID,ucKeyType=TDK,Ӧʧ��,psIV=NULL
	if((ret=NDK_SecCalcDesDukpt(4,SEC_KEY_TYPE_TDK, NULL, 24, DataIn, DataOut, KSN1, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case16:��֤GETDES��,KSN����
	//getksn
	if((ret=NDK_SecGetDukptKsn(2, KSN1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//getdes
	if((ret=NDK_SecCalcDesDukpt(3,SEC_KEY_TYPE_TDK, NULL, 16, DataIn, DataOut, KSN2, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*|SEC_DES_ECBMODE*/))!=NDK_OK)
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
	
	//case17:KSN�ԼӺ���Ӧ������ǰ����Կ���Զ����£�����KSNҪ�Զ�����
	if ((ret=NDK_SecIncreaseDukptKsn(6))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(j=0;j<num;j++)
	{
		//�ԼӺ�����ǰ��Կ���:5635b184dd5d8f415617b6600c6e47cf���ɴ�������DATA�ӽ�����Կ5635b184dda28f415617b6600c9147cf,��׿������ԿΪB7942062C24DAC1D857F90F85A2F1348,�������ݼӽ���KEY RESPONSEΪ9EAAAFD51C2D67AD 7AE163998A911BC9(δ����֮ǰΪ5635B17BDD5D8F415617B69F0C6E47CF)
		memset(KSN1, 0x00, sizeof(KSN1));
		memset(DataOut, 0, sizeof(DataOut));
		memset(PreOut, 0, sizeof(PreOut));
		if((ret=NDK_SecCalcDesDukpt(6,sec_tdk_key_type[j], NULL, 16, DataIn, DataOut, KSN1, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|sec_des_mode[j]))!=NDK_OK)  //linrq NDK_OK
		{
			send_result("line %d:%s����ʧ��(%d, %d)", __LINE__, TESTAPI, ret, j);
			GOTOERR;
		}
		if(j==0)
		{
#if !(ANDIROD_ENABLE||OVERSEAS_ENABLE||CPU5810X_ENABLE||defined ME30GX||defined ME32GX||defined ME50NX||defined ME68||defined ME32||defined ME32THM||defined ME31S||defined ME30S||defined ME30THM)
			//��Ԥ�ڽ��ʹ��16�ֽ���Կ5635b184dda28f415617b6600c9147cf��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\xFD\x16\x62\x03\x49\x2A\xD9\x08\xFD\x16\x62\x03\x49\x2A\xD9\x08", 16);
#else
			//��Ԥ�ڽ��ʹ��16�ֽ���ԿB7942062C24DAC1D857F90F85A2F1348��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\x10\xBF\x5F\x57\xE9\xE8\x14\x5F\x10\xBF\x5F\x57\xE9\xE8\x14\x5F", 16);	
#endif
		}
		else if(j==1)
			//��SEC_DES_MODE_ECBģʽ,��Ԥ�ڽ��ʹ��16�ֽ���Կ9EAAAFD51C2D67AD 7AE163998A911BC9��16�ֽ����ݷ�����8�ֽ�(3333333333333333)���м��ܼ����ƴ�ӳ�16�ֽڽ��
			memcpy(PreOut,"\x36\x9B\x3E\x59\x81\x71\xC2\x63\x36\x9B\x3E\x59\x81\x71\xC2\x63", 16);
		else if(j==2)
			//��SEC_DES_MODE_CBCģʽ,ʹ��16�ֽ��������ݼӽ���KEY:9EAAAFD51C2D67AD 7AE163998A911BC9��16�ֽ�����(0x33)ǰ���ֽ���3des���ܵõ�369B3E598171C263�����ܽ�������ֽ����������õ�05A80D6AB242F150��������Կ��3des����EC7EEFDB95B60CAB
			memcpy(PreOut,"\x36\x9B\x3E\x59\x81\x71\xC2\x63\x49\xC2\x5B\x7F\x2A\xEF\x6C\x82", 16);
		if(memcmp(DataOut, PreOut, 16))
		{
			send_result("line %d:%s����ʧ��(j=%d, 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, j, DataOut[0], DataOut[1], DataOut[2], DataOut[3], DataOut[4], DataOut[5], DataOut[6], DataOut[7], DataOut[8], DataOut[9], DataOut[10], DataOut[11], DataOut[12], DataOut[13], DataOut[14], DataOut[15]);
			GOTOERR;
		}
		if( KSN1[7]!=0x00 || KSN1[8]!=0x00 || KSN1[9]!=0x02)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
	}
	send_result("%s����ͨ��", TESTAPI);
	
ERR:  //���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}

