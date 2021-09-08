#include "UDPServer.h"

#include "Server.h"

UDPServer::UDPServer(Server* server) : server(server){
    Logger = Log::GetNetworkLogger();
}
UDPServer::~UDPServer(){

}

void UDPServer::StartUDP(){
    const int opt = 1;

    bzero(&address, sizeof(address));

    if ((udpListener = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        Logger->error("SERVER: UDP socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_addr.s_addr = htonl(INADDR_ANY);;
    address.sin_port = htons(PORT);
    address.sin_family = AF_INET;

    if (setsockopt(udpListener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0){

        Logger->error("SERVER: UDP setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if (bind(udpListener, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        Logger->error("SERVER: UDP bind failed");
        exit(EXIT_FAILURE);
    }

    mainLoop();
}

void UDPServer::mainLoop()
{
    while (server->serverState == NetworkState::connected || server->serverState == NetworkState::connecting)
    {   

        memset (buffer, 0, BufferSize);
        struct sockaddr_in clientAddress;
        addrlen = sizeof(clientAddress);

        if(readlen = recvfrom(udpListener, buffer, BufferSize, 0, (struct sockaddr *) &clientAddress, (socklen_t*)&addrlen) < 0)
        {
            Logger->warn("SERVER: TCP no data recived");
            continue;
        }
        if (readlen == 0){
            Packet* packet = new Packet((uint8_t*)buffer, BufferSize);
            readlen = packet->ReadInt32() + 4;
        }
        int client;
        std::string ip = inet_ntoa(clientAddress.sin_addr);
        for (size_t i = 1; i < MaxClients; i++)
        {
            if (server->serverClients[i].ip == ip){
                client = i;
                if (!server->serverClients[client].updConnected){
                    server->serverClients[client].udpAdress = clientAddress;
                }
                break;
            }
        }

        Logger->info("SERVER: [{0}] udp data recived Message legth: {1}", (int)client, readlen);

        uint8_t data[readlen];
        memcpy(data, buffer, readlen);
        server->HandelData(data, readlen, client);
    }
}

void UDPServer::SendUDPData(uint8_t client, uint8_t* data, size_t length){
    if (!server->serverClients[client].updConnected){
        Logger->error("SERVER: UDP not setup");
        return;
    }

    if(sendto(udpListener, data, length, 0, (sockaddr*)&server->serverClients[client].udpAdress, sizeof(server->serverClients[client].udpAdress)) < 0)
    {
        Logger->error("SERVER: UDP send error");
    }
}
void UDPServer::DisconnectUDP(uint8_t client){
    
}
void UDPServer::StopUDP(){
    close(udpListener);
}


