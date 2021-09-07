#pragma once
#include <python3.9/Python.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <condition_variable>

struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
enum commands_led
{
    setArray,
    turnOFF,
    animation,
};

class LedStripe
{

public:
    LedStripe(uint32_t numLeds);
    ~LedStripe();

    struct LedData{
        RGB pixelData[102];
        int animationindex = 0;
    }data;

    void Start();
    void Stop();
    void Do_command(commands_led commands);

private:
    PyObject *pyfunktion1 = nullptr;
    PyObject *pyfunktion2 = nullptr;

    uint32_t numLeds = 0;

    bool ready = false;
    bool stop = false;

    struct Flags
    {
        bool setArray : 1;
        bool trunOFF : 1;
        bool animation : 1;
    };

    Flags flags = {false, false, false};
    Flags thread_flags = {false, false, false};

    LedData thread_data;
    
    void MainLoop();
    void setLed(int p, RGB rgb);
    void setLedArray(RGB *data);
    void update();
    void rainbow(int delay);
    void light(int delay);
    void ReadAnimationData();
    void PlayAnimation(int index);

    RGB HSVtoRGB(float h, float s, float v);

    int filecounter = 0;

    std::mutex mutex;
    std::condition_variable cv;

    std::unordered_map<int, std::vector<RGB*>> animtions;
};
