/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sslģ��
* file name		: ssl2.c
* Author 			: chenfm
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_SetSSLBlockingMode�ӿڹ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_SetSSLBlockingMode,NDK_GetSSLBlockingMode"

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	:
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm      20120905	created
*****************************************************************/
void ssl2(void)
{
	int ret;
	NDK_HANDLE handle=NULL;	
	int ciphers[]={SSL3_CIPHER_RSA_NULL_MD5, SSL3_CIPHER_RSA_NULL_SHA, SSL3_CIPHER_RSA_RC4_128_MD5, 0x0};

	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:

	//case1: ���ȴ���һ��ssl�������һ���յľ������NDK_SetSSLBlockingModeӦʧ��
	if((ret=NDK_SetSSLBlockingMode(NULL, NDK_SUSPEND))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_GetSSLBlockingMode(NULL))!=NDK_ERR_SSL_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:�Էǿ�handle������ȷ����
	if((handle=NDK_OpenSSLSocket(HANDSHAKE_SSLv23, SSL_AUTH_NONE, ciphers))==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case2.1: ������Ĳ�����Ӧ����orvMODEUNSUPPORTED
	if((ret=NDK_SetSSLBlockingMode(handle, 234))!=NDK_ERR_SSL_MODEUNSUPPORTED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case2.2: ����ǰ�ȴ���һ��ssl���������ȷ����NDK_SetSSLBlockingModeӦ�ɹ�����NDK_OK,��ȡ��ǰ��״̬Ӧ��ǰ�����õı���һ�¡�
	if((ret=NDK_SetSSLBlockingMode(handle, NDK_NOWAIT))!=NDK_OK||NDK_GetSSLBlockingMode(handle)!=NDK_NOWAIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=NDK_SetSSLBlockingMode(handle, NDK_SUSPEND))!=NDK_OK||NDK_GetSSLBlockingMode(handle)!=NDK_SUSPEND)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Ժ���:
	if((ret=NDK_CloseSSLSocket(handle))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}


