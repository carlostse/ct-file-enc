#ifndef DROPAREA_H
#define DROPAREA_H

#include <QtGui>

class DropArea : public QLabel
{
    Q_OBJECT

public:
    DropArea(QWidget *parent = 0);

public slots:
    void clear();

signals:
    void dropped(const QList<QUrl> list);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QLabel *label;
};

#endif // DROPAREA_H
