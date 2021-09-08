#include "Server.h"

Server::Server(){ 
    Log::Init();
    tcpServer = new TCPServer(this);
    udpServer = new UDPServer(this);
    serverHandle = new ServerHandle(this);
    serverSend = new ServerSend(this);

    packetHandlers.insert({(uint8_t)Packets::debugMessage, &ServerHandle::DebugMessage});
    packetHandlers.insert({(uint8_t)Packets::clientSettings, &ServerHandle::ClientSettings});
    packetHandlers.insert({(uint8_t)Packets::clientUDPConnection, &ServerHandle::ClientUDPConnection});
    packetHandlers.insert({(uint8_t)Packets::clientUDPConnectionStatus, &ServerHandle::ClientUDPConnectionStatus});

    packetHandlers.insert({(uint8_t)Packets::clientGetSLAMMap, &ServerHandle::ClientGetSLAMMap});
    packetHandlers.insert({(uint8_t)Packets::clientGetPosition, &ServerHandle::ClientGetPosition});
    packetHandlers.insert({(uint8_t)Packets::clientSimulatedLidarData, &ServerHandle::ClientSimulatedLidarData});

    packetHandlers.insert({(uint8_t)Packets::clientControllMode, &ServerHandle::ClientGetSLAMMap});
    packetHandlers.insert({(uint8_t)Packets::clientJoystickMove, &ServerHandle::ClientGetSLAMMap});
    packetHandlers.insert({(uint8_t)Packets::clientJoystickRotate, &ServerHandle::ClientGetPosition});
    packetHandlers.insert({(uint8_t)Packets::clientJoystickStop, &ServerHandle::ClientSimulatedLidarData});

    packetHandlers.insert({(uint8_t)Packets::clientSimulatedLidarData, &ServerHandle::ClientSimulatedLidarData});

    serverState = NetworkState::notConnected;
}

Server::~Server(){
    delete tcpServer;
    delete udpServer;
    delete serverHandle;
    delete serverSend;
}


void Server::StartServer(){

    Logger = Log::GetNetworkLogger();

    if (serverState != NetworkState::notConnected) return;

    Logger->info("Starting...");
    serverState = NetworkState::connecting;

    std::thread t1(&TCPServer::StartTCP, tcpServer);
    t1.detach();

    if (serverUDPSupport){
        std::thread t2(&UDPServer::StartUDP, udpServer);
        t2.detach();
    }

    Logger->info("Started");
    serverState = NetworkState::connected;
}

void Server::ConnectClient(uint8_t client){

    Logger->info("Connecting Client {0}...",(int) serverClients[client].id);
    serverClients[client].state = NetworkState::connecting;
    serverSend->ServerSettings(client);
}

void Server::HandelData(uint8_t data[], size_t lengtharray, uint8_t client){
    
    Packet* packet = new Packet(data, lengtharray);
    
    int length = packet->ReadInt32();
    if (length + 4 != lengtharray)
    {
        //TODO Error
        return;
    }
    uint8_t clientId = packet->ReadByte();

    if (clientId == 0 || client != 0 && serverClients[client].id != clientId)
    {
        //TODO Error
        return;
    } 
    if(clientId != 0 && client == 0)
    {
        client = serverClients[clientId].id;
    }
    
    uint8_t packetId = packet->ReadByte();
    
    (serverHandle->*packetHandlers[packetId])(clientId, packet);
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
    for (uint8_t i = 0; i < MaxClients; i++)
    {
        if (serverClients[i].id != 0){
            SendTCPData(i, packet);
        }
    }
}

void Server::SendUDPData(uint8_t client, Packet* packet){
    if (!serverUDPSupport || !serverClients[client].clientUDPSupport ){
        SendTCPData(client, packet);
        return;
    }

    AddHeaderToPacket(packet);
    udpServer->SendUDPData(client, packet->ToArray(), packet->Length());
}

void Server::SendUDPDataToAll(Packet* packet){
    for (uint8_t i = 0; i < MaxClients; i++)
    {
        if (serverClients[i].id != 0){
           SendUDPData(i, packet);
        }
    }
}

void Server::DisconnectClient(uint8_t client){
    tcpServer->DisconnectTCP(client);
    udpServer->DisconnectUDP(client);
}

void Server::StopServer(){

    Logger->info("Stopping...");
    for (uint8_t i = 0; i < MaxClients; i++)
    {
        if (serverClients[i].socket != 0){
            DisconnectClient(i);
        }
    }

    tcpServer->StoptTCP();
    udpServer->StopUDP();

    Logger->info("Stopped");
}
