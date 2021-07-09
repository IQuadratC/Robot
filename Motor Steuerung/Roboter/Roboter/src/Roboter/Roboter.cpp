#include "Roboter.h"
#include <chrono>
float stepPerCM = 500;        // Steps per cm
float stepsPerDegree = 180;    // Steps per Degree

int serial_port = 0; // Serial Port

int maxSpeed = 5; // Maximal speed (kleiner gleich schneller(0-255))


void moveRobot(float y, float x, float Speed)
{
	if (Speed != 0) {

		int Motor12_Steps = 0;
		uint16_t Motor12_Delay = 0;

		int Motor34_Steps = 0;
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

		int m1speed;
		int m2speed;
		int m3speed;
		int m4speed;

		int index_m12 = 1;
		int index_m34 = 1;
		int m12_a = 100;
		int m34_a = 100;

		for (index_m12 = 1; index_m12 < 255; index_m12++)
		{
			m12_a /= 2;
			if (m12_a < Motor12_Delay)
				break;
		}
		for (index_m34 = 1; index_m34 < 255; index_m34++)
		{
			m34_a /= 2;
			if (m34_a < Motor34_Delay)
				break;
		}

		if (index_m12 < maxSpeed) {
			index_m12 = maxSpeed;
		}
		if (index_m34 < maxSpeed) {
			index_m34 = maxSpeed;
		}

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
		senddata[9] = index_m12;
		senddata[10] = index_m12;
		senddata[11] = index_m34;
		senddata[12] = index_m34;
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
		for (int i = 0; i < 15; i++) {
			//std::cout << std::hex << (short)senddata[i] << std::dec << std::endl;
		}
		tcflush(serial_port, TCIOFLUSH);
	}
}

int counter = 0;

bool roboterReadData()
{
	char buffer[15];
	read(serial_port, &buffer, 15);
	for (int i = 0; i < 15; i++) {
		std::cout << std::hex << (short)buffer[i] << std::dec << std::endl;
	}
	std::cout << "_____________"<< counter << std::endl;
	counter++;
	tcflush(serial_port, TCIOFLUSH);
	
}

inline int cmToSteps(int cm){ return cm * 68; }