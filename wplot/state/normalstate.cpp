#include "wplot/state/normalstate.h"
#include "wplot/wplot.h"
#include "wplot/cursor.h"

NormalState::NormalState()
{

}
void NormalState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
    {
        int selected = 0;
        plot.m_lastPoint = event->pos();
        if (plot.m_plotter->cursor->on(plot.m_lastPoint, selected, true))
        {

        }
        else
        {
            m_movingUndo = true;
            m_drag = true;
        }

    }
}
void NormalState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
    {
        if (m_drag)
        {
            m_drag = false;
        }
        if (plot.m_plotter->cursor->getDragged() != 0)
        {
            plot.m_plotter->cursor->release();
        }
    }
}
void NormalState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {
    if (m_drag)
    {
        if (m_movingUndo) // Add undo to be done only once
        {
            plot.m_plotter->axis->zoom->addUndoStatus();
            m_movingUndo = false;
        }

        QPoint delta = plot.m_lastPoint - event->pos();
        plot.m_lastPoint = event->pos();
        plot.m_plotter->axis->scrollXpixel( delta.x());
        plot.m_plotter->axis->scrollYpixel(-delta.y());
        plot.updatePlot();
    }

    int index = plot.m_plotter->cursor->getDragged();
    if (index != 0)
    {
        QPoint delta =  event->pos() - plot.m_lastPoint;
        plot.m_lastPoint = event->pos();
        plot.m_plotter->cursor->scrollPixel(index-1, delta.x());
        plot.updatePlot();
    }

    int selected = 0;
    if (plot.m_plotter->cursor->on(event->pos(), selected, false))
    {
        plot.setCursor(Qt::SizeHorCursor);
    }
    else
    {
        plot.setCursor(Qt::ArrowCursor);
    }
}
void NormalState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {
    // Zoom in
    plot.m_plotter->axis->zoom->addUndoStatus();
    plot.m_plotter->axis->zoom->xToCursors(event->pos());
    plot.updatePlot();
}
void NormalState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    State::keyPressEvent(plot, event);
}
void NormalState::setCursor(WPlot& plot) {
    plot.setCursor(Qt::ArrowCursor);
}
