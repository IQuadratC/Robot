#include "slamMap.h"
#include <iostream>	

#define MAPSIZE 1048
#define MAPLEVELS 10

char *maps;
int mapSizes[MAPLEVELS];

void InitMap(){

    int sum = MAPSIZE * MAPSIZE;
    mapSizes[0] = sum;
    for (size_t i = 1; i < MAPLEVELS; i++)
    {
        int s = MAPSIZE/(2*i) * MAPSIZE/(2*i);
        mapSizes[i] = s;
        sum += s;
    }
    
    maps = new char[sum];
}

inline int index(int x,int y, int level) {
    int index = 0;
    for (size_t i = 0; i < level; i++)
    {
        index += mapSizes[i];
    }
    index += x + (mapSizes[level] * y);
    return index;
}

char GetMap(int x,int y, int level){
    return maps[index(x, y, level)];
}
char GetMap(float x, float y, int level){
    GetMap((int) x, (int) y, level);
}
char GetMap(glm::vec2 pos, int level){
    GetMap(pos.x, pos.y, level);
}

void SetMap(int x,int y, int level, char value){
    maps[index(x, y, level)] = value;
}

void SetMap(float x,float y, int level, char value){
    SetMap((int) x, (int) y, level, value);
}
void SetMap(glm::vec2 pos, int level, char value){
    SetMap(pos.x, pos.y, level, value);
}

void GenLowerMaps(){
    for (size_t i = 1; i < MAPLEVELS; i++)
    {
        
    }
}
