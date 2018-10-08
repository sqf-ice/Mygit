/************************************************************************
*
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		    : alg15.c
* Author 			: zhengry	
* version			:
* DATE				: 20150914
* directory 		:
* description		: ����NDK_AlgSM3Final��������SM3�㷨�Է��鷽ʽע�����һ�����ݺ���������ժҪ��32�ֽڣ��Ĺ��ܣ������㷨��
* related document	: NDK.chm
*
************************************************************************
* log			:
* Revision
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AlgSM3Final"

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
*			  	  	      zhengry           20150914  	    created
*****************************************************************/
void alg15(void)
{
	int ret=0;
	uchar out_buf[32]={0}, out_buf1[33]={0};
	uchar data_buf_8[]="newland!";
	uchar data_buf_129[]="`~!0,@��$#[^%]*(��)'-_=+|&{}:½;?5><.,abcdefghijklmnopqrstuvwxyzdaferjaqbhkjndskanflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpojapowejief.";
	uchar long_buf_2630[]="daferjaqbhkjnd��anflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpo��powejiefad`2112312½3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,1';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557daferjaqbhkjndskanflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpojapowejiefad`21123128u3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,0';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557`~!01@��$#[^%]*(��)'-_=+|&{}:½;?/><.,dlkvnkdmxfkjvnalkjdfpojapowejiefad`21123128u349kanflktoekc8f8a82a567c6df5f500f421335a05aa47a3721887ca0b2acc37a905eb3ee94c03782ff3c0d90987dc875aa23c94094c4712c9b9cf4ee6c40e0d70aa271d24f8fc13fb6461774879141b5e9925e0c664d4706e1a296d8d8c83e6f61fd810c50002c21f36c9ab6d4dd0a1c1def150973ad868f44e1e592286b8f5e144bee9263b3bdc5b642f10777bcd4993829bda479ac89f90aeec76e7671f75079161904a4639725aa0d8b1774ea05d1f0e774b8f51d3f4c0d223c43a913a9b443170bf8ccafde060e525688434a96c5b704ac287d212a7d051130bdd91061cc4a5dd0f30e4c9d139cdb3897633394d324117b894754d1f503bec3f57f32ccb4dfad6060fb13db03df728a01166ddfpo��powejiefad`2112312½3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())nalkjdfpojapowejief.`1ds12213344243155fdgsaer172t4687981uhgaguyt87t782";
	uchar pre_hash[32]={0xd2,0x19,0x41,0xc4,0xe2,0x15,0x44,0x06,
		                0xce,0x55,0x92,0x68,0x5b,0x2f,0x80,0xaa,
		                0x6b,0x10,0x70,0x6b,0x97,0xe6,0x55,0x47,
		                0x2f,0x47,0x42,0x34,0x21,0xfc,0x72,0xa9};//SM3�������data_buf_8���ݵ�ʵ�����ժҪ
	uchar pre_hash1[32]={0x2c,0x53,0x0d,0xc5,0x2c,0xbb,0x31,0xf5,
		                 0x81,0xdd,0xbd,0x78,0x9d,0x59,0xfc,0x96,
		                 0x7f,0x52,0x0b,0x37,0x3f,0x20,0x0b,0x03,
		                 0xd7,0x76,0x35,0xb1,0x33,0xb4,0x94,0x15};//SM3�������data_buf_129���ݵ�ʵ�����ժҪ
	uchar pre_hash2[32]={0x18,0x45,0x1c,0x3c,0x1b,0x39,0x4a,0xd1,
						 0x3c,0xc3,0x36,0xb2,0x2b,0x47,0x99,0x2f,
						 0x52,0x78,0x7e,0x95,0xdb,0xab,0x9c,0x9a,
						 0x4f,0x4e,0xa6,0xc5,0x0f,0xee,0x8e,0x90};//SM3�������long_buf_2630���ݵ�ʵ�����ժҪ
	
	//process body
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	memset(out_buf, 0, sizeof(out_buf));
	memset(out_buf1, 0, sizeof(out_buf1));
	//case1:�쳣����:��δ�������������һ������ Ӧ��ʧ��
	if((ret=NDK_AlgSM3Final(data_buf_8, strlen((char *)data_buf_8), out_buf))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN; 
	}

	//case2:�����쳣����
	if((ret=NDK_AlgSM3Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Final(NULL, strlen((char *)data_buf_8), out_buf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Final(data_buf_8, strlen((char *)data_buf_8), NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:��������: ����Ϊ8�ֽڣ�ֱ�ӵ���finalӦ�ÿ��Գɹ��������ժҪ�빤�߲�����ժҪӦ��һ��,���ժҪΪ32�ֽ�
	if((ret=NDK_AlgSM3Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Final(data_buf_8, strlen((char *)data_buf_8), out_buf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(out_buf1[32]!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if(memcmp(out_buf1, pre_hash, sizeof(pre_hash)))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case4:��������:����Ϊ129�ֽڣ�ֱ�ӵ���finalӦ�ÿ��Գɹ��������ժҪ�빤�߲�����ժҪӦ��һ��
	if((ret=NDK_AlgSM3Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Final(data_buf_129, strlen((char *)data_buf_129), out_buf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(memcmp(out_buf, pre_hash1, sizeof(out_buf)))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case5:��������:����Ϊ2630�ֽڣ����ø��½ӿڸ���1024�ֽں��ٵ��ü������һ�����ݼ���ʣ�������,�õ������ժҪӦ���빤�߲�����һ��
	memset(out_buf, 0, sizeof(out_buf));
	if((ret=NDK_AlgSM3Start())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Update(long_buf_2630, 1024))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM3Final(long_buf_2630+1024, strlen((char *)long_buf_2630)-1024, out_buf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(memcmp(out_buf, pre_hash2, sizeof(out_buf)))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 	
	return;
}
