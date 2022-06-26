#ifndef PLOTTER_H
#define PLOTTER_H

#include <QObject>
#include <QPainter>

#include "sdata.h"

class Plotter : public QObject {
    Q_OBJECT

public:
    typedef enum {
        POINT_STYLE,
        LINE_STYLE
    } style_t;

    Plotter(QSize size, QRectF range, QVector<SData> data, style_t style = LINE_STYLE)
        : m_size(size), m_range(range), m_data(data), m_style(style), m_cursorDrag(0),
          m_debounce(false) { }

    void setRangeX_Min(qreal val) {m_range.setLeft  (val);}
    void setRangeX_Max(qreal val) {m_range.setRight (val);}
    void setRangeY_Min(qreal val) {m_range.setBottom(val);}
    void setRangeY_Max(qreal val) {m_range.setTop   (val);}

    // Scroll
    void scrollX(qreal val);
    void scrollY(qreal val);
    void scrollXpixel(int pix);
    void scrollYpixel(int pix);

    // Zoom
    void zoomX(qreal val);
    void zoomY(qreal val);
    void zoomXToCursors(QPoint point);
    void unZoom(void);

    // Resize
    void setSize(QSize size) {m_size = size;}

    // Cursors
    int getCursorNumber(void) {return m_cursorPos.size();}
    void addCursor(void);
    void addCursor(qreal pos);
    void addCursorAtPixel(int pos);
    void removeCursor(int index);
    void setCursorPos(int index, qreal pos);
    void cursorScroll(int index, qreal pos);
    void cursorScrollPixel(int index, int pix);
    bool onCursor(QPoint point, int& selectedCursor, bool startDrag = false);
    void dragCursor(int index);
    void releaseCursor();
    int getCursorDragged();
    QVector<QVector<double>> getCursorValueTrack(void);
    QVector<double> getCursorValueTrack(int cur);
    QVector<double> getSelectedCursorValueTrack(void);

    // Axis
    bool m_axsisBottom = false;
    bool m_axsisLeft = false;
    bool m_axsisTop = false;
    bool m_axsisRight = false;

    // Undo-Redo
    void Undo(void);
    void Redo(void);
    void AddUndoStatus(void);

signals:
    void done(QImage);
    void cleanup();
    void cursorChanged();

public slots:
    QImage plot();
    void endTimer();

private:
    QPointF map(double x, double y);
    double invMapX(double x);
    double invMapY(double y);
    double mapX(double x);
    double mapY(double y);

    QSize m_size;
    QRectF m_range;
    QVector<SData> m_data;

    style_t m_style;

    // Cursors
    QVector<qreal> m_cursorPos;
    QVector<QRect> m_cursorRect;
    int m_cursorDrag; // 0 none, index + 1 (zero based) if cursor index is dragged
    const int m_cursorMargin = 5;

    // Undo-Redo
    QVector<QRectF> m_undoRangeHystory;
    QVector<QRectF> m_redoRangeHystory;
    bool m_debounce;
};

#endif // PLOTTER_H
