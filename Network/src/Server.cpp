#include "Server.h"

Server::Server(){ 
    
    tcpServer = new TCPServer(this);
    udpServer = new UDPServer(this);
    serverHandle = new ServerHandle(this);
    serverSend = new ServerSend(this);

    packetHandlers.insert({(uint8_t)Packets::clientConnectionReceived, &ServerHandle::ClientConnectionReceived});
    packetHandlers.insert({(uint8_t)Packets::debugMessage, &ServerHandle::DebugMessage});

    serverState = NetworkState::notConnected;
}

Server::~Server(){
    delete tcpServer;
    delete udpServer;
    delete serverHandle;
    delete serverSend;
}


void Server::StartServer(){
    if (serverState != NetworkState::notConnected) return;

    printf( "SERVER: Starting...\n");
    //std::cout << "SERVER: Starting..." << std::endl;
    serverState = NetworkState::connecting;

    std::thread t1(&TCPServer::StartTCP, tcpServer);
    t1.detach();

    std::thread t2(&UDPServer::StartUDP, udpServer);
    t2.detach();

    std::cout << "SERVER: Started" << std::endl;
    serverState = NetworkState::connected;
}

void Server::ConnectClient(uint8_t client){

    std::cout << "SERVER: Connecting Client " << (int) serverClients[client].id << "..." << std::endl;

    serverClients[client].state = NetworkState::connecting;
    serverSend->ServerConnection(client);
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
    
    (serverHandle->*packetHandlers[packetId])(packetId, packet);
    return client;
}

void Server::AddHeaderToPacket(Packet* packet){
    packet->Write(0, (uint8_t) 1);
    packet->Write(0, (int) packet->Length());
}

void Server::SendTCPData(uint8_t client, Packet* packet){
    AddHeaderToPacket(packet);
    tcpServer->SendTCPData(client, packet->ToArray(), packet->Length());
}
void Server::SendTCPDataToAll(Packet* packet){
    AddHeaderToPacket(packet);

    for (uint8_t i = 0; i < MaxClients; i++)
    {
        tcpServer->SendTCPData(i, packet->ToArray(), packet->Length());
    }
}

void Server::SendUDPData(uint8_t client, Packet* packet){
    AddHeaderToPacket(packet);
}

void Server::SendUDPDataToAll(Packet* packet){
    AddHeaderToPacket(packet);
}

void Server::DisconnectClient(uint8_t client){
    tcpServer->DisconnectTCP(client);
    udpServer->DisconnectUDP(client);
}

void Server::StopServer(){
    std::cout << "SERVER: Stop Server:..." << std::endl;

    for (uint8_t i = 0; i < MaxClients; i++)
    {
        if (serverClients[i].socket != 0){
            DisconnectClient(i);
        }
    }

    tcpServer->StoptTCP();
    udpServer->StopUDP();

    std::cout << "SERVER: Stopped" << std::endl;
}
