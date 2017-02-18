#ifndef __TIME_H__
#define __TIME_H__

#include "Globals.h"

class Timer;

class Time
{
public:
	Time();
	virtual ~Time();

	void UpdateTime();
	void UpdateGameTime();

	void StartGameTimer();
	void StopGameTimer();
	void RestartGameTimer();
	void CleanGameTimer();

	float EditorDT()const;
	uint64 EditorFrames()const;
	float ElapsedTime()const;

	float DT()const;
	uint64 GameFrames()const;

private:

public:

private:
	//Real---------------------------------------
	Timer* msTimer = nullptr;
	Timer* fpsTimer = nullptr; //Dont really usefull
	uint64 frameCount = 0;
	float realDT = 0;
	uint fpsCounter = 0;
	uint32 lastFrameMs = -1; //Dont really usefull
	int lastFPS = -1; //Dont really usefull
	float elapsed = 0.0f;

	//Game----------------------------------------
	Timer* msGameTimer = nullptr;
	uint64 gameFrameCount = 0;
	float gameDT = 0;
	uint gameFPSCounter = 0;

}; 
extern Time* time;

#endif // !__TIME_H__