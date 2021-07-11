#include"ServerHandle.h"
#include "Server.h"

ServerHandle::ServerHandle(Server *server) : server(server)
{
    
}

ServerHandle::~ServerHandle()
{
    
}

void ServerHandle::ClientConnectionReceived(uint8_t client,Packet* packet)
{
    std::cout << "SERVER: Client" << client << " connected." << std::endl;
}

void ServerHandle::DebugMessage(uint8_t client, Packet* packet)
{
    std::string message = packet->ReadString();
    std::cout << "SERVER: Client" << client << " Debug: " << message << std::endl;
}
