/* Copyright 2016 Carlos Tse <copperoxide@gmail.com>
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

#include "byte_array.h"
#include <string.h>

namespace ct
{
ByteArray::ByteArray(size_t size)
{
    this->_size = size;
    this->_data = (byte *)malloc(size);
}

ByteArray::ByteArray(size_t size, byte *data)
{
    this->_size = size;
    this->_data = (byte *)malloc(size);
    memcpy(this->_data, data, size);
}

ByteArray::~ByteArray()
{
    if (_data){
        free(_data);
        _data = NULL;
    }
}

size_t ByteArray::size()
{
    return this->_size;
}

byte *ByteArray::data()
{
    return this->_data;
}

ByteArray *ByteArray::copy()
{
    return new ByteArray(_size, _data);
}
}
