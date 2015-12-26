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
#include "drop_window.h"
#include "util.h"

int main(int argc, char *argv[])
{
    // get the key file path
    TCHAR keyFile[MAX_PATH];
    ct::Util::homePath(keyFile, MAX_PATH);
#ifdef WIN32
    _tcscat(keyFile, TEXT("\\"));
#else
    _tcscat(keyFile, TEXT("/"));
#endif
    _tcscat(keyFile, KEY_FILE_NAME);

    QApplication a(argc, argv);
    ct::DropWindow w(keyFile);
    w.show();
    return a.exec();
}
