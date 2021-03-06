#pragma once

#include "Network.h"
#include "Packet.h"
#include "TCPServer.h"
#include "UDPServer.h"
#include "ServerSend.h"
#include "ServerHandle.h"

struct ServerClient
{
    unsigned char id = 0;
    char* ip = 0;
    int socket = 0;
    char receiveBuffer[BufferSize]{0};
    struct sockaddr_in udpAdress{0}; 
    NetworkState state = NetworkState::notConnected;

    bool updConnected;
    bool clientUDPSupport = false;
    bool clientCamSupport = false;
    bool clientJoyStickSupport = false;
    bool clientChatSupport = false;
    bool clientLidarSupport = false;
    bool clientLidarSimSupport = false;
    bool clientSLAMSupport = false;
};

class Server{

public:
    TCPServer* tcpServer;
    UDPServer* udpServer;
    ServerHandle* serverHandle;
    ServerSend* serverSend;

    typedef void (ServerHandle::*MFP)(uint8_t, Packet*);
    std::map<uint8_t, MFP> packetHandlers;

    ServerClient serverClients[MaxClients];
    NetworkState serverState;

    bool serverUDPSupport = false;
    bool serverCamSupport = false;
    bool serverJoyStickSupport = false;
    bool serverChatSupport = false;
    bool serverLidarSupport = false;
    bool serverLidarSimSupport = true;
    bool serverSLAMSupport = true;

    Server();
    ~Server();
    void StartServer();
    void ConnectClient(uint8_t client);

    void HandelData(uint8_t data[], size_t length, uint8_t client = 0);

    void AddHeaderToPacket(Packet* packet);
    void SendTCPData(uint8_t client, Packet* packet);
    void SendTCPDataToAll(Packet* packet);
    void SendUDPData(uint8_t client, Packet* packet);
    void SendUDPDataToAll(Packet* packet);

    void DisconnectClient(uint8_t client);
    void StopServer();

private:
    std::shared_ptr<spdlog::logger> Logger;

};
