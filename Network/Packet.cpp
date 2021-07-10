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

}
void Packet::Write(int pos, uint8_t value){

}
void Packet::Write(uint8_t data[],int length){

}
void Packet::Write(int pos, uint8_t data[],int length){

}
void Packet::Write(int16_t value){

}
void Packet::Write(int pos, int16_t value){

}
void Packet::Write(int32_t value){

}
void Packet::Write(int pos, int32_t value){

}
void Packet::Write(int64_t value){

}
void Packet::Write(int pos, int64_t value){

}
void Packet::Write(float value){

}
void Packet::Write(int pos, float value){

}
void Packet::Write(bool value){

}
void Packet::Write(std::string& value){

}

uint8_t Packet::ReadByte(){ 
    
    if(buffer.size() > readPos){
    
        uint8_t value = buffer[readPos];
        readPos++;
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
