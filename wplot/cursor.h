#ifndef CURSOR_H
#define CURSOR_H

#include <QPainter>
#include <QPen>

class Plotter;

class Cursor : public QObject {
    Q_OBJECT

public:
    QVector<qreal> m_pos;

    Cursor(Plotter* plotter);
    void plot(QPainter& p, QPen& pen);
    int getNumber(void) {return m_pos.size();}
    void add(void);
    void add(qreal pos);
    void addAtPixel(int pos);
    void remove(int index);
    void setPos(int index, qreal pos);
    void scroll(int index, qreal pos);
    void scrollPixel(int index, int pix);
    bool on(QPoint point, int& selectedCursor, bool startDrag = false);
    void drag(int index);
    void release();
    int getDragged();
    QVector<QVector<double>> getValueTrack(void);
    QVector<double> getValueTrack(int cur);
    QVector<double> getSelectedValueTrack(void);

private:
    Plotter* plotter;
    void plotValuesNear(QPainter &p, int cur);

    QVector<QRect> m_rect;
    int m_drag; // 0 none, index + 1 (zero based) if cursor index is dragged
    const int m_margin = 5;

signals:
    void changed();

};

#endif // CURSOR_H
