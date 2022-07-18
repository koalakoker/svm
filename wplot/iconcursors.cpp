#include "wplot/iconcursors.h"

#include <QDebug>

IconCursors::IconCursors()
{
}

QCursor IconCursors::get(Cursor_t type) {
    QCursor cur;
    switch (type) {
    case ZoomIn:
    {
        QPixmap cursor_pixmap = QPixmap(":/png/icons8-zoom-in-24.png");
        cur = QCursor(cursor_pixmap, 10, 10);
    }
        break;
    case ZoomOut:
    {
        QPixmap cursor_pixmap = QPixmap(":/png/icons8-zoom-out-24.png");
        cur = QCursor(cursor_pixmap, 10, 10);
    }
        break;
    default:
        break;
    }
    return cur;
};
