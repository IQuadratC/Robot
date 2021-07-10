#include "Roboter.h"
#include <chrono>
#include "../Logger/Log.h"
static const float stepPerCM = 68;        // Steps per cm
static const float stepsPerDegree = 180;    // Steps per Degree

static int serial_port = 0; // Serial Port

inline int cmToSteps(int cm) { return cm * 68; }

uint8_t SpeedTODelay(float speed) {

	int steps = cmToSteps(speed);
	if (steps > 10000) {
		ROBOTER_LOG_WARN("Speed is to Heigh!(Set Speed to Max Value(10000))");
		steps = 10000;
	}
	float stepsInS = steps * 0.0001; // 0.0001 Interrupt Speed
	uint8_t delay = round(((1 / stepsInS) - 1) * 10);
	if (delay > 255) delay = 255;
	return delay; // mal 10 um delay auflösung zu erhöhen
}

void moveRobot(float x, float y, float Speed)
{
	if (Speed != 0) {

		int32_t Motor12_Steps = 0;
		uint16_t Motor12_Delay = 0;

		int32_t Motor34_Steps = 0;
		uint16_t Motor34_Delay = 0;

		if (x >= 0 && y >= 0 && abs(y) >= abs(x))
		{
			Motor34_Steps = stepPerCM * y;
			Motor34_Delay = Speed;

			Motor12_Steps = stepPerCM * (y - x);
			Motor12_Delay = Speed * ((y - x) / y);
		}
		else if (x >= 0 && y >= 0 && abs(y) <= abs(x))
		{
			Motor34_Steps = stepPerCM * x;
			Motor34_Delay = Speed;

			Motor12_Steps = -stepPerCM * (x - y);
			Motor12_Delay = Speed * ((x - y) / x);
		}
		else if (x >= 0 && y <= 0 && abs(y) <= abs(x))
		{

			Motor12_Steps = -stepPerCM * x;
			Motor12_Delay = Speed;

			Motor34_Steps = stepPerCM * (x + y);
			Motor34_Delay = Speed * ((x + y) / x);
		}
		else if (x >= 0 && y <= 0 && abs(y) >= abs(x))
		{

			Motor12_Steps = stepPerCM * y;
			Motor12_Delay = Speed;

			Motor34_Steps = stepPerCM * (y + x);
			Motor34_Delay = Speed * ((y + x) / y);
		}
		else if (x <= 0 && y <= 0 && abs(y) >= abs(x))
		{

			Motor34_Steps = stepPerCM * y;
			Motor34_Delay = Speed;

			Motor12_Steps = stepPerCM * (y - x);
			Motor12_Delay = Speed * ((y - x) / y);
		}
		else if (x <= 0 && y <= 0 && abs(y) <= abs(x))
		{
			Motor34_Steps = stepPerCM * x;
			Motor34_Delay = Speed;

			Motor12_Steps = -stepPerCM * (x - y);
			Motor12_Delay = Speed * ((x - y) / x);
		}
		else if (x <= 0 && y >= 0 && abs(y) <= abs(x))
		{
			Motor12_Steps = -stepPerCM * x;
			Motor12_Delay = Speed;

			Motor34_Steps = stepPerCM * (x + y);
			Motor34_Delay = Speed * ((x + y) / x);
		}
		else if (x <= 0 && y >= 0 && abs(y) >= abs(x))
		{
			Motor12_Steps = stepPerCM * y;
			Motor12_Delay = Speed;

			Motor34_Steps = stepPerCM * (y + x);
			Motor34_Delay = Speed * ((y + x) / y);
		}

		int m12speed = SpeedTODelay(Motor12_Delay);
		int m34speed = SpeedTODelay(Motor34_Delay);

		char diractions = 0;

		if (Motor12_Steps < 0)
		{
			diractions |= 1 << 0;
			diractions |= 1 << 1;
		}
		if (Motor34_Steps < 0)
		{
			diractions |= 1 << 2;
			diractions |= 1 << 3;
		}

		Motor12_Steps = abs(Motor12_Steps);
		Motor34_Steps = abs(Motor34_Steps);

		uint16_t m1_Steps = 0;
		uint16_t m2_Steps = 0;
		uint16_t m3_Steps = 0;
		uint16_t m4_Steps = 0;

		if (Motor12_Steps > std::numeric_limits<uint16_t>::max())
		{
			m1_Steps = std::numeric_limits<uint16_t>::max();
			m2_Steps = std::numeric_limits<uint16_t>::max();
		}
		else
		{
			m1_Steps = Motor12_Steps;
			m2_Steps = Motor12_Steps;
		}

		if (Motor34_Steps > std::numeric_limits<uint16_t>::max())
		{
			m3_Steps = std::numeric_limits<uint16_t>::max();
			m4_Steps = std::numeric_limits<uint16_t>::max();
		}
		else
		{
			m3_Steps = Motor34_Steps;
			m4_Steps = Motor34_Steps;
		}

		uint8_t senddata[15];
		memset(senddata, 0, 15);

		senddata[0] = 0x3C; //Start Byte
		senddata[1] = m1_Steps >> 8 & 0xFF;
		senddata[2] = m1_Steps & 0xFF;
		senddata[3] = m2_Steps >> 8 & 0xFF;
		senddata[4] = m2_Steps & 0xFF;
		senddata[5] = m3_Steps >> 8 & 0xFF;
		senddata[6] = m3_Steps & 0xFF;
		senddata[7] = m4_Steps >> 8 & 0xFF;
		senddata[8] = m4_Steps & 0xFF;
		senddata[9] = m12speed;
		senddata[10] = m12speed;
		senddata[11] = m34speed;
		senddata[12] = m34speed;
		senddata[13] = diractions;
		senddata[14] = 0x3E; //End Byte

		SendDatatoArduino(senddata);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		m1_Steps = 0;
		m2_Steps = 0;
		m3_Steps = 0;
		m4_Steps = 0;
	}
	else {

		uint8_t senddata[15];
		memset(senddata, 0, 15);
		senddata[0] = 0x3C;
		senddata[14] = 0x3E;
		SendDatatoArduino(senddata);
	}
}

void rotateRobot(int a, int Speedprozent)
{
	if (Speedprozent != 0)
	{
		int diractions = 0;
		if (a > 0)
		{
			diractions |= 1 << 1;
			diractions |= 1 << 3;
		}
		else
		{
			diractions |= 1 << 0;
			diractions |= 1 << 2;
		}

		unsigned char senddata[15];
		memset(senddata, 0, 15);

		int m1 = abs(stepsPerDegree * a);
		int m2 = abs(stepsPerDegree * a);
		int m3 = abs(stepsPerDegree * a);
		int m4 = abs(stepsPerDegree * a);

		int speed = (255 - (Speedprozent * 2.55));

		if (speed < maxSpeed) speed = maxSpeed;

		senddata[0] = 0x3C; //Start Byte
		senddata[1] = m1 >> 8 & 0xFF;
		senddata[2] = m1 & 0xFF;
		senddata[3] = m2 >> 8 & 0xFF;
		senddata[4] = m2 & 0xFF;
		senddata[5] = m3 >> 8 & 0xFF;
		senddata[6] = m3 & 0xFF;
		senddata[7] = m4 >> 8 & 0xFF;
		senddata[8] = m4 & 0xFF;
		senddata[9] = speed;
		senddata[10] = speed;
		senddata[11] = speed;
		senddata[12] = speed;
		senddata[13] = diractions;
		senddata[14] = 0x3E; //End Byte
		SendDatatoArduino(senddata);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));


	}
	else {
		unsigned char senddata[15];
		memset(senddata, 0, 15);
		senddata[0] = 0x3C;
		senddata[14] = 0x3E;
		SendDatatoArduino(senddata);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

}

void roboterTest()
{
	unsigned short m1 = 3200;
	unsigned short m2 = 3200;
	unsigned short m3 = 3200;
	unsigned short m4 = 3200;


	uint8_t speed = 1;

	char diractions = 0;

	diractions |= 0 << 0;
	diractions |= 0 << 1;
	diractions |= 0 << 2;
	diractions |= 0 << 3;


	unsigned char senddata[15];
	memset(senddata, 0, 15);
	senddata[0] = 0x3C; //Start Byte
	senddata[1] = m1 >> 8 & 0xFF;
	senddata[2] = m1 & 0xFF;
	senddata[3] = m2 >> 8 & 0xFF;
	senddata[4] = m2 & 0xFF;
	senddata[5] = m3 >> 8 & 0xFF;
	senddata[6] = m3 & 0xFF;
	senddata[7] = m4 >> 8 & 0xFF;
	senddata[8] = m4 & 0xFF;
	senddata[9] = speed;
	senddata[10] = speed;
	senddata[11] = speed;
	senddata[12] = speed;
	senddata[13] = diractions;
	senddata[14] = 0x3E; //End Byte
	SendDatatoArduino(senddata);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void startserial(bool& serialstart)
{
	//Quelle: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
	serialstart = true;

	serial_port = open("/dev/ttyACM0", O_RDWR);
	if (serial_port == -1)
	{
		printf("[ERROR] UART open()\n");
		serialstart = false;
	}
	if (serialstart)
	{
		struct termios options;
		if (tcgetattr(serial_port, &options) != 0)
		{
			printf("Error %i from tcgetattr: %s\n", errno, strerror);
			serialstart = false;
		}
		//options.c_cc[VTIME] = 10;
		//options.c_cc[VMIN] = 1;
		cfsetspeed(&options, B115200);

		if (tcsetattr(serial_port, TCSANOW, &options) != 0)
		{
			printf("Error %i from tcsetattr: %s\n", errno, strerror);
			serialstart = false;
		}
		else
		{
			serialstart = true;
		}
	}
}

void SendDatatoArduino(unsigned char senddata[15])
{
	if (serial_port != -1)
	{
		int out = write(serial_port, senddata, 15);
		if (out < 0)
		{
			printf("[ERROR] UART TX\n");
		}
		std::stringstream ss;
		for (int i = 0; i < 15; i++) {
			ss << std::hex << (short)senddata[i] << std::dec << " | ";
		}
		ROBOTER_LOG_INFO(ss.str().c_str());
		tcflush(serial_port, TCIOFLUSH);
	}
}

bool roboterReadData()
{
	char buffer[15];
	read(serial_port, &buffer, 15);
	for (int i = 0; i < 15; i++) {
		std::cout << std::hex << (short)buffer[i] << std::dec << std::endl;
	}
	tcflush(serial_port, TCIOFLUSH);
	
}

