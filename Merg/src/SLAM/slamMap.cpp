#include "slamMap.h"

#include <map>
#include <iostream>
#include <cassert>

std::map<glm::vec2, Chunk> chunks;

inline glm::vec2 getChunkPos(glm::vec2 pos){
    return glm::vec2((int)(pos.x/CHUNKBOUNDS), (int)(pos.y/CHUNKBOUNDS));
}

inline Chunk newChunk(glm::vec2 pos){
    Chunk chunk;
    chunk.pos = pos;
    chunk.data = new uint8_t[CHUNKBOUNDS*CHUNKBOUNDS];
    return chunk;
}

uint8_t Get(glm::vec2 pos){
    glm::vec2 chunkPos = getChunkPos(pos);
    Chunk chunk = chunks[chunkPos];
}





/*
uint8_t GetMap(int x,int y, int level){
    if (x < 0 || x >= MAPSIZE || y < 0 || y >= MAPSIZE){
        return 0;
    }
    return maps[index(x / MAPINTERVALL, y / MAPINTERVALL, level)];
}
uint8_t GetMap(float x, float y, int level){
    GetMap((int) x, (int) y, level);
}
uint8_t GetMap(glm::vec2 pos, int level){
    GetMap(pos.x, pos.y, level);
}

void SetMap(int x,int y, int level, uint8_t value){
    if (x < 0 || x >= MAPSIZE || y < 0 || y >= MAPSIZE){
        return;
    }
    int i = index(x / MAPINTERVALL, y / MAPINTERVALL, level);
    maps[i] = value;
}

void SetMap(float x,float y, int level, uint8_t value){
    SetMap((int) x, (int) y, level, value);
}
void SetMap(glm::vec2 pos, int level, uint8_t value){
    SetMap(pos.x, pos.y, level, value);
}

void GenLowerMaps(){
    for (size_t i = 1; i < MAPLEVELS; i++)
    {
        
    }
}
*/
