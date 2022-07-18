#ifndef ZOOM_H
#define ZOOM_H

class Axis;

#include <QPainter>
#include <QPen>

class Zoom : public QObject
{
    Q_OBJECT

private:
    Axis* axis;

public:
    Zoom(Axis* axis);
    void x(qreal val);
    void y(qreal val);
    void xToCursors(QPoint point);
    void xToZoomRange(void);
    void yToZoomRange(void);
    void xToFullRange(void);
    void yToFullRange(void);
    void goBackward(void);
    void startXTrack(QPoint point);
    void trackScrollPixelX(int pix);
    void startYTrack(QPoint point);
    void trackScrollPixelY(int pix);

    // Undo-Redo
    void undo(void);
    void redo(void);
    void addUndoStatus(void);

    void plotTracks(QPainter& p, QPen& pen);

    // Zoom tracks
    bool xTrackVisible[2] = {false, false};
    qreal xTracksPos[2] = {0.0, 0.0};
    bool yTrackVisible[2] = {false, false};
    qreal yTracksPos[2] = {0.0, 0.0};

    // Undo-Redo
    QVector<QRectF> m_undoRangeHystory;
    QVector<QRectF> m_redoRangeHystory;
    bool m_debounce;

private slots:
    void endTimer();
};

#endif // ZOOM_H
