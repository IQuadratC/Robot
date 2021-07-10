#include "TCPServer.h"

#include <stdio.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

struct sockaddr_in address;
int addrlen;
char buffer[BUfferSize]{0};

TCPServer::TCPServer(Server *server) : server(server)
{
    StartTCP();
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
}

void TCPServer::mainLoop()
{
    while (running)
    {
        FD_ZERO(&readfds);
        FD_SET(tcpListener, &readfds);
        for (int i = 0; i < MaxClients; i++)
        {
            sd = server->serverClient[i].socket;

            if (sd > 0) FD_SET(sd, &readfds);

            if (sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) printf("select error");

        if (FD_ISSET(master_socket, &readfds))


    }
}

void TCPServer::SendTCPData(int id, unsigned char *data, std::size_t length)
{
}
