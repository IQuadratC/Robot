#include "TCPServer.h"
#include "Server.h"


TCPServer::TCPServer(Server *server) : server(server)
{
    Logger = Log::GetNetworkLogger();
}
TCPServer::~TCPServer(){

}


void TCPServer::StartTCP()
{
    int opt = 1;

    if ((tcpListener = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        Logger->error("SERVER: TCP socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(tcpListener, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        Logger->error("SERVER: TCP setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(tcpListener, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        Logger->error("SERVER: TCP bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(tcpListener, MaxClients) < 0)
    {
        Logger->info("SERVER: listen");
        exit(EXIT_FAILURE);
        
    }

    addrlen = sizeof(address);

    mainLoop();
}

void TCPServer::mainLoop()
{
    while (server->serverState == NetworkState::connected || server->serverState == NetworkState::connecting)
    {
        FD_ZERO(&readfds);
        FD_SET(tcpListener, &readfds);
        max_sd = tcpListener;

        for (int i = 1; i < MaxClients; i++)
        {
            sd = server->serverClients[i].socket;

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))

            Logger->error("SERVER: select error");

        if (FD_ISSET(tcpListener, &readfds))
        {

            if ((new_socket = accept(tcpListener, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                //TODO: Error
            }

            Logger->info("SERVER: Incoming connection from {0}", inet_ntoa(address.sin_addr));

            for (uint8_t i = 1; i < MaxClients; i++)
            {
                if (server->serverClients[i].socket == 0)
                {
                    server->serverClients[i].id = i;
                    server->serverClients[i].socket = new_socket;
                    server->serverClients[i].ip = inet_ntoa(address.sin_addr);

                    server->ConnectClient(i);
                    break;
                }
            }
        }

        for (int i = 1; i < MaxClients; i++)
        {
            if (FD_ISSET(server->serverClients[i].socket, &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read(server->serverClients[i].socket, 
                                    server->serverClients[i].receiveBuffer, 
                                    BufferSize)) <= 0)
                {
                    server->DisconnectClient(i);
                } else {
                    uint8_t data[valread];
                    memcpy(data,server->serverClients[i].receiveBuffer,valread);
                    server->HandelData(data, valread, i);
                }
            }
        }
    }
}

void TCPServer::SendTCPData(uint8_t client, unsigned char *data, std::size_t length)
{
    if(send(server->serverClients[client].socket,data,length,0) < 0){
        
    }
}

void TCPServer::DisconnectTCP(uint8_t client){
    getpeername(server->serverClients[client].socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    
    Logger->info("Client disconnected, ip {0}, {1}", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    //Close the socket and mark as 0 in list for reuse
    close(server->serverClients[client].socket);
}

void TCPServer::StoptTCP(){
    
    close(tcpListener);
}