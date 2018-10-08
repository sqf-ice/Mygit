/*
 * FileName:       
 * Author:         zhangnw  
 * Description:   ��Ԫ���Զ�����
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version>  <desc>
 *	zhangnw	   20120814			created
 */

#ifndef _CUT_ASSERT_H_
#define _CUT_ASSERT_H_

/*-----------------------includes-------------------------------*/
#include "lib.h"

/*---------------constants/macro definition---------------------*/
//Begin a unit-test case
#define CUT_BEGIN() cls_printf("����%s...", TESTAPI)

//End a unit-test case
#define CUT_END() send_result("%s����ͨ��", TESTAPI)

//Assert that expression is TRUE (non-zero). Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT(expression, actions) \
do\
{\
	if(!(expression))\
	{\
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);actions;\
	}\
}while(0)

#define CUT_ASSERT_OP(actual, OP, expected, actions) \
do\
{\
	int actval = 0, expval = 0;\
	if(!((actval=(int)(actual))OP(expval=(int)(expected))))\
	{\
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, actval, expval);actions;\
	}\
}while(0)

//ע��:CUT_ASSERT_EQUAL��CUT_ASSERT_NOT_EQUALֻ������int������,���������ݵĲ���ֻ��ֱ��дif�ж�
//Assert that actual == expected. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_EQUAL(actual, expected, actions) CUT_ASSERT_OP(actual, ==, expected, actions)

//Assert that actual != expected. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_NOT_EQUAL(actual, expected, actions) CUT_ASSERT_OP(actual, !=, expected, actions)

#define CUT_ASSERT_PTR_OP(actual, OP, expected, actions) \
do\
{\
	if(!(((void *)(actual))OP((void *)(expected))))\
	{\
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);actions;\
	}\
}while(0)

//Assert that pointers actual == expected. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_PTR_EQUAL(actual, expected, actions) CUT_ASSERT_PTR_OP((actual), ==, (expected), actions)	//CUT_ASSERT((actual)==(expected), actions)

//Assert that pointers actual != expected. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_PTR_NOT_EQUAL(actual, expected, actions) CUT_ASSERT_PTR_OP((actual), !=, (expected), actions)	//CUT_ASSERT((actual)!=(expected), actions)

//Assert that pointer value == NULL. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_PTR_NULL(value, actions) CUT_ASSERT_PTR_OP((value), ==, NULL, actions)	//CUT_ASSERT((value)==NULL, actions)

//Assert that pointer value != NULL. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_PTR_NOT_NULL(value, actions) CUT_ASSERT_PTR_OP((value), !=, NULL, actions)	//CUT_ASSERT((value)!=NULL, actions)

//Assert that strings actual and expected are equivalent. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_STRING_EQUAL(actual, expected, actions) CUT_ASSERT(!strcmp(actual, expected), actions)

//Assert that strings actual and expected differ. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_STRING_NOT_EQUAL(actual, expected, actions) CUT_ASSERT(strcmp(actual, expected), actions)

//Assert that 1st count chars of actual and expected are the same. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_NSTRING_EQUAL(actual, expected, count, actions) CUT_ASSERT(!memcmp(actual, expected, count), actions)

//Assert that 1st count chars of actual and expected differ. Actions (return or goto label or other) take place unless ASSERTION is TRUE.
#define CUT_ASSERT_NSTRING_NOT_EQUAL(actual, expected, count, actions) CUT_ASSERT(memcmp(actual, expected, count), actions)

#endif

