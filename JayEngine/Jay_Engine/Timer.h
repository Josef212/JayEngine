#ifndef __TIMER__
#define __TIMER__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	//Returns time in ms
	Uint32 Read();
	//Returns time in sec
	float ReadSec();

private:

	bool	running;
	Uint32	startedAt;
	Uint32	stoppedAt;
};

#endif //__TIMER__