#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */
#include <time.h>
#include <math.h>
void *ThreadMain(void *arg);             /* Main program of a thread */

struct ThreadArgs /* Structure of arguments to pass to client thread */
{
    int clntSock1;                      /* Socket descriptor for client */
    int clntSock2;                      /* Socket descriptor for client */
};


int main(int argc, char *argv[])

{
	struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    DWORD  threadID;                 /* Thread ID from CreateThread() */
	
	int servSock;                    /* Socket descriptor for server */
    int clntSock1,clntSock2;                    /* Socket descriptor for client */
    WSADATA wsaData;                 /* Structure for WinSock setup communication */
	struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
 	int clntLen;            /* Length of client address data structure */

	if (WSAStartup(0x101, &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        printf("WSAStartup() failed");
        exit(1);
    }

	/* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        printf("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(5678);              /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        printf("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, 3) < 0)
        printf("listen() failed");

    printf("Server is waiting for clients.\n");
    for (;;) /* Run forever */
    {
	    clntLen = sizeof(echoClntAddr);
	
	    /* Wait for a client to connect */
	    if ((clntSock1 = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
	        printf("accept() failed");
	    if ((clntSock2 = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
	        printf("accept() failed");

		/* Create separate memory for client argument */
        threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
        threadArgs -> clntSock1 = clntSock1;
        threadArgs -> clntSock2 = clntSock2;

        if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs,
              0, (LPDWORD) &threadID) == NULL)
            printf("thread_create() failed");

        printf("New client with thread ID: %ld and socket:%d and %d\n", threadID,clntSock1,clntSock2);
    }
}// end main

void *ThreadMain(void *threadArgs)
{
	int clntSock1,clntSock2,i;                   /* Socket descriptor for client connection */
    /* Extract socket file descriptor from argument */
    clntSock1 = ((struct ThreadArgs *) threadArgs) -> clntSock1;
	clntSock2 = ((struct ThreadArgs *) threadArgs) -> clntSock2;
	//把所有的牌排列出來
	int card[52]={1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,10,11,12,13}; 
	int c1[52]={0};
	int c2[52]={0};
	char echoBuffer[1000];        /* Buffer for echo string */
	int recvMsgSize;                    /* Size of received message */
	int rand_num;
	int temp, pos;
	srand((unsigned) time(NULL) + getpid());
	for(i=0;i<52;i++)
	{
		int pos = 51 * rand() / RAND_MAX;
		temp = card[i];
		card[i] = card[pos];
		card[pos] = temp;
	}
	for(i=0;i<52;i++)
	{
		printf("%d ",card[i]);
	}
	printf("\n");
	for(i=0;i<26;i++)
	{
		c1[i]=card[i];
		printf("%d ",c1[i]);
	}
	printf("\n");
	for(i=0;i<26;i++)
	{
		c2[i]=card[26+i];
		printf("%d ",c2[i]);
	}
	
	printf("\n");
	u_long iMode=1;//非攔阻模式 
	ioctlsocket(clntSock1,FIONBIO,&iMode);
	ioctlsocket(clntSock2,FIONBIO,&iMode);
	int nError;
	int o=0,w=0;//記錄要傳出去的卡的位置 
	char s[10];
	int bye[52]={0};//紀錄已出過的牌 
	char num[10];
	char str[1024];
	
    /* Send received string and receive again until end of transmission */
    while (1)      /* zero indicates end of transmission */
    {  	
    	int count=0,count1=0;//用來判斷玩家卡牌是否已出完 
    	for(i=0;i<52;i++)
    	{
    		if(c1[i]==0)
    		{
    			count++;
			}
			if(c2[i]==0)
    		{
    			count1++;
			}
		}
		if(count==52)
		{
			printf("C1_WIN\n");
			strcpy(str,"C1_WIN");
			send(clntSock2, str, 8, 0);
			send(clntSock1, str, 8, 0);
			closesocket(clntSock1);    /* Close client socket */   
			closesocket(clntSock2);    /* Close client socket */ 
			break;
		}
		if(count1==52)
		{
			printf("C2_WIN\n");
			strcpy(str,"C2_WIN");
			send(clntSock2, str, 8, 0);
			send(clntSock1, str, 8, 0);
			closesocket(clntSock1);    /* Close client socket */   
			closesocket(clntSock2);    /* Close client socket */ 
			break;
		}
    	
 		//clntSock1
		if ((recvMsgSize = recv(clntSock1, echoBuffer, 1000, 0)) > 0 ) //判斷C1傳來的訊息 
		{
			//bye[o+w]=c1[o];
			int b=(1+o+w)%13;//紀錄目前數到多少 
			if(b==0)
			{
				b=13;
			}
			
			strcpy(s,"o");
			if(strncmp(echoBuffer,s,1)==0)//翻牌 
			{
				bye[o+w]=c1[o];//把出出去的牌 加進 已出過的牌的陣列 
				
				printf("bye %d   (%d)\n",bye[o+w],b);
				itoa(c1[o],num,10);
				printf("C1: %s\n",num);
				send(clntSock2, num, strlen(num)+1, 0);//分別送給C1、C2 
				send(clntSock1, num, strlen(num)+1, 0);
				c1[o]=0;//將發出去的牌歸零 
				o++;
				
				
			}
			else if(strncmp(echoBuffer,"p",1)==0)//搶牌 
			{
				int f=atoi(num);
				b--;
				if(f==b)//若搶牌正確，將已出過的牌加進玩家2 
				{
					printf("C1_COR\n");
					strcpy(str,"C1_COR");
					send(clntSock2, str, 8, 0);
					send(clntSock1, str, 8, 0);
					int j=0;
					for(i=o;i<52;i++)//因前面發出去的牌會歸零 所以要把不是零的牌往前移 
					{
						c2[i-o]=c2[i];
					}
					for(i=o;i<52;i++)
					{
						c1[i-o]=c1[i];	
					}
					for(i=0;i<52;i++)//將已出過的牌加進C2牌堆內 
					{
						if(c2[i]==0)
						{
							c2[i]=bye[j];
							j++;
						}
					}
					
					/*for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c1[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c2[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",bye[i]);
					}
					printf("\n");*/ 
					for(i=0;i<52;i++)//已出過的牌要重新記錄 
					{
						bye[i]=0;
					}
					o=0;
					w=0;
				}
				else
				{
					printf("C1_error\n");
					strcpy(str,"C1_error");
					send(clntSock2, str, 10, 0);
					send(clntSock1, str, 10, 0);
					int j=0;
					for(i=w;i<52;i++)
					{
						c1[i-w]=c1[i];	
					}
					for(i=w;i<52;i++)
					{
						c2[i-w]=c2[i];	
					}
					for(i=0;i<52;i++)
					{
						if(c1[i]==0)
						{
							c1[i]=bye[j];
							j++;
						}
					}
					/*for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c1[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c2[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",bye[i]);
					}*/ 
					printf("\n");
					for(i=0;i<52;i++)
					{
						bye[i]=0;
					}
					o=0;
					w=0;
				}
			}
			
		}
		nError=WSAGetLastError();
		if(nError!=WSAEWOULDBLOCK && nError!=0)
		{
			//printf("Disconnection! Error Code:%d\n",nError);
		}
		
		//clntSock2
		if ((recvMsgSize = recv(clntSock2, echoBuffer, 1000, 0)) > 0) 
		{
			//bye[o+w]=c2[w];
			int b=(1+o+w)%13;
			if(b==0)
			{
				b=13;
			}
			strcpy(s,"w");
			if(strncmp(echoBuffer,s,1)==0)//翻牌 
			{
				bye[o+w]=c2[w];
				printf("bye %d   (%d)\n",bye[o+w],b);
				itoa(c2[w],num,10);
				printf("C2: %s\n",num);
				send(clntSock2, num, strlen(num)+1, 0);
				send(clntSock1, num, strlen(num)+1, 0);
				c2[w]=0;
				w++;
			}
			else if(strncmp(echoBuffer,"q",1)==0)//搶牌 
			{
				int f=atoi(num);
				b--;
				if(f==b)//翻對 
				{
					printf("C2_COR\n");
					strcpy(str,"C2_COR");
					send(clntSock2, str, 8, 0);
					send(clntSock1, str, 8, 0);
					int j=0;
					for(i=w;i<52;i++)
					{
						c1[i-w]=c1[i];
					}
					for(i=w;i<52;i++)
					{
						c2[i-w]=c2[i];
					}
					for(i=0;i<52;i++)
					{
						if(c1[i]==0)
						{
							c1[i]=bye[j];
							j++;
						}
					}
					/*for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c1[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c2[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",bye[i]);
					}*/ 
					printf("\n");
					for(i=0;i<52;i++)
					{
						bye[i]=0;
					}
					o=0;
					w=0;
				}
				else//翻錯 
				{
					printf("C2_error\n");
					strcpy(str,"C2_error");
					send(clntSock2, str, 10, 0);
					send(clntSock1, str, 10, 0);
					int j=0;
					for(i=o;i<52;i++)
					{
						c2[i-o]=c2[i];
					}
					for(i=o;i<52;i++)
					{
						c1[i-o]=c1[i];
					}
					for(i=0;i<52;i++)
					{
						if(c2[i]==0)
						{
							c2[i]=bye[j];
							j++;
						}
					}
					/*for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c1[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",c2[i]);
					}
					printf("\n");
					for(i=0;i<52;i++)////////////////////////////看牌 
					{
						printf("%d ",bye[i]);
					}*/ 
					printf("\n");
					for(i=0;i<52;i++)
					{
						bye[i]=0;
					} 
					o=0;
					w=0;
				}
			}
			
		}
		nError=WSAGetLastError();
		if(nError!=WSAEWOULDBLOCK && nError!=0)
		{
			//printf("Disconnection! Error Code:%d\n",nError);
		}
		
    }
    closesocket(clntSock1);    /* Close client socket */   
	closesocket(clntSock2);    /* Close client socket */ 
    free(threadArgs);
	return (NULL);
}
