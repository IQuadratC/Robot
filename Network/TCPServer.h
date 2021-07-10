#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include "Network.hpp"

class Server;

class TCPServer{

public:
    TCPServer(Server* server);
    ~TCPServer();
    void StartTCP();
    void mainLoop();
    void StoptTCP();
    void DisconnectTCP(uint8_t client);
    void SendTCPData(int id, unsigned char* data, std::size_t length);
    void StartThread();
private:
    int tcpListener;
    Server* server;
    bool running = true;
    fd_set readfds;
    int max_sd,sd,activity,valread,new_socket;
    const std::string connectionMessage = "Connected!";
    

};



