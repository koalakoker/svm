#include "axis.h"
#include "plotter.h"

#include <QDebug>

Axis::Axis(Plotter *plotter, QRectF range) : plotter(plotter) {
    float defDiv = 5.0;
    m_range = range;
    m_x_min = range.left();
    m_x_max = range.right();
    m_y_min = range.bottom();
    m_y_max = range.top();
    m_div.setX((range.right()-range.left())/defDiv);
    m_div.setY((range.bottom()-range.top())/defDiv);
    zoom = new Zoom(this);
}

void Axis::plot(QPainter& p, QPen& pen) {
    int nDiv, firstDiv;
    int divLen = 10;
    int hSpacer = 5;
    int vSpacer = 5;
    int w = plotter->m_size.width();
    int h = plotter->m_size.height();

    // Warning: Top and Bottom are swapped according the common sense
    if ((m_divVisible[1] && (m_div.y() != 0.0f))) {
        nDiv = (int)((m_range.bottom() - m_range.top()) / m_div.y());
        firstDiv = (int)(m_range.top()/m_div.y());
        if (firstDiv > 0) {
            firstDiv++;
        }
        if (firstDiv < 0) {
            firstDiv--;
        }
        for (int div = firstDiv; div <= firstDiv + nDiv; div++) {

            if (div == 0) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
            } else {
                pen.setColor(Qt::lightGray);
                pen.setStyle(Qt::DotLine);
            }
            p.setPen(pen);

            qreal Ypos      = m_div.y() * div;
            qreal XposLeft  = m_range.x();
            qreal XposRigth = m_range.x() + m_range.width();
            p.drawLine(plotter->map(XposLeft, Ypos), plotter->map(XposRigth, Ypos));

            if (m_showLeft) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);
                QFontMetrics fm(p.font());

                QPointF point = plotter->map(XposLeft, Ypos);
                int y = (int)(point.y());
                p.drawLine(QPoint(0, y), QPoint(divLen, y));
                QString valueStr = QString::number(Ypos,'g', 3);
                QSize sz = fm.size(Qt::TextSingleLine,valueStr);
                p.drawText(QPoint(divLen + hSpacer, y + (sz.height()/2) - hSpacer) , valueStr);
            }
            if (m_showRight) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);
                QFontMetrics fm(p.font());

                QPointF point = plotter->map(XposRigth, Ypos);
                int y = (int)(point.y());
                p.drawLine(QPoint(w, y), QPoint(w - divLen, y));
                QString valueStr = QString::number(Ypos,'g', 3);
                QSize sz = fm.size(Qt::TextSingleLine,valueStr);
                p.drawText(QPoint(w - divLen - hSpacer - sz.width(), y + (sz.height()/2) - hSpacer) , valueStr);
            }
        }
    }
    if ((m_divVisible[0]) && (m_div.x() != 0.0f)) {
        nDiv = (int)((m_range.right() - m_range.left()) / m_div.x());
        firstDiv = (int)(m_range.left()/m_div.x());
        if (firstDiv > 0) {
            firstDiv++;
        }
        if (firstDiv < 0) {
            firstDiv--;
        }
        for (int div = firstDiv; div <= firstDiv + nDiv; div++) {

            if (div == 0) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
            } else {
                pen.setColor(Qt::lightGray);
                pen.setStyle(Qt::DotLine);
            }
            p.setPen(pen);

            qreal Xpos       = m_div.x() * div;
            qreal YposTop    = m_range.y();
            qreal YposBottom = m_range.y() + m_range.height();
            p.drawLine(plotter->map(Xpos, YposTop), plotter->map(Xpos, YposBottom));

            if (m_showBottom) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);

                QPointF bottomPoint = plotter->map(Xpos, YposBottom);
                int x = (int)(bottomPoint.x());
                p.drawLine(QPoint(x, h), QPoint(x, h - divLen));
                QString valueStr = QString::number(Xpos,'g', 3);
                p.drawText(QPoint(x + vSpacer, h - hSpacer), valueStr);
            }

            if (m_showTop) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);
                QFontMetrics fm(p.font());

                QPointF topPoint = plotter->map(Xpos, YposTop);
                int x = (int)(topPoint.x());
                p.drawLine(QPoint(x, 0), QPoint(x, divLen));
                QString valueStr = QString::number(Xpos,'g', 3);
                QSize sz = fm.size(Qt::TextSingleLine,valueStr);
                p.drawText(QPoint(x + vSpacer, sz.height()), valueStr);
            }
        }
    }

    zoom->plotTracks(p, pen);
}

void Axis::scrollX(qreal val)
{
    m_range.setLeft(m_range.left() + val);
    m_range.setRight (m_range.right()  + val);
}
void Axis::scrollY(qreal val)
{
    m_range.setTop (m_range.top()  + val);
    m_range.setBottom(m_range.bottom() + val);
}
void Axis::scrollXpixel(int pix)
{
    scrollX((m_range.width()  * (qreal)(pix))/(qreal)(plotter->m_size.width ()));
}
void Axis::scrollYpixel(int pix)
{
    scrollY((m_range.height() * (qreal)(pix))/(qreal)(plotter->m_size.height()));
}
