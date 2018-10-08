/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: 
* file name	: 
* Author 		: jiangym
* version		: 
* DATE			: 20140815
* directory 		: 
* description		: ��֤��Ӧ�ÿ���ʹ��*�����Կ����calcdes
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
#include "NDK.h"

void main(void)
{
	int ret;
	uchar udesin[31]={0},udesout[31]={0};
	memset(udesin, 0x20, sizeof(udesin));
	
	NDK_ScrInitGui();
	
	//���ù�����Կ��
	if((ret=NDK_SecSetKeyOwner("*"))!=NDK_OK)
	{
		NDK_ScrPrintf("line:%d������Կ��ʧ��%d",__LINE__,ret);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	
	//��֤ʹ��ID2�ɹ��ҽ����ȷ
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		NDK_ScrPrintf("line:%d��Ӧ��1����ID2��Կ����ʧ��%d",__LINE__,ret);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 16/*8*/))
	{
		NDK_ScrPrintf("line:%d��Ӧ��1���ܽ������\n",__LINE__);
		NDK_ScrPrintf("0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x", udesout[0], udesout[1],udesout[2],udesout[3],udesout[4],udesout[5],udesout[6],udesout[7],udesout[8],udesout[9],udesout[10],udesout[11],udesout[12],udesout[13],udesout[14],udesout[15]);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	
	//case7:*��İ�ȫ���Ʊ��ر�,��֤��Ӧ�ÿ�����*�����TPK��calcdes
	//TPK
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 8, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8*/))!=NDK_OK)
	{
		NDK_ScrPrintf("line:%d��Ӧ��1����TPK��Կ����ʧ��%d",__LINE__,ret);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	if(memcmp(udesout, "\x7B\x5D\x1D\x52\x9D\x34\xCA\x8E\x7B\x5D\x1D\x52\x9D\x34\xCA\x8E", 16/*8*/))
	{
		NDK_ScrPrintf("line:%d��Ӧ��1���ܽ������\n",__LINE__);
		NDK_ScrPrintf("0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x", udesout[0], udesout[1],udesout[2],udesout[3],udesout[4],udesout[5],udesout[6],udesout[7],udesout[8],udesout[9],udesout[10],udesout[11],udesout[12],udesout[13],udesout[14],udesout[15]);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	
	//case6:B����B1��B2��Կ����֤B1��B2��Ӧʧ�ܡ�
	//������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		NDK_ScrPrintf("line:%d B������Կʧ��\n",__LINE__);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	//��֤B2Ӧʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8|SEC_DES_ECBMODE*/))!=NDK_ERR_SECCR_GET_KEY)
	{
		NDK_ScrPrintf("line:%d B������Կ����֤ʧ��\n",__LINE__);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	
	NDK_ScrPrintf("��Ӧ��1ִ�гɹ�\n");
	NDK_ScrRefresh();
	NDK_KbGetCode(0,NULL);
	return;
}

