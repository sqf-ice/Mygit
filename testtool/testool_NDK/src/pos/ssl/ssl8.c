/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sslģ��
* file name		: ssl5.c
* Author 			: chenfm
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_SSLSend�ӿڹ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_SSLSend"
#define TIMEOUT 60*1000 //(ms)
#define CERTF   "clientcert.pem"
#define KEY     "clientkey.pem"
#define PRIKEY_PWD	"123456"
#define MAXSIZE	8096

/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	:
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm      20120905	created
*****************************************************************/
void ssl8(void)
{
	int ret = 0, j = 0;
	NDK_HANDLE handle=NULL;
	ST_SOCKET_ADDR ss_addr;
	size_t send_bye=0,recvlen=0;
	char buf[MAXSIZE] = {0}, senddata[MAXSIZE] = {0};
	int ciphers[]={SSL3_CIPHER_RSA_NULL_MD5, SSL3_CIPHER_RSA_NULL_SHA, SSL3_CIPHER_RSA_RC4_40_MD5, SSL3_CIPHER_RSA_RC4_128_MD5, SSL3_CIPHER_RSA_RC4_128_SHA,
		             SSL3_CIPHER_RSA_RC2_40_MD5, SSL3_CIPHER_RSA_IDEA_128_SHA, SSL3_CIPHER_RSA_DES_40_CBC_SHA, SSL3_CIPHER_RSA_DES_64_CBC_SHA,
		             SSL3_CIPHER_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_DSS_DES_40_CBC_SHA, SSL3_CIPHER_DH_DSS_DES_64_CBC_SHA,
		             SSL3_CIPHER_DH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_40_CBC_SHA, SSL3_CIPHER_DH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_40_CBC_SHA,
		             SSL3_CIPHER_EDH_DSS_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_EDH_RSA_DES_40_CBC_SHA,
		             SSL3_CIPHER_EDH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_ADH_RC4_40_MD5, SSL3_CIPHER_ADH_RC4_128_MD5,
		             SSL3_CIPHER_ADH_DES_40_CBC_SHA, SSL3_CIPHER_FZA_DMS_NULL_SHA, SSL3_CIPHER_CK_FZA_DMS_FZA_SHA, SSL3_CIPHER_CK_FZA_DMS_RC4_SHA,
		             SSL3_CIPHER_CK_ADH_DES_64_CBC_SHA, SSL3_CIPHER_CK_ADH_DES_192_CBC_SHA, 0x0};


	cls_printf("����%s...", TESTAPI);

	for (j=0; j<sizeof(senddata); j++)
		senddata[j]=rand()%256;
	memset(&ss_addr, 0, sizeof(ss_addr));
	ss_addr.unAddrType=SSL_ADDR_IPV4;	
	ss_addr.usPort=netsetting.SvrTcpPort;
	ss_addr.psAddr=netsetting.SvrIP.Str;

	//����ǰ��:
	if(NetUp()!=SUCC)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	//ʹ��HANDSHAKE_SSLv3
	if((handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv3, SSL_AUTH_NONE, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	//���ر���֤��
	if((ret=NDK_LoadClientCertificate(handle,CERTF,SSL_FILE_PEM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����˽Կ
	if((ret=NDK_LoadClientPrivateKey(handle, KEY, SSL_FILE_PEM, PRIKEY_PWD))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SetSSLBlockingMode(handle, NDK_SUSPEND);
	if((ret=NDK_SSLConnect(handle, &ss_addr, TIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1: �ɹ�Ӧ����NDK_OK
	if((ret=NDK_SSLSend(handle, (const char*)("Hello World!"), strlen("Hello World!"), &send_bye))!=NDK_OK||send_bye!=strlen("Hello World!"))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, send_bye);
		GOTOERR;
	}
	memset(buf, 0, sizeof(buf));
	if((ret=NDK_SSLDataAvailable(handle, 5000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_SSLReceive(handle, (void*)buf, send_bye, &recvlen))!=NDK_OK||send_bye!=recvlen)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, recvlen);
		GOTOERR;
	}
	if(memcmp("Hello World!", buf, send_bye))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case2:���ʹ����ݣ�4k����Ӧ���ͳɹ��������ճɹ�
	if((ret=NDK_SSLSend(handle, senddata, sizeof(senddata), &send_bye))!=NDK_OK||send_bye!=sizeof(senddata))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, send_bye);
		GOTOERR;
	}

	memset(buf, 0, sizeof(buf));
	if((ret=NDK_SSLDataAvailable(handle, 5000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_SSLReceive(handle, (void*)buf, send_bye, &recvlen))!=NDK_OK||send_bye!=recvlen)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, recvlen);
		GOTOERR;
	}
	if(memcmp(senddata, buf, send_bye))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6:���Էִ�(����)���ͣ�һ�ν���linwl20130617
	if((ret=NDK_SSLSend(handle, senddata, sizeof(senddata)/2, &send_bye))!=NDK_OK||send_bye!=sizeof(senddata)/2)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, send_bye);
		GOTOERR;
	}
	if((ret=NDK_SSLSend(handle, senddata+sizeof(senddata)/2, sizeof(senddata)/2, &send_bye))!=NDK_OK||send_bye!=sizeof(senddata)/2)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, send_bye);
		GOTOERR;
	}

	memset(buf, 0, sizeof(buf));
	if((ret=NDK_SSLDataAvailable(handle, 5000))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_SSLReceive(handle, (void*)buf, send_bye*2, &recvlen))!=NDK_OK||send_bye*2!=recvlen)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, recvlen);
		GOTOERR;
	}
	if(memcmp(senddata, buf, recvlen))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case3:������Ч�Բ��ԡ�
	//case3.1:
	if((ret=NDK_SSLSend(NULL, senddata, sizeof(senddata), &send_bye))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.2
	if((ret=NDK_SSLSend(handle, NULL, sizeof(senddata), &send_bye))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.3
	if((ret=NDK_SSLSend(handle, senddata, sizeof(senddata), NULL))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�Ͽ�ssl������Ӧʧ�ܡ�
	if((ret=NDK_SSLDisconnect(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SSLSend(handle, "hello world", strlen("hello world"), &send_bye))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:�ر�ssl,����Ӧʧ�ܡ�
	if((ret=NDK_CloseSSLSocket(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SSLSend(handle, "hello world", strlen("hello world"), &send_bye))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Ժ���:
	if(NetDown()!=SUCC)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	NDK_CloseSSLSocket(handle);
	NetDown();
	return;
}

