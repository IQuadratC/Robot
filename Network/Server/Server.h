#pragma once

#include "../TCP/TCPServer.h"
#include "../UDP/UDPServer.h"
#include "ServerClient.h"
#include "ServerHandle.h"
#include "ServerSend.h"
#include "Packet.h"
#include <map>

enum Packets{
    serverConnection = 1,        
    clientConnectionReceived = 2,
    debugMessage = 3,
};
void ClientConnectionReceived(uint8_t,Packet&);
void DebugMessage(uint8_t,Packet&);


struct ServerClient
{
    unsigned char id;
    int socket;
    char receiveBuffer[BUfferSize];
};



class Server{

public:
    TCPServer* tcpServer;
    UDPServer* udpServer;
    ServerHandle* serverhandle;
    ServerSend* serversend;

    std::map<uint8_t,void(*) (uint8_t,Packet&)> packetHandlers;

    ServerClient serverClient[MaxClients];

    Server();
    ~Server();
    void StartServer();
    void StopServer();
    void HandelData();
    void ConnectClient();
    void AddHeaderToPacket();
    void SendTCPData();
    void SendTCPDataToAll();
    void SendUDPData();
    void SendUDPDataToAll();
    void DisconnectClient();



};