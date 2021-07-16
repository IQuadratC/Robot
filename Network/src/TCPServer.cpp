#include "TCPServer.h"
#include "Server.h"


TCPServer::TCPServer(Server *server) : server(server)
{
    
}
TCPServer::~TCPServer(){

}


void TCPServer::StartTCP()
{
    int opt = 1;

    if ((tcpListener = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "SERVER: socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(tcpListener, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cout << "SERVER: setsockopt" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(tcpListener, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "SERVER: bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(tcpListener, MaxClients) < 0)
    {
        std::cout << "SERVER: listen" << std::endl;
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

            std::cout << "SERVER: select error" << std::endl;

        if (FD_ISSET(tcpListener, &readfds))
        {

            if ((new_socket = accept(tcpListener, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                //TODO: Error
            }

            std::cout << "SERVER: Incoming connection from " << inet_ntoa(address.sin_addr) << std::endl;

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
    
    std::cout << "Client disconnected , ip " << inet_ntoa(address.sin_addr) << ", port " << ntohs(address.sin_port) << std::endl;

    //Close the socket and mark as 0 in list for reuse
    close(server->serverClients[client].socket);
}

void TCPServer::StoptTCP(){
    
    close(tcpListener);
}