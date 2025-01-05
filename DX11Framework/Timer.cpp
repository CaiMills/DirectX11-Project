#include "Timer.h"

Timer::Timer()
{
	duration<float>(steady_clock::now() - lastFrame).count(); //Delta in seconds 
}

float Timer::GetDeltaTime()
{
	return 0.0f;
}

void Timer::Tick()
{
	duration<float>(steady_clock::now() - lastFrame).count();
}
