/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name		: sec39.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_SecGetKeyOwner�ӿڹ���
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
#define TESTAPI	"NDK_SecGetKeyOwner"	

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
*			  	  	   jiangym			20180529	created
*****************************************************************/
void sec39(void)
{
	int ret = -1;
	char Owner[32] = {0}, OldOwner[32] = {0};
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ�û�ȡ��Կ����
	if((ret = NDK_SecGetKeyOwner(sizeof(OldOwner),OldOwner)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	
	//case1:�����Ƿ�
	if((ret = NDK_SecGetKeyOwner(sizeof(Owner),NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecGetKeyOwner(-1,Owner)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:���ò�ͬ��Կ����,�ܹ���ȡ��ȷ������ֵ
	if((ret=NDK_SecSetKeyOwner("*"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecGetKeyOwner(sizeof(Owner),Owner)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//�Ƚ�
	if(strcmp("*",Owner))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, Owner);
		GOTOERR;
	}
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecGetKeyOwner(sizeof(Owner),Owner)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�
	if(strcmp("com.example.highplattest",Owner))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, Owner);
		GOTOERR;
	}
	if((ret=NDK_SecSetKeyOwner("appname"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecGetKeyOwner(sizeof(Owner),Owner)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�
	if(strcmp("appname",Owner))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, Owner);
		GOTOERR;
	}

	//case3:��ͣ�л�������Կ������Ӧ�û�ȡ���һ�ε���Կ����
	if((ret=NDK_SecSetKeyOwner("*"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecSetKeyOwner("appname"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecGetKeyOwner(sizeof(Owner),Owner)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�
	if(strcmp("com.example.highplattest",Owner))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, Owner);
		GOTOERR;
	}

	//case4:������Կ����������ز���(ɾ��)�� �ٻ�ȡ
	NDK_SecKeyErase();
	if((ret=NDK_SecSetKeyOwner("appname"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��װTDK3(��Կ16�ֽ�),ID=3
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;   
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecKeyDelete(3, SEC_KEY_TYPE_TDK))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SecGetKeyOwner(sizeof(Owner),Owner)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�Ƚ�
	if(strcmp("appname",Owner))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, Owner);
		RETURN;
	}	
	send_result("%s����ͨ��", TESTAPI);
	
ERR:
	NDK_SecSetKeyOwner(OldOwner);
	NDK_SecKeyErase();
	return;
}

