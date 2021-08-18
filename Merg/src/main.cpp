#include <cassert>
#include <cstdio>
#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>
#include "Network/Server.h"
#include "Robot/Roboter.h"
#include "Robot/LedStripe.h"
#include "Log.h"
#include "SLAM/slam.h"

Server server;
Roboter roboter;
void signalhandler(int sig)
{
    signal(sig, SIG_IGN);
    server.StopServer();
    roboter.stop();
    exit(0);
}

int main()
{
    using namespace std::chrono_literals;
    signal(SIGINT, signalhandler);
    Log::Init();
    Log::GetLogger()->warn("Initialized Log!");

    LedStripe ledstripe(102);
    server.StartServer();
    std::thread robot(&Roboter::start,&roboter);
    robot.detach();
    std::this_thread::sleep_for(100ms);

    RunSLAM();
}
