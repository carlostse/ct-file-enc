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

#include <openssl/sha.h>
#include <QVBoxLayout>
#include <QDebug>
#include "drop_window.h"

namespace ct
{
DropWindow::DropWindow(const LPCTSTR keyFile)
{
    _keyFile = keyFile;

    // window
    auto size = QSize(WIN_W, WIN_H);
    setWindowTitle(tr("CT File Encrypt %1").arg(__VER__));
    setMinimumSize(size);
    setMaximumSize(size);

    // menu
    //QMenuBar *menu = new QMenuBar(this);
    //QMenu* mainHelp = new QMenu(tr("Help"));
    //menu->addMenu(mainHelp);
    //QAction *actAbout = mainHelp->addAction(tr("About"));
    _lblAbout = new ClickableLabel(tr("About"), this);

    // widgets
    _dropArea = new DropArea(QSize(WIN_W, WIN_H - MSG_H - MENU_H), this);
    _dropArea->setText(tr("Drop the file here to encrypt / decrypt"));

    // debug only
//    QPalette pal(_dropArea->palette());
//    pal.setColor(QPalette::Background, Qt::black);
//    _dropArea->setAutoFillBackground(true);
//    _dropArea->setPalette(pal);

    // prepare key
    KeyResult result;
    _masterKey = new MasterKey();
    _masterKey->prepare(&result, _keyFile);

    QString message = (result.type == TYPE_KEY_FILE_LOAD? tr("Load Key: "): tr("New Key: "));
#ifdef _UNICODE
    message.append(QString::fromWCharArray(_keyFile)).append("\n");
#else
    message.append(_keyFile).append("\n");
#endif
    message.append(tr("Fingerprint: "));
    message.append(QString::fromStdString(result.identity).mid(0, 24));
    if (result.type == TYPE_KEY_FILE_SAVE){
        message.append(tr("\n[Warning] Don't lost the key file otherwise you cannot decrypt the files"));
        message.append(tr("\n[Tips] Recommend encrypt the key file with NTFS EFS"));
    }

    _txtMsgBox = new QPlainTextEdit(this);
    size = QSize(WIN_W, MSG_H);
    _txtMsgBox->setMinimumSize(size);
    _txtMsgBox->setMaximumSize(size);
    _txtMsgBox->setFont(QFont(MSG_BOX_FONT));
    _txtMsgBox->appendPlainText(message);

    // layout
    auto* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, PADDING, PADDING, 0);
    mainLayout->addWidget(_lblAbout, 0, Qt::AlignRight);
    mainLayout->addWidget(_dropArea);
    mainLayout->addWidget(_txtMsgBox);
    setLayout(mainLayout);

    // connect signal
    connect(_dropArea, SIGNAL(dropped(const QList<QUrl>)), this, SLOT(droppedFiles(const QList<QUrl>)));
    //connect(actAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
    connect(_lblAbout, SIGNAL(clicked()), this, SLOT(helpAbout()));
}

DropWindow::~DropWindow()
{
    delete _masterKey;
    delete _dropArea;
    delete _txtMsgBox;
}

void DropWindow::droppedFiles(const QList<QUrl>& list)
{
    QString url, fileName, out;
    LPCTSTR inFileName;
    TSTRING outFileName, errMsg;
    std::string action;

    for (int i = 0; i < list.size(); i++) {
        url = list.at(i).path();
        fileName = url.mid(1, url.length() - 1);
#ifdef UNICODE
        inFileName = (LPCTSTR)fileName.utf16();
#else
        QByteArray arr = fileName.toUtf8();
        inFileName = arr.constData();
#endif
#ifdef WIN32
        if (GetFileAttributes(inFileName) == FILE_ATTRIBUTE_DIRECTORY){
            qDebug() << fileName << " is a directory";
            _txtMsgBox->appendPlainText(
                getWarn().append(" ").append(fileName.replace(pathL, pathR)).
                append(tr(" is a directory. Skipped."))
            );
            continue;
        }
#endif
        if (MasterKey::isEncFile(inFileName)){
            action = "dec";
            outFileName = _masterKey->decrypt(inFileName, errMsg);
        } else {
            action = "enc";
            outFileName = _masterKey->encrypt(inFileName, errMsg);
        }
        qDebug() << "file: " << fileName << " [" << QString::fromStdString(action) << "] -> "
                 << QStringFromString(outFileName);

        out = QStringFromString(outFileName.substr(outFileName.find_last_of('/') + 1, std::string::npos));
        _txtMsgBox->appendPlainText(getLock(action == "enc").append(" ").append(out));

        if (errMsg.length() > 0)
            qDebug() << "error: " << QStringFromString(errMsg);
    }
}

void DropWindow::helpAbout()
{
#ifdef Win32
    QSize size = QSize(400, 190);
#else
    QSize size = QSize(490, 200);
#endif
    auto* about = new QTextEdit();

    QPalette pal(about->palette());
    pal.setColor(QPalette::Base, this->palette().window().color());

    about->setWindowFlags(Qt::Dialog);
    about->setWindowTitle(tr("CT File Encrypt %1").arg(__VER__));
    about->setReadOnly(true);
    about->setMinimumSize(size);
    about->setAutoFillBackground(true);
    about->setPalette(pal);
    about->append(
    "<h3>Copyright 2025 Carlos Tse &lt;carlos@aboutmy.info&gt;</h3>\
    <p>\
    Licensed under the Apache License, Version 2.0 (the \"License\");<br>\
    you may not use this file except in compliance with the License.<br>\
    You may obtain a copy of the License at</p>\
    \
        <p><center>http://www.apache.org/licenses/LICENSE-2.0</center></p>\
    \
    <p>Unless required by applicable law or agreed to in writing, software\
    distributed under the License is distributed on an \"AS IS\" BASIS,\
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\
    See the License for the specific language governing permissions and\
    limitations under the License.</p>");

    // append licence
//    QFile file("./LICENCE");
//    QTextStream stream (&file);

//    if (file.open (QIODevice::ReadOnly)){
//        while (!stream.atEnd())
//            about->append(stream.readLine());
//        file.close();
//    }
    about->moveCursor(QTextCursor::Start);
    about->show();
}

QString DropWindow::getLock(const bool locked)
{
    ushort lock[3];
    lock[0] = 0xD83D;
    lock[1] = locked? 0xDD12: 0xDD13;
    lock[2] = 0;
    return QString::fromUtf16(lock);
}

QString DropWindow::getWarn()
{
    ushort warn[2];
    warn[0] = 0x26A0;
    warn[1] = 0;
    return QString::fromUtf16(warn);
}

}
