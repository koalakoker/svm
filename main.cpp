#include "mainwindow.h"
#include "mwplot.h"
#include "svm.h"
#include "alphabeta.h"
#include "vll.h"
#include "vph.h"
#include <math.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    MWPlot* dutyPlot = new MWPlot("Duty");
    MWPlot* vllPlot = new MWPlot("Vll");
    MWPlot* vphPlot = new MWPlot("Vph");

    float m = SQRT3/2.0f;
    int nSect = 6;
    int angleEnd = 60 * nSect;
    AlphaBeta v;
    for (int i = 0; i < angleEnd; i+=1) {
        float th = (float)(i) * M_PI/180;
        v.alpha = m * cos(th);
        v.beta = m * sin(th);
        Duty d = SVM::calculate(v);

        dutyPlot->addPoint(i,SData(d.a, d.b, d.c));

        float vdc = 100;
        Vll vll(vdc, d);
        vllPlot->addPoint(i, SData(vll.vab, vll.vbc, vll.vca));

        Vph vph(vll);
        vphPlot->addPoint(i, SData(vph.va, vph.vb, vph.vc));
    }

    dutyPlot->show();
    dutyPlot->updatePlot();
    dutyPlot->exportData("duty.txt");

    vllPlot->show();
    vllPlot->updatePlot();
    vllPlot->exportData("vll.txt");

    vphPlot->show();
    vphPlot->updatePlot();
    vphPlot->exportData("vph.txt");

    return a.exec();
}
