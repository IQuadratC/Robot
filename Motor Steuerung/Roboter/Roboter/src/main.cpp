#include"utility/Vector2.h"
#include"Roboter/Roboter.h"
int main(int argc, char** argv) {
    
    bool test;
    std::cout << "Test" << std::endl;
    startserial(test);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 0; i < 512; i++)
    {
        std::cout << test << std::endl;
        roboterTest();
    }
    std::cout << "Finsih" << std::endl;
    std::cin.get();
}