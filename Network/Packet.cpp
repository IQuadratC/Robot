#include"Packet.h"



uint8_t Packet::ReadByte(){ 
    
    if(buffer.size() > readPos){
    
        uint8_t value = buffer[readPos];
        readPos++;
        return value;
    }
    //TODO: Error
}
