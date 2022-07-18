#ifndef ICONCURSORS_H
#define ICONCURSORS_H

#include "QtCore/qnamespace.h"
#include "QtGui/qcursor.h"

class IconCursors
{
private:
    IconCursors();
public:

    typedef enum {
        ZoomIn,
        ZoomOut
    } Cursor_t;

    static QCursor get(Cursor_t type);
};

#endif // ICONCURSORS_H
