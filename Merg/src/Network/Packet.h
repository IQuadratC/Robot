#pragma once

#include "Network.h"

class Packet{

public:

    Packet(uint8_t id);
    Packet(uint8_t data[],int length);
    ~Packet();

    size_t Length(){return buffer.size();}
    uint8_t* ToArray(){return buffer.data();}

    void Write(uint8_t value);
    void Write(int pos, uint8_t value);
    void Write(uint8_t data[],int length);
    void Write(int pos, uint8_t data[],int length);
    void Write(int16_t value);
    void Write(int pos, int16_t value);
    void Write(int32_t value);
    void Write(int pos, int32_t value);
    void Write(int64_t value);
    void Write(int pos, int64_t value);
    void Write(float value);
    void Write(int pos, float value);
    void Write(bool value);
    void Write(std::string& value);

    uint8_t ReadByte();
    void ReadBytes(size_t length,uint8_t* value);
    int16_t ReadInt16();
    int32_t ReadInt32();
    int64_t ReadInt64();
    float ReadFloat();
    bool ReadBool();
    std::string ReadString();

private:
    std::vector<uint8_t> buffer;
    int readPos = 0;

};

