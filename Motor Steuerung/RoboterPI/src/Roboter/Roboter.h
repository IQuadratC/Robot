#pragma once
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <sstream>
#include <thread>
#include <unistd.h>			
#include <fcntl.h>			
#include <termios.h>	

//#include"../LidarCalc/Lidar.h"
//#include"../LidarCalc/LidarCalc.h"

void SendStepsAndSpeed(int32_t m12_Steps, int32_t m34_Steps, float m12_speed, float m34_speed);

void moveRobot(float x, float y, float speed);

void rotateRobot(int a, int speed);

void roboterTest();

void startserial(bool& serialstart);

void roboterstop();

void SendDatatoArduino(unsigned char senddata[13]);

bool roboterReadData();

inline int cmToSteps(int cm);

void Oct1Speed(float x,float y,int32_t Motor12_Steps,int32_t Motor34_Steps,float Motor12_Delay,float Motor34_Delay);
