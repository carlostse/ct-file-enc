#include "drop_area.h"

DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(250, 250);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        emit dropped(mimeData->urls());
    }

    event->acceptProposedAction();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}

void DropArea::clear()
{
    setText(tr("Drop the file here to encrypt / decrypt"));
}
