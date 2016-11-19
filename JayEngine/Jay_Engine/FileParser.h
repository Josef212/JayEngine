#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include "Globals.h"
#include "jsoncpp\json\json.h"
#include <string>
//struct Value;

class FileParser
{
public:
	FileParser();
	FileParser(const char* buffer);
	FileParser(void* t);
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

	//uint writeJson(const char* buffer, bool fastMode = true);
	uint writeJson(std::string& stream, bool fastMode = true);

	void test();

private:
	uint writeStyled(std::string& stream);
	uint writeFast(std::string& stream);

public:
//private:
};


#endif //! __FILEPARSER_H__