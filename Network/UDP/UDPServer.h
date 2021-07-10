#pragma once

#include "../Network.hpp"
#include "../Server/Server.h"

class UDPServer{

public:
    UDPServer(Server* server);
    ~UDPServer();

private:
    Server* server;
};