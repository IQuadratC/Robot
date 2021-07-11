#pragma once

#include "Network.h"
#include "Packet.h"

class Server;

class ServerSend {

public:
    ServerSend(Server* server);
    ~ServerSend();

    void ServerConnection(uint8_t client);
    void DebugMessage(std::string message);

private:
    Server* server;
};