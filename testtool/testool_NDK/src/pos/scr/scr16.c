/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 16.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ���� NDK_ScrSetFontType,NDK_ScrGetFontType�ܷ����úͻ�ȡʹ�õ���ʾ����,NDK_ScrGetFontSize�ܷ��ȡ��ǰϵͳʹ�õĺ�����ʾ�����͸�
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#if K21_ENABLE&&!(defined SP610||defined ME62)
#define 	TESTAPI		"NDK_ScrGetFontSize"//K21��֧��NDK_ScrGetFontSize��������������������֧��
#elif (defined SP610||defined ME62)
#define 	TESTAPI		"NDK_ScrSetFontType,NDK_ScrGetFontSize"
#else
#define 	TESTAPI		"NDK_ScrSetFontType,NDK_ScrGetFontType,NDK_ScrGetFontSize"
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr16
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
#if K21_ENABLE&&!(defined SP610||defined ME62)
void scr16(void)
{
	/*private & local definition*/
	int ret = 0;
	uint punWidth = 0, punHeight = 0;

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//����ΪNULL����ʾ���û�ȡ����ֵ�����Է��سɹ�
	if((ret=NDK_ScrGetFontSize(NULL,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	send_result("%s����ͨ��,���豸��Ļ���ֵ����Ϊ:%d,��Ϊ:%d,���˹��ж��Ƿ���ȷ", TESTAPI, punWidth, punHeight);
	return;
}
//ĿǰSP610��ME62Ҳ֧��NDK_ScrSetFontType�����Ӳ��� 20180712 modify
#elif  (defined SP610||defined ME62)
void scr16(void)
{
	/*private & local definition*/
	int ret = 0;
	uint punWidth = 0, punHeight = 0;
	int i = 0, j = 0;
	uint unMaxlen = 20;
	char EnumName[][20] = {"Ĭ������","��������","�û��Զ�������","24*24����"};
	char pszS[200] = {0};	
	const char pszS3[] = "�����ַ�`~!@#$%^&*() -_=+][{}:;����|\\?/><.,"; 	
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣:emTypeΪ-1;punWidth,punHeight�ֱ�ΪNULL
	//����emTypeΪ-1��Ӧ�÷���ʧ��//�ײ����һ����������ͽ��в��������سɹ�
	if((ret=NDK_ScrSetFontType(-1))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ֻ֧��DISPFONT_CUSTOM��DISPFONT_ASC12X24������ö��ֵ���û�ʧ��
	//�޸�Ϊ֧��DISPFONT_CUSTOM��DISPFONT_ASC12X24��DISPFONT_EXTRA 20180820 modify
/*	if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}*/
	if((ret=NDK_ScrSetFontType(DISPFONT_USER))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//����ΪNULL����ʾ���û�ȡ����ֵ�����Է��سɹ�
	if((ret=NDK_ScrGetFontSize(NULL,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:����ϵͳĬ��,����ȡ����Ŀ�͸�32*32
	if((ret=NDK_ScrSetFontType(DISPFONT_CUSTOM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(punWidth != 32 || punHeight != 32)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
		GOTOERR;
	}
	
	//case7:��������DISPFONT_EXTRA,����ȡ����Ŀ�͸�32*32  20180820 add
	if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(punWidth != 32 || punHeight != 32)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
		GOTOERR;
	}

	//case3:����24*24,����ȡ����Ŀ�͸�
	if((ret=NDK_ScrSetFontType(DISPFONT_ASC12X24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(punWidth != 24 || punHeight != 24)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
		GOTOERR;
	}
	
	//case4:�ָ�����ϵͳĬ��,����ȡ����Ŀ�͸�32*32
	if((ret=NDK_ScrSetFontType(DISPFONT_CUSTOM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(punWidth != 32 || punHeight != 32)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
		GOTOERR;
	}

	if(auto_flag==0||auto_flag==2)
	{
		//case5:���������ʹ�����뷨,������ִ�С��仯��(Ŀǰ��������������Զ�������ᱣ����һ�ε�����)
		if((ret=NDK_ScrSetFontType(DISPFONT_CUSTOM))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		memset(pszS, 0, sizeof(pszS));
		cls_show_msg("ʹ����ĸ���л��������뷨,���������ַ�,���ж��������ַ��Ƿ�ΪĬ������,���������");
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_ENGLISH/*IME_NUMPY*/))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(2, "�������л���24*24����");
		if((ret=NDK_ScrSetFontType(DISPFONT_ASC12X24))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		memset(pszS, 0, sizeof(pszS));
		cls_show_msg("ʹ����ĸ���л��������뷨,���������ַ�,���ж��������ַ��Ƿ�Ϊ24*24����,���������");
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_ENGLISH/*IME_NUMPY*/))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}	
		cls_show_msg1(2, "�������л���DISPFONT_EXTRA����");
		if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		memset(pszS, 0, sizeof(pszS));
		cls_show_msg("ʹ����ĸ���л��������뷨,���������ַ�,���ж��������ַ��Ƿ�ΪDISPFONT_EXTRA����,���������");
		if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_ENGLISH/*IME_NUMPY*/))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}	
		if(cls_show_msg("������ַ���С�Ƿ��������õ������С�仯,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case6:����С�д��������������ַ������Ƿ�Ϊ��ȷ�ַ�	 
		//����ʱ���ע��Ļ��ʾ�Ľ���Ƿ�Ϊ�������ַ�`~!@#$%^&*() -_=+][{}:;����|\?/><.,����Ӧ�ó�����ʾ�������������罫��\����ʾΪ������
		if((ret=NDK_ScrSetFontType(DISPFONT_CUSTOM))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("�����Ļ��\"%s\",��ʾ����ȷ��[ȷ��];����������.", pszS3)!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
		cls_show_msg1(2, "Ĭ������ģʽ����ͨ��!");
		if((ret=NDK_ScrSetFontType(DISPFONT_ASC12X24))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("�����Ļ��\"%s\",��ʾ����ȷ��[ȷ��];����������.", pszS3)!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
		cls_show_msg1(2, "24*24����ģʽ����ͨ��!");
		if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("�����Ļ��\"%s\",��ʾ����ȷ��[ȷ��];����������.", pszS3)!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
		cls_show_msg1(2, "DISPFONT_EXTRA����ģʽ����ͨ��!");
	}
	send_result("%s����ͨ��", TESTAPI);
	//�ָ�Ĭ������
ERR:
	NDK_ScrSetFontType(DISPFONT_CUSTOM);
	return;
}
#else
void scr16(void)
{
	/*private & local definition*/
	int ret = 0;
	uint punWidth = 0, punHeight = 0, puncd = 0;
	EM_DISPFONT oldfonttype = 0;
	int i = 0, j = 0;
	uint unMaxlen = 20;
	char EnumName[][20] = {"Ĭ��С����","����������","���������","�û��Զ�������"};
	EM_DISPFONT fonttype = 0;
	char pszS[200] = {0};	
	const char pszS3[] = "�����ַ�`~!@#$%^&*() -_=+][{}:;����|\\?/><.,";//20150204���������ַ���ʾ 	
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��:�ж�Һ��������,����Ĭ��ֵ����֤
	if((ret=NDK_ScrGetColorDepth(&puncd))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_ScrGetFontType(&oldfonttype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, oldfonttype);
		GOTOERR;
	}

	//case1:�����쳣:emTypeΪ-1;punWidth,punHeight�ֱ�ΪNULL
	//����emTypeΪ-1��Ӧ�÷���ʧ��//�ײ����һ����������ͽ��в��������سɹ�
	if((ret=NDK_ScrSetFontType(-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontType(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//����ΪNULL����ʾ���û�ȡ����ֵ�����Է��سɹ�
	if((ret=NDK_ScrGetFontSize(NULL,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:����ϵͳĬ��,����ȡ����Ŀ�͸�
	if((ret=NDK_ScrSetFontType(DISPFONT_CUSTOM))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_ScrGetFontType(&fonttype))!=NDK_OK)||(fonttype!=DISPFONT_CUSTOM))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, fonttype);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	switch(puncd)
	{
		case 16://ͨ��Һ����������Ĭ�������͸� 16Ϊ����,����Ϊ16*16
			if(punWidth != 16 || punHeight != 16)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
				GOTOERR;
			}
			break;
		case 1:
		case 4://ͨ��Һ����������Ĭ�������͸� 1,4Ϊ�ڰ���,����Ϊ12*12
			if(punWidth != 12 || punHeight != 12)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
				GOTOERR;
			}
			break;
		default:
			send_result("line %d:%sδ֪����Ļ����%d(%d,%d)", __LINE__, TESTAPI, puncd, punWidth, punHeight);
			GOTOERR;
	}
	
	//case3:���ö���ߴ�(��),����ȡ����Ŀ�͸�
	if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_ScrGetFontType(&fonttype))!=NDK_OK)||(fonttype!=DISPFONT_EXTRA))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, fonttype);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	switch(puncd)
	{
		case 16://ͨ��Һ����������Ĭ�������͸� 16Ϊ����,����Ϊ24*24
			if(punWidth != 24 || punHeight != 24)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
				GOTOERR;
			}
			break;
		case 1:
		case 4://ͨ��Һ����������Ĭ�������͸� 1,4Ϊ�ڰ���,����Ϊ16*16
			if(punWidth != 16 || punHeight != 16)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
				GOTOERR;
			}
			break;
		default:
			send_result("line %d:%sδ֪����Ļ����%d(%d,%d)", __LINE__, TESTAPI, puncd, punWidth, punHeight);
			GOTOERR;
	}

	//case4:���ö���ߴ�(��),����ȡ����Ŀ�͸�
	if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA_LARGE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_ScrGetFontType(&fonttype))!=NDK_OK)||(fonttype!=DISPFONT_EXTRA_LARGE))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, fonttype);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	switch(puncd)
	{
		case 16://ͨ��Һ����������Ĭ�������͸� 16Ϊ����,����Ϊ32*32
			if(punWidth != 32 || punHeight != 32)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
				GOTOERR;
			}
			break;
		case 1:
		case 4://ͨ��Һ����������Ĭ�������͸� 1,4Ϊ�ڰ���,����Ϊ16*16
			if(punWidth != 16 || punHeight != 16)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punWidth, punHeight);
				GOTOERR;
			}
			break;
		default:
			send_result("line %d:%sδ֪����Ļ����%d(%d,%d)", __LINE__, TESTAPI, puncd, punWidth, punHeight);
			GOTOERR;
	}
	
	//case5:�����û��Զ�������,����ȡ����Ŀ�͸�:�Զ���ģʽ�ݲ�֧��,���ö���ģʽ֧��
	if((ret=NDK_ScrSetFontType(DISPFONT_EXTRA))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_ScrGetFontType(&fonttype))!=NDK_OK)||(fonttype!=DISPFONT_EXTRA))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, fonttype);
		GOTOERR;
	}
	if((ret=NDK_ScrGetFontSize(&punWidth,&punHeight))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case6:���������л�,�����л�������ܷ�
	for(i=DISPFONT_CUSTOM;i<=DISPFONT_USER;i++)
	{
		if((ret=NDK_ScrSetFontType(i))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_printf("��%sģʽ�л���",EnumName[i]);
		
		for(j=DISPFONT_CUSTOM;j<=DISPFONT_USER;j++)
		{
			if(i>=j)
				continue;
			
			show_msg1(2, "%sģʽ",EnumName[j]);		
			if((ret=NDK_ScrSetFontType(j))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}

			cls_show_msg1(2, "��%sģʽ�л���%sģʽ",EnumName[j],EnumName[i]);
			if((ret=NDK_ScrSetFontType(i))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			cls_printf("��%sģʽ�л���",EnumName[i]);
		}
	}
	
	if(auto_flag==0||auto_flag==2)
	{
		//case7:����С�д������ʹ�����뷨,������ִ�С��仯��
		for(i=DISPFONT_CUSTOM;i<=DISPFONT_EXTRA_LARGE;i++)
		{
			if((ret=NDK_ScrSetFontType(i))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			memset(pszS, 0, sizeof(pszS));
#if OVERSEAS_ENABLE  //�����Ʒ��֧��NDK_KbHZInput  20180427 modify
			cls_show_msg("�����벻����20���ַ�(�ɻ������,С����,����,Сд��ĸ),���ж��������ַ��Ƿ�Ϊ%s.���������...",EnumName[i]);
			if ((ret=NDK_KbGetInput(pszS,0,unMaxlen,&puncd,INPUTDISP_NORMAL,30,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
#else
			cls_show_msg("ʹ����ĸ���л��������뷨,���������ַ�,���ж��������ַ��Ƿ�Ϊ%s,���������",EnumName[i]);
			if ((ret=NDK_KbHZInput(pszS,unMaxlen,IME_ENGLISH/*IME_NUMPY*/))!=NDK_OK)//�����Ʒ��֧����ƴ,�ĳ�ͨ�õ�Ӣ�� 20180309modify
#endif
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}	
		}
		if(cls_show_msg("������ַ���С�Ƿ��������õ������С�仯,��[Enter],��[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case7:����С�д��������������ַ������Ƿ�Ϊ��ȷ�ַ�	20150204 add jiangym,
		//����ʱ���ע��Ļ��ʾ�Ľ���Ƿ�Ϊ�������ַ�`~!@#$%^&*() -_=+][{}:;����|\?/><.,����Ӧ�ó�����ʾ�������������罫��\����ʾΪ������
		for(i=DISPFONT_CUSTOM;i<=DISPFONT_EXTRA_LARGE;i++)
		{
			if((ret=NDK_ScrSetFontType(i))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if(cls_show_msg("�����Ļ��\"%s\",��ʾ����ȷ��[ȷ��];����������.", pszS3)!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
			}
			cls_show_msg1(2, "%sģʽ����ͨ��!",EnumName[i]);
		}
	}
	send_result("%s����ͨ��", TESTAPI);
	//�ָ�Ĭ������
ERR:
	NDK_ScrSetFontType(oldfonttype);
	return;
}
#endif
