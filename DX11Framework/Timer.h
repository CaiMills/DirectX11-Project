#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
private:
	steady_clock::time_point lastFrame;

public:
	Timer();

	float GetDeltaTime(); //Request time since last frame
	void Tick(); //Store time of last frame
};

