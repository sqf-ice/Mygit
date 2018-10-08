#include <stdio.h>
#include <iostream.h>
#include <Winsock.h>
//#include <Winsock2.h>
#include <time.h>

#pragma comment(lib, "wsock32")

#define MAX_NUM 4096+512;
#define RECEIVE 1
#define SEND 2
#define TYPE RECEIVE
//#define TYPE SEND

void do_echo(int sockfd, struct sockaddr *pcliaddr, int clilen) 
{ 
	int n; 
	int len; 
	char mesg[5960]; 
	
	for(;;) 
	{ 
		len = clilen; 
		/* waiting for receive data */ 
		n = recvfrom(sockfd, mesg, 5960, 0, pcliaddr, &len); 
		/* sent data back to client */ 
		sendto(sockfd, mesg, n, 0, pcliaddr, len); 
	} 
} 

int main(int argc,char *argv[])
//int main()
{
	struct sockaddr_in server,cliaddr,clientadd;
//	SOCKADDR clientadd;
    int clilen;

//(sockaddr*)&addrClient,&len)
	SOCKET s,rs;
	int len,pn,n;
	FILE *fd=NULL;
	char buf[5960],ip[20],key;
	u_short IpPortnum=5000;
	char IpPort[10];
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ilen;
	int protoType=0;
	int commuType=0;
	int connectpress=0;
	int portadd=0;
	int ret=0;
	long Rtimes;
	char tempbuf[5960];

	if(argc<6)
	{
		printf("\n�����������......\n��ο����������ʽ˵����");
		printf("\n���� [�˿ں�][Э������][����ѹ��][�˿ں��ۼ�][ͨѶ����]");
		printf("\n�˿ںţ���̫���Ķ˿ں�");
		printf("\nЭ�����ͣ�0��TCP��1��UDP");
		printf("\n����ѹ����0���ǣ�1��ѹ��(ÿ��ͨѶ��Ҫ��������)");
		printf("\n�˿ں��ۼӣ�0���ǣ�1���ۼӣ�ÿ�����Ӷ˿ںŶ��ı䣩");
		printf("\nͨѶ���ͣ�0��˫����1�����ͣ�����������2������");
		printf("\n\nĬ�ϣ�\n�˿ں�[3456]��TCP[0]�����ӷ�ѹ��[0]���˿ںŲ���[0]��˫��[0]\n");
		return 0;
	}
#if 0
	//����˿ں�
	printf("please input the port number:");
	if (fgets(IpPort,10,stdin)!=NULL)
	{	
		printf("IpPort:%s ",IpPort);
		if (!(IpPortnum = atoi(IpPort)))
		{	
			IpPortnum = 3456;
		}
	}
	else 
	{	
		IpPortnum = 3456;
	}
#else
	if (!(IpPortnum = atoi(argv[1])))
	{	
		IpPortnum = 5000;
	}
#endif
	if(IpPortnum==3493) //ΪʲôIpPortnum Ϊ3493(DA5)/3495ʱ,bind���������,bcat?
		IpPortnum=3496;
	if(atoi(argv[2])==1)
		protoType=SOCK_DGRAM;
	else
		protoType=SOCK_STREAM;

    if((connectpress=atoi(argv[3]))!=1)
		connectpress=0;

    if((portadd=atoi(argv[4]))!=1)
		portadd=0;

	switch(atoi(argv[5]))
	{
	case 0:commuType=0;
		break;
	case 1:commuType=1;
		break;
	case 2:commuType=2;
		break;
	default:commuType=0;
		break;
	}

#if TYPE==RECEIVE
	{
		struct tm *t;
		time_t timer;
		while(1)
		{
			if (WSAStartup(version, &wsaData)!=0)                             
			{                                                                  
				printf("WSAStartup Err!");
				return -1;                                                     
			}
			
			printf("protoType(%d)\n",protoType);
			if ((s=socket(AF_INET, protoType, 0))==-1)                         
			{	
				printf("socket error\n");                                      
				return -1;                                                     
			}   
			
			server.sin_family		= AF_INET;                                  
			server.sin_addr.s_addr  = INADDR_ANY;                               
			server.sin_port			= htons(IpPortnum);                               
			len = sizeof(server);
			if (bind(s, (struct sockaddr *)&server, len)==-1)                     
			{	
				printf("bind error\n");                                        
				closesocket(s);                                                
				return -1;                                                     
			}  
			if(	protoType==SOCK_STREAM)
			{
				if (listen(s, 1) ==-1 )                  
				{	
					printf("listen error\n");                                        
					closesocket(s);                                                
					return -1;                                                     
				}
			}
                              
			key = 0;
			fd = fopen("netlog.txt","a+");
			printf("waiting for connecting....................\n"); 
			ilen = fprintf(fd, "waiting for connecting....................\n");
			rs = accept(s, (struct sockaddr *)&server, &len);                      
			printf("some one is connected\n");
			fprintf(fd, "some one is connected\n");
			
			//��¼ʱ��
			timer = time(NULL);
			t = localtime(&timer);
			printf("time: %s", asctime(t));
			fprintf(fd, "time: %s", asctime(t));

			//��¼�ͻ��˵�ַ���˿�
			strcpy(ip, inet_ntoa(server.sin_addr));                             
			pn = (int) ntohs(server.sin_port);                                   
			printf("Client Ip Address=%s Port Numbdr =%d\n", ip, pn);            
			fprintf(fd, "Client Ip Address=%s Port Numbdr =%d\n", ip, pn);  
			while(1)
				{
					Rtimes=0;
//					n = recv(rs, buf, 1024, 0);                              
        			if(	protoType==SOCK_STREAM)
						n = recv(rs, buf, 5000, 0);  
					else
						n = recvfrom(rs,buf,5000,0,(struct sockaddr *)&server,&clilen);
//						n = recvfrom(rs,buf,5000,0,(struct sockaddr *)&clientadd,&clilen);

					if (n>0)                                          
					{
						int i=0;
						buf[n] = 0x00;                                           
						printf("receive data:%d+%d  number:%d\n", buf[0], n);                            
						fprintf(fd, "receive data:%d  number:%d\n", buf[0], n);                                 
						printf("Send the data !\n");
						fprintf(fd, "Send the data :\n");
						fwrite(buf, 1, n, fd);
						fprintf(fd, "\n");
						printf("server port number: %d\n",IpPortnum);

						if(portadd==1)
						{
							IpPortnum ++;	
							if(IpPortnum>8000)
								IpPortnum=5000;
							buf[n] =(IpPortnum>>8) ;
							buf[n+1] = IpPortnum;
							send(rs, buf, n+2, 0); 
							for(i =0;i<n+2;i++)
								printf("%02x " ,buf[i]);
							printf("\n");
						}else{
			        			if(	protoType==SOCK_STREAM)
									send(rs, buf, n, 0);   
								else
									sendto(rs,buf,n,0,(struct sockaddr *)&server,clilen);

//						n = recvfrom(rs,buf,5000,0,(sockaddr*)&clientadd,&clilen));

							for(i =0;i<n;i++)
								printf("%02x " ,buf[i]);
							printf("\n");
						}

						if((commuType==1)&&(connectpress==0))
						{
							while(1)
							{
								if(send(rs, buf, n, 0)!=n)
								{ 
									printf("send error....");
								}
								printf("send data:\n");
								for(i =0;i<n;i++)
									printf("%02x " ,buf[i]);
								printf("\n");
								Sleep(1200); //���뵥λ
							}
						}
						if((commuType==2)&&(connectpress==0))
						{
							printf("\n��[1]�ν�������[����:%d] �ɹ�",n);
							memcpy(tempbuf,buf,n);
   							Rtimes++;
						}

						if((commuType==2)&&(connectpress==0))
						{
							while(1)
							{
								memset(buf,0x00,sizeof(buf));
								ret=recv(rs, buf, 5000, 0); 
								if((ret>0)&&(Rtimes>0))
								{
									printf("\n��(%d)�ν�������[����:%d]",Rtimes+1,ret);
									if (memcmp(tempbuf,buf,ret)!=0) 
										printf("  ʧ��....");
									else
									        printf("  �ɹ�");
									Rtimes++;
								}
								Sleep(100);
							} 
						}
					}
					else if (n==0)                        
					{                               
						printf("disconnect\n");
						fprintf(fd, "disconnect\n");
						key = 'e';
						break;   		            
					}   
					if(connectpress==1)
						break;
				}
				fclose(fd);
				closesocket(rs); 
				closesocket(s);

		}
	}
#else
	{
		int i;
		while(1)
		{
			if (WSAStartup(version, &wsaData)!=0)                             
			{                                                                  
				printf("WSAStartup Err!");
				return -1;                                                     
			}
			
			if ((s=socket(AF_INET, protoType, 0))==-1)                         
			{	
				printf("socket error\n");                                      
				return -1;                                                     
			}   
			server.sin_family		= AF_INET;                                  
 			server.sin_addr.s_addr  = (232<<24)|(2<<16)|(168<<8)|(192); //bcat 232            
// 			server.sin_addr.s_addr  = (160<<24)|(2<<16)|(168<<8)|(192);             
			server.sin_port			= htons(IpPortnum);                               
			len = sizeof(server); 
			if ((connect(s, (struct sockaddr *)&server, len) )==-1)    
			{	
				
				printf("connect error\n");                                        
				closesocket(s);        
				 getchar();     
				return -1;                                              
			} 
			else
			{
				printf("server port number:%d\n",IpPortnum);
			}
			memset(buf,8,8);
			n=8;
			printf("Send the data !\n");
			getchar();
			if((send(s, buf, 8, 0) )==-1)
				printf("send error"); 
			n = recv(s, buf, 1024, 0);                              
			if (n>0)                                          
			{	
				buf[n] = 0x00;                                           
				printf("receive data:%d  number:%d\n", buf[0], n);                            
			}
			else         
			{                               
				printf("disconnect\n");
			}  
			IpPortnum = ((buf[n-2]<<8) |(0x000000ff& (buf[n-1])));
			
			for(i =0;i<n;i++)
				printf("%02x " ,buf[i]);
			printf("\n");
			closesocket(s);  
		}
	}
#endif
	return 0;
}

