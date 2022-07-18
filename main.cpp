#include "mainwindow.h"
#include "svm.h"
#include "alphabeta.h"
#include "vll.h"
#include "vph.h"
#include "wplot/wplot.h"
#include <math.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    WPlot dutyPlot;
    WPlot vllPlot;
    WPlot vphPlot;

    float m = SQRT3/2.0f;
    int nSect = 6;
    int angleEnd = 60 * nSect;
    AlphaBeta v;
    for (int i = 0; i < angleEnd; i+=1) {
        float th = (float)(i) * M_PI/180;
        v.alpha = m * cos(th);
        v.beta = m * sin(th);
        Duty d = SVM::calculate(v);

        dutyPlot.addPoint(i,SData(d.a, d.b, d.c));

        float vdc = 100;
        Vll vll(vdc, d);
        vllPlot.addPoint(i, SData(vll.vab, vll.vbc, vll.vca));

        Vph vph(vll);
        vphPlot.addPoint(i, SData(vph.va, vph.vb, vph.vc));
    }

    dutyPlot.createPlot();
    dutyPlot.show();
    dutyPlot.setWindowTitle("Duty Cycle");
    vllPlot.createPlot();
    vllPlot.show();
    vllPlot.setWindowTitle("Voltages line to line");
    vphPlot.createPlot();
    vphPlot.show();
    vphPlot.setWindowTitle("Phase voltages");

    //w.show();

    return a.exec();
}
