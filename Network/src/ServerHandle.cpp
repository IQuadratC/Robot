#include"ServerHandle.h"
#include "Server.h"

ServerHandle::ServerHandle(Server *server) : server(server)
{
    
}

ServerHandle::~ServerHandle()
{
    
}

void ServerHandle::DebugMessage(uint8_t client, Packet* packet)
{
    std::string message = packet->ReadString();
    std::cout << "SERVER: [" << (int)client << "] Debug: " << message << std::endl;
}

void ServerHandle::ClientSettings(uint8_t client, Packet* packet)
{
    std::cout << "SERVER: [" << (int)client << "] recived client settings" << std::endl;
    server->serverClients[client].clientUDPSupport = packet->ReadBool();

    if (server->serverUDPSupport && server->serverClients[client].clientUDPSupport)
    {
        server->serverSend->ServerStartUDP(client);
    }
    else
    {
        server->serverClients[client].state = NetworkState::connected;
    }
   
}
void ServerHandle::ClientUDPConnection(uint8_t client, Packet* packet)
{
    std::cout << "SERVER: [" << (int)client << "] udp message recived " << std::endl;
    server->serverClients[client].updConnected = true;
    server->serverSend->ServerUDPConnection(client, true);
}
void ServerHandle::ClientUDPConnectionStatus(uint8_t client, Packet* packet)
{
    server->serverClients[client].updConnected = packet->ReadBool() && server->serverUDPSupport;
    std::cout << "SERVER: [" << (int)client << "] UDP connection status: " << server->serverClients[client].updConnected << std::endl;
}