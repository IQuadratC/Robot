#pragma once

#include "ServerHandle.h"
#include "TCPServer.h"
#include "UDPServer.h"
#include "ServerSend.h"
#include "Packet.h"
#include <map>

enum Packets{
    serverConnection = 1,        
    clientConnectionReceived = 2,
    debugMessage = 3,
};
void ClientConnectionReceived(uint8_t,Packet*);
void DebugMessage(uint8_t,Packet*);


struct ServerClient
{
    unsigned char id = 0;
    char* ip = 0;
    int socket = 0;
    char receiveBuffer[BufferSize]{0};
    bool connected{false};
};



class Server{

public:
    TCPServer* tcpServer;
    UDPServer* udpServer;
    ServerHandle* serverhandle;
    ServerSend* serversend;

    std::map<uint8_t,void(*) (uint8_t,Packet*)> packetHandlers;

    ServerClient serverClients[MaxClients];

    Server();
    ~Server();
    void StartServer();
    void StopServer();
    uint8_t HandelData(uint8_t data[], size_t length, uint8_t client = 0);
    void ConnectClient();
    void AddHeaderToPacket();
    void SendTCPData();
    void SendTCPDataToAll();
    void SendUDPData();
    void SendUDPDataToAll();
    void DisconnectClient(uint8_t client);


};