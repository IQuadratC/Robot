#pragma once
#include<iostream>
#include<vector>
#include<string>

class Packet{

public:

    Packet(uint8_t id);
    Packet(uint8_t data[],int length);
    ~Packet();

    size_t Length(){return buffer.size();}

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
    int16_t ReadInt16();
    int32_t ReadInt32();


private:
    std::vector<uint8_t> buffer;
    int readPos;

};

