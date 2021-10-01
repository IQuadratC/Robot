#include "slamMap.h"
#include <iostream>	

uint8_t *maps;
int mapSizes[MAPLEVELS];
int mapYSizes[MAPLEVELS];

void InitMap(){

    int sum = MAPSIZE * MAPSIZE;
    mapSizes[0] = MAPSIZE * MAPSIZE;
    mapYSizes[0] = MAPSIZE;

    for (size_t i = 1; i < MAPLEVELS; i++)
    {
        int s = MAPSIZE/(2*i);
        mapYSizes[i] = s;
        mapSizes[i] = s * s;
        sum += s * s;
    }
    
    maps = new uint8_t[sum];
}

inline int index(int x,int y, int level) {
    int index = 0;
    for (size_t i = 0; i < level; i++)
    {
        index += mapSizes[i];
    }
    index += x + (mapYSizes[level] * y);
    return index;
}

uint8_t* GetMap(int level){
    return maps;
}

uint8_t GetMap(int x,int y, int level){
    return maps[index(x, y, level)];
}
uint8_t GetMap(float x, float y, int level){
    GetMap((int) x, (int) y, level);
}
uint8_t GetMap(glm::vec2 pos, int level){
    GetMap(pos.x, pos.y, level);
}

void SetMap(int x,int y, int level, uint8_t value){
    int i = index(x, y, level);
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
