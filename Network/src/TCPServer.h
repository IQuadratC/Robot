#pragma once

#include "Network.h"

class Server;

class TCPServer{

public:
    TCPServer(Server* server);
    ~TCPServer();

    void StartTCP();
    void mainLoop();
    void DisconnectTCP(uint8_t client);
    void SendTCPData(uint8_t client, unsigned char* data, std::size_t length);
     void StoptTCP();
    
private:
    int tcpListener;
    Server* server;
    fd_set readfds;
    int max_sd,sd,activity,valread,new_socket;
    
    struct sockaddr_in address;
    int addrlen;
    char buffer[BufferSize]{0};

};



