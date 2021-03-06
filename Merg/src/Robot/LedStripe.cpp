#include "LedStripe.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include "../Log.h"

const char *moduleName = "ledStripe";
const char *funktionName1 = "setPixel";
const char *funktionName2 = "ledupdate";
const char *importPath = "./Python";

namespace fs = std::experimental::filesystem;

void printPyErrorIfExists()
{
    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
}

LedStripe::LedStripe(uint32_t numLeds) : numLeds(numLeds)
{
    ReadAnimationData();

    Py_Initialize();
    PyObject *pathList = PySys_GetObject("path");
    PyObject *pathname = PyUnicode_DecodeFSDefault(importPath);
    PyList_Append(pathList, pathname);
    PyObject *pyModuleName = PyUnicode_DecodeFSDefault(moduleName);
    PyObject *pyModule = PyImport_Import(pyModuleName);

    if (!pyModule)
        printPyErrorIfExists();

    pyfunktion1 = PyObject_GetAttrString(pyModule, funktionName1);
    Py_DECREF(pyfunktion1);
    pyfunktion2 = PyObject_GetAttrString(pyModule, funktionName2);
    Py_DECREF(pyfunktion2);
}

LedStripe::~LedStripe()
{
    Py_FinalizeEx();
}

void LedStripe::Start()
{
    std::thread ledStripe(&LedStripe::MainLoop, this);
    ledStripe.detach();
}

void LedStripe::Stop()
{
    ready = true;
    stop = true;
    cv.notify_one();
}

void LedStripe::MainLoop()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mutex);
        cv.wait(lk, [&](){ return ready || stop; });
        if (stop) break;
        thread_flags = flags;
        thread_data = data;
        flags = {false,false,false};
        ready = false;
        lk.unlock();

        if (thread_flags.setArray)
        {
            setLedArray(thread_data.pixelData);
            thread_flags.setArray = false;
        }
        if (thread_flags.animation)
        {
            PlayAnimation(thread_data.animationindex);
            thread_flags.animation = false;
        }
    }
}

void LedStripe::Do_command(commands_led commands)
{
    ready = true;
	std::unique_lock<std::mutex> lk(mutex);
	switch (commands)
    {
    case commands_led::setArray:
        flags.setArray = true;
        break;
    default:
        break;
    }
	lk.unlock();
	cv.notify_one();
}

void LedStripe::setLed(int p, RGB rgb)
{
    PyObject *pyFunktionArgs = PyTuple_New(4);
    PyTuple_SetItem(pyFunktionArgs, 0, PyLong_FromLong(p));
    PyTuple_SetItem(pyFunktionArgs, 1, PyLong_FromLong(rgb.r));
    PyTuple_SetItem(pyFunktionArgs, 2, PyLong_FromLong(rgb.g));
    PyTuple_SetItem(pyFunktionArgs, 3, PyLong_FromLong(rgb.b));
    PyObject_CallObject(pyfunktion1, pyFunktionArgs);
}

void LedStripe::setLedArray(RGB *data)
{

    for (int i = 0; i < numLeds; i++)
        setLed(i, data[i]);
}

void LedStripe::update()
{
    PyObject *pyFunktionArgs = PyTuple_New(0);
    PyObject_CallObject(pyfunktion2, pyFunktionArgs);
}

void LedStripe::rainbow(int delay)
{
    for (int j = 0; j < 360; j++)
    {
        for (int i = 0; i < numLeds; i++)
        {
            int hue = i - j;
            if (hue < 0)
                hue += 360;
            setLed(i, HSVtoRGB(hue, 1.f, 0.50f));
        }
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void LedStripe::light(int delay)
{

    for (int i = 0; i < numLeds; i++)
    {
        setLed(i, {255, 255, 255});
    }
    update();

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    for (int i = 0; i < numLeds; i++)
    {
        setLed(i, {0, 0, 0});
    }
    update();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

RGB LedStripe::HSVtoRGB(float h, float s, float v)
{
    if (h > 360 || h < 0 || s > 1 || s < 0 || v > 1 || v < 0)
    {
        return {0, 0, 0};
    }

    int i = h / 60;
    float C = v * s;
    float X = C * (1 - abs(std::fmod(h / 60.0, 2) - 1));
    float m = v - C;
    struct
    {
        float r, g, b;
    } rgb;

    switch (i)
    {
    case 0:
        rgb = {C, X, 0};
        break;
    case 1:
        rgb = {X, C, 0};
        break;
    case 2:
        rgb = {0, C, X};
        break;
    case 3:
        rgb = {0, X, C};
        break;
    case 4:
        rgb = {X, 0, C};
        break;
    case 5:
        rgb = {C, 0, X};
        break;
    }
    uint8_t R = round((rgb.r + m) * 255);
    uint8_t G = round((rgb.g + m) * 255);
    uint8_t B = round((rgb.b + m) * 255);

    return {R, G, B};
}

void LedStripe::ReadAnimationData()
{

    fs::path p(fs::current_path());

    if (fs::exists(p) && fs::is_directory(p))
    {
        Log::GetLogger()->warn("Search for Animation files ...");
        for (const auto &entry : fs::recursive_directory_iterator(p))
        {

            if (entry.path().extension() == ".bmp")
            {
                Log::GetLogger()->info(entry.path().c_str());

                std::ifstream datei(entry.path().c_str(), std::ifstream::binary | std::ifstream::in);

                if (datei.is_open())
                {

                    auto start_Robot = std::chrono::high_resolution_clock::now();

                    const int fileHeaderSize = 14;

                    unsigned char fileHeader[fileHeaderSize];
                    datei.read(reinterpret_cast<char *>(fileHeader), fileHeaderSize);

                    if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
                    {
                        Log::GetLogger()->error("Wrong file Format!");
                        continue;
                    }

                    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
                    int bitmapdataStart = fileHeader[10] + (fileHeader[11] << 8) + (fileHeader[12] << 16) + (fileHeader[13] << 24);
                    const int informationHeaderSize = bitmapdataStart - fileHeaderSize;

                    unsigned char *informationHeader = new unsigned char[informationHeaderSize];
                    datei.read(reinterpret_cast<char *>(informationHeader), informationHeaderSize);

                    unsigned int width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
                    unsigned int height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

                    const int paddingAmount = ((4 - (width * 3) % 4) % 4);

                    std::vector<RGB *> RGB_data;

                    for (int y = 0; y < height; y++)
                    {
                        RGB *rgb_line = new RGB[102];

                        for (int i = 0; i < 102; i++)
                            rgb_line[i] = {0, 0, 0};

                        for (int x = 0; x < width; x++)
                        {
                            unsigned char color[3];
                            datei.read(reinterpret_cast<char *>(color), 3);
                            rgb_line[x] = {color[2], color[1], color[0]};
                        }
                        RGB_data.push_back(rgb_line);
                        datei.ignore(paddingAmount);
                    }
                    auto end_Robot = std::chrono::high_resolution_clock::now();

                    printf("Zeit:\t\t %I64u ms\n", std::chrono::duration_cast<std::chrono::microseconds>(end_Robot - start_Robot).count());

                    animtions.insert({filecounter++, RGB_data});
                    delete[] informationHeader;
                }
            }
        }
        Log::GetLogger()->warn("Found {0} file(s)", filecounter);
    }
}

void LedStripe::PlayAnimation(int index)
{

}