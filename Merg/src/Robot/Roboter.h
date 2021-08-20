#pragma once
#include <memory>
#include <cstring>
#include "../Log.h"
#include <mutex>
#include <condition_variable>

enum commands
{
	move,
	rotate,
	stop,
	getBettery,
	setMode
};

class Roboter
{
private:
	struct Data
	{
		float speed = 0.f;
		float x = 0.f;
		float y = 0.f;
		float angle = 0.f;
		int battery = 0;
	};

public:
	Roboter();
	~Roboter();

	void Start();
	void Do_command(commands commands);
	void Stop();
	Data data;

private:
	bool stop = false;
	struct Flags
	{
		bool move :1;
		bool rotate : 1;
		bool stop :1;
		bool getBettery :1;
	};
	
	int RoboterMode = 1;

	Flags flags = {false,false,false,false};

	std::mutex mutex;
	std::condition_variable cv;
	bool ready = false;

	void Mainloop();

	void moveRobot(float x, float y, float Speed);
	void rotateRobot(float speed);
	void stopRobot();

	uint8_t SpeedToDelay(float speed);

	const float interruptTime = 0.0001f;

	int serial_port;
	bool serialstart = false;

	void StartSerial();

	void SendDatatoArduino(uint8_t *const senddata);

	void PrepareSendData(int32_t m12_Steps, int32_t m34_Steps, float m12_speed, float m34_speed);

	inline int cmToSteps(int cm) { return cm * 68; }

	std::shared_ptr<spdlog::logger> Logger;

	int acceleration = 50;

	bool newSteps;
	float m12_old_delay = 0;
	float m34_old_delay = 0;

	void Motor12Speed(int32_t Motor12_Steps, int32_t Motor34_Steps, float Motor12_Delay, float Motor34_Delay, float f);
	void Motor34Speed(int32_t Motor12_Steps, int32_t Motor34_Steps, float Motor12_Delay, float Motor34_Delay, float f);
};
