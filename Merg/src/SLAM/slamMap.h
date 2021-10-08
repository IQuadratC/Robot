#pragma once

#include <glm/glm.hpp>
#include <iostream>	
#include <stdint.h>

#define CHUNKBOUNDS 256

struct Chunk{
    glm::vec2 pos;
    uint8_t* data;
}

uint8_t GetMap(int x,int y, int level);
uint8_t GetMap(float x, float y, int level);
uint8_t GetMap(glm::vec2 pos, int level);

void SetMap(int x,int y, int level, uint8_t value);
void SetMap(float x,float y, int level, uint8_t value);
void SetMap(glm::vec2 pos, int level, uint8_t value);