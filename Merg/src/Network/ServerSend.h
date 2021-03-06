#pragma once

#include "Network.h"
#include "Packet.h"

class Server;

class ServerSend {

public:
    ServerSend(Server* server);
    ~ServerSend();

    void DebugMessage(std::string message);
    void ServerSettings(uint8_t client);
    void ServerStartUDP(uint8_t client);
    void ServerUDPConnection(uint8_t client, bool recived);

    void ServertSLAMMap(uint8_t client, uint8_t data[], int length);
    void ServertSLAMMapPart(uint8_t client, uint8_t data[], int startIndex, int endIndex);
    void ServerPosition(uint8_t client);

    void ServerGetSimulatedLidarData(uint8_t client);

private:
    Server* server;
    
    std::shared_ptr<spdlog::logger> Logger;
};
