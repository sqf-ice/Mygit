/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20121217
* directory 		: 
* description		: fsģ�������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define	TESTMODULE	"fsģ��"

/****************************************************************
* function name 	 	: fs
* functional description 	: fsģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm         20121217	created
*				  	
*****************************************************************/
void fs(PF *list, unsigned char id[])
{
	cls_show_msg1(1, "%s����...", TESTMODULE); 
	process(list, id);
	return;
}

