#pragma once
#include"/usr/include/python3.7/Python.h"

struct RGB
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class LedStripe
{
public:
	LedStripe(uint32_t numLeds);
	~LedStripe();

	void setLed(int p, RGB rgb);
	void update();
	void rainbow(int delay);

private:
	PyObject* pyfunktion1;
	PyObject* pyfunktion2;
	uint32_t numLeds = 0;
	RGB HSVtoRGB(float H, float S, float V);
};


