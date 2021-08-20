#include "slam.h"

#include <thread>
#include <chrono>
#include <iostream>
float* lidarDataPolar;
float pos[3];
//uint8_t* 

void RunSLAM()
{
    while (true)
    {
        std::cout << "Test" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void LidarData(float* data)
{
    lidarDataPolar = data;
}