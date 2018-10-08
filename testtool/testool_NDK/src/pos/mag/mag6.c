/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: mag6.c
* Author 			: chenfm
* version			: 
* DATE			: 20121220
* directory 		: 
* description		: ����NDK_MagReadRaw�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_MagReadRaw"

/****************************************************************
* function name 	 	: void mag6(void)
* functional description 	: ��ȡ�ſ���������1��2��3�ŵ���ԭʼ����
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121220	created
*****************************************************************/
/*
static uint ConvetData(uchar *pSrc, uchar *pDest, uint SrcLen)
{
	uchar *pStart;
	uchar *pEnd;
	uint i;

	pStart=pSrc;
	pEnd= &pSrc[SrcLen-1];

	while((*pStart==0)&&(SrcLen!=0))
	{
		pStart++;
		SrcLen--;
	}
	if(SrcLen==0)
	{
		//src len =0
		return 0;
	}
	while((*pEnd==0)&&(SrcLen!=0))
	{
		pEnd--;
		SrcLen--;
	}
	if(SrcLen==0)
	{
		//src len =0
		return 0;
	}
	// read data
	for(i=0;i<SrcLen;i++)
	{
		pDest[i]=pStart[i];
	}		
	return(SrcLen);
}
*/

static int subfunc_3(void)
{
	uchar CardBuf1[MAXTRACKLEN]={0}, CardBuf2[MAXTRACKLEN]={0}, CardBuf3[MAXTRACKLEN]={0};//CardTmp[MAXTRACKLEN]={0}
	ushort punTk1Len=0, punTk2Len=0, punTk3Len=0;//CardTmpLength=0
	const unsigned char track1_data[]={0xd1, 0xf2, 0xb5, 0x9, 0xd2, 0xa2, 0x40, 0xcd, 0x8a, 0xe5, 0x8a, 0xd4, 0x27, 0x4a, 0x89, 0x3, 0x36, 0x2b, 0x96, 0x2b, 0x50, 0x9d, 0x2a, 
									0x24, 0xc, 0xd8, 0xae, 0x58, 0xad, 0x42, 0x74, 0xa8, 0x90, 0x33, 0x62, 0xb9, 0x62, 0xb5, 0x9, 0xd2, 0xa2, 0x40, 0xcd, 0x8a, 0xe5, 0x8a, 0xd4,
									 0x27, 0x4a, 0x89, 0x3, 0x36, 0x2b, 0x96, 0x2b, 0x50, 0x9d, 0x2a, 0x24, 0xc, 0xd8, 0xae, 0x58, 0xad, 0x42, 0x74, 0xa8, 0x90, 0x8a};
	const unsigned char track2_data[]={0xbf, 0x6a, 0x92, 0x62, 0xc, 0x32, 0x83, 0xda, 0xa4, 0x98, 0x83, 0xc, 0xa0, 0xf6, 0xa9, 0x26, 0x20, 0xc3, 0x28, 0x3d, 0xaa, 0x49, 0x88, 0x30, 0x58};
	const unsigned char track3_data[]={0x9f, 0x98, 0x83, 0xc, 0xa0, 0xf6, 0xa9, 0x26, 0x20, 0xc3, 0x28, 0x3d, 0xaa, 0x49, 0x88, 0x30, 0xca, 0xf, 0x6a, 0x92, 0x62, 0xc, 0x32, 0x83, 
									0xda, 0xa4, 0x98, 0x83, 0xc, 0xa0, 0xf6, 0xa9, 0x26, 0x20, 0xc3, 0x28, 0x3d, 0xaa, 0x49, 0x88, 0x30, 0xca, 0xf, 0x6a, 0x92, 0x62, 0xc, 0x32, 0x83,
									 0xda, 0xa4, 0x98, 0x83, 0xc, 0xa0, 0xf6, 0xa9, 0x26, 0x20, 0xc3, 0x28, 0x3d, 0xaa, 0x49, 0x88, 0x30, 0x58};
	int nRet=-1, ret=SUCC;
	uchar pcSwiped=-1;
	//int i=0;

	if((nRet=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		ret=FAIL;
		goto END;
	}
	cls_show_msg("�������ˢ#mag6�ſ���,��[ENTER]����..");
	if((nRet=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=1)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		ret=FAIL;
		goto END;
	}
	if((nRet=NDK_MagReadRaw(CardBuf1, &punTk1Len, CardBuf2, &punTk2Len, CardBuf3, &punTk3Len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		ret=FAIL;
		goto END;
	}
	/* �ŵ����ݶ�ȡ
	CardTmpLength=ConvetData(CardBuf1, CardTmp, punTk1Len);
	for(i=0;i<strlen(CardBuf1);i++)
	{
		fprintf(stderr,"%x,",CardTmp[i]);
	}
	fprintf(stderr,"\n");
	
	CardTmpLength=ConvetData(CardBuf2, CardTmp, punTk2Len);
	for(i=0;i<strlen(CardBuf2);i++)
	{
		fprintf(stderr,"%x,",CardTmp[i]);
	}
	fprintf(stderr,"\n");
	
	CardTmpLength=ConvetData(CardBuf3, CardTmp, punTk3Len);
	for(i=0;i<strlen(CardBuf3);i++)
	{
		fprintf(stderr,"%x,",CardTmp[i]);
	}
	fprintf(stderr,"\n");
	*/
//	CardTmpLength=ConvetData(CardBuf1, CardTmp, punTk1Len);
	if(memcmp((void*)CardBuf1, (void*)track1_data, sizeof(track1_data)/*CardTmpLength*/))//����ԭʼ���ݱ仯����һЩ���ݣ��Ƚ�ֻ�Ƚ�ǰ��ȷ�������� 20140422 linwl
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		ret=FAIL;
		goto END;
	}
	
//	CardTmpLength=ConvetData(CardBuf2, CardTmp, punTk2Len);
	if(memcmp((void*)CardBuf2, (void*)track2_data, sizeof(track2_data)/*CardTmpLength*/))//����ԭʼ���ݱ仯����һЩ���ݣ��Ƚ�ֻ�Ƚ�ǰ��ȷ�������� 20140422 linwl
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		ret=FAIL;
		goto END;
	}	

//	CardTmpLength=ConvetData(CardBuf3, CardTmp, punTk3Len);
	if(memcmp((void*)CardBuf3, (void*)track3_data, sizeof(track3_data)/*CardTmpLength*/))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		ret=FAIL;
		goto END;
	}

END:
	NDK_MagClose();
	return (ret);
}

void mag6(void)
{
	int ret=-1;
	uchar CardBuf1[MAXTRACKLEN]={0}, CardBuf2[MAXTRACKLEN]={0}, CardBuf3[MAXTRACKLEN]={0};
	ushort punTk1Len=0, punTk2Len=0, punTk3Len=0;

	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_MagClose();

	//case1:δ���豸��ֱ�Ӷ���Ӧʧ�ܷ��ء�
	if((ret=NDK_MagReadRaw(CardBuf1, &punTk1Len, CardBuf2, &punTk2Len, CardBuf3, &punTk3Len))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 

	//case2:������ԭʼ���ݣ�Ӧ�óɹ����ء�
	if((ret=subfunc_3())!=SUCC)
	{
		//send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:������Ĳ�����
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_MagReadRaw(NULL, NULL, NULL, &punTk2Len, NULL, &punTk3Len))!=NDK_ERR_PARA
	||(ret=NDK_MagReadRaw(CardBuf1, NULL, CardBuf2, &punTk2Len, CardBuf3, &punTk3Len))!=NDK_ERR_PARA
	||(ret=NDK_MagReadRaw(CardBuf1, &punTk1Len, CardBuf2, NULL, CardBuf3, &punTk3Len))!=NDK_ERR_PARA
	||(ret=NDK_MagReadRaw(CardBuf1, &punTk1Len, CardBuf2, &punTk2Len, CardBuf3, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:�رմſ��豸��ˢ�������ſ���Ӧʧ�ܷ��ء�
	if((ret=NDK_MagClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagReadRaw(CardBuf1, &punTk1Len, CardBuf2, &punTk2Len, CardBuf3, &punTk3Len))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	} 

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
	return;
}

