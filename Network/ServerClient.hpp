#pragma once

#include "Network.hpp"

struct ServerClient
{
    unsigned char id;
    int socket;
    char receiveBuffer[BUfferSize];
};
