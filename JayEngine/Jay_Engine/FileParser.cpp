#include "FileParser.h"

#include "parson.h"
#include "ModuleFileSystem.h"


FileParser::FileParser()
{
	valueRoot = json_value_init_object();
	objRoot = json_value_get_object(valueRoot);
}

FileParser::FileParser(const char* buffer)
{
	if (buffer)
	{
		valueRoot = json_parse_string(buffer);
		if (valueRoot)
			objRoot = json_value_get_object(valueRoot);
	}
}

FileParser::FileParser(JSON_Object* sectionObject) : objRoot(sectionObject)
{
}

FileParser::~FileParser()
{
}

const char* FileParser::getString(const char* name, const char* defaultStr, int index)
{
	JSON_Value* val = getValue(name, index);

	if (val)
		return json_value_get_string(val); //json_value_get_string already check if it has string type
	else
		return defaultStr;
}


JSON_Value* FileParser::getValue(const char* name, int index)
{
	JSON_Value* ret = NULL;

	if (index < 0)
		ret = json_object_get_value(objRoot, name);
	else
	{
		JSON_Array* arr = json_object_get_array(objRoot, name);
		if (arr)
			ret = json_array_get_value(arr, index);
	}

	return ret;
}