#ifndef WCURSORINFO_H
#define WCURSORINFO_H

#include <QWidget>

namespace Ui {
class WCursorInfo;
}

class WCursorInfo : public QWidget
{
    Q_OBJECT

public:
    explicit WCursorInfo(QWidget *parent = 0);
    ~WCursorInfo();

public slots:
    void updateInfo(QVector<QVector<double>> cursorInfo);

signals:
    void cursorPosChanged(int cur, qreal value);

private slots:
    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::WCursorInfo *ui;
};

#endif // WCURSORINFO_H
