/*************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: general
* file name		: general21
* Author 			: linwl
* version			: V1.0
* DATE			: 20080324
* directory 		: .\SRC\pos\general
* description		: ���Լ���Ʋ��������е�Ч��
* related document	: NL_POS����Ա�ο��ֲ�(2007-05-21)
*					 
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM		"CPU����"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static unsigned int fibonach(unsigned int n)
{
	if(n>1) 
		return fibonach(n-1)+fibonach(n-2);
	else    
		return n;
}

static int recursiveTest(void)
{
	unsigned int i = 0;
	int f[30] = {0}, nTmp = 0;
	float fTimeElapsed=0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif	
	//������Ʋ��������е���ȷֵ�����f[30]������
	f[0] = 0;
	f[1] = 1;
	for(i=2; i<28; i++)
		f[i] = f[i-1]+f[i-2];

	cls_printf("����Ʋ�����������...");
	lib_StartStopwatch();
	for(i=1; i<28; i++)
	{
		nTmp = fibonach(i);//���÷Ʋ������ĵݹ麯��
		if(nTmp==f[i])
			;//cls_printf("�Ʋ���������:\nf(%02u)=%u\n", i, nTmp);
		else
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"����Ʋ��������е�%d��Ԫ��ʱʧ��!(%d, %d)", f[i], nTmp);
			return FAIL;
		}
	}
	fTimeElapsed = lib_ReadStopwatch();
#if !K21_ENABLE
	cls_show_msg_record(FILENAME ,FUNCNAME ,"����Ʋ���������ǰ%d��Ԫ������ʱ��Ϊ%.2f�롣", i-1, fTimeElapsed);
#else
	memset(str,0,sizeof(str));
	ftos(fTimeElapsed,str);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"����Ʋ���������ǰ%d��Ԫ������ʱ��Ϊ%s�롣", i-1, str);
#endif
	return SUCC;
}

/****************************************************************
* function name 	 	 : 
* functional description   : 
* input parameter	 	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date		  remarks
*					  lwl				20080324	  created
*****************************************************************/
void systest12(void)
{
	/*private & local definition*/
	int key = 0;
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		
		//ִ��ѹ������
		recursiveTest();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;

	}
	while (1)
	{
		key = cls_show_msg("_CPU���ܲ���_\n"
							"1.�Ʋ��������в���\n");
		switch(key)
		{
		case '1':
			recursiveTest();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

