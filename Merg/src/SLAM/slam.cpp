#include "slam.h"
#include "Log.h"
#include "../Network/Network.h"
#include "../Network/ServerSend.h"

#include <thread>
#include <chrono>
#include <iostream>

float* lidarDataPolar;
float pos[3];

std::shared_ptr<spdlog::logger> Logger;

void RunSLAM(Server* server)
{
    Logger = Log::GetLidarLogger();

    while (true)
    {
        if (server->serverClients[1].state == NetworkState::connected && server->serverClients[1].clientLidarSimSupport){
            server->serverSend->ServerGetSimulatedLidarData(1);
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void LidarData(float* data)
{
    lidarDataPolar = data;
}

