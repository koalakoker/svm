#include "cursor.h"
#include "plotter.h"
#include <QVector>

Cursor::Cursor(Plotter *plotter): plotter(plotter), m_drag(0)
{

}

void Cursor::plot(QPainter& p, QPen& pen) {
    /* Cursors */
    int curLabelYPos = 30;
    QRectF m_range = plotter->axis->m_range;
    m_rect.clear();
    for (int cur = 0; cur < m_pos.size(); cur++)
    {
        qreal curXpos       = m_pos.at(cur);
        qreal curYposTop    = m_range.y();
        qreal curYposBottom = m_range.y()+m_range.height();

        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);

        p.drawLine(plotter->map(curXpos, curYposTop), plotter->map(curXpos, curYposBottom));
        p.drawText(plotter->mapX(curXpos)+5,curLabelYPos,QString::number(cur+1) + "@" + QString::number(curXpos));

        QPointF top     = plotter->map(curXpos, curYposTop);
        QPointF bottom  = plotter->map(curXpos, curYposBottom);
        QPoint  topLeft    (top.x()    - m_margin, top.y());
        QPoint  bottomRight(bottom.x() + m_margin, bottom.y());
        QRect   rect(topLeft, bottomRight);
        m_rect.append(rect);

        this->plotValuesNear(p, cur);
    }
}

void Cursor::plotValuesNear(QPainter& p, int cur) {
    QPen pen;
    QVector<double> values = getValueTrack(cur);
    QVectorIterator<double> it(values);
    qreal curXpos = it.next();
    int i = 0;
    while (it.hasNext()) {
        double yValue = it.next();
        pen.setColor(plotColor[i]);
        pen.setStyle(Qt::SolidLine);
        p.setPen(pen);
        p.drawText(plotter->map(curXpos, yValue) + QPointF(5,0), QString::number(yValue));
        i++;
    }
}

void   Cursor::add(void)
{
    qreal pos = (plotter->invMapX(plotter->m_size.width()) - plotter->invMapX(0))/2;
    add(pos);
    emit changed();
}
void   Cursor::add(qreal pos)
{
    m_pos.append(pos);
    emit changed();
}
void   Cursor::addAtPixel(int pos)
{
    m_pos.append(plotter->invMapX(pos));
    emit changed();
}
void   Cursor::remove(int index)
{
    m_pos.remove(index);
    emit changed();
}
void Cursor::setPos(int index, qreal pos)
{
    m_pos[index] = pos;
    emit changed();
}

void   Cursor::scroll(int index, qreal pos)
{
    m_pos[index] += pos;
    emit changed();
}
void   Cursor::scrollPixel(int index, int pix)
{
    QRectF m_range = plotter->axis->m_range;
    scroll(index, (m_range.width()  * (qreal)(pix))/(qreal)(plotter->m_size.width ()));
}
bool   Cursor::on(QPoint point, int& selectedCursor, bool startDrag)
{
    bool retVal = false;
    for (int cursor = 0; cursor < m_rect.size(); cursor++)
    {
        if (m_rect.at(cursor).contains(point))
        {
            retVal = true;
            selectedCursor = cursor;
            if (startDrag)
            {
                drag(cursor);
            }
        }
    }
    return retVal;
}
void   Cursor::drag(int index)
{
    if ((index >= 0) && (index < m_pos.size()))
    {
        m_drag = index + 1; // 0 none, index + 1 (zero based) if cursor index is dragged
    }
}
void   Cursor::release()
{
    m_drag = 0;
}
int    Cursor::getDragged()
{
    return m_drag;
}
QVector<QVector<double>> Cursor::getValueTrack(void)
{
    QVector<QVector<double>> cursorInfo;
    for (int cur = 0; cur < m_pos.size(); cur++)
    {
        cursorInfo.append(getValueTrack(cur));
    }
    return cursorInfo;
}
QVector<double> Cursor::getValueTrack(int cur)
{
    QVector<double> data;
    qreal x = m_pos.at(cur);
    qreal dx = plotter->curve->m_data[1][0] - plotter->curve->m_data[0][0];
    int i = (int)((x-plotter->curve->m_data[0][0])/dx);
    for (int track = 0; track < plotter->curve->m_data[0].size(); track++)
    {
        if ((i >= 0) && (i < plotter->curve->m_data.size()))
        {
            data.append(plotter->curve->m_data[i][track]);
        }
        else
        {
            data.append(0);
        }
    }
    return data;
}
QVector<double> Cursor::getSelectedValueTrack(void)
{
    return getValueTrack(m_drag);
}
