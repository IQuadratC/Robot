#include "LedStripe.h"
#include"chrono"
#include"thread"
#include<iostream>
const char* moduleName = "ledPy";
const char* funktionName1 = "setPixel";
const char* funktionName2 = "ledupdate";
const char* importPath = "/home/pi/Python";

LedStripe::LedStripe(uint32_t numLeds) : numLeds(numLeds)
{
	Py_Initialize();

	PyObject* pathList = PySys_GetObject("path");

	PyObject* pathname = PyUnicode_DecodeFSDefault(importPath);

	PyList_Append(pathList, pathname);


	PyObject* pyModuleName = PyUnicode_DecodeFSDefault(moduleName);


	PyObject* pyModule = PyImport_Import(pyModuleName);

	pyfunktion1 = PyObject_GetAttrString(pyModule, funktionName1);
	Py_DECREF(pyfunktion1);
    pyfunktion2 = PyObject_GetAttrString(pyModule, funktionName2);
    Py_DECREF(pyfunktion2);
}

LedStripe::~LedStripe()
{
	Py_FinalizeEx();
}

void LedStripe::setLed(int p, RGB rgb)
{
	PyObject* pyFunktionArgs = PyTuple_New(4);
	PyTuple_SetItem(pyFunktionArgs, 0, PyLong_FromLong(p));
	PyTuple_SetItem(pyFunktionArgs, 1, PyLong_FromLong(rgb.r));
	PyTuple_SetItem(pyFunktionArgs, 2, PyLong_FromLong(rgb.g));
	PyTuple_SetItem(pyFunktionArgs, 3, PyLong_FromLong(rgb.b));

	PyObject_CallObject(pyfunktion1, pyFunktionArgs);

}

void LedStripe::update()
{
    PyObject* pyFunktionArgs = PyTuple_New(0);
    PyObject_CallObject(pyfunktion2, pyFunktionArgs);
}

void LedStripe::rainbow(int delay)
{
    for (int j = 0; j < 255; j+=2)
    {
        for (int i = 0; i < 100; i++)
        {
            setLed(i,  HSVtoRGB(i - (j * 1) & 255,(float)100,(float)100));
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        update();
    }
}


RGB LedStripe::HSVtoRGB(float H, float S, float V)
{

    if (H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0)
    {
        return { 0,0,0 };
    }
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(std::fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60)
    {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120)
    {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180)
    {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240)
    {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300)
    {
        r = X, g = 0, b = C;
    }
    else
    {
        r = C, g = 0, b = X;
    }

    uint8_t R = (r + m) * 255;
    uint8_t G = (g + m) * 255;
    uint8_t B = (b + m) * 255;

    return { R, G, B };
}
