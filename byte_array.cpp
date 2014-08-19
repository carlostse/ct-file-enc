#include "byte_array.h"
#include <string.h>

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
