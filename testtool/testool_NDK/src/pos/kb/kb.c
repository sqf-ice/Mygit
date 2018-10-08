/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: 
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"����ģ��"
#define TESTBGPIC "test_bg.png"//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ��������

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: kb
* functional description 	: ����ģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*****************************************************************/
void kb(PF* list, unsigned char id[])
{
	/*private & local definition*/
#if !K21_ENABLE
	int ret = -1;
#endif
	/*process body*/
	if(auto_flag==1)//����ģ�鲻֧���Զ�����
	{
		g_UCID = 0;
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTMODULE);
		return;
	}

#if STABAR_ENABLE
	NDK_ScrStatusbar(STATUSBAR_DISP_ALL);
#endif

#if !K21_ENABLE
	//20140611 linwl added ���䱳��ͼƬ��֤������������뷨�����������Ļ����ͼ����ˢ���쳣
	if(cls_show_msg("�Ƿ����ñ���ͼƬ:��[ȷ��],��[����]")==ENTER)
	{
		cls_show_msg("��������%s����ͼƬ�����������",TESTBGPIC);
		if((ret=NDK_ScrSetbgPic(TESTBGPIC))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTMODULE, ret);
			return;
		}
	}
#endif

	cls_show_msg1(1, "%s����...", TESTMODULE);
	process(list, id);
	
#if !K21_ENABLE
	NDK_ScrClrbgPic();
#endif

#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}

