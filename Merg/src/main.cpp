#include <cassert>
#include <cstdio>
#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>
#include "Network/Server.h"
#include "Robot/Roboter.h"
#include "Robot/LedStripe.h"
#include "SLAM/slam.h"
#include "Log.h"

Server server;
Roboter roboter;
LedStripe ledstripe(102);

void signalhandler(int sig)
{
    signal(sig, SIG_IGN);
    Log::GetLogger()->warn("Shutdown Program");
    server.StopServer();
    roboter.Stop();
    ledstripe.Stop();
    exit(0);
}

int main()
{
    signal(SIGINT, signalhandler);
    server.StartServer();
    Log::GetLogger()->info("Start Robot!");
    roboter.Start();
    Log::GetLogger()->info("Start LedStripe");
    ledstripe.Start();
    ledstripe.Do_command(commands_led::setArray);
    Log::GetLogger()->info("Start SLAM");
    RunSLAM(&server);
}
