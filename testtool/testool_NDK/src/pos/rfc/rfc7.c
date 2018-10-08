/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc7.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_PiccQuickRequest�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_PiccQuickRequest"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		    chenfm	        20130108	 	created
*****************************************************************/
void rfc7(void)
{
	//����Ϊ�ֲ�����
	uchar psPicctype;
	uchar psRecebuf[300]={0}, psDatabuf[256]={0};
	int ret= -1, pnDatalen=0, rcvlen=0, i=0, j=0;
	ushort sw=0;
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	uchar Select_1pay[]= {'\x00', '\x84', '\x00', '\x00', 8};
#else
	uchar Select_1pay[]={0x00, 0xA4, 0x04, 0x00, 0x0E, '1', 'P', 'A', 'Y', '.', 'S', 'Y', 'S', '.', 'D', 'D', 'F', '0', '1', 0x00};
#endif		

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//ԭ��for����B���Ķ�д
	/*
	Ҫ�����е�ͳһAPI��д���ŵĿ�����Ҫ�ڳ�ʼʱ(GetDS5002stateǰ)����һ������
	setpiccquickseek(1);�����׵Ŀ���Ѱ��
	Ԥ������ӿ���Ҫ��2��Ŀ�ģ�
	1�����������ķǱ�׼��
	2��ĳЩ��EMV���ԣ�ǿ��Ѱ���ٶȵȵĲ���
	��Ȼ,�Ա�׼��B�����ǵ���������ӿ�,Ҳ���������������.�����ŵĿ����Ǳ�׼��,����������ΪΪ��׼��,������һ�㲻����ʹ�õ��˽ӿ�
	*/	

	//case1:���Կ���Ѱ��ģʽ
	//case2:������ͨѰ��ģʽ
	for(i=0;i<2;i++)
	{
		//����Ѱ��ģʽ������Ѱ������ͨѰ����
		if((ret=NDK_PiccQuickRequest(i==0?0:rand()%MAX_SIGNINT+1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		for(j=0;j<2;j++)
		{
			//ѰA��B��
			cls_show_msg("���ڸ�Ӧ�����ñ�׼%s���������������...", j==0?"A":"B");
			if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=(j==0?RFCARD_TYPE_A:RFCARD_TYPE_B))
			{
				send_result("line %d:%s����ʧ��(%d, 0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
				GOTOERR;
			}	
			if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=NDK_OK||((sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x9000&&(sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x6d00))
			{
				send_result("line %d:%s����ʧ��(%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
				GOTOERR;
			}
			if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
ERR:
	NDK_PiccQuickRequest(0);//�رռ��׿���Ѱ������,�����µ��Ҳ��ָ�Ĭ������0.�˴���Ϊ�˷�ֹʹ�ú���B�������׼B��ʱ���Ի�ͨ��
	NDK_RfidPiccDeactivate(0);
	return;
}

