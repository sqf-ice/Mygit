/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: unittestģ��
* file name		: 
* Author 			: unittest
* version			: 
* DATE			: 20120809
* directory 		: 
* description		: unittestģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�.ע�⣺���ļ�ֻ��һ������ģ��!
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
//#include "�Լ���Ҫ�õ���ͷ�ļ�"	//���趨��

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"unittestģ��"
//�����궨��,���趨��

/*------------global variables definition-----------------------*/
//��������ȫ�ֱ�������ʹ�ò�������ֵ����ֻ�ڷ��ò���ʱ��ʹ�á�ͬʱ��Ϊ�˲������˵�ȫ�ֱ���������ͻ����ֻ��һ���ļ���ʹ�����static���η�������Ҫ���ļ�ʹ��ʱ������������Ϊ"�ļ���_������"���磬gen100_flg��

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
//���屾����ģ��Ĳ���֧�ź������ɹ���ģ�����������ʹ�ã�����������ֵ���Ϳ����ɶ���(��������)

/****************************************************************
* function name 	 	: unittest
* functional description 	: unittestģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	unittest      20120809	created
*				  	unittest      20120810	modify for xxx
*****************************************************************/
void unittest(PF* list, unsigned char id[])//��ں������������ļ���,Ҳ��ģ����
{
	//����Ϊ�ֲ�����

	//����Ϊ������
	cls_show_msg1(1, "%s����...", TESTMODULE);//��Ļ��ʾ��ǰ���ڵĲ���ģ��


	//ģ�����ǰ�ã�PreHandle��if necessery��
	//if ģ�����ǰ������ʧ��
		//�����˳�����ģ�����;

	process(list, id);

	//ģ����Ժ��ã�PostHandle��if necessery��
	//����ģ����Ժ���

	//ģ����Խ���
	return;
}

//����ı���淶���μ�"DOC\�������������д�淶CHECKLIST.doc"
//ppp.c��ԭ���ڲ����׼�����������PPP����ģ������ļ������ο�

