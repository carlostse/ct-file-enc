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
#include "master_key.h"
#include "clickable_label.h"

#define WIN_H 260
#define WIN_W 260
#define MSG_H 70
#define MENU_H 20
#define PADDING 10

namespace ct
{
const QString pathL("/"), pathR("\\");

class DropWindow : public QWidget
{
    Q_OBJECT

public:
    DropWindow(LPCTSTR);
    virtual ~DropWindow();

public slots:
    void droppedFiles(const QList<QUrl> list);
    void helpAbout();

private:
    LPCTSTR _keyFile;
    MasterKey *_masterKey;
    DropArea *_dropArea;
    ClickableLabel *_lblAbout;
    QPlainTextEdit *_txtMsgBox;
    QString getLock(const bool);
    QString getWarn();
};
}

#endif // DROPWINDOW_H
