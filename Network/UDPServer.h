#pragma once

#include "Network.hpp"

class Server;

class UDPServer{

public:
    UDPServer(Server* server);
    ~UDPServer();

private:
    Server* server;
};