#ifndef STATE_H
#define STATE_H

#include <QMouseEvent>

class WPlot;

class State
{
public:
    State();
    virtual void mousePressEvent(WPlot& plot, QMouseEvent* event) = 0;
    virtual void mouseReleaseEvent(WPlot& plot, QMouseEvent* event) = 0;
    virtual void mouseMoveEvent(WPlot& plot, QMouseEvent* event) = 0;
    virtual void mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) = 0;
    virtual void keyPressEvent(WPlot& plot, QKeyEvent* event);
    virtual void keyReleaseEvent(WPlot& plot, QKeyEvent* event) = 0;

    virtual void setCursor(WPlot& plot) = 0;
};

#endif // STATE_H
