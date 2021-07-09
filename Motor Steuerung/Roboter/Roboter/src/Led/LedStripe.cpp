#include "LedStripe.h"

const char* moduleName = "ledPy";
const char* funktionName = "setPixel";
const char* importPath = "/home/pi/Python";

LedStripe::LedStripe()
{
	Py_Initialize();

	PyObject* pathList = PySys_GetObject("path");

	PyObject* pathname = PyUnicode_DecodeFSDefault(importPath);

	PyList_Append(pathList, pathname);


	PyObject* pyModuleName = PyUnicode_DecodeFSDefault(moduleName);


	PyObject* pyModule = PyImport_Import(pyModuleName);

	pyfunktion = PyObject_GetAttrString(pyModule, funktionName);
	Py_DECREF(pyfunktion);
}
LedStripe::~LedStripe()
{
	Py_FinalizeEx();
}

void LedStripe::setLed(int p, uint8_t r, uint8_t g, uint8_t b)
{
	PyObject* pyFunktionArgs = PyTuple_New(4);
	PyTuple_SetItem(pyFunktionArgs, 0, PyLong_FromLong(p));
	PyTuple_SetItem(pyFunktionArgs, 1, PyLong_FromLong(r));
	PyTuple_SetItem(pyFunktionArgs, 2, PyLong_FromLong(g));
	PyTuple_SetItem(pyFunktionArgs, 3, PyLong_FromLong(b));

	PyObject_CallObject(pyfunktion, pyFunktionArgs);

}

void LedStripe::rainbow(int delay)
{
    for (int j = 0; j < 255; j++)
    {
        for (int i = 0; i < numLeds; i++)
        {
            setLed(i, HSVtoRGB(i - (j * 2), (float)100, (float)100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

RGB HSVtoRGB(float H, float S, float V)
{

    if (H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0)
    {
        return;
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
