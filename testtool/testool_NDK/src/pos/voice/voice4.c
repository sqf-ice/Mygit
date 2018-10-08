/************************************************************************
* copyright(c)2005-2016 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ
* file name			: 4.c
* Author 			: 
* version			: 
* DATE				:20170615
* directory 			: 
* description			: ����NDK_PlayWavFile�ܷ���������WAV��Ƶ�ļ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PlayWavFile"
#define  NOEXIST		  "noexist.wav"
#define  RIGHTFILE 	  "test.wav"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: voice3
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  jiangym			20170613     created
*****************************************************************/
void voice4(void)
{
	/*private & local definition*/
	int ret = 0;

	/*process body*/	
	if(cls_show_msg1(30, "����%s...��ȷ����Ƶ�ļ�%s������,ESC�˳�", TESTAPI,RIGHTFILE)==ESC)
		return;
	
	//case1:�����쳣����
	if((ret=NDK_PlayWavFile(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PlayWavFile(NOEXIST)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������
	if((ret=NDK_PlayWavFile(RIGHTFILE)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(cls_show_msg("�Ƿ��������ֲ���,��ȷ��[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case3:��Ƶ�ļ���ʽ����ȷ,�Ქ��,���ǽ��벻��ȷ,���������ǳ��ӵ�
	//�����ĵ���˵��ֻ�ܲ���WAV�Ĺʴ˵㲻��
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

