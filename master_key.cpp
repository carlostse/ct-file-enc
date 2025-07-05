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

#include "master_key.h"
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

namespace ct
{
MasterKey::MasterKey()
{
    _key = nullptr;
    _iv = nullptr;
}

MasterKey::~MasterKey()
{
    if (_key){
        free(_key);
        _key = nullptr;
    }

    if (_iv){
        free(_iv);
        _iv = nullptr;
    }
}

void MasterKey::prepare(KeyResult *result, const LPCTSTR keyFile)
{
    std::cout << "key file: " << keyFile << std::endl;

    if (exists(keyFile)){
        result->type = TYPE_KEY_FILE_LOAD;
        std::cout << "load from key file" << std::endl;

        const ByteArray* array = Util::readFile(keyFile);
        setKey(static_cast<byte*>(malloc(KEY_LENGTH)));
        setIv(static_cast<byte*>(malloc(IV_LENGTH)));
        memcpy(_key, array->data(), KEY_LENGTH);
        memcpy(_iv, &array->data()[KEY_LENGTH], IV_LENGTH);
        delete array;
    } else {
        result->type = TYPE_KEY_FILE_SAVE;
        generate();
        // save to key file
        const ByteArray array(KEY_LENGTH + IV_LENGTH);
        memcpy(array.data(), _key, KEY_LENGTH);
        memcpy(&array.data()[KEY_LENGTH], _iv, IV_LENGTH);
        Util::writeFile(keyFile, &array);
        std::cout << "save new key file" << std::endl;
    }
    result->identity = Util::hexString(_key, KEY_LENGTH);
}

bool MasterKey::exists(const LPCTSTR keyFile) {
    return Util::isFileExists(keyFile);
}

void MasterKey::generate()
{
    //std::cout << "generate key" << std::endl;
    setKey(static_cast<byte*>(malloc(KEY_LENGTH)));
    RAND_bytes(_key, KEY_LENGTH);

    //std::cout << "generate iv" << std::endl;
    setIv(static_cast<byte*>(malloc(IV_LENGTH)));
    RAND_bytes(_iv, IV_LENGTH);
}

bool MasterKey::isEncFile(const LPCTSTR fileName)
{
    const size_t
    lenExt = _tcslen(KEY_ENC_EXT),
    lenFilename = _tcslen(fileName);

    if (lenFilename < lenExt) return false;

    TCHAR ext[KEY_EXT_LEN + 1];
    _tcscpy(ext, fileName + lenFilename - lenExt);

    return _tcscmp(ext, KEY_ENC_EXT) == 0;
}

bool MasterKey::encrypt(FILE* in, FILE* out, const byte* key, const byte* iv, const byte mode, byte* sha)
{
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(ctx, cipher, nullptr, key, iv, mode);

    byte *ibuf = static_cast<byte*>(malloc(AES_BLOCK_SIZE));
    byte *obuf = static_cast<byte*>(malloc(AES_BLOCK_SIZE + EVP_CIPHER_block_size(cipher)));

    // calc the SHA-1 of the source
    SHA_CTX shaCtx;
    if (sha)
        SHA1_Init(&shaCtx);

    // read in buffer
    int ilen, olen, ret;
    while ((ilen = static_cast<int>(fread(ibuf, 1, AES_BLOCK_SIZE, in))) > 0) {
        // update SHA-1
        if (sha)
            SHA1_Update(&shaCtx, ibuf, ilen);

        // encrypt or decrypt depends on ctx
        ret = EVP_CipherUpdate(ctx, obuf, &olen, ibuf, ilen);
        if (ret != 1) {
            std::cout << "EVP_CipherUpdate failed" << std::endl;
            free(ibuf);
            free(obuf);
            EVP_CIPHER_CTX_reset(ctx);
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
    ret = EVP_CipherFinal_ex(ctx, obuf, &olen);
    EVP_CIPHER_CTX_reset(ctx);

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

void MasterKey::openFile(FILE** in, FILE** out, LPCTSTR inFileName, LPCTSTR outFileName)
{
#ifdef UNICODE
    *in = _wfopen(inFileName, TEXT("rb"));
    *out = _wfopen(outFileName, TEXT("wb"));
#else
    *in = fopen(inFileName, TEXT("rb"));
    *out = fopen(outFileName, TEXT("wb"));
#endif
}

TSTRING MasterKey::encrypt(const LPCTSTR fileName, TSTRING &errMsg, byte *sha) const
{
    const auto inFileName = TSTRING(fileName);
    TSTRING result;

    if (!Util::isFileExists(fileName)){
        errMsg = inFileName + TEXT(" does not exists");
        return result;
    }

    const TSTRING outFileName = inFileName + KEY_ENC_EXT;

    FILE *in, *out;
    openFile(&in, &out, fileName, outFileName.c_str());

    if (encrypt(in, out, _key, _iv, AES_ENCRYPT, sha))
        result = outFileName;
    else
        errMsg = inFileName + TEXT(" encrypt failed");

    fclose(in);
    fclose(out);
    return result;
}

TSTRING MasterKey::decrypt(const LPCTSTR fileName, TSTRING &errMsg) const
{
    const auto inFileName = TSTRING(fileName);
    TSTRING result;

    if (!Util::isFileExists(fileName)){
        errMsg = inFileName + TEXT(" does not exists");
        return result;
    }

    if (!isEncFile(fileName)){
        errMsg = inFileName + TEXT(" is not an encrypted file");
        return result;
    }

    // restore original extension (just remove KEY_ENC_EXT)
    const size_t
    lenExt = _tcslen(KEY_ENC_EXT),
    lenFileName = _tcslen(fileName),
    lenOutName = lenFileName - lenExt;
    auto *outFileName = new TCHAR[lenOutName + 1];
    memcpy(outFileName, fileName, sizeof(TCHAR) * lenOutName);
    outFileName[lenOutName] = 0;

    FILE *in, *out;
    openFile(&in, &out, fileName, outFileName);

    if (encrypt(in, out, _key, _iv, AES_DECRYPT))
        result = outFileName;
    else
        errMsg = inFileName + TEXT(" decrypt failed");

    fclose(in);
    fclose(out);
    delete[] outFileName;
    return result;
}

#pragma region setters and getters

byte* MasterKey::key() const
{
    return this->_key;
}

void MasterKey::setKey(byte* b)
{
    if (_key){
        std::cout << "delete old key" << std::endl;
        free(_key);
    }
    _key = b;
}

byte* MasterKey::iv() const
{
    return this->_iv;
}

void MasterKey::setIv(byte* b)
{
    if (_iv){
        std::cout << "delete old iv" << std::endl;
        free(_iv);
    }
    _iv = b;
}

#pragma endregion
}
