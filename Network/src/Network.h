#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <functional>

#define BufferSize 4096
#define PORT 11001
#define MaxClients 255

enum Packets{
    debugMessage = 1,

    serverSettings = 2,  
    clientSettings = 3,
    serverStartUDP = 4,
    clientUDPConnection = 5,
    serverUDPConnection = 6,
    clientUDPConnectionStatus = 7,

    serverCamImage = 8,
};

enum NetworkState{
    notConnected = 1,        
    connected = 2,
    connecting = 3,
    disconnecting = 4
};

// netstat -np
