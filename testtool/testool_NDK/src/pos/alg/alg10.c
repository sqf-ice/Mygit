/************************************************************************
*
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		    : alg10.c
* Author 			: zhengry	
* version			:
* DATE				: 20150907
* directory 		:
* description		: ����NDK_AlgSM2Decrypt����ʹ��SM2�ǶԳƹ�˽Կ���е�˽Կ���н��ܵĹ��ܣ������㷨��
* related document	: NDK.chm
*
************************************************************************
* log			:
* Revision
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#define     MES_LEN    1024-96

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AlgSM2Decrypt"

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
*			  	  	      zhengry           20150907  	    created
*****************************************************************/
void alg10(void)
{
	int ret=0;
	ushort len=0, cryp_len=0;
	uchar pubkey[64]={0}, prikey[32]={0}, cryp[MES_LEN+96]={0}, message[MES_LEN]={0};
	uchar real_message[]="newland";
	uchar max_message_928[]="daferjaqbhkjnd��anflktoek4ngklnfdlkvnkdmxfkjvnalkjdfpo��powejiefad`2112312½3498324y58y098u-45irosdakjniuhoidso!!@@#$*(Y489DAHI())u&*^&^&%&^$%%#$%~!@#{}{}:>>>?DANJGEURJHDJHFAGEIUGRJHBJKBZJEIURjaoidfknfnejwhioufahoijpofjsdpoi[-o2=i40u12098uy498ytuhrfjgjcxzh98er0-320u5r0989sduifhajbdfnbzmn,mmlkpi-0i-2u!!~$%$&**)!@%#)$#:,^#dfaheruhaiejgrjgrgergfbgefraddfadsaerwtrtytugfvhnbvczxwaeq5rtyuiukbvcadfyr86798776543242fvdhgjiytdfgvqewwriuyjhgbvfqdertyuiuuhgew234567~!@#$%^&*()_+{}[]:?|<>?/.,1';]puguhaepohnjdmyujolyhdafyhbjkwejhiugfghjvnm,.m,cvcvxbtrrteryuuoughjh68uidfcjkllhgjkjl9o87656787907yhiunjkanfjkfadsfffffffffffffffffffffffeeeerrrrwtttttqertyuiytrewrtyuiouyterwetyuppoiuyterwrfdszfvbah2981y58u4tiiiiiii36-o=-op[sdflknvkkpoigfgsrdtyuiyuopoo234789-0876546tyughbjkv@$&*()(&^%&*(YTUHGJKERUTrYTTYIOGo**(&^%*(&&)&hgfgjhghjhklkhgy&i^t^%^*()_ikljh)_lk*/*-++kjljhgdfbmvnmiourtuiop)(**&^$%^*()*&tugjkjhgcvjbknklYUTIYDOUPAFUIHERYTWT106557";
	uchar err_prikey[32]={0xeb,0xa0,0xb5,0xe3,0x02,0x55,0x20,0x22,
						  0xf6,0x96,0x36,0x1d,0x3b,0x6e,0xb2,0xe5,
						  0x07,0x49,0x09,0x6b,0x0e,0xf2,0x52,0x25,
						  0xad,0xfc,0x3e,0x59,0x37,0x1a,0x00,0x00};//����ȷ��˽Կֵ�޸������һ���ֽ�
	uchar err_cryp[112]={0xe0, 0xe6, 0xd4, 0xe6, 0xaf, 0x15, 0xe7, 0xf0, 
						 0x33, 0xf1, 0xff, 0xe2, 0x62, 0x45, 0xbf, 0xb3,
						 0xcd, 0xd2, 0xe7, 0x95, 0xc8, 0xf8, 0x3a, 0x5f,
						 0x8d, 0xa0, 0x83, 0xf5, 0xc6, 0x28, 0xc4, 0x2f,
						 0xc8, 0xdd, 0x80, 0x89, 0x69, 0x75, 0x46, 0xb8,
						 0x1e, 0x38, 0x9d, 0xbf, 0xcb, 0x5b, 0xa8, 0xc4,
						 0xee, 0xa5, 0xa9, 0x47, 0xdd, 0xc2, 0x13, 0x24,
						 0xee, 0xbe, 0x64, 0xff, 0x35, 0xf3, 0x67, 0x53,
						 0x53, 0xc3, 0x17, 0x85, 0x0b, 0xf9, 0xe3, 0x11,
						 0x09, 0xac, 0x8e, 0xac, 0x64, 0x30, 0xff, 0x16,
						 0x1e, 0x31, 0x09, 0x00, 0x85, 0x80, 0xf0, 0x17,
						 0xe4, 0xfc, 0x31, 0xd9, 0x38, 0x12, 0xd6, 0xcc,
						 0x42, 0x63, 0xfe, 0xdf, 0xc6, 0xa4, 0x02, 0x57,
						 0xe7, 0x22, 0x9e, 0xb5, 0x67, 0x18, 0x56, 0x00};//����������Ԥ��������޸������һ���ֽ�
	uchar pre_message[16]={0x66, 0x1c, 0x7f, 0x12, 0x57, 0xb7, 0x54, 0xc4,
						   0x33, 0x8a, 0xca, 0x80, 0x21, 0xcd, 0xa4, 0xcc};//��������Ԥ����ܳ���������
	uchar pre_prikey[32]={0xeb,0xa0,0xb5,0xe3,0x02,0x55,0x20,0x22,
						  0xf6,0x96,0x36,0x1d,0x3b,0x6e,0xb2,0xe5,
						  0x07,0x49,0x09,0x6b,0x0e,0xf2,0x52,0x25,
						  0xad,0xfc,0x3e,0x59,0x37,0x1a,0x00,0x5e};//�����ݺ��������Խ���Ԥ���˽Կ
	uchar pre_cryp[112]={0xe0, 0xe6, 0xd4, 0xe6, 0xaf, 0x15, 0xe7, 0xf0, 
						 0x33, 0xf1, 0xff, 0xe2, 0x62, 0x45, 0xbf, 0xb3,
						 0xcd, 0xd2, 0xe7, 0x95, 0xc8, 0xf8, 0x3a, 0x5f,
						 0x8d, 0xa0, 0x83, 0xf5, 0xc6, 0x28, 0xc4, 0x2f,
						 0xc8, 0xdd, 0x80, 0x89, 0x69, 0x75, 0x46, 0xb8,
						 0x1e, 0x38, 0x9d, 0xbf, 0xcb, 0x5b, 0xa8, 0xc4,
						 0xee, 0xa5, 0xa9, 0x47, 0xdd, 0xc2, 0x13, 0x24,
						 0xee, 0xbe, 0x64, 0xff, 0x35, 0xf3, 0x67, 0x53,
						 0x53, 0xc3, 0x17, 0x85, 0x0b, 0xf9, 0xe3, 0x11,
						 0x09, 0xac, 0x8e, 0xac, 0x64, 0x30, 0xff, 0x16,
						 0x1e, 0x31, 0x09, 0x00, 0x85, 0x80, 0xf0, 0x17,
						 0xe4, 0xfc, 0x31, 0xd9, 0x38, 0x12, 0xd6, 0xcc,
						 0x42, 0x63, 0xfe, 0xdf, 0xc6, 0xa4, 0x02, 0x57,
						 0xe7, 0x22, 0x9e, 0xb5, 0x67, 0x18, 0x56, 0x59};//��������Ԥ�������						  
	uchar max_cryp[1024]={0x52,0xcb,0x53,0x00,0x89,0x66,0x4d,0x47,0xde,0xf7,0x4e,0x7a,0x6b,0x29,0x0f,0x00,0x82,0xcb,0x25,0x0d,0x49,0x2d,0x73,0x2c,0x13,0x46,0x9d,0xb6,0x95,0xdd,0x5a,0x78,
						  0x75,0x4a,0x02,0x2c,0x26,0xa3,0xae,0xa5,0x0d,0x64,0x95,0x6c,0x34,0x64,0x4c,0x65,0x10,0x42,0xd4,0x18,0x2e,0x6f,0xab,0xec,0xf2,0x1d,0x7a,0x78,0x5a,0x73,0x87,0x92,
						  0x07,0x9d,0xfe,0xe5,0xf0,0x13,0x94,0xe7,0x45,0x96,0x4f,0x36,0x8a,0x1f,0x57,0x00,0x3f,0xef,0x69,0x8c,0x57,0x2d,0x73,0x84,0xca,0xf5,0x12,0x1d,0x68,0x11,0x81,0xc6,
						  0x14,0x48,0x33,0xb2,0xc6,0xa1,0x73,0x03,0xa4,0xf6,0x5b,0x1e,0x42,0x5e,0x97,0xac,0xc4,0xa0,0x74,0xab,0xaf,0x84,0x8b,0x3d,0xa9,0xec,0x9d,0x90,0x82,0x8c,0x41,0x2f,
						  0xc8,0x3f,0xc7,0xeb,0x58,0xda,0x90,0x85,0x50,0xd5,0x23,0x9e,0x3b,0xcc,0xc0,0xc7,0x7a,0x50,0x9c,0x05,0xed,0xe1,0x56,0x91,0x92,0x1c,0xac,0x2b,0x56,0x75,0x2e,0x61,
						  0x3c,0x3c,0x72,0x8c,0x77,0xd7,0x47,0x32,0x87,0x56,0x1b,0xed,0x32,0x09,0xd3,0x3b,0x33,0x35,0x80,0x37,0xcf,0x6b,0x31,0xfa,0x09,0xbf,0x96,0x21,0x50,0xf7,0xab,0x08,
						  0x8a,0xe2,0x91,0x0c,0x6f,0xb0,0x83,0xfb,0x07,0x12,0xa3,0x99,0xde,0xe8,0x52,0x30,0x6d,0x07,0xb9,0x61,0x03,0x5c,0xd0,0x99,0xe0,0xe2,0xc0,0x13,0x8f,0x8c,0x34,0x17,
						  0xf2,0x4a,0x2d,0xeb,0x58,0x33,0x3b,0xb2,0x88,0x57,0xbf,0x65,0x22,0x4c,0xf9,0x29,0x23,0xe9,0x4a,0x3c,0x66,0x28,0xfb,0xd4,0xb3,0xe4,0x66,0xe3,0x1e,0x74,0xf4,0x87,
						  0x91,0x31,0xf2,0x43,0x73,0x0e,0x2f,0x17,0xae,0xfb,0x23,0x01,0x94,0x49,0xb0,0x8c,0xf2,0xf3,0x77,0xce,0x89,0x8f,0xd6,0x4c,0xc8,0xf9,0x6f,0x62,0xbc,0xf2,0x87,0x42,
						  0x9b,0x56,0x26,0x01,0x42,0x49,0xf2,0x59,0x9a,0xd2,0x5a,0x6b,0xf2,0x77,0xda,0x53,0x75,0xd2,0x69,0xc0,0xa6,0x9b,0x4a,0x2b,0x92,0x78,0x3b,0x7f,0x04,0xf7,0xc9,0xe2,
						  0x4b,0x3c,0x8b,0x0e,0x83,0x9e,0x31,0x26,0x8d,0x65,0x49,0x62,0xa8,0x6f,0xe1,0x6c,0xc8,0x8a,0xf5,0xb4,0x21,0x5e,0x5e,0xc1,0x89,0x56,0x66,0x86,0xf6,0xb6,0x98,0xe9,
						  0xd1,0x16,0xd0,0x25,0x38,0xff,0x96,0xa4,0xf0,0xa3,0x14,0x08,0x95,0xdf,0xae,0x44,0x22,0x82,0x0b,0x05,0xb1,0x1b,0x65,0xe1,0x2c,0x02,0x4b,0x4a,0xe8,0x4b,0x49,0x30,
						  0x53,0xcf,0xa8,0x16,0xc9,0xf8,0x7f,0xfa,0x0b,0xf4,0xb9,0x2c,0x9c,0x41,0x93,0x2d,0xac,0x41,0xf0,0x3a,0x0a,0xa2,0xb6,0x96,0xaa,0x40,0xb0,0x94,0x0e,0xa1,0x4a,0x93,
						  0xcd,0xca,0x4e,0x79,0xe8,0x24,0xe6,0xa0,0xe8,0x31,0x45,0x71,0x41,0x1c,0x04,0x56,0x41,0xab,0xd0,0x09,0xc3,0x17,0xf8,0x12,0xcc,0x73,0x93,0xb8,0xf8,0x7e,0xd7,0x26,
						  0x71,0xca,0xd5,0x75,0x4d,0x3a,0x98,0xbd,0x8a,0x48,0x68,0xaa,0x9a,0x92,0x9e,0x23,0x86,0x5f,0xd2,0xc2,0x08,0x94,0x1c,0xc6,0xd1,0xc7,0xcf,0x8f,0x70,0x51,0x39,0x4f,
						  0x4b,0x04,0xfe,0x5e,0xab,0x16,0xef,0x7b,0xa6,0xbd,0x5b,0x1f,0x9d,0x8b,0x80,0xbf,0x8e,0xdb,0xd9,0xb8,0x63,0x28,0xad,0x59,0x14,0x32,0x8e,0x51,0xdd,0xd1,0x8c,0x93,
						  0x3c,0x3d,0x02,0x7f,0x0d,0xa9,0xc9,0xd3,0xb5,0xbf,0xd2,0x62,0xf0,0x4a,0x2c,0x22,0xfb,0xc6,0xaf,0x59,0x55,0x4b,0xc8,0xc4,0x4a,0x26,0xc0,0x78,0xa3,0xa6,0xbf,0x9e,
						  0xe1,0xa0,0xd3,0x3c,0x44,0xd1,0x3b,0xde,0x20,0x0f,0xe5,0x61,0x7a,0x7d,0x3d,0xb2,0xc2,0xa3,0x1b,0xd3,0x39,0xe4,0x74,0x69,0xb6,0xc3,0xb2,0xb9,0xef,0xce,0x74,0x06,
						  0xe8,0x6c,0xa1,0x4b,0x1a,0xc2,0x73,0x10,0x53,0x73,0x35,0x34,0x67,0xa1,0x1c,0x21,0xd5,0xae,0x4f,0x6d,0x70,0x5c,0x39,0xf5,0x43,0x33,0x1d,0x94,0xfc,0x88,0x04,0x0d,
						  0xbb,0xf8,0x79,0xd2,0x0b,0x66,0xf1,0x69,0x4d,0x34,0x10,0xf7,0x6b,0x86,0x3f,0xd0,0xa0,0xe3,0xd8,0x9c,0x9f,0xc3,0x9b,0x2b,0xa7,0x88,0xd5,0x9e,0xb8,0x06,0x5d,0x61,
						  0xb8,0x1d,0x3f,0xab,0x24,0xed,0xa2,0x7b,0x5e,0x6c,0x78,0x30,0x6f,0xba,0x9e,0xd1,0x13,0xb7,0xfc,0xbb,0xcb,0x3e,0xb6,0x8a,0xfa,0x53,0x69,0x6c,0xd4,0x08,0xc6,0x88,
						  0x8c,0xf6,0xdb,0xed,0x8a,0x8c,0x02,0x9d,0xdc,0xda,0x4d,0xbe,0x77,0x85,0x1c,0x0f,0xb2,0xc9,0x12,0x35,0x20,0xb0,0xe4,0x9a,0x2b,0xe3,0xe6,0xd8,0x5a,0x9b,0x65,0x19,
						  0xc7,0xde,0xc8,0x3a,0x33,0x40,0xc1,0xca,0x6b,0x12,0xea,0xc4,0x01,0xac,0xb0,0x38,0x09,0x84,0xf3,0xa1,0xea,0xfd,0x69,0x94,0xea,0xe9,0xa5,0x8d,0x2d,0x1e,0x4f,0xa1,
						  0xf6,0x22,0x05,0xc7,0xa7,0x0a,0xc5,0x2a,0x58,0x61,0xac,0x81,0x30,0xdf,0x65,0xa2,0x19,0x32,0x8d,0x76,0xb4,0x9f,0x13,0x17,0xe1,0x00,0x44,0xfe,0x68,0xc2,0xc9,0xe4,
						  0xd7,0xfc,0x21,0x59,0x55,0xc3,0x74,0x09,0x7d,0xe8,0xe2,0x1b,0xdb,0x1c,0x19,0x50,0xf0,0x3e,0x80,0x1e,0x01,0x17,0x39,0x2c,0xfb,0xce,0xfa,0x15,0x47,0x72,0xe9,0x1c,
						  0xcf,0xdd,0x4b,0x36,0x7f,0xb4,0x22,0x5e,0x71,0xd3,0xea,0xa2,0xaf,0x99,0xe4,0x7b,0x3b,0xbb,0xc3,0xc4,0xdd,0x67,0x05,0xbe,0xc5,0x9e,0xbc,0x8c,0x86,0x1e,0xec,0xdd,
						  0x94,0xcc,0xb5,0x05,0x1b,0xb7,0x82,0xe8,0xe8,0xa4,0x63,0x3d,0xb4,0x91,0x87,0xd5,0x1b,0x14,0x40,0xcd,0x72,0x74,0xeb,0x4c,0x4f,0x0e,0x2f,0xcc,0xaa,0x18,0xbc,0x3c,
						  0x69,0x74,0x39,0xc3,0x34,0x24,0x21,0x58,0xe3,0xc2,0xb2,0xbd,0x91,0xa7,0x07,0x91,0x00,0xe6,0x4b,0x85,0xd2,0x5c,0x0e,0xf4,0xed,0xaf,0x91,0xef,0x30,0xb0,0x5f,0xb8,
						  0x84,0x2e,0x30,0xec,0x5b,0x90,0x33,0x05,0xfd,0x10,0xae,0x31,0xbc,0xf7,0xe4,0x54,0x48,0x6e,0x02,0x5c,0x6e,0x36,0xda,0xb8,0x82,0x7a,0xe8,0x77,0x11,0xbe,0x9d,0xa5,
						  0x3b,0x7e,0xd8,0x11,0x81,0xc9,0x10,0x67,0x66,0xfb,0x2a,0xa0,0x06,0x13,0x38,0x9c,0x50,0x8e,0x47,0x8a,0xda,0x46,0x6e,0xab,0x79,0x25,0xfb,0xb5,0xa7,0x98,0x0a,0x32,
						  0xf7,0xaa,0xaf,0xf7,0x77,0xa2,0x77,0x0f,0x12,0xfa,0x49,0x1c,0x40,0x9a,0x28,0x04,0x23,0xa6,0xe3,0xb2,0x87,0x90,0x56,0xc0,0x19,0xb7,0x8c,0x7e,0xe3,0x67,0x39,0x9d,
						  0x25,0x5f,0x55,0xd9,0x9d,0xb1,0x0f,0x80,0xac,0x5f,0x57,0x9a,0x95,0x4f,0x4f,0x81,0xdd,0xbe,0xb2,0xab,0xed,0x33,0x1f,0x8f,0x15,0x99,0xb2,0x29,0x0c,0x41,0x5d,0xed};//Ӳ����������
	uchar max_cryp1[1024]={0xbe,0xcb,0x01,0xe7,0x31,0xa1,0x47,0x01,0x58,0xdc,0xa1,0xa9,0xf5,0x30,0x61,0xf4,0x6d,0xa2,0x19,0x81,0x80,0x34,0x1b,0xdb,0x96,0x72,0x72,0x4c,0x32,0xb4,0xb2,0x27,
						  0xbe,0x05,0xb5,0x63,0xa7,0xc0,0xed,0x32,0xe9,0x28,0x19,0xc4,0xb5,0x5f,0xb6,0x62,0x2b,0x39,0x94,0xee,0x78,0x6e,0xe1,0xb9,0xb1,0x65,0x7b,0xf9,0x5f,0x3a,0x63,0x3f,
						  0x3a,0x8f,0x5b,0xa3,0x1a,0x14,0x4b,0xa2,0x83,0x21,0x5f,0xdd,0x32,0x3d,0xf2,0xfd,0x4f,0x99,0x1b,0xcc,0xa0,0x48,0x6a,0xd1,0xbc,0xa7,0xe8,0xe4,0x0a,0x64,0x32,0xe9,
						  0x18,0x4a,0xd4,0x6f,0x77,0x47,0x3f,0x72,0x28,0xf5,0x0d,0xcb,0x69,0x1f,0x5a,0x3e,0x76,0xb5,0x24,0x7e,0x77,0x3e,0xe7,0x4d,0xb9,0xa7,0xf3,0x0f,0x1c,0x34,0xff,0x19,
						  0xea,0x94,0xb2,0x7d,0x1a,0x24,0xc6,0xce,0xc6,0xea,0x59,0xad,0xdb,0x16,0x2f,0x01,0x3a,0x3c,0x71,0xd4,0x0a,0x54,0x38,0x59,0xe5,0x7b,0xa4,0x00,0x45,0x22,0x1c,0xcb,
						  0xee,0x6b,0xbe,0xcc,0x04,0xf8,0xe2,0x42,0x37,0xce,0x82,0x48,0x50,0xb4,0xd7,0xa5,0x7d,0x49,0xf6,0x21,0x43,0x99,0x3f,0x83,0xb6,0x3f,0x15,0xe1,0x84,0xa8,0x3b,0xc3,
						  0xd9,0x2f,0x17,0xa8,0x81,0x33,0x2e,0xec,0xaa,0x9a,0x8a,0x87,0xde,0xc3,0x50,0x8a,0x27,0xb6,0xa4,0x03,0xa5,0x75,0xb2,0x51,0x9b,0xe1,0xd2,0x5e,0x42,0xdf,0x0f,0x18,
						  0xb6,0x5a,0x17,0x11,0x91,0x17,0xb3,0xec,0x12,0xf2,0xe4,0x76,0xbc,0xf6,0x54,0x52,0x9c,0x14,0x8f,0x12,0x6c,0x74,0x00,0xf1,0x18,0x68,0x65,0x94,0x39,0x26,0xf6,0xcb,
						  0x77,0x1e,0xc4,0x43,0xbb,0xa5,0xd1,0xb0,0x5f,0x2b,0x11,0x51,0xcf,0x85,0xf5,0x9f,0x95,0x64,0x36,0x14,0xa3,0x10,0xa6,0x61,0x87,0x33,0xff,0x01,0x73,0x07,0x3b,0xa6,
						  0x10,0x34,0x38,0xb9,0xaa,0x46,0x56,0xed,0x03,0xc0,0x79,0x6f,0x79,0xbf,0xf6,0x3e,0x59,0x21,0x75,0x24,0x9c,0x71,0xeb,0xcc,0xdb,0x5b,0x91,0x8c,0xee,0x62,0x5e,0x68,
						  0x4c,0x96,0x33,0xe7,0x74,0xe4,0x93,0xcf,0xac,0x18,0x4a,0x73,0xb4,0xfd,0xc2,0x29,0xf9,0x1b,0xfc,0x63,0xfe,0xbb,0xde,0x40,0x98,0xa0,0xd4,0xe0,0x2f,0x3b,0xd4,0x17,
						  0x0f,0xd1,0x15,0x44,0x20,0x3f,0x3d,0xdb,0xda,0x40,0x27,0xbe,0x6a,0xcd,0x11,0x35,0x24,0x54,0x51,0x82,0x4d,0xd6,0x8c,0x52,0x4f,0x90,0x62,0xbd,0x17,0x7b,0x21,0x55,
						  0x58,0x18,0x3a,0x48,0x40,0xc1,0xa4,0xa3,0x6c,0xa4,0xb5,0x1c,0x3a,0x4a,0xd4,0x65,0x71,0x54,0xf4,0x59,0x1b,0x2b,0xeb,0x1f,0x85,0x6c,0x95,0xa5,0x36,0xbe,0x5d,0xaf,
						  0x49,0x3f,0x30,0xe3,0x6d,0x23,0xbb,0x4f,0xba,0x97,0x00,0x8b,0xf8,0xaa,0x65,0x64,0xe0,0xab,0xbb,0x0a,0x9c,0xbe,0x9c,0xd6,0xd0,0xaf,0x43,0xd6,0x31,0x4b,0xe5,0x9f,
						  0x96,0xb0,0x03,0xcf,0xe4,0x94,0xbf,0x07,0x6e,0x5c,0x45,0x43,0xa2,0x4d,0xb9,0x9b,0x24,0x22,0xa6,0x5c,0x49,0xee,0xdb,0xc1,0xe0,0x03,0xa4,0xc8,0xcc,0xb1,0xa3,0x94,
						  0x3f,0xc9,0x42,0x14,0xb0,0x06,0xb3,0x08,0x2b,0xd5,0x9e,0x36,0x92,0x68,0xcb,0xeb,0x1b,0xe8,0x88,0x94,0x02,0x84,0x03,0xc3,0x4a,0x89,0x34,0xf4,0x22,0xfd,0x8b,0x91,
						  0xaf,0x2d,0x0e,0xa4,0x82,0xad,0x45,0xba,0x57,0x07,0x51,0x7e,0x88,0xaa,0xd2,0xc3,0x60,0xa9,0x8d,0xe3,0xc7,0x1b,0xe8,0xbd,0x90,0x21,0xd2,0x00,0xc8,0x4d,0xee,0x32,
						  0x05,0x5f,0x18,0xdb,0x11,0x66,0x8d,0x15,0x42,0xc9,0x68,0x26,0xc1,0xe0,0x30,0x98,0xd8,0xd9,0x36,0x8e,0xc5,0x6f,0x14,0xed,0x3b,0x79,0x47,0x1f,0xaa,0xfe,0x44,0x82,
						  0x6f,0x71,0x70,0x60,0x2c,0xba,0xdb,0xfe,0x2a,0xf3,0xc6,0x55,0xc5,0x07,0x43,0xf4,0x91,0x85,0x2e,0x6a,0x43,0x95,0xfa,0xb4,0xf0,0x78,0xdd,0x48,0xb1,0x67,0xdb,0x86,
						  0x1e,0x2c,0xba,0xe3,0x25,0xf1,0x25,0x3c,0x16,0xcd,0x6e,0xea,0x31,0x01,0x9d,0xc1,0xdf,0x1b,0xb4,0x84,0xe1,0x9b,0xc2,0x54,0x67,0xc8,0x0b,0xd2,0x64,0xd3,0x14,0x3f,
						  0xf3,0x80,0x6c,0x38,0x46,0xdb,0xd7,0x61,0xec,0x25,0xc8,0x74,0xae,0xcf,0x4f,0x19,0x0e,0x84,0xb8,0x1e,0xed,0x5c,0x46,0x56,0xdb,0xc8,0xd4,0xfb,0x2d,0x0e,0x8c,0x97,
						  0xb4,0x9f,0xaf,0x28,0x1c,0xb6,0x7e,0x3c,0xd7,0xef,0xa4,0x71,0xca,0xb4,0x46,0x8d,0xb6,0x87,0x7c,0xa0,0x00,0xb3,0xa4,0x89,0x99,0x91,0x5e,0xad,0xe3,0x56,0xb4,0xd1,
						  0x0d,0xd7,0xb4,0x50,0x3d,0x20,0x8a,0x8d,0x1f,0x66,0xf4,0x13,0x4d,0xdc,0x26,0x46,0x21,0x0d,0x32,0xf9,0xd6,0x96,0x0e,0x1e,0x24,0xe6,0xc6,0x16,0x45,0x09,0xf3,0x4c,
						  0x13,0xd0,0x85,0xa7,0x0b,0x2e,0xfd,0x6c,0x4d,0x7e,0xf5,0xf6,0x38,0x50,0x23,0xa6,0x59,0x8b,0xe9,0xe0,0xd9,0xa2,0x3d,0x8b,0x5e,0xd4,0x89,0x11,0xf2,0x67,0xa5,0x30,
						  0x05,0x0e,0xbf,0x31,0x23,0xb6,0x27,0x70,0x6d,0xac,0x4d,0x91,0xd1,0xc5,0x5d,0x79,0xba,0xd6,0x62,0xb8,0xdc,0xf4,0xba,0x71,0x17,0x74,0xc2,0x4c,0xbf,0x37,0x94,0x32,
						  0xe2,0x77,0x10,0x07,0xa8,0x48,0x90,0x52,0xf1,0xb4,0xc3,0xa9,0x84,0xd6,0x5e,0xcc,0x79,0x62,0x83,0x15,0xc7,0xdc,0x66,0xc2,0xfd,0x3b,0xc3,0xee,0x73,0xa6,0xd0,0xcf,
						  0x63,0x91,0x3d,0x2d,0x61,0x77,0xfb,0x75,0x02,0xc3,0xe6,0x86,0xe5,0xc6,0x51,0xab,0xd6,0xfe,0x5f,0x83,0xa3,0x9c,0x0e,0xb9,0x22,0x71,0xb3,0x46,0x79,0xea,0x71,0x08,
						  0x51,0x84,0x8c,0x9d,0x6d,0x95,0x7d,0x8d,0xfa,0x65,0x14,0x64,0x6c,0xe1,0xb8,0xe6,0x3c,0xe2,0xff,0xd6,0x9d,0xe2,0xc2,0xe9,0xad,0x22,0xdc,0xd4,0x56,0x10,0xf5,0x3e,
						  0xd1,0xc6,0xb1,0x4e,0xf4,0x40,0xfd,0x43,0x9d,0x9c,0x15,0x25,0x39,0x91,0x5f,0x7e,0x25,0x27,0xea,0xd2,0x5e,0x63,0x74,0x9a,0x7f,0x86,0xa5,0xf6,0xf1,0xf9,0x34,0x70,
						  0xfb,0xf4,0xc5,0xb4,0x77,0x84,0x9b,0xbb,0xff,0x82,0xfe,0xed,0x65,0xec,0x7d,0x72,0x0c,0x5b,0x04,0x06,0x4a,0xc2,0xf3,0xa2,0x80,0x39,0xe7,0x4d,0x8d,0x0f,0x81,0xda,
						  0x63,0x63,0xf0,0x62,0x81,0x43,0x15,0x72,0xd3,0x8b,0xc6,0x3b,0xe0,0x1b,0x09,0xa3,0x8a,0xbb,0x59,0x42,0x7c,0x02,0xe1,0x11,0xf9,0x4a,0xf0,0xe7,0x6b,0xa0,0x0c,0x4f,
						  0xfb,0x60,0x4a,0xf4,0xfa,0x79,0x17,0x33,0x09,0xcf,0xf7,0x2b,0x73,0xd6,0x3e,0x54,0xe7,0x6a,0xed,0xbc,0x81,0xfc,0x43,0x4e,0xd1,0x44,0xbc,0xa3,0x1f,0x60,0xc4,0x9e};//������������
	//process body
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	memset(pubkey, 0, sizeof(pubkey));
	memset(prikey, 0, sizeof(prikey));
	memset(message, 0, sizeof(message));
	if((ret=NDK_AlgSM2KeyPairGen(pubkey, prikey))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_AlgSM2Encrypt(pubkey, real_message, strlen((char*)real_message), cryp, &cryp_len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case1:�����쳣����
	if((ret=NDK_AlgSM2Decrypt(NULL, cryp, cryp_len, message, &len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2Decrypt(prikey, NULL, cryp_len, message, &len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2Decrypt(prikey, cryp, 0, message, &len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2Decrypt(prikey, cryp, cryp_len, NULL, &len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2Decrypt(prikey, cryp, cryp_len, message, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2Decrypt(prikey, cryp, 1025, message, &len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�쳣����, �ô����˽Կ����,Ӧ�÷���ʧ��
	if((ret=NDK_AlgSM2Decrypt(err_prikey, pre_cryp, sizeof(pre_cryp), message, &len))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:�쳣����, �ô����˽Կ����,Ӧ�÷���ʧ��
	if((ret=NDK_AlgSM2Decrypt(pre_prikey, err_cryp, sizeof(pre_cryp), message, &len))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:��������:Ԥ���˽Կ���ù��߶����Ľ��м��ܣ��ٵ��ý��ܺ������õ���ʵ������Ӧ������һ��
	if((ret=NDK_AlgSM2Decrypt(pre_prikey, pre_cryp, sizeof(pre_cryp), message, &len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(len!=sizeof(pre_message))
	{
		send_result("line %d:%sSM2����ʧ��(get_len=%d, real_len=%d)", __LINE__, TESTAPI, len, sizeof(pre_message));
		RETURN;
	}
	if(memcmp(message, pre_message, len))
	{
		send_result("line %d:%sSM2����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//Ŀǰ����ֻ֧�����ĳ��ȵ�32�ֽڵģ�����32�ֽڳ���
	//case4:(Ӳ����������)�����ݽ��ܲ���:��������Ӳ���㷨���ܳ�Ԥ�����ĵ����ģ���������Ϊ928�ֽڣ��ٵ��ý��ܺ������õ���ʵ������Ӧ������һ��
	memset(message, 0, sizeof(message));
	if((ret=NDK_AlgSM2Decrypt(pre_prikey, max_cryp, sizeof(max_cryp), message, &len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN; 
	}
	if(len!=strlen((char*)max_message_928))
	{
		send_result("line %d:%sSM2����ʧ��(get_len=%d, real_len=%d)", __LINE__, TESTAPI, len, strlen((char*)max_message_928));
		RETURN;  
	}
	if(memcmp(message, max_message_928, len))
	{
		send_result("line %d:%sSM2����ʧ��", __LINE__, TESTAPI);
		RETURN; 
	}

	//case5:(������������)�����ݽ��ܲ���:��������Ӳ���㷨���ܳ�Ԥ�����ĵ����ģ���������Ϊ928�ֽڣ��ٵ��ý��ܺ������õ���ʵ������Ӧ������һ��
	memset(message, 0, sizeof(message));
	if((ret=NDK_AlgSM2Decrypt(pre_prikey, max_cryp1, sizeof(max_cryp1), message, &len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN; 
	}
	if(len!=strlen((char*)max_message_928))
	{
		send_result("line %d:%sSM2����ʧ��(get_len=%d, real_len=%d)", __LINE__, TESTAPI, len, strlen((char*)max_message_928));
		RETURN; 
	}
	if(memcmp(message, max_message_928, len))
	{
		send_result("line %d:%sSM2����ʧ��", __LINE__, TESTAPI);
		RETURN; 
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 	
	return;

}