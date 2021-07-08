#include"utility/Vector2.h"
#include"Roboter/Roboter.h"
#include"Lidar/Lidar.h"
#include"Led/lib/ws2812-rpi.h"
#include"iostream"
#include<cstdio>

int main(int argc, char** argv) {

    std::cout << "test" << std::endl;
    NeoPixel* n = new NeoPixel(24);
    std::cout << "Test2" << std::endl;
    while (true) {
        std::cout << "Test" << std::endl;
        n->rainbow(5);
        printf("hallo");
    }
    delete n;

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
}