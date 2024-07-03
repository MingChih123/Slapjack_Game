#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024
void *ThreadMain(void *arg);
struct ThreadArgs
{
	SOCKET sd;
};
int main(int argc, char** argv) 
{
   SOCKET        	sd;
   struct sockaddr_in serv;
   char  		str[1024];
   WSADATA 		wsadata;
   DWORD threadID;
   struct ThreadArgs *threadArgs;
   int n;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

   sd=socket(AF_INET, SOCK_STREAM, 0);  
   threadArgs=(struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
   threadArgs->sd=sd;
   
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 連接至 echo server
   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) ThreadMain,threadArgs,0,(LPDWORD) &threadID)==NULL)
   {
   		printf("CreateThread() failed");
   }

	while(1)//傳送 
	{
	    str[0]=getch();
	   	n = send(sd, str, 1, 0); //傳送###至server
	   	if (n < 0)
	   	{
	   		closesocket(sd);
		}
	   
	   
	   //printf("send:%s\n",str);
	
	}
   
}
void *ThreadMain(void *threadArgs)//接收 
{
	SOCKET sd;
	char str[1024];
	int n;
	sd=((struct ThreadArgs *) threadArgs) ->sd;
	while(1)
	{
		n=recv(sd,str,strlen(str)+10,0);
		
		if(n<=0)
		{
			break;
		}
		printf("%s\n",str);
	}
	closesocket(sd); //關閉TCP socket
    WSACleanup();  // 結束 WinSock DLL 的使用
    return NULL;
}
