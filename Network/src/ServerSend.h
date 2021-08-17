#pragma once

#include "Network.h"
#include "Packet.h"
#include "../vendor/toojpeg.h"

class Server;

class ServerSend {

public:
    ServerSend(Server* server);
    ~ServerSend();

    void DebugMessage(std::string message);
    void ServerSettings(uint8_t client);
    void ServerStartUDP(uint8_t client);
    void ServerUDPConnection(uint8_t client, bool recived);

private:
    Server* server;
};