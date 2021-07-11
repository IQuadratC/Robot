#pragma once

#include "Network.h"
#include "Packet.h"

class Server;

class ServerHandle{

public:
    ServerHandle(Server* server);
    ~ServerHandle();

    void ClientConnectionReceived(uint8_t,Packet*);
    void DebugMessage(uint8_t, Packet*);

private:
    Server* server;
};

