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
#include "key_util.h"
#include <openssl/sha.h>

namespace ct
{
DropWindow::DropWindow(const char *keyFile)
{
    QSize size;

    setWindowTitle(tr("CT File Encrypt %1").arg(__VER__));
    size = QSize(250, 250);
    setMinimumSize(size);
    setMaximumSize(size);
    _keyFile = keyFile;

    // menu
    QMenuBar *menu = new QMenuBar(this);
    QMenu* mainHelp = new QMenu(tr("Help"));
    menu->addMenu(mainHelp);
    QAction *actAbout = mainHelp->addAction(tr("About"));

    // widgets
    _dropArea = new DropArea(QSize(250, 180 - MENU_H), this);
    _dropArea->setText(tr("Drop the file here to encrypt / decrypt"));

    // prepare key
    KeyResult result;
    _keyUtil = new KeyUtil();
    _keyUtil->prepareKeyIv(&result, _keyFile);

    QString message = (result.type == TYPE_KEY_FILE_LOAD? tr("Load Key: "): tr("New Key: "));
    message.append(_keyFile).append("\n");
    message.append(tr("Fingerprint: "));
    message.append(QString::fromStdString(result.identity).mid(0, 24));
    if (result.type == TYPE_KEY_FILE_SAVE){
        message.append("\n");
        message.append(tr("[Warning] Don't lost the key file otherwise you cannot decrypt the files"));
    }

    _msgBox = new QPlainTextEdit(this);
    size = QSize(250, 70);
    _msgBox->setMinimumSize(size);
    _msgBox->setMaximumSize(size);
    _msgBox->setFont(QFont("Segoe UI Symbol"));
    _msgBox->appendPlainText(message);

    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, MENU_H, 0 ,0);
    mainLayout->addWidget(_dropArea);
    mainLayout->addWidget(_msgBox);
    setLayout(mainLayout);

    // connect signal
    connect(_dropArea, SIGNAL(dropped(const QList<QUrl>)), this, SLOT(droppedFiles(const QList<QUrl>)));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
}

DropWindow::~DropWindow()
{
    delete _keyUtil;
    delete _dropArea;
    delete _msgBox;
}

void DropWindow::droppedFiles(const QList<QUrl> list)
{
    QString url, fileName, outFileName, out, errMsg;
    std::string action;

    for (int i = 0; i < list.size(); i++) {
        url = list.at(i).path();
        fileName = url.mid(1, url.length() - 1);

        if (KeyUtil::isEncFile(fileName)){
            action = "dec";
            outFileName = _keyUtil->decrypt(fileName, errMsg);
        } else {
            action = "enc";
            outFileName = _keyUtil->encrypt(fileName, errMsg);
        }
        qDebug() << "file: " << fileName << " [" << QString::fromStdString(action) << "] -> " << outFileName;

        out = outFileName.mid(outFileName.lastIndexOf("/") + 1, outFileName.length() - 1);
        _msgBox->appendPlainText(getLock(action == "enc").append(" ").append(out));

        if (errMsg.length() > 0)
            qDebug() << "error: " << errMsg;
    }
}

void DropWindow::helpAbout()
{
    std::cout << "about" << std::endl;
//    QMessageBox::aboutQt(this, tr("About"));
    QDialog(this);
}

QString DropWindow::getLock(const bool locked)
{
    ushort lock[3];
    lock[0] = 0xD83D;
    lock[1] = locked? 0xDD12: 0xDD13;
    lock[2] = 0;
    return QString::fromUtf16(lock);
}
}
