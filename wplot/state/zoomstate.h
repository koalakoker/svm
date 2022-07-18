#ifndef ZOOMSTATE_H
#define ZOOMSTATE_H

#include "state.h"

class ZoomState : public State
{
public:
    ZoomState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseReleaseEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
    void mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event);
    void keyPressEvent(WPlot& plot, QKeyEvent* event);
    void keyReleaseEvent(WPlot& plot, QKeyEvent* event);
    void setCursor(WPlot& plot);

private:
    bool m_shiftKeyPressed = false;;
};

#endif // ZOOMSTATE_H
