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
    bool exists(LPCTSTR) const;
    void generate();
    void setKey(byte *);
    void setIv(byte *);
    bool encrypt(FILE *in, FILE *out, const byte *key, const byte *iv, const byte mode, byte * = NULL);
    void openFile(FILE **in, FILE **out, LPCTSTR inFileName, LPCTSTR outFileName);
public:
    static const int KEY_LENGTH = 32;
    static const int IV_LENGTH = 16;
    MasterKey();
    ~MasterKey();
    void prepare(KeyResult *, LPCTSTR);
    static bool isEncFile(LPCTSTR);
    TSTRING encrypt(LPCTSTR fileName, TSTRING &errMsg, byte * = NULL);
    TSTRING decrypt(LPCTSTR fileName, TSTRING &errMsg);
    byte *key();
    byte *iv();
};
}

#endif // MASTERKEY_H
