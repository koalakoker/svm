#include "wplot/plotter.h"

#include <algorithm>
#include <QFontMetrics>
#include <QElapsedTimer>

#include <QDebug>

Plotter::Plotter(QSize size, QRectF range, Curve::style_t style)
    : m_size(size) {
    axis = new Axis(this, range);
    curve = new Curve(this, style);
    cursor = new Cursor(this);
}

QImage Plotter::plot()
{
    QImage img(m_size, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::white);
    QPainter p(&img);
    QPen pen(Qt::darkBlue);
    pen.setWidth(1);
    p.setPen(pen);

    curve->plot(p, pen);
    cursor->plot(p, pen);
    axis->plot(p, pen);

    return img;
}

QPointF Plotter::map(double x, double y)
{
    return QPointF(mapX(x),mapY(y));
}
double Plotter::mapX(double x)
{
    QRectF m_range = axis->m_range;
    return (m_size.width () * ((x - m_range.x()) / m_range.width ()));
}
double Plotter::mapY(double y)
{
    QRectF m_range = axis->m_range;
    return (m_size.height()-(m_size.height() * ((y - m_range.y()) / m_range.height())));
}
double Plotter::invMapX(double x)
{
    QRectF m_range = axis->m_range;
    return (((x / m_size.width()) * m_range.width()) + m_range.x());
}
double Plotter::invMapY(double y)
{
    QRectF m_range = axis->m_range;
    return (((1 - (y / m_size.height())) * m_range.height()) + m_range.y());
}
