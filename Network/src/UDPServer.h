#pragma once

#include "Network.h"

class Server;

class UDPServer{

public:
    UDPServer(Server* server);
    ~UDPServer();

    void StartUDP();
    void mainLoop();
    void SendUDPData(uint8_t client, uint8_t* data, size_t length);
    void DisconnectUDP(uint8_t client);
    void StopUDP();

private:
    
    int udpListener;
    Server* server;
    fd_set readfds;

    struct sockaddr_in address;
    int addrlen;
    char buffer[BufferSize]{0};
};