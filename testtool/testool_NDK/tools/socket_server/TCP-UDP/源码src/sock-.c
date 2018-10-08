/*
[ת]vc��socket��̲��� 
    sockets���׽��֣���������֣���ʽ�׽��֣�SOCK_STREAM�������ݱ��׽��֣�SOCK_DGRAM����ԭʼ�׽��֣�SOCK_RAW��������TCP��socket����ǲ��õ���ʽ�׽��֡�����������У�������������ӵ�һ����������Ҫ����һ��ws2_32.lib�Ŀ��ļ���

�������˱�̵Ĳ��裺
1�������׽��ֿ⣬�����׽���(WSAStartup()/socket())��
2�����׽��ֵ�һ��IP��ַ��һ���˿���(bind())��
3�����׽�������Ϊ����ģʽ�ȴ���������(listen())��
4���������󣬽����������󣬷���һ���µĶ�Ӧ�ڴ˴����ӵ��׽���(accept())��
5���÷��ص��׽��ֺͿͻ��˽���ͨ��(send()/recv())��
6�����أ��ȴ���һ��������
7���ر��׽��֣��رռ��ص��׽��ֿ�(closesocket()/WSACleanup())��
*/
//�������˴������£�
#include <stdio.h>
#include <Winsock2.h>
void main()
{
 WORD wVersionRequested;
 WSADATA wsaData;
 int err;
 
 wVersionRequested = MAKEWORD( 1, 1 );
 
 err = WSAStartup( wVersionRequested, &wsaData );
 if ( err!=0 ) {
  return;
 }
 
 if ( LOBYTE( wsaData.wVersion )!=1 ||
        HIBYTE( wsaData.wVersion )!=1 ) {
  WSACleanup( );
  return;
 }
 SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);

 SOCKADDR_IN addrSrv;
 addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
 addrSrv.sin_family=AF_INET;
 addrSrv.sin_port=htons(6000);
 
 bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

 listen(sockSrv,5);

 SOCKADDR_IN addrClient;
 int len=sizeof(SOCKADDR);
 while(1)
 {
  SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
  char sendBuf[50];
  sprintf(sendBuf,"Welcome %s to here!",inet_ntoa(addrClient.sin_addr));
  send(sockConn,sendBuf,strlen(sendBuf)+1,0);
  char recvBuf[50];
  recv(sockConn,recvBuf,50,0);
  printf("%s\n",recvBuf);
  closesocket(sockConn);
 }

}
/*
�ͻ��˱�̵Ĳ��裺
1�������׽��ֿ⣬�����׽���(WSAStartup()/socket())��
2���������������������(connect())��
3���ͷ������˽���ͨ��(send()/recv())��
4���ر��׽��֣��رռ��ص��׽��ֿ�(closesocket()/WSACleanup())��
*/
//�ͻ��˵Ĵ������£�
#include <stdio.h>
#include <Winsock2.h>
void main()
{
 WORD wVersionRequested;
 WSADATA wsaData;
 int err;
 
 wVersionRequested = MAKEWORD( 1, 1 );
 
 err = WSAStartup( wVersionRequested, &wsaData );
 if ( err!=0 ) {
  return;
 }
 
 if ( LOBYTE( wsaData.wVersion )!=1 ||
        HIBYTE( wsaData.wVersion )!=1 ) {
  WSACleanup( );
  return;
 }
 SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
 
 SOCKADDR_IN addrSrv;
 addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
 addrSrv.sin_family=AF_INET;
 addrSrv.sin_port=htons(6000);
 connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
 send(sockClient,"hello",strlen("hello")+1,0);
 char recvBuf[50];
 recv(sockClient,recvBuf,50,0);
 printf("%s\n",recvBuf);
 
 closesocket(sockClient);
 WSACleanup();
}