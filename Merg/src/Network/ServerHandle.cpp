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
    std::string version = packet->ReadString();

    if(version == "1.1"){
        server->serverClients[client].clientUDPSupport = packet->ReadBool();
        server->serverClients[client].clientCamSupport = packet->ReadBool();
        server->serverClients[client].clientJoyStickSupport = packet->ReadBool();
        server->serverClients[client].clientChatSupport = packet->ReadBool();
        server->serverClients[client].clientLidarSupport = packet->ReadBool();
    }

    std::cout << "SERVER: [" << (int)client << "] recived client settings:" <<
    "\nVersion " << version <<
    "\nUDP " << server->serverClients[client].clientUDPSupport <<
    "\nCam " << server->serverClients[client].clientCamSupport <<
    "\nJoystick " << server->serverClients[client].clientJoyStickSupport <<
    "\nChat " << server->serverClients[client].clientChatSupport <<
    "\nLidar " << server->serverClients[client].clientLidarSupport <<
    std::endl;

    if (server->serverUDPSupport && server->serverClients[client].clientUDPSupport)
    {
        server->serverSend->ServerStartUDP(client);
    }
    else
    {
        std::cout << "SERVER: [" << (int)client << "] connection init done." << std::endl;
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
   
    std::cout << "SERVER: [" << (int)client << "] connection init done." << std::endl;
    server->serverClients[client].state = NetworkState::connected;
}

void ServerHandle::ClientSimulatedLidarData(uint8_t client, Packet* packet)
{
    float* data = new float[360];
    for (size_t i = 0; i < 360; i++)
    {
        data[i] = packet->ReadFloat();
    }


}

void ServerHandle::ClientGetSLAMMap(uint8_t client, Packet* packet)
{
    
}

void ServerHandle::ClientGetPosition(uint8_t client, Packet* packet)
{
   
}