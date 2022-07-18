#include "wplot/state/state.h"
#include "wplot/wplot.h"

State::State()
{

}
void State::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    Qt::KeyboardModifiers mask = Qt::ControlModifier | Qt::ShiftModifier;
    Qt::KeyboardModifiers maskedModifiers = event->modifiers() & mask;
    if (event->key() == Qt::Key_Z) {
        if (maskedModifiers == Qt::ControlModifier) {
            plot.zoom_Undo();
            event->accept();
        } else if (maskedModifiers == mask) {
            plot.zoom_Redo();
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }

}
