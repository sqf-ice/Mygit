/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: sysģ�������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define	TESTMODULE	"sysģ��"

/****************************************************************
* function name 	 	: sys
* functional description 	: sysģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	      linwei        20130121	created
*				  	
*****************************************************************/
void sys(PF *list, unsigned char id[])
{
#if STABAR_ENABLE
	NDK_ScrStatusbar(STATUSBAR_DISP_ALL);
#endif
	cls_show_msg1(1, "%s����...", TESTMODULE);
	process(list, id);
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}

