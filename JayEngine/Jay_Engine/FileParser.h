#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include "Globals.h"


typedef struct json_object_t JSON_Object;
//typedef struct json_array_t  JSON_Array;
typedef struct json_value_t  JSON_Value;

class FileParser
{
public:
	FileParser();
	FileParser(const char* buffer);
	FileParser(JSON_Object* sectionObject);
	virtual ~FileParser();


	const char* getString(const char* name, const char* defaultStr, int index = -1);

private:

public:
	JSON_Value* getValue(const char* name, int index = -1);
private:
	JSON_Value* valueRoot = NULL;
	JSON_Object* objRoot = NULL;
};


#endif //! __FILEPARSER_H__