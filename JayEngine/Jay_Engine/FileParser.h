#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include "Globals.h"
#include "MathGeoLib\include\Math\float3.h"
#include "Color.h"

typedef struct json_object_t JSON_Object;
typedef struct json_value_t  JSON_Value;
//typedef struct json_array_t  JSON_Array;

class FileParser
{
public:
	FileParser();
	FileParser(const char* buffer);
	FileParser(JSON_Object* section);
	virtual ~FileParser();

	FileParser getSection(const char* sectionName);
	FileParser addSection(const char* sectionName);

	const char* getString(const char* name, const char* defaultStr, int index = -1);//TODO
	bool getBool(const char* name, bool defaultBool, int index = -1);
	int getInt(const char* name, int defaultInt, int index = -1);
	float getFloat(const char* name, float defaultFloat, int index = -1);

	double getDouble(const char* name, double defaultDouble, int index = -1);
	float* getFloatArray(const char* name); //Get floats from array  with index

	float3 getFloat3(const char* name, float3 default);
	Color getColor(const char* name, Color default);
	//getArray
	//getUID
	//FileParser getArray...

	bool addString(const char* name, const char* value);
	bool addBool(const char* name, bool value);
	bool addInt(const char* name, int value);
	bool addFloat(const char* name, float value);
	bool addDouble(const char* name, double value);

	bool addFloat3(const char* name, float3 vec);
	bool addColor(const char* name, Color col);

	bool addIntArray(const char* name, int* iArray, uint size);
	bool addFloatArray(const char* name, float* fArray, uint size);
	bool addBoolArray(const char* name, bool* bArray, uint size);
	bool addStringArray(const char* name, const char** sArray, uint size);
	//addArray
	//addUID
	//getArray(FileParser)...

	uint writeJson(char** buffer, bool fastMode = true);

private:
	JSON_Value* getVal(const char* valName, int index);
	uint writeStyled(char** buffer);
	uint writeFast(char** buffer);

private:
	JSON_Object* objRoot = NULL;
	JSON_Value* valRoot = NULL;
	bool clean = false;
};


#endif //! __FILEPARSER_H__