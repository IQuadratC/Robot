#pragma once
#include <python3.6/Python.h>
#include <unordered_map>
#include <vector>
struct RGB
{
    float r;
    float g;
    float b;
};

enum commands_led
{
    setLed,
    setArray,
    turnOFF,
};

class LedStripe
{
private:
public:
    LedStripe(uint32_t numLeds);
    ~LedStripe();

    RGB pixelData[102];

    void start();
    void do_command(commands_led commands);

private:
    struct RGB_DATA
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    PyObject *pyfunktion1 = nullptr;
    PyObject *pyfunktion2 = nullptr;
    uint32_t numLeds = 0;

    struct Flags
    {
        bool setLed : 1;
        bool setArray : 1;
        bool rainbow : 1;
        bool trunOFF : 1;
    };
    Flags flags = {false, false, false, false};
    Flags thread_flags = {false, false, false, false};

    RGB HSVtoRGB(float h, float s, float v);

    void setLed(int p, RGB rgb);
    void update();
    void rainbow(int delay);
    void light(int delay);
    void ReadAnimationData();

    int filecounter = 0;

    std::unordered_map<int, std::vector<RGB_DATA *>> animtions;
};
