#include"utility/Vector2.h"
#include"Roboter/Roboter.h"
#include"Lidar/Lidar.h"

int main(int argc, char** argv) {
    
    bool test;
    std::cout << "Test" << std::endl;
    startserial(test);
    std::cout << test << std::endl;
    startLidar(test);
    std::cout << test << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 0; i < 512; i++)
    {
        
        roboterTest();
    }
    std::cout << "Finsih" << std::endl;
    std::cin.get();
}