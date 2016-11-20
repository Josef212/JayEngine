#ifndef __TIME_H__
#define __TIME_H__

#include "Globals.h"

class Timer;

class Time
{
public:
	Time();
	virtual ~Time();

	void updateTime();
	void updateGameTime();

	void startGameTimer();
	void stopGameTimer();
	void restartGameTimer();
	void cleanGameTimer();

	float editorDT()const;
	uint64 editorFrames()const;

	float DT()const;
	uint64 gameFrames()const;

private:

public:

private:
	//Real---------------------------------------
	Timer* msTimer = NULL;
	Timer* fpsTimer = NULL; //Dont really usefull
	uint64 frameCount = 0;
	float realDT = 0;
	uint fpsCounter = 0;
	uint32 lastFrameMs = -1; //Dont really usefull
	int lastFPS = -1; //Dont really usefull

	//Game----------------------------------------
	Timer* msGameTimer = NULL;
	uint64 gameFrameCount = 0;
	float gameDT = 0;
	uint gameFPSCounter = 0;

}; 
extern Time* time;

#endif // !__TIME_H__