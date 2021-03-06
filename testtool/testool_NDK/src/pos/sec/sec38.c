/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: 安全模块
* file name		: sec38.c
* Author 		:  	
* version		: 
* DATE			:  
* directory 		: 
* description		: 测试NDK_SecGetPin,NDK_SecGetPinResult函数AES PIN计算接口功能
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
#define TESTAPI	"NDK_SecGetPin,NDK_SecGetPinResult"	
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
* input parameter	 	:无
* output parameter	 	:无
* return value		 	:无
* history 		 		:  author		date	remarks
*			  	  	      jiangym	  20180322  created
*****************************************************************/
void sec38(void)
{
	int ret = 0, nStatus = 0, i = 0;
	char keyval[33] = {0};
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_EXTEND_KEYBLOCK stExtendKey;
	uchar szPinOut[17]={0}, PinAESOut[17]={0}, PinAESOut1[17]={0}, PinAESOut2[17]={0};
	uchar *szPan=(uchar *)"6225885916163157";
	uchar szPan1[16]={0x46,0x22,0x58,0x85,0x91,0x61,0x63,0x15,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
	//uchar szPan2[16]={0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //使用主账号填充方式2
	//uchar PinKeyValue[16]={0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19};//密钥明文
#if	0//TR31_ENABLE    
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0}, skey[17]={0};
#endif
#if DUKPT_ENABLE	//支持DUKPT密钥密钥体系的才进行DUKPT密钥安装
	uchar sKeyValueIn[17]={0},sKsnIn[11]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	uchar *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif
	if(!flag_sdk3)
	{
		send_result("line %d:sdk2.0版本不支持%s,无需测试", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "测试%s...", TESTAPI); 
	//测试前置:擦除所有密钥
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//以SEC_KCV_NONE方式明文安装TMK, ID=5
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x11, stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//以SEC_KCV_NONE方式明文安装TAK, ID=1
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x13, stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//以SEC_KCV_NONE方式密文安装TPK1（由TMK发散）, ID=2
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memcpy(keyval, "\x0A\x8D\xF7\xD1\x7C\xBF\x0C\xBA\x66\x84\xF4\x37\x66\xE6\x4B\x47", stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//AES 暂时不支持 TR31格式安装
#if 0//TR31_ENABLE	//支持TR31格式密钥安装的设备才进行TR31测试
	//以TR31格式打包安装TPK2（由TMK发散）, ID=3
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x11, 16);/*由TMK发散*/
	memset(skey, 0x17, 8);	/*TPK2密钥明文:8个字节的0x17+8个字节0x19*/
	memset(skey+8, 0x19, 8);	
	
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_PIN_TYPE, 4);	/*TPK*/
	tr31_st.key = skey;
	tr31_st.keylen = 16;
	
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	p=(ST_EXTEND_KEYBLOCK*)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	p->pblock=(char *)block;
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//不支持TR31格式的产品改用明文安装方式安装密钥
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x17, stExtendKey.len/2);
	memset(keyval+stExtendKey.len/2, 0x19, stExtendKey.len/2);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//以SEC_KCV_NONE方式明文安装TPK3, ID=4
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x19, stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//明文安装RSA, ID=6
	RsaKey.usBits=RSA_KEY_LEN_512;
	strcpy((char *)RsaKey.sExponent, "03");
	memcpy(RsaKey.sModulus, Pmod, RsaKey.usBits*2/8);
	if ((ret=NDK_SecLoadRsaKey(6, &RsaKey))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//支持DUKPT密钥密钥体系的才进行DUKPT密钥安装
	//明文安装DUKPT,ID=7	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(sKeyValueIn, 0x1d, 16);
	memset(sKsnIn, 0x12, 10);
	if ((ret=NDK_SecLoadTIK(7, 0, 16, sKeyValueIn, sKsnIn, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		//新增用例:以SEC_KCV_NONE方式明文安装TPK4, ID=8,32个字节的0x31  20180417 sull modify
		memset(&stKeyInfo, 0, sizeof(stKeyInfo));
		memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=8;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 32;
		memset(keyval, 0x31, stExtendKey.len);
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		stKcvInfoIn.nLen=0;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

 if(auto_flag==0||auto_flag==1)
 {
	//case1.1:异常测试:不初始化触屏获取屏幕按键值，则返回初始化失败
#if ANDIROD_ENABLE && !defined IM81AZ
	if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResult(szPinOut, &nStatus))!=NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.2:异常测试:以SEC_PIN_ISO9564_4(使用主账号方式),主账号pszDataIn为NULL,应失败
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, (uchar *)"0", NULL, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_PARA)		
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.3:异常测试:以SEC_PIN_ISO9564_4,pszExpPinLenIn=13,应失败(密码长度不能超过12)
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, (uchar *)"13", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_PARA)		
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.4:异常测试:若输入时间超过指定时间,应失败
	cls_printf("请不要按键,耐心等待%ds...", PINTIMEOUT_MIN/1000);
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_SECVP_TIMED_OUT/*NDK_ERR_SECP_TIMEOUT*/)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:异常测试:密钥为TAK类型,应失败
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(1, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.6:异常测试:密钥为RSA类型,应失败
#if SEC_RSA_ENABLE
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(6, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//支持DUKPT密钥密钥体系的才进行DUKPT密钥测试
	//case1.7:异常测试:密钥为DUKPT类型,应失败
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(7, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.8:异常测试:当密钥索引指定的密钥不存在时,应失败
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(9, (uchar *)"4", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
 }
 if(auto_flag==0||auto_flag==2)
 {
	//case2:正常测试：在设定等待时间内，不应进入休眠，以SEC_PIN_ISO9564_4,pszExpPinLenIn=0,pszDataIn!=NULL,id=2,应成功
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("请等待60秒后按[确认],等待过程中不应该进入休眠");
#else
	cls_printf("请等待60秒后按[确认],等待过程中不应该进入休眠");
#endif
	if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)		
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("看到本信息前是否进入过休眠\n[ENTER]是,[其他]否")==ENTER)
	{
		send_result("line %d:%s测试失败", __LINE__, TESTAPI);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16)) /*与开发确认后,若pszExpPinLenIn="0"则返回全0*/
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
	
	//case3:正常测试:以SEC_PIN_ISO9564_4,pszExpPinLenIn="0,0,0",pszDataIn=NULL,输入长度限制出现重复"0,0,0"预期按确认返回NDK_OK
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("请尽快按[确认]...");
#else
	cls_printf("请尽快按[确认]...");
#endif
	if((ret=NDK_SecGetPin(2, (uchar *)"0,0,0", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16)) /*与开发确认后,若pszExpPinLenIn="0"则返回全0*/
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
	
	//case4:正常测试:只有输入设定的密码长度时,才可继续,以SEC_PIN_ISO9564_4,pszExpPinLenIn=4,pszDataIn=NULL,id=2,应成功
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入123并确认,若无反应继续输4并确认...");
#else
	cls_printf("尽快输入123并确认,若无反应继续输4并确认...");
#endif	
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret); 
		GOTOERR;
	}
	//分析源码得:主账号经过计算得szPan1   pin密码计算得:44 12 34  +随机数
	//pin密码块与密钥加密得pinkey,pinkey与主账号块进行异或,异或后的结果再与密钥进行AES加密得到pinblock
	//所以对pinblock进行解密,解密后再与主账号进行异或,异或后的结果再解密得到的pin密码块,前几位比较应该一致
	//cls_show_msg("line %d:(%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)", __LINE__, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x44\x12\x34", 3))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}
	
	//case5:正常测试:输入密码的位数应不超过设定值,并且允许退格进行修改,以SEC_PIN_ISO9564_4(使用主账号填充方式1，密码不足位数补随机数),pszExpPinLenIn=6,id=3,应成功
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("请尝试输入1237890后,如不能输入再退格改为123456并确认...");
#else
	cls_printf("请尝试输入1237890后,如不能输入再退格改为123456并确认...");
#endif		
	if((ret=NDK_SecGetPin(3, (uchar *)"6", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 3, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 3, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x46\x12\x34\x56", 4))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}
		
	
	//case7:正常测试:以SEC_PIN_ISO9564_4,pszExpPinLenIn=12,应成功
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入123456789012并确认...");
#else
	cls_printf("尽快输入123456789012并确认...");
#endif	
	if((ret=NDK_SecGetPin(3, (uchar *)"12", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 3, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 3, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x4c\x12\x34\x56\x78\x90\x12", 7))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}
	
	//case8:支持在输密码时,连续按两次取消,以SEC_PIN_SM4_5,pszExpPinLenIn=6,应支持连续两次取消
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入123456并按2次取消...");
#else
	cls_printf("尽快输入123456并按2次取消...");
#endif	
	if((ret=NDK_SecGetPin(3, (uchar *)"6", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_ERR_SECVP_GET_ESC)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//按键保护
	cls_show_msg1(2, "子用例通过!不要再按[取消]");
	
	//case9:正常测试:以SEC_PIN_SM4_2,pszExpPinLenIn="4,5,12"
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入12345并确认...");
#else
	cls_printf("尽快输入12345并确认...");
#endif	
	if((ret=NDK_SecGetPin(2, (uchar *)"4,5,12", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x45\x12\x34\x5a", 4))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}

	//case10:当输出为NULL时,应不该显示出输入按键,以SEC_PIN_SM4_2,pszExpPinLenIn="4",NDK_SecGetPinResult()函数获取 PINBLOCK 
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入4321(将不会显输入按键)并确认...");
#else
	cls_printf("尽快输入4321(将不会显输入按键)并确认...");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"4", szPan, NULL, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !K21_ENABLE//K21平台不支持忙状态判断 lijq
	//新增用例验证处于PIN输入状态下设备处于忙状态，将不会自动休眠,且NDK_SysGoSuspend增加PIN忙状态返回： NDK_ERR_PIN_BUSY = -3107	
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=-3107)
#else
	if((ret=NDK_SysGoSuspend())!=-3107)
#endif
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	
#if 0   /*根据开发人员建议,修改NDK_SecGetPinResult使用*/
	if((ret=NDK_SecGetPinResult(szPinOut, &nStatus))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else	
	do {
		ret=NDK_SecGetPinResult(szPinOut, &nStatus);
		if (ret != 0) {
			send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if (nStatus == SEC_VPP_KEY_ENTER) { 	/*确认键按下，可返回数据*/
			break;
		} else if (nStatus == SEC_VPP_KEY_ESC) {	/*取消键按下，返回错误*/
			send_result("line %d:%s测试失败", __LINE__, TESTAPI);
			goto ERR;
			break;
		} else if (nStatus == SEC_VPP_KEY_PIN || nStatus == SEC_VPP_KEY_BACKSPACE || nStatus == SEC_VPP_KEY_CLEAR ||  nStatus == SEC_VPP_KEY_NULL) {   
			//由于本函数在存在pinblock时，界面由状态栏处理，这里对其他键不处理
			//继续循环等待
			//由于取PIN函数需要非阻塞，即使没值也要返回应用程序处理（判断是否中断退出），因此增加SEC_VPP_NULL 返回值
		} else {
			send_result("line %d:%s测试失败", __LINE__, TESTAPI);
			goto ERR;
			break;
		}
		NDK_SysMsDelay(10);//usleep(10*1000);
	} while(1);
#endif
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x44\x43\x21", 3))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}
	cls_show_msg("按任意键,[确认键]除外,应该可以马上进入休眠,休眠后请唤醒");
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	usleep(1000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#else
	if((ret=NDK_SysGoSuspend())!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);
#endif
	if(cls_show_msg("看到本信息前是否进入过休眠\n[ENTER]是,[其他]否")!=ENTER)
	{
		send_result("line %d:%s测试失败", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case11:正常测试:输入错误pin时,取消重新输入正确pin应该可以成功
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入4321后,再按1次取消,再输入1234并确认...");
#else
	cls_printf("尽快输入4321后,再按1次取消,再输入1234并确认...");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"4", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x44\x12\x34", 3))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}

	//case10:正常测试:在不为预期输入位数时候不让确认
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("尽快输入12345并确认,若无反应继续输6并按确认");
#else
	cls_printf("尽快输入12345并确认,若无反应继续输6并按确认");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"6,4", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x46\x12\x34\x56", 4))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}


	//case12:正常测试:在有预期输入位数限定后应不能超过最大值
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("请尝试输入123456789,应无法输入9,再按确认");
#else
	cls_printf("请尝试输入123456789,应无法输入9,再按确认");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"8,6,4", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut1, 0, sizeof(PinAESOut1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(PinAESOut2, 0, sizeof(PinAESOut2));
	for (i = 0; i < 16; i++) 
	{
		PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
	}
	memset(PinAESOut, 0, sizeof(PinAESOut));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 2, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(PinAESOut, "\x48\x12\x34\x56\x78", 5))
	{
		send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
		GOTOERR;
	}

#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		//case13:正常测试:验证使用32位字节密钥进行pin计算能成功
		if((ret=touchscrean_getnum())!=NDK_OK)
		{
			send_result("line %d:%s随机数字键盘初始化失败(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if ANDIROD_ENABLE
		NDK_ScrPrintf("请尽快输入1234并确认...");
#else
		cls_printf("请尽快输入1234并确认...");
#endif		
		if((ret=NDK_SecGetPin(8, (uchar *)"4", szPan, szPinOut, SEC_PIN_ISO9564_4, PINTIMEOUT_MAX))!=NDK_OK)
		{
			send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		memset(PinAESOut1, 0, sizeof(PinAESOut1));
		if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 8, szPinOut, 16, PinAESOut1, SEC_AES_DECRYPT))!=NDK_OK)
		{
			send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		memset(PinAESOut2, 0, sizeof(PinAESOut2));
		for (i = 0; i < 16; i++) 
		{
			PinAESOut2[i] = PinAESOut1[i] ^ szPan1[i];
		}
		memset(PinAESOut, 0, sizeof(PinAESOut));
		if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 8, PinAESOut2, 16, PinAESOut, SEC_AES_DECRYPT))!=NDK_OK)
		{
			send_result("line %d:%s测试失败(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(memcmp(PinAESOut, "\x44\x12\x34", 3))
		{
			send_result("line %d:%s测试失败(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinAESOut[0],PinAESOut[1],PinAESOut[2],PinAESOut[3],PinAESOut[4],PinAESOut[5],PinAESOut[6],PinAESOut[7],PinAESOut[8],PinAESOut[9],PinAESOut[10],PinAESOut[11],PinAESOut[12],PinAESOut[13],PinAESOut[14],PinAESOut[15]);
			GOTOERR;
		}
	}
}
else
	send_result("line %d:%s部分子用例需手动测试", __LINE__, TESTAPI);

	send_result("%s测试通过", TESTAPI);
ERR://测试后置:擦除所有密钥
	NDK_SecKeyErase();
	return;
}

