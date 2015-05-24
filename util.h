#ifndef UTIL_H
#define UTIL_H
/* Copyright 2015 Carlos Tse <copperoxide@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdint.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "constant.h"
#include "byte_array.h"

namespace ct
{
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
    static std::string getHomePath();
    static std::string getExeFileName();
    static std::string getExePath();
};
}

#endif // UTIL_H
