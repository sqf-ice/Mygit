#include "NDK.h"
#include <fcntl.h>

int doEvent(int nModuleID, void *pInBuf,int nInLen,void *pOut,int * nOutLen)
{
	NDK_ScrInitGui();
	NDK_ScrPrintf("�����ӹ���%d����:\n%s",nModuleID,pInBuf);
	strcpy(pOut,"�������");
	*nOutLen = strlen(pOut);
	fprintf(stderr,"%s,%d\n",__FUNCTION__,__LINE__);
	NDK_KbGetCode(0,NULL);
	return 0;
}

int main(int argc,char *argv[])
{
	int percentage;
	int fd,ret,ret1,ret2,ret3;
	char buf[4096];
	struct tm time;
	int i = 0;
	
	NDK_AppSetEventCallBack(doEvent);
	ret = NDK_SysInit();
	NDK_ScrInitGui();
	NDK_ScrPrintf("���ǳ��������� %d",ret);
	NDK_KbGetCode(0,NULL);
	return 0;
}

