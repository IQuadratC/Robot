#include"ServerSend.h"
#include "Server.h"

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

    std::cout << "SERVER: [" << (int)client << "] sending settings" << std::endl;

    Packet* packet = new Packet((uint8_t) Packets::serverSettings);
    packet->Write(client);
    packet->Write(server->serverClients[client].clientUDPSupport);
    server->SendTCPData(client, packet);

}

void ServerSend::ServerStartUDP(uint8_t client){

    std::cout << "SERVER: [" << (int)client << "] starting udp test" << std::endl;

    Packet* packet = new Packet((uint8_t) Packets::serverStartUDP);
    server->SendTCPData(client, packet);
    /* TODO
    Threader.RunAsync(() =>
    {
        Thread.Sleep(2000);
        if (client.updConnected) {return;}
        ServerUDPConnection(client, false);
    });
    */
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

Packet* packet;
void camJepgOutput(unsigned char byte)
{
    packet->Write(byte);
}

void ServerSend::ServerCamImage(uint8_t client, u_char* data, int width, int height){

    packet = new Packet((uint8_t) Packets::serverCamImage);

    auto ok = TooJpeg::writeJpeg(camJepgOutput, data, width, height, true, 90, false, 0);

    server->SendUDPData(client, packet);
}