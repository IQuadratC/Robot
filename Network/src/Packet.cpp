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
    int length = value.size();
    Write(length);

    const char* text = value.c_str();
    for (size_t i = 0; i < length; i++)
    {
       buffer.push_back(text[i]);
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

void Packet::ReadBytes(size_t length,uint8_t* value){ 
    
    if(buffer.size() > readPos){
    
        for (size_t i = 0; i < length; i++)
        {
            value[i] = buffer[readPos];
             readPos++;
        }
    }
    //TODO: Error
}

 int16_t Packet::ReadInt16(){

      if(buffer.size() > readPos){

        int16_t value = int16_t(
            buffer[readPos + 1] << 8 |
            buffer[readPos + 0]
        );

        readPos += 2;
        return value;
    }
    //TODO: Error
 }

 int32_t Packet::ReadInt32(){
      if(buffer.size() > readPos){
    
        int32_t value = int32_t(
            buffer[readPos + 3] << 24 | 
            buffer[readPos + 2] << 16 |
            buffer[readPos + 1] << 8 |
            buffer[readPos + 0]
        );

        readPos += 4;
        return value;
    }
    //TODO: Error
 }

 int64_t Packet::ReadInt64(){
      if(buffer.size() > readPos){
    
        int64_t value = int64_t(
            buffer[readPos + 7] << 56 | 
            buffer[readPos + 6] << 48 |
            buffer[readPos + 5] << 40 |
            buffer[readPos + 4] << 32 |
            buffer[readPos + 3] << 24 | 
            buffer[readPos + 2] << 16 |
            buffer[readPos + 1] << 8 |
            buffer[readPos + 0]
        );

        readPos += 8;
        return value;
    }
    //TODO: Error
 }

float Packet::ReadFloat(){
      if(buffer.size() > readPos){
    
        float value = float(
            buffer[readPos + 3] << 24 | 
            buffer[readPos + 2] << 16 |
            buffer[readPos + 1] << 8 |
            buffer[readPos + 0]
        );

        readPos += 4;
        return value;
    }
    //TODO: Error
}

bool Packet::ReadBool(){
      if(buffer.size() > readPos){
    
        bool value = bool(buffer[readPos]);
        readPos++;
        return value;
    }
    //TODO: Error
}

std::string Packet::ReadString(){
    int length = ReadInt32();
    if(buffer.size() > readPos){
        uint8_t bytes[length];
        ReadBytes(length,&bytes[0]);
        std::string value((char*) bytes,length);
        return value;
    }
    //TODO: Error
}

