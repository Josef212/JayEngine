#include "FileParser.h"

#include "parson\parson.h"

//json_value_get_object
//get an object from a value

FileParser::FileParser()
{
	valRoot = json_value_init_object();
	if (valRoot)
	{
		objRoot = json_value_get_object(valRoot);
		clean = true;
	}
}

FileParser::FileParser(const char* buffer)
{
	if (buffer)
	{
		valRoot = json_parse_string(buffer);
		if (valRoot)
		{
			objRoot = json_value_get_object(valRoot);
			clean = true;
		}
	}
}

FileParser::FileParser(JSON_Object* section) : objRoot(section)
{
}

FileParser::~FileParser()
{
	if(clean && valRoot)
		json_value_free(valRoot);
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
	return (val) ? ((float)json_value_get_number(val)) : (defaultFloat);
}

double FileParser::getDouble(const char* name, double defaultDouble, int index)
{
	JSON_Value* val = getVal(name, index);
	return (val) ? ((double)json_value_get_number(val)) : (defaultDouble);
}

float* FileParser::getFloatArray(const char* name)
{
	//For now we can get a float or any value type from array using the float getter + index
	return NULL;
}

float3 FileParser::getFloat3(const char* name, float3 default)
{
	return float3(getFloat(name, default.x, 0), getFloat(name, default.y, 1), getFloat(name, default.z, 2));
}

Color FileParser::getColor(const char* name, Color default)
{
	return Color(getFloat(name, default.r, 0),
		getFloat(name, default.g, 1),
		getFloat(name, default.b, 2),
		getFloat(name, default.a, 3));
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

bool FileParser::addDouble(const char* name, double value)
{
	return json_object_set_number(objRoot, name, value) == JSONSuccess;
}

bool FileParser::addIntArray(const char* name, int* fArray, uint size)
{
	if (!name || !fArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_number(ar, fArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool FileParser::addFloatArray(const char* name, float* iArray, uint size)
{
	if (!name || !iArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_number(ar, iArray[i]);

	json_object_set_value(objRoot, name, val);
	
	return true;
}

bool FileParser::addBoolArray(const char* name, bool* bArray, uint size)
{
	if (!name || !bArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_boolean(ar, bArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool FileParser::addStringArray(const char* name, const char** sArray, uint size)
{
	if (!name || !sArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_string(ar, sArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool FileParser::addFloat3(const char* name, float3 vec)
{
	return addFloatArray(name, vec.ptr(), 3);
}

bool FileParser::addColor(const char* name, Color col)
{
	return addFloatArray(name, &col, 4);
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