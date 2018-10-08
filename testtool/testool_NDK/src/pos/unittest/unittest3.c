/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: unittestģ��
* file name		: 
* Author 			: unittest
* version			: 
* DATE			: 20120815
* directory 		: 
* description		: ��������һ�����²��Է��(���Զ���)���в��ԵĿɱ���ͨ������������.��������operate���˷���multiply�����Ƿ���ȷ
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
//#include "�Լ���Ҫ�õ���ͷ�ļ�"	//���趨��

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"operate,multiply"	//����API��API�Ե����֣�����API�ԣ���ʽ��"api1,api2"��

#define STEP1()
#define STEP2() SUCC

/*------------global variables definition-----------------------*/
//��������ȫ�ֱ�������ʹ�ò�������ֵ����ֻ�ڷ��ò���ʱ��ʹ�á�ͬʱ��Ϊ�˲������˵�ȫ�ֱ���������ͻ����ֻ��һ���ļ���ʹ�����static���η�������Ҫ���ļ�ʹ��ʱ������������Ϊ"�ļ���_������"���磬gen100_flg��
static int init = FALSE;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
//������unittest.c�ж���Ŀɹ���ģ�����������ʹ�õı�����ģ��Ĳ���֧�ź���,��Ҫ��extern����

/*---------------functions definition---------------------------*/
//����ģ�Ȿ�����Ĳ���ǰ��
static int PreHandle(void)
{
	//����Ϊ�ֲ�����

	//����Ϊ������
	//�ȷ�˵,��Ҫ������һ���ļ����ܽ���multiply����
	return SUCC;
}

//����ģ��һ�Ա��⺯��.����,operateΪ�޲��޷���,�乤���Ƿ�������Ҫͨ��multiply����֤;ͬʱ,ִ��multiplyǰһ�㶼��Ҫ��Ե���operate. �ʹ�ϵ���е����Ƿ�һ�����
static void operate(void){init = TRUE;}

static int multiply(int a, int b)
{
	//����Ϊ�ֲ�����
	int cnt = 1, result = a;

	//����Ϊ������
	if(init==FALSE)
		return FAIL;
	for(; cnt<b; cnt++)
		result+=a;
	init = FALSE;
	return result;
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	unittest      20120809	created
*				  	unittest      20120810	modify for xxx
*****************************************************************/
void unittest3(void)//������Ϊ"ģ����+������"����ʽ,�������������ļ���,�޲����޷���
{
	//����Ϊ�ֲ�����
	int ret = 2012, *ptr = NULL;
	char *str = "hello world!", *str1 = "hello world!", *str2 = "hello World!";

	//����Ϊ������
	CUT_BEGIN();

//-------------------------------------------��Ҫ����������Ա��д�ĵ�1���֣������������壩�����--------------------------------------------
	//����ǰ�ã�PreCondition��PreHandle��if necessery��
	CUT_ASSERT_EQUAL(PreHandle(), SUCC, goto PRE_EXIT);

	//case1: ������operate,ֱ�ӵ���multiplyӦʧ��
	CUT_ASSERT_EQUAL(multiply(1, 1), FAIL, goto EXIT1);

	//case2: ��ȷʹ��multiply��������,Ӧ��Ԥ�����.���·ּ������������в���
	operate();
	CUT_ASSERT_EQUAL(multiply(8, 7), 56, goto EXIT1);
	operate();
	CUT_ASSERT_EQUAL(multiply(0, 7), 0, goto EXIT1);
	operate();
	CUT_ASSERT_EQUAL(multiply(99, -3), -297, cls_show_msg("2012 is the end of the world?!");ret = 2013;goto EXIT1);//�˴���һ��actions��������Ϊ�������������,�������м���;�ָ�

	//case3: ��֤multiply�Ƿ�����"�˷�������"
	operate();
	CUT_ASSERT_EQUAL(multiply(4, 5), multiply(5, 4), goto EXIT2);

	//caseX: �������������Ϊ�˱���������֤Ŀ��.����Ϊ����ʾ�������Զ��Ե��÷�.������Զ��Լ��÷�˵����cut_assert.h
	//ע��:ʹ�ò��Զ�����Ȼ����,������ ��֧�ֿɱ��,��Ŀǰ�ṩ�Ĳ��Ժ겻�������������ʱ,�����unittest2.cʹ��send_result�ķ��
	operate();
	CUT_ASSERT(multiply(3, 3)>3, return);
	operate();
	CUT_ASSERT_EQUAL(multiply(3, 3), 9, return);
	operate();
	CUT_ASSERT_NOT_EQUAL(multiply(3, 3), 90, return);
	
	CUT_ASSERT_STRING_EQUAL(str, str1, return);
	CUT_ASSERT_STRING_NOT_EQUAL(str, str2, return);
	CUT_ASSERT_NSTRING_EQUAL(str, str2, 5, return);
	CUT_ASSERT_NSTRING_NOT_EQUAL(str, str2, strlen(str), return);
	CUT_ASSERT_PTR_NULL(ptr, return);
	CUT_ASSERT_PTR_EQUAL(ptr, NULL, return);
	ptr = &ret;
	CUT_ASSERT_PTR_NOT_NULL(ptr, return);
	CUT_ASSERT_PTR_NOT_EQUAL(ptr, NULL, return);

	//���Ժ��ã�PostCondition��PostHandle��if necessery��
	//������1
	STEP1();
	//������2,Ԥ��Ӧִ�гɹ�
	CUT_ASSERT_EQUAL(STEP2(), SUCC, goto POST_EXIT);
//-------------------------------------------��Ҫ����������Ա��д�ĵ�1���֣������������壩���յ�--------------------------------------------

	//���Խ���
	CUT_END();
	return;
//-------------------------------------------��Ҫ����������Ա��д�ĵ�2���֣������������--------------------------------------------
PRE_EXIT://if necessary
EXIT1:
	//����PreHandle�д������ļ�
	return;
EXIT2:	//if necessary
	//����PreHandle�д������ļ�
	//����,����һЩ�����������
	return;
POST_EXIT://if necessary
	//һЩ������
	return;
//-------------------------------------------��Ҫ����������Ա��д�ĵ�2���֣����������յ�--------------------------------------------
}

//����ı���淶���μ�"DOC\�������������д�淶CHECKLIST.doc".ǿ�ҽ����Ķ�������Ӧ�Ķ���6���֣��е�"����ʱ������"��"����ʱ������"��,��Ϊ������������Ҫ����һ�����˽�.
//����ģ����,��ע�͵����Ҫ��:����Բ��Ե�caseX����ע��
//���ڹ����д���Ĳ��Խ�����ϱ���
// 1��ֱ��ͨ��if֮��������жϺ󣬵���send_result�����ϱ�������send_result֧�ֿɱ�Σ��ֲ��˲��Զ��ԶԲ��������ƣ���if�ж��ڻ������������������˷�ʽ��Ϊ���(��ͨ�õķ���)��
// 2��ʹ��CUT_ASSERTһ��Ķ��ԣ����弰����˵����inc\pos\cut_assert.h�У����в��ԣ��˷�ʽ��ʹ���Դ��������٣���Ҫע����Ҫ���г�����ʱ��Ҫʹ����Ӧ�Ĳ��Ժ꣬ͬʱ��Щ���֧�ֵ�Ԫ����ʹ�ã���֧��δ����ϵͳ���ԣ����δ���еĻ�����
//rfcard4.c��ppp1.c��display18.c��ԭ���ڲ����׼����������еĲ����������룬���ο�

