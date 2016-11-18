#include "FileParser.h"

#include <sstream>
#include "jsoncpp\json\json.h"

FileParser::FileParser()
{
	root = Json::Value((Json::objectValue));
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

bool FileParser::addSection(const char* sectionName)
{
	
	return false;
}

bool FileParser::addString(const char* name, const char* value)
{
	return true;
}

bool FileParser::addBool(const char* name, bool value)
{
	if (name)
	{		
		root[name] = value; //TODO: check if value is initialized
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

uint FileParser::writeJson(std::string& stream, bool fastMode)
{
	/*uint ret = 0;

	if (fastMode)
		ret = writeFast(stream);
	else
		ret = writeStyled(stream);
	
	return ret;*/
	return (fastMode) ? (writeFast(stream)) : (writeStyled(stream));
}

uint FileParser::writeFast(std::string& stream)
{
	uint ret = 0;

	Json::FastWriter writer;
	stream.assign(writer.write(root));

	ret = stream.size();

	return ret;
}

uint FileParser::writeStyled(std::string& stream)
{
	uint ret = 0;

	Json::StyledWriter writer;
	stream.assign(writer.write(root));

	ret = stream.size();

	return ret;
}