/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: ���� NDK_IccSetType, NDK_IccGetType�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_IccSetType,NDK_IccGetType"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: icc2
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	 	 chenfm     20130107	created
*				  	 
*****************************************************************/
void icc2(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int ret= -1, j=0;
	EM_ICTYPE pemIctype;
	char IC_TYPE[][16]={
#if IC1_ENABLE	 //Ĭ�����޸�ΪIC1_ENABLE 20180510 
						"IC", 
#endif
#if SAM1_ENABLE	//20140218 ������ȷ��K21��֧��С��
						"SAM1", 
#endif
#if SAM2_ENABLE	//20140218 ������ȷ��K21��֧��С��
						"SAM2",
#endif
#if SAM3_ENABLE
						"SAM3",
#endif
#if SAM4_ENABLE
						"SAM4",
#endif
						"at24c32", "at24c01", "at24c02", "at24c04", "at24c08", "at24c16", "at24c32", "at24c64", "sle44x2", "sle44x8", "sle5528", "at88sc102", "at88sc1604", "at88sc1608", "at88sc153"};
	EM_ICTYPE IcType[]={
#if IC1_ENABLE	 //Ĭ�����޸�ΪIC1_ENABLE 20180510 
						ICTYPE_IC, 
#endif
#if SAM1_ENABLE	//20140218 ������ȷ��K21��֧��С��	
						ICTYPE_SAM1, 
#endif
#if SAM2_ENABLE //20140218 ������ȷ��K21��֧��С��
						ICTYPE_SAM2,
#endif						
#if SAM3_ENABLE	
						ICTYPE_SAM3,
#endif
#if SAM4_ENABLE	
						ICTYPE_SAM4,
#endif
						ICTYPE_M_1, ICTYPE_M_1_1, ICTYPE_M_1_2, ICTYPE_M_1_4, ICTYPE_M_1_8, ICTYPE_M_1_16, ICTYPE_M_1_32, ICTYPE_M_1_64, ICTYPE_M_2, ICTYPE_M_3, ICTYPE_M_3, ICTYPE_M_4, ICTYPE_M_5, ICTYPE_M_6, ICTYPE_M_7};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��

	//case1:���ÿ����ͣ���ȡ������Ӧ�������õ�һ��
	for(j=0;j<SZ_ARRAY(IC_TYPE);j++)
	{
		if((ret=NDK_IccSetType(IcType[j]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, IC_TYPE[j], ret);
			RETURN;
		}
		if((ret=NDK_IccGetType(&pemIctype))!=NDK_OK||pemIctype!=IcType[j])
		{
			send_result("line %d:%s����ʧ��(%d:%s, %d)", __LINE__, TESTAPI, pemIctype, IC_TYPE[j], ret);
			RETURN;
		}
	}

	//case3:������IC��������SAM1��������ȡ����Ӧ�������һ�����õ�SAM1�� 20140603 added by jiangym
	if((ret=NDK_IccSetType(IcType[0]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, IC_TYPE[0], ret);
		RETURN;
	}
	if((ret=NDK_IccSetType(IcType[1]))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, IC_TYPE[1], ret);
		RETURN;
	}
	if((ret=NDK_IccGetType(&pemIctype))!=NDK_OK||pemIctype!=IcType[1])
	{
		send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, IC_TYPE[j], ret);
		RETURN;
	}

	//case2:�쳣����
	if((ret=NDK_IccSetType(-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_IccSetType(ICTYPE_M_1_64+1))!=NDK_ERR_PARA)//������07816�������ã����ԷǷ�ֵҪ��+1�ĳ�+2 20140815 linwl  ��������153��20150325
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_IccGetType(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	

	//�ָ�Ĭ�ϵ�����
	if((ret=NDK_IccSetType(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); // �ָ�ȱʡ����
		RETURN;
	}
	if((ret=NDK_IccGetType(&pemIctype))!=NDK_OK||pemIctype!=ICTYPE_IC)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pemIctype);
		RETURN;
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
	return;
}

