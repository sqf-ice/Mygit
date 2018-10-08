/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sslģ��
* file name		: ssl9.c
* Author 			: chenfm
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_SSLConnect�ӿڹ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_SSLConnect,NDK_GetSSLConnectStatus"
#define TIMEOUT 60*1000  //(ms)
#define CERTF   "clientcert.pem"
#define KEY     "clientkey.pem"
#define PRIKEY_PWD	"123456"

/****************************************************************
* function name 	 	:
* functional description 	:
* input parameter	 	:
* output parameter	 	:
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm      20120905	created
*****************************************************************/
void ssl4(void)
{
	int ret;
	int state=0;
	NDK_HANDLE handle=NULL;
	ST_SOCKET_ADDR ss_addr;
	int ciphers[]={SSL3_CIPHER_RSA_NULL_MD5, SSL3_CIPHER_RSA_NULL_SHA, SSL3_CIPHER_RSA_RC4_40_MD5, SSL3_CIPHER_RSA_RC4_128_MD5, SSL3_CIPHER_RSA_RC4_128_SHA,
		             SSL3_CIPHER_RSA_RC2_40_MD5, SSL3_CIPHER_RSA_IDEA_128_SHA, SSL3_CIPHER_RSA_DES_40_CBC_SHA, SSL3_CIPHER_RSA_DES_64_CBC_SHA,
		             SSL3_CIPHER_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_DH_DSS_DES_40_CBC_SHA, SSL3_CIPHER_DH_DSS_DES_64_CBC_SHA,
		             SSL3_CIPHER_DH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_DH_RSA_DES_40_CBC_SHA, SSL3_CIPHER_DH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_40_CBC_SHA,
		             SSL3_CIPHER_EDH_DSS_DES_64_CBC_SHA, SSL3_CIPHER_EDH_DSS_DES_192_CBC3_SHA, SSL3_CIPHER_EDH_RSA_DES_40_CBC_SHA,
		             SSL3_CIPHER_EDH_RSA_DES_64_CBC_SHA, SSL3_CIPHER_EDH_RSA_DES_192_CBC3_SHA, SSL3_CIPHER_ADH_RC4_40_MD5, SSL3_CIPHER_ADH_RC4_128_MD5,
		             SSL3_CIPHER_ADH_DES_40_CBC_SHA, SSL3_CIPHER_FZA_DMS_NULL_SHA, SSL3_CIPHER_CK_FZA_DMS_FZA_SHA, SSL3_CIPHER_CK_FZA_DMS_RC4_SHA,
		             SSL3_CIPHER_CK_ADH_DES_64_CBC_SHA, SSL3_CIPHER_CK_ADH_DES_192_CBC_SHA, 0x0};

	cls_printf("����%s...", TESTAPI);

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
	//ʹ��HANDSHAKE_SSLv2
	if((handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv2, SSL_AUTH_NONE, ciphers))==NULL)
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
	//case1:(blocking)��ȷ����
	if((ret=NDK_SetSSLBlockingMode(handle, NDK_SUSPEND))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_OK||state!=SSL_IS_DISCONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}
	if((ret=NDK_SSLConnect(handle, &ss_addr, TIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_OK||state!=SSL_IS_CONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}
	if((ret=NDK_SSLDisconnect(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_OK||state!=SSL_IS_DISCONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}
	if((ret=NDK_CloseSSLSocket(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}        

	//case2:(non-blocking)��ȷ����
	if((handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv2, SSL_AUTH_NONE, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
 	if((ret=NDK_LoadClientCertificate(handle, CERTF, SSL_FILE_PEM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LoadClientPrivateKey(handle, KEY, SSL_FILE_PEM, PRIKEY_PWD))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}   	
	if((ret=NDK_SetSSLBlockingMode(handle, NDK_NOWAIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_OK||state!=SSL_IS_DISCONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}
	if((ret=NDK_SSLConnect(handle, &ss_addr, TIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_OK||state==SSL_IS_CONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}
	if(WaitNDKSSLState(handle, SSL_IS_CONNECTED, TIMEOUT)!=SUCC)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_SSLDisconnect(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_OK||state!=SSL_IS_DISCONNECTED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, state);
		GOTOERR;
	}	

	//case3:����ȷ���ã�Ӧ����ʧ�ܡ�
	//case3.1:���յ�SSL�����Ӧ����ʧ�ܡ�
	if((ret=NDK_SSLConnect(NULL, &ss_addr, TIMEOUT))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.2:���յĵ�ַ��Ӧ����ʧ�ܡ�
	if((ret=NDK_SSLConnect(handle, NULL, TIMEOUT))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.3:ʱ��Ϊ������Ӧ����ʧ�ܡ�
	if((ret=NDK_SSLConnect(handle, &ss_addr, -1))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.4:
	if((ret=NDK_GetSSLConnectStatus(NULL, &state))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3.5:
	if((ret=NDK_GetSSLConnectStatus(handle, NULL))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//�ر�ssl
	if((ret=NDK_CloseSSLSocket(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�����ѹرյ�ssl���Ӧʧ�ܷ��ء�
	if((ret=NDK_SSLConnect(handle, &ss_addr, TIMEOUT))!=NDK_ERR_SSL_ALREADCLOSE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetSSLConnectStatus(handle, &state))!=NDK_ERR_SSL_ALREADCLOSE)
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

