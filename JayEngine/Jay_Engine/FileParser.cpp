#include "FileParser.h"

#include <sstream>
#include "jsoncpp\json\json.h"

FileParser::FileParser()
{
	root.begin();
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
	root.end();
}

//------------------------------------------------
//---------------Getters--------------------------
//------------------------------------------------

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
	}*/ //TODO: why this doesnt work??
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

//------------------------------------------------
//---------------Setters--------------------------
//------------------------------------------------

bool FileParser::addString(const char* name, const char* value)
{
	return true;
}

bool FileParser::addBool(const char* name, bool value)
{
	if (name)
	{
		root[name] = value;
		return true;
	}

	return false;
}

bool FileParser::addInt(const char* name, int value)
{
	if (name)
	{
		root[name] = value;
		return true;
	}

	return false;
}

bool FileParser::addFloat(const char* name, float value)
{
	if (name)
	{
		root[name] = value;
		return true;
	}

	return false;
}


//--------------------------------------

void FileParser::writeJson(const char* buffer, bool fastMode)
{
	uint ret = 0;

	if (fastMode)
		ret = writeFast(buffer);
	else
		writeStyled(buffer);
	
	//return ret;
	//return (fastMode) ? (writeFast(buffer)) : (writeStyled(buffer));
}

uint FileParser::writeFast(const char* buffer)
{
	uint ret = 0;

	Json::FastWriter writer;
	std::string data = writer.write(root);
	if (data.size() > 0)
	{
		ret = data.size();
		buffer = new char[ret];

		if (!buffer)
			_LOG(LOG_ERROR, "Error writing json.");

		memcpy(&buffer, data.c_str(), ret);
	}
	else
		_LOG(LOG_ERROR, "Error writing json: invalid size.");

	return ret;
}

uint FileParser::writeStyled(const char* buffer)
{
	uint ret = 0;

	Json::StyledWriter writer;
	std::string data = writer.write(root);
	if (data.size() > 0)
	{
		ret = data.size();
		buffer = new char[ret];

		if (!buffer)
			_LOG(LOG_ERROR, "Error writing json.");

		memcpy(&buffer, data.c_str(), ret);
	}
	else
		_LOG(LOG_ERROR, "Error writing json: invalid size.");

	return ret;
}