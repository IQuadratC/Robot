#include "slam.h"
#include "Log.h"
#include "../Network/Network.h"
#include "../Network/ServerSend.h"
#include "slamMap.h"
#include "slamMath.h"

#include <thread>
#include <chrono>
#include <iostream>

float* lidarDataPolar;
float pos[3];

std::shared_ptr<spdlog::logger> Logger;

bool writeToMap = true;
bool newLidarData = false;

void RunSLAM(Server* server)
{
    Logger = Log::GetLidarLogger();

    InitMap();

    while (true)
    {
        if (server->serverClients[1].state == NetworkState::connected && server->serverClients[1].clientLidarSimSupport){
            server->serverSend->ServerGetSimulatedLidarData(1);
            server->serverSend->ServertSLAMMap(1, GetMap(0), MAPSIZE * MAPSIZE);
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void LidarData(float* data)
{
    lidarDataPolar = data;
    newLidarData = true;

    if (writeToMap) {
        for (size_t i = 0; i < 360; i++)
        {
            glm::vec2 pos = polarToCart(i, lidarDataPolar[i]) + glm::vec2(MAPSIZE/2, MAPSIZE/2);
            SetMap(pos, 0, 1);
        }
    }
}

