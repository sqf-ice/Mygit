/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: spfsģ��
* file name		: 
* Author 			: linying
* version			: 
* DATE			: 20180828
* directory 		: 
* description		:�߶˲�Ʒ fsģ�������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define	TESTMODULE	"spfsģ��"

/****************************************************************
* function name 	 	: spfs
* functional description 	:�߶˲�ƷK21��fsģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	        linying       20180828	created
*				  	
*****************************************************************/
void spfs(PF *list, unsigned char id[])
{
	cls_show_msg1(1, "%s����...", TESTMODULE); 
	process(list, id);
	return;
}

