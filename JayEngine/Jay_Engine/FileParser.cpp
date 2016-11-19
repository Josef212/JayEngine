#include "FileParser.h"

#include <sstream>

FileParser::FileParser()
{
}

FileParser::FileParser(const char* buffer)
{
	if (buffer)
	{
	}
}

FileParser::FileParser(void* t)
{
}

FileParser::~FileParser()
{
}

//------------------------------------------------
//---------------Getters--------------------------
//------------------------------------------------

FileParser FileParser::getSection(const char* sectionName)
{
	//assert(sectionName);
	return FileParser();
}

const char* FileParser::getString(const char* name, const char* defaultStr, int index)
{
	return defaultStr;
}

bool FileParser::getBool(const char* name, bool defaultBool, int index)
{
	return defaultBool;
}

int FileParser::getInt(const char* name, int defaultInt, int index)
{
	return defaultInt;
}

float FileParser::getFloat(const char* name, float defaultFloat, int index)
{
	return defaultFloat;
}

//--------------------------------------

//------------------------------------------------
//---------------Setters--------------------------
//------------------------------------------------

FileParser FileParser::addSection(const char* sectionName)
{
	return FileParser();
}

bool FileParser::addString(const char* name, const char* value)
{
	return false;
}

bool FileParser::addBool(const char* name, bool value)
{
	return false;
}

bool FileParser::addInt(const char* name, int value)
{
	return false;
}

bool FileParser::addFloat(const char* name, float value)
{
	return false;
}


//--------------------------------------

uint FileParser::writeJson(std::string& stream, bool fastMode)
{
	return (fastMode) ? (writeFast(stream)) : (writeStyled(stream));
}

uint FileParser::writeFast(std::string& stream)
{
	uint ret = 0;

	return ret;
}

uint FileParser::writeStyled(std::string& stream)
{
	uint ret = 0;

	return ret;
}

void FileParser::test()
{
}