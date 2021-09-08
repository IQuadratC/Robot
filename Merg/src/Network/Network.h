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
#include <sstream>
#include <future>
#include <chrono>

#include "../Log.h"

#define BufferSize 4096
#define PORT 11001
#define MaxClients 255

enum Packets{

// 1 - 3 Base
debugMessage = 1,
serverSettings = 2,  
clientSettings = 3,

// 4 - 9 UDP
serverStartUDP = 4,
clientUDPConnection = 5,
serverUDPConnection = 6,
clientUDPConnectionStatus = 7,

// 10 - 29 Movement
clientControllMode = 10, // int: 1 = no control, 2 = Joystick, 3 = AI

clientJoystickMove = 11, // dir norm Vec2, speed float
clientJoystickRotate = 12, // speed float, pos = right, neg = left
clientJoystickStop = 13, 

clientMoveAI = 21,

// 30 - 39 Cam
serverCamMode = 31, // int  1 = on 2 = off

// 40 - 49 Chat
chatMessage = 40,

// 50 - 59 Lidar
clientLidarMode = 50,  // Client requset to start or stop Lidar sensor int 1 = on,  2 = off
serverLidarStatus = 51, // Status of the Lidar 1 = on, 2 = off

clientGetSLAMMap = 52,
servertSLAMMap = 53,

clientGetPosition = 54,
serverPosition = 55,

// Lidar Sim 60 - 69
serverGetSimulatedLidarData = 60,
clientSimulatedLidarData = 61,

// LedStripe 70 - 79

};

enum NetworkState{
    notConnected = 1,        
    connected = 2,
    connecting = 3,
    disconnecting = 4
};