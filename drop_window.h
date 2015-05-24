#ifndef DROPWINDOW_H
#define DROPWINDOW_H
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
#include "drop_area.h"
#include "key_util.h"

namespace ct
{
class DropWindow : public QWidget
{
    Q_OBJECT

public:
    DropWindow(std::string);
    virtual ~DropWindow();

public slots:
    void droppedFiles(const QList<QUrl> list);

private:
    std::string _keyFile;
    KeyUtil *_keyUtil;
    DropArea *_dropArea;
    QPlainTextEdit *_msgBox;
    QString getLock(const bool);
};
}

#endif // DROPWINDOW_H
