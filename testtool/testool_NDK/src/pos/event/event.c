/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: eventģ��
* file name			: event.c
* Author 			: linwl
* version			: 
* DATE				: 20160912
* directory 		: 
* description		: eventģ�������
* related document	: �߶�ƽ̨ͳһAPI��̬���ӿ����ֲ�V2.00.doc
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define	TESTMODULE	"eventģ��"

/****************************************************************
* function name 	 	: event
* functional description 	: eventģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	      linwl        20160912	created
*				  	
*****************************************************************/
void event(PF *list, unsigned char id[])
{
	cls_show_msg1(1, "%s����...", TESTMODULE);
	process(list, id);
	
	return;
}
