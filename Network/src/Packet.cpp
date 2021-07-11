#include"Packet.h"

Packet::Packet(uint8_t id){
    Write(id);
}

Packet::Packet(uint8_t data[],int length){
    Write(data,length);
}
Packet::~Packet(){

}

void Packet::Write(uint8_t value){
    buffer.push_back(value);
}
void Packet::Write(int pos, uint8_t value){
    auto itPos = buffer.begin() + pos;
    buffer.insert(itPos,value);
}
void Packet::Write(uint8_t data[],int length){

    for (size_t i = 0; i < length; i++)
    {
       buffer.push_back(data[i]);
    }
}
void Packet::Write(int pos, uint8_t data[], int length){
    for (size_t i = 0; i < length; i++)
    {
        auto itPos = buffer.begin() + pos + i;
        buffer.insert(itPos,data[i]);
    }
}
void Packet::Write(int16_t value){
    union{
        int32_t a;
        uint8_t bytes[2];
    }data;
    data.a = value;
    Write(data.bytes,2);
}
void Packet::Write(int pos, int16_t value){
    union{
        int32_t a;
        uint8_t bytes[2];
    }data;
    data.a = value;
    Write(pos,data.bytes,2);
}
void Packet::Write(int32_t value){
    union{
        int32_t a;
        uint8_t bytes[4];
    }data;
    data.a = value;
    Write(data.bytes,4);
}
void Packet::Write(int pos, int32_t value){
    union{
        int32_t a;
        uint8_t bytes[4];
    }data;
    data.a = value;
    Write(pos,data.bytes,4);
}
void Packet::Write(int64_t value){
    union{
        int64_t a;
        uint8_t bytes[8];
    }data;
    data.a = value;
    Write(data.bytes,8);
}
void Packet::Write(int pos, int64_t value){
    union{
        int64_t a;
        uint8_t bytes[8];
    }data;
    data.a = value;
    Write(pos,data.bytes,8);
}
void Packet::Write(float value){
    union {
        float a;
        uint8_t bytes[4];
    } data;
    data.a = value;
    Write(data.bytes,4);
}
void Packet::Write(int pos, float value){
    union {
        float a;
        uint8_t bytes[4];
    } data;
    data.a = value;
    Write(pos, data.bytes,4);
}
void Packet::Write(bool value){
    Write((uint8_t) value);
}
void Packet::Write(std::string& value){
    Write((int) value.length());

    for (size_t i = 0; i < value.length(); i++)
    {
        Write(value[i]);
    }
}

uint8_t Packet::ReadByte(){ 
    
    if(buffer.size() > readPos){
    
        uint8_t value = buffer[readPos];
        readPos++;
        return value;
    }
    //TODO: Error
}

uint8_t* Packet::ReadBytes(size_t length){ 
    
    uint8_t value[length];
    if(buffer.size() > readPos){
    
        for (size_t i = 0; i < length; i++)
        {
            value[i] = buffer[readPos];
             readPos++;
        }
        return value;
    }
    //TODO: Error
}

 int16_t Packet::ReadInt16(){

      if(buffer.size() > readPos){

        int16_t value = int16_t(
            buffer[readPos + 0] << 8 |
            buffer[readPos + 1]
        );

        readPos += 2;
        return value;
    }
    //TODO: Error
 }

 int32_t Packet::ReadInt32(){
      if(buffer.size() > readPos){
    
        int32_t value = int32_t(
            buffer[readPos + 0] << 24 | 
            buffer[readPos + 1] << 16 |
            buffer[readPos + 2] << 8 |
            buffer[readPos + 3]
        );

        readPos += 4;
        return value;
    }
    //TODO: Error
 }

 int64_t Packet::ReadInt64(){
      if(buffer.size() > readPos){
    
        int64_t value = int64_t(
            buffer[readPos + 0] << 56 | 
            buffer[readPos + 1] << 48 |
            buffer[readPos + 2] << 40 |
            buffer[readPos + 3] << 32 |
            buffer[readPos + 4] << 24 | 
            buffer[readPos + 5] << 16 |
            buffer[readPos + 6] << 8 |
            buffer[readPos + 7]
        );

        readPos += 8;
        return value;
    }
    //TODO: Error
 }

float Packet::ReadFloat(){
      if(buffer.size() > readPos){
    
        float value = float(
            buffer[readPos + 0] << 24 | 
            buffer[readPos + 1] << 16 |
            buffer[readPos + 2] << 8 |
            buffer[readPos + 3]
        );

        readPos += 4;
        return value;
    }
    //TODO: Error
}

bool Packet::ReadBool(){
      if(buffer.size() > readPos){
    
        float value = float(buffer[readPos]);
        readPos++;
        return value;
    }
    //TODO: Error
}

std::string Packet::ReadString(){

    int length = ReadInt64();
    if(buffer.size() > readPos){

        uint8_t* bytes = ReadBytes(length);
        std::string value( reinterpret_cast<char const*>(bytes), length );
        readPos += length;

        return value;
    }
    //TODO: Error
}

