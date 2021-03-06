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

JSON_Value* FileParser::GetVal(const char* valName, int index)
{
	if (index < 0)
		return json_object_get_value(objRoot, valName);
	else
	{
		JSON_Array* ary = json_object_get_array(objRoot, valName);
		return (ary) ? (json_array_get_value(ary, index)) : (nullptr);
	}
}

FileParser FileParser::GetSection(const char* sectionName)
{
	//assert(sectionName);
	return FileParser(json_object_get_object(objRoot, sectionName));
}

const char* FileParser::GetString(const char* name, const char* defaultStr, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? (json_value_get_string(val)) : (defaultStr);
}

bool FileParser::GetBool(const char* name, bool defaultBool, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? (json_value_get_boolean(val)) : (defaultBool);
}

int FileParser::GetInt(const char* name, int defaultInt, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? ((int)json_value_get_number(val)) : (defaultInt);
}

float FileParser::GetFloat(const char* name, float defaultFloat, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? ((float)json_value_get_number(val)) : (defaultFloat);
}

double FileParser::GetDouble(const char* name, double defaultDouble, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? ((double)json_value_get_number(val)) : (defaultDouble);
}

float* FileParser::GetFloatArray(const char* name)
{
	//For now we can get a float or any value type from array using the float getter + index
	return nullptr;
}

FileParser FileParser::GetArray(const char* name, int index)const
{
	JSON_Array* array = json_object_get_array(objRoot, name);
	if (array)
		return FileParser(json_array_get_object(array, index));
	return FileParser((JSON_Object*)nullptr);
}

int FileParser::GetArraySize(const char* name)const
{
	int ret = -1;
	JSON_Array* array = json_object_get_array(objRoot, name);
	if (array)
		ret = json_array_get_count(array);
	return ret;
}

float3 FileParser::GetFloat3(const char* name, float3 default)
{
	return float3(GetFloat(name, default.x, 0), GetFloat(name, default.y, 1), GetFloat(name, default.z, 2));
}

Color FileParser::GetColor(const char* name, Color default)
{
	return Color(GetFloat(name, default.r, 0),
		GetFloat(name, default.g, 1),
		GetFloat(name, default.b, 2),
		GetFloat(name, default.a, 3));
}

//--------------------------------------

//------------------------------------------------
//---------------Setters--------------------------
//------------------------------------------------

FileParser FileParser::AddSection(const char* sectionName)
{
	json_object_set_value(objRoot, sectionName, json_value_init_object());
	return GetSection(sectionName);
}

bool FileParser::AddString(const char* name, const char* value)
{
	return json_object_set_string(objRoot, name, value) == JSONSuccess;
}

bool FileParser::AddBool(const char* name, bool value)
{
	return json_object_set_boolean(objRoot, name, value) == JSONSuccess;
}

bool FileParser::AddInt(const char* name, int value)
{
	return json_object_set_number(objRoot, name, (double)value) == JSONSuccess;
}

bool FileParser::AddFloat(const char* name, float value)
{
	return json_object_set_number(objRoot, name, (double)value) == JSONSuccess;
}

bool FileParser::AddDouble(const char* name, double value)
{
	return json_object_set_number(objRoot, name, value) == JSONSuccess;
}

bool FileParser::AddArray(const char* name)
{
	JSON_Value* val = json_value_init_array();
	array = json_value_get_array(val);

	return json_object_set_value(objRoot, name, val) == JSONSuccess;
}

bool FileParser::AddArrayEntry(const FileParser& file)
{
	if (array)
		return json_array_append_value(array, json_value_deep_copy(file.valRoot)) == JSONSuccess;
	return false;
}

bool FileParser::AddIntArray(const char* name, int* iArray, uint size)
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

bool FileParser::AddFloatArray(const char* name, float* fArray, uint size)
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

bool FileParser::AddFloatArray(const char* name, const float* fArray, uint size)
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

bool FileParser::AddBoolArray(const char* name, bool* bArray, uint size)
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

bool FileParser::AddStringArray(const char* name, const char** sArray, uint size)
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

bool FileParser::AddFloat3(const char* name, float3 vec)
{
	return AddFloatArray(name, vec.ptr(), 3);
}

bool FileParser::AddColor(const char* name, Color col)
{
	return AddFloatArray(name, &col, 4);
}

//--------------------------------------

uint FileParser::WriteJson(char** buffer, bool fastMode)
{
	return (fastMode) ? (WriteFast(buffer)) : (WriteStyled(buffer));
}

uint FileParser::WriteFast(char** buffer)
{
	uint ret = json_serialization_size(valRoot);
	*buffer = new char[ret];
	json_serialize_to_buffer(valRoot, *buffer, ret);

	return ret;
}

uint FileParser::WriteStyled(char** buffer)
{
	uint ret = json_serialization_size_pretty(valRoot);
	*buffer = new char[ret];
	json_serialize_to_buffer_pretty(valRoot, *buffer, ret);

	return ret;
}