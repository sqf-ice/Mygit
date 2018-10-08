/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc9.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: �����ܷ���ȷ����Mifare-1��(APDU��д)
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_M1xxx����"

/*------------global variables definition-----------------------*/
static int (*rf5_subcase)(uchar *, int) = NULL;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
static int subcase_0(uchar *UID, int UidLen);
static int subcase_1(uchar *UID, int UidLen);
static int subcase_2(uchar *UID, int UidLen);
static int subcase_3(uchar *UID, int UidLen);
static int subcase_4(uchar *UID, int UidLen);
static int subcase_5(uchar *UID, int UidLen);

/*---------------functions definition---------------------------*/
//������������ǰ��������һ��Ϊ"Ѱ���ɹ�",֮��Ϳɵ���������������
//Ŀǰ���¼�������������˳�����(���ڼ��ٴ����еĿ���,�����������������ϵ)
//������Ʋ�������״̬ģʽ��C����ʵ��,�нϺõĿ���չ�����ά����
//������ʹ��ע��:�洢,װ��key��ʧ��(���뿨����ͨ��),�ǲ���Ҫ���·�Ѱ�������,��һ��Ҫ��ô��,Ӧ�ر����߳����ٴ�,��Ѱ��...

static int subcase_0(uchar *UID, int UidLen)
{
#if !ANDIROD_ENABLE  //�������Ʒ���˵�߶˲�ƷNDK_M1KeyStore�ѷϳ� �ʲ��� 20180718 modify
	/*private & local definition*/
	int ret=0, ret1=0, ret2=0;

	/*process body*/
	//cls_show_msg1(2, "�洢��ԿA��...");
	//NDK_M1KeyStorey�쳣��������
	if((ret=NDK_M1KeyStore(0xff, 0x00, AUTHKEY))!=NDK_ERR_PARA
	||(ret=NDK_M1KeyStore(AUTHKEY_TYPE_B, 0x10, AUTHKEY))!=NDK_ERR_PARA
	||(ret=NDK_M1KeyStore(AUTHKEY_TYPE_B, 0x00, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2);
		return FAIL;
	}	
	//NDK_M1KeyStorey�������ԣ�Ӧ�ɹ�����
	if((ret=NDK_M1KeyStore(AUTHKEY_TYPE_B, 0x00, AUTHKEY))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_M1KeyStore(AUTHKEY_TYPE_A, 0x00, AUTHKEY))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_M1InternalAuthen(-1, UID, AUTHKEY_TYPE_A, 0x01))!=NDK_ERR_PARA
	||(ret1=NDK_M1InternalAuthen(UidLen, NULL, AUTHKEY_TYPE_A, 0x01))!=NDK_ERR_PARA
	||(ret2=NDK_M1InternalAuthen(UidLen, UID, 0xff, 0x01))!=NDK_ERR_PARA)
	{
		send_result("line %d:����ʧ��(%d,%d,%d)", __LINE__, ret, ret1, ret2);
		return FAIL;
	}
#if !K21_ENABLE  //�˽ӿ��ѷϳ�,Ϊ�˱�����ν�����Իᱨ��,�Ͷ˲�Ʒ����˵� 20180714 modify
	if((ret=NDK_M1InternalAuthen(UidLen, UID, AUTHKEY_TYPE_A, 0x01))==NDK_OK)//!=MI_AUTHERR)//ʵ�ʷ���-1??
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
#endif
#endif
	rf5_subcase=subcase_1;
	return SUCC;
}

static int subcase_1(uchar *UID, int UidLen)
{
	/*private & local definition*/
	int ret=0, pnDatalen=0;
	uchar out[MAXLEN_DATA]={0};
	/*process body*/

	if((ret=NDK_M1Read(0x01, &pnDatalen, out))!=NDK_ERR_MI_NOTAUTHERR)//==MI_OK)//!=MI_NOTAUTHERR)//ʵ�ʷ���-12??
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	rf5_subcase=subcase_2;
	return SUCC;
}

static int subcase_2(uchar *UID, int UidLen)
{
#if !ANDIROD_ENABLE  //�������Ʒ���˵�߶˲�ƷNDK_M1KeyLoad�ѷϳ� �ʲ��� 20180718 modify
	/*private & local definition*/
	int ret=0, ret1=0;
	
	/*process body*/
	//NDK_M1KeyLoad�쳣����
	if((ret=NDK_M1KeyLoad(0xff, 0x00))!=NDK_ERR_PARA||(ret1=NDK_M1KeyLoad(AUTHKEY_TYPE_A, 0x10))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		return FAIL;
	}
	//NDK_M1KeyLoad��������
	if((ret=NDK_M1KeyLoad(AUTHKEY_TYPE_A, 0x00))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_M1InternalAuthen(UidLen, UID, AUTHKEY_TYPE_A, 0x01))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}

	//caseX:������֤����20130225add by zhangnw.ԭ�Ȳ�֧������ʹ��,�����֤֮�����Ҫ���µ�,����֧�ִ�ʹ�÷�ʽ��
	if((ret=NDK_M1InternalAuthen(UidLen, UID, AUTHKEY_TYPE_A, 0x01))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
#endif
	//ע��:"���ִ����Ӧ��ϵ"ָ�ڲ���֤��ʽ��,store/load��ucKeynum��NDK_M1InternalAuthen��ucBlocknum����Ӧ��sector�Ų�Ҫ��Ҫһ��,
	//�������ucKeynum=9ȥ��֤ucBlocknum=01��10,ֻ��Ҫ00����(01����)��02����(10����)��key��ʹ��API����POS�ϵ�keyһ������
	//�����ⲿ��֤����,������������ucKeynum,���޴�һ˵.���������,Ҫ��д��ǰ,�ڻ�����֤���Ǳ����,ͬʱ��д��
	//�������֤�Ŀ��������������һ��.������˵��֤��һ����,�������������Բ�����.
#if 0	//ĿǰMIFARE-1�������ִ����Ӧ��ϵ,���ν���Ӧ����������ȷ��Ӧ
	if((ret=CMD_Auth(AUTHKEY_TYPE_A, UID, 0x00, 0x09))==MI_OK)//��00��������Կ��֤���������Ŀ�
	{
		send_result("line %d:��֤Ӧʧ��(ret=%d)", __LINE__, ret);
		return FAIL;
	}
#endif

	rf5_subcase=subcase_3;
	return SUCC;
}

static int subcase_3(uchar *UID, int UidLen)
{
	/*private & local definition*/
	int ret=0, ret1=0, pnDatalen=0;
	uchar out[MAXLEN_DATA]={0};
	//uchar inc[]={0x01, 0x00, 0x00, 0x00};//"\x01\x00\x00\x00";//uint inc = 0x01;

	/*process body*/
#if !ANDIROD_ENABLE //���ݿ�������NDK_M1InternalAuthen�ѷϳ�20180718 modify
	//subcase_4��load����Կ��,����ֻ��Ҫ������֤һ�¼���
	if((ret=NDK_M1InternalAuthen(UidLen, UID, AUTHKEY_TYPE_A, 0x01))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
#endif

	//NDK_M1Write�쳣����1�������쳣
	if((ret=NDK_M1Write(0x02, NULL, BLK02DATA_FF))!=NDK_ERR_PARA||(ret1=NDK_M1Write(0x02, &pnDatalen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:����ʧ��(%d,%d)", __LINE__, ret, ret1);
		return FAIL;
	}
	
	//NDK_M1Write�쳣����2��δ��֤�����ж�д
	pnDatalen=LEN_BLKDATA;//strlen(BLK02DATA_FF);
	if((ret=NDK_M1Write(0x09, &pnDatalen, BLK02DATA_FF))==NDK_OK/*NDK_ERR_MI_NOTAUTHERR*/||(ret1=NDK_M1Read(0x09, &pnDatalen, out))==NDK_OK/*NDK_ERR_MI_NOTAUTHERR*/)
	{
		send_result("line %d:����ʧ��(%d,%d)", __LINE__, ret, ret1);
		return FAIL;
	}
	
	rf5_subcase=subcase_4;
	return SUCC;
}

static int subcase_4(uchar *UID, int UidLen)
{
	/*private & local definition*/
	int ret=0, ret1=0, pnDatalen=0;
	uchar out[MAXLEN_DATA]={0};
	uchar inc[]={0x01, 0x00, 0x00, 0x00};//"\x01\x00\x00\x00";//uint inc = 0x01;

	/*process body*/
#if !ANDIROD_ENABLE //���ݿ�������NDK_M1InternalAuthen�ѷϳ�20180718 modify
	//subcase_4��load����Կ��,����ֻ��Ҫ������֤һ�¼���
	if((ret=NDK_M1InternalAuthen(UidLen, UID, AUTHKEY_TYPE_A, 0x01))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
#endif

	//NDK_M1Write��������
	pnDatalen=LEN_BLKDATA;//strlen(BLK02DATA_FF);
	if((ret=NDK_M1Write(0x02, &pnDatalen, BLK02DATA_FF))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
	//NDK_M1Read�쳣����
	if((ret=NDK_M1Read(0x01, NULL, out))!=NDK_ERR_PARA||(ret1=NDK_M1Read(0x01, &pnDatalen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		return FAIL;
	}
	//NDK_M1Read��������
	if((ret=NDK_M1Read(0x02, &pnDatalen, out))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	else
	{
		if(memcmp(BLK02DATA_FF, out, LEN_BLKDATA))
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
	}
	//����������֮ǰ,���ϵ����ݱ����ȷ���Ǯ����ʽ,���������ʧ��
	if((ret=NDK_M1Increment(0x02, sizeof(inc), NULL))!=NDK_ERR_PARA||(ret1=NDK_M1Increment(0x02, -1, inc))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		return FAIL;
	}	
	if((ret=NDK_M1Increment(0x02, sizeof(inc), inc))!=NDK_ERR_MI_INCRERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_M1Decrement(0x02, sizeof(inc), NULL))!=NDK_ERR_PARA||(ret1=NDK_M1Decrement(0x02, -1, inc))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		return FAIL;
	}
	if((ret=NDK_M1Decrement(0x02, sizeof(inc), inc))!=NDK_ERR_MI_DECRERR)//ʵ�ʷ���-1
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}

	rf5_subcase=subcase_5;
	return SUCC;
}

static int subcase_5(uchar *UID, int UidLen)
{
	/*private & local definition*/
	int ret=0, pnDatalen=0;
	uchar out[MAXLEN_DATA]={0};
	uchar inc[]={0x01, 0x00, 0x00, 0x00};//"\x01\x00\x00\x00";//uint inc = 0x01;

	/*process body*/
#if !ANDIROD_ENABLE //���ݿ�������NDK_M1InternalAuthen�ѷϳ�20180718 modify
	//subcase_4��load����Կ��,����ֻ��Ҫ������֤һ�¼���
	if((ret=NDK_M1InternalAuthen(UidLen, UID, AUTHKEY_TYPE_A, 0x01))!=NDK_OK)//!=MI_AUTHERR)//ʵ�ʷ���-1??
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
#endif
	
	pnDatalen=LEN_BLKDATA;//strlen(BLK02DATA_ORI);
	if((ret=NDK_M1Write(0x02, &pnDatalen, BLK02DATA_ORI))!=NDK_OK)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}		
#if 0 //Ŀǰ�ĵײ������������ֱ�ӽ�������E2ROM(����Ҫ����,�ָ�ָ��,������ָ��ʵ������Ч��,ֱ�ӷ��سɹ�)
		if((ret=CMD_BLKInc(0x02, inc))!=MI_OK)
		{
			send_result("line %d:����������ʧ��(ret=%d)", __LINE__, ret);
			return FAIL;
		}
		if((ret=CMD_BLKRestore(0x02))!=MI_OK)
		{
			send_result("line %d:��ָ�ʧ��(ret=%d)", __LINE__, ret);
			return FAIL;		
		}
#endif
	if((ret=NDK_M1Increment(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_M1Increment(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	
#if 0 //Transfer��Ч����еĵ���,����Ϊ +1 3��
	if((ret=NDK_M1Decrement(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
#else
	if((ret=NDK_M1Increment(0x02, sizeof(inc), inc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_M1Transfer(0x02))!=NDK_OK)//ע��:NDK��Transfer��Ч,��ֻ��Transferǰ���һ��inc/dec��Ч
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}	
#endif

	//�����������õ�����ʵ��Ч��Ϊ+1
#if 0
		if((ret=NDK_M1Read(0x02,&pnDatalen,out))!=MI_OK)
		{
			send_result("line %d:����ʧ��(%d)", __LINE__, ret);
			return FAIL;
		}
		else
		{
			if(memcmp(BLK02DATA_ORI, out, LEN_BLKDATA))
			{
				send_result("line %d:����ʧ��", __LINE__);
				return FAIL;
			}			
		}
		if((ret=NDK_M1Transfer(0x02))!=MI_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
			return FAIL;			
		}
#endif
	if((ret=NDK_M1Read(0x02, &pnDatalen, out))!=NDK_OK||pnDatalen!=LEN_BLKDATA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	else
	{
		if(memcmp(BLK02DATA_INC1, out, LEN_BLKDATA))
		{
			send_result("line %d:���дʧ��", __LINE__);
			return FAIL;
		}
	}
	rf5_subcase=NULL;
	return SUCC;
}

static int handle_subcase(uchar *UID, int UidLen)
{
	/*private & local definition*/

	/*process body*/
	if(rf5_subcase!=NULL)
		return rf5_subcase(UID, UidLen);
	else
		return (-2);
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		    chenfm	        20130108	 	created
*****************************************************************/
void rfc9(void)
{
	/*private & local definition*/
	uchar UID[LEN_UID]={0}, UID1[LEN_UID]={0}, psDatabuf[256]={0}, psSakbuf[256]={0};
	int ret=0, ret1= -1, ret2= -1, subcase=0, pnDatalen=0;
	rf5_subcase=subcase_0;
	
	/*process body*/
#if !K21_ENABLE//�Ͷ�ƽ̨���ڿռ����ⲻ֧���ڲ���֤��ʽ�ϵ�����������Կ�Ĺ��ܣ����Բ�����ز��� 20150324 linwl
	if(ENTER!=cls_show_msg("�Ƿ�Ϊ�����ϵ���״����б�����?\n[ȷ��]��[����]��"))
	{
		send_result("line %d:%s������ֹ(���������µ�)", __LINE__, TESTAPI);
		return;
	}
	if(ENTER==cls_show_msg("�Ƿ�����������Կ�Ĳ���?\n[ȷ��]��[����]��"))
	{
		rf5_subcase=subcase_1;
	}
#endif

	cls_printf("����%s...", TESTAPI);
	NDK_RfidPiccDeactivate(0);//����

	cls_show_msg("�뽫��Ӧ������Mifare-1��,���������...");	
	//case1:���ÿ�����ΪB����ѰM1����Ӧʧ�ܷ��ء�
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_B))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_ERR_MI_CardTypeErr)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//caseX:���ÿ�����ΪAB����ѰM1����Ӧʧ�ܷ��ء�
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_ERR_MI_CardTypeErr)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//caseX:���ÿ�����ΪA����ѰM1����
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:MI_NOTAGERR
	cls_show_msg("�뽫��Ӧ����Mifare-1���ƿ�,���������...");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:���δ������//zhangnw20110617add
	//�ԽӴ���ǽӶ���,�β�󿨻�ص����̬,POSҲӦ�ص����̬.Ȼ�������ϵ�/����...
	//ͬ�ſ�
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("�뽫��Ӧ����Mifare-1���ƿ�,���������...");
	if((ret=NDK_M1Anti(&pnDatalen, UID))==NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	cls_show_msg("���ڸ�Ӧ������ͬһ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Anti(&pnDatalen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	cls_show_msg("�뽫��Ӧ����Mifare-1���ƿ�,���������...");
	if((ret=NDK_M1Select(pnDatalen, UID, psSakbuf))==NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���²��Ի�������������չ�쳣���ԣ�ͬʱ��һ�������쳣���������������ԣ���
	cls_show_msg("���ڸ�Ӧ������ͬһ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	if((ret=NDK_M1Request(REQA, NULL, psDatabuf))!=NDK_ERR_PARA||(ret=NDK_M1Request(REQA, &pnDatalen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}	
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M1Anti(NULL, UID1))!=NDK_ERR_PARA||(ret1=NDK_M1Anti(&pnDatalen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}	
	if((ret=NDK_M1Anti(&pnDatalen, UID1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(memcmp(UID, UID1, sizeof(UID)))
	{
		send_result("line %d:ͬ�ſ�UID��һ��", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_M1Select(-1, UID1, psSakbuf))!=NDK_ERR_PARA
	||(ret1=NDK_M1Select(pnDatalen, NULL, psSakbuf))!=NDK_ERR_PARA
	||(ret2=NDK_M1Select(pnDatalen, UID1, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2);
		GOTOERR;
	}
	if(((ret=NDK_M1Select(pnDatalen, UID1, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��ͬ��,��֤���ص�UIDӦ��ͬ
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	//GUI_DispStringEx(12*6, 0, "��", TEXT_ATTRIBUTE_REVERSE);
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_M1Anti(&pnDatalen, UID1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(!memcmp(UID, UID1, sizeof(UID)))
	{
		send_result("line %d:��ͬ��UIDӦ��һ��", __LINE__);
		GOTOERR;
	}
	NDK_RfidPiccDeactivate(10);//�ر�һ���ٴ򿪱������һ�������ĸ���

#if !(defined ME30MH||defined ME32||defined ME30GX||defined ME32GX||defined ME15B) //����˵ME30MH��֧����Կ�洢\װ��\�ڲ���֤��������ӿڹ������������ 20170527
	//case4:��������������
TAG:cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");//Ŀǰ��֧�������ķ���ͻ, (subcase==0)?"��":"ͬһ");
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_M1Anti(&pnDatalen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_M1Select(pnDatalen, UID, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2, "��׼����Ѱ���ѳɹ�!");

	//��ϸ������������
	cls_show_msg1(2, "%d#����������...", subcase++);
	ret=handle_subcase(UID, pnDatalen);
	if(ret==FAIL)
		GOTOERR
	else if(ret==SUCC)
	{
		show_msg1(2, "�ɹ�!");
		//˵��:�������������Ƿ����洢��װ��keyʧ��,ʵ���Ͽ��Բ�Ѱ��(���뿨ͨ��û�д���,��ȥѰ����Դ������),��Ϊ�˵��õ�ͳһ,���������Ϊ�ȹر��ٴ򿪳�,�Ա�֤Ѱ���ɹ�
		NDK_RfidPiccDeactivate(10);//�ر�һ���ٴ򿪱������һ�������ĸ���
		goto TAG;
	}
	else	//-2
		;
#endif

	//CMD_Powerdown(������)
	cls_printf("�����ѹر�,255ms���.�뱣�ָ�Ӧ����һֱ��Mifare-1��,�����ĵȴ�!");//cls_printf("�����ѹر�,10s���.�뱣�ָ�Ӧ����һֱ��Mifare-1��,�����ĵȴ�!");
	if((ret=NDK_RfidPiccDeactivate(255))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0 //�ײ����Ϊ������
	if((ret=NDK_M1Request(REQA))!=MI_RESETERR)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI,ret);
		GOTOERR;
	}
	//cls_printf("�����ѹر�,10s���.�뱣�ָ�Ӧ����һֱ��Mifare-1��,�����ĵȴ�!");
	msdelay(10000);
	if((ret=NDK_RfidPiccDeactivate(10))!=NDK_OK)//�ر�10s���ٴ�
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI,ret);
		GOTOERR;
	}
	msdelay(10);
#endif
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);//�����Թر�
	return;
}

