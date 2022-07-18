#ifndef CURVE_H
#define CURVE_H

#include "wplot/sdata.h"

#include <QPainter>
#include <QPen>

class Plotter;

class Curve
{
private:
    Plotter* plotter;

public:
    typedef enum {
        POINT_STYLE,
        LINE_STYLE
    } style_t;

    Curve(Plotter* plotter, style_t style);
    void plot(QPainter& p, QPen& pen);
    QRectF getFullExtent(void);
    QVector<SData> m_data;
    style_t m_style;
};

#endif // CURVE_H
