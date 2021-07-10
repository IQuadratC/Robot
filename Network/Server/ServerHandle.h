#pragma once
#include"Server.h"

class ServerHandle{


public:
    ServerHandle(Server* server);
    ~ServerHandle();



private:
    Server* server;

};

