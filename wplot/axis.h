#ifndef AXIS_H
#define AXIS_H

#include "zoom.h"

#include <QPainter>
#include <QPen>

class Plotter;

class Axis
{
public:
    Axis(Plotter* plotter, QRectF range);
    Plotter* plotter;
    Zoom* zoom;

    void plot(QPainter& p, QPen& pen);
    void setRangeX_Min(qreal val) {m_range.setLeft  (val);}
    void setRangeX_Max(qreal val) {m_range.setRight (val);}
    void setRangeY_Min(qreal val) {m_range.setBottom(val);}
    void setRangeY_Max(qreal val) {m_range.setTop   (val);}

    void scrollX(qreal val);
    void scrollY(qreal val);
    void scrollXpixel(int pix);
    void scrollYpixel(int pix);

    bool m_showBottom = false;
    bool m_showLeft = false;
    bool m_showTop = false;
    bool m_showRight = false;

    QRectF m_range;
    double m_x_max = 0, m_x_min = 0;
    double m_y_max = 0, m_y_min = 0;
    QPointF m_div;
    bool m_divVisible[2] = {true, true};
};

#endif // AXIS_H
