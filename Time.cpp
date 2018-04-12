#include "Time.h"

void Time::initTime()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&this->ticksPerSecond))
	{
		this->ticksPerSecond = 10000;
	}
	QueryPerformanceCounter((LARGE_INTEGER *)&this->ticksAtStart);
}
void Time::update(HWND wndHandle)
{
	this->numFrames++;
	UINT64 currentUpdate = getTicks();
	if (currentUpdate - this->lastUpdate > this->updateInterval * this->ticksPerSecond)
	{
		this->fps = this->numFrames / ((float)(currentUpdate - this->lastUpdate) / this->ticksPerSecond);
		this->lastUpdate = currentUpdate;
		this->numFrames = 0;
		//SetWindowTextA(wndHandle, std::to_string(fps).c_str());
	}
}
UINT64 Time::getTicks()
{
	UINT64 ticks;
	QueryPerformanceCounter((LARGE_INTEGER *)&ticks);
	ticks -= this->ticksAtStart;
	return ticks;
}
float Time::getTime()
{
	return (float)getTicks() / (float)this->ticksPerSecond;
}
float Time::getDeltaTime()
{
	//check this one again, wording is not correct?
	UINT64 dt = getTicks() - this->lastTimeSinceStart;
	this->lastTimeSinceStart += dt;
	return (float)dt / this->ticksPerSecond;
}

float Time::getFPS()
{
	return this->fps;
}