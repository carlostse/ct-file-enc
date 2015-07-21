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
#include "master_key.h"
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

namespace ct
{
MasterKey::MasterKey()
{
    _key = NULL;
    _iv = NULL;
}

MasterKey::~MasterKey()
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

void MasterKey::prepare(KeyResult *result, LPCTSTR keyFile)
{
    std::cout << "key file: " << keyFile << std::endl;

    if (exists(keyFile)){
        result->type = TYPE_KEY_FILE_LOAD;
        std::cout << "load from key file" << std::endl;

        ByteArray *array = Util::readFile(keyFile);
        setKey((byte *)malloc(KEY_LENGTH));
        setIv((byte *)malloc(IV_LENGTH));
        memcpy(_key, array->data(), KEY_LENGTH);
        memcpy(_iv, &array->data()[KEY_LENGTH], IV_LENGTH);
        delete array;
    } else {
        result->type = TYPE_KEY_FILE_SAVE;
        generate();
        // save to key file
        ByteArray array(KEY_LENGTH + IV_LENGTH);
        memcpy(array.data(), _key, KEY_LENGTH);
        memcpy(&array.data()[KEY_LENGTH], _iv, IV_LENGTH);
        Util::writeFile(keyFile, &array);
        std::cout << "save new key file" << std::endl;
    }
    result->identity = Util::hexString(_key, KEY_LENGTH);
}

bool MasterKey::exists(LPCTSTR keyFile) const
{
    return Util::isFileExists(keyFile);
}

void MasterKey::generate()
{
    //std::cout << "generate key" << std::endl;
    setKey((byte *)malloc(KEY_LENGTH));
    RAND_bytes(_key, KEY_LENGTH);

    //std::cout << "generate iv" << std::endl;
    setIv((byte *)malloc(IV_LENGTH));
    RAND_bytes(_iv, IV_LENGTH);
}

bool MasterKey::isEncFile(const QString fileName)
{
    int extLen = fileName.length() - strlen(KEY_ENC_EXT);
    return fileName.lastIndexOf(KEY_ENC_EXT) == extLen;
}

bool MasterKey::encrypt(FILE *in, FILE *out, const byte *key, const byte *iv, const byte mode, byte *sha)
{
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, cipher, 0, key, iv, mode);

    byte *ibuf = (byte *)malloc(AES_BLOCK_SIZE);
    byte *obuf = (byte *)malloc(AES_BLOCK_SIZE + EVP_CIPHER_block_size(cipher));

    // calac the SHA-1 of the source
    SHA_CTX shaCtx;
    if (sha)
        SHA1_Init(&shaCtx);

    // read in buffer
    int ilen, olen, ret;
    while ((ilen = fread(ibuf, 1, AES_BLOCK_SIZE, in)) > 0) {
        // update SHA-1
        if (sha)
            SHA1_Update(&shaCtx, ibuf, ilen);

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

    // finalize SHA-1
    if (sha){
        SHA1_Final(sha, &shaCtx);
    }

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

QString MasterKey::encrypt(const QString fileName, QString &errMsg, byte *sha)
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

    if (encrypt(in, out, _key, _iv, AES_ENCRYPT, sha))
        result = outFileName;
    else
        errMsg = fileName + " encrypt failed";

    fclose(in);
    fclose(out);
    return result;
}

QString MasterKey::decrypt(const QString fileName, QString &errMsg)
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

#pragma region setters and getters

byte *MasterKey::key()
{
    return this->_key;
}

void MasterKey::setKey(byte *b)
{
    if (_key){
        std::cout << "delete old key" << std::endl;
        free(_key);
    }
    _key = b;
}

byte *MasterKey::iv()
{
    return this->_iv;
}

void MasterKey::setIv(byte *b)
{
    if (_iv){
        std::cout << "delete old iv" << std::endl;
        free(_iv);
    }
    _iv = b;
}

#pragma endregion
}