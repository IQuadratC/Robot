#pragma once
#include<iostream>
class LedStripe
{

public:
    LedStripe(int numLeds);
    ~LedStripe();

    void setLed(uint32_t pos, RGB rgb);
    void rainbow(int delay);

private:
    int numLeds = 0;


};


