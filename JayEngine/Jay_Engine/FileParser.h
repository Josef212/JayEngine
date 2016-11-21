#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include "Globals.h"

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
	//getFloat3
	//getArray
	//getColor
	//getUID
	//getDouble
	//FileParser getArray...

	bool addString(const char* name, const char* value);
	bool addBool(const char* name, bool value);
	bool addInt(const char* name, int value);
	bool addFloat(const char* name, float value);
	//addFloat3
	//addArray
	//addColor
	//addUID
	//addDouble
	//getArray(FileParser)
	//addArrayBool
	//addArrayInt
	//addArrayUint
	//addArrayFloat
	//addArrayString...

	uint writeJson(char** buffer, bool fastMode = true);

private:
	JSON_Value* getVal(const char* valName, int index);
	uint writeStyled(char** buffer);
	uint writeFast(char** buffer);

private:
	JSON_Object* objRoot = NULL;
	JSON_Value* valRoot = NULL;
};


#endif //! __FILEPARSER_H__