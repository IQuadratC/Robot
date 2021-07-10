#pragma once

class Server;

class ServerHandle{


public:
    ServerHandle(Server* server);
    ~ServerHandle();



private:
    Server* server;

};

