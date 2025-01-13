#include "Timer.h"

Timer::Timer()
{

}

float Timer::GetDeltaTime()
{
	return 0.0f;
}

void Timer::Tick()
{
	duration<float>(steady_clock::now() - lastFrame).count();
}
