/* Copyright 2019 Carlos Tse <carlos@aboutmy.info>
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

#ifndef UTIL_H
#define UTIL_H

#include "constant.h"
#include "byte_array.h"
#include <iostream>

namespace ct
{
class Util
{
public:
    static std::string hexString(const byte*, const size_t);
    static std::string hexString(const ByteArray*);
    static byte* key(const std::string&);
    static byte* iv(const std::string&);
    static bool isFileExists(const LPCTSTR);
    static ByteArray* readFile(const LPCTSTR);
    static void writeFile(const LPCTSTR, const ByteArray*);
    static int lastIndexOf(const char*, const char);
    static bool homePath(TCHAR*, DWORD);
//    static std::string getExeFileName();
//    static std::string getExePath();
};
}

#endif // UTIL_H
