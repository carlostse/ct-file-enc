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

#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

#define __VER__ "1.1.2"

#define KEY_FILE_NAME TEXT("key")
#define KEY_ENC_EXT TEXT(".enc")
#define KEY_DEC_EXT TEXT(".dec")
#define KEY_EXT_LEN 4

#define TYPE_KEY_FILE_LOAD 'L'
#define TYPE_KEY_FILE_SAVE 'S'

#ifndef uint
typedef unsigned int uint;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <userenv.h>
#include <tchar.h>
#else
#define TEXT(str) str
#define _tcslen(txt) strlen(txt)
#define _tcscpy(dest, src) strcpy(dest, src)
#define _tcscat(dest, src) strcat(dest, src)
#define _tcscmp(txt1, txt2) strcmp(txt1, txt2)
typedef unsigned long DWORD;
typedef char TCHAR;
typedef const TCHAR* LPCTSTR;
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#endif

#ifdef _UNICODE
typedef std::wstring TSTRING;
#define QStringFromString(str) QString::fromStdWString(str)
#else
typedef std::string TSTRING;
#define QStringFromString(str) QString::fromStdString(str)
#endif

#ifdef WIN32
#define MSG_BOX_FONT "Segoe UI Symbol"
#else
#define MSG_BOX_FONT "Helvetica"
#endif

#endif // CONSTANT_H
