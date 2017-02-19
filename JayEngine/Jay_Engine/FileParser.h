#ifndef __FILEPARSER__
#define __FILEPARSER__

#include "Globals.h"
#include "MathGeoLib\include\Math\float3.h"
#include "Color.h"

typedef struct json_object_t JSON_Object;
typedef struct json_value_t  JSON_Value;
typedef struct json_array_t  JSON_Array;

class FileParser
{
public:
	FileParser();
	FileParser(const char* buffer);
	FileParser(JSON_Object* section);
	virtual ~FileParser();

	FileParser GetSection(const char* sectionName);
	FileParser AddSection(const char* sectionName);

	const char* GetString(const char* name, const char* defaultStr, int index = -1);//TODO
	bool GetBool(const char* name, bool defaultBool, int index = -1);
	int GetInt(const char* name, int defaultInt, int index = -1);
	float GetFloat(const char* name, float defaultFloat, int index = -1);

	double GetDouble(const char* name, double defaultDouble, int index = -1);
	float* GetFloatArray(const char* name); //Get floats from array  with index

	FileParser GetArray(const char* name, int index)const;
	int GetArraySize(const char* name)const;

	float3 GetFloat3(const char* name, float3 default);
	Color GetColor(const char* name, Color default);

	bool AddString(const char* name, const char* value);
	bool AddBool(const char* name, bool value);
	bool AddInt(const char* name, int value);
	bool AddFloat(const char* name, float value);
	bool AddDouble(const char* name, double value);

	bool AddArray(const char* name);
	bool AddArrayEntry(const FileParser& file);

	bool AddFloat3(const char* name, float3 vec);
	bool AddColor(const char* name, Color col);

	bool AddIntArray(const char* name, int* iArray, uint size);
	bool AddFloatArray(const char* name, float* fArray, uint size);
	bool AddFloatArray(const char* name, const float* fArray, uint size);
	bool AddBoolArray(const char* name, bool* bArray, uint size);
	bool AddStringArray(const char* name, const char** sArray, uint size);

	uint WriteJson(char** buffer, bool fastMode = true);

private:
	JSON_Value* GetVal(const char* valName, int index);
	uint WriteStyled(char** buffer);
	uint WriteFast(char** buffer);

private:
	JSON_Object* objRoot = nullptr;
	JSON_Value* valRoot = nullptr;
	JSON_Array* array = nullptr;
	bool clean = false;
};


#endif //! __FILEPARSER__