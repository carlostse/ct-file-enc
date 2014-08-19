#include "drop_window.h"
#include "key_util.h"

DropWindow::DropWindow(std::string directory)
{
    setWindowTitle(tr("Magicam Decrypt"));
    setMinimumSize(250, 250);
    setMaximumSize(250, 250);

    _directory = directory;
    _keyUtil = new KeyUtil();
    _keyUtil->prepareKeyIv(_directory.c_str());

    _dropArea = new DropArea(this);
    _msgBox = new QLabel(this);
    _msgBox->setFont(QFont("Segoe UI Symbol"));

    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(_dropArea);
    mainLayout->addWidget(_msgBox);
    setLayout(mainLayout);

    // connect signal
    connect(_dropArea, SIGNAL(dropped(const QList<QUrl>)), this, SLOT(droppedFiles(const QList<QUrl>)));
}

DropWindow::~DropWindow()
{
    delete _keyUtil;
    delete _dropArea;
    delete _msgBox;
}

void DropWindow::droppedFiles(const QList<QUrl> list)
{
    QString errMsg;
    std::string action;

    for (int i = 0; i < list.size(); i++) {
        QString url = list.at(i).path();
        QString fileName = url.mid(1, url.length() - 1), outFileName;

        if (KeyUtil::isEncFile(fileName)){
            action = "dec";
            outFileName = _keyUtil->decrypt(fileName, errMsg);
        } else {
            action = "enc";
            outFileName = _keyUtil->encrypt(fileName, errMsg);
        }
        std::cout << "file: " << fileName.toUtf8().data() << " [" << action << "] -> " << outFileName.toUtf8().data() << std::endl;

        QString text = _msgBox->text();
        _msgBox->setText((text.length() > 1? text + "\n": text) + getLock(action == "enc") + outFileName);

        if (errMsg.length() > 0)
            qDebug() << "error: " << errMsg;
    }
}

QString DropWindow::getLock(const bool locked)
{
    ushort lock[3];
    lock[0] = 0xD83D;
    lock[1] = locked? 0xDD12: 0xDD13;
    lock[2] = 0;
    return QString::fromUtf16(lock);
}
