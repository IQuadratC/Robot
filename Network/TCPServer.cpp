#include "TCPServer.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

int tcpSocket;

void StartTCP(int port){

    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket == -1){
        perror("TCP Socket failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
}
