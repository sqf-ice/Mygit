/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: iccģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130107
* directory 		: 
* description		: ���� NDK_Iccrw�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_Iccrw"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern void icpowerdown_all(void);

/*---------------functions definition---------------------------*/
static int test_iccard_errcode(void)
{
	//����Ϊ�ֲ�����
	uchar psAtrbuf[256]={0}, sendbuf[]={'\x00', '\x84', '\x00', '\x00', 8}, recvbuf[256]={0};
	int pnAtrlen=0, recvlen=0, ret=0;
#if !K21_ENABLE
	uchar  overload[256]={0};
#endif
	//����Ϊ������
	//case1:NDK_ERR_ICC_CARDNOREADY_ERR  ��δ׼���� 
	cls_show_msg("��ȷ��IC���Ƴ�,�����������...");
	memset(psAtrbuf, 0, sizeof(psAtrbuf));
#if  !K21_ENABLE //K21�޷�ȡIC���忨״̬,ֱ�ӷ����쳣
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_ERR_ICC_CARDNOREADY_ERR)
#else
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_ERR_IOCTL)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}

	//case2��δ�ϵ磬ʹ��NDK_Iccrw��ȡ��������Ӧʧ�ܷ��ء�
	cls_show_msg("��ȷ��IC�Ѳ���,�����������...");	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &recvlen, recvbuf))!=NDK_ERR_IOCTL)//NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	cls_show_msg1(2, "����������ͨ��!");

	//case3��NDK_ERR_ICC_CARDPULL_ERR�����γ��ˣ���
	cls_show_msg("��ȷ��IC�Ѳ���,�����������...");
	memset(psAtrbuf, 0, sizeof(psAtrbuf));
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	cls_show_msg("IC�ϵ�ɹ�!��β�һ��IC,�����������...");
	//ʹ��iccrw��ȡ���������
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &recvlen, recvbuf))!=NDK_ERR_IOCTL)//NDK_ERR_ICC_CARDPULL_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}	

	//case4:NDK_ERR_ICC_COM_ERR���������
	cls_show_msg("��ȷ��IC�Ѳ���,�����������...");
	memset(psAtrbuf, 0, sizeof(psAtrbuf));
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	memset(recvbuf, 0, sizeof(recvbuf));
#if  0//K21_ENABLE&&!ANDIROD_ENABLE&&!defined ME30THM &&!defined ME32
	if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf)-2, sendbuf+2, &recvlen, recvbuf))!=NDK_OK)//K21 APIֱ�ӷ��سɹ������ý���ɵײ��������� 20140526 ���redmine2325
#else  //������ȷ���еͶ˶�ͳһ����������жϣ������ʽ���Ծͷ��ش��� 20180129 modify by jym
	if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf)-2, sendbuf+2, &recvlen, recvbuf))!=NDK_ERR_IOCTL)//NDK_ERR_ICC_COM_ERR)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}	
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	cls_show_msg1(2, "����������ͨ��!");

#if !K21_ENABLE//20150423 �����͹�����λ����K21ƽ̨û��254���ƣ��ײ�ֻ����ָ�����Ƭ������Ƭ�ķ����޷�Ԥ�ڣ��Ϳ�Ƭ�й�ϵ����K21�����Ը�����
	//case5:APDU���ݳ��ȴ���254������IC����д�����ݳ���������254���ֽ�
	cls_show_msg("��ȷ��IC�Ѳ���,�����������...");
	//ע��:��һ����ִ�к�(�������ߵ���Ϊ��iccrw����),IC�����µ�,��Ҫ�����ϵ�
	memset(psAtrbuf, 0, sizeof(psAtrbuf));
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	memset(overload, 0, sizeof(overload));
	memcpy(overload, sendbuf, sizeof(sendbuf));
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(overload), overload, &recvlen, recvbuf))!=NDK_ERR_IOCTL)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	cls_show_msg1(2, "����������ͨ��!");

	//�µ�
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
#endif

	return SUCC;
}

/****************************************************************
* function name 	 	: icc3
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	 	 chenfm     20130107	created
*				  	 
*****************************************************************/
void icc3(void)
{
	//����Ϊ�ֲ�����
#include "icsam.h"

	int i=0, ret=0, ret1=0, ret2=0, ret3=0, ret4=0, rlen=0, maxid=ICSAMNUM+1/*SZ_ARRAY(CardNo)*/, pnAtrlen=0;
	uchar recvbuf[128]={0}, psAtrbuf[256]={0};
	ushort sw=0;
	uchar sendbuf[]={'\x00', '\x84', '\x00', '\x00', 8};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��
	icpowerdown_all();

	//case1:��ȷ����
	do
	{
		//case1.1:�ϵ�,ʹ��NDK_Iccrw��ȡ���������,Ӧ�óɹ����ء�
		memset(psAtrbuf, 0, sizeof(psAtrbuf));
		NDK_IccPowerDown(CardNo[i]);
#if 0//linwl20130614 �����޸����µ�ɹ������NDK_IccSetType�������õĿ������޹أ�ֻ�����µ纯��һ�������й�
		if((ret=NDK_IccSetType(CardNo[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}
#endif
		if((ret=NDK_IccPowerUp(CardNo[i], psAtrbuf, &pnAtrlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}
		memset(recvbuf, 0, sizeof(recvbuf));
		if((ret=NDK_Iccrw(CardNo[i], sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_OK||((sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000&&(sw=SMART_GETSW(recvbuf+rlen-2))!=0x6d00))
		{
			//NDK_IccPowerDown(CardNo[i]);						
			send_result("line %d:%s����ʧ��(%s:%d,0x%04x)", __LINE__, TESTAPI, CardNoStr[i], ret, sw);
			GOTOERR;
		}

		//case1.2:��д���ٴ��ϵ��µ��������
		memset(psAtrbuf, 0, sizeof(psAtrbuf));
		if((ret=NDK_IccPowerUp(CardNo[i], psAtrbuf, &pnAtrlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}		
		if((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}
	}
	while(++i!=(maxid-1));

	//case2:��������
	i=0;
	do
	{
		//case2.1:�ϵ磬���������Ӧʧ�ܷ��ء�
		memset(psAtrbuf, 0, sizeof(psAtrbuf));
		if((ret=NDK_IccPowerUp(CardNo[i], psAtrbuf, &pnAtrlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}
		memset(recvbuf, 0, sizeof(recvbuf));
		if((ret=NDK_Iccrw(CardNo[i], sizeof(sendbuf)-2, sendbuf+2, &rlen, recvbuf))!=NDK_ERR_IOCTL)//NDK_ERR_IOCTL)
		{
			//NDK_IccPowerDown(CardNo[i]);						
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}		
		if((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%s:%d)", __LINE__, TESTAPI, CardNoStr[i], ret);
			GOTOERR;
		}
	}
	while(++i!=(maxid-1));
	
	//����N510��֧��IC��,ֻ֧��sam1��,����������쳣���Ե�20180510 add
#if SAM1_ENABLE
	//case5.1:�쳣��������
	if((ret=NDK_IccPowerUp(ICTYPE_SAM1, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(-1, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_ERR_PARA/*NDK_ERR_OPEN_DEV*/
   	||(ret1=NDK_Iccrw(ICTYPE_SAM1, -1, sendbuf, &rlen, recvbuf))!=NDK_ERR_PARA
   	||(ret2=NDK_Iccrw(ICTYPE_SAM1, sizeof(sendbuf), NULL, &rlen, recvbuf))!=NDK_ERR_PARA
   	||(ret3=NDK_Iccrw(ICTYPE_SAM1, sizeof(sendbuf), sendbuf, NULL, recvbuf))!=NDK_ERR_PARA
   	||(ret4=NDK_Iccrw(ICTYPE_SAM1, sizeof(sendbuf), sendbuf, &rlen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:���󷵻�ֵ����ʧ��(%d,%d,%d,%d,%d)", __LINE__, ret, ret1, ret2, ret3, ret4);
		GOTOERR;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_SAM1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#if IC1_ENABLE
	if(auto_flag==0 || auto_flag==2)
	{
		//case4:IC�����������(ʹ�ö����е�IC)
		if(test_iccard_errcode()!=SUCC)
			GOTOERR;
	
	//case5:�쳣��������
	cls_show_msg("��ȷ��IC�Ѳ���,�����������...");
	}
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(-1, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_ERR_PARA/*NDK_ERR_OPEN_DEV*/
   	||(ret1=NDK_Iccrw(ICTYPE_IC, -1, sendbuf, &rlen, recvbuf))!=NDK_ERR_PARA
   	||(ret2=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), NULL, &rlen, recvbuf))!=NDK_ERR_PARA
   	||(ret3=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, NULL, recvbuf))!=NDK_ERR_PARA
   	||(ret4=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &rlen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:���󷵻�ֵ����ʧ��(%d,%d,%d,%d,%d)", __LINE__, ret, ret1, ret2, ret3, ret4);
		GOTOERR;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
//NDK_IccSetType��NDK_IccGetType��������û�����壬�ӿڻ��Ǳ����������ϵ罻���������ɵ��ú�������ĵ�һ����������
#if 0
	//20130407:����Ŀǰ�������Ϊsettype�����������type�����ϸ�һ�²��ܳɹ�???
	//case6:settype���ϵ硢rw����Բ���(��27�ֳ����г�ȡ������ȷ�ʹ���ĳ������в���)
	//case6.1:����typeΪSAM1������IC���ϵ磬��дӦ�ɹ����ء�
	if((ret=NDK_IccSetType(ICTYPE_SAM1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); // �ָ�ȱʡ����
		GOTOERR;
	}
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_OK||(sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000)
	{
		//NDK_IccPowerDown(CardNo[i]);						
		send_result("line %d:%s����ʧ��(%s:%d,0x%04x)", __LINE__, TESTAPI, CardNoStr[i], ret, sw);
		GOTOERR;
	}

	//case6.2:����typeΪMEMORY������IC��,��SAM���ϵ磬��дӦ�ɹ����ء�
	if((ret=NDK_IccSetType(ICTYPE_M_1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccPowerUp(ICTYPE_SAM1, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_SAM1, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_OK||(sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000)
	{
		//NDK_IccPowerDown(CardNo[i]);						
		send_result("line %d:%s����ʧ��(%s:%d,0x%04x)", __LINE__, TESTAPI, CardNoStr[i], ret, sw);
		GOTOERR;
	}
	if((ret=NDK_IccSetType(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_SAM1, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_OK||(sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000)
	{
		//NDK_IccPowerDown(CardNo[i]);						
		send_result("line %d:%s����ʧ��(%s:%d,0x%04x)", __LINE__, TESTAPI, CardNoStr[i], ret, sw);
		GOTOERR;
	}

	//case6.3:����typeΪIC������IC�ϵ磬��MEM�����ж�д��Ӧʧ�ܷ��ء�
	if((ret=NDK_IccSetType(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_M_1, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_ERR_PARA)
	{
		//NDK_IccPowerDown(CardNo[i]);						
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//case6.4:����typeΪMEM������IC���ϵ磬��MEM�����ж�д��Ӧʧ�ܷ��ء�
	if((ret=NDK_IccSetType(ICTYPE_M_1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	memset(recvbuf, 0, sizeof(recvbuf));
	if((ret=NDK_Iccrw(ICTYPE_M_1, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_ERR_PARA)
	{
		//NDK_IccPowerDown(CardNo[i]);						
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
#endif
if(auto_flag==0 || auto_flag==2)
{
#if !K21_ENABLE||defined ME30||defined ME30S||defined ME30THM||defined ME32||defined ME30GX||defined ME32GX||defined ME32THM||ANDIROD_ENABLE //||defined IM81||defined N900
	//case7:����NDK_Iccrw�������Զ�memory�����ж�д(������ʹ��24C�����ж��������м���֤������memory���������ۺ������н���)
	cls_show_msg("��ȷ���Ѳ��� AT24C032 memory��,���������...");
	if ((ret=NDK_IccPowerUp(ICTYPE_M_1, psAtrbuf, &pnAtrlen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_Iccrw(ICTYPE_M_1, 9,(uchar *)"\x00\xD0\x00\x01\x04\x31\x32\x33\x34",&rlen,recvbuf))!=NDK_OK||((sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000&&(sw=SMART_GETSW(recvbuf+rlen-2))!=0x6d00))			
	{
		send_result("line %d:%s����ʧ��(%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
		GOTOERR;
	}

	if((ret=NDK_Iccrw(ICTYPE_M_1, 5,(uchar *)"\x00\xB0\x00\x01\x04",&rlen,recvbuf))!=NDK_OK||((sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000&&(sw=SMART_GETSW(recvbuf+rlen-2))!=0x6d00))			
	{
		send_result("line %d:%s����ʧ��(%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
		GOTOERR;
	}

	if(memcmp("\x31\x32\x33\x34",recvbuf,rlen-2))
	{
		send_result("line %d:�Ƚ�ʧ��(%s)", __LINE__, recvbuf);
		GOTOERR;
	}

	if((ret=NDK_IccPowerDown(ICTYPE_M_1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	if(cls_show_msg("�Ƿ���в�����EMV��׼������ISO07816��׼������IC������?��:[ȷ��],��:[����]")==ENTER)
	{
		cls_show_msg("�����ֻ������ISO7816������IC��,�����������");
		memset(psAtrbuf, 0, sizeof(psAtrbuf));
		NDK_IccPowerDown(ICTYPE_IC);
#if !K21_ENABLE//K21ƽ̨��Ʒ½����ʼ֧��7816�Ŀ�Ƭ��������û��֧�����ÿ����ͺ��������� �ϵ緽ʽ�޸�Ϊ��3v��5v ��ʽ����ֱ��ʹ��7816��Ƭ
		if((ret=NDK_IccSetType(ICTYPE_ISO7816))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ISO7816:%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif
		if((ret=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &pnAtrlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ISO7816:%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		memset(recvbuf, 0, sizeof(recvbuf));
		if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &rlen, recvbuf))!=NDK_OK||((sw=SMART_GETSW(recvbuf+rlen-2))!=0x9000&&(sw=SMART_GETSW(recvbuf+rlen-2))!=0x6d00))
		{			
			send_result("line %d:%s����ʧ��(ISO7816:%d,0x%04x)", __LINE__, TESTAPI, ret, sw);
			GOTOERR;
		}
		if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ISO7816:%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
	}
	
	//���Ժ���
	cls_show_msg("��ȷ��IC�Ѳ���,���������...");
}
else
{
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
}
#endif
	send_result("%s����ͨ��", TESTAPI);
ERR:
	icpowerdown_all();
	NDK_IccPowerDown(ICTYPE_M_1);
#if !K21_ENABLE
	NDK_IccSetType(ICTYPE_IC);//�ָ�Ĭ��
#endif
	return;
}

