/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 15.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrSetContrast�ܷ�������Ļ�Աȶ�
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrSetContrast"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr15
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
#if OVERSEAS_OS70_ENABLE  //�����Ʒ7.0�汾��ʼ����֧�����öԱȶ�,�Ҳ���Ҫ��rootȨ����  20180425 modify
void scr15(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0, i = 0;
	uint puncd = 0;
		
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��:�ж�Һ��������
	if((ret=NDK_ScrGetColorDepth(&puncd))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//����ǰ��:����ϵͳ����
	//����Ҫ����Ĭ�ϵĶԱȶȣ�����޷��ú�����ȡĬ�϶Աȶȣ���������Ҫ�ҿ���ȷ�϶Աȶ��Ƕ��٣����˳������ʱ�����ó����ȷ�ϵ�Ĭ��ֵ
	if(puncd == 16)
	{
		//case1:�����쳣:unContrastΪ0,21
		if((ret=NDK_ScrSetContrast(0))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		if((ret=NDK_ScrSetContrast(21))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		//case2:����NDK_ScrSetContrast(�ֱ�����1-20),�ۿ�Ч��,�˹��ж�
		cls_show_msg1(5,"���ĵȴ�10�뿴��Ļ�仯,������Ļ�Աȶ���ǳ����,��������ó�����,��ۿ�Ч��");
		for(i=1;i<=20;i++)
		{
			if((ret=NDK_ScrSetContrast(i))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
				goto ERR;
			}		
			NDK_SysDelay(5);
		}	
		NDK_ScrSetContrast(12);
	}
	else if(puncd ==1 || puncd == 4)
	{
		//case1:�����쳣:unContrastΪ-1,64
		if((ret=NDK_ScrSetContrast(-1))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		if((ret=NDK_ScrSetContrast(64))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		
		//case2:����NDK_ScrSetContrast(�ֱ�����0-63),�ۿ�Ч��,�˹��ж�
		cls_show_msg1(5,"���ĵȴ�30�뿴��Ļ�仯,������Ļ�Աȶ���ǳ����,��������ó�����,��ۿ�Ч��");
		for(i=0;i<=63;i++)
		{
			if((ret=NDK_ScrSetContrast(i))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}		
			NDK_SysDelay(5);
		}	
		NDK_ScrSetContrast(32);
	}
	else
	{
		send_result("line %d:%s����ʧ��:��Ļ�ǲ�����ڰ���(puncd = %d)", __LINE__, TESTAPI, puncd);
		GOTOERR;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "[ENTER]ͨ��,[����]��ͨ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
			send_result("%s����ʧ��", TESTAPI);
	goto ERR;

	//�ָ�Ĭ������
ERR:
	if(puncd == 16)	
		NDK_ScrSetContrast(12);
	else
		NDK_ScrSetContrast(32);
	return;
}
#else
void scr15(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint i = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	if(cls_show_msg1(5, "��������Ҫ��rootȨ���½���,[Enter]����,��������" )!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI);
		return;
	}

	//����ǰ��:����ϵͳ����
	//����Ҫ����Ĭ�ϵĶԱȶȣ�����޷��ú�����ȡĬ�϶Աȶȣ���������Ҫ�ҿ���ȷ�϶Աȶ��Ƕ��٣����˳������ʱ�����ó����ȷ�ϵ�Ĭ��ֵ
	
	//case1:�����쳣:unContrastΪ-1,64
	if((ret=NDK_ScrSetContrast(-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_ScrSetContrast(64))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:����NDK_ScrSetContrast(�ֱ�����0-63),�ۿ�Ч��,�˹��ж�
	cls_show_msg1(5,"���ĵȴ�30�뿴��Ļ�仯,������Ļ�Աȶ���ǳ����,��������ó�����,��ۿ�Ч��");
#if defined ME30S  //������˵ME30S����Ч�����Ǵ�1-63,�����޸�,��������������� 20180517 linying modify
	i=1;
#else 
	i=0;
#endif
	for(i;i<=63;i++)
	{
		if((ret=NDK_ScrSetContrast(i))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			goto ERR;
		}		
		NDK_SysDelay(5);
	}	
	NDK_ScrSetContrast(32);
	nKeyin = cls_show_msg1(MAXWAITTIME, "[ENTER]ͨ��,[����]��ͨ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

	//�ָ�Ĭ������
ERR:
	//Ĭ��ֵΪ32
#if !(defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME30GX||defined ME32THM||defined ME31||defined SP10)
	NDK_ScrSetContrast(32);
#else
	NDK_ScrSetContrast(19);
#endif
	return;
}
#endif

