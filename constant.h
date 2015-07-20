#ifndef CONSTANT_H
#define CONSTANT_H
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
#define __VER__ "1.0.0"
#define KEY_FILE_NAME "key"
#define KEY_ENC_EXT ".enc"
#define KEY_DEC_EXT ".dec"

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
#else
#define MAX_PATH 260
typedef unsigned long DWORD
#endif

#endif // CONSTANT_H
