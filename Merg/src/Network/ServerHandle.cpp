#include "ServerHandle.h"
#include "Server.h"
#include "../Robot/Roboter.h"
#include "../SLAM/slam.h"


ServerHandle::ServerHandle(Server *server) : server(server)
{
    Logger = Log::GetNetworkLogger();
}
ServerHandle::~ServerHandle()
{
    
}

void ServerHandle::DebugMessage(uint8_t client, Packet* packet)
{
    std::string message = packet->ReadString();
    Logger->info("[{0}] Debug: {1}",(int)client,message);
}

void ServerHandle::ClientSettings(uint8_t client, Packet* packet)
{
    std::stringstream version(packet->ReadString());
    std::string segment;
    std::vector<std::string> versions;

    while(std::getline(version, segment, ','))
    {
        versions.push_back(segment);
    }

    for (size_t i = versions.size(); i > 0; i--)
    {
        if(versions[i] == "1.3"){
            server->serverClients[client].clientUDPSupport = packet->ReadBool();
            server->serverClients[client].clientCamSupport = packet->ReadBool();
            server->serverClients[client].clientJoyStickSupport = packet->ReadBool();
            server->serverClients[client].clientChatSupport = packet->ReadBool();
            server->serverClients[client].clientLidarSupport = packet->ReadBool();
            server->serverClients[client].clientLidarSimSupport = packet->ReadBool();
            server->serverClients[client].clientSLAMSupport = packet->ReadBool();
            break;
        }
        else if(versions[i] == "1.2"){
            server->serverClients[client].clientUDPSupport = packet->ReadBool();
            server->serverClients[client].clientCamSupport = packet->ReadBool();
            server->serverClients[client].clientJoyStickSupport = packet->ReadBool();
            server->serverClients[client].clientChatSupport = packet->ReadBool();
            server->serverClients[client].clientLidarSupport = packet->ReadBool();
            server->serverClients[client].clientLidarSimSupport = packet->ReadBool();
            break;
        }
        else if(versions[i] == "1.1"){
            server->serverClients[client].clientUDPSupport = packet->ReadBool();
            server->serverClients[client].clientCamSupport = packet->ReadBool();
            server->serverClients[client].clientJoyStickSupport = packet->ReadBool();
            server->serverClients[client].clientChatSupport = packet->ReadBool();
            server->serverClients[client].clientLidarSupport = packet->ReadBool();
            break;
        }
    }
    
    std::stringstream ss;
    ss << "[" << (int)client << "] recived client settings:" <<
    "\nVersion " << version.str() <<
    "\nUDP " << server->serverClients[client].clientUDPSupport <<
    "\nCam " << server->serverClients[client].clientCamSupport <<
    "\nJoystick " << server->serverClients[client].clientJoyStickSupport <<
    "\nChat " << server->serverClients[client].clientChatSupport <<
    "\nLidar " << server->serverClients[client].clientLidarSupport <<
    "\nLidarSim " << server->serverClients[client].clientLidarSimSupport <<
    "\nSLAMMap " << server->serverClients[client].clientSLAMSupport;

    Logger->info(ss.str().c_str());

    if (server->serverUDPSupport && server->serverClients[client].clientUDPSupport)
    {
        server->serverSend->ServerStartUDP(client);
    }
    else
    {
        Logger->info("[{0}] connection init done.",(int)client);
        server->serverClients[client].state = NetworkState::connected;
    }
   
}

void ServerHandle::ClientUDPConnection(uint8_t client, Packet* packet)
{
    Logger->info("[{0}] udp message recived ",(int)client);

    server->serverClients[client].updConnected = true;
    server->serverSend->ServerUDPConnection(client, true);
}

void ServerHandle::ClientUDPConnectionStatus(uint8_t client, Packet* packet)
{
    server->serverClients[client].updConnected = packet->ReadBool() && server->serverUDPSupport;
    Logger->info("[{0}] UDP connection status: {1}", (int)client, server->serverClients[client].updConnected);
   
    Logger->info("[{0}] connection init done.",(int)client);
    server->serverClients[client].state = NetworkState::connected;
}

void ServerHandle::ClientSimulatedLidarData(uint8_t client, Packet* packet)
{
    int lenght = packet->ReadInt32();
    if (lenght!=360){
        Logger->error("ClientSimulatedLidarData did not contain exactly 360 entrys.");
        return;
    }

    float data[360];
    for (size_t i = 0; i < lenght; i++)
    {
        data[i] = packet->ReadFloat();
    }
    Logger->info("Recived Simulated Data");

    LidarData(data);
}

void ServerHandle::ClientGetSLAMMap(uint8_t client, Packet* packet)
{
    
}

void ServerHandle::ClientGetPosition(uint8_t client, Packet* packet)
{
   
}

extern Roboter roboter;

void ServerHandle::ClientControllMode(uint8_t client, Packet* packet)
{
   
}

void ServerHandle::ClientJoystickMove(uint8_t client, Packet* packet)
{
    roboter.data.x = packet->ReadFloat();
    roboter.data.y = packet->ReadFloat();
    roboter.data.speed = packet->ReadFloat();
    roboter.Do_command(commands::move);
}

void ServerHandle::ClientJoystickRotate(uint8_t client, Packet* packet)
{
    roboter.data.speed = packet->ReadFloat();
    roboter.Do_command(commands::rotate);
}

void ServerHandle::ClientJoystickStop(uint8_t client, Packet* packet) {
    roboter.Do_command(commands::stop);
}


