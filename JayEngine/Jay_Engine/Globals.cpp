#include "Globals.h"
#include "SDL\include\SDL.h"
#include "Application.h"

void _log(logType type, const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	//OutputDebugString(tmp_string2);

	SDL_Log(tmp_string2);

	if(app)
	{
		sprintf_s(tmp_string2, 4096, "%s", tmp_string);
		app->Log(tmp_string2, type);
	}
}

char* clearTexPath(char* path)
{
	if (!path)
		return NULL;

	uint size = strlen(path);

	if (size > 0)
	{
		char* it = path;
		char* start = path;

		it = start + size;

		while (*(it - 1) != '\\' && *(it - 1) != '/' && it != start)
			--it;

		return it;
	}
	else
		return NULL;
}