#include "Server.h"
#include <thread>

void ClientConnectionReceived(uint8_t,Packet*){
    //TODO Log
}
void DebugMessage(uint8_t,Packet*){
    //TODO Log
}


Server::Server(){ 
    
    tcpServer = new TCPServer(this);
    udpServer = new UDPServer(this);

    serverhandle = new ServerHandle(this);
    serversend = new ServerSend(this);

    packetHandlers.insert({(uint8_t)Packets::clientConnectionReceived,ClientConnectionReceived});
    packetHandlers.insert({(uint8_t)Packets::debugMessage,DebugMessage});

}


Server::~Server(){
    delete tcpServer;
    delete udpServer;
    delete serversend;
    delete serverhandle;

}


void Server::StartServer(){
    
    tcpServer->StartThread();

}

void Server::ConnectClient(){

}

uint8_t Server::HandelData(uint8_t data[], size_t lengtharray, uint8_t client){
    
    Packet* packet = new Packet(data, lengtharray);
    
    int length = packet->ReadInt32();
    if (length + 4 != lengtharray)
    {
        //TODO Error
        return 0;
    }
    uint8_t clientId = packet->ReadByte();

    if (clientId == 0 || client != 0 && serverClients[client].id != clientId)
    {
        //TODO Error
        return 0;
    } 
    if(clientId != 0 && client == 0)
    {
        client = serverClients[clientId].id;
    }
    
    uint8_t packetId = packet->ReadByte();
    
    packetHandlers[packetId](client, packet);
    return client;
    
}

void Server::AddHeaderToPacket(){

}

void Server::SendTCPData(){

}
void Server::SendTCPDataToAll(){

}

void Server::SendUDPData(){

}
void Server::SendUDPDataToAll(){

}

void Server::DisconnectClient(uint8_t client){
    tcpServer->DisconnectTCP(client);
}


void Server::StopServer(){
    
}
