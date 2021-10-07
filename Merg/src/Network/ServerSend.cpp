#include"ServerSend.h"
#include "Server.h"
#include "../SLAM/slam.h"
#include "../SLAM/slamMap.h"

ServerSend::ServerSend(Server *server) : server(server)
{
    Logger = Log::GetNetworkLogger();
}
ServerSend::~ServerSend(){

}

void ServerSend::DebugMessage(std::string message){

    Packet* packet = new Packet((uint8_t) Packets::debugMessage);
    packet->Write(message);
    server->SendTCPDataToAll(packet);

}

void ServerSend::ServerSettings(uint8_t client){

    std::string version = "1.1,1.2,1.3,1.4";

    std::stringstream ss;
    ss << "[" << (int)client << "] sending settings:" <<
    "\nVersion " << version <<
    "\nUDP " << server->serverUDPSupport <<
    "\nCam " << server->serverCamSupport <<
    "\nJoystick " << server->serverJoyStickSupport <<
    "\nChat " << server->serverChatSupport <<
    "\nLidar " << server->serverLidarSupport <<
    "\nLidarSim " << server->serverLidarSimSupport <<
    "\nSLAMMap " << server->serverSLAMSupport <<
    "\nSLAMMapSize " << MAPSIZE <<
    "\nSLAMMapIntervall " << MAPINTERVALL;

    Logger->info(ss.str().c_str());

    Packet* packet = new Packet((uint8_t) Packets::serverSettings);
    packet->Write(client);

    packet->Write(version);
    packet->Write(server->serverUDPSupport);
    packet->Write(server->serverCamSupport);
    packet->Write(server->serverJoyStickSupport);
    packet->Write(server->serverChatSupport);
    packet->Write(server->serverLidarSupport);
    packet->Write(server->serverLidarSimSupport);
    packet->Write(server->serverSLAMSupport);
    packet->Write(MAPSIZE);
    packet->Write(MAPINTERVALL);

    server->SendTCPData(client, packet);
}

void ServerSend::ServerStartUDP(uint8_t client){

    Logger->info("[{0}] starting udp test", (int)client );

    Packet* packet = new Packet((uint8_t) Packets::serverStartUDP);
    server->SendTCPData(client, packet);


    std::async(std::launch::async,[&](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (!server->serverClients[client].updConnected){
            ServerUDPConnection(client, false);
        }
    }); 
}

void ServerSend::ServerUDPConnection(uint8_t client, bool recived){

    Logger->info("[{0}] udp test message", (int)client);

    Packet* packet = new Packet((uint8_t) Packets::serverUDPConnection);
    packet->Write(recived);

    if (recived)
    {
        server->SendUDPData(client, packet);
    }
    else
    {
        server->SendTCPData(client, packet);
    }
}

void ServerSend::ServerGetSimulatedLidarData(uint8_t client){

    Logger->info("[{0}] request Lidar Data", (int)client);

    Packet* packet = new Packet((uint8_t) Packets::serverGetSimulatedLidarData);
    
    server->SendTCPData(client, packet);
}

#define SLAMMAPPARTSIZE 1024

void ServerSend::ServertSLAMMap(uint8_t client, uint8_t data[], int length){

    Logger->info("[{0}] send SLAM Map", (int)client);

    Packet* packet = new Packet((uint8_t) Packets::servertSLAMMap);

    packet->Write(length);
    packet->Write(data, length);

    if (length > SLAMMAPPARTSIZE){
        for (int start = 0; start < length; start += SLAMMAPPARTSIZE)
        {
            int end = start + SLAMMAPPARTSIZE;
            ServertSLAMMapPart
        }
        
    }

    server->SendUDPData(client, packet);
}

void ServerSend::ServertSLAMMapPart(uint8_t client, uint8_t data[], int startIndex, int endIndex){

    Logger->info("[{0}] send SLAM Map Part", (int)client);

    Packet* packet = new Packet((uint8_t) Packets::servertSLAMMapPart);

    packet->Write(startIndex);
    packet->Write(endIndex);
    packet->Write(data, endIndex - startIndex);

    server->SendUDPData(client, packet);
}

void ServerSend::ServerPosition(uint8_t client){

    Logger->info("[{0}] request Position", (int)client);

    Packet* packet = new Packet((uint8_t) Packets::serverPosition);

    extern float pos[3];
    packet->Write(pos[0]);
    packet->Write(pos[1]);
    packet->Write(pos[2]);

    server->SendUDPData(client, packet);
}
