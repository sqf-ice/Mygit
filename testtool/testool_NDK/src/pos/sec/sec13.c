/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec13.c
* Author 		: 
* version		: 
* DATE			:
* directory 		: 
* description		: ���� NDK_SecVerifyCipherPin �ӿڹ���
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
#define TESTAPI	"NDK_SecVerifyCipherPin"	
#define PINTIMEOUT_MAX	(200*1000)
#define PINTIMEOUT_MIN	(5*1000)

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
*			                sull      20180427       created
*****************************************************************/
//ֻ�ܼ򵥵���֤�����ı���,���ں�������ֵ������ȷ��֤,��Ӧ���ǻ�������п���ȷ�����������ȷ��Ӧ��
void sec13(void)
{
	int ret = -1, nStatus = 0;
	uchar sAtrbuf[32] = {0}, sSW[8+1] = {0};
	int nAtrLen = 0;
	ST_SEC_RSA_KEY RsaKey;
	const uchar mod[]= {
		0x96,0x92,0xF2,0xDC,0x0D,0xFE,0xA1,0x34,0xF6,0xD5,0xDA,0xF7,0x56,0x34,0xCA,0xEC,
		0xC8,0x55,0xEC,0x77,0x4F,0x4B,0xFE,0x6F,0x90,0xDD,0xD4,0x32,0xB5,0xDB,0x1C,0xDC,
		0xFB,0xAC,0x96,0x98,0x02,0xC6,0x4A,0x60,0x69,0xFD,0x26,0x7C,0x41,0xC1,0xAC,0x82,
		0xE3,0x8E,0xA2,0x54,0xF4,0xAA,0x4B,0xD0,0x9B,0x04,0x51,0x6C,0x19,0xE1,0x8A,0xC5
	};
	const uchar exp[]= {0x00,0x00,0x03};
	
	cls_printf("%s������...", TESTAPI);

	//����ǰ��:����������Կ
	if((ret = NDK_SecKeyErase()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	//��ͬic���˴�����Կ���ܲ�ͬ,��������ʹ�ô���Կ��֤����
	memset(&RsaKey, 0,sizeof(ST_SEC_RSA_KEY));
	RsaKey.usBits=RSA_KEY_LEN_512;
	memcpy((char *)RsaKey.sExponent, exp, sizeof(exp));
	memcpy(RsaKey.sModulus, mod, sizeof(mod));

	//case1:����ʼ��������ȡ��Ļ����ֵ���򷵻س�ʼ��ʧ��
#if ANDIROD_ENABLE && !defined IM81AZ
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"0", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MIN)) != NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case2:�����Ƿ�:��ʱʱ�䲻�ںϷ���Χ��
	if((ret = touchscrean_getnum()) != NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4,5,6,7,8,9,10,11,12", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MIN-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = touchscrean_getnum()) != NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4,5,6,7,8,9,10,11,12", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX+1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�����Ƿ�:pszExpPinLenIn=NULL
	if((ret = touchscrean_getnum()) != NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, NULL, &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:δ��IC��������½����ѻ�PIN���㣬Ӧʧ��
	if((ret = touchscrean_getnum()) != NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"0", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MIN)) != NDK_ERR_SECVP_CUSTOMERCARDNOTPRESENT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:IC���ϵ��ε�IC���ٽ����ѻ�PIN���㣬Ӧʧ��
	cls_show_msg("���IC�������꿨�����������...");
	memset(sAtrbuf, 0, sizeof(sAtrbuf));
	if((ret = NDK_IccPowerUp(ICTYPE_IC, sAtrbuf, &nAtrLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���IC���������������...");
	if((ret = touchscrean_getnum()) != NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4,5,12", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MIN)) != NDK_ERR_SECVP_CUSTOMERCARDNOTPRESENT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_IccPowerDown(ICTYPE_IC);

	//����ǰ��:IC���ϵ�
	cls_show_msg("���IC�������꿨�����������...");
	if((ret = NDK_IccPowerUp(ICTYPE_IC, sAtrbuf, &nAtrLen)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if ANDIROD_ENABLE
	//case6:�����Ƿ�:����EMV���ģʽ���в���
	memset(sSW, 0, sizeof(sSW));
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"0", &RsaKey, sSW, ICC_EMV_MODE-1, PINTIMEOUT_MAX)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4", &RsaKey, sSW, ICC_EMV_MODE+1, PINTIMEOUT_MAX)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case7:��������:��pin��������в�Ӧ��������
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if !K21_ENABLE//�Ͷ�û���Զ����߹���
	cls_printf("��ȴ�60���[ȷ��],�ȴ������в�Ӧ�ý�������");
#else
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�뾡�찴[ȷ��]...");
#else
	cls_printf("�뾡�찴[ȷ��]...");
#endif
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"0", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !K21_ENABLE//�Ͷ�û���Զ����߹���	
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif	
	cls_show_msg1(2, "������ͨ��!");

	//case8:��֤���볤�����Ƴ����ظ�"0,0,0"Ԥ�ڰ�ȷ�Ϸ���NDK_OK��Ӧ�÷��ָ�����°�ȷ�ϻ᷵��ʧ��-1121��BUG����˵��������
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�뾡�찴[ȷ��]...");
#else
	cls_printf("�뾡�찴[ȷ��]...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"0,0,0", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:pin�����볤��Ҫ�ﵽҪ�󣬲Ž����ѻ�PIN����
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123��ȷ��,���޷�Ӧ������4��ȷ��...");
#else
	cls_printf("��������123��ȷ��,���޷�Ӧ������4��ȷ��...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4,4", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:֧�ְ��ʵ��ļ����磬���˸�����ɾ��һλ��ǰ�����PIN��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�볢������1237890��,�粻���������˸��Ϊ123456��ȷ��...");
#else
	cls_printf("�볢������1237890��,�粻���������˸��Ϊ123456��ȷ��...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"6", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case11:pin������󳤶�֧�ֵ�12
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123456789012��ȷ��...");
#else
	cls_printf("��������123456789012��ȷ��...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"12", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case12:pin���볤��Ϊ3(�Ϸ�����Ϊ0,4-12)��Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"3", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case13:pin���볤��Ϊ13(�Ϸ�����Ϊ0,4-12)��Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"13", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case14:֧�������˳������ʵ��ļ����磬��ȡ��2�Σ��˳�PIN����״̬
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123456����2��ȡ��...");
#else
	cls_printf("��������123456����2��ȡ��...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"12", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_ERR_SECVP_GET_ESC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������
	cls_show_msg1(2, "������ͨ��!��Ҫ�ٰ�[ȡ��]");

	//case15:������������NULLʱ��Ϊ������ģʽ��Ӧ�ṩȡPIN�������״̬�����ֵ�Ĺ���
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������12345��ȷ��...");
#else
	cls_printf("��������12345��ȷ��...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4,5,12", &RsaKey, NULL, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !K21_ENABLE//K21ƽ̨��֧��æ״̬�ж� 	
	//��֤����PIN����״̬���豸����æ״̬���������Զ�����,��NDK_SysGoSuspend����PINæ״̬���أ� NDK_ERR_PIN_BUSY = -3107	
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_PIN_BUSY)
#else
	if((ret=NDK_SysGoSuspend())!=NDK_ERR_PIN_BUSY)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	do {
		ret=NDK_SecGetPinResult(sSW, &nStatus);
		if (ret != 0) {
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
			break;
		}
		if (nStatus == SEC_VPP_KEY_ENTER) { 	/*ȷ�ϼ����£��ɷ�������*/
			break;
		} else if (nStatus == SEC_VPP_KEY_ESC) {	/*ȡ�������£����ش���*/
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
			break;
		} else if (nStatus == SEC_VPP_KEY_PIN || nStatus == SEC_VPP_KEY_BACKSPACE || nStatus == SEC_VPP_KEY_CLEAR ||  nStatus == SEC_VPP_KEY_NULL) {   
			//���ڱ������ڴ���pinblockʱ��������״̬�����������������������
			//����ѭ���ȴ�
			//����ȡPIN������Ҫ����������ʹûֵҲҪ����Ӧ�ó������ж��Ƿ��ж��˳������������SEC_VPP_NULL ����ֵ
		} else {
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
			break;
		}
		NDK_SysMsDelay(10);//usleep(10*1000);
	} while(1);

	cls_show_msg("�������,[ȷ�ϼ�]����,Ӧ�ÿ������Ͻ�������,���ߺ��뻽��");
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	usleep(1000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	if((ret=NDK_SysGoSuspend())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);
#endif
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case16:����PIN�Ĳ�������������ʱʱ��Ӧ�˳�����ģʽ���������ʵķ���ֵ������뷴�������ʵĶ���
	memset(sSW, 0, sizeof(sSW));
	cls_printf("�벻Ҫ����,���ĵȴ�%ds...", PINTIMEOUT_MIN/1000);
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MIN)) != NDK_ERR_SECVP_TIMED_OUT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case17:֧�����ȫ�������ʵ��ļ����磬��ȡ��1�λ��˸������ǰ�����ȫ��PIN��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������4321��,�ٰ�1��ȡ��,������1234��ȷ��...");
#else
	cls_printf("��������4321��,�ٰ�1��ȡ��,������1234��ȷ��...");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"4", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case18:�ڲ�ΪԤ������λ��ʱ����ȷ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������12345��ȷ��,���޷�Ӧ������6����ȷ��");
#else
	cls_printf("��������12345��ȷ��,���޷�Ӧ������6����ȷ��");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"6,4", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case19:����Ԥ������λ���޶���Ӧ���ܳ������ֵ
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(sSW, 0, sizeof(sSW));
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�볢������123456789,Ӧ�޷�����9,�ٰ�ȷ��");
#else
	cls_printf("�볢������123456789,Ӧ�޷�����9,�ٰ�ȷ��");
#endif
	if((ret = NDK_SecVerifyCipherPin(0, (uchar*)"8,6,4", &RsaKey, sSW, 1/*ICC_EMV_MODE*/, PINTIMEOUT_MAX)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//IC���µ�
	if((ret = NDK_IccPowerDown(ICTYPE_IC)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	NDK_IccPowerDown(ICTYPE_IC);
	return;	
}



