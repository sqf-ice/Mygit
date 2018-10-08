/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ſ�ģ�� 
* file name			: 
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: �ſ�ģ���������ϲ���
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"�ſ�ģ����������"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
	MagOpen=0 ,			 
	MagClose,
	MagSwiped,	
	MagReadNormal,
	MagReset,
#if !K21_ENABLE
	MagReadRaw,
#endif
	MaxNum,//����ö��ֵʱ�ڴ�ֵǰ���,Ҫ��֤��ֵΪ���һ��ֵ
}EM_MAG;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20170831		created
*****************************************************************/
static int card_randtest(int funnum)
{
	int  pnErrorCode=0;
	uchar pcSwiped=-1;
	char pszTk1[MAXTRACKLEN]={0}, pszTk2[MAXTRACKLEN]={0}, pszTk3[MAXTRACKLEN]={0};
#if !K21_ENABLE
	ushort punTk1Len=0, punTk2Len=0, punTk3Len=0;
	uchar CardBuf1[MAXTRACKLEN]={0}, CardBuf2[MAXTRACKLEN]={0}, CardBuf3[MAXTRACKLEN]={0};
#endif
	
	switch(funnum)
	{	
		case MagOpen:  
			NDK_MagOpen();
			break;
		case MagClose:  
			NDK_MagClose();
			break;
		case MagSwiped:	
			NDK_MagSwiped(&pcSwiped);
			break;
		case MagReadNormal:  
			NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode);
			break;
		case MagReset:  
			NDK_MagReset();
			break;
#if !K21_ENABLE
		case MagReadRaw:  
			NDK_MagReadRaw(CardBuf1, &punTk1Len, CardBuf2, &punTk2Len, CardBuf3, &punTk3Len);
			break;	
#endif
	}	
	return SUCC;
}

void mag200(void)
{
	/*private & local definition*/
	int  i = 0, bak = 0, num[20] ={0}, j = 0; 
	int ret = -1,cnt =50, len = 0;//cnt ����
#if !K21_ENABLE
	char *magstr[] = {"NDK_MagOpen","NDK_MagClose","NDK_MagSwiped","NDK_MagReadNormal","NDK_MagReset"};
#else
	char *magstr[] = {"NDK_MagOpen","NDK_MagClose","NDK_MagSwiped","NDK_MagReadNormal","NDK_MagReset","NDK_MagReadRaw"};
#endif		

	/*process body*/
	len = (MaxNum>20)?MaxNum:20;
	while(cnt)
	{	
		cnt--;
		bak++;
		
		//���������
		cls_printf("��%d�����˳��:",bak);
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
		
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret = card_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",magstr[num[j]]);
				goto ERR;
			}
		}	
	}
	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
	return;
}


