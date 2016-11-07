#include "FileParser.h"

#include <sstream>
#include "jsoncpp\json\json.h"

FileParser::FileParser()
{
}

FileParser::FileParser(const char* buffer)
{
	if (buffer)
	{
		std::stringstream stream(buffer);
		Json::Reader reader;
		reader.parse(stream, root);
	}
}

FileParser::FileParser(Json::Value* sectionObject) : root(sectionObject)
{
}

FileParser::~FileParser()
{
}

FileParser FileParser::getSection(const char* sectionName)
{
	//assert(sectionName);
	return FileParser(&root[sectionName]);
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

Json::Value* FileParser::getValue(const char* name, int index)
{
	return NULL;
}