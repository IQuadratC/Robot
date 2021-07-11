#include <cstdio>
#include<iostream>
#include<signal.h>
#include "Server.h"

    
Server server;
void  signalhandler(int sig)
{
	signal(sig, SIG_IGN);

    server.StopServer();

	exit(0);
}

int main()
{
    signal(SIGINT, signalhandler);

    server.StartServer();

    while (true)
    {
        /* code */
    }
}