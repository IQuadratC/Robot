#include <cstdio>
#include<iostream>
#include<signal.h>
#include "Server.h"

Server server;
void  signalhandler(int sig)
{
	signal(sig, SIG_IGN);

    server.StopServer();

	exit(0);
}


void udpServer(){
    char buffer[100];
    char *message = "Hello Client";
    int listenfd, len;
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));
  
    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);        
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET; 
   
    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
       
    //receive the datagram
    len = sizeof(cliaddr);

     puts("Server Online:");
    int n = recvfrom(listenfd, buffer, sizeof(buffer),
            0, (struct sockaddr*)&cliaddr, (socklen_t*)&len); //receive message from server
    buffer[n] = '\0';
    puts("Server Data recived:");
    puts(buffer);
           
    // send the response
    sendto(listenfd, message, 1000, 0,
          (struct sockaddr*)&cliaddr, sizeof(cliaddr));
}

void udpClient(){
    char buffer[100];
    char *message = "Hello Server";
    int sockfd, n;
    struct sockaddr_in servaddr;
      
    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
      
    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      
    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }
  
    // request to send datagram
    // no need to specify server address in sendto
    // connect stores the peers IP and port
    sendto(sockfd, message, 1000, 0, (struct sockaddr*)NULL, sizeof(servaddr));
      
    // waiting for response
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
    puts(buffer);
  
    // close the descriptor
    close(sockfd);
}

int main()
{
    signal(SIGINT, signalhandler);

    server.StartServer();

    while (true)
    {
        /* code */
    }
}

