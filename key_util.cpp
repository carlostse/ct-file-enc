#include "key_util.h"
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#ifdef WIN32
#include <windows.h>
#endif

KeyUtil::KeyUtil()
{
    _key = NULL;
    _iv = NULL;
}

KeyUtil::~KeyUtil()
{
    if (_key){
        free(_key);
        _key = NULL;
    }

    if (_iv){
        free(_iv);
        _iv = NULL;
    }
}

void KeyUtil::prepareKeyIv(const char *keyFile)
{
    std::cout << "key file: " << keyFile << std::endl;

    if (isKeyExists(keyFile)){
        std::cout << "load from key file" << std::endl;
        ByteArray *array = Util::readFile(keyFile);

        setKey((byte *)malloc(KeyUtil::KEY_LENGTH));
        setIv((byte *)malloc(KeyUtil::IV_LENGTH));
        memcpy(_key, array->data(), KeyUtil::KEY_LENGTH);
        memcpy(_iv, &array->data()[KeyUtil::KEY_LENGTH], KeyUtil::IV_LENGTH);
        delete array;

    } else {
        generate();
        // save to key file
        ByteArray array(KeyUtil::KEY_LENGTH + KeyUtil::IV_LENGTH);
        memcpy(array.data(), _key, KeyUtil::KEY_LENGTH);
        memcpy(&array.data()[KeyUtil::KEY_LENGTH], _iv, KeyUtil::IV_LENGTH);
        Util::writeFile(keyFile, &array);
        std::cout << "save new key file" << std::endl;
    }
}

bool KeyUtil::isKeyExists(const char *keyFile)
{
    return Util::isFileExists(keyFile);
}

void KeyUtil::generate()
{
    //std::cout << "generate key" << std::endl;
    setKey((byte *)malloc(KeyUtil::KEY_LENGTH));
    RAND_bytes(_key, KeyUtil::KEY_LENGTH);

    //std::cout << "generate iv" << std::endl;
    setIv((byte *)malloc(KeyUtil::IV_LENGTH));
    RAND_bytes(_iv, KeyUtil::KEY_LENGTH);
}

bool KeyUtil::isEncFile(const QString fileName)
{
    int extLen = fileName.length() - strlen(KEY_ENC_EXT);
    return fileName.lastIndexOf(KEY_ENC_EXT) == extLen;
}

bool KeyUtil::isEncFile(const std::string fileName)
{
    return fileName.find_last_of(KEY_ENC_EXT) == fileName.length() - 1;
}

bool KeyUtil::encrypt(FILE *in, FILE *out, const byte *key, const byte *iv, const byte mode)
{
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, cipher, 0, key, iv, mode);

    byte *ibuf = (byte *)malloc(AES_BLOCK_SIZE);
    byte *obuf = (byte *)malloc(AES_BLOCK_SIZE + EVP_CIPHER_block_size(cipher));

    // read in buffer
    int ilen, olen, ret;
    while ((ilen = fread(ibuf, 1, AES_BLOCK_SIZE, in)) > 0) {
        // encrypt or decrypt depends on ctx
        ret = EVP_CipherUpdate(&ctx, obuf, &olen, ibuf, ilen);
        if (ret != 1) {
            std::cout << "EVP_CipherUpdate failed" << std::endl;
            free(ibuf);
            free(obuf);
            EVP_CIPHER_CTX_cleanup(&ctx);
            return false;
        }
        // write to output file
        fwrite(obuf, 1, olen, out);
    }
    // done reading

    // clear up any last bytes left in the output buffer
    ret = EVP_CipherFinal_ex(&ctx, obuf, &olen);
    EVP_CIPHER_CTX_cleanup(&ctx);

    if (ret != 1){
        std::cout << "EVP_CipherUpdate failed" << std::endl;
        free(ibuf);
        free(obuf);
        return false;
    }

    // write the last bytes out and close
    fwrite(obuf, 1, olen, out);

    free(ibuf);
    free(obuf);
    return true;
}

QString KeyUtil::encrypt(const QString fileName, QString &errMsg)
{
    QString result = "";

    if (!QFile(fileName).exists()){
        errMsg = fileName + QString(" does not exists");
        return result;
    }

    QString outFileName = fileName + QString(KEY_ENC_EXT);

    FILE *in, *out;
#ifdef WIN32
    const wchar_t
            *inWc = (const wchar_t *)fileName.utf16(),
            *outWc = (const wchar_t *)outFileName.utf16();

    in = _wfopen(inWc, L"rb");
    out = _wfopen(outWc, L"wb");
#endif

    if (encrypt(in, out, _key, _iv, AES_ENCRYPT))
        result = outFileName;
    else
        errMsg = fileName + " encrypt failed";

    fclose(in);
    fclose(out);
    return result;
}

QString KeyUtil::decrypt(const QString fileName, QString &errMsg)
{
    QString result = "";

    if (!QFile(fileName).exists()){
        errMsg = fileName + " does not exists";
        return result;
    }

    if (!isEncFile(fileName)){
        errMsg = fileName + " is not an encrypted file";
        return result;
    }

    QString outFileName = fileName.mid(0, fileName.length() - strlen(KEY_ENC_EXT));

    FILE *in, *out;
#ifdef WIN32
    const wchar_t
            *inWc = (const wchar_t *)fileName.utf16(),
            *outWc = (const wchar_t *)outFileName.utf16();

    in = _wfopen(inWc, L"rb");
    out = _wfopen(outWc, L"wb");
#endif

    if (encrypt(in, out, _key, _iv, AES_DECRYPT))
        result = outFileName;

    fclose(in);
    fclose(out);
    return result;
}

byte *KeyUtil::key()
{
    return this->_key;
}

void KeyUtil::setKey(byte *b)
{
    if (_key){
        std::cout << "delete old key" << std::endl;
        free(_key);
    }
    _key = b;
}

byte *KeyUtil::iv()
{
    return this->_iv;
}

void KeyUtil::setIv(byte *b)
{
    if (_iv){
        std::cout << "delete old iv" << std::endl;
        free(_iv);
    }
    _iv = b;
}
