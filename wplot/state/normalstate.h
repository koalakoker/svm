#ifndef NORMALSTATE_H
#define NORMALSTATE_H

#include "state.h"
#include <QWidget>

class NormalState : public State
{
public:
    NormalState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseReleaseEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
    void mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event);
    void keyPressEvent(WPlot& plot, QKeyEvent* event);
    void keyReleaseEvent(WPlot& plot, QKeyEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
    void setCursor(WPlot& plot);

private:
    bool m_drag = false;
    bool m_movingUndo;
};

#endif // NORMALSTATE_H
