/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: icc ģ��
* file name			: 
* Author 			: liny
* version			: 
* DATE				: 20170901
* directory 			: 
* description			: �����ϵ���icc ģ�麯�����Ƿ���ܷ�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"
/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"ICCģ����������"

/*------------global variables definition-----------------------*/
/*----------global variables declaration("extern")--------------*/
/*---------------structure definition---------------------------*/
//����ö��
typedef enum {
    	IccPowerUp =0,
#if !(ANDIROD_ENABLE||K21_ENABLE)
	IccSetType,			 
	IccGetType,
#endif
	IccPowerDown,
	Iccrw,
	IccDetect,
	MaxNum,
}EM_ICC;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*
*****************************************************************/
//�������ICCģ�麯��
static int ICC_RandTest(int funnum)
{
    //����Ϊ�ֲ�����
    	uchar psAtrbuf[256]={0},sendbuf[256]={'\x00', '\x84', '\x00', '\x00', 8}, recvbuf[256]={0};
	int pnAtrlen=0, recvlen=0,pnSta= -1;
#if !K21_ENABLE
	EM_ICTYPE pemIctype;
#endif
	//���忨Ƭ����
	EM_ICTYPE IcType[]={
#if IC1_ENABLE
					ICTYPE_IC, 
#endif
#if SAM1_ENABLE	
					ICTYPE_SAM1, 
#endif
#if SAM2_ENABLE
					ICTYPE_SAM2,
#endif						
#if SAM3_ENABLE	
					ICTYPE_SAM3,
#endif
#if SAM4_ENABLE	
					ICTYPE_SAM4,
#endif
					ICTYPE_M_1, ICTYPE_M_1_1, ICTYPE_M_1_2, ICTYPE_M_1_4, ICTYPE_M_1_8, ICTYPE_M_1_16, ICTYPE_M_1_32, ICTYPE_M_1_64, ICTYPE_M_2, ICTYPE_M_3, ICTYPE_M_3, ICTYPE_M_4, ICTYPE_M_5, ICTYPE_M_6, ICTYPE_M_7};
   	 srand((unsigned)time(NULL));
	switch(funnum)
    	{
       		case IccPowerUp:
			NDK_IccPowerUp(IcType[rand()%SZ_ARRAY(IcType)],psAtrbuf, &pnAtrlen);
			break;
#if !K21_ENABLE	
    		case IccSetType:
			NDK_IccSetType(IcType[rand()%SZ_ARRAY(IcType)]);
		    break;
		case IccGetType:
			NDK_IccGetType(&pemIctype);
			break;
#endif
		case IccPowerDown:
			NDK_IccPowerDown(IcType[rand()%SZ_ARRAY(IcType)]);
			break;
		case Iccrw:
			NDK_Iccrw(IcType[rand()%SZ_ARRAY(IcType)],sizeof(sendbuf),sendbuf,&recvlen,recvbuf);
			break;
		case IccDetect:
			NDK_IccDetect(&pnSta);
			break;
    	}
	return SUCC;
}
//��������������ú��� ICC_RandTest���������ϲ���
void icc200(void)
{
	/*private & local definition*/
	int i = 0, bak = 0,ret = -1,cnt =50, len = 0;//cnt ����   
	int num[30]={0}, j = 0;
#if K21_ENABLE
	char *iccstr[] = {"NDK_IccPowerUp","NDK_IccPowerDown","NDK_Iccrw","NDK_IccDetect"};
#else
	char *iccstr[] = {"NDK_IccPowerUp","NDK_IccSetType","NDK_IccGetType","NDK_IccPowerDown","NDK_Iccrw","NDK_IccDetect"};
#endif
	
	/*process body*/
	if(MaxNum>20)
		len = MaxNum;	 
	else
		len = 20;
	while(cnt)
	{	
		cnt--;
		bak++;			
		//���������
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
		}
		cls_printf("��%d�����˳��:",bak);
		for(i=0;i<len;i++)
		{
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret =  ICC_RandTest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",iccstr[num[j]]);
				goto ERR;
			}
		}			
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_IccPowerDown(ICTYPE_IC);//�µ�
	return;
}


