/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: 
* Author 			: chenfm
* version			: 
* DATE			: 20130104
* directory 		: 
* description		: fsģ�������
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"�ſ�ģ��"
#define  MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
#if !K21_ENABLE||ANDIROD_ENABLE     //�и߶˲�Ʒ�쳣������ж�||defined IM81||defined N900
static void check_stripe_errcode(int cardno, char *track2, char *track3, int *perrcard, int *err)
{
	/*private & local definition*/
	char track[MAXTRACKLEN]={0};

	/*process body*/
	//����Ԥ�����Ǹ��ݡ��ſ����԰���.doc��
	//ʵ�ʲ�����,�����Զ�����(����һ��),��Ϊerrcode���ĵ�����һ����

	switch(cardno)
	{
  	case 1://����1#��        1�ſ� ����ʼ��־�ͽ�����־��ȷ��LRC �������ݣ� ������͵ײ��жϵ�������Ϊ�� ��ֱ�ӷ���7f����LRC��Ϊ����
		if(((track2[0]!=0x7e)&&(track2[0]!=0x7f))||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((track3[0]!=0x7e)&&(track3[0]!=0x7f))||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//��������������λ,������ˢ����,��ˢ��ȷʱ,����˴���λ
		//*perrcard &= ~(1<<(cardno-1));
		break;
  	case 2://����2#��
  	case 4://����4#��
  	case 5://����5#��
		if(track2[0]!=0x7f||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))//������
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x", __FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(track3[0]!=0x7f||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))//������
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x,err=0x%02x", __FUNCTION__, __LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 3://����3#��
  	case 6://����6#��
  	case 7://����7#��
#if 0
		if(cardno==6)
		{
			cls_show_msg("GetErrorCode = %x\n",*err);
			cls_show_msg("track2(%d):%s\n",strlen(track2),track2);
			cls_show_msg("track3(%d):%s\n",strlen(track3),track3);
		}
#endif
		if(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR)	//LRCУ���
		{
			memset(track, '2', sizeof(track));
			track[37]=0;
			if(strcmp(track2, track))
			{
				cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
				*perrcard|=(1<<(cardno-1));
			}
		}
		else
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR)	//LRCУ���
		{
			memset(track, '3', sizeof(track));
			track[104]=0;
			if(strcmp(track3, track))
			{
				cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
				*perrcard|=(1<<(cardno-1+16));
			}
		}
		else
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 8://����8#��
		memset(track, '1', sizeof(track));
		track[37]=0;
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((track3[0]!=0x7e)&&(track3[0]!=0x7f))||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x, 0x%02x",  __FUNCTION__,__LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 9://����9#��
		if(((track2[0]!=0x7e)&&(track2[0]!=0x7f))||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x",  __FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1));
		}
		memset(track, '2', sizeof(track));
		track[89]=0;
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 10://����10#��
		memset(track, '1', sizeof(track));
		track[32]=0;
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		memset(track, '2', sizeof(track));
		track[100]=0;
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 11://����11#��
		memset(track, '1', sizeof(track));
		track[37]=0;
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		memset(track, '2', sizeof(track));
		track[104]=0;
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 12://����12#��
		if(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR)	//LRCУ���
		{
			memset(track, '1', sizeof(track));
			track[30]=';';
			track[31]=0;
			if(strcmp(track2, track))
			{
				cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
				*perrcard|=(1<<(cardno-1));
			}
		}
		else
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR)	//LRCУ���
		{
			memset(track, '2', sizeof(track));
			track[100]=';';
			track[101]=0;
			if(strcmp(track3, track))
			{
				cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
				*perrcard|=(1<<(cardno-1+16));
			}
		}
		else
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 13://����13#�� ���ݵ���żУ������LRCУ���
		if(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR)	//LRCУ���
		{
			memset(track, '1', sizeof(track));
			track[19]=0;
			if(strcmp(track2, track))
			{
				cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
				*perrcard|=(1<<(cardno-1));
			}
		}
		else
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR)	//LRCУ���
		{
			memset(track, '2', sizeof(track));
			track[29]=0;
			if(strcmp(track3, track))
			{
				cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
				*perrcard|=(1<<(cardno-1+16));
			}
		}
		else
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 14://����14#��   2��LRCУ����ȷ��3��LRCУ�����
		strcpy(track, "1234567890123456789012345678901233");
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		strcpy(track, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567892");
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 15://����15#��
		strcpy(track, "1234567890123456789012345678901234567");
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))//if(!memcmp(track2, track, 37))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		strcpy(track, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234");
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR))//if(!memcmp(track3, track, 104))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	default:
		cls_show_msg1(2, "��֧�ֶ�%d#�ſ��Ĳ���!", cardno);
		return;
		break;
	}

	return;
}
#else
static void check_stripe_errcode(int cardno, char *track2, char *track3, int *perrcard, int *err)
{
	/*private & local definition*/
	char track[MAXTRACKLEN]={0};

	/*process body*/
	//����Ԥ�����Ǹ��ݡ��ſ����԰���.doc��
	//ʵ�ʲ�����,�����Զ�����(����һ��),��Ϊerrcode���ĵ�����һ����

	switch(cardno)
	{
  	case 1://����1#��
		if(((track2[0]!=0x7e)&&(track2[0]!=0x7f))||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((track3[0]!=0x7e)&&(track3[0]!=0x7f))||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//��������������λ,������ˢ����,��ˢ��ȷʱ,����˴���λ
		//*perrcard &= ~(1<<(cardno-1));
		break;
  	case 2://����2#��
  	case 4://����4#��
  	case 5://����5#��
  	
  	case 3://����3#��
  	case 6://����6#��
  	case 7://����7#��
  	
	case 12://����12#��	
	case 13://����13#��
		if(track2[0]!=0x7f||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		if(track3[0]!=0x7f||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		break;
  	
  	case 8://����8#��
		//memset(track, '1', sizeof(track));
		//track[37]=0;
		if(track2[0]!=0x7f||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1));
		}
		if(((track3[0]!=0x7e)&&(track3[0]!=0x7f))||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 9://����9#��
		if(((track2[0]!=0x7e)&&(track2[0]!=0x7f))||(((*err)&MAGCARD_TK2_LRC_ERR)!=MAGCARD_TK2_LRC_ERR))//������ //20140822 linwl added ����ײ㷵��7f���ײ��޷�����7E 7F
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�track2[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track2[0], *err);
			*perrcard|=(1<<(cardno-1));
		}
		//memset(track, '2', sizeof(track));
		//track[89]=0;
		if(track3[0]!=0x7f||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�track3[0]=0x%02x,err=0x%02x",__FUNCTION__, __LINE__, cardno,track3[0], *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 10://����10#��
		memset(track, '1', sizeof(track));
		track[32]=0;
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		memset(track, '2', sizeof(track));
		track[100]=0;
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 11://����11#��
		memset(track, '1', sizeof(track));
		track[37]=0;
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		memset(track, '2', sizeof(track));
		track[104]=0;
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 14://����14#��
		strcpy(track, "1234567890123456789012345678901233");
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		strcpy(track, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567892");
		if(track3[0]!=0x7f||(((*err)&MAGCARD_TK3_LRC_ERR)!=MAGCARD_TK3_LRC_ERR))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	case 15://����15#��
		strcpy(track, "1234567890123456789012345678901234567");
		if(strcmp(track2, track)||(((*err)&MAGCARD_TK2_LRC_ERR)==MAGCARD_TK2_LRC_ERR))//if(!memcmp(track2, track, 37))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���2�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1));
		}
		strcpy(track, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234");
		if(strcmp(track3, track)||(((*err)&MAGCARD_TK3_LRC_ERR)==MAGCARD_TK3_LRC_ERR))//if(!memcmp(track3, track, 104))
		{
			cls_show_msg1(3, "%s,line %d:��%d�ſ���3�ŵ��д�(0x%02x)", __FUNCTION__, __LINE__, cardno, *err);
			*perrcard|=(1<<(cardno-1+16));
		}
		//*perrcard &=  ~(1<<(cardno-1));
		break;
  	default:
		cls_show_msg1(2, "��֧�ֶ�%d#�ſ��Ĳ���!", cardno);
		return;
		break;
	}

	return;
}

#endif

#define	MAXCARDCNT			(15)
//����15�������ſ���ר�����
int test_mag_errcode(int *perrcard1, int *perrcard2, int *perrcard3, int *perrcard4)
{
	/*private & local definition*/
	char track1[MAXTRACKLEN]={0}, track2[MAXTRACKLEN]={0}, track3[MAXTRACKLEN]={0}, cardnostr[4]={0};
	int ret=SUCC, mode=0, cardno=0, err=0;
	uint unLen=0;
	int *intptr[4];
#if SCR_DISABLE
	char pMsg[64]={0};
#endif
	/*process body*/
	*perrcard1= *perrcard2= *perrcard3= *perrcard4=0;
	intptr[0]=perrcard1;intptr[1]=perrcard2;intptr[2]=perrcard3;intptr[3]=perrcard4;
	while(1)
	{
		if(ESC==cls_show_msg1(MAXWAITTIME, "��׼��%d�������ſ�,�������дſ�ErrCode����,ESC�˳����������", MAXCARDCNT))
			return FAIL;

		if(++cardno>MAXCARDCNT)
			cardno=1;

#if ANDIROD_ENABLE//����IM81ʹ�û�������,NDK_KbGetInput���������Ѵ��ڵ��ַ��޷���ʾ�ڴ�����,���Խ���������պ���ʾҪ���ԵĿ���defined IM81||defined N900||
		memset(cardnostr, 0, sizeof(cardnostr));
		cls_printf("ÿ�ſ�����4��ģʽ���в���.�����������ſ���%d", cardno);
#else
		sprintf(cardnostr, "%d", cardno);
		cls_printf("ÿ�ſ�����4��ģʽ���в���.�����������ſ���.��ǰ�ſ���:");
#endif	
		if((ret=lib_kbgetinput(cardnostr, 0, sizeof(cardnostr)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			continue;
		else
			cardno=atoi(cardnostr);

		if(cardno<1||cardno>MAXCARDCNT)
		{
			cardno=0;
			continue;
		}

		mode=0;//��λ
		while(mode<4)
		{
#if SCR_DISABLE	//�ض��򵽴���
			sprintf(pMsg,"��%s��%sˢ%d��������(%02d��)��\n",(mode/2)?"��":"��", (mode%2)?"��":"��",cardno, MAXWAITTIME);
#if 0//ANDIROD_ENABLE
			NDK_AZPortWrite(g_com, strlen(pMsg), pMsg);
#else
			NDK_PortWrite(g_com, strlen(pMsg), pMsg);
#endif
#else
#if ANDIROD_ENABLE
			cls_printf("��%s��%sˢ%d��������(%02d��)��\n",(mode/2)?"��":"��", (mode%2)?"��":"��",cardno, MAXWAITTIME);
#else
			cls_printf("��  ��  ˢ%d��������(%02d��)��\n", cardno, MAXWAITTIME);
			GUI_DispStringEx(12, 0, (mode/2)?"��":"��", TEXT_ATTRIBUTE_REVERSE);
			GUI_DispStringEx(36, 0, (mode%2)?"��":"��", TEXT_ATTRIBUTE_REVERSE);
#endif
#endif
			ret=readcard3(TK1_2_3, MAXWAITTIME, track2, track3, track1, &err);

			switch(ret)
			{
			case NDK_ERR_TIMEOUT:
				continue;
			case STRIPE:
				check_stripe_errcode(cardno, track2, track3, intptr[mode], &err);
				mode++;
				break;
			case NDK_ERR_QUIT://ESC
			default:
				cls_show_msg("line %d:��%d��������ʧ��(%d)", __LINE__, cardno, ret);
				*intptr[mode]|=(1<<(cardno-1));
				*intptr[mode]|=(1<<(cardno-1+16));
				mode++;
				break;
			}
		}
	}

	if(*perrcard1==0&&*perrcard2==0&&*perrcard3==0&&*perrcard4==0)
		return SUCC;
	else
		return FAIL;
}

/****************************************************************
* function name 	 	: mag
* functional description 	: magģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  chenfm         20130104	created
*				  	
*****************************************************************/
void mag(PF *list, unsigned char id[])
{
	/*private & local definition*/

	/*process body*/
	if(auto_flag==1)//��֧���Զ�����
	{
		g_UCID = 0;
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTMODULE);
		return;
	}
	cls_show_msg1(1, "%s����...", TESTMODULE);
	process(list, id);
	return;
}

