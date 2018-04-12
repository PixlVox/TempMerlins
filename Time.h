#ifndef TIME_H
#define TIME_H
#include <windows.h>
#include <string>

class Time
{
private:
	UINT64 ticksAtStart;
	UINT64 ticksPerSecond;
	UINT64 lastUpdate = 0;
	float updateInterval = 0.5f;
	unsigned int  numFrames = 0;
	float fps = 0;

	// For movement physics
	UINT64 lastTimeSinceStart = 0;
	UINT64 getTicks();

public:

	void initTime();
	void update(HWND wndHandle);
	float getTime();
	float getDeltaTime();
	float getFPS();

};
#endif
