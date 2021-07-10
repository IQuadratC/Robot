#include "Server.h"
#include <thread>


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
    
    std::thread threadTCP();
    std::thread threadUDP();

}

void Server::ConnectClient(){

}

void Server::HandelData(){

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

void Server::DisconnectClient(){

}


void Server::StopServer(){
    
}
