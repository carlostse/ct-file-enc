#ifndef KEYUTIL_H
#define KEYUTIL_H

#include <QtCore>
#include "util.h"

#define KEY_FILE_NAME "key"
#define KEY_ENC_EXT ".enc"
#define KEY_DEC_EXT ".dec"

class KeyUtil
{
private:
    byte *_key, *_iv;
    bool isKeyExists(const char *);
    void generate();
    void setKey(byte *);
    void setIv(byte *);
    bool encrypt(FILE *in, FILE *out, const byte *key, const byte *iv, const byte mode);
public:
    static const int KEY_LENGTH = 32;
    static const int IV_LENGTH = 16;
    KeyUtil();
    ~KeyUtil();
    void prepareKeyIv(const char *keyFile);
    static bool isEncFile(const QString fileName);
    static bool isEncFile(const std::string fileName);
    QString encrypt(const QString fileName, QString &errMsg);
    QString decrypt(const QString fileName, QString &errMsg);
    byte *key();
    byte *iv();
};

#endif // KEYUTIL_H
