#include "TCPServer.h"

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <thread>
#include <future>
#include "Server.h"

struct sockaddr_in address;
int addrlen;
char buffer[BufferSize]{0};

TCPServer::TCPServer(Server *server) : server(server)
{
    
}

TCPServer::~TCPServer()
{
}

void TCPServer::StartTCP()
{

    int opt = 1;

    if ((tcpListener = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(tcpListener, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(tcpListener, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(tcpListener, MaxClients) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);

    mainLoop();
}

void TCPServer::mainLoop()
{
    while (running)
    {
        std::cout << "Test" << std::endl;
        FD_ZERO(&readfds);
        FD_SET(tcpListener, &readfds);
        for (int i = 0; i < MaxClients; i++)
        {
            sd = server->serverClients[i].socket;

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
            printf("select error");

        if (FD_ISSET(tcpListener, &readfds))
        {

            if ((new_socket = accept(tcpListener, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                //TODO: Error
            }

            if (send(new_socket, connectionMessage.c_str(), connectionMessage.size(), 0) != connectionMessage.size())
            {
                //TODO: Error
            }
            for (int i = 1; i < MaxClients; i++)
            {
                if (server->serverClients[i].socket == 0)
                {
                    server->serverClients[i].socket = new_socket;
                    server->serverClients[i].ip = inet_ntoa(address.sin_addr);
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
            for (int i = 0; i < MaxClients; i++)
			{
				if (FD_ISSET(sd, &readfds))
				{
					//Check if it was for closing , and also read the incoming message
					if ((valread = read(server->serverClients[i].socket, 
                                        server->serverClients[i].receiveBuffer, 
                                        BufferSize)) <= 0)
					{
                        server->DisconnectClient(i);
					} else {
                        uint8_t data[valread];
                        memccpy(data,server->serverClients[i].receiveBuffer,0,valread);
                        server->HandelData(data, valread, i);
                    }
                }
            }
        }
    }
}

void TCPServer::SendTCPData(int id, unsigned char *data, std::size_t length)
{
    if(send(server->serverClients[id].socket,data,length,0) < 0){
        
    }
}

void  TCPServer::DisconnectTCP(uint8_t client){
    getpeername(server->serverClients[client].socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("Client disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    //Close the socket and mark as 0 in list for reuse
    close(server->serverClients[client].socket);
    
}

void TCPServer::StartThread(){

   std::thread test(StartTCP);
}
