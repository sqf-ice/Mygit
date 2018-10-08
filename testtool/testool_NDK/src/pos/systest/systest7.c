/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/06/09
* directory 		: 
* description		: DES�ۺϲ���
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2013/06/09  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��ȫ����,ѹ��"

#define	CNT			(500)	//(10000)
#define	DESCNT		(200)	//(50)
#define	MAXCNT		(5000)

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
int f_sdk3 = 0;
static void sec_press()
{
	/*private & local definition*/
	int i = 0, nRet = 0, succ = 0, nLen = 0, cnt = 0, j = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	char sKey[16+1] = {0x34,0x78,0x12,0x67,0x93,0x11,0x54,0x28,0x34,0x78,0x12,0x67,0x93,0x11,0x54,0x28};
	uchar sOutData[16+1] = {0};
	uchar szBuf[] = {"879423714192374092137"};
	char CntStr[6] = {"3000"};
#if SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};	
	char *cal_type[] = {"DES", "SM4"};
	EM_SEC_MAC mac_type[2]={SEC_MAC_ECB, SEC_MAC_SM4};
#else
	int algnum = 1;
	int key_flag[2] = {0,0} ;
	char *cal_type[] = {"DES"};
	EM_SEC_MAC mac_type[1]={SEC_MAC_ECB};
#endif
#if SEC_AES_ENABLE
	ST_EXTEND_KEYBLOCK stExtendKey;
	char keyval[17] = {0};
#endif	

	/*process body*/
	cls_show_msg1(2, "ѹ��������...\n");

	//Ϊ���������������ĸ���,���뱾����ǰ�Ƚ���ȫ��key�Ĳ���
	if((nRet=NDK_SecKeyErase())!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}

	//ͨ��NDK_SecSetIntervaltime�Ѽ��ʱ����С,���ڲ����ֳ�ʱ����½��г�����֤
#if 0
	if((nRet = NDK_SecSetIntervaltime(0,4))!=NDK_OK)
	{
		cls_show_msg("%d,���ü��ʱ��ʧ��%d", __LINE__,nRet);
		return;
	}
#endif

	if(g_SequencePressFlag==1)
	{
		cnt = MAXCNT;
	}
	else
	{
		cls_printf("Ĭ��ѹ�����Դ���:");
		if((nRet=lib_kbgetinput(CntStr, 2, sizeof(CntStr)-1, (uint *)&nLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)
			cnt = MAXCNT;
		else
			cnt = atoi(CntStr);
	}
	for(j = 0;j<algnum;j++)
	{	
		succ=0;
		//���İ�װTAK�����Ͻ���MAC����(ѡȡMAC������Ϊ��������Ĵ�����)ֱ������,�������Ҫͳ��
		memset(&stKeyInfo,0,sizeof(stKeyInfo));
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|key_flag[j];
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.nDstKeyLen=16;
		if(j==0)
			memcpy(stKeyInfo.sDstKeyValue,sKey,16); 
		else
			memcpy(stKeyInfo.sDstKeyValue,"\x21\x21\x21\x21\x21\x21\x21\x21\x23\x23\x23\x23\x23\x23\x23\x23",16); 

		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((nRet = NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NDK_SecLoadKeyװ��%s��Կʧ��(ret=%d,j=%d)", __LINE__, cal_type[j], nRet, j);
			return;
		}
		cls_printf("ѹ��������!\n%s MAC������...", cal_type[j]);
		for(i=0; i<cnt; i++)
		{			
			if((nRet=NDK_SecGetMac(1, szBuf, sizeof(szBuf), sOutData, mac_type[j]))!=NDK_OK)
			{
				if(cls_show_msg1_record(FILENAME,FUNCNAME, g_keeptime,"line %d��%d�μ���MACʧ��(ret=%d,j=%d),��ESC���˳�", __LINE__, i+1, nRet, j)==ESC)
					break;
				else
					continue;
			}
			else
			{	
				if(j==0)
				{
					if(memcmp(sOutData, "\x45\x45\x39\x39\x45\x31\x34\x37", 8))
					{
						if(cls_show_msg1_record(FILENAME,FUNCNAME, g_keeptime,"line %d��%d�μ���MACʧ��(ret=%d,j=%d),��ESC���˳�", __LINE__, i+1, nRet, j)==ESC)
							break;
						else
							continue;
					}
				}
				else 
				{
					if(memcmp(sOutData, "\x44\x33\xd0\xd5\x97\x9d\x03\x72\x68\x62\xa9\x43\x4b\xe1\xe2\x96", stKeyInfo.nDstKeyLen))
					{	
						if(cls_show_msg1_record(FILENAME,FUNCNAME, g_keeptime,"line %d��%d�μ���MACʧ��(ret=%d,j=%d),��ESC���˳�", __LINE__, i+1, nRet, j)==ESC)
							break;
						else
							continue;
					}
				}
			}
			succ++;
		}
		cls_show_msg_record(FILENAME,FUNCNAME,"%s�㷨�µ�MAC����ѹ���������!�ܹ�����%d��,�ɹ�%d��",cal_type[j],i,succ);
	}
	//AES������ȡ����������Ϊһ���� 20180808 modify
#if SEC_AES_ENABLE
	if(f_sdk3)  //sdk3.0ִ��AES��ز���
	{
		succ=0;
		j = 2;
		//���İ�װTAK�����Ͻ���MAC����(ѡȡMAC������Ϊ��������Ĵ�����)ֱ������,�������Ҫͳ��
		memset(&stKeyInfo,0,sizeof(stKeyInfo));
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memcpy(keyval,"\x21\x21\x21\x21\x21\x21\x21\x21\x23\x23\x23\x23\x23\x23\x23\x23",16); 
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((nRet = NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:NDK_SecLoadKeyװ��%s��Կʧ��(ret=%d,j=%d)", __LINE__, cal_type[j], nRet, j);
			return;
		}
		cls_printf("ѹ��������!\nAES MAC������...");
		for(i=0; i<cnt; i++)
		{			
			if((nRet=NDK_SecGetMac(1, szBuf, sizeof(szBuf), sOutData, SEC_MAC_AES))!=NDK_OK)
			{
				if(cls_show_msg1_record(FILENAME,FUNCNAME, g_keeptime,"line %d��%d�μ���MACʧ��(ret=%d,j=%d),��ESC���˳�", __LINE__, i+1, nRet, j)==ESC)
					break;
				else
					continue;
			}
			else
			{	
				if(memcmp(sOutData, "\x89\x23\xAD\xB4\xD3\x18\xC6\xA1\x3D\x11\xE5\x3B\x65\x73\x50\x28 ", stKeyInfo.nDstKeyLen))
				{	
					if(cls_show_msg1_record(FILENAME,FUNCNAME, g_keeptime,"line %d��%d�μ���MACʧ��(ret=%d,j=%d),��ESC���˳�", __LINE__, i+1, nRet, j)==ESC)
						break;
					else
						continue;
				}
			}
			succ++;
		}
		cls_show_msg_record(FILENAME,FUNCNAME,"AES�㷨�µ�MAC����ѹ���������!�ܹ�����%d��,�ɹ�%d��",i,succ);
	}
#endif	
	return;
}

#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE||ANDIROD_ENABLE
static void DUKPT_Test(void)
{
	//Ӳ��DUKPT(T)DES����(��NDK_SecCalcDesDukpt,����8bytes(����ʹ����Ӳ������һ����),ģʽ����)
	//��ʱ��֧�֣��ȵ�Ԫ����֧�ֺ�����������
	uchar DataIn[33]={0}, sIV[9]={0}, DataOut[33] = {0}, KsnIn[17] = {0}, KeyValueIn[17] = {0};
	int loop = 0, bak = 5;
	ST_SEC_KCV_INFO stKcvInfoIn;
#if K21_ENABLE
	char str[32] = {0};
#endif	
	if(NDK_SecKeyErase()!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
	memset(KsnIn, 0xff, 10);
	memset(KeyValueIn, 0x11, 16);
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	if(NDK_SecLoadTIK(2, 0, 16, KeyValueIn, KsnIn, &stKcvInfoIn)!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
		return;
	}

	//case3:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,TDK1
	cls_show_msg1(2,"8�ֽ���ԿECBģʽDUKPT����ִ����...");
	memset(DataIn,0x33,sizeof(DataIn));
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDesDukpt(2,SEC_KEY_TYPE_TDK, sIV, 8, DataIn, DataOut, KsnIn, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s��%d�β���ʧ��",__LINE__,TESTITEM,loop);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"8�ֽ���ԿECBģʽNDK_SecCalcDesDukpt��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
    cls_show_msg_record(FILENAME,FUNCNAME,"8�ֽ���ԿECBģʽNDK_SecCalcDesDukpt��������:%s��/s",str);
#endif	

	cls_show_msg1(2,"16�ֽ���ԿCBCģʽNDK_SecCalcDesDukpt����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDesDukpt(2,SEC_KEY_TYPE_TDK, sIV, 16, DataIn, DataOut, KsnIn, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*|SEC_DES_ECBMODE*/)!=NDK_OK)
		{	
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s��%d�β���ʧ��",__LINE__,TESTITEM,loop);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿCBCģʽNDK_SecCalcDesDukpt��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿCBCģʽNDK_SecCalcDesDukpt��������:%s��/s",str);
#endif	

	cls_show_msg1(2,"24�ֽ���Կ��ģʽNDK_SecCalcDesDukpt����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDesDukpt(2,SEC_KEY_TYPE_TDK, sIV, 24, DataIn, DataOut, KsnIn, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24/*|SEC_DES_ECBMODE*/)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s��%d�β���ʧ��",__LINE__,TESTITEM,loop);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"24�ֽ���Կ��ģʽNDK_SecCalcDesDukpt��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"24�ֽ���Կ��ģʽNDK_SecCalcDesDukpt��������:%s��/s",str);
#endif	
	return;
}
#else
static void DUKPT_Test(void){cls_show_msg1(5,"��֧��DUKPT�㷨");return;}
#endif

#if ALG_RSA_ENABLE	//֧��RSA�㷨�Ž���RSA����
static void RSA_Test(void)
{
	int loop=0,bak=200;
	ST_RSA_PUBLIC_KEY pubkey1024={0}, pubkey2048={0};
	ST_RSA_PRIVATE_KEY prikey1024={0}, prikey2048={0};
	uchar sDataIn[512]={0},sDataOut[512]={0},sTempOut[512]={0},sTempIn129[129]={0},sTempIn257[257]={0};
#if K21_ENABLE
	char str[32] = {0};
#else
	ST_RSA_PUBLIC_KEY pubkey512={0};
	ST_RSA_PRIVATE_KEY prikey512={0};
	ST_SEC_RSA_KEY RsaKeyIn;
	uchar sTempIn65[65]={0};
#endif

	memset(sDataIn, 'a', sizeof(sDataIn));
	//����ǰ��:����������Կ
	if(NDK_SecKeyErase()!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
	
	//�������RSA��(512/1024/2048/...)
#if !K21_ENABLE	
	cls_show_msg1(2,"��������RSA512��Կ��ִ��ʱ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		cls_printf("��%d������RSA512��˽Կ��,�����ĵȴ�",loop+1);
		if(NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_512,RSA_EXP_3,&pubkey512,&prikey512) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"����RSA512��Կ��ʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);
#endif	

	cls_show_msg1(2,"��������RSA1024��ִ��ʱ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		cls_printf("��%d������RSA1024��˽Կ��,�����ĵȴ�",loop+1);
		if(NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024,RSA_EXP_10001,&pubkey1024,&prikey1024) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"����RSA1024��Կ��ʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
 	cls_show_msg_record(FILENAME,FUNCNAME,"����RSA1024��Կ��ʱ��(ͳ��ֵ):%ss", str);
#endif

	cls_show_msg1(2,"��������RSA2048��ִ��ʱ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		cls_printf("��%d������RSA2048��˽Կ��,�����ĵȴ�",loop+1);
		if(NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_2048,RSA_EXP_10001,&pubkey2048,&prikey2048) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE	
	cls_show_msg_record(FILENAME,FUNCNAME,"����RSA2048��Կ��ʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
 	cls_show_msg_record(FILENAME,FUNCNAME,"����RSA2048��Կ��ʱ��(ͳ��ֵ):%ss", str);
#endif

#if SEC_RSA_ENABLE
	//װ��512����˽Կ����ԿID=1��˽ԿID=2;
	cls_show_msg1(2,"����NDK_SecLoadRsaKeyװ��512����Կִ��ʱ����...");
	memset(&RsaKeyIn, 0,sizeof(ST_SEC_RSA_KEY));
	RsaKeyIn.usBits=pubkey512.bits;
	strcpy((char *)RsaKeyIn.sExponent,(char *)pubkey512.exponent);
	memcpy(RsaKeyIn.sModulus, pubkey512.modulus, RsaKeyIn.usBits*2/8);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadRsaKey(1, &RsaKeyIn)!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}	
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadRsaKeyװ��512����Կʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);

	cls_show_msg1(2,"����NDK_SecLoadRsaKeyװ��512��˽Կִ��ʱ����...");
	strcpy((char *)RsaKeyIn.sExponent, (char *)prikey512.exponent);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadRsaKey(2, &RsaKeyIn)!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadRsaKeyװ��512��˽Կʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);
	
	//װ��1024����˽Կ����ԿID=3��˽ԿID=4;
	cls_show_msg1(2,"����NDK_SecLoadRsaKeyװ��1024����Կִ��ʱ����...");
	memset(&RsaKeyIn, 0,sizeof(ST_SEC_RSA_KEY));
	RsaKeyIn.usBits=pubkey1024.bits;
	strcpy((char *)RsaKeyIn.sExponent,(char *)pubkey1024.exponent);
	memcpy(RsaKeyIn.sModulus, pubkey1024.modulus, RsaKeyIn.usBits*2/8);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadRsaKey(3, &RsaKeyIn)!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}	
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadRsaKeyװ��1024����Կʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);

	cls_show_msg1(2,"����NDK_SecLoadRsaKeyװ��1024��˽Կִ��ʱ����...");
	strcpy((char *)RsaKeyIn.sExponent, (char *)prikey1024.exponent);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadRsaKey(4, &RsaKeyIn)!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadRsaKeyװ��1024��˽Կʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);

	//װ��2048����˽Կ����ԿID=5��˽ԿID=6;
	cls_show_msg1(2,"����NDK_SecLoadRsaKeyװ��2048����Կִ��ʱ����...");
	memset(&RsaKeyIn, 0,sizeof(ST_SEC_RSA_KEY));
	RsaKeyIn.usBits=pubkey2048.bits;
	strcpy((char *)RsaKeyIn.sExponent,(char *)pubkey2048.exponent);
	memcpy(RsaKeyIn.sModulus, pubkey2048.modulus, RsaKeyIn.usBits*2/8);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadRsaKey(5, &RsaKeyIn)!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadRsaKeyװ��2048����Կʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);

	cls_show_msg1(2,"����NDK_SecLoadRsaKeyװ��2048��˽Կִ��ʱ����...");
	strcpy((char *)RsaKeyIn.sExponent, (char *)prikey2048.exponent);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadRsaKey(6, &RsaKeyIn)!= NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}	
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadRsaKeyװ��2048��˽Կʱ��(ͳ��ֵ):%fs",lib_ReadStopwatch()/bak);
	
	//Ӳ��RSA���ܼ�����(������ָʹ��ExpΪ3��0x10001��Կ(Kpub)��������,������ָʹ��Kpub��Ӧ��Kpri��������),
	//RSA512������
	cls_show_msg1(2,"Ӳ��RSA512����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sDataOut,0,sizeof(sDataOut));
		if(NDK_SecRecover(1, sDataIn, 64, sDataOut)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}	
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA512��������:%f��/s",bak/lib_ReadStopwatch());
	cls_show_msg1(2,"Ӳ��RSA512����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sTempOut,0,sizeof(sTempOut));
		if(NDK_SecRecover(2, sDataOut, 64, sTempOut)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA512��������:%f��/s",bak/lib_ReadStopwatch());
	if(memcmp(sTempOut,sDataIn,64))
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA512�ӽ���ͳ�������");
		return;
	}

	//RSA1024������
	cls_show_msg1(2,"Ӳ��RSA1024����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sDataOut,0,sizeof(sDataOut));
		if(NDK_SecRecover(3, sDataIn, 128, sDataOut)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA1024��������:%f��/s",bak/lib_ReadStopwatch());

	cls_show_msg1(2,"Ӳ��RSA1024����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sTempOut,0,sizeof(sTempOut));
		if(NDK_SecRecover(4, sDataOut, 128, sTempOut)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA1024��������:%f��/s",bak/lib_ReadStopwatch());
	if(memcmp(sTempOut,sDataIn,128))
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA1024�ӽ���ͳ�������");
		return;
	}
	
	//RSA2048������
	cls_show_msg1(2,"Ӳ��RSA2048����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sDataOut,0,sizeof(sDataOut));
		if(NDK_SecRecover(5, sDataIn, 256, sDataOut)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA2048��������:%f��/s",bak/lib_ReadStopwatch());

	cls_show_msg1(2,"Ӳ��RSA2048����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sTempOut,0,sizeof(sTempOut));
		if(NDK_SecRecover(6, sDataOut, 256, sTempOut)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA2048��������:%f��/s",bak/lib_ReadStopwatch());
	if(memcmp(sTempOut,sDataIn,256))
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecRecover����RSA2048�ӽ���ͳ�������");
		return;
	}

	//���RSA���ܼ�����(������ָʹ��ExpΪ3��0x10001��Կ(Kpub)��������,������ָʹ��Kpub��Ӧ��Kpri��������)(IN�����������һ��)
	//RSA512������
	cls_show_msg1(2,"RSA512�����ִ����...");
	memcpy(sTempIn65,sDataIn,64);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sDataOut,0,sizeof(sDataOut));
		if(NDK_AlgRSARecover(pubkey512.modulus,pubkey512.bits/8,pubkey512.exponent,sTempIn65,sDataOut) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA512��������:%f��/s",bak/lib_ReadStopwatch());
	
	cls_show_msg1(2,"RSA512�����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sTempOut,0,sizeof(sTempOut));
		if(NDK_AlgRSARecover(prikey512.modulus,prikey512.bits/8,prikey512.exponent,sDataOut,sTempOut) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA512��������:%f��/s",bak/lib_ReadStopwatch());

	//�Ƚ���������ݺͼӽ��ܺ������Ӧ��һ��
	if(memcmp(sTempOut,sTempIn65,strlen((char *)sTempIn65)))
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
#endif

	//RSA1024������
#if K21_ENABLE
	bak = 10000;//�Ͷ˺��������ٶ�̫��,����Ҫ�Ӵ������߲��Ծ���,�������ֽ��Խ�����2147483647.2147483647��/s   20161228 linwl
#endif
	cls_show_msg1(2,"RSA1024�����ִ����...");
	memcpy(sTempIn129,sDataIn,128);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sDataOut,0,sizeof(sDataOut));
		if(NDK_AlgRSARecover(pubkey1024.modulus,pubkey1024.bits/8,pubkey1024.exponent,sTempIn129,sDataOut) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA1024��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA1024��������:%s��/s", str);
#endif

#if K21_ENABLE
	bak = 200;//����ĳ�10000��˴��ָ���20��߲���Ч��
#endif
	cls_show_msg1(2,"RSA1024�����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sTempOut,0,sizeof(sTempOut));
		if(NDK_AlgRSARecover(prikey1024.modulus,prikey1024.bits/8,prikey1024.exponent,sDataOut,sTempOut) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA1024��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA1024��������:%s��/s", str);
#endif
	//�Ƚ���������ݺͼӽ��ܺ������Ӧ��һ��
	if(memcmp(sTempOut,sTempIn129,strlen((char *)sTempIn129)))
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}

	//RSA2048������
	cls_show_msg1(2,"RSA2048�����ִ����...");
	memcpy(sTempIn257,sDataIn,256);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sDataOut,0,sizeof(sDataOut));
		if(NDK_AlgRSARecover(pubkey2048.modulus,pubkey2048.bits/8,pubkey2048.exponent,sTempIn257,sDataOut) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA2048��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA2048��������:%s��/s", str);
#endif

	cls_show_msg1(2,"RSA2048�����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		memset(sTempOut,0,sizeof(sTempOut));
		if(NDK_AlgRSARecover(prikey2048.modulus,prikey2048.bits/8,prikey2048.exponent,sDataOut,sTempOut) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA2048��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgRSARecover����RSA2048��������:%s��/s", str);
#endif

	//�Ƚ���������ݺͼӽ��ܺ������Ӧ��һ��
	if(memcmp(sTempOut,sTempIn257,strlen((char *)sTempIn257)))
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}

	cls_show_msg_record(FILENAME,FUNCNAME,"RSA���ܲ���ͨ��");
}
#else
static void RSA_Test(void){cls_show_msg1(5,"��֧��RSA�㷨");return;}
#endif

#if SHA_ENABLE
static void SHA_Test(void)
{
	/*private & local definition*/
	int loop=0,bak=2000;
	uchar TestSha[1024+1]={0},ResultSha[64+1]={0};
#if K21_ENABLE
	char str[32] = {0};
#endif	

	/*process body*/
	// SHA1/SHA256/SHA512ͬһ1K����
	for(loop=0;loop<1024;loop++)
		TestSha[loop] = rand()%256;	//�������1K����
	cls_show_msg1(2,"SHA1ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgSHA1(TestSha, sizeof(TestSha), ResultSha) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSHA1����:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSHA1����:%s��/s",str);
#endif	

	cls_show_msg1(2,"SHA256ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgSHA256(TestSha, sizeof(TestSha), ResultSha) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSHA256����:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSHA256����:%s��/s",str);
#endif

#if !K21_ENABLE
	cls_show_msg1(2,"SHA512ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgSHA512(TestSha, sizeof(TestSha), ResultSha) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSHA512����:%f��/s",bak/lib_ReadStopwatch());
#endif
}
#else
static void SHA_Test(void){cls_show_msg1(5,"��֧��SHA�㷨");return;}
#endif

//����load����key(TMK(˫����)/TIK), ����load����key(TDK(������)/TDK(˫����)/TDK(������)), 
static void DES_Test(void)
{
	/*private & local definition*/
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	
	int bak=20,loop;
	const unsigned char buf1_tmp[24+1]={0x36,0x72,0xc2,0xbc,0x7f,0x17,0xf2,0x9c,0x65,0x87,0x35,0x86,0xbc,0x7f,0x17,0xf2,0x36,0x72,0xc2,0xbc,0x7f,0x17,0xf2,0x9c};
	//const unsigned char buf2_tmp[24+1]={0x55,0x43,0x32,0xbc,0x7f,0x17,0xf2,0x9c,0x65,0x87,0x35,0x86,0xbc,0x7f,0x17,0xf2,0x36,0x72,0xc2,0xbc,0x7f,0x17,0xf2,0x9c};
	uchar KsnIn[17]={0},KeyValueIn[17]={0},udesin[32]={0},udesout[32]={0};
	const unsigned char buf1_in[8+1]={0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31};
	uchar key8[8]={0},key16[16]={0},key24[24]={0};
#if K21_ENABLE
	char str[32] = {0};
#endif

	/*process body*/

	//����ǰ��:����������Կ
	if(NDK_SecKeyErase()!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
	
	//����load TMK(˫����),ID=1 len=16 TMK
	cls_show_msg1(2,"����NDK_SecLoadKey����װ��ִ��ʱ����...");
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue,buf1_tmp,stKeyInfo.nDstKeyLen);
	lib_StartStopwatch();
	for(loop=0; loop<bak; loop++)
	{
		if(NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey TMKͳ��ֵÿ��ʱ��:%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey TMKͳ��ֵÿ��ʱ��:%ss",str);
#endif

#if DUKPT_ENABLE	//֧��DUKPT�ĲŲ���NDK_SecLoadTIK����
	//����load TIK ��Կ
	cls_show_msg1(2,"����NDK_SecLoadTIKװ��ִ��ʱ����...");
	memset(KsnIn, 0xff, 10);
	memset(KeyValueIn, 0x11, 16);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecLoadTIK(2, 0, 16, KeyValueIn, KsnIn, &stKcvInfoIn)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadTIKͳ��ֵÿ��ʱ��:%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadTIKͳ��ֵÿ��ʱ��:%ss",str);
#endif
#endif

#if !(OVERSEAS_ENABLE||defined N910P)
	// ����load (TDK(������)
	cls_show_msg1(2,"����NDK_SecLoadKeyװ�����ĵ�����TDKִ��ʱ����...");
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	stKeyInfo.ucScrKeyType = SEC_KEY_TYPE_TMK;
	stKeyInfo.ucScrKeyIdx = 1;
	stKeyInfo.ucDstKeyIdx= 4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=8;
	memcpy(stKeyInfo.sDstKeyValue,buf1_in,stKeyInfo.nDstKeyLen);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if (NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey������TDKͳ��ֵÿ��ʱ��:%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey������TDKͳ��ֵÿ��ʱ��:%ss",str);
#endif
#endif

	//����load TDK(˫����)/TDK(������))
	cls_show_msg1(2,"����NDK_SecLoadKeyװ������˫����TDKִ��ʱ����...");
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	stKeyInfo.ucScrKeyType = SEC_KEY_TYPE_TMK;
	stKeyInfo.ucScrKeyIdx = 1;
	stKeyInfo.ucDstKeyIdx= 5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue,buf1_in,8);
	memcpy(stKeyInfo.sDstKeyValue+8,buf1_in,8);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if (NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey˫����TDKͳ��ֵÿ��ʱ��:%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey˫����TDKͳ��ֵÿ��ʱ��:%ss",str);
#endif

#if !(OVERSEAS_ENABLE||defined N910P)
	//����load TDK(������))
	cls_show_msg1(2,"����NDK_SecLoadKeyװ������������TDKִ��ʱ����...");	
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	stKeyInfo.ucScrKeyType = SEC_KEY_TYPE_TMK;
	stKeyInfo.ucScrKeyIdx = 1;
	stKeyInfo.ucDstKeyIdx= 6;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=24;
	memcpy(stKeyInfo.sDstKeyValue,buf1_in,8);
	memcpy(stKeyInfo.sDstKeyValue+8,buf1_in,8);
	memcpy(stKeyInfo.sDstKeyValue+16,buf1_in,8);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if (NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey������TDKͳ��ֵÿ��ʱ��:%fs",lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey������TDKͳ��ֵÿ��ʱ��:%ss",str);
#endif
#endif

	//�ӽ��ܲ��Դ���Ϊ200�� 20130913 linwl
	bak = 200;
#if !(OVERSEAS_ENABLE||defined N910P)
	//Ӳ��(T)DES����(��NDK_SecCalcDes,����8bytes,�ֱ���8(ECB)/16(CBC)/24(��ģʽ)����Կ��������), 
	//�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,TDK id=4
	memset(udesin, 0x20, sizeof(udesin));
	cls_show_msg1(2,"8�ֽ���ԿECBģʽNDK_SecCalcDes����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 4, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/)!=NDK_OK)
			return;
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"8�ֽ���ԿECBģʽNDK_SecCalcDes��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"8�ֽ���ԿECBģʽNDK_SecCalcDes��������:%s��/s",str);
#endif
#endif

	//�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_CBCMODEģʽ, 8�ֽ���������,TDK id=5
	cls_show_msg1(2,"16�ֽ���ԿCBCģʽNDK_SecCalcDes����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 5, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*|SEC_DES_CBCMODE*/)!=NDK_OK)
			return;
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿCBCģʽNDK_SecCalcDes��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿCBCģʽNDK_SecCalcDes��������:%s��/s",str);
#endif

#if !(OVERSEAS_ENABLE||defined N910P)
	//�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|��ģʽ, 8�ֽ���������,TDK id=6
	cls_show_msg1(2,"24�ֽ���Կ��ģʽNDK_SecCalcDes����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 6, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"24�ֽ���Կ��ģʽNDK_SecCalcDes��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"24�ֽ���Կ��ģʽNDK_SecCalcDes��������:%s��/s",str);
#endif
#endif

#if K21_ENABLE
	bak = 10000;//�Ͷ˲�Ʒ����3�����������ٶ�̫��,����Ҫ�Ӵ������߲��Ծ���
#endif
	//���(T)DES����(��NDK_AlgTDes,����8bytes(����ʹ����Ӳ������һ����),�ֱ���8/16/24����Կ��������)
	cls_show_msg1(2,"NDK_AlgTDesʹ��8�ֽ���Կ�����ִ����...");
	memset(key8,0x11,sizeof(key8));
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgTDes(udesin, udesout, key8 , sizeof(key8), ALG_TDS_MODE_ENC) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgTDesʹ��8�ֽ���Կ��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgTDesʹ��8�ֽ���Կ��������:%s��/s",str);
#endif

	cls_show_msg1(2,"NDK_AlgTDesʹ��16�ֽ���Կ����ִ����...");
	memset(key16,0x22,sizeof(key16));
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgTDes(udesin, udesout,key16, sizeof(key16), ALG_TDS_MODE_ENC) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgTDesʹ��16�ֽ���Կ��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgTDesʹ��16�ֽ���Կ��������:%s��/s",str);
#endif

	cls_show_msg1(2,"NDK_AlgTDesʹ��24�ֽ���Կ����ִ����...");
	memset(key24,0x33,sizeof(key24));
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgTDes(udesin, udesout,key24, sizeof(key24), ALG_TDS_MODE_ENC) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgTDesʹ��24�ֽ���Կ��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgTDesʹ��24�ֽ���Կ��������:%s��/s",str);
#endif
	return;
}

#if SEC_SM4_ENABLE
static void SM4_Sec_Test(void)
{
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	int bak=20,loop=0;
	const unsigned char buf1_tmp[16]={0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31};
	const unsigned char buf1_in[16]={0x90,0x92,0x9C,0xA4,0x1B,0x8D,0xD3,0xB2,0x87,0x09,0x0D,0xD5,0x6F,0x3C,0x38,0x8D}; //��Ӧ����Ϊ0X33
	uchar udesin[16]={0},udesout[16]={0};

#if K21_ENABLE
	char str[32] = {0};
#endif	
	//����ǰ��:����������Կ
	if(NDK_SecKeyErase()!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
	
	//���İ�װTMK��Կ,ID=1
	cls_show_msg1(2,"����NDK_SecLoadKey����װ��ִ��ʱ����...");
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue,buf1_tmp,stKeyInfo.nDstKeyLen);
	lib_StartStopwatch();
	for(loop=0; loop<bak; loop++)
	{
		if(NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪSM4�����,TMKͳ��ֵÿ��ʱ��:%fs", lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪSM4�����,TMKͳ��ֵÿ��ʱ��:%ss", str);
#endif

	//���İ�װTDK��Կ
	cls_show_msg1(2,"����NDK_SecLoadKey װ������˫����TDKִ��ʱ����...");
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	stKeyInfo.ucScrKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucScrKeyIdx = 1;
	stKeyInfo.ucDstKeyIdx= 5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue,buf1_in,stKeyInfo.nDstKeyLen);
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if (NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪSM4�����,˫����TDKͳ��ֵÿ��ʱ��:%fs", lib_ReadStopwatch()/bak);
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch()/bak,str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪSM4�����,˫����TDKͳ��ֵÿ��ʱ��:%ss", str);
#endif

	//�ӽ��ܲ��Դ���Ϊ200��
	bak = 200;
	//�� SEC_SM4_ENCRYPTģʽ, 16�ֽ���������,TDK id=5
	memset(udesin, 0x20, sizeof(udesin));
	cls_show_msg1(2,"16�ֽ���ԿSM4����ģʽNDK_SecCalcDes����ִ����...");
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 5, udesin, sizeof(udesin), udesout, SEC_SM4_ENCRYPT)!=NDK_OK)
			return;
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿECBģʽSM4Ӳ��������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿECBģʽSM4Ӳ��������:%s��/s",str);
#endif		
}
#else
static void SM4_Sec_Test(void){cls_show_msg1(5,"��֧�ֹ��ܰ�ȫ�㷨");return;}
#endif

#if ALG_SM_ENABLE
static void SM4_Alg_Test(void)
{
	int bak=20,loop=0;
	uchar udesin[16]={0},udesout[16]={0};
	uchar key16[16]={0},CBC_vector[16]={0x2c, 0x5c, 0x0e, 0x04, 0x2c, 0x8f, 0x06, 0xd2,0x33, 0xff, 0xd0, 0x25, 0x83, 0x65, 0xdf, 0x49};
	
#if K21_ENABLE
		char str[32] = {0};
#endif	
#if K21_ENABLE
	bak = 10000;//�Ͷ˺��������ٶ�̫��,����Ҫ�Ӵ������߲��Ծ���,�������ֽ��Խ�����2147483647.2147483647��/s   20161228 linwl
#endif
	//���(T)SM4 ECB����(��NDK_AlgSM4Compute,����16bytes(����ʹ����Ӳ������һ����)),
	cls_show_msg1(2,"NDK_AlgSM4Computeʹ��16�ֽ���ԿECBģʽ�����ִ����...");
	memset(key16,0x11,sizeof(key16));
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgSM4Compute(key16, NULL, sizeof(udesin), udesin, udesout, ALG_SM4_ENCRYPT_ECB) !=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			RETURN;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSM4Computeʹ��16�ֽ���ԿECBģʽ���������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSM4Computeʹ��16�ֽ���ԿECBģʽ���������:%s��/s",str);
#endif

	//���(T)SM4 CBC����(��NDK_AlgSM4Compute,����16bytes(����ʹ����Ӳ������һ����)),
	cls_show_msg1(2,"NDK_AlgSM4Computeʹ��16�ֽ���ԿCBCģʽ�����ִ����...");
	memset(udesout, 0, sizeof(udesout));
	lib_StartStopwatch();
	for(loop=0;loop<bak;loop++)
	{
		if(NDK_AlgSM4Compute(key16, CBC_vector, sizeof(udesin), udesin, udesout, ALG_SM4_ENCRYPT_CBC)!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
			RETURN;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSM4Computeʹ��16�ֽ���ԿCBCģʽ���������:%f��/s",bak/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(bak/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"NDK_AlgSM4Computeʹ��16�ֽ���ԿCBCģʽ���������:%s��/s",str);
#endif
	return;
}
#else
static void SM4_Alg_Test(void){cls_show_msg1(5,"��֧�ֹ���Alg�㷨");return;}
#endif

#if SEC_AES_ENABLE
static void AES_Test(void)
{
	if( !f_sdk3)
	{
		cls_show_msg1(5,"��֧��AES��ȫ�㷨");
		return;
	}
	else
	{
		ST_SEC_KEY_INFO stKeyInfo;
		ST_SEC_KCV_INFO stKcvInfoIn;
		ST_EXTEND_KEYBLOCK stExtendKey;
		int bak=20,loop=0;
		const unsigned char buf1_tmp[16]={0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21};
		const unsigned char buf1_in[16]={0xF5,0xF6,0xD1,0x51,0xAD,0x4F,0xCF,0xE9,0x6C,0x2F,0xDF,0xF6,0x6C,0xAA,0x02,0x42 }; //��Ӧ����Ϊ0X33
		uchar udesin[16]={0},udesout[16]={0};
		char keyval[17]={0};

#if K21_ENABLE
		char str[32] = {0};
#endif	
		//����ǰ��:����������Կ
		if(NDK_SecKeyErase()!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}
		
		//���İ�װTMK��Կ,ID=1
		cls_show_msg1(2,"����NDK_SecLoadKey����װ��ִ��ʱ����...");
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(stKeyInfo));
		memset(&stExtendKey,0,sizeof(stExtendKey));
		stKeyInfo.ucDstKeyIdx=1;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memcpy(keyval,buf1_tmp,stExtendKey.len);
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		lib_StartStopwatch();
		for(loop=0; loop<bak; loop++)
		{
			if(NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn)!=NDK_OK)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
				return;
			}
		}
#if !K21_ENABLE
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪAES�����,TMKͳ��ֵÿ��ʱ��:%fs", lib_ReadStopwatch()/bak);
#else
		memset(str,0,sizeof(str));
		ftos(lib_ReadStopwatch()/bak,str);
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪAES�����,TMKͳ��ֵÿ��ʱ��:%ss", str);
#endif

		//���İ�װTDK��Կ
		cls_show_msg1(2,"����NDK_SecLoadKey װ������˫����TDKִ��ʱ����...");
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(stKeyInfo));
		stKeyInfo.ucScrKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucScrKeyIdx = 1;
		stKeyInfo.ucDstKeyIdx= 5;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memcpy(keyval,buf1_in,stExtendKey.len);
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		lib_StartStopwatch();
		for(loop=0;loop<bak;loop++)
		{
			if (NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn) != NDK_OK)
			{
				cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s����ʧ��",__LINE__,TESTITEM);
				return;
			}
		}
#if !K21_ENABLE
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪAES�����,˫����TDKͳ��ֵÿ��ʱ��:%fs", lib_ReadStopwatch()/bak);
#else
		memset(str,0,sizeof(str));
		ftos(lib_ReadStopwatch()/bak,str);
		cls_show_msg_record(FILENAME,FUNCNAME,"NDK_SecLoadKey�㷨����ΪAES�����,˫����TDKͳ��ֵÿ��ʱ��:%ss", str);
#endif

		//�ӽ��ܲ��Դ���Ϊ200��
		bak = 200;
		//�� SEC_AES_ENCRYPTģʽ, 16�ֽ���������,TDK id=5
		memset(udesin, 0x20, sizeof(udesin));
		cls_show_msg1(2,"16�ֽ���ԿAES����ģʽNDK_SecCalcDes����ִ����...");
		lib_StartStopwatch();
		for(loop=0;loop<bak;loop++)
		{
			if(NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 5, udesin, sizeof(udesin), udesout, SEC_AES_ENCRYPT)!=NDK_OK)
				return;
		}
#if !K21_ENABLE
		cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿECBģʽAESӲ��������:%f��/s",bak/lib_ReadStopwatch());
#else
		memset(str,0,sizeof(str));
		ftos(bak/lib_ReadStopwatch(),str);
		cls_show_msg_record(FILENAME,FUNCNAME,"16�ֽ���ԿECBģʽAESӲ��������:%s��/s",str);
#endif		
		return;
	}
}
#else
static void AES_Test(void){cls_show_msg1(5,"��֧��AES��ȫ�㷨");return;}
#endif

//SEC���ܲ���(ȫ���ֹ�ȷ��,�Լ�¼����ֵ)
//���˵��:����ע���ܡ�ѹ��(������һ��ǿ�����Ƿ�ɹ�)��,����ע����
//Ҳ����עsec_ability����,����ֵ�ں������й����и���
//ȡ��������ƽ��ֵ.ʱ���ȡʹ��lib_lib_StartStopwatch/lib_ReadStopwatch
static void sec_ability(void)
{
	/*private & local definition*/
	int nKeyin=0;
#if K21_ENABLE
	char str[32] = {0};
#endif
	
	/*process body*/
	lib_StartStopwatch();
	NDK_SecKeyErase();	//�ų����������Ա������ĸ���
#if !K21_ENABLE
	cls_show_msg1(3,"NDK_SecKeyEraseִ��ʱ��:%fs", lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch(),str);
	cls_show_msg1(3,"NDK_SecKeyEraseִ��ʱ��:%ss", str);
#endif

	if(auto_flag==1)
	{
		//�Զ�����������
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�����������ܲ���",TESTITEM);
		
		//ִ��DES����
		DES_Test();
		
		//ִ��SHA����
		SHA_Test();

		//ִ��DUKPT����
		DUKPT_Test();

		//ִ��RSA����
		RSA_Test();

		//���ܰ�ȫ�㷨
		SM4_Sec_Test();		

		//AES ��ȫ�㷨
		AES_Test();

		//����ALG�㷨
		SM4_Alg_Test();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������ܲ��Խ���",TESTITEM);
		return;
	}

	while(1)
	{
		nKeyin = cls_show_msg("1:DES   2:SHA\n"
							"3:DUKPT  4:RSA\n"
							"5:SM4(sec)  6:AES\n"
							"7:SM4(alg)"
							);

		switch(nKeyin)
		{
			case '1':
				DES_Test();
				break;
			case '2':
				SHA_Test();
				break;
			case '3':
				DUKPT_Test();
				break;
			case '4':
				RSA_Test();
				break;
			case '5':
				SM4_Sec_Test();
				break;			
			case '6':
				AES_Test();
				break;
			case '7':
				SM4_Alg_Test();
				break;	
			case ESC:
				return;
				break;
			default:
				continue;
				break;
		}
	}

	return;
}

static void sec_tamper(void)
{

	/*private & local definition*/
	int nKeyin = 0,nRet = 0, i=0, len=0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar sKey[16+1]={0x12,0x34,0x56,0x78,0x87,0x65,0x43,0x21,0x12,0x34,0x56,0x78,0x87,0x65,0x43,0x21};
	uchar sData[16+1]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	uchar Pre_Data[16+1]={0x38,0x91,0x64,0x1A,0xA4,0x5B,0xA5,0x36,0x0E,0xC0,0x18,0xED,0x94,0x34,0x18,0xD8};
	uchar sOutData[16+1]={0};
	uchar sOutData2[16+1]={0};
	int  nStatus = 0;
	char *szSecStatus[]={"�ް�ȫ����","Ӳ����ȫ����","��ȫ�Ĵ���ֵ����","�ļ�У�����","�豸δʹ��"};
#if SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};	
	char *cal_type[] = {"DES", "SM4"};
	EM_SEC_DES des_type[2]={SEC_DES_ENCRYPT, SEC_SM4_ENCRYPT};
#else
	int algnum = 1;
	int key_flag[2] = {0,0} ;
	char *cal_type[] = {"DES"};
	EM_SEC_DES des_type[1]={SEC_DES_ENCRYPT};
#endif
#if SEC_AES_ENABLE 
	ST_EXTEND_KEYBLOCK stExtendKey;
	char keyval[17] = {0};
#endif

	/*process body*/
	while(1)
	{
			nKeyin = cls_show_msg("ѡ����ʵĶ���:\n"
														"1.ȡ��ȫ״̬\n"
														"2.��Կ����\n"
														"3.��ȫ����\n");
		switch(nKeyin) 
		{
		case '1':
			cls_show_msg("�빹�찲ȫ����,���������...");
			
			//TO DO://��������Ҫ��ϰ�ȫ������Ʒ�������֤
			if((nRet = NDK_SecGetTamperStatus(&nStatus))!=NDK_OK)
			{
				cls_show_msg("line %d:NDK_SecGetTamperStatusȡ״̬ʧ��(ret=%d)", __LINE__ , nRet);
				return;
			}
				
			if(cls_show_msg("��ȫ����״̬Ϊ[%s]:,��ȷ���Ƿ���Ԥ��һ��\n[ENTER]һ�� [����]��",szSecStatus[nStatus&1])!=ENTER)//����&1�Ĳ���,ֻȥ��ȫ�������,��������������������ܷ�
			{
				cls_show_msg("line %d:���찲ȫ����ʧ��(ret=%d)", __LINE__ , nRet);
				return;
			}
			break;
		case '2':
			//Ϊ���������������ĸ���,���뱾����ǰ�Ƚ���ȫ��key�Ĳ���
			NDK_SecKeyErase();
			
			for(i=0;i<algnum;i++)
			{
				//TO DO:
				//���İ�װһ��16�ֽڳ���TDK
				memset(&stKeyInfo,0,sizeof(stKeyInfo));
				memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
				stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|key_flag[i];
				stKeyInfo.ucDstKeyIdx = 2;
				memcpy(stKeyInfo.sDstKeyValue,sKey,16);
				stKeyInfo.nDstKeyLen = 16;
				stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
				if ((nRet=NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!=NDK_OK)
				{
					cls_show_msg("line %d:%s�㷨��NDK_SecLoadKeyװ����Կʧ��(ret=%d)", __LINE__, cal_type[i] , nRet);
					break;
				}
				cls_show_msg1(2, "%s�㷨��,��Կ����ɹ�!", cal_type[i]);
			}
#if SEC_AES_ENABLE
			if(f_sdk3)
			{
				memset(&stKeyInfo,0,sizeof(stKeyInfo));
				memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
				stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
				stKeyInfo.ucDstKeyIdx = 2;
				stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
				stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
				stExtendKey.len = 16;
				memcpy(keyval,sKey,stExtendKey.len);
				stExtendKey.pblock = keyval;
				memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
				stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
				if ((nRet=NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!=NDK_OK)
				{
					cls_show_msg("line %d:AES�㷨��NDK_SecLoadKeyװ����Կʧ��(ret=%d)", __LINE__, nRet);
					break;
				}
				cls_show_msg1(2, "AES�㷨��,��Կ����ɹ�!");
			}
#endif			
			break;
		case '3':
			//TO DO:
			//���㲢��Ԥ�ڼ��ܽ���Ա�
			for(i=0;i<algnum;i++)
			{
				memset(&sOutData,0,sizeof(sOutData));
				memset(&sOutData2,0,sizeof(sOutData2));
				if(i==0)
					len=8;
				else
					len=16;
				
				if((nRet=NDK_SecCalcDes(SEC_KEY_TYPE_TDK,2,sData,len,sOutData,des_type[i]))!=NDK_OK)//Ӳ������
				{
					cls_show_msg("line %d:%s�㷨��NDK_SecCalcDes����ʧ��(ret=%d)", __LINE__, cal_type[i] , nRet);
					break;
				}
				if(i==0)
				{
					if((nRet=NDK_AlgTDes(sData,sOutData2,sKey,16,ALG_TDS_MODE_ENC))!=NDK_OK)//�������
					{
						cls_show_msg("line %d:%s�㷨��NDK_AlgTDes����ʧ��(ret=%d)", __LINE__, cal_type[i] , nRet);
						break;
					}
				}
				else 
				{
#if ALG_SM_ENABLE			
					if((nRet=NDK_AlgSM4Compute(sKey, NULL, len, sData, sOutData2, ALG_SM4_ENCRYPT_ECB))!=NDK_OK)
					{
						cls_show_msg("line %d:%s�㷨NDK_AlgSM4Compute����ʧ��(ret=%d)", __LINE__, cal_type[i] , nRet);
						break;
					}
#endif					
				}
				if(memcmp(sOutData,sOutData2,len))
				{
					cls_show_msg("line %d:%s�㷨��,��ȫ����ʧ��!", __LINE__, cal_type[i]);
					break;
				}
				cls_show_msg1(2, "%s�㷨��,��ȫ����ɹ�!", cal_type[i]);
			}
#if SEC_AES_ENABLE			
			if(f_sdk3)
			{
				memset(&sOutData,0,sizeof(sOutData));
				len=16;
				if((nRet=NDK_SecCalcDes(SEC_KEY_TYPE_TDK,2,sData,len,sOutData,SEC_AES_ENCRYPT))!=NDK_OK)//Ӳ������
				{
					cls_show_msg("line %d:%s�㷨��NDK_SecCalcDes����ʧ��(ret=%d)", __LINE__, cal_type[i] , nRet);
					break;
				}
				memcpy(sOutData2,Pre_Data,16);
				if(memcmp(sOutData,sOutData2,len))
				{
					cls_show_msg("line %d:AES�㷨��,��ȫ����ʧ��!", __LINE__);
					break;
				}
				cls_show_msg1(2, "AES�㷨��,��ȫ����ɹ�!");
			}
#endif			
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}	

	return;
}

static void sec_powerdown(void)
{

	/*private & local definition*/
	int nKeyin = 0,nRet = 0, nFlag=1, i=0, len=0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar sMainKey[16+1]={0x12,0x34,0x56,0x78,0x87,0x65,0x43,0x21,0x12,0x34,0x56,0x78,0x87,0x65,0x43,0x21};
	uchar sData[16+1]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	uchar Pre_Data[16+1]={0x38,0x91,0x64,0x1A,0xA4,0x5B,0xA5,0x36,0x0E,0xC0,0x18,0xED,0x94,0x34,0x18,0xD8};
	uchar sOutData[16+1]={0};
	uchar sOutData2[16+1]={0};
#if SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};	
	char *cal_type[] = {"DES", "SM4"};
	EM_SEC_DES des_type[2]={SEC_DES_ENCRYPT, SEC_SM4_ENCRYPT};
#else
	int algnum = 1;
	int key_flag[2] = {0,0} ;
	char *cal_type[] = {"DES"};
	EM_SEC_DES des_type[1]={SEC_DES_ENCRYPT};
#endif
#if SEC_AES_ENABLE
	ST_EXTEND_KEYBLOCK stExtendKey;
	char keyval[17] = {0};
#endif
	
	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("ѡ����ʵĶ���:\n"
													"1.���緽ʽ\n"
													"2.��Կ����\n"
													"3.��ȫ����\n");
		switch(nKeyin) 
		{
		case '1':
			//TO DO:
			//�Զ����ֶ�����,����ֻ��¼��־.
			nFlag = cls_show_msg("�Զ������ֶ�����,�����꿪�����Ե�3��\n1:�Զ� 2:�ֶ�")=='1'?1:2;
			cls_show_msg1(2,"���ñ�־Ϊ%d",nFlag);
			break;
		case '2':
			//Ϊ���������������ĸ���,���뱾����ǰ�Ƚ���ȫ��key�Ĳ���
			NDK_SecKeyErase();
			cls_show_msg("��������԰���3��֤�����Ƿ�ᱣ����Կ");
			//TO DO:
			for(i=0;i<algnum;i++)
			{
				//���İ�װһ��16�ֽڳ���TDK,�������Զ����ֶ�����.�Զ��ϵ�ʹ��NDK_SysShutDown,�ֶ�����ʾһ����xxx���жϵ�Դ
				memset(&stKeyInfo,0,sizeof(stKeyInfo));
				memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
				stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|key_flag[i];
				stKeyInfo.ucDstKeyIdx = 1;
				memcpy(stKeyInfo.sDstKeyValue,sMainKey,16);
				stKeyInfo.nDstKeyLen = 16;
				stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
				if ((nRet=NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!=NDK_OK)
				{
					cls_show_msg("line %d:%s�㷨��NDK_SecLoadKeyװ����Կʧ��(ret=%d)", __LINE__, cal_type[i], nRet);
					break;
				}
			}
#if SEC_AES_ENABLE
			//���İ�װһ��16�ֽڳ���TDK,�������Զ����ֶ�����.�Զ��ϵ�ʹ��NDK_SysShutDown,�ֶ�����ʾһ����xxx���жϵ�Դ
			if(f_sdk3)
			{
				memset(&stKeyInfo,0,sizeof(stKeyInfo));
				memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
				stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
				stKeyInfo.ucDstKeyIdx = 1;
				stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
				stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
				stExtendKey.len = 16;
				memcpy(keyval,sMainKey,stExtendKey.len);
				stExtendKey.pblock = keyval;
				memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
				stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
				if ((nRet=NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!=NDK_OK)
				{
					cls_show_msg("line %d:AES�㷨��NDK_SecLoadKeyװ����Կʧ��(ret=%d)", __LINE__, nRet);
					break;
				}
			}
#endif
			if(nFlag==1)
			{
#if !(ANDIROD_ENABLE)
				cls_printf("�Զ��ϵ���,���Ժ�!!!\n");
				NDK_SysShutDown();
#else
				cls_show_msg("����Ʒ��֧���Զ��ػ�����,��ѡ���ֶ���������²���,�����������!!!\n");
#endif
			}
			else
			{
				cls_printf("�������ε���Դ!!!\n");
				while(1);
			}
			break;
		case '3':
			//TO DO:
			//���㲢��Ԥ�ڼ��ܽ���Ա�
			for(i=0;i<algnum;i++)
			{		
				memset(&sOutData,0,sizeof(sOutData));
				memset(&sOutData2,0,sizeof(sOutData2));
				if(i==0)
					len=8;
				else
					len=16;
				if((nRet=NDK_SecCalcDes(SEC_KEY_TYPE_TDK,1,sData,len,sOutData,des_type[i]))!=NDK_OK)//Ӳ������
				{
					cls_show_msg("line %d:%s�㷨��NDK_SecCalcDes����ʧ��(ret=%d)", __LINE__, cal_type[i], nRet);
					break;
				}
				if(i==0)
				{
					if((nRet=NDK_AlgTDes(sData,sOutData2,sMainKey,16,ALG_TDS_MODE_ENC))!=NDK_OK)//�������
					{
						cls_show_msg("line %d:%s�㷨��NDK_AlgTDes����ʧ��(ret=%d)", __LINE__, cal_type[i], nRet);
						break;
					}
				}
				else 
				{
#if ALG_SM_ENABLE
					if((nRet=NDK_AlgSM4Compute(sMainKey, NULL, len, sData, sOutData2, ALG_SM4_ENCRYPT_ECB))!=NDK_OK)
					{
						cls_show_msg("line %d:%s�㷨��NDK_AlgSM4Compute����ʧ��(ret=%d)", __LINE__, cal_type[i], nRet);
						break;
					}
#endif
				}
				if(memcmp(sOutData,sOutData2,len))
				{
					cls_show_msg("line %d:%s�㷨��,��ȫ����ʧ��!", __LINE__, cal_type[i]);
					break;
				}
				cls_show_msg1(2, "%s�㷨��,��ȫ����ɹ�!", cal_type[i]);
			}
#if SEC_AES_ENABLE
			if(f_sdk3)
			{		
				memset(&sOutData,0,sizeof(sOutData));
				len=16;
				if((nRet=NDK_SecCalcDes(SEC_KEY_TYPE_TDK,1,sData,len,sOutData,SEC_AES_ENCRYPT))!=NDK_OK)//Ӳ������
				{
					cls_show_msg("line %d:%s�㷨��NDK_SecCalcDes����ʧ��(ret=%d)", __LINE__, cal_type[i], nRet);
					break;
				}
				memcpy(sOutData2,Pre_Data,16);
				if(memcmp(sOutData,sOutData2,len))
				{
					cls_show_msg("line %d:AES�㷨��,��ȫ����ʧ��!", __LINE__);
					break;
				}
				cls_show_msg1(2, "AES�㷨��,��ȫ����ɹ�!");
			}
#endif
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}	

	return;
	//������/���ķ�ʽ��װ��TWK,DUKPT,RSA, �������غ�,

}

#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE||ANDIROD_ENABLE
static void sec_ksn(void)
{
	/*private & local definition*/
	uchar KsnIn[11]={0},KeyValueIn[17]={0},psKsnOut[11]={0};
	ST_SEC_KCV_INFO stKcvInfoIn;
	int nKeyin = 0,i = 0,nRet=0;
	char szBcdKsnStr[11]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xe0,0x00,0x06};
	/*process body*/

	while(1)
	{
		nKeyin = cls_show_msg("ѡ����ʵĶ���:\n"
							   "1.��װDUKPT\n"
							   "2.ksnУ��\n");
		switch(nKeyin) 
		{
			case '1':
				cls_printf("��װDUKPT��Կ��...");
				memset(KsnIn, 0xff, 10);
				memset(KeyValueIn, 0x11, 16);
				memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
				if(NDK_SecLoadTIK(2, 0, 16, KeyValueIn, KsnIn, &stKcvInfoIn)!=NDK_OK)
				{
					cls_show_msg("line %d:%s����ʧ��",__LINE__,TESTITEM);
					return;
				}
				for(i=0;i<5;i++)
				{
					if(NDK_SecIncreaseDukptKsn(2)!=NDK_OK)
					{
						cls_show_msg("line %d:%s����ksnʧ��", __LINE__,TESTITEM);
					}
				}
				cls_show_msg("���˳������������,Ȼ���ڽ��뱾����2.ksnУ��");
				break;
			case '2':
				if((nRet = NDK_SecGetDukptKsn(2,psKsnOut))!=NDK_OK)
				{
					cls_show_msg("line %d:%s����ʧ��(%d)",__LINE__,TESTITEM,nRet);
					return;
				}
				for(i=0;i<10;i++)
				{
					if(psKsnOut[i]!=szBcdKsnStr[i])
					{
						cls_show_msg("ksnУ��ʧ��(0x%2x,0x%2x)", psKsnOut[i], szBcdKsnStr[i]);
						return;
					}
				}
				cls_show_msg("KSNֵУ�����ͨ��");
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
	return;
}
#else
static void sec_ksn(void){cls_show_msg1(5,"��֧��DUKPT�㷨");return;}
#endif

#if ALG_SM_ENABLE
static int sleeptype = 1;//Ĭ�Ϸ�ʽΪ��������
static void set_sleeptype(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("��ѡ�����߷�ʽ\n"
								"1.��������\n"		//����������ָ�ж˲�Ʒ�����ó���֧����������ʱ��ķ�ʽ���ɵͲ���ʱ�䵽��֮��������ߵķ�ʽ
								"2.��������\n");	//����������ָ�еͶ˲�Ʒ��Ӧ�ó���������ߺ�����������

		switch (nKeyin)
		{
		case '1':
			sleeptype = 0;
			return;
			break;
		case '2':
			sleeptype = 1;
			return;
			break;
		case ESC:
			sleeptype = 1;
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

static void sec_sm2_cryp(void)
{
	int ret=0;
	ushort len=0, cryp_len=0;
	uchar pubkey[64]={0}, prikey[32]={0}, cryp[1024]={0}, message[928]={0};
	uchar max_message[]="dafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjndkanfknwejojfaeinfknfkdsnkfnjkdbngiuhqoptreopikpkdnnkwnjnfjnbkfasdfaesfaeafsfadfaedafdkjkkemfkweknfjn";
	memset(prikey, 0, sizeof(prikey));
	memset(pubkey, 0, sizeof(pubkey));
	if((ret=NDK_AlgSM2KeyPairGen(pubkey, prikey))!=NDK_OK)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
		return;
	}
	while(1)
	{
		if(sleeptype==0)
		{
			if(cls_show_msg("��ȴ������Զ�����,���ߺ���,���Ѻ󰴼�����,��ȡ�����˳�")==ESC)
				return;
		}
		else
		{
			if(cls_show_msg("��������󽫽��뺯������,���ߺ���,��ȡ�����˳�")==ESC)
				return;
#if ANDIROD_ENABLE
			if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
			if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
			{
				cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
			sleep(2);//���ߺ�����Ҫʱ����Ч
		}
		memset(cryp, 0, sizeof(cryp));
		memset(message, 0, sizeof(message));
		if((ret=NDK_AlgSM2Encrypt(pubkey, max_message, strlen((char*)max_message), cryp, &cryp_len))!=NDK_OK||cryp_len!=strlen((char*)max_message)+96)
		{
			cls_show_msg("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTITEM, ret, cryp_len);
			return;
		}
		if((ret=NDK_AlgSM2Decrypt(prikey, cryp, strlen((char*)max_message)+96, message, &len))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if(memcmp(message, max_message, len))
		{
			cls_show_msg("line %d:%s����ʧ��", __LINE__, TESTITEM);
			return; 
		}
		cls_show_msg1(1,"���߻��Ѻ�SM2�ӽ��ܲ���ͨ��");//20170115 ������ʾ��߲�������
		//return;
	}
}

static void sec_sm2_sign(void)
{
	int ret=0;
	uchar prikey[32]={0}, pubkey[64]={0}, summary[32]={0}, sign_data[64]={0}, message[]="newland";
	uchar ID[]={"test"};
	memset(prikey, 0, sizeof(prikey));
	memset(pubkey, 0, sizeof(pubkey));
	if((ret=NDK_AlgSM2KeyPairGen(pubkey, prikey))!=NDK_OK)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
		return;
	}
	while(1)
	{
		if(sleeptype==0)
		{
			if(cls_show_msg("��ȴ������Զ�����,���ߺ���,���Ѻ󰴼�����,��ȡ�����˳�")==ESC)
				return;
		}
		else
		{
			if(cls_show_msg("��������󽫽��뺯������,���ߺ���,��ȡ�����˳�")==ESC)
				return;
#if ANDIROD_ENABLE
			if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
			if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
			{
				cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
			sleep(2);//���ߺ�����Ҫʱ����Ч
		}	
		memset(summary, 0, sizeof(summary));
		memset(sign_data, 0, sizeof(sign_data));
		if((ret=NDK_AlgSM2GenE(strlen((char*)ID), ID, strlen((char*)message), message, pubkey, summary))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if((ret=NDK_AlgSM2Sign(prikey, summary, sign_data))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if((ret=NDK_AlgSM2Verify(pubkey, summary, sign_data))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		cls_show_msg1(1,"���߻��Ѻ�SM2ǩ����ǩ����ͨ��");//20170115 ������ʾ��߲�������
		//return;
	}
}

static void sec_sm3(void)
{
	int ret=0;
	uchar out_buf[32]={0};
	uchar long_buf_2630[]="daferjaqbhkjnd��anflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpo��powejiefad`2112312½3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,1';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557daferjaqbhkjndskanflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpojapowejiefad`21123128u3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,0';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557`~!01@��$#[^%]*(��)'-_=+|&{}:½;?/><.,dlkvnkdmxfkjvnalkjdfpojapowejiefad`21123128u349kanflktoekc8f8a82a567c6df5f500f421335a05aa47a3721887ca0b2acc37a905eb3ee94c03782ff3c0d90987dc875aa23c94094c4712c9b9cf4ee6c40e0d70aa271d24f8fc13fb6461774879141b5e9925e0c664d4706e1a296d8d8c83e6f61fd810c50002c21f36c9ab6d4dd0a1c1def150973ad868f44e1e592286b8f5e144bee9263b3bdc5b642f10777bcd4993829bda479ac89f90aeec76e7671f75079161904a4639725aa0d8b1774ea05d1f0e774b8f51d3f4c0d223c43a913a9b443170bf8ccafde060e525688434a96c5b704ac287d212a7d051130bdd91061cc4a5dd0f30e4c9d139cdb3897633394d324117b894754d1f503bec3f57f32ccb4dfad6060fb13db03df728a01166ddfpo��powejiefad`2112312½3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())nalkjdfpojapowejief.`1ds12213344243155fdgsaer172t4687981uhgaguyt87t782";
	uchar pre_hash[32]={0x18,0x45,0x1c,0x3c,0x1b,0x39,0x4a,0xd1,
						0x3c,0xc3,0x36,0xb2,0x2b,0x47,0x99,0x2f,
						0x52,0x78,0x7e,0x95,0xdb,0xab,0x9c,0x9a,
						0x4f,0x4e,0xa6,0xc5,0x0f,0xee,0x8e,0x90};//SM3�������long_buf_2630���ݵ�ʵ�����ժҪ
	while(1)
	{
		if(sleeptype==0)
		{
			if(cls_show_msg("��ȴ������Զ�����,���ߺ���,���Ѻ󰴼�����,��ȡ�����˳�")==ESC)
				return;
		}
		else
		{
			if(cls_show_msg("��������󽫽��뺯������,���ߺ���,��ȡ�����˳�")==ESC)
				return;
#if ANDIROD_ENABLE
			if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
			if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
			{
				cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
			sleep(2);//���ߺ�����Ҫʱ����Ч
		}
		
		memset(out_buf, 0, sizeof(out_buf));
		if((ret=NDK_AlgSM3Start())!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if((ret=NDK_AlgSM3Update(long_buf_2630, 1024))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if((ret=NDK_AlgSM3Final(long_buf_2630+1024, strlen((char*)long_buf_2630)-1024, out_buf))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if(memcmp(out_buf, pre_hash, sizeof(out_buf)))
		{
			cls_show_msg("line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}
		cls_show_msg1(1,"���߻��Ѻ�SM3����ͨ��");//20170115 ������ʾ��߲�������
		//return;
	}
}

static void sec_sm4(void)
{
	int ret=0;
	uchar out_buf[1072]={0}, out_buf1[3000]={0};
	uchar ECB_key[16]={0xd5, 0xc3, 0xa5, 0xec, 0x08, 0x61, 0x86, 0x9c,
					   0xa7, 0x1c, 0x59, 0x74, 0xa4, 0xf7, 0x5d, 0xcf};
    uchar CBC_key[16]={0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,
						0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48},
		  CBC_vector[16]={0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,
						   0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48},
		  CBC_max_data[1072]={0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
		  					  0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,
		  					  0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48},
	      CBC_max_cryp[1072]={0xa9,0x7a,0x34,0x11,0x7a,0xb7,0x7f,0xfa,0xbc,0x7d,0x22,0x90,0xc7,0xe4,0x08,0x96,0xac,0x6d,0xff,0x28,0xb5,0x04,0x68,0x5e,0x00,0xa9,0x2b,0xc2,0xb1,0x47,0xc7,0xb0,
	      					  0xb2,0x85,0xb4,0x85,0xdc,0x85,0xdd,0x93,0x13,0xb6,0xcf,0xcb,0xcf,0x24,0x78,0x3f,0xc4,0x93,0x09,0xc3,0xd2,0x88,0xd3,0x34,0x4f,0x30,0x77,0x0c,0x0b,0xc4,0xad,0xfe,
	      					  0xfb,0x16,0xc9,0xec,0x66,0x8b,0x12,0xb5,0xd0,0xcf,0xd5,0x2d,0x9b,0x18,0x26,0x5d,0xb7,0x74,0x26,0x5b,0x97,0x04,0x18,0x2d,0x02,0x58,0x5b,0x90,0x22,0xe6,0x66,0x75,
	      					  0xd8,0xad,0x08,0xe8,0xe5,0x3d,0x4a,0x62,0xe5,0x6c,0x9a,0xea,0x2d,0x6f,0xac,0xe7,0x5b,0x85,0xde,0xc2,0x89,0xa5,0x9e,0x2e,0xee,0x48,0x11,0x55,0x79,0x1a,0x53,0xfc,
	      					  0xf5,0x59,0x48,0x70,0x88,0x64,0x8f,0x72,0x47,0xd9,0x9c,0xbf,0x12,0x8b,0x7b,0xa7,0x7a,0xbf,0xd3,0x50,0x6b,0x2b,0xae,0x2e,0xf0,0x88,0x07,0x86,0xc7,0x47,0x64,0xe3,
	      					  0x29,0x46,0xe7,0xec,0xb5,0xb2,0x02,0x17,0x3e,0x0e,0x1c,0xa3,0xd4,0x4a,0xde,0x07,0xd8,0x44,0x6f,0x41,0xb5,0xef,0xdc,0x57,0x97,0x31,0xe5,0x5f,0xd6,0x46,0xac,0xd3,
	      					  0x0c,0x5e,0x20,0x49,0xcc,0x48,0x1e,0x5a,0xf9,0xc2,0xc6,0xa3,0x8e,0x70,0x35,0x11,0x3e,0x21,0x02,0x18,0xf5,0xff,0x49,0x52,0x33,0x7b,0xc2,0x00,0xc7,0x50,0x6d,0x21,
	      					  0xf5,0x3e,0x45,0x53,0x68,0x94,0x16,0xb7,0x6d,0x65,0x2b,0x40,0x39,0x77,0x7f,0x70,0x47,0x76,0x18,0x0f,0xbc,0x1d,0x6c,0x88,0x7a,0x6d,0x4f,0x3d,0xba,0x4e,0xd8,0x72,
	      					  0xd8,0xa0,0x02,0x05,0x16,0x36,0x3a,0x09,0xaf,0xdc,0x57,0x73,0x7a,0x82,0x5b,0x59,0x14,0x52,0x93,0xe6,0xea,0xca,0xbb,0x78,0x38,0xa7,0xa3,0xd4,0x16,0x4c,0xb4,0x29,
	      					  0xaf,0xd9,0xdb,0x15,0x21,0x34,0x39,0xce,0x03,0xb6,0x06,0x6a,0x0c,0x05,0x24,0x65,0xe6,0x6d,0x6d,0xfa,0x28,0x08,0xde,0x91,0x5c,0xd8,0x11,0x59,0x19,0x83,0xe2,0xa0,
	      					  0x15,0xf8,0xee,0x2d,0x18,0x5b,0xaa,0x99,0xaa,0xe3,0x8f,0x72,0x7e,0xb5,0x53,0x8c,0x14,0xcf,0xb8,0xd4,0x72,0x17,0x1c,0xf0,0x9b,0x7d,0xbe,0x5b,0x90,0x5d,0x0d,0xbd,
	      					  0xb0,0x9b,0x94,0x8a,0x8b,0xbb,0x10,0x77,0xcc,0x83,0x1f,0xdc,0x42,0xdf,0xc9,0xc6,0x3d,0xc5,0xc2,0xa3,0x03,0x74,0x66,0x14,0x07,0x22,0x56,0x9a,0xf3,0x8c,0x36,0x1e,
	      					  0x3b,0xa8,0xaa,0xc2,0x0b,0x01,0xa3,0xf1,0x18,0x75,0xc0,0x22,0xfe,0x8f,0x45,0xf6,0x83,0xe5,0xe3,0xcb,0xb3,0x4a,0xec,0xb2,0xff,0xae,0x5d,0xd2,0x2b,0x38,0x7f,0x1f,
	      					  0xdd,0x11,0x8d,0xd0,0x3a,0xec,0x40,0xaf,0xfd,0x15,0x6c,0x13,0x66,0x33,0x7c,0x4e,0x66,0x72,0xdb,0xe1,0x94,0xc9,0x93,0xf3,0x88,0x07,0x66,0x27,0xab,0xa4,0xcf,0x93,
	      					  0xf2,0x5b,0x56,0xd9,0xb5,0xdd,0xfe,0x75,0x2c,0xa8,0x77,0x50,0x9c,0xc3,0x0c,0xbc,0x60,0x9e,0xc1,0xee,0x34,0x81,0x75,0x8d,0x93,0x70,0x84,0x06,0x03,0x4b,0xbd,0x49,
	      					  0x88,0xe3,0x3a,0x55,0x76,0xee,0x6b,0xbc,0xf0,0xa0,0x40,0x29,0xe9,0xb9,0x62,0x0f,0xfa,0x8d,0xd5,0xcd,0x2c,0xfc,0x30,0x07,0xbf,0x54,0x69,0x54,0x1d,0x5f,0x40,0x1f,
	      					  0x1a,0x4a,0x88,0x9c,0x54,0xd4,0xba,0x24,0x6a,0xd2,0xdb,0xde,0x9e,0xd3,0x7a,0xf1,0xa6,0xfc,0x11,0x7a,0xf6,0x70,0xf9,0x02,0x80,0x8f,0x6f,0x06,0x87,0xe8,0xe6,0xb0,
	      					  0x62,0xca,0xde,0x7a,0x6e,0x86,0xef,0xfd,0xe8,0x03,0xad,0x8f,0x3a,0x5a,0xa7,0x3c,0x22,0x17,0x4a,0xd0,0x2c,0xe8,0x35,0xaf,0xff,0x30,0x79,0x1f,0x23,0x01,0x75,0x06,
	      					  0xac,0x97,0x04,0xab,0x04,0x47,0x9a,0xd4,0xc8,0x5d,0xdb,0x53,0xf2,0xf8,0x95,0x80,0xf7,0x46,0x9d,0x67,0x38,0xb6,0xe4,0x7b,0x1c,0xb3,0xa9,0xbd,0x5c,0x68,0xe8,0x64,
	      					  0xfa,0x81,0x9b,0x6e,0xfe,0x44,0xc8,0x5e,0x61,0x51,0xf1,0x0a,0xdc,0x6a,0x89,0xb9,0xb0,0xa4,0x19,0x22,0x01,0xa5,0xa8,0x9f,0x4a,0x5b,0x47,0xcb,0x8d,0x87,0x52,0x3f,
	      					  0xb0,0xca,0xe5,0x62,0x7e,0x9f,0x02,0x57,0x5f,0xa5,0xf3,0x52,0x7c,0x92,0xe5,0xd2,0x82,0x12,0x25,0x8d,0x6c,0xc3,0x8b,0x03,0x1b,0x6c,0x8c,0x45,0x6f,0xc0,0x9f,0xdf,
	      					  0x05,0xae,0x86,0xe1,0x4c,0xdf,0x1f,0xc2,0xfa,0x2d,0x3b,0x7a,0x38,0xdd,0x67,0xe4,0x4c,0xe3,0x17,0x07,0x47,0xc0,0x5f,0x9a,0xf5,0x8c,0xb7,0x4c,0x74,0xb2,0xf9,0x43,
	      					  0xb8,0x20,0x28,0x28,0x5e,0xec,0x9b,0x18,0x61,0x2b,0x3e,0x30,0x49,0x37,0x44,0x58,0xbc,0x0b,0x8a,0xa7,0x35,0x0c,0x7d,0x6f,0xdb,0xa6,0xe0,0x71,0x23,0xb4,0x6d,0x0b,
	      					  0xb2,0x76,0xc4,0xb4,0xe5,0xb0,0xaa,0x2a,0x17,0x05,0x46,0x9a,0xd1,0x96,0x88,0x35,0x35,0x37,0x9e,0xf9,0xca,0x81,0x1d,0x2f,0xe3,0xd9,0xde,0x1f,0x43,0xba,0xce,0xd5,
	      					  0x84,0x1b,0x36,0xef,0xe3,0x19,0xf2,0x03,0x69,0x1e,0x11,0x9f,0x32,0x7d,0x38,0x9b,0x05,0x6d,0x9a,0xfc,0xf9,0xc3,0xf8,0x53,0x4e,0x2d,0x14,0xd7,0x0d,0x6e,0xa8,0x26,
	      					  0x4b,0x6e,0x50,0x20,0x46,0xed,0xba,0xf0,0x54,0x9e,0x81,0x11,0x6f,0x9a,0x14,0x5f,0x60,0x05,0xd7,0x98,0x86,0xad,0xf0,0x27,0xb4,0xc9,0xe7,0xaf,0xcf,0xbb,0xd2,0x64,
	      					  0x90,0xa2,0x03,0xe1,0x4f,0x68,0x2b,0x43,0x86,0x44,0x23,0x31,0x55,0x53,0x41,0x2b,0x9a,0x64,0xfb,0x25,0xed,0xb7,0x42,0x13,0x63,0x2a,0x2a,0x1d,0x15,0xb9,0x0a,0xdf,
	      					  0x7d,0xc0,0xd9,0xd7,0x4c,0x11,0x07,0x2b,0x77,0x5c,0x7b,0x9c,0x58,0xe9,0xd7,0xef,0x98,0xcd,0x51,0xf9,0xac,0x73,0x5f,0xe6,0x5f,0x66,0xc0,0x36,0x65,0xa5,0x8c,0x2d,
	      					  0x72,0x01,0xbf,0x41,0xce,0xe2,0x44,0x84,0x9e,0xe7,0xf1,0xa5,0x3b,0xb6,0x6f,0x75,0x17,0xfa,0x0a,0xf4,0x96,0x35,0x23,0x10,0x5b,0xbc,0xb3,0x58,0x18,0x9d,0xd5,0xaa,
	      					  0x95,0x40,0x64,0xc9,0x0b,0x78,0xeb,0xf6,0x43,0xe7,0x60,0x0e,0x23,0xde,0x20,0x6d,0x20,0x69,0x8b,0xaa,0x9d,0xbe,0x80,0xe1,0x8e,0x39,0x32,0x9b,0x7a,0x79,0x40,0xa0,
	      					  0x9f,0xe6,0x5f,0x7f,0x89,0xa6,0x75,0x12,0x4e,0x4e,0xb0,0x4c,0x54,0x30,0xbd,0x8e,0x0e,0x53,0x3b,0x48,0x12,0x6b,0x19,0x9c,0xad,0x32,0xc6,0xe9,0x98,0x4d,0x77,0xa2,
	      					  0xb3,0x33,0x32,0x58,0xbb,0x2a,0x15,0x86,0x8c,0xd8,0x30,0xd4,0xa7,0xe3,0xab,0x56,0x42,0x2f,0xab,0x37,0x7e,0xd6,0x50,0x05,0x8c,0xd7,0x83,0x00,0xb0,0x11,0xb1,0x25,
	      					  0x59,0xef,0xa8,0xc2,0xed,0x70,0xdc,0xea,0x11,0xd3,0xac,0x22,0xda,0xf8,0x88,0xa2,0xf5,0xee,0x6c,0x5f,0x14,0x13,0x64,0x91,0x53,0x5e,0xbb,0x52,0xc7,0xdd,0xc0,0x97,
	      					  0xb6,0xd6,0xa3,0x82,0x10,0x93,0x82,0xbf,0xbe,0x1b,0x91,0x1f,0x05,0x5f,0x81,0x91};//CBC�����ݼ��ܽ���ģʽ�µ�����
	uchar ECB_max_data[]="daferjaqbhkjnd��anflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpo��powejiefad`2112312½3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,1';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557daferjaqbhkjndskanflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpojapowejiefad`21123128u3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,0';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557`~!01@��$#[^%]*(��)'-_=+|&{}:½;?/><.,dlkvnkdmxfkjvnalkjdfpojapowejiefad`21123128u349kanflktoekc8f8a82a567c6df5f500f421335a05aa47a3721887ca0b2acc37a905eb3ee94c03782ff3c0d90987dc875aa23c94094c4712c9b9cf4ee6c40e0d70aa271d24f8fc13fb6461774879141b5e9925e0c664d4706e1a296d8d8c83e6f61fd810c50002c21f36c9ab6d4dd0a1c1def150973ad868f44e1e592286b8f5e144bee9263b3bdc5b642f10777bcd4993829bda479ac89f90aeec76e7671f75079161904a4639725aa0d8b1774ea05d1f0e774b8f51d3f4c0d223c43a913a9b443170bf8ccafde060e525688434a96c5b704ac287d212a7d051130bdd91061cc4a5dd0f30e4c9d139cdb3897633394d324117b894754d1f503bec3f57f32ccb4dfad6060fb13db03df728a01166d!@#$%^&*()dhh(&#$UUDFHBdjw~d8u823uhuhbdhfb9239794375694698977*xi9u93u49098034y57981yhjbjkfdsa09-09u98432yu9841y�´�½.,><<<<}{}|:;~!@#$fduaghufgeuafuuhuidhafoih";
	uchar ECB_max_cryp[2640]={0xD2,0x8A,0x9C,0x00,0x9D,0x53,0x0B,0x32,0xE8,0x8A,0x3B,0x3A,0x0A,0x29,0xE2,0xF5,0xAE,0x05,0xFC,0x82,0x47,0xD7,0x91,0xA5,0xCC,0xAD,0x53,0x40,0xF2,0x65,0x1D,0x9E,0x46,0x6C,0x93,0xED,0xC2,0x39,0xC5,0xEC,0x26,0xDC,0x64,0x00,0xDD,0xC3,0xE1,0xE9,0xB5,0x6E,0x36,0xFD,0x57,0x44,0x6B,0x23,0x3E,0xB8,0x10,0xAB,0x8B,0x32,0x26,0x3E,0xE0,0x5A,0xA3,0x4D,0x3B,0x81,0x46,0x4F,0x16,0xF9,0xE4,0x80,0x0C,0x39,0xF4,0x70,
							  0x07,0xB3,0x5A,0x2E,0x8E,0xD4,0xFA,0xAA,0x0B,0x8C,0x72,0x4A,0x8A,0x17,0x5C,0xB3,0x04,0x3E,0xDF,0xBD,0xEE,0x24,0xF3,0xD8,0xC4,0x82,0x6D,0xA4,0xBA,0xAF,0x8E,0xC8,0x7E,0xB8,0x73,0xBE,0x8A,0xE9,0x7F,0xD4,0xAC,0x79,0x98,0xE3,0x44,0x81,0x8F,0xA3,0x5D,0xDC,0xCE,0x84,0xC2,0x7D,0x1C,0x41,0xA6,0x07,0x00,0x70,0x6A,0xD8,0x7B,0xEB,0x0C,0x5A,0x1F,0x1A,0x98,0x82,0x00,0xFD,0x63,0x59,0x17,0x54,0xD6,0xC3,0x8C,0xE4,
							  0xFB,0x59,0x1B,0x4E,0x32,0x61,0xE6,0x06,0x0D,0x96,0x61,0x6F,0xB7,0x27,0x85,0x05,0x1A,0x8D,0x4F,0xFE,0x00,0x9B,0x00,0x2F,0xD2,0x5B,0x4D,0xFA,0xAE,0xC5,0xC0,0xFE,0x32,0x6A,0x05,0x1A,0x04,0x0A,0xDB,0xED,0x5D,0xFB,0xB5,0x7F,0x5E,0x07,0xE6,0xE4,0xAB,0x56,0x28,0x92,0x9E,0x0F,0x6D,0x6B,0x2C,0x6A,0x24,0x3D,0xFA,0x53,0xC5,0x25,0x04,0x21,0x71,0x35,0xEC,0xD4,0xE1,0x06,0xA3,0xCD,0xAE,0x01,0x4D,0x1C,0x95,0xC8,
							  0x42,0x62,0x6C,0x06,0x7C,0xDE,0x21,0x9D,0x53,0x8F,0x3E,0xE2,0x69,0x32,0x85,0xB4,0xC3,0x5F,0xFD,0xC4,0x88,0x5A,0x6B,0x39,0xC0,0x19,0xB0,0x5C,0x21,0x24,0x3F,0xDC,0xFE,0xBB,0xB7,0x50,0x2E,0xC3,0xF6,0x84,0xCF,0xA4,0x84,0x26,0x0D,0xE1,0x76,0x17,0xBD,0xD3,0x22,0x1D,0xD4,0x3A,0x57,0x31,0x10,0x5A,0x9D,0x17,0xD3,0x3C,0xEB,0x08,0xF4,0x88,0xA2,0x2A,0xF2,0xE1,0x32,0x65,0xA3,0x52,0x22,0x89,0x04,0xCB,0xDE,0xEB,
							  0x23,0xF4,0x17,0x13,0x38,0xE5,0x65,0x4A,0x81,0x7E,0xAB,0xBE,0x7F,0x48,0xC4,0x27,0x12,0x2B,0xBE,0x4A,0x82,0x2E,0x64,0xEB,0x3C,0xC9,0x5B,0xCC,0x64,0xA0,0xC3,0x15,0x26,0x92,0xE9,0x33,0x20,0xB9,0xEA,0x66,0x1A,0x6C,0xD2,0xB5,0x3A,0x12,0xBB,0xFA,0x26,0xDE,0xC4,0x86,0x88,0x79,0xDA,0xE5,0xF0,0xBF,0xD3,0x83,0x41,0x57,0x2D,0x74,0xF8,0xEB,0xFE,0xAC,0xB8,0xC9,0x70,0x1C,0xFC,0xE6,0xBB,0x7A,0x7F,0xD7,0x57,0xFD,
							  0xB4,0x70,0x2F,0x3C,0x22,0x53,0x1C,0xF9,0x05,0xEB,0xDD,0x12,0xD9,0x13,0xA0,0x8C,0xA5,0xBB,0x88,0x84,0x0F,0x19,0xA0,0xBA,0x6A,0x50,0x9B,0x64,0x4D,0x60,0xD0,0x97,0x6C,0x68,0x8A,0x24,0xD6,0xC8,0xCF,0x1C,0x53,0x1F,0x56,0x02,0xBD,0x7C,0x03,0xC3,0x3E,0x97,0x3A,0xF9,0x2B,0x65,0xF7,0x6A,0x87,0x61,0x4F,0x19,0xBF,0xD3,0xE5,0x2B,0x0E,0xCC,0x6F,0x4E,0xE6,0xCB,0xEB,0x78,0xEB,0xB1,0x38,0x76,0x63,0xA8,0x04,0x8D,
							  0x92,0x6A,0xE5,0x47,0xFD,0xF8,0xEE,0x6C,0x04,0xDB,0xE5,0xF3,0xB3,0x76,0xE8,0x2C,0xCE,0x22,0x8A,0x74,0xE4,0xE4,0x81,0xCF,0x7F,0xF1,0x25,0xFD,0xFE,0xB7,0x26,0x69,0xF6,0xD2,0xF9,0x04,0x3B,0xE7,0x66,0xC2,0xD8,0x2F,0xDA,0x2D,0xD7,0x45,0xA6,0x7B,0x10,0xFA,0xC0,0xE0,0x6C,0xDA,0x6D,0xC3,0x28,0x88,0xD5,0xA8,0xC4,0xC3,0x36,0x3A,0xCE,0x4E,0x7D,0xDF,0x11,0x9D,0x41,0x99,0xE4,0x06,0xC3,0x38,0xB4,0x55,0x05,0x95,
							  0xAA,0xE2,0xF2,0xE7,0x27,0x54,0x20,0xD4,0xFD,0x2D,0x9C,0x63,0x05,0x66,0xF1,0xDF,0xF7,0x0D,0xB5,0x58,0x7D,0xBA,0x13,0xCF,0x5F,0x12,0xB3,0xD8,0xB1,0x22,0x7F,0x1A,0xA3,0xAF,0xC0,0x2A,0xDE,0xFC,0x10,0xF7,0x72,0x29,0x34,0x3A,0xF6,0x1E,0x84,0x97,0x29,0x20,0xAB,0xE0,0xE4,0xF8,0x54,0x40,0x59,0x2E,0x96,0xA3,0xB5,0x02,0x3F,0x6E,0x6C,0x15,0x8E,0x2F,0xCD,0x23,0x05,0x0F,0xEB,0xDC,0xA4,0x9D,0x69,0x5E,0x01,0x3E,
							  0xCC,0xB2,0xB7,0x16,0x5F,0xF2,0xFB,0xC8,0xBE,0x3A,0x38,0x13,0x0F,0xC9,0xAD,0xF3,0x60,0x09,0x5E,0xA3,0x8C,0x3B,0x05,0x0D,0xDB,0x1E,0x82,0x3E,0x81,0x69,0xB6,0x24,0xF2,0xEB,0x6B,0x08,0xE0,0x34,0xE1,0xE5,0xF5,0xED,0xE5,0xB8,0xBF,0x09,0x0C,0xD0,0x7C,0xD2,0x6E,0x36,0x5A,0xAB,0x5E,0x3E,0xBC,0x4D,0xB6,0xE7,0xE9,0x41,0xB6,0x81,0xD7,0x47,0xFE,0xFC,0x31,0xB9,0x57,0x6E,0xDF,0x27,0xB4,0x58,0xC7,0x0D,0x64,0xDE,
							  0xE5,0x74,0xD9,0x8E,0x7F,0x95,0x80,0xFD,0x13,0xA5,0x45,0xA4,0x2B,0x2E,0x5B,0x26,0x68,0x4F,0x26,0xFC,0x81,0xD5,0x20,0xF2,0x0F,0x6F,0x4A,0xD2,0xB9,0xCE,0x03,0x2B,0x3C,0x4A,0x3D,0x6D,0x71,0x38,0x60,0x3B,0x85,0x7C,0x01,0x0D,0x68,0x7B,0xD9,0x48,0x57,0xF4,0x14,0xD5,0xEB,0x7B,0x9B,0xCA,0xA4,0xFD,0x38,0x11,0x00,0x14,0xAA,0xE5,0xC9,0x38,0xC9,0x0B,0x2E,0x13,0x64,0xAC,0xAD,0xD6,0xC9,0xD3,0xAA,0x2D,0x61,0x91,
							  0x63,0x68,0x2D,0x74,0x57,0x9B,0x50,0x2A,0x1A,0x1A,0x09,0x3B,0x67,0xF4,0xD7,0x37,0xC8,0x01,0xF3,0x0C,0xF4,0x7C,0x83,0xBE,0xB9,0x4B,0x3C,0xE0,0x8A,0xA4,0x05,0x0E,0x5A,0x38,0x68,0xC5,0x40,0x62,0x97,0x27,0x1C,0x1F,0x89,0xF7,0x23,0x6D,0xDC,0x7A,0xF1,0x01,0xA6,0xBB,0x7E,0xE5,0xDC,0xEC,0x5A,0x93,0xF0,0xB1,0xD5,0x07,0xCB,0x9F,0x44,0xD9,0x98,0x3A,0x52,0xC0,0x75,0x0E,0x62,0xF6,0x24,0x16,0x20,0x9B,0xAD,0x8D,
							  0x88,0x5D,0x73,0x2D,0x8B,0x82,0xA9,0x32,0xC4,0x7A,0x7F,0xC5,0x80,0x73,0xD2,0x92,0x0D,0xE1,0xB8,0xF2,0x32,0x28,0x8E,0x17,0x17,0x1A,0x32,0x4B,0xFB,0x1B,0x03,0x8F,0x94,0x6B,0x39,0x55,0x79,0xA3,0x71,0xFC,0xD1,0x69,0x38,0x54,0x9F,0x83,0xC2,0xB6,0xBF,0x53,0x14,0x92,0xBB,0xDF,0x16,0x15,0xC6,0x17,0x8D,0x9F,0x0D,0x5D,0x45,0xDA,0xAE,0x05,0xFC,0x82,0x47,0xD7,0x91,0xA5,0xCC,0xAD,0x53,0x40,0xF2,0x65,0x1D,0x9E,
							  0x46,0x6C,0x93,0xED,0xC2,0x39,0xC5,0xEC,0x26,0xDC,0x64,0x00,0xDD,0xC3,0xE1,0xE9,0xA1,0x8E,0x0F,0x35,0x46,0xBB,0xCF,0x20,0xA2,0xDA,0x60,0x47,0xC7,0xFE,0x78,0xE7,0xD9,0xD7,0x4B,0x01,0xE5,0x03,0x67,0x36,0x49,0x99,0x00,0x1D,0x9B,0x10,0x4D,0xCF,0x07,0xB3,0x5A,0x2E,0x8E,0xD4,0xFA,0xAA,0x0B,0x8C,0x72,0x4A,0x8A,0x17,0x5C,0xB3,0x04,0x3E,0xDF,0xBD,0xEE,0x24,0xF3,0xD8,0xC4,0x82,0x6D,0xA4,0xBA,0xAF,0x8E,0xC8,
							  0x7E,0xB8,0x73,0xBE,0x8A,0xE9,0x7F,0xD4,0xAC,0x79,0x98,0xE3,0x44,0x81,0x8F,0xA3,0x5D,0xDC,0xCE,0x84,0xC2,0x7D,0x1C,0x41,0xA6,0x07,0x00,0x70,0x6A,0xD8,0x7B,0xEB,0x0C,0x5A,0x1F,0x1A,0x98,0x82,0x00,0xFD,0x63,0x59,0x17,0x54,0xD6,0xC3,0x8C,0xE4,0xFB,0x59,0x1B,0x4E,0x32,0x61,0xE6,0x06,0x0D,0x96,0x61,0x6F,0xB7,0x27,0x85,0x05,0x1A,0x8D,0x4F,0xFE,0x00,0x9B,0x00,0x2F,0xD2,0x5B,0x4D,0xFA,0xAE,0xC5,0xC0,0xFE,
							  0x32,0x6A,0x05,0x1A,0x04,0x0A,0xDB,0xED,0x5D,0xFB,0xB5,0x7F,0x5E,0x07,0xE6,0xE4,0xAB,0x56,0x28,0x92,0x9E,0x0F,0x6D,0x6B,0x2C,0x6A,0x24,0x3D,0xFA,0x53,0xC5,0x25,0x04,0x21,0x71,0x35,0xEC,0xD4,0xE1,0x06,0xA3,0xCD,0xAE,0x01,0x4D,0x1C,0x95,0xC8,0x42,0x62,0x6C,0x06,0x7C,0xDE,0x21,0x9D,0x53,0x8F,0x3E,0xE2,0x69,0x32,0x85,0xB4,0xC3,0x5F,0xFD,0xC4,0x88,0x5A,0x6B,0x39,0xC0,0x19,0xB0,0x5C,0x21,0x24,0x3F,0xDC,
							  0xFE,0xBB,0xB7,0x50,0x2E,0xC3,0xF6,0x84,0xCF,0xA4,0x84,0x26,0x0D,0xE1,0x76,0x17,0xBD,0xD3,0x22,0x1D,0xD4,0x3A,0x57,0x31,0x10,0x5A,0x9D,0x17,0xD3,0x3C,0xEB,0x08,0xF4,0x88,0xA2,0x2A,0xF2,0xE1,0x32,0x65,0xA3,0x52,0x22,0x89,0x04,0xCB,0xDE,0xEB,0x23,0xF4,0x17,0x13,0x38,0xE5,0x65,0x4A,0x81,0x7E,0xAB,0xBE,0x7F,0x48,0xC4,0x27,0x12,0x2B,0xBE,0x4A,0x82,0x2E,0x64,0xEB,0x3C,0xC9,0x5B,0xCC,0x64,0xA0,0xC3,0x15,
							  0x26,0x92,0xE9,0x33,0x20,0xB9,0xEA,0x66,0x1A,0x6C,0xD2,0xB5,0x3A,0x12,0xBB,0xFA,0x26,0xDE,0xC4,0x86,0x88,0x79,0xDA,0xE5,0xF0,0xBF,0xD3,0x83,0x41,0x57,0x2D,0x74,0xF8,0xEB,0xFE,0xAC,0xB8,0xC9,0x70,0x1C,0xFC,0xE6,0xBB,0x7A,0x7F,0xD7,0x57,0xFD,0xB4,0x70,0x2F,0x3C,0x22,0x53,0x1C,0xF9,0x05,0xEB,0xDD,0x12,0xD9,0x13,0xA0,0x8C,0xA5,0xBB,0x88,0x84,0x0F,0x19,0xA0,0xBA,0x6A,0x50,0x9B,0x64,0x4D,0x60,0xD0,0x97,
							  0x6C,0x68,0x8A,0x24,0xD6,0xC8,0xCF,0x1C,0x53,0x1F,0x56,0x02,0xBD,0x7C,0x03,0xC3,0x3E,0x97,0x3A,0xF9,0x2B,0x65,0xF7,0x6A,0x87,0x61,0x4F,0x19,0xBF,0xD3,0xE5,0x2B,0x0E,0xCC,0x6F,0x4E,0xE6,0xCB,0xEB,0x78,0xEB,0xB1,0x38,0x76,0x63,0xA8,0x04,0x8D,0xF7,0x22,0xF1,0xC1,0xFB,0x6D,0xD0,0x58,0xDF,0x8A,0x00,0x2B,0xDA,0x03,0xBA,0x87,0xCE,0x22,0x8A,0x74,0xE4,0xE4,0x81,0xCF,0x7F,0xF1,0x25,0xFD,0xFE,0xB7,0x26,0x69,
							  0xF6,0xD2,0xF9,0x04,0x3B,0xE7,0x66,0xC2,0xD8,0x2F,0xDA,0x2D,0xD7,0x45,0xA6,0x7B,0x10,0xFA,0xC0,0xE0,0x6C,0xDA,0x6D,0xC3,0x28,0x88,0xD5,0xA8,0xC4,0xC3,0x36,0x3A,0xCE,0x4E,0x7D,0xDF,0x11,0x9D,0x41,0x99,0xE4,0x06,0xC3,0x38,0xB4,0x55,0x05,0x95,0xAA,0xE2,0xF2,0xE7,0x27,0x54,0x20,0xD4,0xFD,0x2D,0x9C,0x63,0x05,0x66,0xF1,0xDF,0xF7,0x0D,0xB5,0x58,0x7D,0xBA,0x13,0xCF,0x5F,0x12,0xB3,0xD8,0xB1,0x22,0x7F,0x1A,
							  0xA3,0xAF,0xC0,0x2A,0xDE,0xFC,0x10,0xF7,0x72,0x29,0x34,0x3A,0xF6,0x1E,0x84,0x97,0x29,0x20,0xAB,0xE0,0xE4,0xF8,0x54,0x40,0x59,0x2E,0x96,0xA3,0xB5,0x02,0x3F,0x6E,0x6C,0x15,0x8E,0x2F,0xCD,0x23,0x05,0x0F,0xEB,0xDC,0xA4,0x9D,0x69,0x5E,0x01,0x3E,0xCC,0xB2,0xB7,0x16,0x5F,0xF2,0xFB,0xC8,0xBE,0x3A,0x38,0x13,0x0F,0xC9,0xAD,0xF3,0x60,0x09,0x5E,0xA3,0x8C,0x3B,0x05,0x0D,0xDB,0x1E,0x82,0x3E,0x81,0x69,0xB6,0x24,
							  0xF2,0xEB,0x6B,0x08,0xE0,0x34,0xE1,0xE5,0xF5,0xED,0xE5,0xB8,0xBF,0x09,0x0C,0xD0,0x7C,0xD2,0x6E,0x36,0x5A,0xAB,0x5E,0x3E,0xBC,0x4D,0xB6,0xE7,0xE9,0x41,0xB6,0x81,0xD7,0x47,0xFE,0xFC,0x31,0xB9,0x57,0x6E,0xDF,0x27,0xB4,0x58,0xC7,0x0D,0x64,0xDE,0xE5,0x74,0xD9,0x8E,0x7F,0x95,0x80,0xFD,0x13,0xA5,0x45,0xA4,0x2B,0x2E,0x5B,0x26,0x68,0x4F,0x26,0xFC,0x81,0xD5,0x20,0xF2,0x0F,0x6F,0x4A,0xD2,0xB9,0xCE,0x03,0x2B,
							  0x3C,0x4A,0x3D,0x6D,0x71,0x38,0x60,0x3B,0x85,0x7C,0x01,0x0D,0x68,0x7B,0xD9,0x48,0x57,0xF4,0x14,0xD5,0xEB,0x7B,0x9B,0xCA,0xA4,0xFD,0x38,0x11,0x00,0x14,0xAA,0xE5,0xC9,0x38,0xC9,0x0B,0x2E,0x13,0x64,0xAC,0xAD,0xD6,0xC9,0xD3,0xAA,0x2D,0x61,0x91,0x63,0x68,0x2D,0x74,0x57,0x9B,0x50,0x2A,0x1A,0x1A,0x09,0x3B,0x67,0xF4,0xD7,0x37,0xC8,0x01,0xF3,0x0C,0xF4,0x7C,0x83,0xBE,0xB9,0x4B,0x3C,0xE0,0x8A,0xA4,0x05,0x0E,
							  0x5A,0x38,0x68,0xC5,0x40,0x62,0x97,0x27,0x1C,0x1F,0x89,0xF7,0x23,0x6D,0xDC,0x7A,0xF1,0x01,0xA6,0xBB,0x7E,0xE5,0xDC,0xEC,0x5A,0x93,0xF0,0xB1,0xD5,0x07,0xCB,0x9F,0x44,0xD9,0x98,0x3A,0x52,0xC0,0x75,0x0E,0x62,0xF6,0x24,0x16,0x20,0x9B,0xAD,0x8D,0x88,0x5D,0x73,0x2D,0x8B,0x82,0xA9,0x32,0xC4,0x7A,0x7F,0xC5,0x80,0x73,0xD2,0x92,0x0D,0xE1,0xB8,0xF2,0x32,0x28,0x8E,0x17,0x17,0x1A,0x32,0x4B,0xFB,0x1B,0x03,0x8F,
							  0x94,0x6B,0x39,0x55,0x79,0xA3,0x71,0xFC,0xD1,0x69,0x38,0x54,0x9F,0x83,0xC2,0xB6,0x69,0xDB,0xED,0xE8,0x19,0x62,0xBF,0x04,0xCC,0xF5,0x9F,0x61,0x52,0x23,0x7C,0xFB,0x10,0x1A,0x3B,0x3D,0x08,0x9E,0x39,0x29,0x49,0xDB,0x10,0xFD,0xF8,0x8D,0xF5,0x76,0x27,0x38,0x5C,0xBA,0x61,0xA8,0x72,0x0D,0x5C,0x50,0x54,0xAA,0xEE,0xF1,0x42,0xA5,0x42,0x15,0x51,0xD5,0xF1,0xEB,0x92,0x8E,0x77,0xCF,0x38,0xD3,0xF4,0xD9,0x61,0x21,
							  0x17,0x17,0x35,0x31,0xB3,0x58,0x4D,0x55,0xF9,0x10,0xF3,0xE5,0xA1,0x02,0x84,0xE0,0xF3,0x6F,0x4C,0x32,0xEC,0xAA,0x99,0xBB,0x64,0x80,0x00,0xDA,0x79,0x03,0x95,0x64,0xCF,0xFD,0x30,0x37,0x9E,0x41,0x82,0x46,0xA3,0x29,0x1A,0x0D,0xCB,0xEA,0x3D,0xE8,0xB4,0xB2,0xF6,0xAB,0x36,0xF4,0x36,0xF0,0x57,0xCE,0xC2,0x55,0x4A,0x01,0xAA,0x6F,0x36,0x35,0x38,0x4D,0x30,0x8D,0xDC,0x31,0x08,0x7C,0x8D,0xFB,0xDC,0xE2,0x79,0x92,
							  0xD3,0x13,0x88,0x8B,0x82,0x3F,0x83,0xF7,0xC3,0xDF,0xFA,0x79,0xCD,0xF8,0xCE,0x5F,0xEE,0x69,0x2A,0x9F,0xD9,0x19,0xC3,0xFD,0x26,0x83,0x41,0x4D,0x35,0xA9,0x0F,0x61,0xE5,0xFC,0x91,0x74,0xD7,0x3E,0x6E,0x86,0xD2,0x48,0x14,0xD5,0xF4,0xDE,0x97,0x42,0x73,0x6E,0xDA,0x45,0xF0,0xE0,0x2B,0xE9,0xDC,0x67,0x2C,0xF5,0x6B,0x22,0x92,0xAB,0x86,0xD4,0x5E,0xB5,0x39,0xB3,0xC8,0x10,0x4D,0x00,0x2A,0x62,0x33,0x4A,0xF7,0x41,
							  0x6E,0x47,0x4B,0x07,0x14,0x41,0xC1,0x2C,0xA1,0x98,0xD0,0x1A,0x72,0xFF,0x19,0xDB,0x38,0x1B,0x20,0x55,0x8C,0x04,0xB4,0x9F,0x2C,0x35,0x94,0x0D,0x4B,0xDC,0xF9,0x26,0xEE,0xC7,0x02,0x06,0x10,0x6B,0x33,0xC0,0xA2,0xD3,0x8C,0xE7,0x3F,0x48,0x33,0x27,0xDB,0x31,0xC4,0xD1,0x5A,0xCD,0x50,0x94,0x43,0xD1,0xD4,0xFA,0x9B,0xDA,0x13,0x2A,0x66,0x98,0xBD,0x54,0x39,0xBC,0x12,0x50,0x00,0xDD,0x5D,0x66,0xDF,0x41,0x4F,0x81,
							  0xB6,0x20,0x2C,0x06,0x61,0xD2,0xDD,0x57,0x26,0xDA,0xDA,0xA3,0xFB,0x01,0x4B,0xAE,0x55,0xF3,0xA3,0x6E,0x84,0x7C,0x96,0x25,0xF9,0xC0,0xA9,0x77,0xA8,0x53,0xCA,0x74,0x4F,0x8C,0x21,0x0D,0x5F,0x33,0x98,0xD8,0x5E,0x74,0x14,0x66,0xD0,0x8B,0xF3,0x40,0x39,0x17,0xF4,0x5B,0x8C,0x74,0xD9,0x9C,0xCC,0x92,0xB1,0xA1,0x5B,0x20,0xAB,0xF6,0x13,0x1D,0xF1,0x02,0xE1,0x9C,0xFE,0x97,0x1C,0x3D,0xE1,0xAF,0xC0,0xEA,0xCF,0x46,
							  0x03,0x69,0x9A,0x6B,0x9A,0x9E,0x84,0xAE,0xA0,0x84,0x75,0x71,0x88,0x83,0xC4,0x5F,0x34,0xE2,0xD7,0x94,0x40,0x6B,0xC8,0xC6,0x2B,0x6B,0x75,0x1D,0x2F,0xF8,0x50,0xA4,0x11,0xD2,0x96,0x37,0x0E,0xDB,0x25,0x68,0x4F,0x72,0x9A,0x66,0x41,0x28,0x48,0x89,0x5B,0xCB,0x65,0xE8,0xF4,0x64,0xF6,0x2D,0x62,0xE9,0x24,0xBD,0x77,0x06,0x8D,0x14,0xE3,0x49,0x35,0x9D,0x65,0xFC,0xB0,0x62,0x3F,0x79,0x61,0x37,0xC4,0x13,0x48,0xF8,
							  0xB5,0xDB,0x88,0x7D,0x26,0x13,0xBD,0xDE,0xBC,0x81,0xD9,0xD8,0x7B,0xF5,0xEC,0xBB,0x4F,0xB4,0x93,0x74,0x89,0x66,0x99,0x80,0x65,0x56,0xEE,0x08,0xC0,0x60,0x7F,0xDE,0x97,0xF3,0x80,0xF8,0x8A,0x2E,0x32,0xA1,0x57,0xC6,0x59,0xDE,0x24,0x03,0x29,0xFF,0x6A,0xF9,0x1B,0x1A,0xAD,0xC8,0x2D,0xE9,0xF8,0x56,0x1D,0x11,0x05,0xC3,0xFC,0x87,0xDB,0x55,0x38,0xF2,0x5C,0x5F,0x26,0x37,0x55,0x12,0xE8,0x0F,0xAB,0x72,0x28,0x5B,
							  0x92,0x35,0xD9,0xA9,0x56,0x75,0x59,0x33,0x4E,0x42,0x5E,0x6F,0x4A,0x21,0x5E,0x3F,0x5B,0xD0,0xCB,0xD4,0x13,0xE9,0x31,0xC1,0x8A,0xEE,0x4F,0x0B,0x59,0x18,0x2E,0x70,0xE1,0x03,0xEA,0x3C,0x5C,0x6A,0xFA,0xD7,0xB3,0xA1,0xC5,0xD7,0x76,0x5D,0xB7,0xCB,0x37,0x11,0xDB,0x82,0x03,0x06,0x9F,0x2B,0xCC,0xE5,0x60,0xDB,0x3E,0x66,0x2C,0xD1,0xE0,0xAC,0x02,0x9A,0x0A,0x4C,0xFA,0x46,0xB0,0x96,0x07,0x42,0x11,0xC2,0x54,0xA6,
							  0x1A,0xB2,0x5E,0xBF,0x27,0x28,0x7C,0xC6,0x72,0xA2,0xC1,0x24,0x05,0x0E,0x96,0x9A,0x21,0xDC,0x80,0xD0,0x92,0xB9,0xCD,0xB9,0x1D,0xB4,0xCD,0x29,0xAE,0x62,0xBF,0x50,0x5D,0x43,0x9F,0x56,0xAD,0x49,0x32,0xF7,0xDE,0xFE,0x28,0x12,0xFD,0xB3,0x87,0xEF,0xB1,0xBA,0x6B,0x7A,0x60,0x32,0xED,0xBF,0xAF,0x68,0x0D,0x3D,0x2A,0x98,0x17,0x9A,0x4C,0x8A,0xFC,0x05,0xB4,0xD5,0x40,0x9C,0x70,0x07,0x6E,0x03,0x3A,0x92,0xF3,0x4E,
							  0x82,0x9B,0x35,0xA0,0xC7,0x8E,0xE0,0x54,0x4C,0x0D,0x8C,0x1C,0xAD,0x2F,0x8C,0xDC,0xE2,0xD9,0x61,0xDF,0x4D,0xD2,0x80,0xCC,0xB4,0x1D,0xEF,0xAC,0xB6,0x88,0x05,0x94,0xBC,0xD7,0x0E,0x3A,0xC6,0x94,0x89,0x37,0x88,0x2A,0x3F,0x13,0x4D,0xE6,0x49,0x85,0x48,0x30,0xEA,0x9E,0x10,0x12,0x76,0x67,0x80,0x92,0xC6,0xC1,0xD4,0xEE,0x64,0x2F,0x21,0x06,0xEB,0x96,0x00,0x70,0x30,0x1A,0x1C,0x75,0x3F,0x08,0xD9,0xFF,0x28,0xE3};//ECB�����ݼ��ܽ���ģʽ�µ�����
	
	while(1)
	{
		if(sleeptype==0)
		{
			if(cls_show_msg("��ȴ������Զ�����,���ߺ���,���Ѻ󰴼�����,��ȡ�����˳�")==ESC)
				return;
		}
		else
		{
			if(cls_show_msg("��������󽫽��뺯������,���ߺ���,��ȡ�����˳�")==ESC)
				return;
#if ANDIROD_ENABLE
			if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
			if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
			{
				cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
			sleep(2);//���ߺ�����Ҫʱ����Ч
		}

		//case:CBC�����ݼ���ģʽ
		memset(out_buf, 0, sizeof(out_buf));
		if((ret=NDK_AlgSM4Compute(CBC_key, CBC_vector, sizeof(CBC_max_data), CBC_max_data, out_buf, ALG_SM4_ENCRYPT_CBC))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if(memcmp(out_buf, CBC_max_cryp, sizeof(CBC_max_cryp)))
		{
			cls_show_msg("line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}

		//case:CBC�����ݽ���ģʽ
		memset(out_buf, 0, sizeof(out_buf));
		if((ret=NDK_AlgSM4Compute(CBC_key, CBC_vector, sizeof(CBC_max_cryp), CBC_max_cryp, out_buf, ALG_SM4_DECRYPT_CBC))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}	
		if(memcmp(out_buf, CBC_max_data, sizeof(CBC_max_data)))
		{
			cls_show_msg("line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}

		//case:ECB�����ݼ���ģʽ
		memset(out_buf1, 0, sizeof(out_buf1));
		if((ret=NDK_AlgSM4Compute(ECB_key, NULL, strlen((char*)ECB_max_data), ECB_max_data, out_buf1, ALG_SM4_ENCRYPT_ECB))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if(memcmp(out_buf1, ECB_max_cryp, sizeof(ECB_max_cryp)))
		{
			cls_show_msg("line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}

		//case:ECB�����ݽ���ģʽ
		memset(out_buf1, 0, sizeof(out_buf1));
		if((ret=NDK_AlgSM4Compute(ECB_key, NULL, sizeof(ECB_max_cryp), ECB_max_cryp, out_buf1, ALG_SM4_DECRYPT_ECB))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			return;
		}
		if(memcmp(out_buf1, ECB_max_data, strlen((char*)ECB_max_data)))
		{
			cls_show_msg("line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}
		cls_show_msg1(1,"���߻��Ѻ�SM4����ͨ��");//20170115 ������ʾ��߲�������
		//return;
	}	
}

static void sec_sm(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	cls_show_msg1(3, "��ȷ���Ѵ��Զ����߹���");
	
	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("1.sm2�ӽ��ܲ���\n"
								"2.sm2ǩ����ǩ����\n"
								"3.sm3���� 4.sm4����\n"
							    "5.�������߷�ʽ");

		switch (nKeyin)
		{
		case '1':
			sec_sm2_cryp();
			break;
		case '2':
			sec_sm2_sign();
			break;
		case '3':
			sec_sm3();
			break;
		case '4':
			sec_sm4();
			break;
		case '5':
			set_sleeptype();
			break;	
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}
#else
static void sec_sm(void){cls_show_msg1(5,"��֧�ֹ��ܰ�ȫ�㷨");return;}
#endif

static void sec_abnormity(void)
{

	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("1.��ȫ����\n"
							"2.�������\n"
							    "3.ksnֵУ��\n"
							        "4.�������߲���\n");

		switch (nKeyin)
		{
		case '1':
			sec_tamper();
			break;
		case '2':	
			sec_powerdown();
			break;
		case '3':
			sec_ksn();
			break;
		case '4':
			sec_sm();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

#if PINKEY_ENABLE
static void sec_PINtest(void)
{
	int ret = -1,i = 0, randnum=0;
	uint cfginfo = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	//uchar szPinOut[8+1]={0};
	uchar szPan[]={"6225885916163157"};
	uchar KSN[11]={0};
	ST_SEC_KCV_INFO KcvInfoIn;
	uchar DukptKeyValue[16]={0xD5,0xD4,0x4F,0xF7,0x20,0x68,0x3D,0x0D,0x70,0xBE,0x25,0x28,0x18,0xE2,0xA1,0x8A};//DUKPY��ʼ������Կ	
#if SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};
	EM_SEC_PIN alg_type[2]={SEC_PIN_ISO9564_0, SEC_PIN_SM4_2};
#else
	int algnum = 1;
	int key_flag[2] = {0,0} ;
	EM_SEC_PIN alg_type[2]={SEC_PIN_ISO9564_0, SEC_PIN_ISO9564_0};
#endif
#if SEC_AES_ENABLE 
	ST_EXTEND_KEYBLOCK stExtendKey;
	char keyval[17] = {0};
#endif
	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		return;
	}
	for(i=0;i<algnum;i++)
	{
		//��װTMK, ID=5
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=5;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		stKeyInfo.nDstKeyLen=16;
		if(i==0)
			memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
		else
			memset(stKeyInfo.sDstKeyValue, 0x21, stKeyInfo.nDstKeyLen);
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(ret=%d,i=%d)", __LINE__, ret, i);
			return;
		}


		//��װTPK1(8bytes), ID=2,���İ�װ
		stKeyInfo.ucScrKeyIdx=5;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|key_flag[i];
		stKeyInfo.nDstKeyLen=16;
		if(i==0)
			memcpy(stKeyInfo.sDstKeyValue, "\x14\x5F\x5C\x6E\x3D\x91\x44\x57\x14\x5F\x5C\x6E\x3D\x91\x44\x57", stKeyInfo.nDstKeyLen);/*��Ӧ��key����Ϊ8���ֽڵ�0x15*/
		else
			memcpy(stKeyInfo.sDstKeyValue, "\x76\x94\xFA\x66\xC5\xAA\x29\x12\x20\x66\x32\x4D\x62\xA2\x5A\x02", stKeyInfo.nDstKeyLen);/*��Ӧ��key����Ϊ16���ֽڵ�0x23*/
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(ret=%d,i=%d)", __LINE__, ret, i);
			return;
		}
	}
#if SEC_AES_ENABLE
	if(f_sdk3)
	{
		//��װTMK, ID=5
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=5;
		stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memset(keyval, 0x31, stExtendKey.len);
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(ret=%d)", __LINE__, ret);
			return;
		}

		//��װTPK1(8bytes), ID=2,���İ�װ
		stKeyInfo.ucScrKeyIdx=5;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=2;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 16;
		memcpy(keyval, "\x47\x69\xEA\x73\x14\x71\xD5\x87\xE7\xCA\x12\x48\xD6\x8F\x5D\x25", stExtendKey.len);/*��Ӧ��key����Ϊ16���ֽڵ�0x17*/
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			cls_show_msg("line %d:����ʧ��(ret=%d)", __LINE__, ret);
			return;
		}
	}
#endif

	//��װID=6��DUKPT��Կ,���İ�װ
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);//��ʼKSN
	if ((ret=NDK_SecLoadTIK(6, 0, 16, DukptKeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		return;
	}

#if !(OVERSEAS_ENABLE||defined N910P)
	//��ȡĬ�ϵİ�ȫ�ȼ�
	if(( ret = NDK_SecGetCfg(&cfginfo))!=NDK_OK)
	{
		cls_show_msg("line %d:��ȡ��ȫ�ȼ�ʧ��(ret=%d)", __LINE__ , ret);
		return;
	}	
	if(cls_show_msg("�Ƿ�����д������ƿ���,��[Enter],��[����]")==ENTER)
	{
		//���ø߰�ȫ�ȼ�
		if(( ret = NDK_SecSetCfg(0x08))!=NDK_OK)
		{
			cls_show_msg("line %d:���ð�ȫ�ȼ�ʧ��(ret=%d)", __LINE__, ret);
			return;
		}	
	}
#endif

	for(i=0;i<125;i++)//PIN���д���Ϊ120��ÿСʱ
	{
#if K21_ENABLE
		NDK_SysMsDelay(100);
#endif
		cls_printf("���ڽ��е�%d��PIN���д���������...", i+1);
		if(algnum==2)
			randnum=rand()%3;		
		else
			randnum=rand()%2;
		if(randnum==0)
		{
			if((ret=touchscrean_getnum())!=NDK_OK)
			{
				cls_show_msg("line %d:������ּ��̳�ʼ��ʧ��(ret=%d)", __LINE__, ret);
				NDK_SecSetCfg(cfginfo);
				return;
			}
			if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, NULL/*szPinOut*/, alg_type[0], 50*100))!=NDK_OK)
			{
				cls_show_msg("DES�㷨��,PIN����ﵽ���д�������(%d��)",i+1);
				NDK_SecSetCfg(cfginfo);
				return;
			}
		}
		else if(randnum==1)
		{
			if((ret=touchscrean_getnum())!=NDK_OK)
			{
				cls_show_msg("line %d:������ּ��̳�ʼ��ʧ��(ret=%d)", __LINE__, ret);
				NDK_SecSetCfg(cfginfo);
				return;
			}
			if((ret=NDK_SecGetPinDukpt(6, (uchar *)"0", szPan, KSN, NULL/*szPinOut*/, alg_type[0], 50*100))!=NDK_OK)
			{
				cls_show_msg("DUKPT PIN����ﵽ���д�������(%d��)",i+1);
				NDK_SecSetCfg(cfginfo);
				return;
			}
		}
		else 
		{
			if((ret=touchscrean_getnum())!=NDK_OK)
			{
				cls_show_msg("line %d:������ּ��̳�ʼ��ʧ��(ret=%d)", __LINE__, ret);
				NDK_SecSetCfg(cfginfo);
				return;
			}
			if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, NULL/*szPinOut*/, alg_type[1], 50*100))!=NDK_OK)
			{
				cls_show_msg("SM4�㷨��,PIN����ﵽ���д�������(%d��)",i+1);
				NDK_SecSetCfg(cfginfo);
				return;
			}
		}
	}
//��AES��pin������ȡ��������һ��������20180808 modify
#if SEC_AES_ENABLE
	if(f_sdk3)
	{	
		for(i=0;i<125;i++)//PIN���д���Ϊ120��ÿСʱ
		{
#if K21_ENABLE
			NDK_SysMsDelay(100);
#endif
			cls_printf("���ڽ��е�%d��PIN���д���������...", i+1);		
			if((ret=touchscrean_getnum())!=NDK_OK)
			{
				cls_show_msg("line %d:������ּ��̳�ʼ��ʧ��(ret=%d)", __LINE__, ret);
				NDK_SecSetCfg(cfginfo);
				return;
			}
			if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, NULL/*szPinOut*/, SEC_PIN_ISO9564_4, 50*100))!=NDK_OK)		
			{
				cls_show_msg("AES�㷨��,PIN����ﵽ���д�������(%d��,ret=%d)",i+1,ret);
				NDK_SecSetCfg(cfginfo);
				return;
			}
		}
	}
#endif
	cls_show_msg("PIN�������д���δ����");
	//�ָ�Ĭ�ϵİ�ȫ�ȼ�
	if(( ret = NDK_SecSetCfg(cfginfo))!=NDK_OK)
	{
		cls_show_msg("line %d:���ð�ȫ�ȼ�ʧ��(ret=%d)", __LINE__ , ret);
		return;
	}	
	return;
}
#else
static void sec_PINtest(void){cls_show_msg1(5,"��֧��PIN����");return;}
#endif

#if 0
//�ִֵض�PCIҪ���е�һЩ��ȫҪ����в���(�����ڵ�Ԫ�������Ѹ��ǵĲ��Ե�,�ڱ��������ظ�����)
static void sec_PCItest(void)
{
	/*private & local definition*/
	int i = 0,nRet = 0;
	uint cfginfo = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	char sKey[8+1]={0x34,0x78,0x12,0x67,0x93,0x11,0x54,0x28};
	uchar sOutData[8+1]={0};
	uchar szBuf[]={"879423714192374092137"};

	/*process body*/
	cls_show_msg1(2, "���з��񳬴β�����...\n");

	//Ϊ���������������ĸ���,���뱾����ǰ�Ƚ���ȫ��key�Ĳ���
	NDK_SecKeyErase();

	//ͨ��NDK_SecSetIntervaltime�Ѽ��ʱ����С,���ڲ����ֳ�ʱ����½��г�����֤

#if 0
	if((nRet = NDK_SecSetIntervaltime(0,4))!=NDK_OK)
	{
		cls_show_msg("%d,���ü��ʱ��ʧ��%d", __LINE__,nRet);
		return;
	}
#endif

	//���İ�װTAK�����Ͻ���MAC����(ѡȡMAC������Ϊ��������Ĵ�����)ֱ������,�������Ҫ
	//���İ�װTPK�����Ͻ���PIN����(ѡȡPIN������Ϊ��������Ĵ�����)ֱ������,�������Ҫ
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.ucDstKeyIdx = 1;
	stKeyInfo.nDstKeyLen = 8;
	memcpy(stKeyInfo.sDstKeyValue,sKey,8);
	if ((nRet=NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!=NDK_OK)
	{
		cls_show_msg("line %d:NDK_SecLoadKeyװ����Կʧ��(ret=%d)", __LINE__ , nRet);
		return;
	}
	
	//��ȡĬ�ϵİ�ȫ�ȼ�
	if(( nRet = NDK_SecGetCfg(&cfginfo))!=NDK_OK)
	{
		cls_show_msg("line %d:��ȡ��ȫ�ȼ�ʧ��(ret=%d)", __LINE__ , nRet);
		return;
	}	
	if(cls_show_msg("�Ƿ����ø߰�ȫ�ȼ�,��[Enter],��[����]")==ENTER)
	{
		//���ø߰�ȫ�ȼ�
		if(( nRet = NDK_SecSetCfg(0x08))!=NDK_OK)
		{
			cls_show_msg("line %d:���ð�ȫ�ȼ�ʧ��(ret=%d)", __LINE__ , nRet);
			return;
		}	
	}

	for(i=0; i<MAXCNT; i++)
	{
		cls_printf("���з��񳬴β�����,��%d��MAC������...", i+1);
		if(NDK_SecGetMac(1,szBuf,sizeof(szBuf),sOutData,SEC_MAC_ECB)!=NDK_OK)
		{
			cls_show_msg("���з��񳬴β������(�ѳɹ�ʹ��%d��,�����Ѵ�ʹ������)", i+1);
			NDK_SecSetCfg(cfginfo);
			return;
		}
	}
	cls_show_msg("���з��񳬴β���ʧ��,ʹ�ô��������������(%d)!", MAXCNT);
	//�ָ�Ĭ�ϵİ�ȫ�ȼ�
	if(( nRet = NDK_SecSetCfg(cfginfo))!=NDK_OK)
	{
		cls_show_msg("line %d:���ð�ȫ�ȼ�ʧ��(ret=%d)", __LINE__ , nRet);
		return;
	}	
	return;
}
#endif

static void sec_keyspace(void)
{
	#include <stdlib.h>
	/*private & local definition*/
	#define MAXGN 255
	int nRet = 0,gid = 0,loop=0,nNum1=0,nNum2=0,nNum3=0,nNum4=0, i=0;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	char szBcd[16+1]={0};
#if SEC_SM4_ENABLE
	int algnum = 2;//�㷨����
	EM_SEC_KEY_ALG key_flag[2] = {SEC_KEY_DES , SEC_KEY_SM4};
	char *cal_type[] = {"DES", "SM4"};
#else
	int algnum = 1;
	int key_flag[2] = {0,0} ;
	char *cal_type[] = {"DES"};
#endif
#if SEC_AES_ENABLE 
	ST_EXTEND_KEYBLOCK stExtendKey;
#endif
	
	/*process body*/
	//Ϊ���������������ĸ���,���뱾����ǰ�Ƚ���ȫ��key�Ĳ���
	NDK_SecKeyErase();
	for(i=0;i<algnum;i++)
	{
		memset(&stKeyInfo,0,sizeof(stKeyInfo));
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn)); 

		cls_show_msg1(2, "%s��Կ�ռ������...\n",cal_type[i]);
		//���İ�װ���TMK��Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{	
			cls_printf("����Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=0;
			stKeyInfo.ucScrKeyType=0;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			stKeyInfo.nDstKeyLen=16;
			for (loop=0; loop<stKeyInfo.nDstKeyLen; loop++)
				szBcd[loop] = rand()%256;
			memcpy(stKeyInfo.sDstKeyValue,szBcd,stKeyInfo.nDstKeyLen);
			
			if (NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)!= NDK_OK)
				break;
		}
		nNum1 = gid;
		cls_show_msg1(5,"%s�㷨��,�������Կ�ռ�Ϊ%d��\n", cal_type[i], nNum1);
		
		if(cls_show_msg1(10,"�Ƿ���PIN��Կ�ռ�,[������]:��,[ȷ�ϼ�]����\n")!=ENTER)
		{
		cls_show_msg1(2, "��Կ�ռ������...\n");
		//Pin��Կ�����Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{
			cls_printf("PIN��Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=1;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|key_flag[i];
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			stKeyInfo.nDstKeyLen=16;
			for (loop=0; loop<stKeyInfo.nDstKeyLen; loop++)
				szBcd[loop] = rand()%256;
			memcpy(stKeyInfo.sDstKeyValue,szBcd,stKeyInfo.nDstKeyLen);

			if ((nRet = NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)) != NDK_OK)
			{	
				break;
			}	
		}
		nNum2 = gid;
		cls_show_msg1(5,"%s�㷨��,���Pin��Կ�ռ�Ϊ%d��\n", cal_type[i], nNum2);
		}

		if(cls_show_msg1(10,"�Ƿ���MAC��Կ�ռ�,[������]:��,[ȷ�ϼ�]����\n")!=ENTER)
		{
		cls_show_msg1(2, "��Կ�ռ������...\n");
		//Mac��Կ�����Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{
			cls_printf("MAC��Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=2;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|key_flag[i];
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			stKeyInfo.nDstKeyLen=16;
			for (loop=0; loop<stKeyInfo.nDstKeyLen; loop++)
				szBcd[loop] = rand()%256;
			memcpy(stKeyInfo.sDstKeyValue,szBcd,stKeyInfo.nDstKeyLen);

			if ((nRet = NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)) != NDK_OK)
			{	
				break;
			}	
		}
		nNum3 = gid;
		cls_show_msg1(5,"%s�㷨��,���Mac��Կ�ռ�Ϊ%d��\n", cal_type[i], nNum3);
		}

		if(cls_show_msg1(10,"�Ƿ���DATA��Կ�ռ�,[������]:��,[ȷ�ϼ�]����\n")!=ENTER)
		{
		cls_show_msg1(2, "��Կ�ռ������...\n");
		//Data��Կ�����Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{
			cls_printf("DATA��Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=3;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|key_flag[i];
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|key_flag[i];
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			stKeyInfo.nDstKeyLen=16;
			for (loop=0; loop<stKeyInfo.nDstKeyLen; loop++)
				szBcd[loop] = rand()%256;
			memcpy(stKeyInfo.sDstKeyValue,szBcd,stKeyInfo.nDstKeyLen);
			
			if ((nRet = NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)) != NDK_OK)
			{	
				break;
			}	
		}
		nNum4 = gid;
		cls_show_msg1(5,"%s�㷨��,���Data��Կ�ռ�Ϊ%d��\n", cal_type[i], nNum4);
		}
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�㷨��,��Կ�ռ����ͨ��:��⵽��Կ�洢�ռ�����Կ:%d�鷢ɢPIN:%d��,MAC:%d��,DATA:%d��", cal_type[i], nNum1,nNum2,nNum3,nNum4);
	}	
	
//AES������ȡ������һ������������ 
#if SEC_AES_ENABLE
	if(f_sdk3)
	{
		memset(&stKeyInfo,0,sizeof(stKeyInfo));
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn)); 
		memset(&stExtendKey,0,sizeof(stExtendKey));

		cls_show_msg1(2, "AES��Կ�ռ������...\n");
		//���İ�װ���TMK��Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{	
			cls_printf("����Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=0;
			stKeyInfo.ucScrKeyType=0;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			for (loop=0; loop<16; loop++)
				szBcd[loop] = rand()%256;
			stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
			stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
			stExtendKey.len = 16;
			stExtendKey.pblock = szBcd;
			memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
			
			if (NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)!= NDK_OK)
				break;
		}
		nNum1 = gid;
		cls_show_msg1(5,"AES�㷨��,�������Կ�ռ�Ϊ%d��\n", nNum1);
		
		if(cls_show_msg1(10,"�Ƿ���PIN��Կ�ռ�,[������]:��,[ȷ�ϼ�]����\n")!=ENTER)
		{
		cls_show_msg1(2, "��Կ�ռ������...\n");
		//Pin��Կ�����Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{
			cls_printf("PIN��Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=1;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			for (loop=0; loop<16; loop++)
				szBcd[loop] = rand()%256;
			stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
			stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
			stExtendKey.len = 16;
			stExtendKey.pblock = szBcd;
			memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

			if ((nRet = NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)) != NDK_OK)
			{	
				break;
			}	
		}
		nNum2 = gid;
		cls_show_msg1(5,"AES�㷨��,���Pin��Կ�ռ�Ϊ%d��\n", nNum2);
		}

		if(cls_show_msg1(10,"�Ƿ���MAC��Կ�ռ�,[������]:��,[ȷ�ϼ�]����\n")!=ENTER)
		{
		cls_show_msg1(2, "��Կ�ռ������...\n");
		//Mac��Կ�����Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{
			cls_printf("MAC��Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=2;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			for (loop=0; loop<16; loop++)
				szBcd[loop] = rand()%256;
			stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
			stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
			stExtendKey.len = 16;
			stExtendKey.pblock = szBcd;
			memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

			if ((nRet = NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)) != NDK_OK)
			{	
				break;
			}	
		}
		nNum3 = gid;
		cls_show_msg1(5,"AES�㷨��,���Mac��Կ�ռ�Ϊ%d��\n", nNum3);
		}

		if(cls_show_msg1(10,"�Ƿ���DATA��Կ�ռ�,[������]:��,[ȷ�ϼ�]����\n")!=ENTER)
		{
		cls_show_msg1(2, "��Կ�ռ������...\n");
		//Data��Կ�����Կ�ռ�
		for(gid=0; gid<MAXGN; gid++)
		{
			cls_printf("DATA��Կ�ռ������...\nװ�ص�%d��",gid+1);
			stKeyInfo.ucScrKeyIdx=3;
			stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_AES;
			stKeyInfo.ucDstKeyIdx=gid+1;
			memset(szBcd,0,sizeof(szBcd));
			memset(stKeyInfo.sDstKeyValue,0,sizeof(stKeyInfo.sDstKeyValue));
			for (loop=0; loop<16; loop++)
				szBcd[loop] = rand()%256;
			stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
			stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
			stExtendKey.len = 16;
			stExtendKey.pblock = szBcd;
			memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
			
			if ((nRet = NDK_SecLoadKey(&stKeyInfo,&stKcvInfoIn)) != NDK_OK)
			{	
				break;
			}	
		}
		nNum4 = gid;
		cls_show_msg1(5,"AES�㷨��,���Data��Կ�ռ�Ϊ%d��\n", nNum4);
		}
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"AES�㷨��,��Կ�ռ����ͨ��:��⵽��Կ�洢�ռ�����Կ:%d�鷢ɢPIN:%d��,MAC:%d��,DATA:%d��", cal_type[i], nNum1,nNum2,nNum3,nNum4);
	}
#endif
	NDK_SecKeyErase();
	return;
}

void systest7(void)
{
	/*private & local definition*/
	int keyin = 0;
#if ANDIROD_ENABLE
	int ret = -1;
	unsigned int unLen=0;
	char sBuf[128]={0};
#endif
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);

#if ANDIROD_ENABLE
	//�ж���2.0����3.0�汾  20180725 sull add
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BIOS_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:��ȡBIOS��Ϣʧ��(%d)", __LINE__, ret);
		return;
	}
	if((sBuf[5]-'0')==3 ||(sBuf[5]-'0')==0 ) //��6λΪ3����sdk3.0�������Ķ���sdk2.0   PCI�汾2.07_00_01025801 ��6λΪ0
		f_sdk3 = 1;
#endif

	if(auto_flag==1)//autotest_flag
	{
		//�Զ�����������
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		
		//ִ��ѹ������
		sec_press();

		//ִ�����ܲ���
		sec_ability();

		//�ռ���
		sec_keyspace();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}

	//����ѡ����Ծ��ṩ�˵�,������ѡ��,�򲻱��ṩ�˵�
	//����ѹ��,ѹ������ͨ����,�ſ�������
	while(1)
	{
		keyin = cls_show_msg("1.ѹ�� 2.����\n"
													"3.�쳣\n"
													"5.�ռ���\n"
													"6.PIN");
		switch(keyin) 
		{
		case '1':// 1�� ������֤Ӳ�������D-BUS���ߵ��ȶ����뽡׳��
			/*
			���Ը�Ҫ���:
			ͨ��NDK_SecSetIntervaltime�Ѽ��ʱ����С,����MAC�����ѹ������(ѡȡMAC������Ϊ��ȫ����Ĵ�����)
			*/
			sec_press();
			break;
		case '2':
			/*
			���Ը�Ҫ���:��Ҫ��������������(ȡ��������ƽ��ֵ)
			NDK_SecKeyErase, ����load����key(TMK(˫����)/TIK/RSA), ����load����key(TDK(������)/TDK(˫����)/TDK(������)), 
			Ӳ��(T)DES����(��NDK_SecCalcDes,����8bytes,�ֱ���8(ECB)/16(CBC)/24(��ģʽ)����Կ��������), 
			Ӳ��DUKPT (T)DES����(��NDK_SecCalcDesDukpt,����8bytes(����ʹ����Ӳ������һ����),ģʽ����), 			
			���(T)DES����(��NDK_AlgTDes,����8bytes(����ʹ����Ӳ������һ����),�ֱ���8/16/24����Կ��������),
			SHA1/SHA256/SHA512ͬһ1K����
			�������RSA��(512/1024/2048/...)
			Ӳ��RSA���ܼ�����(������ָʹ��ExpΪ3��0x10001��Կ(Kpub)��������,������ָʹ��Kpub��Ӧ��Kpri��������),
			���RSA���ܼ�����(������ָʹ��ExpΪ3��0x10001��Կ(Kpub)��������,������ָʹ��Kpub��Ӧ��Kpri��������)(IN�����������һ��)
			*/
			sec_ability();
			break;
		case '3':
			sec_abnormity();
			break;
		//case '4'://�ִֵض�PCIҪ���е�һЩ��ȫҪ����в���(�����ڵ�Ԫ�������Ѹ��ǵĲ��Ե�,�ڱ��������ظ�����)
			//sec_PCItest();
			//break;
		case '5':
			sec_keyspace();
			break;			
		case '6':
			sec_PINtest();
			break;	
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	//test over
	return;
}

