#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include <stddef.h>
#include <cstdlib>
// #include <crypto++/config.h>

#ifndef byte
typedef unsigned char byte;
#endif

class ByteArray
{
private:
    size_t _size;
    byte *_data;
public:
    ByteArray(size_t);
    ByteArray(size_t, byte *);
    ~ByteArray();
    size_t size();
    byte *data();
    ByteArray *copy();
};

#endif // BYTE_ARRAY_H
