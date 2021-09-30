#pragma once

#include <glm/glm.hpp>

void InitMap();

char GetMap(int x,int y, int level);
char GetMap(float x, float y, int level);
char GetMap(glm::vec2 pos, int level);

void SetMap(int x,int y, int level, char value);
void SetMap(float x,float y, int level, char value);
void SetMap(glm::vec2 pos, int level, char value);