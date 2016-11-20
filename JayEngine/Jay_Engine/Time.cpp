#include "Time.h"
#include "Timer.h"


Time::Time()
{
	msTimer = new Timer();
	fpsTimer = new Timer();

	msGameTimer = new Timer();
}


Time::~Time()
{
}

float Time::editorDT()const
{
	return realDT;
}

uint64 Time::editorFrames()const
{
	return frameCount;
}

float Time::DT()const
{
	return gameDT;
}

uint64 Time::gameFrames()const
{
	return gameFPSCounter;
}

//----------------------------

void Time::updateTime()
{
	realDT = (float)msTimer->ReadSec();
	msTimer->Start();
	++frameCount;
}

void Time::updateGameTime()
{
	gameDT = (float)msGameTimer->ReadSec();
	msGameTimer->Start();
	++gameFPSCounter;
}

void Time::restartGameTimer()
{
	msGameTimer->Start();
	gameFPSCounter = 0;
	gameDT = 0;
}

void Time::startGameTimer()
{
	msGameTimer->Start();
}

void Time::stopGameTimer()
{
	msGameTimer->Stop();
}

void Time::cleanGameTimer()
{
	msGameTimer->Stop();
	gameFPSCounter = 0;
	gameDT = 0;
}