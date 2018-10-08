/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name		: sec25.c
* Author 		: zhengry
* version		: 
* DATE			:  
* directory 		: ����NDK_SecVppTpInit��ʼ��PIN����Ĵ���������
* description		: ��ȫ����
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>

/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"NDK_SecVppTpInit"
#define PINTIMEOUT (200*1000)
#define CONSTVALUE  0x4321
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: �ṩ��ȫ�ȼ����������ȡ����,��ԿΨһ��,��Կר����,��Կ����ǿ�ȵ���֤
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author			date				remarks
*			  	  	  	zhengry 			20150827	      created
*****************************************************************/
void sec25(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, j = 0, nStatus = 0;
	uchar num[80] = {0}, err_num1[80] = {0}, fun[36] = {0}, err_fun1[36] = {0}, err_fun2[36] = {0}, numserial[11] = {0}, szPinOut[8+1] = {0};
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	int cnt= 10000, bak =0;
#if 1
	int x0 = 0, x1 = 135, x2 = 270, x3 = 405, x4 = 540, y0 = 462, y1 = 568, y2 = 674, y3 = 780, y4 = 886;
#elif defined N910AZ
	int x0 = 0, x1 = 180, x2 = 240, x3 = 360,x4 = 480,x5 = 540, x6 = 720, y0 = 0, y1 = 320, y2 = 640, y3 = 960, y4 = 1280;
#else//������Ҫ��������������ڴ˴�����
		;
#endif
	
	//N900�ֱ��ʱ��ֲ��䲻���ļ�ȥ��ȡ����ֱ��д����  N910����Ҫȥ��ȡ�ļ���ȡ�ֱ�������Ǹ߷ֱ��ʾ͸ĳɲ�һ��������
#if 1//defined N910AZ||defined N700
	int fp = -1;
	char touch_hr[] = {"1280x720"};//�߷ֱ�����1280x720
	char touch_hr_850[] = {"1024x600"};//850�ķֱ����� 1024x600 
	char readbuf[20] = {0};
	if((fp=open("/sys/class/touchscreen/resolution", O_RDONLY))<0)
	{
		cls_show_msg1(2,"line %d: ���ļ�ʧ��(%d)", __LINE__, fp);
		return ;
	}
	memset(readbuf, 0, sizeof(readbuf));
	read(fp,readbuf, sizeof(readbuf));
	//��ȡ�ֱ��ʻ�ȡ���߷ֱ�����ת����������ǵͷֱ��ʲ����ı�
	if(!memcmp(touch_hr,readbuf,4))//ʹ��ǰ�ĸ����Ƚϲ���
	{
#if defined N920
		x0 = 0; x1 = 180; x2 = 360; x3 = 540;x4 = 720;
		y0 =614; y1 = 755; y2 = 896; y3 = 1037; y4 = 1178;
#else
		x0 = 0; x1 = 180; x2 = 360; x3 = 540;x4 = 720;
		y0 = 665; y1 = 818; y2 = 972; y3 = 1126; y4 = 1280;
#endif
	}
	//����N850�ķֱ���
	else if(!memcmp(touch_hr_850,readbuf,4))  //850
	{
		x0 = 0; x1 = 150; x2 = 300; x3 = 450;x4 = 600;
		y0 = 495; y1 = 609; y2 = 723; y3 = 837; y4 = 952;
	}
	else   //900
	{
		x0 = 0; x1 = 135; x2 = 270; x3 = 405;
		x4 = 540;y0 = 462;y1 = 568; y2 = 674; y3 = 780; y4 = 886;
	}
	close(fp);
#endif	

	int numInt[40] = { 
			x0, y0, x1, y1, x1, y0, x2, y1, x2, y0, x3, y1,
	
			x0, y1, x1, y2, x1, y1, x2, y2, x2, y1, x3, y2,
	
			x0, y2, x1, y3, x1, y2, x2, y3, x2, y2, x3, y3,
	
			x1, y3, x2, y4};
	int funInt[18] = { //ѡ����Ĺ��ܼ�Ϊ:ȡ�����˸�ȷ��
			0x1B, 0, x3, y0, x4, y1,
			
			0x0A, 0, x3, y1, x4, y2,
			
			0x0D, 0, x3, y2, x4, y4};

	int numInt1[40] = { 
			x1, y1, x0, y0, x1, y0, x2, y1, x2, y0, x3, y1,
			
			x0, y1, x1, y2, x1, y1, x2, y2, x2, y1, x3, y2,
			
			x0, y2, x1, y3, x1, y2, x2, y3, x2, y2, x3, y3,
			
			x1, y3, x2, y4};
	int funInt1[18] = { 
			0x1B, 0, x4, y1, x3, y0,
			
			0x0A, 0, x3, y1, x4, y2,
			
			0x0D, 0, x3, y2, x4, y4};
			
	int funInt2[18] = { 
			0x1d, 0, x4, y1, x3, y0,
	
			0x0A, 0, x3, y1, x4, y2,
	
			0x0D, 0, x3, y2, x4, y4};

	int funInt3[18] = { //ѡ����Ĺ��ܼ�Ϊ:�˳�����գ�ȷ��
			0x9B, 0,  x3, y0, x4, y1,
	
			0x9C, 0, x3, y1, x4, y2,
	
			0x0D, 0, x3, y2, x4, y4};
			
	int cmpvalue = CONSTVALUE;

	if(auto_flag==1)
	{
		//send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI);
			
	for (i = 0, j = 0; i < SZ_ARRAY(numInt); i++, j++) 
	{
		num[j] = (uchar) (numInt[i] & 0xff);
		j++;
		num[j] = (uchar) ((numInt[i] >> 8) & 0xff);
	}
	for (i = 0, j = 0; i < SZ_ARRAY(funInt); i++, j++) 
	{
		fun[j] = (uchar) (funInt[i] & 0xff);
		j++;
		fun[j] = (uchar) ((funInt[i] >> 8) & 0xff);
	}
	for (i = 0, j = 0; i < SZ_ARRAY(numInt1); i++, j++) 
	{
		err_num1[j] = (uchar) (numInt1[i] & 0xff);
		j++;
		err_num1[j] = (uchar) ((numInt1[i] >> 8) & 0xff);
	}
	for (i = 0, j = 0; i < SZ_ARRAY(funInt1); i++, j++) 
	{
		err_fun1[j] = (uchar) (funInt1[i] & 0xff);
		j++;
		err_fun1[j] = (uchar) ((funInt1[i] >> 8) & 0xff);
	}
	for (i = 0, j = 0; i < SZ_ARRAY(funInt2); i++, j++) 
	{
		err_fun2[j] = (uchar) (funInt2[i] & 0xff);
		j++;
		err_fun2[j] = (uchar) ((funInt2[i] >> 8) & 0xff);
	}
	//��װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��װTPK1(8bytes), ID=2,���İ�װ //Ϊ���ܲ⺣���Ʒ��ĳ�16�ֽڰ�װ 20180809 modify 
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16/*8*/;
	memcpy(stKeyInfo.sDstKeyValue, "\x14\x5F\x5C\x6E\x3D\x91\x44\x57\x14\x5F\x5C\x6E\x3D\x91\x44\x57", stKeyInfo.nDstKeyLen);/*��Ӧ��key����Ϊ16���ֽڵ�0x15*/
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:��������
	if((ret=NDK_SecVppTpInit(NULL, fun, numserial))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecVppTpInit(num, NULL, numserial))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	/*
	if((ret=NDK_SecVppTpInit(num, fun, NULL))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}*/
	//case2.1:�쳣����:���ּ����Ͻǵ��������ڵ������½ǵ������ʱ����Ӧ�÷���ʧ��
	if((ret=NDK_SecVppTpInit(err_num1, fun, numserial))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2.2:�쳣����:���ܼ����Ͻǵ��������ڵ������½ǵ������ʱ����Ӧ�÷���ʧ��
	if((ret=NDK_SecVppTpInit(num, err_fun1, numserial))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2.3:�쳣����:���ܼ���ֵ����ʱ��Ӧ����ʧ��
	if((ret=NDK_SecVppTpInit(num, err_fun2, numserial))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case8:����:��Ե�����������NULL���ܳ�����������쳣(��ջ���Ļ��ܷ�)���������ѹ������,һֱ���ô˺������Ƿ������쳣20171109add
	while(cnt)
	{
		bak++;
		cnt--;
		if((ret=NDK_SecVppTpInit(num, fun, NULL))!=NDK_OK)
		{
			send_result("line %d:%s��%d�β���ʧ��(%d)", __LINE__, TESTAPI, bak, ret);
			return;
		}
	}

	//case3:��������:��ʾ������̣�������󣬴�����Ӧ������ʹ��
	memset(numserial, 0, sizeof(numserial));
	if((ret=NDK_SecVppTpInit(num, fun, numserial))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("��ʱ���������������˳��Ϊ\n%c,%c,%c,ȡ��\n%c,%c,%c,�˸�\n%c,%c,%c,ȷ��\n  %c\n", numserial[0], numserial[1], numserial[2], numserial[3], numserial[4], numserial[5], numserial[6], numserial[7], numserial[8], numserial[9]);
	NDK_ScrPrintf("�뾡������4321���˸���޸ĳ�1234��ȷ��...");
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, szPinOut, SEC_PIN_ISO9564_2, PINTIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\xa8\x89\xc1\xdf\x10\xc8\xb0\x47", 8)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}
	if(cls_show_msg("��ʱ��׿�˵Ļ���Ӧ��������ʹ��...ͨ��[ENTER]����ͨ��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��������:��ʾ�̶�����ļ��̣����ҹ��ܼ�ѡ��ȡ�� �˸񣬲���ȡ�������ܺ��������룬������󣬴�����Ӧ������ʹ��
	if((ret=NDK_SecVppTpInit(num, fun, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("��ʱ�����������ּ��̵�˳��Ϊ\n0,1,2,ȡ��\n3,4,5,�˸�\n6,7,8, ȷ��\n  9\n");
	NDK_ScrPrintf("�뾡������4321����1��ȡ�����޸ĳ�1234��ȷ��...");
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, szPinOut, SEC_PIN_ISO9564_2, PINTIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\xa8\x89\xc1\xdf\x10\xc8\xb0\x47", 8)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	} 
	if(cls_show_msg("��ʱ��׿�˵Ļ���Ӧ��������ʹ��...ͨ��[ENTER]����ͨ��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:�޸Ĺ��ܼ�ʹ���˳�����գ�ȷ�ϣ������˳����ܼ�
	for (i = 0, j = 0; i < SZ_ARRAY(funInt3); i++, j++) 
	{
		fun[j] = (uchar) (funInt3[i] & 0xff);
		j++;
		fun[j] = (uchar) ((funInt3[i] >> 8) & 0xff);
	}
	memset(numserial, 0, sizeof(numserial));
	if((ret=NDK_SecVppTpInit(num, fun, numserial))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("��ʱ���������������˳��Ϊ\n%c,%c,%c,�˳�\n%c,%c,%c,���\n%c,%c,%cȷ��\n  %c\n", numserial[0], numserial[1], numserial[2], numserial[3], numserial[4], numserial[5], numserial[6], numserial[7], numserial[8], numserial[9]);
	NDK_ScrPrintf("�뾡������1234���˳���");
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, szPinOut, SEC_PIN_ISO9564_2, PINTIMEOUT))!=NDK_ERR_SECVP_GET_ESC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(cls_show_msg("��ʱ��׿�˵Ļ���Ӧ��������ʹ��...ͨ��[ENTER]����ͨ��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:�޸Ĺ��ܼ�ʹ���˳�����գ�ȷ�ϣ�������չ��ܼ�
	if((ret=NDK_SecVppTpInit(num, fun, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("��ʱ�����������ּ��̵�˳��Ϊ\n0,1,2,�˳�\n3,4,5,���\n6,7,8,ȷ��\n  9\n");
	NDK_ScrPrintf("�뾡������4321������պ��޸ĳ�1234��ȷ��...");
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, szPinOut, SEC_PIN_ISO9564_2, PINTIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(memcmp(szPinOut, "\xa8\x89\xc1\xdf\x10\xc8\xb0\x47", 8)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	} 
	if(cls_show_msg("����պ�����������뱻ȫ����գ��Ҵ�ʱ��׿�˵Ļ���Ӧ��������ʹ��...ͨ��[ENTER]����ͨ��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:���Թ��ܼ��ķ���״̬
	if((ret=NDK_SecVppTpInit(num, fun, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("��ʱ�����������ּ��̵�˳��Ϊ\n0,1,2,�˳�\n3,4,5,���\n6,7,8,ȷ��\n  9\n");
	NDK_ScrPrintf("�ɶ���������ֺ���ռ���Ӧ�ܻ�ȡ����ռ�,���˳������˳�\n");
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, NULL, SEC_PIN_ISO9564_2, PINTIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	do 
	{
		ret=NDK_SecGetPinResult(szPinOut, &nStatus);
		if (ret != 0) 
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if (nStatus == SEC_VPP_KEY_CLEAR) 
		{
			NDK_ScrPrintf("��ȡ����ռ�,����������ͨ��\n");
		}
		if (nStatus == SEC_VPP_KEY_ESC) 
		{
			cls_printf("��ȡ���˳���,����������ͨ��");
			break;
		}
		if (nStatus == SEC_VPP_KEY_PIN || nStatus == SEC_VPP_KEY_BACKSPACE ||  nStatus == SEC_VPP_KEY_NULL) 
		{   
			//���ڱ������ڴ���pinblockʱ��������״̬�����������������������
			//����ѭ���ȴ�
			//����ȡPIN������Ҫ����������ʹûֵҲҪ����Ӧ�ó������ж��Ƿ��ж��˳������������SEC_VPP_NULL ����ֵ
		}
		NDK_SysMsDelay(10);
	} while(1);

	//������ֵ�Ƚ�,���ڴ��Ƿ�ı�20171109 add 
	if(cmpvalue != CONSTVALUE)
	{
		send_result("line %d:�ڴ�ֵ�Ƚ�ʧ��(%d)", __LINE__, TESTAPI, cmpvalue);
		goto ERR;
	}
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


