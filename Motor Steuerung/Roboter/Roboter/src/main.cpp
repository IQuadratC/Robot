#include"utility/Vector2.h"
#include"Roboter/Roboter.h"
#include"Lidar/Lidar.h"
#include"Led/LedStripe.h"
#include"Logger/Log.h"

#include<iostream>
#include<cstdio>
#include<chrono>

int main(int argc, char** argv) {

    Log::Init();
    Log::GetLogger()->warn("Initialized Log!");

    LedStripe led(50);
    bool test;
    startserial(test);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    moveRobot(2.5, 10, 10);

    /*while (true)
    {
        led.rainbow(1);
        Log::GetLogger()->info("Test");
        char senddata[15];
        memset(&senddata, 255, 15);
        std::stringstream ss;
        for (int i = 0; i < 15; i++) {
            ss << std::hex << (short)senddata[i] << std::dec << " | ";
        }
        ROBOTER_LOG_INFO(ss.str().c_str());
    }*/

}
    /*ol test;
    std::cout << "Test" << std::endl;
    startserial(test);
    std::cout << test << std::endl;
    //startLidar(test);
    std::cout << test << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 0; i < 128; i++)
    {
        roboterTest();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        roboterReadData();

    }
    std::cout << "Finsih" << std::endl;*/