#include"utility/Vector2.h"
#include"Roboter/Roboter.h"
#include"Lidar/Lidar.h"
#include"Led/LedStripe.h"
#include"iostream"
#include<cstdio>
#include<chrono>

int main(int argc, char** argv) {




    LedStripe test;
    test.setLed(0, 255, 0, 0);
    int index = 60;
    
    while (true)
    {
        int blue = 0;
        for (int i = 1; i < index; i++)
        {
            test.setLed(i, 10, 0, blue);
            test.setLed(i - 1,0, 0, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            blue++;
        }
        index--;
        std::cout << "test" << std::endl;
    }
        
 
    std::cin.get();
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