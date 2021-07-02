#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <thread>
#include <future>
#include <chrono>
#include <ostream>
#include"../Roboter/Roboter.h"
#include"../utility/Vector2.h"

#define delay 100

struct ClientData {
	std::string ip;
	int permission = 0;
};

struct StringData
{
	std::string command;
	std::string args;
};

void TCPServer(bool& lidarstart);