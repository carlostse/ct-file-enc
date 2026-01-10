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

#include "util.h"
//#include <sys/stat.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#ifndef WIN32
#include <pwd.h>
#include <unistd.h>
#endif

namespace ct
{
std::string Util::hexString(const byte* b, const size_t len)
{
    if (!b)
        return "";

    const auto hex = static_cast<char*>(malloc(len * 2 + 1));
    for (uint i = 0; i < len; i++){
        sprintf(&hex[i * 2], "%02x", b[i]);
    }
    hex[len * 2] = 0;
    auto result = std::string(hex);
    free(hex);
    return result;
}

std::string Util::hexString(const ByteArray* array)
{
    return hexString(array->data(), array->size());
}

byte* Util::key(const std::string& str)
{
    const auto digest = new byte[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char* >(str.c_str()), str.length(), digest);
    return digest;
}

byte* Util::iv(const std::string& str)
{
    const auto digest = new byte[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char* >(str.c_str()), str.length(), digest);
    return digest;
}

bool Util::isFileExists(const LPCTSTR fileName)
{
//    struct stat buf;
//    return stat(fileName, &buf) != -1;
    const std::ifstream file(fileName);
    return file.good();
}

ByteArray* Util::readFile(const LPCTSTR fileName)
{
    if (!isFileExists(fileName)){
        std::cout << fileName << " does not exists" << std::endl;
        return nullptr;
    }

    std::ifstream file(fileName);
    file.seekg(0, std::ios::end);
    const size_t len = file.tellg(); // the key file is small
    file.seekg(0, std::ios::beg);

    if (len < 1){
        std::cout << fileName << " empty" << std::endl;
        return nullptr;
    }

    auto *array = new ByteArray(len);
    file.read(reinterpret_cast<char*>(array->data()), len);
    file.close();

    return array;
}

void Util::writeFile(const LPCTSTR fileName, const ByteArray* data)
{
    std::ofstream file(fileName, std::ios::out | std::ios::binary);
    file.seekp(0);
    file.write(reinterpret_cast<char*>(data->data()), data->size());
    file.close();
}

int Util::lastIndexOf(const char* str, const char c)
{
    int len = static_cast<int>(std::strlen(str));
    while (len >= 0){
        if (str[len] == c){
            return len;
        }
        len--;
    }
    return -1;
}

bool Util::homePath(TCHAR* path, DWORD size)
{
#ifdef WIN32
    HANDLE hToken;
    return OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken) &&
           GetUserProfileDirectory(hToken, path, &size);
#else
    struct passwd *pw = getpwuid(getuid());
    strcpy(path, pw->pw_dir);
    return strlen(path) > 0;
#endif
}
/*
std::string Util::getExeFileName()
{
#ifdef WIN32
    wchar_t wc[MAX_PATH];
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, wc, MAX_PATH);
    WideCharToMultiByte(CP_UTF8, 0, wc, MAX_PATH, buffer, MAX_PATH, 0, 0);
    return std::string(buffer);
#endif
}

std::string Util::getExePath()
{
#ifdef WIN32
    std::string f = getExeFileName();
    return f.substr(0, f.find_last_of("\\/"));
#endif
}
*/
}
