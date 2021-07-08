#include"LidarCalc.h"
#include"Lidar.h"



static void clearData(float* data) {
	for (int i = 0; i < 360; i++) data[i] = -1;
}

float* referenceData = new float[360];
float* newData = new float[360];

int LidarMain() {

	clearData(referenceData);
	clearData(newData);

	CalculateSinAndCos();

	getLidarData(referenceData);

	int angle = 0;

	while (true)
	{
		clearData(newData);
		getLidarData(newData);

		angle = calculateAngle();

	}
}