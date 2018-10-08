/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 3.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_KbGetInput
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_KbGetInput"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: kb3
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void kb3(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0, count = 0;
	char pszBuf[200] = {0}; 
	char szbuf[200] = {0}; 
	uint unMin = 0, unMaxlen = 10, punLen = 0; 
	EM_INPUTDISP emMode = 0; 
	EM_INPUT_CONTRL emControl = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣:pszBuf,punLen�ֱ�ΪNULL,emMode,emControl�ֱ�Ϊ-1,5
	//case1.1:pszBuf,punLen�ֱ�ΪNULL
	memset(pszBuf, 0, sizeof(pszBuf));
	if ((ret=NDK_KbGetInput(NULL,unMin,unMaxlen,&punLen,emMode,MAXWAITTIME,emControl))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1.2:unMin>unMaxlen,unMaxlenΪ0�����
	if ((ret=NDK_KbGetInput(pszBuf,6,5,&punLen,emMode,MAXWAITTIME,emControl))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:������ȡֵINPUTDISP_NORMAL����ʾ�ַ�,����emControl������4�����
	//case2.1:INPUT_CONTRL_NOLIMIT������ASCII���ַ���������ֱ�ӷ���
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����,����,Сд��ĸ)��������ʾ,��10�ַ����Զ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punLen,strlen(pszBuf));
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case2.2:INPUT_CONTRL_LIMIT��ֻ��������С���㣬������ֱ�ӷ��� 
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����)��������ʾ,��10�ַ����Զ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_LIMIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case2.3:INPUT_CONTRL_NOLIMIT_ERETURN������ASCII���ַ���������ȴ�ȷ�ϼ�����
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����,����,Сд��ĸ)��������ʾ,��10�ַ����밴ȷ�ϼ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case2.4:INPUT_CONTRL_LIMIT_ERETURN��ֻ��������С���㣬������ȴ�ȷ�ϼ����� 
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����)��������ʾ,��10�ַ����밴ȷ�ϼ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_LIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case3:������ȡֵINPUTDISP_PASSWD����ʾ'*'	,����emControl������4�����
	//case3.1:INPUT_CONTRL_NOLIMIT������ASCII���ַ���������ֱ�ӷ���
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����,����,Сд��ĸ)����ʾ*��,��10�ַ����Զ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_PASSWD,MAXWAITTIME,INPUT_CONTRL_NOLIMIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case3.2:INPUT_CONTRL_LIMIT��ֻ��������С���㣬������ֱ�ӷ��� 
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����)����ʾ*��,��10�ַ����Զ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_PASSWD,MAXWAITTIME,INPUT_CONTRL_LIMIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case3.3:INPUT_CONTRL_NOLIMIT_ERETURN������ASCII���ַ���������ȴ�ȷ�ϼ�����
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����,����,Сд��ĸ)����ʾ*��,��10�ַ����밴ȷ�ϼ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_PASSWD,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case3.4:INPUT_CONTRL_LIMIT_ERETURN��ֻ��������С���㣬������ȴ�ȷ�ϼ����� 
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����10���ַ�(��������,С����)����ʾ*��,��10�ַ����밴ȷ�ϼ�����.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_PASSWD,MAXWAITTIME,INPUT_CONTRL_LIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case4:������ȡֵINPUTDISP_OTHER����ʾ�Ѿ��Ӽ��������������,������������ʾ����,����emControl������4�����
	//case4.1:INPUT_CONTRL_NOLIMIT������ASCII���ַ���������ֱ�ӷ���
	memset(pszBuf, 0, sizeof(pszBuf));
	count = rand()%(unMaxlen-1)+1;
	memset(pszBuf, 'a', count);
	cls_show_msg("���ǰΪ���������ַ�(%s),(��������,С����,����,Сд��ĸ)��������ʾ,����10�ַ����Զ�����.���������...", pszBuf);
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_NOLIMIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case4.2:INPUT_CONTRL_LIMIT��ֻ��������С���㣬������ֱ�ӷ��� 
	memset(pszBuf, 0, sizeof(pszBuf));
	strcpy(pszBuf, "01234");
	cls_show_msg("���ǰΪ���������ַ�(%s),(��������,С����)��������ʾ,����10�ַ����Զ�����.���������...", pszBuf);
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_LIMIT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case4.3:INPUT_CONTRL_NOLIMIT_ERETURN������ASCII���ַ���������ȴ�ȷ�ϼ�����
	memset(pszBuf, 0, sizeof(pszBuf));
	count = rand()%(unMaxlen-1)+1;
	memset(pszBuf, 'a', count);
	cls_show_msg("���ǰΪ���������ַ�(%s),(��������,С����,����,Сд��ĸ)��������ʾ,����10�ַ����밴ȷ�ϼ�����.���������...", pszBuf);
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case4.4:INPUT_CONTRL_LIMIT_ERETURN��ֻ��������С���㣬������ȴ�ȷ�ϼ����� 
	memset(pszBuf, 0, sizeof(pszBuf));
	strcpy(pszBuf, "01234");
	cls_show_msg("���ǰΪ���������ַ�(%s),(��������,С����)��������ʾ,����10�ַ����밴ȷ�ϼ�����.���������...", pszBuf);
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_LIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case5:����������ַ�����ͨ���˸��ȡ��
	memset(pszBuf, 0, sizeof(pszBuf));
	strcpy(pszBuf, "abcdef");
	cls_printf("�����볬��һ�е������ַ���,��[BASP]��Ϊ%s,��[ȷ��]����\n", "10086");
#if defined IM81AZ
	cls_printf("���ǰΪ���������ַ�(%s),��[BASP]��Ϊ%s,��[ȷ��]����\n", pszBuf, "10086");
#endif
	if ((ret=NDK_KbGetInput(pszBuf,unMin,60/*30*/,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)//�����豸30���ַ�����һ��,���Ըĳ�60���ַ� 20140611 linwl modified
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	if(strcmp("10086", pszBuf))
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//����������case����ͬ�Ĳ�����ʾ��Ϣ

	//case13:����Ĭ�ϻ����д��ڳ���1�е��ַ����˸���Ƿ��������ɾ�� //20150312�Ͷ�ƽ̨���ֵ��������ݳ���1��ʱ�˸��ɾ�޷��˵���һ�� linwl
	memset(pszBuf, 0, sizeof(pszBuf));
	strcpy(pszBuf, "01234567890123456789012345678901234567890123456789");
	cls_printf("�밴[BASP]��Ϊ%s,��[ȷ��]����\n", "10086");
#if defined IM81AZ
	cls_printf("���ǰΪ���������ַ�(%s),��[BASP]��Ϊ%s,��[ȷ��]����\n", pszBuf, "10086");
#endif
	if ((ret=NDK_KbGetInput(pszBuf,unMin,60/*30*/,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)//�����豸30���ַ�����һ��,���Ըĳ�60���ַ� 20140611 linwl modified
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	if(strcmp("10086", pszBuf))
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//����������case����ͬ�Ĳ�����ʾ��Ϣ

	//case12:����������ַ��ﵽ���ֵ�����ͨ��ȡ�����޸�,������ֱ���˳�
	memset(pszBuf, 0, sizeof(pszBuf));
	strcpy(pszBuf, "abcdef");
	cls_printf("������10���ַ���,��[BASP]��Ϊ%s,��[ȷ��]����\n", "0123456789");
#if defined IM81AZ
	cls_printf("���ǰΪ���������ַ�(%s),��������5���ַ�����10��[BASP]��Ϊ%s,��[ȷ��]����\n", pszBuf, "0123456789");
#endif
	if ((ret=NDK_KbGetInput(pszBuf,10,10,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	if(strcmp("0123456789", pszBuf))
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//����������case����ͬ�Ĳ�����ʾ��Ϣ

	//case6:������������ֻ����ȷ�ϼ�,��֧��ȷ�ϣ�ȡ�����˸�����Ĺ��ܼ�
	memset(pszBuf, 0, sizeof(pszBuf));
	strcpy(pszBuf, "0123456789");
	cls_printf("�ȳ��������[ESC][ENTER][BASP]��ļ�,�ٰ�[ENTER]!\n");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_OTHER,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(pszBuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}
	if (strcmp(pszBuf, "0123456789"))
	{
		send_result("line %d: getRETURNline(2)����ʧ��", __LINE__);
		RETURN;
	}

	//case7:���Կ��ٰ���ʱ�᲻�ᶪ�� zhengdc 20120719 add
	memset(szbuf, 0, sizeof(szbuf));
	cls_show_msg("�����100��(��������������191919191919),��������ʾ,Ҫȷ���������,���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(szbuf,unMin,100,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(punLen!=strlen(szbuf))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, punLen);
		RETURN;
	}

	//case13:����ס��5��0�������һ���ּ���ģ�⿨�������ٰ��������ּ�,��ȡ���ļ�ֵ��ӦΪû�а��µ�������ֵ,
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("����ס��5��0�������һ���ּ���ģ�⿨�������ٰ��������ּ�,��ȡ���ļ�ֵ��ӦΪû�а��µ�������ֵ,���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case8:����ESC�����ز���ʧ��
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_show_msg("��������10���ַ�(��������,С����,����,Сд��ĸ)��������ʾ,��������а�[ESC]��.���������...");
	cls_printf("������");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(1,"ESC��ȡ���������ͨ��");
/*//�����Ѿ�����ʧ�ܣ��Ͳ��ٹ��Ļ������Ƿ����������� 20140806
	nKeyin = cls_show_msg1(MAXWAITTIME, "���봮Ϊ%s,"
	"ͨ����ȷ��,����������", strlen(pszBuf)?pszBuf:"��");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
*/	
	//case9:���Գ�ʱ�˳����ͳ�ʱʱ��׼ȷ��//���ڳ�ʱ�������ǽ��뺯���Ϳ�ʼ��ʱ,���ǰ��������ʱ��,�ʲ����Գ�ʱ��׼ȷ��
	memset(pszBuf, 0, sizeof(pszBuf));
	cls_printf("������1.3��(����[ȷ��]),���ĵȴ�����...\n");
	lib_StartStopwatch();
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT))!=NDK_ERR_TIMEOUT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case10:unWaittime����Ϊ-1ʱ,�Ƿ���ܷ�
	cls_printf("�밴[ȷ��]����...\n");
	if ((ret=NDK_KbGetInput(pszBuf,unMin,unMaxlen,&punLen,emMode,-1,emControl))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

#if 0
	//case11:���Լ���������ƹ���
	j = GUI_GetLCDHeight()-2*GUI_GetFontHeight();
	cls_printf("���Լ���������ƹ���,���������������ַ�,������ϰ�[ȷ��]");
	NDK_ScrGotoxy(0, j);
	if ((ret=NDK_KbGetInput(szbuf,50,200,&punLen,INPUTDISP_NORMAL,MAXWAITTIME,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
		
	nKeyin = cls_show_msg1(MAXWAITTIME, "��ȷ����ʾ���.ͨ����[ȷ��],����[����]");
	if (nKeyin!=0 && nKeyin!=ENTER)
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

