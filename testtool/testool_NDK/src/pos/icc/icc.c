/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: iccģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"iccģ��"
#define MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
//�������µ�ɰ�
void icpowerdown_all(void)
{
	/*private & local definition*/
#include "icsam.h"
	int i=0, maxid=ICSAMNUM+1/*SZ_ARRAY(CardNo)*/;

	/*process body*/
	for(i=0;i<maxid;i++)
		NDK_IccPowerDown(CardNo[i]);
}

/****************************************************************
* function name 	 	: icc
* functional description 	:iccģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenfm      20130107	created
*				  	
*****************************************************************/
void icc(PF *list, unsigned char id[])//��ں������������ļ���,Ҳ��ģ����
{
	//����Ϊ�ֲ�����

	//����Ϊ������
	if(cls_show_msg1(MAXWAITTIME, "%s����...��ȷ������������ic/sam���п�...ESC�˳���ģ�����", TESTMODULE)==ESC)
		return;
	
	icpowerdown_all();
	process(list, id);
	icpowerdown_all();
	return;
}

