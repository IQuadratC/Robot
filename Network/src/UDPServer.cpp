#include "UDPServer.h"

#include "Server.h"

 UDPServer::UDPServer(Server* server) : server(server){

 }
 UDPServer::~UDPServer(){

 }

void UDPServer::StartUDP(){
    int opt = 1;

    if ((udpListener = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        std::cout << "SERVER: socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(udpListener, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    {
        std::cout << "SERVER: setsockopt" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(udpListener, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "SERVER: bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    mainLoop();
}

void UDPServer::mainLoop()
{
    while (server->serverState == NetworkState::connected)
    {   
        struct sockaddr clientAddress;
        addrlen = sizeof(clientAddress);

        if(recvfrom (udpListener, buffer, BufferSize, 0, (struct sockaddr *) &clientAddress, (socklen_t*)&addrlen) < 0)
        {
            std::cout << "SERVER: no data recived" << std::endl;
            continue;
        }

        uint8_t data[addrlen];
        memcpy(data, buffer, addrlen);
        uint8_t client = server->HandelData(data, addrlen);
    }
}

void UDPServer::SendUDPData(uint8_t client, uint8_t* data, size_t length){
    if(sendto(udpListener, data, length, 0, (sockaddr*)&server->serverClients[client].udpAdress, sizeof(server->serverClients[client].udpAdress)) < 0)
    {
        std::cout << "SERVER: UDP send error" << std::endl;
    }
}
void UDPServer::DisconnectUDP(uint8_t client){

}
void UDPServer::StopUDP(){

}