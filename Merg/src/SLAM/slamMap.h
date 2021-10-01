#pragma once

#include <glm/glm.hpp>

#define MAPSIZE 2048
#define MAPLEVELS 10

void InitMap();

uint8_t* GetMap(int level);
uint8_t GetMap(int x,int y, int level);
uint8_t GetMap(float x, float y, int level);
uint8_t GetMap(glm::vec2 pos, int level);

void SetMap(int x,int y, int level, uint8_t value);
void SetMap(float x,float y, int level, uint8_t value);
void SetMap(glm::vec2 pos, int level, uint8_t value);