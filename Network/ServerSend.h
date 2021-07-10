#pragma once

class Server;

class ServerSend {

public:
    ServerSend(Server* server);
    ~ServerSend();

private:
    Server* server;
};