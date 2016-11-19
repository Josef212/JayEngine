#include "FileParser.h"

#include "parson\parson.h"

//json_value_get_object
//get an object from a value

FileParser::FileParser()
{
	valRoot = json_value_init_object();
	objRoot = json_value_get_object(valRoot);
	//TODO: Memory???
}

FileParser::FileParser(const char* buffer)
{
	if (buffer)
	{
		valRoot = json_parse_string(buffer);
		objRoot = json_value_get_object(valRoot);
		//TODO: Memory???
	}
}

FileParser::FileParser(JSON_Object* section) : objRoot(section)
{
}

FileParser::~FileParser()
{
	/*if(valRoot)
		json_value_free(valRoot);*/
}

//------------------------------------------------
//---------------Getters--------------------------
//------------------------------------------------

JSON_Value* FileParser::getVal(const char* valName, int index)
{
	if (index < 0)
		return json_object_get_value(objRoot, valName);
	else
	{
		JSON_Array* ary = json_object_get_array(objRoot, valName);
		return (ary) ? (json_array_get_value(ary, index)) : (NULL);
	}
}

FileParser FileParser::getSection(const char* sectionName)
{
	//assert(sectionName);
	return FileParser(json_object_get_object(objRoot, sectionName));
}

const char* FileParser::getString(const char* name, const char* defaultStr, int index)
{
	JSON_Value* val = getVal(name, index);
	return (val) ? (json_value_get_string(val)) : (defaultStr);
}

bool FileParser::getBool(const char* name, bool defaultBool, int index)
{
	JSON_Value* val = getVal(name, index);
	return (val) ? (json_value_get_boolean(val)) : (defaultBool);
}

int FileParser::getInt(const char* name, int defaultInt, int index)
{
	JSON_Value* val = getVal(name, index);
	return (val) ? ((int)json_value_get_number(val)) : (defaultInt);
}

float FileParser::getFloat(const char* name, float defaultFloat, int index)
{
	JSON_Value* val = getVal(name, index);
	return (val) ? ((int)json_value_get_number(val)) : (defaultFloat);
}

//--------------------------------------

//------------------------------------------------
//---------------Setters--------------------------
//------------------------------------------------

FileParser FileParser::addSection(const char* sectionName)
{
	json_object_set_value(objRoot, sectionName, json_value_init_object());
	return getSection(sectionName);
}

bool FileParser::addString(const char* name, const char* value)
{
	return json_object_set_string(objRoot, name, value) == JSONSuccess;
}

bool FileParser::addBool(const char* name, bool value)
{
	return json_object_set_boolean(objRoot, name, value) == JSONSuccess;
}

bool FileParser::addInt(const char* name, int value)
{
	return json_object_set_number(objRoot, name, (double)value) == JSONSuccess;
}

bool FileParser::addFloat(const char* name, float value)
{
	return json_object_set_number(objRoot, name, (double)value) == JSONSuccess;
}


//--------------------------------------

uint FileParser::writeJson(char** buffer, bool fastMode)
{
	return (fastMode) ? (writeFast(buffer)) : (writeStyled(buffer));
}

uint FileParser::writeFast(char** buffer)
{
	uint ret = json_serialization_size(valRoot);
	*buffer = new char[ret];
	json_serialize_to_buffer(valRoot, *buffer, ret);

	return ret;
}

uint FileParser::writeStyled(char** buffer)
{
	uint ret = json_serialization_size_pretty(valRoot);
	*buffer = new char[ret];
	json_serialize_to_buffer_pretty(valRoot, *buffer, ret);

	return ret;
}