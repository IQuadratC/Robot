#include "TCPServer.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>


int tcpListener;
struct sockaddr_in address;
int addrlen;
char buffer[BUfferSize] = {0};


void StartTCP(){
    int opt = 1;

    if ((tcpListener = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(tcpListener, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(tcpListener, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(tcpListener, MaxClients) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

    addrlen = sizeof(address);

}

void SendTCPData(int id, unsigned char* data, int length){
    
}
