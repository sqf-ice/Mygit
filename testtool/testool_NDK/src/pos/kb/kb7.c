/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name			: 7.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			:����NDK_PS2GetCode��ʱʱ���ڶ�ȡPS2���̰���ֵ��NDK_PS2Hit��������ȡPS2���̼�ֵ
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PS2GetCode,NDK_PS2Hit"
#define  MAXWAITTIME 30
#define  WUCHASEC 0.03//0.03 //���ֵ����
#define	 MAXTIMEOUT	10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: kb7.c
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*							sull			20180522		created
*
*****************************************************************/
void kb7(void)
{
	/*private & local definition*/
	int ret = 0, pnCode = -1, key = -1, cnt = 0;
	time_t oldtime = 0, diff = 0;
	char str[32] = {0};
	float tmp = 0;

#include "kbtable.h"
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	cls_show_msg("�뽫POS����PS2�������������������������");

	//����ǰ��:ʹ��PS2����
	if((ret = NDK_PS2Start()) != NDK_OK)
	 {
		 send_result("line %d:%sʹ��PS2����ʧ��(%d)", __LINE__, TESTAPI,ret);
		 return;
	 }

	//case1:pnCodeΪNULL
	if((ret = NDK_PS2Hit(NULL)) != NDK_OK)
	 {
		 send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		 GOTOERR;
	 }
		 
	 //pnCodeΪNULLʱ,���سɹ�,��Ϊֻ���ڵȴ�ʱ�䰴��,��û��Ҫ��ȡ��ǰ�İ�������
	 if ((ret=NDK_PS2GetCode(2,NULL))!=NDK_OK)
	 {
		 send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,ret);
		 GOTOERR;
	 }	 
	 
	 //case2:����NDK_PS2GetCode(30)������������ֲ���
	 cnt = rand()%(SZ_ARRAY(ps2_num));
	 cls_printf("����%ds�ڰ��¼�����ߵ�%s��,NDK_PS2GetCode(30)��������\n", MAXWAITTIME, ps2_num[cnt].keyname);
	 if((ret = NDK_PS2GetCode(30,&key)) != NDK_OK)
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d)", __LINE__, ret);
		 GOTOERR;
	 }
	if(key!= ps2_num[cnt].keycode)
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(0x%02x)", __LINE__, key);
		 GOTOERR;
	}
	cls_show_msg1(1,"������1����ͨ��");
		
	 //case3:����NDK_PS2GetCode(0)������F ������
	 cnt = rand()%(SZ_ARRAY(ps2_f));
	 cls_printf("�밴��%s��,NDK_PS2GetCode(0)��������\n", ps2_f[cnt].keyname);
	 if((ret = NDK_PS2GetCode(0, &key)) != NDK_OK)
	  {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d)", __LINE__, ret);
		 GOTOERR;
	 }
	 if (key!=ps2_f[cnt].keycode)
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(0x%02x)", __LINE__, key);
		 GOTOERR;
	 }
	 cls_show_msg1(1,"������2����ͨ��");
	 
	 //case4:��ʱ����
	 cls_printf("����%ds�ڲ������κμ�,�����ĵȴ�...", MAXTIMEOUT);
	 lib_StartStopwatch();
	 if((ret = NDK_PS2GetCode(MAXTIMEOUT, &key)) != NDK_OK)
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d)", __LINE__, ret);
		 GOTOERR;
	 }
	 if (key!=0||(tmp=fabs(lib_ReadStopwatch()-MAXTIMEOUT))>WUCHASEC)
	 {
		 memset(str,0,sizeof(str));
		 ftos(tmp, str);
		 send_result("line %d: %s����ʧ��(0x%02x,%s)", __LINE__, TESTAPI, key, str);
		 GOTOERR;
	 }

	 //case5:����NDK_PS2Hitû�м�������������0
	 if(((ret = NDK_PS2Hit(&pnCode)) != NDK_OK) || (pnCode != 0))
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d,0x%02x)", __LINE__, ret, pnCode);
		 GOTOERR;
	 }

	 //case6:�����а�����NDK_PS2Hit��ȷ���񰴼���Сд��ĸ����
	 cnt = rand()%(SZ_ARRAY(ps2_small_letter));
	 cls_printf("����%ds�ڰ���%s��,NDK_PS2Hit��������\n", MAXWAITTIME, ps2_small_letter[cnt].keyname);
	 oldtime = time(NULL);
	 while(1)
	 {
	 	if((time(NULL)-oldtime) >= MAXWAITTIME)
	 	{
	 		send_result("line %d:�û�30s��δ��������ʱ�˳�", __LINE__);
	 		GOTOERR;
			break;
	 	}
		 NDK_PS2Hit(&pnCode);
		 if(pnCode==0)
			continue;
		 else if(pnCode!=ps2_small_letter[cnt].keycode)
		 {
			send_result("line %d: NDK_PS2GetCode����ʧ��(0x%02x)", __LINE__, pnCode);
		 	GOTOERR;
			break;
		 }
		 else
			break;	
	 }
	 cls_show_msg1(1,"������3����ͨ��");

	 //case7:���Ա����Ų���
	 cnt = rand()%(SZ_ARRAY(ps2_char));
	 cls_printf("����%ds�ڰ��¼�����ߵ�%s��,NDK_PS2GetCode(30)�������������ڼ����Ϸ��ģ��Ȱ�shift�����ٰ���Ӧ����\n", MAXWAITTIME, ps2_char[cnt].keyname);
	 if(((ret = NDK_PS2GetCode(30,&key)) != NDK_OK) ||(key!= ps2_char[cnt].keycode))
	 {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d,0x%02x)", __LINE__, ret, key);
		 GOTOERR;
	 }
	 cls_show_msg1(1,"������4����ͨ��");

	//case8:���Դ�д��ĸ
	 cnt = rand()%(SZ_ARRAY(ps2_big_letter));
	 cls_printf("����%ds�ڰ���%s��,NDK_PS2Hit��������[�Ȱ�shift�ٰ���ĸ��]\n", MAXWAITTIME, ps2_big_letter[cnt].keyname);
	 oldtime = time(NULL);
	 while(1)
	 {
	 	if((time(NULL)-oldtime) >= MAXWAITTIME)
	 	{
	 		send_result("line %d:�û�30s��δ��������ʱ�˳�, __LINE__");
	 		GOTOERR;
			break;
	 	}
		NDK_PS2Hit(&pnCode);
		if(pnCode==0)
			continue;
		else if(pnCode!=ps2_big_letter[cnt].keycode)
		{
			send_result("line %d: NDK_PS2GetCode����ʧ��(0x%02x)", __LINE__, pnCode);
			GOTOERR;
			break;
		}
		else
			break;	
	 }
	 cls_show_msg1(1,"������5����ͨ��");

	 //case9:�����ұ߼��̲���
	 cnt = rand()%(SZ_ARRAY(ps2_right));
	 cls_printf("�밴���ұߵ�%s��,NDK_PS2GetCode(0)��������\n", ps2_right[cnt].keyname);
	 if(((ret = NDK_PS2GetCode(0, &key)) != NDK_OK) || (key!=ps2_right[cnt].keycode))
	  {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d,0x%02x)", __LINE__, ret, key);
		 GOTOERR;
	 }
	 cls_show_msg1(1,"������6����ͨ��");
 	
	 //case10:���԰���Num Lock֮���ұ�С���̰���Ӧ��Ч  //�ײ�δʵ�������÷�����Ӧ������ʵ��
	
	//case11:�������ⰴ������
	 cnt = rand()%(SZ_ARRAY(ps2_other));
	 cls_printf("�밴��%s��,NDK_PS2GetCode(0)��������\n", ps2_other[cnt].keyname);
	 if(((ret = NDK_PS2GetCode(0, &key)) != NDK_OK) || (key!=ps2_other[cnt].keycode))
	  {
		 send_result("line %d: NDK_PS2GetCode����ʧ��(%d,0x%02x)", __LINE__, ret, key);
		 GOTOERR;
	 }

	 //���Ժ���:�ر�PS2����
	 if((ret = NDK_PS2Stop()) != NDK_OK)
	  {
		 send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		 goto ERR;
	 }	

	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_PS2Stop();
	return;
}



