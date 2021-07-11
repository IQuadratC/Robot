#include"ServerSend.h"
#include "Server.h"

ServerSend::ServerSend(Server *server) : server(server)
{
    
}
ServerSend::~ServerSend(){

}

void ServerSend::ServerConnection(uint8_t client){

    std::cout << "SERVER: Sending ID to Client " << (int) server->serverClients[client].id << std::endl;

    Packet* packet = new Packet((uint8_t) Packets::serverConnection);
    packet->Write(client);
    server->SendTCPData(client, packet);

}

void ServerSend::DebugMessage(std::string message){

    Packet* packet = new Packet((uint8_t) Packets::debugMessage);
    packet->Write(message);
    server->SendTCPDataToAll(packet);

}