/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sslģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: sslģ�������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

#define	TESTMODULE	"sslģ��"

/****************************************************************
* function name 	 	: ssl
* functional description 	: sslģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm         20120905	created
*				  	
*****************************************************************/
void ssl(PF *list, unsigned char id[])
{
	cls_show_msg1(1, "%s����...", TESTMODULE); 

	switch(conf_conn_sock())//port=1111,����������123456
	{
  	case SUCC:
		break;
  	case FAIL:
		cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
  	case NDK_ERR_QUIT:
		//�û�����
  	default:
		return;
		break;
	}

	process(list, id);
	return;
}

