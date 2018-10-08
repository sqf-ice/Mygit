#ifndef _DEBUG_H_
#define _DEBUG_H_

/******************************************************************************
** Copyright (c) 2008�����´�½���Թ�˾���ڲ�Ʒ��������
** �汾��  	��V0.1
** ��������	��2008-09-16
** ������  	��lipm
** �ļ�����	�����Ժ���ͷ�ļ�
**
** �ļ�����	�����Ժ����궨��
** ��Ҫ�������书�ܣ�
				     DOWNLOAD_ASSERT		��ʾ��������λ��
				     DOWNLOAD_DUMP			��ӡ������Ϣ����־�ļ�

** �������ļ��Ĺ�ϵ��
**           		 Ϊ���س����ģ���ṩ���Ժ����궨��.

** �޸���  	����
** �ա���  	��
** �衡��  	����
******************************************************************************/
#include "NDK.h"
#include "Dump.h"


//ά��������־
#define WriteLog(fp, x...) debug_print(fp, __FILE__, __LINE__, __FUNCTION__, ##x);

//���Կ��أ���ʽ�汾�У��뽫�˺궨��ע��
//#define DEBUG


#ifdef DEBUG

/* DOWNLOAD_ASSERT ��Ĺ���:
    �ڵ��Ի����£�����������з�����������pos��Ļ����ʾ��������λ��*/
#define DOWNLOAD_ASSERT(x) if(!(x)) \
	{ \
		int nKey;\
		NDK_ScrClrs();\
		NDK_ScrPrintf("%s(%d):\n%s", __FILE__, __LINE__, __FUNCTION__); \
		NDK_KbGetCode(0,&nKey); \
	} \

/* DOWNLOAD_DUMP ��Ĺ���:
    �ڵ��Ի����£���������Ϣ��ӡ���ļ�fp_log��*/
#define DOWNLOAD_DUMP(fp, x...) debug_print(fp, __FILE__, __LINE__, __FUNCTION__, ##x);

#else

#define DOWNLOAD_ASSERT(x)
#define DOWNLOAD_DUMP(fp, x...)

#endif
#endif
