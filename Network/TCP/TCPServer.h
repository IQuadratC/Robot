#pragma once
#include <sys/types.h>
#include <sys/socket.h>

#include "../Network.hpp"
#include "../Server/Server.h"

class TCPServer{

public:
    TCPServer(Server* server);
    ~TCPServer();
    void StartTCP();
    void mainLoop();
    void StoptTCP();
    void SendTCPData(int id, unsigned char* data, std::size_t length);
private:
    int tcpListener;
    Server* server;
    bool running = true;
    fd_set readfds;
    int max_sd,sd,activity;

};



