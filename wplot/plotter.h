#ifndef PLOTTER_H
#define PLOTTER_H

#include <QObject>
#include <QPainter>

#include "axis.h"
#include "cursor.h"
#include "curve.h"

const Qt::GlobalColor plotColor[] = {Qt::black, Qt::blue, Qt::green, Qt::red, Qt::yellow, Qt::gray};

class Plotter : public QObject {
    Q_OBJECT

public:
    Plotter(QSize size, QRectF range, Curve::style_t style);
    Curve* curve;
    Axis* axis;
    Cursor* cursor;

    QPointF map(double x, double y);
    double invMapX(double x);
    double invMapY(double y);
    double mapX(double x);
    double mapY(double y);

    QSize m_size;
    void setSize(QSize size) {m_size = size;}

signals:
    void done(QImage);
    void cleanup();

public slots:
    QImage plot();

private:

};

#endif // PLOTTER_H
