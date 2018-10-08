/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* Author 			: bcat
* version			: V1.0
* DATE			    : 20090311
* description		: ��̫�����Ժ�̨-TCP&UDP
*				  
************************************************************************/
#include <stdio.h>
#include <iostream.h>
#include <Winsock.h>
#include <time.h>

#pragma comment(lib, "wsock32")

#define MAX_NUM 4096+512;

int main(int argc,char *argv[])
{
	struct sockaddr_in server;
	SOCKET s,rs;
	int len,pn,n;
	FILE *fd=NULL;
	char buf[5960],ip[20],key;
	u_short IpPortnum=5000;
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ilen;
	int protoType=0;
	int commuType=0;
	int connectpress=0;
	int portadd=0;
	int ret=0,ret1;
	long Rtimes;
	char tempbuf[5960];
	struct tm *t;
	time_t timer;
	char IpPort[11];

	if(argc==1)
	{
		printf("\n�����ʽ1��ֱ����������磺nettest");
		printf("\n�����ʽ2������ȫ�����������£�");
		printf("\n���� [�˿ں�][Э������][����ѹ��][�˿ں��ۼ�][ͨѶ����]\n\n");
		Sleep(800); //���뵥λ
		printf("\nִ�������ʽ1\n");
		printf("������˿ں�:");
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
	}else if(argc<6)
	{
		printf("\n�����������......\n��ο����������ʽ˵����");
		printf("\n���� [�˿ں�][Э������][����ѹ��][�˿ں��ۼ�][ͨѶ����]");
		printf("\n�˿ںţ���̫���Ķ˿ں�");
		printf("\nЭ�����ͣ�0��TCP��1��UDP");
		printf("\n����ѹ����0���ǣ�1��ѹ��(ÿ��ͨѶ��Ҫ��������)");
		printf("\n�˿ں��ۼӣ�0���ǣ�1���ۼӣ�ÿ�����Ӷ˿ںŶ��ı䣩");
		printf("\nͨѶ���ͣ�0��˫����1�����ͣ�����������2������");
		printf("\n\nĬ�ϣ�\n�˿ں�[5000]��TCP[0]�����ӷ�ѹ��[0]���˿ںŲ���[0]��˫��[0]\n");
        printf("\n\n�������� 5000 0 0 0 0\n");

		Sleep(4300); //���뵥λ
		return 0;
	}
	if(argc==1)
	{
		protoType=SOCK_STREAM;
		connectpress=0;
		portadd=0;
		commuType=0;
	}
	else
	{

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
	}
	
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
		if(	protoType==SOCK_STREAM)
		{
			rs = accept(s, (struct sockaddr *)&server, &len);                      
			printf("some one is connected\n");
			fprintf(fd, "some one is connected\n");
		}
		
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
			if(	protoType==SOCK_STREAM)
				n = recv(rs, buf, 5000, 0);  
			else
				n = recvfrom(s,buf,5000,0,(struct sockaddr *)&server,&len);
			
			if (n>0)                                          
			{
				int i=0;
				buf[n] = 0x00;                                           
				printf("receive data:%d  number:%d\n", buf[0], n);                            
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
					if(	protoType==SOCK_STREAM)
						send(rs, buf, n+2, 0);   
					else
						sendto(s,buf,n+2,0,(struct sockaddr *)&server,len);
					
					//							send(rs, buf, n+2, 0); 
#if 0
					for(i =0;i<n+2;i++)
						printf("%02x " ,buf[i]);
					printf("\n");
#endif
				}else{
					if(	protoType==SOCK_STREAM)
						send(rs, buf, n, 0);   
					else
						sendto(s,buf,n,0,(struct sockaddr *)&server,len);
#if 0
					for(i =0;i<n;i++)
						printf("%02x " ,buf[i]);
					printf("\n");
#endif
				}
				
				if((commuType==1)&&(connectpress==0))
				{
					while(1)
					{
#if 1
						if(	protoType==SOCK_STREAM)
							ret1=send(rs, buf, n, 0);   
						else
							ret1=sendto(s,buf,n,0,(struct sockaddr *)&server,len);
#else
						if(send(rs, buf, n, 0)!=n)
						{ 
							printf("send error....");
						}
#endif
						if(ret1!=n)
							printf("send error....");
						printf("\nsend data:(%d),lenth(%d)",buf[0],n);
#if 0
						for(i =0;i<n;i++)
							printf("%02x " ,buf[i]);
						printf("\n");
#endif
#if 0	//���Ե�������ʱ,����ĳ�ʱ��Ӱ��,������ 20180714					
						if(	protoType==SOCK_STREAM)
							Sleep(1300); //���뵥λ
						else
							Sleep(1900); //���뵥λ
#endif
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
#if 0
						ret=recv(rs, buf, 5000, 0); 
#else
						if(	protoType==SOCK_STREAM)
							ret = recv(rs, buf, 5000, 0);  
						else
							ret = recvfrom(s,buf,5000,0,(struct sockaddr *)&server,&len);
						
#endif
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
				else if (n<0) //bcat 2009.04.21 
				{
					printf("SOCKET_ERROR\ndisconnect\n");
					fprintf(fd, "SOCKET_ERROR\ndisconnect\n");
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
	return 0;
}

