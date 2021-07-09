#pragma once
class LedStripe
{
public:
	LedStripe();
	~LedStripe();

	void setLed(int p, uint8_t r, uint8_t g, uint8_t b);
private:
	PyObject* pyfunktion;
};


