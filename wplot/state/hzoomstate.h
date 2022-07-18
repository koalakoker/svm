#ifndef HZOOMSTATE_H
#define HZOOMSTATE_H

#include "state.h"

class HZoomState : public State
{
public:
    HZoomState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseReleaseEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
    void mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event);
    void keyPressEvent(WPlot& plot, QKeyEvent* event);
    void keyReleaseEvent(WPlot& plot, QKeyEvent* event);
    void setCursor(WPlot& plot);

private:
    bool m_drag = false;
};

#endif // HZOOMSTATE_H
