#pragma once
#include "Server.h"

class ServerSend {

public:
    ServerSend(Server* server);
    ~ServerSend();

private:
    Server* server;
};