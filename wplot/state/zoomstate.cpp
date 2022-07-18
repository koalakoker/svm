#include "wplot/state/zoomstate.h"
#include "wplot/wplot.h"
#include "wplot/iconcursors.h"

ZoomState::ZoomState()
{

}
void ZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (this->m_shiftKeyPressed)  {
            // Zoom Out
            plot.m_plotter->axis->zoom->addUndoStatus();
            plot.m_plotter->axis->zoom->goBackward();
            plot.updatePlot();

        } else {
            // Zoom in
            plot.m_plotter->axis->zoom->addUndoStatus();
            plot.m_plotter->axis->zoom->xToCursors(event->pos());
            plot.updatePlot();
        }
    }
}
void ZoomState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
void ZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
void ZoomState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {
    if (this->m_shiftKeyPressed)  {
        // Zoom Out
        plot.m_plotter->axis->zoom->addUndoStatus();
        plot.m_plotter->axis->zoom->goBackward();
        plot.updatePlot();

    } else {
        // Zoom in
        plot.m_plotter->axis->zoom->addUndoStatus();
        plot.m_plotter->axis->zoom->xToCursors(event->pos());
        plot.updatePlot();
    }
}
void ZoomState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        plot.state = &plot.normalState;
        plot.state->setCursor(plot);
        event->accept();
    } else if (event->key() == Qt::Key_Shift) {
        this->m_shiftKeyPressed = true;
        plot.setCursor(IconCursors::get(IconCursors::ZoomOut));
    } else {
        State::keyPressEvent(plot, event);
    }
}
void ZoomState::keyReleaseEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Shift) {
        this->m_shiftKeyPressed = false;
        plot.setCursor(IconCursors::get(IconCursors::ZoomIn));
    }
}
void ZoomState::setCursor(WPlot& plot) {
    plot.setCursor(IconCursors::get(IconCursors::ZoomIn));
}
