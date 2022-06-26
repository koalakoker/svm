#ifndef MWPLOT_H
#define MWPLOT_H

#include "wcursorinfo.h"
#include "sdata.h"

#include <QMainWindow>
#include <QDebug>

namespace Ui {
class MWPlot;
}

class MWPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit MWPlot(QString name, QWidget *parent = 0);
    ~MWPlot();
    void loadDataFile(QString fileName);
    void exportData(QString filename);
    QVector<QVector<double>> getCursorValueTrack(void);
    QVector<double> getCursorValueTrack(int cur);
    QVector<double> getSelectedCursorValueTrack(void);
    void addPoint(double t, SData y);
    void updatePlot(void);

public slots:
    void onNewPlotter();
    void onCursorChange();

signals:
    void cursorChanged();

private slots:
    void on_cursorPosChanged(int cur, qreal value);
    void on_actionOpen_data_file_triggered();
    void on_actionExport_data_file_triggered();
    void on_actionZoom_Undo_triggered();
    void on_actionZoom_Redo_triggered();
    void on_actionInfo_data_triggered(bool checked);
    void on_actionBottom_toggled(bool arg1);
    void on_actionLeft_toggled(bool arg1);
    void on_actionRight_toggled(bool arg1);
    void on_actionTop_toggled(bool arg1);
    void on_actionBottom_Left_toggled(bool arg1);
    void on_actionTop_Right_toggled(bool arg1);
    void on_actionAdd_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();

    void on_actionTest_triggered();

private:
    Ui::MWPlot *ui;
    WCursorInfo *wCursorInfo;
    QString m_name;
};

#endif // MWPLOT_H
