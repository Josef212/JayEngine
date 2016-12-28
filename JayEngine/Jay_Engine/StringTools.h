#ifndef __STRINGTOOLS_H__
#define __STRINGTOOLS_H__

#include <string>
#include "Globals.h"

void clearPath(const char* path, std::string& cleanPath)
{
	if (!path)
		return;

	uint size = strlen(path) + 1;

	if (size > 0)
	{
		char* tmp = new char[size];
		strcpy_s(tmp, size, path);

		if (tmp)
		{
			char* it = tmp;
			char* start = tmp;

			it = start + size;

			while (*(it - 1) != '\\' && *(it - 1) != '/' && it != start)
				--it;

			cleanPath.assign(it);
		}

		RELEASE_ARRAY(tmp);
	}
}

void removeExtension(const char* str, std::string& output)
{
	if (!str)
		return;

	uint size = strlen(str) + 1;
	if (size > 0)
	{
		char* tmp = new char[size];
		strcpy_s(tmp, size, str);

		if (tmp)
		{
			char* it = tmp + size;

			while (*it != '.')
			{
				--it;
				--size;
			}

			if (size < strlen(tmp))
				tmp[size] = '\0';

			output.assign(tmp);
		}

		RELEASE_ARRAY(tmp);
	}
}

#endif // !__STRINGTOOLS_H__