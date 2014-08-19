#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "byte_array.h"

#ifndef uint
typedef unsigned int uint;
#endif

class Util
{
public:
    static std::string hexString(byte *, uint32_t);
    static std::string hexString(ByteArray *);
    static byte *key(std::string);
    static byte *iv(std::string);
    static bool isFileExists(const char *fileName);
    static ByteArray *readFile(const char *fileName);
    static void writeFile(const char* fileName, ByteArray *data);
    static int lastIndexOf(const char *, char);
    static std::string getExeFileName();
    static std::string getExePath();
};

#endif // UTIL_H
