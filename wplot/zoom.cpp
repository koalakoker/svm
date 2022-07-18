#include "zoom.h"
#include "axis.h"
#include "plotter.h"

#include <QTimer>

Zoom::Zoom(Axis *axis): axis(axis), m_debounce(false) {}

void Zoom::plotTracks(QPainter& p, QPen& pen) {
    QRectF m_range = axis->m_range;
    for (int i = 0; i < 2; i++) {
        if (this->xTrackVisible[i]) {
            qreal curXpos       = this->xTracksPos[i];
            qreal curYposTop    = m_range.y();
            qreal curYposBottom = m_range.y() + m_range.height();

            pen.setColor(Qt::black);
            p.setPen(pen);

            p.drawLine(axis->plotter->map(curXpos, curYposTop), axis->plotter->map(curXpos, curYposBottom));
        }
    }
    for (int i = 0; i < 2; i++) {
        if (this->yTrackVisible[i]) {
            qreal curYpos       = this->yTracksPos[i];
            qreal curXposLeft    = m_range.x();
            qreal curXposRigth = m_range.x()+ m_range.width();

            pen.setColor(Qt::black);
            p.setPen(pen);

            p.drawLine(axis->plotter->map(curXposLeft, curYpos), axis->plotter->map(curXposRigth, curYpos));
        }
    }
}

void Zoom::x(qreal val)
{
    QRectF m_range = axis->m_range;
    qreal delta = m_range.width() * val * 0.05;
    axis->m_range.setLeft(m_range.left() + delta);
    axis->m_range.setRight(m_range.right() - delta);
}
void Zoom::y(qreal val)
{
    QRectF m_range = axis->m_range;
    qreal delta = m_range.height() * val * 0.05;
    axis->m_range.setTop   (m_range.top()    + delta);
    axis->m_range.setBottom(m_range.bottom() - delta);
}
void Zoom::xToCursors(QPoint point)
{
    QRectF m_range = axis->m_range;
    qreal xMin,xMax;
    qreal xPos = axis->plotter->invMapX(point.x());

    //QVector<qreal> xPoints;
    std::vector<qreal> xPoints;
    xPoints.push_back(xMin = m_range.left());
    xPoints.push_back(xMax = m_range.right());

    int i;
    for (i = 0; i < axis->plotter->cursor->m_pos.size(); i++)
    {
        xPoints.push_back(axis->plotter->cursor->m_pos.at(i));
    }


    std::sort(xPoints.begin(), xPoints.end());

    for (i = 0; i < (int)(xPoints.size()); i++)
    {
        if (xPos < xPoints.at(i))
        {
            xMin = xPoints.at(i-1);
            xMax = xPoints.at(i);
            break;
        }
    }

    if ((m_range.left() == xMin) && (m_range.right() == xMax))
    {
        x(5);
    }
    else
    {
        axis->m_range.setLeft(xMin);
        axis->m_range.setRight(xMax);
    }
}
void Zoom::xToZoomRange() {
    qreal left  = this->xTracksPos[0];
    qreal rigth = this->xTracksPos[1];

    for (int i = 0; i < 2;  i++) {
        this->xTrackVisible[i] = false;
    }

    if (left == rigth) return;
    if (left > rigth) {
        qreal tmp = left;
        left = rigth;
        rigth = tmp;
    }
    axis->m_range.setLeft(left);
    axis->m_range.setRight(rigth);
}
void Zoom::yToZoomRange(void) {
    qreal top    = this->yTracksPos[0];
    qreal bottom = this->yTracksPos[1];

    for (int i = 0; i < 2;  i++) {
        this->yTrackVisible[i] = false;
    }

    if (top == bottom) return;
    if (top > bottom) {
        qreal tmp = top;
        top = bottom;
        bottom = tmp;
    }
    axis->m_range.setTop(top);
    axis->m_range.setBottom(bottom);
}
void Zoom::xToFullRange(void) {
    axis->m_range.setLeft(axis->m_x_min);
    axis->m_range.setRight(axis->m_x_max);
}
void Zoom::yToFullRange(void) {
    axis->m_range.setBottom(axis->m_y_min);
    axis->m_range.setTop(axis->m_y_max);
}
void Zoom::goBackward(void)
{
    x(-5);
}
void Zoom::startXTrack(QPoint point) {
    for (int i = 0;  i < 2; i++) {
        this->xTrackVisible[i] = true;
        this->xTracksPos[i] = axis->plotter->invMapX(point.x());
    }
}
void Zoom::trackScrollPixelX(int pix) {
    QRectF m_range = axis->m_range;
    this->xTracksPos[1] += (m_range.width()  * (qreal)(pix))/(qreal)(axis->plotter->m_size.width ());
}
void Zoom::startYTrack(QPoint point) {
    for (int i = 0;  i < 2; i++) {
        this->yTrackVisible[i] = true;
        this->yTracksPos[i] = axis->plotter->invMapY(point.y());
    }
}
void Zoom::trackScrollPixelY(int pix) {
    QRectF m_range = axis->m_range;
    this->yTracksPos[1] -= (m_range.height()  * (qreal)(pix))/(qreal)(axis->plotter->m_size.height());
}

// Undo
void Zoom::undo(void)
{
    QRectF m_range = axis->m_range;
    if (!m_undoRangeHystory.isEmpty())
    {
        m_redoRangeHystory.append(m_range);
        axis->m_range = m_undoRangeHystory.takeLast();
    }
}
void Zoom::redo(void)
{
    QRectF m_range = axis->m_range;
    if (!m_redoRangeHystory.isEmpty())
    {
        m_undoRangeHystory.append(m_range);
        axis->m_range = m_redoRangeHystory.takeLast();
    }
}
void Zoom::addUndoStatus(void)
{
    QRectF m_range = axis->m_range;
    if (!m_debounce)
    {
        m_undoRangeHystory.append(m_range);
        m_redoRangeHystory.clear();
        QTimer::singleShot(500, this, SLOT(endTimer()));
        m_debounce = true;
    }
}
void Zoom::endTimer()
{
    m_debounce = false;
}

