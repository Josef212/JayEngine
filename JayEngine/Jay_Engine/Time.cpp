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

float Time::EditorDT()const
{
	return realDT;
}

uint64 Time::EditorFrames()const
{
	return frameCount;
}

float Time::DT()const
{
	return gameDT;
}

uint64 Time::GameFrames()const
{
	return gameFPSCounter;
}

float Time::ElapsedTime()const
{
	return elapsed;
}

//----------------------------

void Time::UpdateTime()
{
	elapsed += realDT;
	realDT = (float)msTimer->ReadSec();
	msTimer->Start();
	++frameCount;
}

void Time::UpdateGameTime()
{
	gameDT = (float)msGameTimer->ReadSec();
	msGameTimer->Start();
	++gameFPSCounter;
}

void Time::RestartGameTimer()
{
	msGameTimer->Start();
	gameFPSCounter = 0;
	gameDT = 0;
}

void Time::StartGameTimer()
{
	msGameTimer->Start();
}

void Time::StopGameTimer()
{
	msGameTimer->Stop();
}

void Time::CleanGameTimer()
{
	msGameTimer->Stop();
	gameFPSCounter = 0;
	gameDT = 0;
}