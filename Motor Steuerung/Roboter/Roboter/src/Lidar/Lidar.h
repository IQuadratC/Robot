#pragma once
#ifdef __arm__
#include "/home/pi/Header/rplidar.h"
#else
#include "rplidar.h"
#endif
#include <iostream>
#include <map>

using namespace rp::standalone::rplidar;

void startLidar(bool& lidarStart);

void getLidarData(std::map<int, int>& lidardata);
