#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include <iostream>
#include <cstring>
#include <fstream>

#include "QueryResult.h"
#include "BitmapSet.h"
#include "DeserialType.h"

class QueryProcessor{
public:
    static void serialSampleQuery(BitmapSet* BTMP, char* text, char* file_path, int column);
    static void columnSerialQuery(BitmapSet* BTMP, int* column, int colnumbers, char* file, char* out_file_path);
    static DeType* deserialization(BitmapSet* BTMP, char* text, unsigned int column);
    static bool isNumber(const char* str);
    static bool isScientificNotation(const char* str);
};

#endif