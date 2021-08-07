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

#define MaxFPS 30.0
#define MillisPerSec 1000.0
#define MillisPerFrame MillisPerSec / MaxFPS

float delta;
std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

void logFPS(std::chrono::duration<double, std::milli> work_time) {
    double possibleFPS = MillisPerSec / work_time.count();
    double fps = std::min(possibleFPS, MaxFPS);
    
#ifdef DEBUG
    std::chrono::duration<double, std::milli> sleep_time = b - a;

    //printf("FPS: %.0f Possible: %0.f Work: %.4fms Sleep: %.4fms\n", fps, possibleFPS, work_time.count(), sleep_time.count());
#else

    printf("FPS: %.0f Possible: %.0f\n", fps, possibleFPS);
#endif
}

u_char* createImage(int width, int height, int timestemp){
    int bytesPerPixel = 3;
    u_char* image = new unsigned char[width * height * bytesPerPixel];

    // create a nice color transition (replace with your code)
    for (auto y = 0; y < height; y++){
        for (auto x = 0; x < width; x++)
        {
        // memory location of current pixel
        auto offset = (y * width + x) * bytesPerPixel;

        // red and green fade from 0 to 255, blue is always 127
        image[offset    ] = 255 * x / width;
        image[offset + 1] = 255 * y / height;
        image[offset + 2] = timestemp % 255;
        }
    }
    return image;
}

int i;

void run() {

    u_char* images[255];
    int x = 100;
    int y = 100;
    for (size_t i = 0; i < 255; i++)
    {
        images[i] = createImage(x, y, i);
    }

    while (true) {
        a = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> work_time = a - b;

        if (work_time.count() < MillisPerFrame) {
            delta = MillisPerFrame / MillisPerSec;

            std::chrono::duration<double, std::milli> delta_ms(MillisPerFrame - work_time.count());
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }
        else {
            delta = work_time.count() / MillisPerSec;
        }
        b = std::chrono::system_clock::now();

        logFPS(work_time);
        
        // Loop:
        if (server.serverClients[1].state == NetworkState::connected){
            
            int id = i % 255;
            i++;

            server.serverSend->ServerCamImage(1, images[id], x, y);
        }
    }
}



int main()
{
    signal(SIGINT, signalhandler);

    server.StartServer();

    run();
}

