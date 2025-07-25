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

#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include "constant.h"

namespace ct
{
class ByteArray
{
private:
    size_t _size;
    byte *_data;
public:
    ByteArray(const size_t);
    ByteArray(const size_t, const byte*);
    ~ByteArray();
    size_t size() const;
    byte* data() const;
    ByteArray* copy() const;
};
}

#endif // BYTE_ARRAY_H
