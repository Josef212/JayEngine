#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include "Globals.h"
#include "jsoncpp\json\json.h"

//struct Value;

class FileParser
{
public:
	FileParser();
	FileParser(const char* buffer);
	FileParser(Json::Value* sectionObject);
	virtual ~FileParser();

	FileParser getSection(const char* sectionName);

	const char* getString(const char* name, const char* defaultStr, int index = -1);
	bool getBool(const char* name, bool defaultBool, int index = -1);
	int getInt(const char* name, int defaultInt, int index = -1);
	float getFloat(const char* name, float defaultFloat, int index = -1);

private:

public:
	Json::Value* getValue(const char* name, int index = -1);
//private:
	Json::Value root = NULL;
};


#endif //! __FILEPARSER_H__