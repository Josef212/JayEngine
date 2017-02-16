#include <stdlib.h>
#include "Globals.h"
#include "Application.h"
#include "Time.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/lib/win32/SDL2.lib" )
#pragma comment( lib, "SDL/lib/win32/SDL2main.lib" )

enum MainStates
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* app = NULL;
Time* time = NULL;

int main(int argc, char ** argv)
{
	_LOG(LOG_STD, "Starting game engine %s...", TITLE);

	int mainReturn = EXIT_FAILURE;
	MainStates state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			_LOG(LOG_STD, "-------------- Application Creation --------------");
			app = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			_LOG(LOG_STD, "-------------- Application Init --------------");
			if (app->Init() == false)
			{
				_LOG(LOG_ERROR, "Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				_LOG(LOG_STD, "-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int updateReturn = app->Update();

			if (updateReturn == UPDATE_ERROR)
			{
				_LOG(LOG_ERROR, "Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (updateReturn == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			_LOG(LOG_STD, "-------------- Application CleanUp --------------");
			if (app->CleanUp() == false)
			{
				_LOG(LOG_ERROR, "Application CleanUp exits with ERROR");
			}
			else
				mainReturn = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	_LOG(LOG_STD, "Exiting game engine %s...\n", TITLE);

	delete app;

	return mainReturn;
}