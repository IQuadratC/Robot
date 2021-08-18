#pragma once

#include "Network.h"
#include "Packet.h"

class Server;

class ServerHandle{

public:
    ServerHandle(Server* server);
    ~ServerHandle();

    void DebugMessage(uint8_t, Packet*);

    void ClientSettings(uint8_t client, Packet* packet);
    void ClientUDPConnection(uint8_t client, Packet* packet);
    void ClientUDPConnectionStatus(uint8_t client, Packet* packet);
    void ClientSimulatedLidarData(uint8_t client, Packet* packet);
    void ClientGetSLAMMap(uint8_t client, Packet* packet);
    void ClientGetPosition(uint8_t client, Packet* packet);
    
private:
    Server* server;
};

