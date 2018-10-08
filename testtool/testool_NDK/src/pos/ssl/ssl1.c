/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sslģ��
* file name		: ssl1.c
* Author 			: chenfm
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_OpenSSLSocket�ӿڹ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_OpenSSLSocket"
#define MULTI_SSL_CHANNEL	(6)

/****************************************************************
* function name 	 	: 
* functional description 	:
* input parameter	 	:
* output parameter	 	:
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm      20120905	created
*****************************************************************/
void ssl1(void)
{
	int ret;
	NDK_HANDLE ssl_handle;
	NDK_HANDLE handle[6]={NULL};
	int i=0;
	int ciphers[]={SSL3_CIPHER_RSA_NULL_MD5, SSL3_CIPHER_RSA_NULL_SHA, SSL3_CIPHER_RSA_RC4_40_MD5, SSL3_CIPHER_RSA_RC4_128_MD5, SSL3_CIPHER_RSA_RC4_128_SHA,
		             SSL3_CIPHER_RSA_RC2_40_MD5, SSL3_CIPHER_RSA_IDEA_128_SHA, SSL3_CIPHER_RSA_DES_40_CBC_SHA, SSL3_CIPHER_RSA_DES_64_CBC_SHA,
		             SSL3_CIPHER_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_DSS_DES_40_CBC_SHA, SSL3_CIPHER_DH_DSS_DES_64_CBC_SHA,
		             SSL3_CIPHER_DH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_40_CBC_SHA, SSL3_CIPHER_DH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_40_CBC_SHA,
		             SSL3_CIPHER_EDH_DSS_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_EDH_RSA_DES_40_CBC_SHA,
		             SSL3_CIPHER_EDH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_ADH_RC4_40_MD5, SSL3_CIPHER_ADH_RC4_128_MD5,
		             SSL3_CIPHER_ADH_DES_40_CBC_SHA, SSL3_CIPHER_FZA_DMS_NULL_SHA, SSL3_CIPHER_CK_FZA_DMS_FZA_SHA, SSL3_CIPHER_CK_FZA_DMS_RC4_SHA,
		             SSL3_CIPHER_CK_ADH_DES_64_CBC_SHA, SSL3_CIPHER_CK_ADH_DES_192_CBC_SHA, 0x0};

	int ciphers1[]={SSL3_CIPHER_RSA_NULL_MD5, SSL3_CIPHER_RSA_NULL_SHA, 100, 0x0};


	cls_printf("����%s...", TESTAPI);

	//case1:�쳣����
	//case1.1
	if(NDK_OpenSSLSocket(123, SSL_AUTH_CLIENT, ciphers)!=NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case1.2
	if(NDK_OpenSSLSocket(HANDSHAKE_SSLv23, 234, ciphers)!=NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case1.3:ciphers=NULL��cipher���Ϸ���Ӧopenʧ�ܡ�
	if(NDK_OpenSSLSocket(HANDSHAKE_SSLv23, SSL_AUTH_CLIENT, NULL)!=NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if(NDK_OpenSSLSocket(HANDSHAKE_SSLv23, SSL_AUTH_CLIENT, ciphers1)!=NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case2: ��ȷ����NDK_OpenSSLSocket�ɹ�����һ��ssl���	
	//case2.1:
	if((ssl_handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv23, SSL_AUTH_CLIENT, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if((ret=NDK_CloseSSLSocket(ssl_handle))!=NDK_OK)
	{
		send_result("line %d:%s�ر�SSLʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	//case2.2: 
	if((ssl_handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv2, SSL_AUTH_NONE, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if((ret=NDK_CloseSSLSocket(ssl_handle))!=NDK_OK)
	{
		send_result("line %d:%s�ر�SSLʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case2.3: //linwl20130718,����NDK V4.3.9���ȡ��SSL_AUTH_CLIENT_FAIL��֤��ʽ��ʹ��֧�ֵ���֤��ʽ�滻��֤
	if((ssl_handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv3, SSL_AUTH_NONE/*SSL_AUTH_CLIENT_FAIL*/, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if((ret=NDK_CloseSSLSocket(ssl_handle))!=NDK_OK)
	{
		send_result("line %d:%s�ر�SSLʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case2.4: 
	if((ssl_handle=NDK_OpenSSLSocket(HANDSHAKE_TLSv1, SSL_AUTH_CLIENT/*SSL_AUTH_CLIENT_FAIL*/, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if((ret=NDK_CloseSSLSocket(ssl_handle))!=NDK_OK)
	{
		send_result("line %d:%s�ر�SSLʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:ͬʱ�򿪶��SSL���
	for(;i<MULTI_SSL_CHANNEL;i++)
	{
		if((handle[i]=NDK_OpenSSLSocket(HANDSHAKE_SSLv23, SSL_AUTH_NONE, ciphers))==NULL)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	for(i=0;i<MULTI_SSL_CHANNEL;i++)
	{
		if((ret=NDK_CloseSSLSocket(handle[i]))!=NDK_OK)
		{
			send_result("line %d:%s�ر�SSLʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
ERR:
	for(i=0;i<MULTI_SSL_CHANNEL;i++)
		NDK_CloseSSLSocket(handle[i]);
	return;
}

