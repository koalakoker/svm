#include "plotter.h"

#include <algorithm>
#include <QFontMetrics>
#include <QElapsedTimer>
#include <QTimer>
#include <QDebug>

// Plot
QImage Plotter::plot()
{
    const Qt::GlobalColor plotColor[] = {Qt::black, Qt::blue, Qt::green, Qt::red};

    QElapsedTimer t;
    t.start();
    QImage img(m_size, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::white);
    QPainter p(&img);
    QPen pen(Qt::darkBlue);
    pen.setWidth(1);
    p.setPen(pen);

    if (m_data.size() != 0)
    {
        int trackNum = m_data[0].size();
        int sampleNum = m_data.size();

        double x0 = invMapX(0);
        double x1 = invMapX(1);
        double dX = x1-x0;
        double dY = m_data[1][0]-m_data[0][0];
        int di = dX/dY;
        if (di <= 0)
        {
            di = 1;
        }

        QVector<double> tSample;
        if (trackNum > 1)
        {
            tSample.reserve(sampleNum);
        }
        for (int track = 1; track < trackNum; track++) // Skip index 0 that is the time (x)
        {
            pen.setColor(plotColor[track-1]);
            p.setPen(pen);

            switch (m_style)
            {
            case POINT_STYLE:
                {
                    if (track == 1) /* See below the optimizaton description */
                    {
                        for (int i = 0; i < m_data.size(); i++)
                        {
                            QPointF point;
                            p.drawPoint(point = map(m_data[i][0], m_data[i][track]));
                            tSample.append(point.x());
                        }
                    }
                    else
                    {
                        for (int i = 0; i < m_data.size(); i++)
                        {
                            p.drawPoint(QPointF(tSample[i], mapY(m_data[i][track])));
                        }
                    }
                }
                break;
            case LINE_STYLE:
                {
                    if (track == 1) /* For first trac are computed t/x values and stored in temporary array tSample */
                    {
                        QPointF nextP, prevP = map(m_data[0][0], m_data[0][track]); /* To track the line the prev point is used */
                        tSample.append(prevP.x());
                        for (int i = 0; i < m_data.size() - di; i+=di)
                        {
                            p.drawLine(prevP, nextP = map(m_data[i+1][0], m_data[i+1][track]));
                            prevP = nextP;
                            tSample.append(nextP.x());
                        }

                    }
                    else /* For the t/x value are used the preprocessed values */
                    {
                        int j = 0;
                        QPointF nextP, prevP = QPointF(tSample[j], mapY(m_data[0][track]));
                        for (int i = 0; i < m_data.size() - di; i+=di)
                        {
                            p.drawLine(prevP, nextP = QPointF(tSample[j+1], mapY(m_data[i+1][track])));
                            prevP = nextP;
                            j++;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
        //p.setPen(QPen(Qt::red));
        //p.drawText(10,20,QString("Rect: Top %1, Bottom %2").arg(_range.top()).arg(_range.bottom()));
    }

    /* Cursors */
    int curLabelYPos = 30;

    pen.setColor(Qt::black);
    pen.setStyle(Qt::DashLine);
    p.setPen(pen);

    m_cursorRect.clear();
    for (int cur = 0; cur < m_cursorPos.size(); cur++)
    {
        qreal curXpos       = m_cursorPos.at(cur);
        qreal curYposTop    = m_range.y();
        qreal curYposBottom = m_range.y()+m_range.height();
        p.drawLine(map(curXpos, curYposTop), map(curXpos, curYposBottom));
        p.drawText(mapX(curXpos)+5,curLabelYPos,QString::number(cur+1));

        QPointF top     = map(curXpos, curYposTop);
        QPointF bottom  = map(curXpos, curYposBottom);
        QPoint  topLeft    (top.x()    - m_cursorMargin, top.y());
        QPoint  bottomRight(bottom.x() + m_cursorMargin, bottom.y());
        QRect   rect(topLeft, bottomRight);
        m_cursorRect.append(rect);
    }

    /* Axis */
    int hDivNum = 5;
    int vDivNum = 5;
    int divLen = 10;
    int hSpacer = 5;
    int vSpacer = 5;

    int h = m_size.height();
    int w = m_size.width();
    QFontMetrics fm(p.font());

    int hDivSpace = m_size.width()  / hDivNum;
    int vDivSpace = m_size.height() / vDivNum;

    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    p.setPen(pen);
    if (m_axsisBottom)
    {
        for (int i = 0; i < (hDivNum-1); i++)
        {
            int x = hDivSpace * (i+1);
            p.drawLine(QPoint(x, h), QPoint(x, h - divLen));
            QString valueStr = QString::number(invMapX(x),'g', 3);
            p.drawText(QPoint(x + vSpacer, h - hSpacer), valueStr);
        }
    }
    if (m_axsisTop)
    {
        for (int i = 0; i < (hDivNum-1); i++)
        {
            int x = hDivSpace * (i+1);
            p.drawLine(QPoint(x, 0), QPoint(x, divLen));
            QString valueStr = QString::number(invMapX(x),'g', 3);
            QSize sz = fm.size(Qt::TextSingleLine,valueStr);
            p.drawText(QPoint(x + vSpacer, sz.height()), valueStr);
        }
    }
    if (m_axsisLeft)
    {
        for (int i = 0; i < (vDivNum-1); i++)
        {
            int y = vDivSpace * (i+1);
            p.drawLine(QPoint(0, y), QPoint(divLen, y));
            QString valueStr = QString::number(invMapY(y),'g', 3);
            QSize sz = fm.size(Qt::TextSingleLine,valueStr);
            p.drawText(QPoint(divLen + hSpacer, y + (sz.height()/2) - hSpacer) , valueStr);
        }
    }
    if (m_axsisRight)
    {
        for (int i = 0; i < (vDivNum-1); i++)
        {
            int y = vDivSpace * (i+1);
            p.drawLine(QPoint(w, y), QPoint(w - divLen, y));
            QString valueStr = QString::number(invMapY(y),'g', 3);
            QSize sz = fm.size(Qt::TextSingleLine,valueStr);
            p.drawText(QPoint(w - divLen - hSpacer - sz.width(), y + (sz.height()/2) - hSpacer) , valueStr);
        }
    }

    //qDebug() << "plotted in" << t.elapsed() << "msec";
    return img;
}
QPointF Plotter::map(double x, double y)
{
    return QPointF(mapX(x),mapY(y));
}
double Plotter::mapX(double x)
{
    return (m_size.width () * ((x - m_range.x()) / m_range.width ()));
}
double Plotter::mapY(double y)
{
    return (m_size.height()-(m_size.height() * ((y - m_range.y()) / m_range.height())));
}
double Plotter::invMapX(double x)
{
    return (((x / m_size.width()) * m_range.width()) + m_range.x());
}
double Plotter::invMapY(double y)
{
    return (((1 - (y / m_size.height())) * m_range.height()) + m_range.y());
}

// Scroll
void Plotter::scrollX(qreal val)
{
    m_range.setLeft(m_range.left() + val);
    m_range.setRight (m_range.right()  + val);
}
void Plotter::scrollY(qreal val)
{
    m_range.setTop (m_range.top()  + val);
    m_range.setBottom(m_range.bottom() + val);
}
void Plotter::scrollXpixel(int pix)
{
    scrollX((m_range.width()  * (qreal)(pix))/(qreal)(m_size.width ()));
}
void Plotter::scrollYpixel(int pix)
{
    scrollY((m_range.height() * (qreal)(pix))/(qreal)(m_size.height()));
}

// Zoom
void Plotter::zoomX(qreal val)
{
    qreal delta = m_range.width() * val * 0.05;
    m_range.setLeft(m_range.left() + delta);
    m_range.setRight(m_range.right() - delta);
}
void Plotter::zoomY(qreal val)
{
    qreal delta = m_range.height() * val * 0.05;
    m_range.setTop   (m_range.top()    + delta);
    m_range.setBottom(m_range.bottom() - delta);
}
void Plotter::zoomXToCursors(QPoint point)
{
    qreal xMin,xMax;
    qreal xPos = invMapX(point.x());

    //QVector<qreal> xPoints;
    std::vector<qreal> xPoints;
    xPoints.push_back(xMin = m_range.left());
    xPoints.push_back(xMax = m_range.right());

    int i;
    for (i = 0; i < m_cursorPos.size(); i++)
    {
        xPoints.push_back(m_cursorPos.at(i));
    }


    std::sort(xPoints.begin(), xPoints.end());

    for (i = 0; i < xPoints.size(); i++)
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
        zoomX(5);
    }
    else
    {
        m_range.setLeft(xMin);
        m_range.setRight(xMax);
    }
}
void Plotter::unZoom(void)
{
    zoomX(-5);
}

// Cursors
void   Plotter::addCursor(void)
{
    qreal pos = (invMapX(m_size.width()) - invMapX(0))/2;
    addCursor(pos);
    emit cursorChanged();
}
void   Plotter::addCursor(qreal pos)
{
    m_cursorPos.append(pos);
    emit cursorChanged();
}
void   Plotter::addCursorAtPixel(int pos)
{
    m_cursorPos.append(invMapX(pos));
    emit cursorChanged();
}
void   Plotter::removeCursor(int index)
{
    m_cursorPos.remove(index);
    emit cursorChanged();
}
void Plotter::setCursorPos(int index, qreal pos)
{
    m_cursorPos[index] = pos;
    emit cursorChanged();
}

void   Plotter::cursorScroll(int index, qreal pos)
{
    m_cursorPos[index] += pos;
    emit cursorChanged();
}
void   Plotter::cursorScrollPixel(int index, int pix)
{
    cursorScroll(index, (m_range.width()  * (qreal)(pix))/(qreal)(m_size.width ()));
}
bool   Plotter::onCursor(QPoint point, int& selectedCursor, bool startDrag)
{
    bool retVal = false;
    for (int cursor = 0; cursor < m_cursorRect.size(); cursor++)
    {
        if (m_cursorRect.at(cursor).contains(point))
        {
            retVal = true;
            selectedCursor = cursor;
            if (startDrag)
            {
                dragCursor(cursor);
            }
        }
    }
    return retVal;
}
void   Plotter::dragCursor(int index)
{
    if ((index >= 0) && (index < m_cursorPos.size()))
    {
        m_cursorDrag = index + 1; // 0 none, index + 1 (zero based) if cursor index is dragged
    }
}
void   Plotter::releaseCursor()
{
    m_cursorDrag = 0;
}
int    Plotter::getCursorDragged()
{
    return m_cursorDrag;
}
QVector<QVector<double>> Plotter::getCursorValueTrack(void)
{
    QVector<QVector<double>> cursorInfo;
    for (int cur = 0; cur < m_cursorPos.size(); cur++)
    {
        cursorInfo.append(getCursorValueTrack(cur));
    }
    return cursorInfo;
}
QVector<double> Plotter::getCursorValueTrack(int cur)
{
    QVector<double> data;
    qreal x = m_cursorPos.at(cur);
    qreal dx = m_data[1][0] - m_data[0][0];
    int i = (int)(x/dx);
    for (int track = 0; track < m_data[0].size(); track++)
    {
        if ((i >= 0) && (i < m_data.size()))
        {
            data.append(m_data[i][track]);
        }
        else
        {
            data.append(0);
        }
    }
    return data;
}
QVector<double> Plotter::getSelectedCursorValueTrack(void)
{
    return getCursorValueTrack(m_cursorDrag);
}

// Undo
void Plotter::Undo(void)
{
    if (!m_undoRangeHystory.isEmpty())
    {
        m_redoRangeHystory.append(m_range);
        m_range = m_undoRangeHystory.takeLast();
    }
}
void Plotter::Redo(void)
{
    if (!m_redoRangeHystory.isEmpty())
    {
        m_undoRangeHystory.append(m_range);
        m_range = m_redoRangeHystory.takeLast();
    }
}
void Plotter::AddUndoStatus(void)
{
    if (!m_debounce)
    {
        m_undoRangeHystory.append(m_range);
        m_redoRangeHystory.clear();
        QTimer::singleShot(500, this, SLOT(endTimer()));
        m_debounce = true;
    }
}
void Plotter::endTimer()
{
    m_debounce = false;
}
