#include"ServerSend.h"
#include "Server.h"
#include <future>
#include <chrono>
#include "../SLAM/slam.h"

ServerSend::ServerSend(Server *server) : server(server)
{
    
}
ServerSend::~ServerSend(){

}

void ServerSend::DebugMessage(std::string message){

    Packet* packet = new Packet((uint8_t) Packets::debugMessage);
    packet->Write(message);
    server->SendTCPDataToAll(packet);

}

void ServerSend::ServerSettings(uint8_t client){

    std::string version = "1.1";
    std::cout << "SERVER: [" << (int)client << "] sending settings:" <<
    "\nVersion " << version <<
    "\nUDP " << server->serverUDPSupport <<
    "\nCam " << server->serverCamSupport <<
    "\nJoystick " << server->serverJoyStickSupport <<
    "\nChat " << server->serverChatSupport <<
    "\nLidar " << server->serverLidarSupport <<
    std::endl;

    Packet* packet = new Packet((uint8_t) Packets::serverSettings);
    packet->Write(client);

    packet->Write(version);
    packet->Write(server->serverUDPSupport);
    packet->Write(server->serverCamSupport);
    packet->Write(server->serverJoyStickSupport);
    packet->Write(server->serverChatSupport);
    packet->Write(server->serverLidarSupport);

    server->SendTCPData(client, packet);
}

void ServerSend::ServerStartUDP(uint8_t client){

    std::cout << "SERVER: [" << (int)client << "] starting udp test" << std::endl;

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

    std::cout << "SERVER: [" << (int)client << "] udp test message" << std::endl;

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

void ServerSend::ServerGetimulatedLidarData(uint8_t client){

    std::cout << "SERVER: [" << (int)client << "] request Lidar Data" << std::endl;

    Packet* packet = new Packet((uint8_t) Packets::serverGetimulatedLidarData);

    server->SendTCPData(client, packet);
}

void ServerSend::ServertSLAMMap(uint8_t client){

    std::cout << "SERVER: [" << (int)client << "] request SLAM Map" << std::endl;

    Packet* packet = new Packet((uint8_t) Packets::servertSLAMMap);

    

    server->SendUDPData(client, packet);
}

void ServerSend::ServerPosition(uint8_t client){

    std::cout << "SERVER: [" << (int)client << "] request Position" << std::endl;

    Packet* packet = new Packet((uint8_t) Packets::serverPosition);

    extern float pos[3];
    packet->Write(pos[0]);
    packet->Write(pos[1]);
    packet->Write(pos[2]);

    server->SendUDPData(client, packet);
}