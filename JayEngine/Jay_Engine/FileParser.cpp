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

FileParser::FileParser(Json::Value& sectionObject) : root(sectionObject)
{
}

FileParser::~FileParser()
{
}

FileParser FileParser::getSection(const char* sectionName)
{
	//assert(sectionName);
	return FileParser(root[sectionName]);
}

const char* FileParser::getString(const char* name, const char* defaultStr, int index)
{
	/*if (index < 0)
	{
		std::string ret = root.get(name, defaultStr).asString();
		return ret.c_str();
	}
	else
	{
		Json::Value array = root[name];
		return array[index].asCString();
	}*/
	return defaultStr;
}

std::string FileParser::getStdString(const char* name, const char* defaultStr, int index)
{
	if (index < 0)
	{
		return root.get(name, defaultStr).asString();
	}
	else
	{
		Json::Value array = root[name];
		return array[index].asCString();
	}
	return defaultStr;
}

bool FileParser::getBool(const char* name, bool defaultBool, int index)
{
	if (index < 0)
	{
		return root.get(name, defaultBool).asBool();
	}
	else
	{
		Json::Value array = root[name];
		return array[index].asBool();
	}
	return defaultBool;
}

int FileParser::getInt(const char* name, int defaultInt, int index)
{
	if (index < 0)
		return root.get(name, defaultInt).asInt();
	else
	{
		Json::Value array = root[name];
		return array[index].asInt();
	}
	return defaultInt;
}

float FileParser::getFloat(const char* name, float defaultFloat, int index)
{
	if (index < 0)
		return root.get(name, defaultFloat).asFloat();
	else
	{
		Json::Value array = root[name];
		return array[index].asFloat();
	}
	return defaultFloat;
}

//--------------------------------------

Json::Value* FileParser::getValue(const char* name, int index)
{
	return &root.get(name, "error");
}