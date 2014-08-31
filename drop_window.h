#ifndef DROPWINDOW_H
#define DROPWINDOW_H

#include "drop_area.h"
#include "key_util.h"

class DropWindow : public QWidget
{
    Q_OBJECT

public:
    DropWindow(std::string);
    virtual ~DropWindow();

public slots:
    void droppedFiles(const QList<QUrl> list);

private:
    std::string _directory;
    KeyUtil *_keyUtil;
    DropArea *_dropArea;
    QPlainTextEdit *_msgBox;
    QString getLock(const bool);
};

#endif // DROPWINDOW_H
