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

#ifndef MASTERKEY_H
#define MASTERKEY_H

#include "util.h"

namespace ct
{
typedef struct {
    char type;
    std::string identity;
} KeyResult;

class MasterKey
{
private:
    byte *_key, *_iv;

    static bool exists(const LPCTSTR);
    void generate();
    void setKey(byte*);
    void setIv(byte*);
    static bool encrypt(FILE* in, FILE* out, const byte* key, const byte* iv, const byte mode, byte* = nullptr);
    static void openFile(FILE** in, FILE** out, LPCTSTR inFileName, LPCTSTR outFileName);
public:
    static constexpr int KEY_LENGTH = 32;
    static constexpr int IV_LENGTH = 16;
    MasterKey();
    ~MasterKey();
    void prepare(KeyResult*, const LPCTSTR);
    static bool isEncFile(const LPCTSTR);
    TSTRING encrypt(const LPCTSTR fileName, TSTRING &errMsg, byte* = nullptr) const;
    TSTRING decrypt(const LPCTSTR fileName, TSTRING &errMsg) const;
    byte* key() const;
    byte* iv() const;
};
}

#endif // MASTERKEY_H
