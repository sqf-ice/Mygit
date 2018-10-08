/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 4.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_KbHZInput�ܷ���к�������
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_KbHZInput"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: kb4
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*****************************************************************/
void kb4(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin = 0, i = 0;	
	char pszS[200] = {0},szmessage[200]={"����֤֧������������к��������ַ����˸���޸Ĺ���"};
	char szmessage2[200]={"����֤֧������������а�ȡ�����˳�"};
	uint unMaxlen = 20;
	EM_IME emMethod = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//case1:�����쳣:pszSΪNULL
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(NULL,unMaxlen,emMethod))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//"NDK_ERR_MACLLOC �ڴ�ռ䲻��" �˷���ֵ���첻���� 20140922 jiangym add
	
	//case13:�Բ�֧����д���뷨�Ĳ�����д���뷨�᷵��NDK_ERR_NOT_SUPPORT   20140922 jiangym add
	if(cls_show_msg("�˴���Ҫʹ�ò�֧����д���뷨�Ļ�������,[ESC]����,[����]������")!=ESC)
	{
		memset(pszS, 0, sizeof(pszS));
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_HANDWRITE))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
	}
	
	//case2:������ƴ���뷨IME_NUMPY
	cls_show_msg1(3,"������ƴ���뷨,���������⺺��,��,\"��\"(yi)\"��������\"(wo)");///added 20130905֧��GB18030�����끥
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_NUMPY))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);

	//case3:����Ӣ�����뷨IME_ENGLISH
	cls_show_msg1(3,"����Ӣ�����뷨,����������Ӣ��,��,abcdef");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_ENGLISH))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);
	
	//case4:�����������뷨IME_NUM
	cls_show_msg1(3,"�����������뷨,��������������,��,\"4\"");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_NUM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);

#if !defined ME50//֧�ֺ�������,����֧����ƴ�����֡�Ӣ�����뷨
	//case5:���Աʻ����뷨IME_BIHUA
	cls_show_msg1(3,"���Աʻ����뷨,���������⺺��,��,\"��\"(12341234)");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_BIHUA))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case11:���Աʻ����뷨IME_BIHUA,��Ч�������
	cls_show_msg1(10,"�밴[1��2��3Ʋ4��5��]����İ���,������6,7,8,9,*,F1,F2,F3,F4��Ϊ��Ч����,������\"��\"(12341234)");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_BIHUA))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp(pszS,"��"))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, pszS);
		RETURN;
	}
	
	//case6:������λ���뷨IME_QUWEI
	cls_show_msg1(3,"������λ���뷨,��,\"��\"(2222)");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_QUWEI))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);

	//case11:�����������뷨IME_GBK
	//cls_show_msg1(3,"�����������뷨,���������⺺��,\"��\"(bda3)");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_GBK))!=NDK_ERR_PARA/*NDK_OK*/)//NDK5.0�Ѳ�֧������,�ʷ���ʧ��20140822 jiangym
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);

	if(cls_show_msg("�Ƿ�֧����д���뷨,�ǰ�[ȷ��],����[����]")==ENTER)
	{
		//case12:������д���뷨IME_HANDWRITE
		cls_show_msg1(3,"������д���뷨,��,\"��\"");
		memset(pszS, 0, sizeof(pszS));
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_HANDWRITE))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);
	}
#endif

	//case7:����EM_IME֮�������ֵ����ú���Ĭ�ϼ�����ƴ���뷨
	cls_show_msg1(3,"������ƴ���뷨,���������⺺��,��,\"��\"(yi)");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_MAXNUM+1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);

	//case8:���������ӿ������뷨������£������ּ���0�����ֱ����ţ�����������������ƶ�ѡ����ţ���ȷ�ϼ���ѡ����
	cls_show_msg1(5,"���Ա������,�밴0��,���ɰ�[�˵�]��ҳ,������ƶ�ѡ�����,��ȷ�ϼ���ѡ����");
	memset(pszS, 0, sizeof(pszS));
	if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_MAXNUM+1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);

	//case9: ֧�ֶ���������Ļ�ɾ�Խ����޸ĵĹ��ܣ�����ʲô���뷨����
	for(i=0;i<IME_MAXNUM;i++)
	{
		memset(pszS,0,sizeof(pszS));
		switch(i)
		{
			case 0:
				strcpy(pszS,"��ƴ����,���������⺺��,��,\"��\"(yi),");
				strcat(pszS,szmessage);
				break;
			case 1:
				strcpy(pszS,"Ӣ�Ĳ���,����������Ӣ��,��,abcdef,");
				strcat(pszS,szmessage);
				break;
			case 2:
				strcpy(pszS,"���ֲ���,��������������,��,\"4\",");
				strcat(pszS,szmessage);
				break;
			case 3:
				strcpy(pszS,"�ʻ�����,���������⺺��,��,\"��\"(12341234),");
				strcat(pszS,szmessage);
				break;
			case 4:
				strcpy(pszS,"��λ����,���������⺺��,\"��\"(2222),");
				strcat(pszS,szmessage);
				break;
			case 5://NDK5.0�Ѿ���֧������
				//strcpy(pszS,"�������,���������⺺��,\"��\"(bda3),");
				//strcat(pszS,szmessage);
				continue/*break*/;
			case 6:
				strcpy(pszS,"��д����,���������⺺��,\"��\",");
				strcat(pszS,szmessage);
				break;
			default:
				break;
		}
		cls_show_msg1(MAXWAITTIME,pszS);
		memset(pszS,0,sizeof(pszS));
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,i))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		cls_show_msg1(MAXWAITTIME, "���������%s,�������������...", pszS);
	}

	//case10:֧����������������а�ESCֱ���˳�����״̬������ʲô���뷨�������ע�뺺����ص����뷨��
	for(i=0;i<IME_MAXNUM;i++)
	{
		memset(pszS,0,sizeof(pszS));
		switch(i)
		{
			case 0:
				strcpy(pszS,"��ƴ����,���������⺺��,��,\"��\"(yi),");
				strcat(pszS,szmessage2);
				break;
			case 1:
				strcpy(pszS,"Ӣ�Ĳ���,����������Ӣ��,��,abcdef,");
				strcat(pszS,szmessage2);
				break;
			case 2:
				strcpy(pszS,"���ֲ���,��������������,��,\"4\",");
				strcat(pszS,szmessage2);
				break;
			case 3:
				strcpy(pszS,"�ʻ�����,���������⺺��,��,\"��\"(12341234),");
				strcat(pszS,szmessage2);
				break;
			case 4:
				strcpy(pszS,"��λ����,���������⺺��,\"��\"(2222),");
				strcat(pszS,szmessage2);
				break;
			case 5: //NDK5.0�Ѿ���֧������
				//strcpy(pszS,"�������,���������⺺��,\"��\"(bda3),");
				//strcat(pszS,szmessage2);
				continue/*break*/;
			case 6:
				strcpy(pszS,"��д����,���������⺺��,\"��\",");
				strcat(pszS,szmessage2);
				break;
			default:
				break;
		}
		cls_show_msg1(MAXWAITTIME,pszS);
		memset(pszS,0,sizeof(pszS));
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,i))!=NDK_ERR_QUIT/*NDK_OK*/)//Ӧ������NDK_KbHZInput�ڰ�ESC��ȡ������ʱ��NDK�ӿ�Ҫ����NDK_ERR_QUIT linwl 20140102
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
	}

	//���Խ���
	nKeyin = cls_show_msg1(MAXWAITTIME, "��ȷ�ϼ�����ͨ��,����������");	
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	
	//zhangnw20100128 add,ͨ���ںв��Բ���ֿ������ֿ���������(��������:1.�ֿⲻȫ;2.�������).
	GUI_DispStringEx(0, 0, "��ʾ:����ʹ��[�Լ�]�е�[���뷨]��ƴ��,�ʻ�����ȫ��������ϵĳ�������!", TEXT_ATTRIBUTE_REVERSE);
	show_msg1(MAXWAITTIME, "���������...");
	return;
}

