#include "Roboter.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sstream>
#include <sys/ioctl.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <vector>
#include <iostream>
#include <experimental/filesystem>

inline static float map(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

namespace fs = std::experimental::filesystem;

Roboter::Roboter()
{
	fs::path p("/dev/ttyACM0");
	if (fs::exists(p))
	{
		system("stty -F /dev/ttyACM0 -hupcl");
	}
}

Roboter::~Roboter()
{
	close(serial_port);
}

void Roboter::Start()
{
	std::thread robot(&Roboter::Mainloop,this);
	robot.detach();
}

void Roboter::Stop()
{
	ready = true;
	stop = true;
	cv.notify_one();
}

void Roboter::Mainloop()
{
	StartSerial();
	Log::GetRoboterLogger()->warn("Initialized Roboter Log!");
	Logger = Log::GetRoboterLogger();
	while (true)
	{
		auto start_Robot = std::chrono::high_resolution_clock::now();
		std::unique_lock<std::mutex> lk(mutex);
		cv.wait(lk, [&](){ return ready || stop; });
		if (stop) break;
		Logger->info("Save Data");
		Flags thread_flags = flags;
		Data thread_data = data;
		ready = false;
		lk.unlock();

		if (thread_flags.move)
		{
			moveRobot(thread_data.x, thread_data.y, thread_data.speed);
			thread_flags.move = false;
		}
		if (thread_flags.rotate)
		{
			rotateRobot(thread_data.speed);
			thread_flags.rotate = false;
		}
		if (thread_flags.stop)
		{
			stopRobot();
			thread_flags.stop = false;
		}
		if (thread_flags.getBettery)
		{
			thread_flags.getBettery = false;
		}
		auto end_Robot = std::chrono::high_resolution_clock::now();
		printf("Zeit:\t\t %I64u ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(end_Robot - start_Robot).count());
	}
}

void Roboter::Do_command(commands commands)
{
	ready = true;
	std::unique_lock<std::mutex> lk(mutex);

	switch (commands)
	{
	case commands::setMode:
		
		break;
	case commands::move:
		flags.move = true;
		break;
	case commands::rotate:
		flags.rotate = true;
		break;
	case commands::stop:
		flags.stop = true;
		break;
	case commands::getBettery:
		flags.getBettery = true;
		break;
	}
	lk.unlock();
	cv.notify_one();
}

void Roboter::moveRobot(float x, float y, float Speed)
{
	//Logger->info("MoveTest");
	int32_t Motor12_Steps = 0;
	float Motor12_Delay = 0;

	int32_t Motor34_Steps = 0;
	float Motor34_Delay = 0;

	if (x >= 0 && y >= 0 && abs(y) >= abs(x))
	{
		Motor34_Steps = cmToSteps(y);
		Motor34_Delay = Speed;
		float f = ((y - x) / y);
		Motor12_Steps = cmToSteps(y - x);
		Motor12_Delay = Speed * f;
		Motor12Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x >= 0 && y >= 0 && abs(y) <= abs(x))
	{
		Motor34_Steps = cmToSteps(x);
		Motor34_Delay = Speed;
		float f = ((x - y) / x);
		Motor12_Steps = -cmToSteps(x - y);
		Motor12_Delay = Speed * ((x - y) / x);
		Motor12Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x >= 0 && y <= 0 && abs(y) <= abs(x))
	{
		Motor12_Steps = -cmToSteps(x);
		Motor12_Delay = Speed;
		float f = ((x + y) / x);
		Motor34_Steps = cmToSteps(x + y);
		Motor34_Delay = Speed * f;
		Motor34Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x >= 0 && y <= 0 && abs(y) >= abs(x))
	{

		Motor12_Steps = cmToSteps(y);
		Motor12_Delay = Speed;
		float f = ((y + x) / y);
		Motor34_Steps = cmToSteps(y + x);
		Motor34_Delay = Speed * f;
		Motor34Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x <= 0 && y <= 0 && abs(y) >= abs(x))
	{

		Motor34_Steps = cmToSteps(y);
		Motor34_Delay = Speed;
		float f = ((y - x) / y);
		Motor12_Steps = cmToSteps(y - x);
		Motor12_Delay = Speed * f;
		Motor12Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x <= 0 && y <= 0 && abs(y) <= abs(x))
	{
		Motor34_Steps = cmToSteps(x);
		Motor34_Delay = Speed;
		float f = ((x - y) / x);
		Motor12_Steps = -cmToSteps(x - y);
		Motor12_Delay = Speed * f;
		Motor12Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x <= 0 && y >= 0 && abs(y) <= abs(x))
	{
		Motor12_Steps = -cmToSteps(x);
		Motor12_Delay = Speed;
		float f = ((x + y) / x);
		Motor34_Steps = cmToSteps(x + y);
		Motor34_Delay = Speed * f;
		Motor34Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
	else if (x <= 0 && y >= 0 && abs(y) >= abs(x))
	{
		Motor12_Steps = cmToSteps(y);
		Motor12_Delay = Speed;
		float f = ((y + x) / y);
		Motor34_Steps = cmToSteps(y + x);
		Motor34_Delay = Speed * f;
		Motor34Speed(Motor12_Steps, Motor34_Steps, Motor12_Delay, Motor34_Delay, f);
	}
}

void Roboter::rotateRobot(float speed)
{

	//TODO: Motor acceleration
	uint8_t diractions = 0;
	if (speed > 0)
	{
		diractions |= 1 << 1;
		diractions |= 1 << 3;
	}
	else
	{
		diractions |= 1 << 0;
		diractions |= 1 << 2;
	}

	uint8_t delay = SpeedToDelay(speed);

	unsigned char senddata[15];
	memset(senddata, 0, 15);

	senddata[0] = 0x3C; //Start Byte
	senddata[1] = 0xff;
	senddata[2] = 0xff;
	senddata[3] = 0xff;
	senddata[4] = 0xff;
	senddata[5] = 0xff;
	senddata[6] = 0xff;
	senddata[7] = 0xff;
	senddata[8] = 0xff;
	senddata[9] = delay;
	senddata[10] = delay;
	senddata[11] = delay;
	senddata[12] = delay;
	senddata[13] = diractions;
	senddata[14] = 0x3E; //End Byte
	SendDatatoArduino(senddata);
}

void Roboter::stopRobot()
{
	unsigned char senddata[15];
	memset(senddata, 0, 15);
	senddata[0] = 0x3C;
	senddata[14] = 0x3E;
}

void Roboter::StartSerial()
{
	fs::path p("/dev/ttyACM0");
	if (fs::exists(p))
	{
		system("stty -F /dev/ttyACM0 -hupcl");
		serial_port = open("/dev/ttyACM0", O_RDWR);
		if (serial_port == -1)
		{
			printf("[ERROR] UART open()\n");
			serialstart = false;
		}
		else
		{
			Logger->info("Init Serial");
			struct termios options;
			if (tcgetattr(serial_port, &options) != 0)
			{
				//Logger->error("Error {0} from tcgetattr: {1}\n", errno, strerror);
				serialstart = false;
			}
			options.c_cflag = B115200 | CS8 | CREAD;
			options.c_iflag = IGNPAR;
			options.c_oflag = 0;
			options.c_lflag = 0;
			//options.c_cc[VTIME] = 10;
			//options.c_cc[VMIN] = 1;
			cfsetspeed(&options, B115200);
			tcflush(serial_port, TCIFLUSH);
			if (tcsetattr(serial_port, TCSANOW, &options) != 0)
			{
				//Logger->error("Error {0} from tcsetattr: {1}\n", errno, strerror);
				serialstart = false;
			}
			else
			{
				serialstart = true;
			}
		}
	}
}

void Roboter::SendDatatoArduino(uint8_t *const senddata)
{
	if (serialstart)
	{
		int out = write(serial_port, senddata, 15);
		//if (out < 0) Logger->error("Serial Transmission Error");

		union
		{
			short data;
			uint8_t bytes[2];
		} power;
		memset(power.bytes, 0, 2);

		std::stringstream ss;
		for (int i = 0; i < 15; i++)
			ss << std::hex << (short)senddata[i] << std::dec << " | ";
		//Logger->info(ss.str().c_str());

		//read(serial_port, power.bytes, 2);
		//Logger->info(power.data);
		//Logger->info((int)map(power.data, 680, 800, 0, 100));
		tcflush(serial_port, TCIOFLUSH);
	}
}

void Roboter::PrepareSendData(int32_t m12_Steps, int32_t m34_Steps, float m12_speed, float m34_speed)
{
	uint8_t m12speed;
	uint8_t m34speed;
	if (m12_speed == 0)
	{
		m12speed = 255;
	}
	else
	{
		m12speed = SpeedToDelay(m12_speed);
	}
	if (m34_speed == 0)
	{
		m34speed = 255;
	}
	else
	{
		m34speed = SpeedToDelay(m34_speed);
	}

	char diractions = 0;

	if (m12_Steps < 0)
	{
		diractions |= 1 << 0;
		diractions |= 1 << 1;
	}
	if (m34_Steps < 0)
	{
		diractions |= 1 << 2;
		diractions |= 1 << 3;
	}
	if (newSteps)
	{
		diractions |= 0 << 7;
	}
	else
	{
		diractions |= 1 << 7;
	}
	//diractions |= 1 << 4;
	int32_t Motor12_Steps = abs(m12_Steps);
	int32_t Motor34_Steps = abs(m34_Steps);

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
}

uint8_t Roboter::SpeedToDelay(float speed)
{
	int steps = cmToSteps(speed);
	if (steps > 10000)
	{
		//Logger->warn("Speed is to Heigh!(Set Speed to Max Value(10000))");
		steps = 10000;
	}
	float stepsInS = steps * interruptTime;
	int delay = round(((1 / stepsInS) - 1) * 10);
	if (delay > 255)
		delay = 255;
	//Logger->info(delay);
	return (uint8_t)delay;
}

void Roboter::Motor12Speed(int32_t Motor12_Steps, int32_t Motor34_Steps, float Motor12_Delay, float Motor34_Delay, float f)
{
	bool m12 = true;
	bool m34 = true;

	newSteps = true;

	while (m12 || m34)
	{
		if (ready)
			return;

		if ((Motor12_Delay - m12_old_delay) > 0.5)
		{
			m12_old_delay += 1 * f;
		}
		else if (m12_old_delay - Motor12_Delay > 0.5)
		{
			m12_old_delay -= 1 / f;
		}
		else if (abs(Motor12_Delay - m12_old_delay) <= 0.5)
		{
			m12_old_delay = Motor12_Delay;
			m12 = false;
		}

		if ((Motor34_Delay - m34_old_delay) > 0.5)
		{
			m34_old_delay += 1;
		}
		else if (m34_old_delay - Motor34_Delay > 0.5)
		{
			m34_old_delay -= 1;
		}
		else if (abs(Motor34_Delay - m34_old_delay) <= 0.5)
		{
			m34_old_delay = Motor34_Delay;
			m34 = false;
		}

		PrepareSendData(Motor12_Steps, Motor34_Steps, m12_old_delay, m34_old_delay);
		newSteps = false;
		std::stringstream ss;
		ss << "[m12]" << m12_old_delay << "|" << m34_old_delay << "|" << f;
		Logger->info(ss.str().c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(acceleration));
	}
	newSteps = true;
}

void Roboter::Motor34Speed(int32_t Motor12_Steps, int32_t Motor34_Steps, float Motor12_Delay, float Motor34_Delay, float f)
{
	bool m12 = true;
	bool m34 = true;

	newSteps = true;

	while (m12 || m34)
	{
		if (ready)
			return;

		if ((Motor12_Delay - m12_old_delay) > 0.5)
		{
			m12_old_delay += 1;
		}
		else if (m12_old_delay - Motor12_Delay > 0.5)
		{
			m12_old_delay -= 1;
		}
		else if (abs(Motor12_Delay - m12_old_delay) <= 0.5)
		{
			m12_old_delay = Motor12_Delay;
			m12 = false;
		}

		if ((Motor34_Delay - m34_old_delay) > 0.5)
		{
			m34_old_delay += 1 * f;
		}
		else if (m34_old_delay - Motor34_Delay > 0.5)
		{
			m34_old_delay -= 1 / f;
		}
		else if (abs(Motor34_Delay - m34_old_delay) <= 0.5)
		{
			m34_old_delay = Motor34_Delay;
			m34 = false;
		}
		PrepareSendData(Motor12_Steps, Motor34_Steps, m12_old_delay, m34_old_delay);
		newSteps = false;
		std::stringstream ss;
		ss << "[m34]" << m12_old_delay << "|" << m34_old_delay << "|" << f;
		Logger->info(ss.str().c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(acceleration));
	}
	newSteps = true;
}